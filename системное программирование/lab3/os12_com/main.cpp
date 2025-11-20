//#define OS12COM_EXPORTS
#include <Windows.h>
#include "os12_com.h"
#include "Registry.h"
extern "C" BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    if (fdwReason == DLL_PROCESS_ATTACH)
        g_hModule = hinstDLL;
    return TRUE;
}

extern "C" HRESULT __stdcall DllGetClassObject(REFCLSID rclsid, REFIID riid, void** ppv)
{
    if (rclsid != clsid_os12_com)
        return CLASS_E_CLASSNOTAVAILABLE;

    os12Factory* factory = new os12Factory();
    if (!factory)
        return E_OUTOFMEMORY;

    HRESULT hr = factory->QueryInterface(riid, ppv);
    factory->Release();
    return hr;
}

 extern "C" HRESULT __stdcall DllCanUnloadNow(void)
{
    if (g_objCount == 0 && g_serverLocks == 0)
        return S_OK;
    else
        return S_FALSE;
}

 extern "C" HRESULT __stdcall DllRegisterServer(void)
 {
     const WCHAR szFriendlyName[] = L"os12 COM Component";
     const WCHAR szVerIndProgID[] = L"os12.lab3";
     const WCHAR szProgID[] = L"os12.lab3.1";

     HRESULT hr = RegisterServer(g_hModule, clsid_os12_com, szFriendlyName, szVerIndProgID, szProgID);

    

     return hr;
 }

 extern "C" HRESULT __stdcall DllUnregisterServer(void)
{
    const WCHAR szVerIndProgID[] = L"os12.lab3";
    const WCHAR szProgID[] = L"os12.lab3.1";

    return UnregisterServer(clsid_os12_com, szVerIndProgID, szProgID);
}