#pragma once

#include <functional>

#include "debug.hpp"

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

template <
		template <typename...> typename C,
		typename... T
	>
class linq{
	C<T...> vec;
public:
	linq(C<T...>&& vec) :
			vec(std::move(vec))
	{}

	C<T...> get(){
		return std::move(this->vec);
	}

	template <typename F> auto select(F func){
		std::vector<decltype(func(std::move(this->vec[0])))> ret;
		ret.reserve(this->vec.size());

		for(auto&& e : this->vec){
			ret.push_back(func(std::move(e)));
		}

		return linq<std::vector, typename decltype(ret)::value_type>(std::move(ret));
	}
};

#endif

}
