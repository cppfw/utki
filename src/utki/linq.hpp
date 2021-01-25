#pragma once

#include <functional>
#include <vector>
#include <map>

#include "debug.hpp"
#include "types.hpp"

namespace utki{

#if __cplusplus >= 201703L

template <typename C> class linq_collection_aggregator{
	typename std::conditional<std::is_rvalue_reference<C>::value,
			typename std::remove_reference<C>::type,
			C
		>::type collection;
	
	typedef typename std::remove_reference<decltype(collection)>::type::value_type value_type;
public:
	linq_collection_aggregator(C collection) :
			collection(std::move(collection))
	{}

	decltype(collection) get(){
		return std::move(this->collection);
	}

	template <typename F> auto select(F func){
		typedef typename std::add_rvalue_reference<value_type>::type func_arg_type;

		static constexpr bool func_one_arg = is_type_defined<std::invoke_result<F, func_arg_type>>::value;

		static_assert(
				func_one_arg || is_type_defined<std::invoke_result<F, func_arg_type, size_t>>::value,
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

	template <typename F> auto group_by(F func){
		typedef typename std::add_lvalue_reference<value_type>::type func_arg_type;
		typedef typename std::remove_reference<
				typename std::invoke_result<F, func_arg_type>::type
			>::type func_return_type;

		std::map<func_return_type, std::vector<value_type>> ret;

		for(auto&& v : this->collection){
			const auto& cv = v;
			ret[func(cv)].push_back(std::move(v));
		}

		return linq_collection_aggregator<decltype(ret)&&>(std::move(ret));
	}

	template <typename F> auto where(F func){
		typename utki::remove_constref<decltype(this->collection)>::type ret;

		// this assertion makes sure that functor argument is not a non-const reference
		// static_assert(
		// 		is_type_defined<std::invoke_result<F, const typename remove_constref<C>::type::value_type>>::value,
		// 		"functor must have const reference argument"
		// 	);

		// static_assert(
		// 		!is_type_defined<std::invoke_result<F, value_type>>::value,
		// 		"functor must have const reference argument"
		// 	);

		typedef std::invoke_result<
				F,
				const value_type&
			> invoke_result;

		// this assertion makes sure that functor argument is reference
		static_assert(
				is_type_defined<invoke_result>::value,
				"functor must have const reference argument"
			);

		static_assert(
				std::is_same<bool, typename invoke_result::type>::value,
				"functor must return bool"
			);

		std::copy_if(
				std::make_move_iterator(this->collection.begin()),
				std::make_move_iterator(this->collection.end()),
				std::inserter(ret, ret.end()),
				func
			);

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
