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

template <class T> class traversal{
	static_assert(
			std::is_same<decltype(T::value_type::children), typename std::remove_const<T>::type>::value,
			"T::value_type must have 'children' member of type T"
		);

	T& roots;
public:
	traversal(T& roots) :
			roots(roots)
	{}

	typedef typename std::conditional<std::is_const<T>::value, const typename T::value_type, typename T::value_type>::type value_type;
	typedef value_type* pointer;
	typedef const value_type* const_pointer;
	typedef value_type& reference;
	typedef const value_type& const_reference;
	typedef typename T::size_type size_type;
	typedef size_t difference_type;

private:
	template <bool Is_const> class iterator_internal{
		typedef typename std::conditional<Is_const, const T, T>::type list_type;
		std::vector<list_type*> list_stack;
		typedef typename std::conditional<Is_const, typename T::const_iterator, typename T::iterator>::type iterator_type;
		std::vector<iterator_type> iter_stack;

		template<class> friend class traversal;

		iterator_internal() = default;

		iterator_internal(T& roots, iterator_type i){
			this->list_stack.push_back(&roots);
			this->iter_stack.push_back(i);
		}

	public:
		typedef typename std::conditional<Is_const, const traversal::value_type, traversal::value_type>::type value_type;
		typedef value_type* pointer;
		typedef const value_type* const_pointer;
		typedef value_type& reference;
		typedef const value_type& const_reference;
		typedef std::bidirectional_iterator_tag iterator_category;
		typedef size_type difference_type;

		iterator_internal& operator++()noexcept{
			ASSERT(!this->iter_stack.empty())
			ASSERT(this->iter_stack.size() == this->list_stack.size())

			{
				ASSERT(this->list_stack.back())
				auto& iter = this->iter_stack.back();

				ASSERT(iter != this->list_stack.back()->end())
				if(!iter->children.empty()){
					this->list_stack.push_back(&iter->children);
					this->iter_stack.push_back(this->list_stack.back()->begin());
					return *this;
				}
				++iter;
			}

			ASSERT(this->iter_stack.size() >= 1)
			for(; this->iter_stack.size() != 1;){
				if(this->iter_stack.back() == this->list_stack.back()->end()){
					this->list_stack.pop_back();
					this->iter_stack.pop_back();
					++this->iter_stack.back();
				}else{
					break;
				}
			}
			
			return *this;
		}

		iterator_internal& operator--()noexcept{
			ASSERT(!this->iter_stack.empty())
			ASSERT(!this->list_stack.empty())

			if(this->iter_stack.back() == this->list_stack.back()->begin()){
				this->iter_stack.pop_back();
				this->list_stack.pop_back();
				return *this;
			}

			for(;;){
				auto& iter = this->iter_stack.back();
				ASSERT(iter != this->list_stack.back()->begin())
				--iter;

				if(!iter->children.empty()){
					this->list_stack.push_back(&iter->children);
					this->iter_stack.push_back(this->list_stack.back()->end());
				}else{
					break;
				}
			}

			return *this;
		}

		bool operator==(const iterator_internal& iter)const noexcept{
			return this->iter_stack == iter.iter_stack;
		}

		bool operator!=(const iterator_internal& iter)const noexcept{
			return !this->operator==(iter);
		}

		typename std::conditional<Is_const, const typename T::value_type, typename T::value_type>::type& operator*()noexcept{
			return *this->iter_stack.back();
		}

		typename std::conditional<Is_const, const typename T::value_type, typename T::value_type>::type* operator->()noexcept{
			return this->iter_stack.back().operator->();
		}

		std::vector<size_type> index()const noexcept{
			ASSERT(this->iter_stack.size() == this->list_stack.size())
			std::vector<size_type> ret;
			for(typename decltype(this->iter_stack)::size_type i = 0; i != this->iter_stack.size(); ++i){
				ret.push_back(std::distance(this->list_stack[i]->begin(), this->iter_stack[i]));
			}
			return ret;
		}
	};

public:
	typedef iterator_internal<std::is_const<T>::value> iterator;
	typedef iterator_internal<true> const_iterator;
	typedef std::reverse_iterator<iterator> reverse_iterator;
	typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

private:
	template <bool Is_const> iterator_internal<Is_const> make_iterator_internal(const std::vector<size_type>& index)const{
		iterator_internal<Is_const> ret;

		auto* list = &this->roots;
		auto iter = this->roots.begin();

		for(auto i : index){
			ret.list_stack.push_back(list);
			ASSERT(i < list->size())
			ret.iter_stack.push_back(std::next(iter, i));
			list = &ret.iter_stack.back()->children;
			iter = list->begin();
		}

		return ret;
	}
public:
	iterator make_iterator(const std::vector<size_type> & index){
		return this->make_iterator_internal<std::is_const<T>::value>(index);
	}

	const_iterator make_const_iterator(const std::vector<size_type>& index)const{
		return this->make_iterator_internal<true>(index);
	}

	bool is_valid(const std::vector<size_type>& index)const{
		if(index.empty()){
			return false;
		}
		
		auto li = &this->roots;
		for(auto i : index){
			if(i >= li->size()){
				return false;
			}
			li = &(*li)[i].children;
		}
		return true;
	}

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

template<class T> traversal<T> make_traversal(T& roots){
	return traversal<T>(roots);
}

}
