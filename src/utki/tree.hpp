#pragma once

#include <vector>

namespace utki{

//TODO: doxygen
template <class T, template <class, class> class C = std::vector, template <class> class A = std::allocator > class tree :
        public C<tree<T, C, A>, A<tree<T, C, A>>>
{
    T value;
public:

    typedef C<tree, A<tree>> container_type;

    tree() = default;

    tree(const tree&) = default;

    tree& operator=(const tree&) = default;

    container_type& children()noexcept{
        return *this;
    }

    const container_type& children()const noexcept{
        return *this;
    }

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

    T& leaf()noexcept{
        return this->value;
    }

    const T& get()const noexcept{
        return this->value;
    }

    bool operator==(const T& value)const noexcept{
        return this->get() == value;
    }
};

}
