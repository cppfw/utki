#pragma once

#include <vector>

template <class T, template <class, class> class C = std::vector, class A = std::allocator<T> > class tree :
        public C<tree<T>, A>
{
    T value_v;
public:

    tree() = default;
    tree(const tree&) = default;
    tree(tree&& t) = default;

    tree(std::initializer_list<tree<T, C, A>> l) :
            C<tree<T>, A>(l)
    {}

    tree(const T& value, std::initializer_list<tree<T, C, A>> l) :
            C<tree<T>, A>(l),
            value_v(value)
    {}

    tree(const T& value) :
            value_v(value)
    {}

    tree(T&& value) :
            value_v(std::move(value))
    {}

    T& get(){
        return this->value_v;
    }

    const T& get()const{
        return this->value_v;
    }
};
