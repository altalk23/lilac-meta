#ifndef _LILAC_META_X86_OPTCALL_HPP_
#define _LILAC_META_X86_OPTCALL_HPP_

#include "function.hpp"

namespace lilac::meta::x86 {
    template<class Ret, class... Args>
    class Optcall : public CallConv<Args...> {
    private:
        using MyConv = CallConv<Args...>;
        using MyTuple = Tuple<Args...>;
        
        template<class Class>
        struct ret_constructed {
            static constexpr bool value = std::is_class_v<Class>; 
        };

        template<class Class>
        struct gpr_passable {
            static constexpr bool value = 
                any_of<
                    std::remove_cv_t<Class>,
                    bool, signed char, signed short, signed int, signed long,
                    unsigned char, unsigned short, unsigned int, unsigned long,
                    char, short, int, long
                > || 
                std::is_pointer_v<Class> || 
                std::is_reference_v<Class>;
        };

        template<class Class>
        struct sse_passable {
            static constexpr bool value = 
                any_of<
                    std::remove_cv_t<Class>,
                    float, double
                >;
        };

        template <class Ret>
        static constexpr size_t ret_fix = 
            (ret_constructed<Ret>::value) ?
                ((sizeof(Ret) % sizeof(void*) == 0) ? 
                sizeof(Ret) : 
                sizeof(Ret) - (sizeof(Ret) % sizeof(void*)) + sizeof(void*)) : 0;
        
        template<class... Stack>
        static constexpr size_t stack_fix = 
            (((sizeof(Stack) % sizeof(void*) == 0) ?
                sizeof(Stack) :
                sizeof(Stack) - (sizeof(Stack) % sizeof(void*)) + sizeof(void*)) + ...);

        template<>
        static constexpr size_t stack_fix<> = 0;

    public:
        template<size_t i, class Current>
        struct filter {
            static constexpr bool value = 
                (!gpr_passable<Current>::value || i > 1) &&
                (!sse_passable<Current>::value || i > 3);
        };
        
        template<size_t... indices>
        static decltype(auto) invoke(
            Ret(*addr)(Args...),
            const Tuple<Args...>&& all,
            const std::index_sequence<indices...>&&
        ) {
            Ret(__vectorcall* raw)(
                typename MyConv::template type_if<0, sse_passable, float>,
                typename MyConv::template type_if<1, sse_passable, float>,
                typename MyConv::template type_if<2, sse_passable, float>,
                typename MyConv::template type_if<3, sse_passable, float>,
                float,
                float,
                typename MyConv::template type_if<0, gpr_passable, int>,
                typename MyConv::template type_if<1, gpr_passable, int>,
                typename MyTuple::template type_at<indices>...
            ) = reinterpret_cast<decltype(raw)>(addr);

            if constexpr (!std::is_same_v<Ret, void>) {
                Ret ret = raw(
                    MyConv::template value_if<0, sse_passable>(all, 1907.0f),
                    MyConv::template value_if<1, sse_passable>(all, 1907.0f),
                    MyConv::template value_if<2, sse_passable>(all, 1907.0f),
                    MyConv::template value_if<3, sse_passable>(all, 1907.0f),
                    1907.0f,
                    1907.0f,
                    MyConv::template value_if<0, gpr_passable>(all, 1907),
                    MyConv::template value_if<1, gpr_passable>(all, 1907),
                    all.template at<indices>()...
                );

                static constexpr size_t fix = ret_fix<Ret> + stack_fix<typename MyTuple::template type_at<indices>...>;
                if constexpr(fix != 0) {
                    __asm add esp, [fix]
                }

                return ret;
            }
            else {
                raw(
                    MyConv::template value_if<0, sse_passable>(all, 1907.0f),
                    MyConv::template value_if<1, sse_passable>(all, 1907.0f),
                    MyConv::template value_if<2, sse_passable>(all, 1907.0f),
                    MyConv::template value_if<3, sse_passable>(all, 1907.0f),
                    1907.0f,
                    1907.0f,
                    MyConv::template value_if<0, gpr_passable>(all, 1907),
                    MyConv::template value_if<1, gpr_passable>(all, 1907),
                    all.template at<indices>()...
                );

                static constexpr size_t fix = stack_fix<typename MyTuple::template type_at<indices>...>;
                if constexpr(fix != 0) {
                    __asm add esp, [fix]
                }
            }
        }
    };
}

#endif /* _LILAC_META_X86_OPTCALL_HPP_ */
