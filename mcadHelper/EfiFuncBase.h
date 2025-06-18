#ifndef MCADHELPER_EFIFUNC_H_INCLUDED__
#define MCADHELPER_EFIFUNC_H_INCLUDED__
#pragma once
// #include "MCADINCL.H"
#include <windef.h>
#include <list>

namespace mcadHelper {

	class EfiFuncBase
	{
	public:
		template<class ReturnType, class... Args>
		EfiFuncBase(const char *name, const char *params, const char *desc, LRESULT(*Pfn)(ReturnType, Args...));
		~EfiFuncBase() = default;
	private:
		EfiFuncBase() = default;
		EfiFuncBase(const EfiFuncBase &) = delete;
		EfiFuncBase(EfiFuncBase&&) = default;
		EfiFuncBase&operator=(const EfiFuncBase &) = delete;
		EfiFuncBase&operator=(EfiFuncBase &&) = default;
		EfiFuncBase(const char *name, const char *params, const char *desc, void *pfn, long unsigned int returnType, size_t nArgs);
	public:
		static void registerFunctions(HMODULE hMod);
		static void createFunctionInfo();
	private:
		template<class Head, class... Tail> 
		struct Next;

		template<class T>
		static long unsigned int mcType();

		FUNCTIONINFO info{ nullptr,nullptr,nullptr,nullptr,0,0,{ 0,0,0,0,0,0,0,0,0,0 } };
		const std::string name_{};
		const std::string params_{};
		const std::string desc_{};

		static std::list<EfiFuncBase *> &efiFunctions();
	};

#define STR1(a) #a
#define STR(a) STR1(a)

	template<class ReturnType, class... Args>
	inline
	EfiFuncBase::EfiFuncBase(const char *name, const char *params, const char *desc, LRESULT(*Pfn)(ReturnType, Args...))
		: EfiFuncBase(name, params, desc, Pfn, mcType<ReturnType>(), sizeof...(Args))
	{
		static_assert(sizeof...(Args) <= MAX_ARGS, "Too many arguments. Can only have a maximum of" STR(MAX_ARGS) " arguments. ");
		static_assert(sizeof...(Args) > 0, "Must have at least argument.");
		long unsigned int *ptr = info.argType;
		Next<Args...> foo{ ptr };
	}

	template<class Head, class... Tail>
	struct EfiFuncBase::Next : public EfiFuncBase::Next<Tail...>
	{
		Next(long unsigned int *ptr)
			: EfiFuncBase::Next<Tail...>{ (ptr + 1) }
		{
			*ptr = EfiFuncBase::mcType<Head>();
		}
	};

	template<class Head>
	struct EfiFuncBase::Next<Head>
	{
		Next(long unsigned int *ptr)
		{
			*ptr = EfiFuncBase::mcType<Head>();
		}
	};

	template<class T>
	class EfiFunc : public EfiFuncBase
	{
	public:
		EfiFunc()
			: EfiFuncBase(typename T::name, typename T::params, typename T::desc, typename T::fn)
		{

		}
	};
};

#endif // MCADHELPER_EFIFUNC_H_INCLUDED__
