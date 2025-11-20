#pragma once
#include <Unknwn.h>

extern HMODULE g_hModule;
extern LONG g_serverLocks;
extern LONG g_objCount;

//#ifdef OS12COM_EXPORTS  
//#define OS12COM_API extern "C" __declspec(dllexport)
//#else
//#define OS12COM_API extern "C" __declspec(dllimport)
//#endif

//OS12COM_API HRESULT __stdcall DllGetClassObject(REFCLSID rclsid, REFIID riid, void** ppv);
//OS12COM_API HRESULT __stdcall DllCanUnloadNow(void);
//OS12COM_API HRESULT __stdcall DllRegisterServer(void);
//OS12COM_API HRESULT __stdcall DllUnregisterServer(void);

// {AAF50891-54E5-449B-90BA-D0EB504989EC}
static const CLSID clsid_os12_com =
{ 0xaaf50891, 0x54e5, 0x449b, { 0x90, 0xba, 0xd0, 0xeb, 0x50, 0x49, 0x89, 0xec } };
// {F27B442D-2651-4782-B7F6-0665637B8825}
static const IID iid_adder =
{ 0xf27b442d, 0x2651, 0x4782, { 0xb7, 0xf6, 0x6, 0x65, 0x63, 0x7b, 0x88, 0x25 } };

// {EF7107EC-2938-4080-9666-97FE2C4E204E}
static const IID iid_multiplier =
{ 0xef7107ec, 0x2938, 0x4080, { 0x96, 0x66, 0x97, 0xfe, 0x2c, 0x4e, 0x20, 0x4e } };

struct IAdder : public IUnknown
{
	virtual HRESULT __stdcall Add(const double x, const double y, double& z) =0;
	virtual HRESULT __stdcall Sub(const double x, const double y, double& z) = 0;

};
struct IMultiplier : public IUnknown
{
	virtual HRESULT __stdcall Mul(const double x, const double y, double& z) = 0;
	virtual HRESULT __stdcall Div(const double x, const double y, double& z) = 0;
};

class os12 : public IAdder, public IMultiplier
{
    LONG m_refCount;

public:
    os12();
    virtual ~os12();

    HRESULT __stdcall QueryInterface(REFIID riid, void** ppv) override;
    ULONG __stdcall AddRef() override;
    ULONG __stdcall Release() override;

    HRESULT __stdcall Add(const double x, const double y, double& z) override;
    HRESULT __stdcall Sub(const double x, const double y, double& z) override;

    HRESULT __stdcall Mul(const double x, const double y, double& z) override;
    HRESULT __stdcall Div(const double x, const double y, double& z) override;
};
class os12Factory : public IClassFactory
{
    LONG m_refCount;
public:
    os12Factory();
    virtual ~os12Factory();

    HRESULT __stdcall QueryInterface(REFIID riid, void** ppv) override;
    ULONG __stdcall AddRef() override;
    ULONG __stdcall Release() override;

    HRESULT __stdcall CreateInstance(IUnknown* pUnkOuter, REFIID riid, void** ppvObject) override;
    HRESULT __stdcall LockServer(BOOL fLock) override;
};