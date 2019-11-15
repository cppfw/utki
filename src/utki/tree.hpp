#pragma once

#include <vector>

namespace utki{

//TODO: doxygen
template <class T, template <class, class> class C = std::vector, template <class> class A = std::allocator > class tree{
public:
    typedef T value_type;
    typedef C<tree, A<tree>> container_type;

    T value;
    container_type children;

    tree() = default;

    tree(const tree&) = default;

    tree& operator=(const tree&) = default;

    tree(tree&& t) = default;

    tree(std::initializer_list<tree> l) :
            children(l)
    {}

    tree(const T& value, std::initializer_list<tree> l) :
            value(value),
            children(l)
    {}

    tree(const T& value) :
            value(value)
    {}

    tree(T&& value) :
            value(std::move(value))
    {}

    bool operator==(const T& value)const noexcept{
        return this->value == value;
    }
};

}
