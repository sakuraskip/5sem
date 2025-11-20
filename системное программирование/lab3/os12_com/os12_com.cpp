#include "os12_com.h"
#include <Windows.h> 

HMODULE g_hModule = NULL;
LONG g_serverLocks = 0;
LONG g_objCount = 0;


os12::os12() : m_refCount(1)
{
    InterlockedIncrement(&g_objCount);
}

os12::~os12()
{
    InterlockedDecrement(&g_objCount);
}

HRESULT __stdcall os12::QueryInterface(REFIID riid, void** ppv)
{
    if (riid == IID_IUnknown)
        *ppv = static_cast<IUnknown*>(static_cast<IAdder*>(this));
    else if (riid == iid_adder)
        *ppv = static_cast<IAdder*>(this);
    else if (riid == iid_multiplier)
        *ppv = static_cast<IMultiplier*>(this);
    else
    {
        *ppv = NULL;
        return E_NOINTERFACE;
    }
    AddRef();
    return S_OK;
}

ULONG __stdcall os12::AddRef()
{
    return InterlockedIncrement(&m_refCount);
}

ULONG __stdcall os12::Release()
{
    ULONG res = InterlockedDecrement(&m_refCount);
    if (res == 0)
        delete this;
    return res;
}

HRESULT __stdcall os12::Add(const double x, const double y, double& z)
{
    z = x + y;
    return S_OK;
}

HRESULT __stdcall os12::Sub(const double x, const double y, double& z)
{
    z = x - y;
    return S_OK;
}

HRESULT __stdcall os12::Mul(const double x, const double y, double& z)
{
    z = x * y;
    return S_OK;
}

HRESULT __stdcall os12::Div(const double x, const double y, double& z)
{
    if (y == 0) return E_INVALIDARG;
    z = x / y;
    return S_OK;
}


os12Factory::os12Factory() : m_refCount(1) {}

os12Factory::~os12Factory() {}

HRESULT __stdcall os12Factory::QueryInterface(REFIID riid, void** ppv)
{
    if (riid == IID_IUnknown || riid == IID_IClassFactory)
    {
        *ppv = static_cast<IClassFactory*>(this);
        AddRef();
        return S_OK;
    }
    *ppv = NULL;
    return E_NOINTERFACE;
}

ULONG __stdcall os12Factory::AddRef()
{
    return InterlockedIncrement(&m_refCount);
}

ULONG __stdcall os12Factory::Release()
{
    ULONG res = InterlockedDecrement(&m_refCount);
    if (res == 0)
        delete this;
    return res;
}

HRESULT __stdcall os12Factory::CreateInstance(IUnknown* pUnkOuter, REFIID riid, void** ppvObject)
{
    if (pUnkOuter != NULL)
        return CLASS_E_NOAGGREGATION;

    os12* calc = new os12();
    if (!calc)
        return E_OUTOFMEMORY;

    HRESULT hr = calc->QueryInterface(riid, ppvObject);
    calc->Release();

    return hr;
}

HRESULT __stdcall os12Factory::LockServer(BOOL fLock)
{
    if (fLock)
        InterlockedIncrement(&g_serverLocks);
    else
        InterlockedDecrement(&g_serverLocks);
    return S_OK;
}