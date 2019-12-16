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

    tree(const T& value) :
            value(value)
    {}

    tree(T&& value) :
            value(std::move(value))
    {}

    tree(const container_type& c) :
            children(c)
    {}

    tree(container_type&& c) :
            children(std::move(c))
    {}

    tree(std::initializer_list<tree> l) :
            children(l)
    {}

    tree(const T& value, std::initializer_list<tree> l) :
            value(value),
            children(l)
    {}

    tree(T&& value, std::initializer_list<tree> l) :
            value(std::move(value)),
            children(l)
    {}

    tree(const T& value, const container_type& c) :
            value(value),
            children(c)
    {}

    tree(const T& value, container_type&& c) :
            value(value),
            children(std::move(c))
    {}

    tree(T&& value, container_type&& c) :
            value(std::move(value)),
            children(std::move(c))
    {}

    bool operator==(const T& value)const noexcept{
        return this->value == value;
    }
};

}
