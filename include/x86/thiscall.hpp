#ifndef _LILAC_META_X86_THISCALL_HPP_
#define _LILAC_META_X86_THISCALL_HPP_

#include "function.hpp"

namespace lilac::meta::x86 {
    template<class Ret, class... Args>
    class Thiscall : public CallConv<Args...> {
    private:
        using MyConv = CallConv<Args...>;
        using MyTuple = Tuple<Args...>;

    public:
        template<size_t i, class Current>
        struct filter {
            static constexpr bool value = true;
        };

        template<size_t... indices>
        static decltype(auto) invoke(
            Ret(*addr)(Args...),
            const Tuple<Args...>&& all,
            const std::index_sequence<indices...>&&
        ) {
            Ret(__thiscall* raw)(
                typename MyTuple::template type_at<indices>...
            ) = reinterpret_cast<decltype(raw)>(addr);

            if constexpr (!std::is_same_v<Ret, void>) {
                Ret ret = raw(
                    all.template at<indices>()...
                );
                return ret;
            }
            else {
                raw(
                    all.template at<indices>()...
                );
            }
        }
    };
}

#endif /* _LILAC_META_X86_MEMBERCALL_HPP_ */
