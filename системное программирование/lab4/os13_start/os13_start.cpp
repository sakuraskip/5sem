#include <iostream>
#include <windows.h>
#include <conio.h>
#include "os13_lib.h"

using namespace std;

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        cerr << "usage: ht11open.exe {filename}" << endl;
        return 1;
    }

    char* filename = argv[1];

    HANDLE globalMutex = CreateMutexA(NULL, FALSE, "ht_mutex");
    if (!globalMutex)
    {
        cerr << "initMutex failed" << endl;
        return 1;
    }

    WaitForSingleObject(globalMutex, INFINITE);

    HRESULT hr = InitializeCOM();
    if (FAILED(hr)) {
        cerr << "Failed to initialize COM. HRESULT: 0x" << hex << hr << endl;
        ReleaseMutex(globalMutex);
        CloseHandle(globalMutex);
        return 1;
    }

    IHT* pIHT = nullptr;
    hr = GetIHTInterface(&pIHT);
    if (FAILED(hr) || pIHT == nullptr) {
        cerr << "Failed to get IHT interface. HRESULT: 0x" << hex << hr << endl;
        UninitializeCOM();
        ReleaseMutex(globalMutex);
        CloseHandle(globalMutex);
        return 1;
    }

    HTHANDLE* ht = nullptr;
    hr = pIHT->Open(filename, &ht);

    if (FAILED(hr) || ht == nullptr)
    {
        cerr << "ht11Open: Open error via COM. HRESULT: 0x" << hex << hr << endl;
        pIHT->Release();
        UninitializeCOM();
        ReleaseMutex(globalMutex);
        CloseHandle(globalMutex);
        return 1;
    }

    if (ht != nullptr)
    {
        cout << "ht-storage opened\nfilename: " << filename
            << " snapInterval: " << ht->SecSnapshotInterval
            << " capacity: " << ht->Capacity
            << " maxkeylength: " << ht->MaxKeyLength
            << " maxpayloadlength: " << ht->MaxPayloadLength << endl;
    }

    cout << "\npress any key to close the application " << endl;

    do
    {
        Sleep(ht->SecSnapshotInterval * 1000);

        hr = pIHT->Snap(ht);
        if (FAILED(hr)) {
            cerr << "Snapshot failed. HRESULT: 0x" << hex << hr << endl;
        }
        else {
            cout << "Snapshot created successfully" << endl;
        }

    } while (!_kbhit());

    hr = pIHT->Close(ht);

    pIHT->Release();

    HRESULT uninitHr = UninitializeCOM();
    if (FAILED(uninitHr)) {
        cerr << "Warning: Failed to uninitialize COM properly. HRESULT: 0x" << hex << uninitHr << endl;
    }

    ReleaseMutex(globalMutex);
    CloseHandle(globalMutex);

    if (SUCCEEDED(hr))
    {
        cout << "closed successfully" << endl;
        return 0;
    }

    cout << "close error. HRESULT: 0x" << hex << hr << endl;
    return 1;
}