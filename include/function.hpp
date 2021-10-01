#ifndef __LILAC_META_FUNCTION_HPP__
#define __LILAC_META_FUNCTION_HPP__

#include "tuple.hpp"
#include "common.hpp"

#include <type_traits>

namespace lilac::meta {
    template<typename F>
    class Function;

    template<class Ret, class... Args>
    class Function<Ret(Args...)> {
    private:
        using MyTuple = Tuple<Args...>;

        template<class Class>
        static constexpr bool gpr_passable = 
            any_of<
                std::remove_cv_t<Class>,
                bool, signed char, signed short, signed int, signed long,
                unsigned char, unsigned short, unsigned int, unsigned long,
                char, short, int, long
            > || 
            std::is_pointer_v<Class>;

        template<class Class>
        static constexpr bool sse_passable = 
            any_of<
                std::remove_cv_t<Class>,
                float, double
            >;

        template<size_t i, class Current>
        struct OptcallFilter {
            static constexpr bool value = 
                (!gpr_passable<Current> &&
                !sse_passable<Current>) || i > 3;
        };

        template<class... Stack>
        static constexpr size_t stack_fix = 
            (((sizeof(Stack) % sizeof(void*) == 0) ?
                sizeof(Stack) :
                sizeof(Stack) - (sizeof(Stack) % sizeof(void*)) + sizeof(void*)) + ...);

        template<>
        static constexpr size_t stack_fix<> = 0;

    private:
        const unsigned int addr;

    private:
        template<size_t... indices>
        decltype(auto) invoke(
            const MyTuple&& all,
            std::index_sequence<indices...>&&
        ) const {
            #define check(index, cond, alt)\
            typename std::conditional_t<\
                        (MyTuple::size > index) &&\
                        (cond<typename MyTuple::template type_at<index>>),\
                        typename MyTuple::template type_at<index>,\
                        alt\
                    >

            Ret(__vectorcall* raw)(
                check(0, sse_passable, float),
                check(1, sse_passable, float),
                check(2, sse_passable, float),
                check(3, sse_passable, float),
                float, float,
                check(0, gpr_passable, int),
                check(1, gpr_passable, int),
                typename MyTuple::template type_at<indices>...
            ) = reinterpret_cast<decltype(raw)>(addr);

            #undef check

            #define check(index, cond, alt)\
                ternary<\
                    (MyTuple::size > index) &&\
                    (cond<typename MyTuple::template type_at<index>>)\
                >::val(all.template at<index>(), alt)

            raw(
                check(0, sse_passable, 1907.0f),
                check(1, sse_passable, 1907.0f),
                check(2, sse_passable, 1907.0f),
                check(3, sse_passable, 1907.0f),
                1907.0f,
                1907.0f,
                check(0, gpr_passable, 1907),
                check(1, gpr_passable, 1907),
                all.template at<indices>()...
            );
            
            #undef check

            static constexpr size_t fix = stack_fix<typename MyTuple::template type_at<indices>...>;
            if constexpr (fix != 0) {
                __asm add esp, [fix]
            }
        }

    public:
        Function(const unsigned int addr)
            : addr(addr) {}

        decltype(auto) operator()(Args... all) const {
            return invoke(
                { all... },
                typename MyTuple::template filter<OptcallFilter>{}
            );
        }
    };
}

#endif /* __LILAC_META_FUNCTION_HPP__ */