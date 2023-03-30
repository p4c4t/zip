#pragma once

#include <iterator>

template <typename TupleT, std::size_t... Indexes>
bool AnyEqual(const TupleT& x, const TupleT& y, std::index_sequence<Indexes...>) {
    bool any = false;
    ((any = (any || std::get<Indexes>(x) == std::get<Indexes>(y))), ...);
    return any;
}


template <typename... Iters>
class ZipAbstraction {
    std::tuple<Iters...> begins_;
    std::tuple<Iters...> ends_;
public:
    ZipAbstraction(const std::tuple<Iters...>& begins, const std::tuple<Iters...>& ends)
        : begins_(begins), ends_(ends) { }

    struct ZipIterator {
        using iterator_category = std::forward_iterator_tag;
        using difference_type = std::ptrdiff_t;

        using value_type = std::tuple<decltype(*Iters())&...>;
        using pointer = value_type*;
        using reference = value_type&;

        std::tuple<Iters...> curr;

        ZipIterator(const std::tuple<Iters...>& iters) {
            curr = iters;
        }

        ZipIterator& operator++() {
            curr = std::apply([](auto&... x) { return std::make_tuple(std::next(x)...); }, curr);
            return *this;
        }

        ZipIterator operator++(int) {
            ZipIterator tmp = *this;
            ++(*this);
            return tmp;
        }
        value_type operator*() {
            return std::apply([](auto&... x) { return std::tie(*x...); }, curr);
        }

        bool operator==(const ZipIterator& other) const {
            return AnyEqual<>(curr, other.curr, std::make_index_sequence<std::tuple_size_v<decltype(curr)>>());
        }
        bool operator!=(const ZipIterator& other) const {
            return !operator==(other);
        }
    };

    ZipIterator begin() {
        return ZipIterator(begins_);
    }

    ZipIterator end() {
        return ZipIterator(ends_);
    }
};

template <typename... Seqs>
auto Zip(Seqs&... seq) {
    return ZipAbstraction(std::make_tuple(std::begin(seq)...), std::make_tuple(std::end(seq)...));
}
