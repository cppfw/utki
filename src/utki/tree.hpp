#pragma once

#include <vector>

namespace utki{

//TODO: doxygen
template <class T, template <class, class> class C = std::vector, template <class> class A = std::allocator> class tree{
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

template <class T> class traverser{
	T& roots;
public:
	traverser(T& roots) :
			roots(roots)
	{}

	typedef typename std::conditional<std::is_const<T>::value, const typename T::value_type, typename T::value_type>::type value_type;
	typedef value_type* pointer;
	typedef const value_type* const_pointer;
	typedef value_type& reference;
	typedef const value_type& const_reference;
	typedef size_t size_type;
	typedef size_t difference_type;

	template <bool Is_const> class iterator_internal{
		typedef typename std::conditional<Is_const, const T, T>::type list_type;
		std::vector<list_type*> c;
		typedef typename std::conditional<Is_const, typename T::const_iterator, typename T::iterator>::type iterator_type;
		std::vector<iterator_type> i;

		template<class> friend class traverser;

		iterator_internal(T& roots, iterator_type i){
			this->c.push_back(&roots);
			this->i.push_back(i);
		}

	public:
		typedef typename std::conditional<Is_const, const traverser::value_type, traverser::value_type>::type value_type;
		typedef value_type* pointer;
		typedef const value_type* const_pointer;
		typedef value_type& reference;
		typedef const value_type& const_reference;
		typedef std::bidirectional_iterator_tag iterator_category;
		typedef size_t difference_type;

		iterator_internal& operator++()noexcept{
			ASSERT(!this->i.empty())
			ASSERT(this->i.size() == this->c.size())

			{
				ASSERT(this->c.back())
				auto& iter = this->i.back();

				ASSERT(iter != this->c.back()->end())
				if(!iter->children.empty()){
					this->c.push_back(&iter->children);
					this->i.push_back(this->c.back()->begin());
					return *this;
				}
				++iter;
			}

			ASSERT(this->i.size() >= 1)
			for(; this->i.size() != 1;){
				if(this->i.back() == this->c.back()->end()){
					this->c.pop_back();
					this->i.pop_back();
					++this->i.back();
				}else{
					break;
				}
			}
			
			return *this;
		}

		iterator_internal& operator--()noexcept{
			ASSERT(!this->i.empty())
			ASSERT(!this->c.empty())

			if(this->i.back() == this->c.back()->begin()){
				this->i.pop_back();
				this->c.pop_back();
				return *this;
			}

			for(;;){
				auto& iter = this->i.back();
				ASSERT(iter != this->c.back()->begin())
				--iter;

				if(!iter->children.empty()){
					this->c.push_back(&iter->children);
					this->i.push_back(this->c.back()->end());
				}else{
					break;
				}
			}

			return *this;
		}

		bool operator==(const iterator_internal& iter)const noexcept{
			return this->i == iter.i;
		}

		bool operator!=(const iterator_internal& iter)const noexcept{
			return !this->operator==(iter);
		}

		typename std::conditional<Is_const, const typename T::value_type, typename T::value_type>::type& operator*()noexcept{
			return *this->i.back();
		}

		typename std::conditional<Is_const, const typename T::value_type, typename T::value_type>::type* operator->()noexcept{
			return this->i.back().operator->();
		}
	};

	typedef iterator_internal<std::is_const<T>::value> iterator;
	typedef iterator_internal<true> const_iterator;
	typedef std::reverse_iterator<iterator> reverse_iterator;
	typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

	iterator begin(){
		return iterator(this->roots, this->roots.begin());
	}

	const_iterator cbegin()const{
		return const_iterator(this->roots, this->roots.cbegin());
	}

	iterator end(){
		return iterator(this->roots, this->roots.end());
	}

	const_iterator cend()const{
		return const_iterator(this->roots, this->roots.cend());
	}

	const_reverse_iterator crbegin()const noexcept{
		return const_reverse_iterator(this->cend());
	}

	const_reverse_iterator crend()const noexcept{
		return const_reverse_iterator(this->cbegin());
	}
	
	reverse_iterator rbegin()noexcept{
		return reverse_iterator(this->end());
	}

	reverse_iterator rend()noexcept{
		return reverse_iterator(this->begin());
	}
};

template<class T> traverser<T> make_traverser(T& roots){
	return traverser<T>(roots);
}

}
