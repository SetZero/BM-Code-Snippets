#pragma once
#include <limits.h>
#include "concepts.h"

namespace utils {


	//////////////////////////////////////////////////
	//// aha meta is cool hmmmm

	template<bool condition, typename T1, typename T2>
	struct conditional;

	template<typename T, T X>
	struct integralConstant;

	template<typename T, typename O>
	struct isEqual;

	//should be used with makro
	template<typename T, T x>
	struct	minRequired;

	//should be used with makro
	template<typename T, T x>
	struct minRequiredUnsigned;
	

	template<typename T>
	struct isUnsigned;

	namespace details {
		template<bool condition, typename T1, typename T2>
		struct conditional
		{
			using type = T2;
		};

		template<typename T1, typename T2>
		struct conditional<true, T1, T2>
		{
			using type = T1;
		};


		template<typename T, typename O>
		struct isEqual
		{
			static constexpr bool value = false;
		};

		template<typename T>
		struct isEqual<T, T>
		{
			static constexpr bool value = true;
		};

	
		constexpr size_t abs(const ssize_t ABS) {
			return (ABS < 0 ? -ABS : ABS);
		}

		template<long long x>
		struct	minRequired {
			using min = 							
				typename conditional<x <= SCHAR_MAX && x >= SCHAR_MIN , char,
					typename conditional<x <= SHRT_MAX && x >= SHRT_MIN, short,
						typename conditional<x <= INT_MAX && x >= INT_MIN, int,
							typename conditional<x <= LONG_MAX && x >= LONG_MIN, long, long long
							>::type
						>::type
					>::type
				>::type;
		};

		template<unsigned long long x>
		struct	minRequiredUnsigned {
			using min = 
				typename conditional<x <= UCHAR_MAX, unsigned char,
					typename conditional<x <= USHRT_MAX, unsigned short,
						typename conditional<x <= UINT_MAX, unsigned int,
							typename conditional<x <= ULONG_MAX, unsigned long, unsigned long long
							>::type
						>::type
					>::type
				>::type;
		};

		template<typename T>
		struct isUnsigned {
			static constexpr bool value = false;
		};

		template<>
		struct isUnsigned<unsigned char>
		{
			static constexpr bool value = true;
		};

		template<>
		struct isUnsigned<unsigned short>
		{
			static constexpr bool value = true;
		};

		template<>
		struct isUnsigned<unsigned int>
		{
			static constexpr bool value = true;
		};

		template<>
		struct isUnsigned<unsigned long>
		{
			static constexpr bool value = true;
		};

		template<>
		struct isUnsigned<unsigned long long>
		{
			static constexpr bool value = true;
		};

		template<typename integral, integral val>
		struct isPositive {
			static constexpr bool value = val > 0 ? true : false;
		};
	}

	template<bool condition, typename T1, typename T2>
	struct conditional {
		using type = typename details::conditional<condition, T1, T2>::type;
	};

	template<typename T, T X>
	struct integralConstant {
		static constexpr T value = X;
		using value_type = T;
		using type = integralConstant;
	};

	template<typename T, typename O>
	struct isEqual {
		static constexpr bool value = details::isEqual<T, O>::value;
	};

	#define minReqS(value) typename utils::minRequired<decltype(value),value>::type
	template<typename T, T x>
	struct	minRequired {
		using type = typename conditional<x <= INT_MAX,typename details::minRequired<x>::min,void>::type;
		static_assert(!isEqual<type, void>::value, "signed value is too big to fit to an unsigned type");
	};

	#define minReqU(value) typename utils::minRequiredUnsigned<decltype(value),value>::type
	template<typename T, T x>
	struct minRequiredUnsigned {
		static_assert(x >= 0, " tried to use unsigned type for negative value");
		using type = typename details::minRequiredUnsigned<x>::min;
	};

	template<typename T>
	struct isUnsigned {
		static constexpr bool value = details::isUnsigned<T>::value;
	};

	template<typename first,typename ...T>
	struct list {};


	template<typename T>
	struct front { using type = T; };

	template<template<typename,typename...> typename list, typename F, typename ...T>
	struct front<list<F,T...>> {
		using type = F;
	};

	template<typename Push, typename L>
	struct push_front {};

	template<typename Push,template<typename...> typename List, typename... T>
	struct push_front<Push,List<T...>>
	{
		using type = List<Push,T...>;
	};

	template<typename first,typename ...T>
	constexpr bool sameTypes() {
		if constexpr(sizeof...(T) == 0) 
			return true;
		else {
			if constexpr (!isEqual<first, typename front<list<T...>>::type>::value)
				return false;
			else
				return sameTypes<T...>();
		}
	
	};

	struct safe_size_t {
		size_t* wrappedVal = nullptr;

		constexpr safe_size_t(ssize_t x) {
			if (x >= 0)
				*wrappedVal = x;
		}

		constexpr safe_size_t(size_t x) {
			*wrappedVal = x;
		}
	};

	/*
	template<template<typename, typename...> typename List, typename pop, typename F, typename... T>
	struct popType {
		using type = 
			typename conditional < sizeof...(T) == 0, popType,
				typename conditional<isEqual<F, front<List>::type>::value, popType<List, pop>, popType<list<F, T...>, pop>
				>::type
			>::type;//isEqual<F, first<list>::type>::value ? popType<list, F, T..., pop> : popType<list<F, T...>, F, T..., pop>>::type;
	};	  */

}