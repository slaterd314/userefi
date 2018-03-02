#include <SDKDDKVer.h>
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>
#include "mcadHelper.h"
#include "EfiFuncBase.h"


namespace mcadHelper
{
	//BOOL registerUserFunctions(HINSTANCE hInst);
	BOOL registerErrors(HINSTANCE hInst);
}

static
HMODULE
_glbHModule{ nullptr };

void *getModuleInstance()
{
	return reinterpret_cast<void *>(_glbHModule);
}

BOOL APIENTRY DllMain(HMODULE hModule,
					  DWORD  ul_reason_for_call,
					  LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
			_glbHModule = hModule;

			if (!mcadHelper::registerErrors(hModule))
			{
				break;
			}

			mcadHelper::EfiFuncBase::registerFunctions(hModule);

			//mcadHelper::registerUserFunctions(hModule);
			break;
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
			break;
		case DLL_PROCESS_DETACH:
			_glbHModule = nullptr;
			break;
	}
	return TRUE;
}
