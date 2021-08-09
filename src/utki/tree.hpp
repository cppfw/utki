/*
The MIT License (MIT)

Copyright (c) 2015-2021 Ivan Gagis <igagis@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

/* ================ LICENSE END ================ */

#pragma once

#include <vector>
#include <cstddef>

#include "debug.hpp"
#include "span.hpp"

namespace utki{

/**
 * @brief N-ary tree data structure.
 * This class represents a tree node.
 * Each instance of tree node contains some value and a list of children tree nodes.
 * @param T - the tree node data type.
 * @param C - the container type the tree is based on.
 * @param A - memory allocator.
 */
template <
		class T,
		template <class, class> class C = std::vector,
		template <class> class A = std::allocator
	>
class tree{
public:
	/**
	 * @brief Type of the tree node value.
	 */
	typedef T value_type;

	/**
	 * @brief Type of the child tree nodes container.
	 * The container type the tree is based on.
	 */
	typedef C<tree, A<tree>> container_type;

	/**
	 * @brief Value of the tree node.
	 */
	T value;

	/**
	 * @brief List of child tree nodes.
	 */
	container_type children;

	tree() = default;
	tree(const tree&) = default;
	tree& operator=(const tree&) = default;
	tree(tree&&) = default;

	/**
	 * @brief Create a tree node with the given value.
	 * Creates a tree node and intialize its value with a copy of the given value.
	 * Children list of the created node is empty.
	 * @param value - initializer of the tree node value.
	 */
	tree(const T& value) :
			value(value)
	{}

	/**
	 * @brief Create a tree node with the given value.
	 * Creates a tree node and initializes its value with the given value.
	 * Children list of the created node is empty.
	 * @param value - initializer of the tree node value.
	 */
	tree(T&& value) :
			value(std::move(value))
	{}

	/**
	 * @brief Create a tree node with the given list of children.
	 * The value of the created node will be contructed with its default constructor.
	 * @param children - initializer of the tree node children.
	 */
	tree(const container_type& children) :
			children(children)
	{}

	/**
	 * @brief Create a tree node with the given list of children.
	 * The value of the created node will be contructed with its default constructor.
	 * @param children - initializer of the tree node children.
	 */
	tree(container_type&& children) :
			children(std::move(children))
	{}

	/**
	 * @brief Create a tree node with the given list of children.
	 * The value of the created node will be contructed with its default constructor.
	 * @param children - initializer of the tree node children.
	 */
	tree(std::initializer_list<tree> children) :
			children(children)
	{}

	/**
	 * @brief Create a tree node with the given value and list of children.
	 * @param value - initializer of the tree node value.
	 * @param children - initializer of the tree node children.
	 */
	tree(const T& value, std::initializer_list<tree> children) :
			value(value),
			children(children)
	{}

	/**
	 * @brief Create a tree node with the given value and list of children.
	 * @param value - initializer of the tree node value.
	 * @param children - initializer of the tree node children.
	 */
	tree(T&& value, std::initializer_list<tree> children) :
			value(std::move(value)),
			children(children)
	{}

	/**
	 * @brief Create a tree node with the given value and list of children.
	 * @param value - initializer of the tree node value.
	 * @param children - initializer of the tree node children.
	 */
	tree(const T& value, const container_type& children) :
			value(value),
			children(children)
	{}

	/**
	 * @brief Create a tree node with the given value and list of children.
	 * @param value - initializer of the tree node value.
	 * @param children - initializer of the tree node children.
	 */
	tree(const T& value, container_type&& children) :
			value(value),
			children(std::move(children))
	{}

	/**
	 * @brief Create a tree node with the given value and list of children.
	 * @param value - initializer of the tree node value.
	 * @param children - initializer of the tree node children.
	 */
	tree(T&& value, container_type&& children) :
			value(std::move(value)),
			children(std::move(children))
	{}

	/**
	 * @brief Compare this tree node's value with another value for equality.
	 * Calls operator==() on the tree node's value.
	 * This operator is provided for compatibility with STL algorithms.
	 */
	template <typename compare_value_type>
	bool operator==(const compare_value_type& value)const noexcept{
		return this->value == value;
	}

	bool operator==(const tree& t)const noexcept{
		return this->value == t.value && this->children == t.children;
	}
};

/**
 * @brief Helper for traversing the tree data structure.
 * This class is a wrapper class which provides STL-like container interface which
 * allows traversal of the tree data structure.
 * The traversal is done in pre-order, i.e. first the parent node is visited, then its child nodes are visited.
 * @param C - container type the tree is based on.
 */
template <class C> class traversal{
	static_assert(
			std::is_same<decltype(C::value_type::children), typename std::remove_const<C>::type>::value,
			"C::value_type must have 'children' member of type C"
		);

	C& roots;
public:
	/**
	 * @brief Construct a traversal object around given list of trees.
	 * The traversal object does not take ownership of the passed in list of trees, but it holds a reference to that.
	 * So, the given list of trees must remain alive during the whole life-span of the traversal object.
	 * @param trees - list of trees through which the traversal will be performed.
	 */
	traversal(C& trees) :
			roots(trees)
	{}

	/**
	 * @brief Tree node type.
	 */
	typedef typename std::conditional<std::is_const<C>::value, const typename C::value_type, typename C::value_type>::type value_type;

	/**
	 * @brief Pointer to tree node type.
	 */
	typedef value_type* pointer;

	/**
	 * @brief Constant pointer to tree node type.
	 */
	typedef const value_type* const_pointer;

	/**
	 * @brief Reference to tree node type.
	 */
	typedef value_type& reference;

	/**
	 * @brief Constant reference to tree node type.
	 */
	typedef const value_type& const_reference;

	/**
	 * @brief List size type.
	 */
	typedef typename C::size_type size_type;

	/**
	 * @brief List index difference type.
	 */
	typedef typename C::difference_type difference_type;

private:
	template <bool Is_const> class iterator_internal{
		typedef typename std::conditional<Is_const, const C, C>::type list_type;
		std::vector<list_type*> list_stack;
		typedef typename std::conditional<Is_const, typename C::const_iterator, typename C::iterator>::type iterator_type;
		std::vector<iterator_type> iter_stack;

		template<class> friend class traversal;

		iterator_internal(C& roots, iterator_type i){
			this->list_stack.push_back(&roots);
			this->iter_stack.push_back(i);
		}

	public:
		iterator_internal() = default;

		/**
		 * @brief Tree node type.
		 */
		typedef typename std::conditional<Is_const, const typename traversal::value_type, typename traversal::value_type>::type value_type;
		
		typedef value_type* pointer;
		typedef const value_type* const_pointer;
		typedef value_type& reference;
		typedef const value_type& const_reference;
		typedef std::bidirectional_iterator_tag iterator_category;
		typedef typename C::difference_type difference_type;

		/**
		 * @brief Move iterator to the next tree node.
		 * The iterator is moved to the next tree node through tree hieararchy, so that it performs
		 * a traversal of the tree. I.e. visiting each tree node.
		 * @return Reference to this iterator.
		 */
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

		/**
		 * @brief Move iterator to the previous tree node.
		 * The iterator is moved to the previous tree node through tree hieararchy, so that it performs
		 * a traversal of the tree. I.e. visiting each tree node.
		 * @return Reference to this iterator.
		 */
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

		/**
		 * @brief Chack that two iterators point to the same tree node.
		 * @return false in case this iterator points to a different tree node than the given iterator.
		 * @return true in case this iterator points to the same tree node as the given iterator.
		 */
		bool operator==(const iterator_internal& iter)const noexcept{
			return this->iter_stack == iter.iter_stack;
		}

		/**
		 * @brief Check that iterators point to different tree nodes.
		 * @return true in case this iterator points to a different tree node than the given iterator.
		 * @return false in case this iterator points to the same tree node as the given iterator.
		 */
		bool operator!=(const iterator_internal& iter)const noexcept{
			return !this->operator==(iter);
		}

		/**
		 * @brief Check if this iterator precedes given iterator in traversal order.
		 * @param iter - iterator to compare this iterator with.
		 * @return true if this iterator precedes the given iterator in traversal order.
		 * @return false otherwise.
		 */
		bool operator<(const iterator_internal& iter)const noexcept{
			return this->iter_stack < iter.iter_stack;
		}

		/**
		 * @brief Check if this iterator follows given iterator in traversal order.
		 * @param iter - iterator to compare this iterator with.
		 * @return true if this iterator follows the given iterator in traversal order.
		 * @return false otherwise.
		 */
		bool operator>(const iterator_internal& iter)const noexcept{
			return this->iter_stack > iter.iter_stack;
		}

		/**
		 * @brief Check if this iterator precedes given iterator in traversal order or is equal to it.
		 * @param iter - iterator to compare this iterator with.
		 * @return true if this iterator precedes the given iterator in traversal order or is equal to it.
		 * @return false otherwise.
		 */
		bool operator<=(const iterator_internal& iter)const noexcept{
			return this->iter_stack <= iter.iter_stack;
		}

		/**
		 * @brief Check if this iterator follows given iterator in traversal order or is equal to it.
		 * @param iter - iterator to compare this iterator with.
		 * @return true if this iterator follows the given iterator in traversal order or is equal to it.
		 * @return false otherwise.
		 */
		bool operator>=(const iterator_internal& iter)const noexcept{
			return this->iter_stack >= iter.iter_stack;
		}

		/**
		 * @brief Dereference the tree node.
		 * @return Reference to the tree node this iterator points to.
		 */
		typename std::conditional<Is_const, const typename C::value_type, typename C::value_type>::type& operator*()noexcept{
			return *this->iter_stack.back();
		}

		/**
		 * @brief Dereference the tree node.
		 * Constant version of operator*().
		 * @return Reference to the tree node this iterator points to.
		 */
		const typename C::value_type& operator*()const noexcept{
			return *this->iter_stack.back();
		}

		/**
		 * @brief Dereference the tree node.
		 * @return Pointer to the tree node this iterator points to.
		 */
		typename std::conditional<Is_const, const typename C::value_type, typename C::value_type>::type* operator->()noexcept{
			return this->iter_stack.back().operator->();
		}

		/**
		 * @brief Dereference the tree node.
		 * Constant version of operator->().
		 * @return Pointer to the tree node this iterator points to.
		 */
		const typename C::value_type* operator->()const noexcept{
			return this->iter_stack.back().operator->();
		}

		/**
		 * @brief Get index of the tree node.
		 * The index of the tree node into the tree hierarchy is the list of indices into
		 * tree node children lists starting from the root list of tree nodes.
		 * @return the index into tree hierarchy of the tree node this iterator points to.
		 */
		std::vector<size_type> index()const noexcept{
			ASSERT(this->iter_stack.size() == this->list_stack.size())
			std::vector<size_type> ret;
			for(typename decltype(this->iter_stack)::size_type i = 0; i != this->iter_stack.size(); ++i){
				ret.push_back(std::distance(this->list_stack[i]->begin(), this->iter_stack[i]));
			}
			return ret;
		}

		/**
		 * @brief Get iterator depth.
		 * Zero depth means that the iterator does not point to any tree node.
		 * Depth of one means that the iterator points to one of the root nodes.
		 * And so on.
		 * @return number depth of the tree node the iterator points to.
		 */
		size_t depth()const noexcept{
			return this->iter_stack.size();
		}

		/**
		 * @brief Dereference the tree node at given tree level.
		 * @param d - tree level.
		 * @return Reference to the tree node this iterator points to on the requested tree level.
		 * @throw std::out_of_range - in case the requested tree level is higher than the iterator depth.
		 */
		typename std::conditional<Is_const, const typename C::value_type, typename C::value_type>::type&
		at_level(size_t d)noexcept{
			return *this->iter_stack.at(d);
		}

		/**
		 * @brief Dereference the tree node at given tree level.
		 * Constant version of at_level().
		 * @return Reference to the tree node this iterator points to on the requested tree level.
		 * @throw std::out_of_range - in case the requested tree level is higher than the iterator depth.
		 */
		const typename C::value_type& at_level(size_t d)const noexcept{
			return *this->iter_stack.at(d);
		}

		/**
		 * @brief Check if this iterator points to the last tree node on the current tree level.
		 * @return true if the iterator points to the last tree node on the current tree level.
		 * @return false otherwise.
		 */
		bool is_last_child()const noexcept{
			if(this->iter_stack.back() == this->list_stack.back()->end()){
				return false;
			}

			return std::next(this->iter_stack.back()) == this->list_stack.back()->end();
		}
	};

public:
	/**
	 * @brief Iterator type.
	 * The iterator performs the traversal of the tree.
	 */
	typedef iterator_internal<std::is_const<C>::value> iterator;

	/**
	 * @brief Constant iterator type.
	 * The iterator performs the traversal of the tree.
	 */
	typedef iterator_internal<true> const_iterator;

	/**
	 * @brief Reverse iterator type.
	 * The iterator performs the traversal of the tree.
	 */
	typedef std::reverse_iterator<iterator> reverse_iterator;

	/**
	 * @brief Constant reverse iterator type.
	 * The iterator performs the traversal of the tree.
	 */
	typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

private:
	template <bool Is_const> iterator_internal<Is_const> make_iterator_internal(utki::span<const size_type> index)const{
		iterator_internal<Is_const> ret;

		auto* list = &this->roots;
		auto iter = this->roots.begin();

		for(auto i : index){
			ret.list_stack.push_back(list);
			if(i >= list->size()){
				throw std::out_of_range("given index points out of the tree");
			}
			ret.iter_stack.push_back(std::next(iter, i));
			list = &ret.iter_stack.back()->children;
			iter = list->begin();
		}

		return ret;
	}
public:
	/**
	 * @brief Create iterator which points to the tree node given by index.
	 * @param index - the index of the tree node.
	 * @return Iterator which points to the tree node given by index.
	 */
	iterator make_iterator(utki::span<const size_type> index){
		return this->make_iterator_internal<std::is_const<C>::value>(index);
	}

	/**
	 * @brief Create iterator which points to the tree node given by index.
	 * @param index - the index of the tree node.
	 * @return Iterator which points to the tree node given by index.
	 */
	iterator make_iterator(const std::vector<size_type>& index){
		return this->make_iterator(utki::make_span(index));
	}

	/**
	 * @brief Create iterator which points to the tree node given by index.
	 * @param index - the index of the tree node.
	 * @return iterator which points to the tree node given by index.
	 */
	iterator make_iterator(std::initializer_list<size_t> index){
		return this->make_iterator(std::vector<size_t>(index));
	}

	/**
	 * @brief Create constant iterator which points to the tree node given by index.
	 * @param index - the index of the tree node.
	 * @return Constant iterator which points to the tree node given by index.
	 */
	const_iterator make_const_iterator(utki::span<const size_type> index)const{
		return this->make_iterator_internal<true>(index);
	}

	/**
	 * @brief Create constant iterator which points to the tree node given by index.
	 * @param index - the index of the tree node.
	 * @return Constant iterator which points to the tree node given by index.
	 */
	const_iterator make_const_iterator(const std::vector<size_type>& index)const{
		return this->make_const_iterator(utki::make_span(index));
	}

	/**
	 * @brief Create constant iterator which points to the tree node given by index.
	 * @param index - the index of the tree node.
	 * @return Constant iterator which points to the tree node given by index.
	 */
	const_iterator make_const_iterator(std::initializer_list<size_t> index)const{
		return this->make_const_iterator(std::vector<size_t>(index));
	}

	/**
	 * @brief Check if the given index is valid.
	 * Checks that the given index points to the existing tree node within the
	 * tree hierarchy, i.e. the index does not point out of the tree bounds.
	 * @param index - index to check for validity.
	 * @return true in case the given index is valid.
	 * @return fals in case the given index is invalid.
	 */
	bool is_valid(utki::span<const size_type> index)const{
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

	/**
	 * @brief Check if the given index is valid.
	 * Checks that the given index points to the existing tree node within the
	 * tree hierarchy, i.e. the index does not point out of the tree bounds.
	 * @param index - index to check for validity.
	 * @return true in case the given index is valid.
	 * @return fals in case the given index is invalid.
	 */
	bool is_valid(const std::vector<size_type>& index)const{
		return this->is_valid(utki::make_span(index));
	}

	/**
	 * @brief Check if the given index is valid.
	 * Checks that the given index points to the existing tree node within the
	 * tree hierarchy, i.e. the index does not point out of the tree bounds.
	 * @param index - index to check for validity.
	 * @return true in case the given index is valid.
	 * @return fals in case the given index is invalid.
	 */
	bool is_valid(std::initializer_list<size_t> index)const{
		return this->is_valid(std::vector<size_t>(index));
	}

	/**
	 * @brief Get reference to tree element by index.
	 * @param index - index of the tree element.
	 * @return reference to the tree elemenet referred by index.
	 */
	typename C::value_type& operator[](utki::span<const size_type> index){
		ASSERT(this->is_valid(index))
		// TODO: rewrite without creating iterator, for more performance
		return *this->make_iterator(index);
	}

	/**
	 * @brief Get constant reference to tree element by index.
	 * @param index - index of the tree element.
	 * @return reference to the tree elemenet referred by index.
	 */
	const typename C::value_type& operator[](utki::span<const size_type> index)const{
		ASSERT(this->is_valid(index))
		// TODO: rewrite without creating iterator, for more performance
		return *this->make_const_iterator(index);
	}

	/**
	 * @brief Get reference to tree element by index.
	 * @param index - index of the tree element.
	 * @return reference to the tree elemenet referred by index.
	 */
	typename C::value_type& operator[](const std::vector<size_t>& index){
		return this->operator[](utki::make_span(index));
	}

	/**
	 * @brief Get constant reference to tree element by index.
	 * @param index - index of the tree element.
	 * @return reference to the tree elemenet referred by index.
	 */
	const typename C::value_type& operator[](const std::vector<size_t>& index)const{
		return this->operator[](utki::make_span(index));
	}

	/**
	 * @brief Get reference to tree element by index.
	 * @param index - index of the tree element.
	 * @return reference to the tree elemenet referred by index.
	 */
	typename C::value_type& operator[](std::initializer_list<size_t> index){
		return this->operator[](std::vector<size_t>(index));
	}

	/**
	 * @brief Get constant reference to tree element by index.
	 * @param index - index of the tree element.
	 * @return reference to the tree elemenet referred by index.
	 */
	const typename C::value_type& operator[](std::initializer_list<size_t> index)const{
		return this->operator[](std::vector<size_t>(index));
	}

	/**
	 * @brief Get iterator pointing to the beginning of the tree hierarchy.
	 * @return Iterator pointing to the beginning of the tree hierarchy.
	 */
	iterator begin(){
		return iterator(this->roots, this->roots.begin());
	}

	/**
	 * @brief Get constant iterator pointing to the beginning of the tree hierarchy.
	 * @return Constant iterator pointing to the beginning of the tree hierarchy.
	 */
	const_iterator cbegin()const{
		return const_iterator(this->roots, this->roots.cbegin());
	}

	/**
	 * @brief Get iterator pointing to the end of the tree hierarchy.
	 * @return Iterator pointing to the end of the tree hierarchy.
	 */
	iterator end(){
		return iterator(this->roots, this->roots.end());
	}

	/**
	 * @brief Get constant iterator pointing to the end of the tree hierarchy.
	 * @return constant iterator pointing to the end of the tree hierarchy.
	 */
	const_iterator cend()const{
		return const_iterator(this->roots, this->roots.cend());
	}

	/**
	 * @brief Get constant reverse iterator pointing to the reverse beginning of the tree hierarchy.
	 * @return Constant reverse iterator pointing to the reverse beginning of the tree hierarchy.
	 */
	const_reverse_iterator crbegin()const noexcept{
		return const_reverse_iterator(this->cend());
	}

	/**
	 * @brief Get constant reverse iterator pointing to the reverse end of the tree hierarchy.
	 * @return constant reverse iterator pointing to the reverse end of the tree hierarchy.
	 */
	const_reverse_iterator crend()const noexcept{
		return const_reverse_iterator(this->cbegin());
	}
	
	/**
	 * @brief Get reverse iterator pointing to the reverse beginning of the tree hierarchy.
	 * @return Reverse iterator pointing to the reverse beginning of the tree hierarchy.
	 */
	reverse_iterator rbegin()noexcept{
		return reverse_iterator(this->end());
	}

	/**
	 * @brief Get reverse iterator pointing to the reverse end of the tree hierarchy.
	 * @return Reverse iterator pointing to the reverse end of the tree hierarchy.
	 */
	reverse_iterator rend()noexcept{
		return reverse_iterator(this->begin());
	}

	/**
	 * @brief Insert new tree node before given iterator.
	 * Insertion happens on the current tree level.
	 * This operation invalidates all obtained iterators.
	 * @param i - iterator pointing to the tree node to insert the new node before.
	 * @param t - tree node to insert.
	 * @return iterator pointing to the newly inserted tree node.
	 */
	iterator insert(iterator i, value_type&& t){
		i.iter_stack.back() = i.list_stack.back()->insert(i.iter_stack.back(), std::move(t));
		return i;
	}

	/**
	 * @brief Insert new tree node before given iterator.
	 * Insertion happens on the current tree level.
	 * This operation invalidates all obtained iterators.
	 * @param i - iterator pointing to the tree node to insert the new node before.
	 * @param t - tree node to insert.
	 * @return iterator pointing to the newly inserted tree node.
	 */
	iterator insert(iterator i, const value_type& t){
		return this->insert(i, value_type(t));
	}

	/**
	 * @brief Insert new tree node after given iterator.
	 * Insertion happens on the current tree level.
	 * This operation invalidates all obtained iterators.
	 * The result of this operation is undefined if performed on an end iterator.
	 * @param i - iterator pointing to the tree node to insert the new node after.
	 * @param t - tree node to insert.
	 * @return iterator pointing to the newly inserted tree node.
	 */
	iterator insert_after(iterator i, value_type&& t){
		i.iter_stack.back() = i.list_stack.back()->insert(std::next(i.iter_stack.back()), std::move(t));
		return i;
	}

	/**
	 * @brief Insert new tree node after given iterator.
	 * Insertion happens on the current tree level.
	 * This operation invalidates all obtained iterators.
	 * The result of this operation is undefined if performed on an end iterator.
	 * @param i - iterator pointing to the tree node to insert the new node after.
	 * @param t - tree node to insert.
	 * @return iterator pointing to the newly inserted tree node.
	 */
	iterator insert_after(iterator i, const value_type& t){
		return this->insert_after(i, value_type(t));
	}

	/**
	 * @brief Erase tree node pointed by given iterator.
	 * This operation invalidates all obtained iterators.
	 * @param i - iterator pointing to the tree node to remove.
	 * @return iterator pointing to the next tree node in traversal order after the removed one.
	 */
	iterator erase(iterator i){
		i.iter_stack.back() = i.list_stack.back()->erase(i.iter_stack.back());
		while(i.iter_stack.size() > 1 && i.iter_stack.back() == i.list_stack.back()->end()){
			i.list_stack.pop_back();
			i.iter_stack.pop_back();
			ASSERT(i.iter_stack.size() == i.list_stack.size())
			++i.iter_stack.back();
		}
		return i;
	}
};

/**
 * @brief Construct traversal object out given list of tree nodes.
 * This is a helper method for automatic template arguments resolution when creating traversal object.
 * @param trees - list of tree nodes to make traversal object around.
 * @return Traversal object.
 */
template<class C> traversal<C> make_traversal(C& trees){
	return traversal<C>(trees);
}

}
