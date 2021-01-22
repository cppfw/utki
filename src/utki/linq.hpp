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

template <typename C> class linq_aggregator{
	typename std::conditional<std::is_rvalue_reference<C>::value,
			typename std::remove_reference<C>::type,
			C
		>::type collection;
public:
	linq_aggregator(C collection) :
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

		return linq_aggregator<decltype(ret)&&>(std::move(ret));
	}
};

template <typename C> linq_aggregator<C&&> linq(C&& collection){
	TRACE(<< "linq&&" << std::endl)
	return linq_aggregator<C&&>(std::move(collection));
}

template <typename C> linq_aggregator<const C&> linq(const C& collection){
	TRACE(<< "const linq&" << std::endl)
	return linq_aggregator<const C&>(collection);
}

#endif

}
