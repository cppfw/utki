#pragma once

#include <array>
#include <vector>
#include <ostream>

#ifdef DEBUG
#	include <iostream>
#endif

#include "util.hpp"
#include "debug.hpp"

#include "span.hpp"

// TODO: deprecated, remove whole file. Use 'span' instedad.

namespace utki{


template <class T> using Buf = span<T>;


template <class T> inline utki::Buf<T> wrapBuf(T* buf, size_t size){
	return utki::Buf<T>(buf, size);
}

template <class T> inline const utki::Buf<T> wrapBuf(const T* buf, size_t size){
	return utki::Buf<T>(const_cast<T*>(buf), size);
}

template <class T, std::size_t array_size> inline utki::Buf<T> wrapBuf(std::array<T, array_size>& a){
	return wrapBuf(a.size() == 0 ? nullptr : &*a.begin(), a.size());
}

template <class T, std::size_t array_size> inline const utki::Buf<T> wrapBuf(const std::array<T, array_size>& a){
	return wrapBuf(a.size() == 0 ? nullptr : &*a.begin(), a.size());
}

template <class T> inline utki::Buf<T> wrapBuf(std::vector<T>& v){
	return wrapBuf(v.size() == 0 ? nullptr : &*v.begin(), v.size());
}

template <class T> inline const utki::Buf<T> wrapBuf(const std::vector<T>& v){
	return wrapBuf(v.size() == 0 ? nullptr : &*v.begin(), v.size());
}

inline std::string toString(const Buf<char>& buf){
	return std::string(&*buf.begin(), buf.size());
}

}
