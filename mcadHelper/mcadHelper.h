#ifndef MCADHELPER_MCADHELPER_H_INCLUDED__
#define MCADHELPER_MCADHELPER_H_INCLUDED__
#pragma once
#include "MCADINCL.H"
#include <string>
#include "EfiFuncBase.h"
#include <initializer_list>
#include <type_traits>
#include <tuple>
#include <exception>

namespace mcadHelper {


	// Neither of these can be the return type.
	// Use this for INFILE
	using LPCINFILE = const char *;
	// use this for OUTFILE
	using LPCOUTFILE = const unsigned char *;

	WORD registerErrorMessage(const char *message);

}


#define BEGIN_FN(n,p,d) \
namespace {namespace n##FN { struct FN : public mcadHelper::EfiFunc<FN> { \
	static constexpr char *name = #n; \
	static constexpr char *params = p; \
	static constexpr char *desc = d


#define MCFN(n,p,d,...) BEGIN_FN(n,p,d); \
	static LRESULT fn(__VA_ARGS__)

#define END_FN }fn{}; }}


template<class container>
bool RealVector(LPCOMPLEXARRAY arr, const container &c)
{
	auto success = (std::empty(c) == false);
	if (success)
	{
		success = (FALSE != MathcadArrayAllocate(arr, std::size(c), 1, true, false));
		if(success)
		{
			auto *ptr = arr->hReal[0];
			for (auto &&e : c)
			{
				*ptr = static_cast<double>(e);
				++ptr;
			}
		}
	}
	return success;
}

template<class T>
bool RealVector(LPCOMPLEXARRAY arr, const std::initializer_list<T> &c)
{
	auto success = (std::empty(c) == false);
	if (success)
	{
		success = (FALSE != MathcadArrayAllocate(arr, std::size(c), 1, true, false));
		if(success)
		{
			auto *ptr = arr->hReal[0];
			for (auto &&e : c)
			{
				*ptr = static_cast<double>(e);
				++ptr;
			}
		}
	}
	return success;
}

template<class T, class Int, typename std::enable_if< std::is_integral<typename std::decay<Int>::type>::value, int>::type = 0 >
bool RealVector(LPCOMPLEXARRAY arr, T *c, Int&& size)
{
	auto success = (size > 0);
	if (success)
	{
		success = (FALSE != MathcadArrayAllocate(arr, size, 1, true, false));
		if(success)
		{
			auto *ptr = arr->hReal[0];
			for(auto i=0UL; i<static_cast<unsigned int>(size);++i)
			{
				ptr[i] = static_cast<double>(c[i]);
			}
		}
	}
	return success;
}

namespace details {

	template<class T, typename std::enable_if< std::negation_v<std::is_pointer<std::remove_reference_t<T>>>, int>::type = 0>
	void RealVec(double *ptr, T a)
	{
		*ptr = static_cast<double>(a);
	}

	template<class T, class... Targs, typename std::enable_if< std::negation_v<std::is_pointer<std::remove_reference_t<T>>>, int>::type = 0>
	void RealVec(double *ptr, T a, Targs&&... args)
	{
		*ptr = static_cast<double>(a);
		++ptr;
		RealVec(ptr, std::forward<Targs>(args)...);
	}
}

template<class... Targs>
bool RealVector(LPCOMPLEXARRAY arr, Targs... args)
{
	auto size = sizeof...(args);
	auto success = size > 0;
	if (success)
	{
		success = (FALSE != MathcadArrayAllocate(arr, static_cast<unsigned int>(size), 1, true, false));
		if (success)
		{
			details::RealVec(arr->hReal[0], std::forward<Targs&&>(args)...);
		}
	}
	return success;
}


template<unsigned N>
struct apply_func
{
	template<typename... Targs, typename... Qargs, typename... Args>
	static bool apply(LPCOMPLEXARRAY arr,bool(*f)(LPCOMPLEXARRAY , Targs...), const std::tuple<Qargs...> &t, Args... args)
	{
		return apply_func<N - 1>::apply(arr, f, t, std::get<N - 1>(t), args...);
	}
};

template <>
struct apply_func<0>
{
	template<typename... Targs, typename... Qargs, typename... Args>
	static bool apply(LPCOMPLEXARRAY arr,bool(*f)(LPCOMPLEXARRAY , Targs...), const std::tuple<Qargs...> &, Args... args)
	{
		return f(arr, args...);
	}

};

template<class... Targs, class... Fargs>
bool applyTuple(LPCOMPLEXARRAY arr, bool(*f)(LPCOMPLEXARRAY arr, Targs...), const std::tuple<Fargs...> &c)
{
	return apply_func<sizeof...(Targs)>::apply(arr, f, c);
}


template<class... Targs>
bool RealVectorT(LPCOMPLEXARRAY arr, const std::tuple<Targs...> &args)
{
	return applyTuple(arr, RealVector<Targs...>, args);
}

class mcVector : public COMPLEXARRAY
{
public:
	explicit
	mcVector(size_t size)
	{
		if (!MathcadArrayAllocate(this, static_cast<unsigned int>(size), 1, TRUE, FALSE))
			throw std::bad_alloc{};
	}
	template<class... Targs>
	mcVector(Targs... args)
	{
		if (!MathcadArrayAllocate(this, sizeof...(args), 1, TRUE, FALSE))
			throw std::bad_alloc{};
		assign(0, std::forward<Targs>(args)...);
	}
private:
	void assign(size_t n)
	{
	}
	template<class... Targs>
	void assign(size_t n, double d, Targs... args)
	{
		this->hReal[0][n] = d;
		assign(n + 1, std::forward<Targs>(args)...);
	}
};

void *getModuleInstance();


#ifdef _M_X64
#pragma comment(linker, "/include:DllMain")
#else
#pragma comment(linker, "/include:_DllMain@12")
#endif
#pragma comment(lib, "mcaduser")

#endif // MCADHELPER_MCADHELPER_H_INCLUDED__
