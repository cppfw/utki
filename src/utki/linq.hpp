#pragma once

#include <functional>

#include "debug.hpp"

namespace utki{

#if __cplusplus >= 201703L
template <
		template <typename...> typename C,
		typename T,
		typename F
	>
auto select(C<T>&& collection, F func){
	C<decltype(func(std::move(collection[0])))> ret;

	for(auto&& e : collection){
		ret.push_back(func(std::move(e)));
	}

	return ret;
}

template <
		template <typename...> typename C,
		typename T,
		typename F
	>
auto select(const C<T>& collection, F func){
	C<decltype(func(collection[0]))> ret;

	for(auto& e : collection){
		ret.push_back(func(e));
	}

	return ret;
}
#endif

}
