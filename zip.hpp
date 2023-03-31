#pragma once

#include <iterator>

template<typename T>
concept Array = std::is_array_v<T>;

template<typename T>
concept Iterator = requires(T a, T b) {
    ++a;
    *a;
    a == b;
    a != b;
};

template<typename T>
concept Collection = requires(T a){
    { a.begin() } -> Iterator;
};

template<typename T>
struct TypesOf {
};

template<Collection T>
struct TypesOf<T> {
    using iter_type = decltype(T().begin());
    using value_type = decltype(*iter_type());
};

template<Array T>
struct TypesOf<T> {
    using iter_type = std::remove_extent<T>::type *;
    using value_type = std::remove_extent<T>::type;
};

template<typename TupleT, std::size_t... Indexes>
bool AnyEqual(const TupleT &x, const TupleT &y, std::index_sequence<Indexes...>) {
    std::initializer_list<bool> pair_eq = {std::get<Indexes>(x) == std::get<Indexes>(y)...};
    return std::any_of(pair_eq.begin(), pair_eq.end(), [](auto x) { return x; });
}

template<typename... Seqs>
class Zip {
    using IterTuple = std::tuple<typename TypesOf<Seqs>::iter_type...>;
    using ValueTuple = std::tuple<typename TypesOf<Seqs>::value_type &...>;

    IterTuple begins_;
    IterTuple ends_;

public:
    explicit Zip(Seqs &... seq) :
            begins_(std::make_tuple(std::begin(seq)...)),
            ends_(std::make_tuple(std::end(seq)...)) {}

    struct ZipIterator {
        using iterator_category = std::forward_iterator_tag;
        using difference_type = std::ptrdiff_t;

        using value_type = ValueTuple;
        using pointer = value_type *;
        using reference = value_type &;

        IterTuple curr;

        explicit ZipIterator(const IterTuple &iterTuple) : curr(iterTuple) {}

        ZipIterator &operator++() {
            std::apply([](auto &... x) { (++x, ...); }, curr);
            return *this;
        }

        value_type operator*() {
            return std::apply([](auto &... x) { return std::tie(*x...); }, curr);
        }

        bool operator==(const ZipIterator &other) const {
            return AnyEqual<>(curr, other.curr, std::make_index_sequence<sizeof...(Seqs)>());
        }

        bool operator!=(const ZipIterator &other) const {
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