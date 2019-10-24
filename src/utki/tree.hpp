#pragma once

#include <vector>

template <class T, template <class, class> class C = std::vector, template <class> class A = std::allocator > class tree :
        public C<tree<T, C, A>, A<tree<T, C, A>>>
{
    T value;
public:

    typedef C<tree, A<tree>> container_type;

    tree() = default;

    tree(const tree&) = default;

    tree& operator=(const tree&) = default;

    tree(tree&& t) = default;

    tree(std::initializer_list<tree> l) :
            container_type(l)
    {}

    tree(const T& value, std::initializer_list<tree> l) :
            container_type(l),
            value(value)
    {}

    tree(const T& value) :
            value(value)
    {}

    tree(T&& value) :
            value(std::move(value))
    {}

    T& get(){
        return this->value;
    }

    const T& get()const{
        return this->value;
    }
};
