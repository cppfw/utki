#pragma once

#include <functional>

#include "debug.hpp"
#include "types.hpp"

namespace utki{

#if __cplusplus >= 201703L

template <typename C> class linq_collection_aggregator{
	typename std::conditional<std::is_rvalue_reference<C>::value,
			typename std::remove_reference<C>::type,
			C
		>::type collection;
public:
	linq_collection_aggregator(C collection) :
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

		return linq_collection_aggregator<decltype(ret)&&>(std::move(ret));
	}
};

template <typename C> linq_collection_aggregator<const C&> linq(const C& collection){
	return linq_collection_aggregator<const C&>(collection);
}

template <typename C> linq_collection_aggregator<const C&> linq(C& collection){
	return linq_collection_aggregator<const C&>(collection);
}

template <typename C> linq_collection_aggregator<C&&> linq(C&& collection){
	return linq_collection_aggregator<C&&>(std::move(collection));
}

#endif

}
