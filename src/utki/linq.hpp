#pragma once

#include <functional>

#include "debug.hpp"
#include "types.hpp"

namespace utki{

#if __cplusplus >= 201703L
template <
		template <typename...> typename C,
		typename... T,
		typename F
	>
auto select(C<T...>&& collection, F func){
	C<decltype(func(std::move(collection[0])))> ret;
	ret.reserve(collection.size());

	for(auto&& e : collection){
		ret.push_back(func(std::move(e)));
	}

	return ret;
}

template <
		template <typename...> typename C,
		typename... T,
		typename F
	>
auto select(const C<T...>& collection, F func){
	C<decltype(func(collection[0]))> ret;
	ret.reserve(collection.size());

	for(auto& e : collection){
		ret.push_back(func(e));
	}

	return ret;
}

template <typename C> class linq{
	typename utki::remove_constref<C>::type collection;
public:
	linq(C collection) :
			collection(std::move(collection))
	{}

	decltype(collection) get(){
		return std::move(this->collection);
	}

	template <typename F> auto select(F func){
		std::vector<decltype(func(std::move(*this->collection.begin())))> ret;
		ret.reserve(this->collection.size());

		for(auto&& e : this->collection){
			ret.push_back(func(std::move(e)));
		}

		return linq<decltype(ret)&&>(std::move(ret));
	}
};

#endif

}
