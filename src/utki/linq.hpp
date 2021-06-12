#pragma once

#include <functional>
#include <vector>
#include <map>

#include "debug.hpp"
#include "types.hpp"
#include "config.hpp"

namespace utki{

#if M_CPP >= 17

template <typename C> class linq_collection_aggregator{
	template <typename CC> friend class linq_collection_aggregator;
	template <typename CC> friend linq_collection_aggregator<const CC&> linq(const CC&);
	template <typename CC> friend linq_collection_aggregator<const CC&> linq(CC&);
	template <typename CC> friend linq_collection_aggregator<CC&&> linq(CC&&);

	typename std::conditional<std::is_rvalue_reference<C>::value,
			typename std::remove_reference<C>::type,
			C
		>::type collection;
	
	typedef typename std::remove_reference<decltype(collection)>::type::value_type value_type;

	struct noncopyable_value_type : value_type{
		noncopyable_value_type(const noncopyable_value_type&) = delete;
		noncopyable_value_type& operator=(const noncopyable_value_type&) = delete;
	};

	linq_collection_aggregator(C collection) :
			collection(std::move(collection))
	{}

public:
	decltype(collection) get(){
		return std::move(this->collection);
	}

	template <typename F> auto select(F func){
		typedef typename std::add_rvalue_reference<value_type>::type func_arg_type;

		static constexpr bool func_one_arg = std::is_invocable_v<F, func_arg_type>;

		static_assert(
				func_one_arg || std::is_invocable_v<F, func_arg_type, size_t>,
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
		static_assert(
				std::is_invocable_v<F, noncopyable_value_type>, // non-const lvalue reference cannot bind to temproray objects, thus F(auto&) will fail the assert, but F(const auto&) succeeds
				"functor must have const reference argument"
			);

		static_assert(
				!std::is_same_v<void, std::invoke_result_t<F, const value_type&>>,
				"functor must return non-void"
			);

		typedef typename std::add_lvalue_reference<value_type>::type func_arg_type;
		typedef typename std::remove_reference<
				typename std::invoke_result<F, func_arg_type>::type
			>::type func_return_type;

		std::map<func_return_type, std::vector<value_type>> ret;

		for(auto&& v : this->collection){
			ret[func(v)].push_back(std::move(v));
		}

		return linq_collection_aggregator<decltype(ret)&&>(std::move(ret));
	}

	template <typename F> auto where(F func){
		static_assert(
				std::is_invocable_v<F, noncopyable_value_type>, // non-const lvalue reference cannot bind to temproray objects, thus F(auto&) will fail the assert, but F(const auto&) succeeds
				"functor must have const reference argument"
			);

		static_assert(
				std::is_same<bool, std::invoke_result_t<F, const value_type&>>::value,
				"functor must return bool"
			);

		typename utki::remove_const_reference<decltype(this->collection)>::type ret;

		std::copy_if(
				std::make_move_iterator(this->collection.begin()),
				std::make_move_iterator(this->collection.end()),
				std::inserter(ret, ret.end()),
				func
			);

		return linq_collection_aggregator<decltype(ret)&&>(std::move(ret));
	}

	template <typename F> auto order_by(F func){
		static_assert(
				std::is_invocable_v<F, noncopyable_value_type>, // non-const lvalue reference cannot bind to temproray objects, thus F(auto&) will fail the assert, but F(const auto&) succeeds
				"functor must have const reference argument"
			);

		typedef std::invoke_result_t<F, const value_type&> invoke_result;

		static_assert(
				std::is_reference<invoke_result>::value &&
						std::is_const<typename std::remove_reference<invoke_result>::type>::value
					,
				"functor must return const reference"
			);
		
		auto comparer = [&func](const value_type& a, const value_type& b){
			return func(a) < func(b);
		};

		if constexpr (std::is_rvalue_reference<C>::value){
			std::sort(
					this->collection.begin(),
					this->collection.end(),
					comparer
				);
			return linq_collection_aggregator<decltype(this->collection)&&>(std::move(this->collection));
		}else{
			std::vector<value_type> ret;
			std::copy(
					this->collection.begin(),
					this->collection.end(),
					std::back_inserter(ret)
				);
			
			std::sort(
					ret.begin(),
					ret.end(),
					comparer
				);
			return linq_collection_aggregator<decltype(ret)&&>(std::move(ret));
		}
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
