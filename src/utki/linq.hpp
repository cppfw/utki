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
	
	typedef typename std::add_rvalue_reference<
			typename std::remove_reference<decltype(collection)>::type::value_type
		>::type func_arg_type;
public:
	linq_collection_aggregator(C collection) :
			collection(std::move(collection))
	{}

	decltype(collection) get(){
		return std::move(this->collection);
	}

	template <typename F> auto select(F func){
		static constexpr bool func_one_arg = !std::is_same<void, typename type_or_void<std::invoke_result<F, func_arg_type>>::type>::value;

		static_assert(
				func_one_arg || !std::is_same<void, typename type_or_void<std::invoke_result<F, func_arg_type, size_t>>::type>::value,
				"passed in function must have one argument or two arguments with the second argument of type size_t"
			);

		typedef typename std::conditional<
				func_one_arg,
				typename type_or_void<std::invoke_result<F, func_arg_type>>::type,
				typename type_or_void<std::invoke_result<F, func_arg_type, size_t>>::type
			>::type func_return_type;

		std::vector<func_return_type> ret;
		ret.reserve(this->collection.size());

		if constexpr (func_one_arg){
			for(auto&& e : this->collection){
				ret.push_back(func(std::move(e)));
			}
		}else{ // two arguments
			size_t i = 0;
			for(auto&& e : this->collection){
				ret.push_back(func(std::move(e), i));
				++i;
			}
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
