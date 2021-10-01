#ifndef __LILAC_META_COMMON_HPP__
#define __LILAC_META_COMMON_HPP__

#include <type_traits>

namespace lilac::meta {
    template<bool cond>
    class ternary {
    public:
        template<class T, class F>
        static constexpr decltype(auto) val(T t, F f) { return f; }

        template<class T, class F>
        using type = F;
    };

    template<>
    class ternary<true> {
    public:
        template<class T, class F>
        static constexpr decltype(auto) val(T t, F f) { return t; }

        template<class T, class F>
        using type = T;
    };

    template<class Class, class... Compare>
    static constexpr bool any_of = std::disjunction_v<std::is_same<Class, Compare>...>;

}

#endif /* __LILAC_META_COMMON_HPP__ */