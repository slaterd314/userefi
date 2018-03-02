#include "mcadHelper.h"
#include <vector>
#include <list>
#include <vector>
#include <unordered_map>
#include "EfiFuncBase.h"
#include <fstream>

namespace mcadHelper {

	
#define MCTYPE(Type,value) \
	template<> long unsigned int EfiFuncBase::mcType<LP##Type>() { return value;} \
	template<> long unsigned int EfiFuncBase::mcType<LPC##Type>() { return value;} \
	template<> long unsigned int EfiFuncBase::mcType<std::remove_const<LP##Type>::type>() { return value;} \
	template<> long unsigned int EfiFuncBase::mcType<std::remove_const<LPC##Type>::type>() { return value;}

	MCTYPE(COMPLEXSCALAR, COMPLEX_SCALAR);
	MCTYPE(COMPLEXARRAY, COMPLEX_ARRAY);
	MCTYPE(MCSTRING, STRING);

	template<> long unsigned int EfiFuncBase::mcType<LPCINFILE>() { return INFILE; }
	template<> long unsigned int EfiFuncBase::mcType<LPCOUTFILE>() { return OUTFILE; }

#undef MCTYPE


	EfiFuncBase::EfiFuncBase(const char *name, const char *params, const char *desc, void *pfn, long unsigned int returnType, size_t nArgs)
		: info{}
		, name_{ name }
		, params_{params }
		, desc_{ desc }
	{
		info.lpstrName = const_cast<char *>(name_.c_str());
		info.lpstrParameters = const_cast<char *>(params_.c_str());
		info.lpstrDescription = const_cast<char *>(desc_.c_str());
		info.lpfnMyCFunction = reinterpret_cast<LPCFUNCTION>(pfn);
		info.returnType = returnType;
		info.nArgs = static_cast<unsigned int>(nArgs);
		efiFunctions().emplace_back(this);
	}

	// static 
	std::list<EfiFuncBase *> &EfiFuncBase::efiFunctions()
	{
		static std::list<EfiFuncBase *> theList{};
		return theList;
	}


	void EfiFuncBase::registerFunctions(HMODULE hMod)
	{
		for (auto &efi : efiFunctions())
		{
			CreateUserFunction(hMod, &efi->info);
		}
#ifdef _DEBUG
		createFunctionInfo();
#endif
	}

	static
		const char *header = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
		"<FUNCTIONS>\n"
		"\t<help_file>help_%s\\user\\userHelp.htm\\n"
		"\t<!--Insert the name of your custom help file and path here-->\n"
		"\t<!--help_%s looks in the help file in ..\\doc that corresponds-->\n"
		"\t<!--to your language version.The user folder is there. -->\n"
		"\t</help_file>\n";

	static const char *footer = "</FUNCTIONS>";


	// static 
	void EfiFuncBase::createFunctionInfo()
	{
		std::ofstream os{ "d:\\Dev\\userefi\\qd.xml" };
		os << header;
		for (auto &efi : efiFunctions())
		{
			os << "\t<function>\n";
			os << "\t\t<name>" << efi->info.lpstrName << "</name>\n";
			os << "\t\t<params>" << efi->info.lpstrParameters << "</params>\n";
			os << "\t\t<category>double-double/quad-double functions</category>\n";
			os << "\t\t<description>" << efi->info.lpstrDescription << "</description>\n";
			os << "\t</function>\n";
		}
		os << footer << std::endl;
	}

	static std::unordered_map< std::string, WORD > &errorMessages()
	{
		static std::unordered_map< std::string, WORD > theMap{};
		return theMap;
	}

	static std::list<FUNCTIONINFO> &userFunctions()
	{
		static std::list<FUNCTIONINFO> theList{};
		return theList;
	}


	static std::vector<char *> errorTable{};
	
	static WORD errorNum{ 0 };

	WORD registerErrorMessage(const char *message)
	{
		WORD code = 0;
		// errorMessages.emplace_back(message);

		std::string str{ message };
		auto &map = errorMessages();
		auto it = map.find(str);
		if (it == std::end(map))
		{
			code = ++errorNum;
			map.emplace(std::move(str), code);
		}
		else
		{
			code = it->second;
		}

		return code;

	}

	BOOL registerErrors(HINSTANCE hInst)
	{
		auto success = (hInst != nullptr);
		auto &map = errorMessages();
		if (success && !map.empty())
		{
			errorTable.resize(map.size());
			for (const auto &s : map)
			{
				errorTable[s.second - 1] = const_cast<char *>(s.first.c_str());
			}
			success = CreateUserErrorMessageTable(hInst, static_cast<unsigned int>(errorTable.size()), errorTable.data());
		}
		return success;
	}

	namespace details {
		bool registerUserFunction(FUNCTIONINFO &info)
		{
			auto success = (info.nArgs > 0 && info.nArgs < MAX_ARGS);
			if (success)
			{
				userFunctions().emplace_back(std::move(info));
			}
			return success;
		}
	}

	BOOL registerUserFunctions(HINSTANCE hInst)
	{
		auto success = TRUE;
		for (auto &f : userFunctions())
		{
			if (!CreateUserFunction(hInst, &f))
			{
				break;
			}
		}
		return success;
	}
}