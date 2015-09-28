/**
 * @author Ivan Gagis <igagis@gmail.com>
 * @brief buffer wrapper class.
 */


#pragma once

#include <array>
#include <vector>
#include <ostream>

#ifdef DEBUG
#	include <iostream>
#endif

#include "util.hpp"
#include "debug.hpp"



namespace utki{



/**
 * @brief Buffer template class.
 * This class is a wrapper of continuous memory buffer, it encapsulates pointer to memory block and size of that memory block.
 * It does not own the memory.
 */
template <class T> class Buf final{
public:
	typedef T value_type;
	typedef value_type* pointer;
	typedef const value_type* const_pointer;
	typedef value_type& reference;
	typedef const value_type& const_reference;
	typedef value_type* iterator;
	typedef const value_type* const_iterator;
	typedef std::size_t size_type;
	typedef std::ptrdiff_t difference_type;
	typedef std::reverse_iterator<iterator> reverse_iterator;
	typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
	
private:
	pointer buf = nullptr;
	size_type bufSize = 0;
	
public:

	Buf(const Buf&) = default;
	Buf& operator=(const Buf&) = default;
	
	
	/**
	 * @brief Create a Buffer object.
	 * Creates a Buffer object which wraps given memory buffer of specified size.
	 * Note, the memory will not be freed upon this Buffer object destruction,
	 * Buffer does not own the memory.
	 * @param bufPtr - pointer to the memory buffer.
	 * @param bufSize - size of the memory buffer.
	 */
	Buf(pointer bufPtr, size_type bufSize)noexcept :
			buf(bufPtr),
			bufSize(bufSize)
	{}

	
	//TODO: make constructor for automatic conversion from nullptr
	Buf()noexcept{}
	
	
	/**
	 * @brief Create Buffer from std::array.
	 * Creates a Buffer object pointing to the contents of given std::array.
     * @param a - reference to instance of std::array.
     */
	template <std::size_t array_size> Buf(const std::array<typename std::remove_const<T>::type, array_size>& a) :
			buf(a.size() == 0 ? 0 : &*a.begin()), //operator* on invalid pointer may cause crash
			bufSize(a.size())
	{}
	
	template <std::size_t array_size> Buf(std::array<T, array_size>& a) :
			buf(a.size() == 0 ? 0 : &*a.begin()), //operator* on invalid pointer may cause crash
			bufSize(a.size())
	{}
	
	
	/**
	 * @brief Create Buffer from std::vector.
	 * Creates a Buffer object pointing to the contents of given std::vector.
     * @param v - reference to instance of std::vector.
     */
	Buf(const std::vector<typename std::remove_const<T>::type>& v) :
			buf(v.size() == 0 ? 0 : &*v.begin()), //operator* on invalid pointer may cause crash
			bufSize(v.size())
	{}
	
	//TODO: remove vector and array constructors and add corresponding wrapBuf() overloads
	Buf(std::vector<T>& v) :
			buf(v.size() == 0 ? 0 : &*v.begin()), //operator* on invalid pointer may cause crash
			bufSize(v.size())
	{}
	
	
	/**
	 * @brief Automatic conversion to const type.
     * @return 
     */
	operator Buf<const T>()const{
		return Buf<const T>(this->buf, this->size());
	}
	
	
	
	/**
	 * @brief get buffer size.
	 * @return number of elements in buffer.
	 */
	size_type size()const noexcept{
		return this->bufSize;
	}



	/**
	 * @brief get size of element.
	 * @return size of element in bytes.
	 */
	size_type sizeOfElem()const noexcept{
		return sizeof(this->buf[0]);
	}



	/**
	 * @brief get size of buffer in bytes.
	 * @return size of array in bytes.
	 */
	size_type sizeInBytes()const noexcept{
		return this->size() * this->sizeOfElem();
	}



	/**
	 * @brief access specified element of the buffer.
	 * Const version of Buffer::operator[].
	 * @param i - element index.
	 * @return reference to i'th element of the buffer.
	 */
	const_reference operator[](size_type i)const noexcept{
		ASSERT(i < this->size())
		return this->buf[i];
	}



	/**
	 * @brief access specified element of the buffer.
	 * @param i - element index.
	 * @return reference to i'th element of the buffer.
	 */
	reference operator[](size_type i)noexcept{
		ASSERT_INFO(i < this->size(), "operator[]: index out of bounds")
		return this->buf[i];
	}



	/**
	 * @brief get pointer to first element of the buffer.
	 * @return pointer to first element of the buffer.
	 */
	iterator begin()noexcept{
		return this->buf;
	}



	/**
	 * @brief get pointer to first element of the buffer.
	 * @return pointer to first element of the buffer.
	 */
	const_iterator begin()const noexcept{
		return this->cbegin();
	}

	const_iterator cbegin()const noexcept{
		return this->buf;
	}


	/**
	 * @brief get pointer to "after last" element of the buffer.
	 * @return pointer to "after last" element of the buffer.
	 */
	iterator end()noexcept{
		return this->buf + this->bufSize;
	}



	/**
	 * @brief get const pointer to "after last" element of the buffer.
	 * @return const pointer to "after last" element of the buffer.
	 */
	const_iterator end()const noexcept{
		return this->cend();
	}
	
	const_iterator cend()const noexcept{
		return this->buf + this->bufSize;
	}

	
	const_reverse_iterator crbegin()const noexcept{
		return const_reverse_iterator(this->end());
	}

	const_reverse_iterator crend()const noexcept{
		return const_reverse_iterator(this->begin());
	}
	
	reverse_iterator rbegin()noexcept{
		return reverse_iterator(this->end());
	}

	const_reverse_iterator rbegin()const noexcept{
		return const_reverse_iterator(this->end());
	}

	reverse_iterator rend()noexcept{
		return reverse_iterator(this->begin());
	}

	const_reverse_iterator rend()const noexcept{
		return const_reverse_iterator(this->begin());
	}
	
	pointer data()noexcept{
		return this->buf;
	}

	const_pointer data()const noexcept{
		return this->buf;
	}
	


	/**
	 * @brief Checks if pointer points somewhere within the buffer.
	 * @param p - pointer to check.
	 * @return true - if pointer passed as argument points somewhere within the buffer.
	 * @return false otherwise.
	 */
	bool overlaps(const_pointer p)const noexcept{
		return this->begin() <= p && p <= (this->end() - 1);
	}



	friend std::ostream& operator<<(std::ostream& s, const Buf<T>& buf){
		for(auto i = buf.begin(); i != buf.end(); ++i){
			s << "\t" << (*i) << std::endl;
		}
		return s;
	}
};



template <class T> inline utki::Buf<T> wrapBuf(T* buf, size_t size){
	return utki::Buf<T>(buf, size);
}

template <class T> inline const utki::Buf<T> wrapBuf(const T* buf, size_t size){
	return utki::Buf<T>(const_cast<T*>(buf), size);
}



}//~namespace
