#include "lab1HT.h"
#include <iostream>
#include <Windows.h>
#include <cstring>
#include <thread>

using namespace std;

namespace HT
{
    HTHANDLE::HTHANDLE():
    Capacity(0),
    SecSnapshotInterval(0),
    MaxKeyLength(0),
    MaxPayloadLength(0),
    File(NULL),
    FileMapping(NULL),
    Addr(NULL),
    lastsnaptime(0)
    {

    }
    HTHANDLE::HTHANDLE(int Capacity,int SecSnapshotInterval,int MaxKeyLength,int MaxPayloadLength,const char FileName[512]):
    Capacity(Capacity),SecSnapshotInterval(SecSnapshotInterval),MaxKeyLength(MaxKeyLength),MaxPayloadLength(MaxPayloadLength),
    File(NULL),FileMapping(NULL),Addr(NULL)
    {
        strcpy_s(this->FileName,512,FileName);
    }

    Element::Element():
    key(nullptr),
    keylength(0),
    payload(nullptr),
    payloadlength(0)
    {}
    Element::Element(const void* key,int keylength):
    key(key),
    keylength(keylength),
    payload(nullptr),
    payloadlength(0)
    {}
    Element::Element(const void* key, int keylength,const void* payload,int payloadlength):
    key(key),
    keylength(keylength),
    payload(payload),
    payloadlength(payloadlength)
    {}
    Element::Element(Element* oldelement,const void* newpayload, int newpayloadlength):
    key(oldelement->key),
    keylength(oldelement->keylength),
    payload(newpayload),
    payloadlength(newpayloadlength)
    {}

    HTHANDLE* Create(int Capacity,int SecSnapshotInterval, int MaxKeyLength, int MaxPayloadLength, const char FileName[512])
    {
        HTHANDLE* ht = new HTHANDLE(Capacity,SecSnapshotInterval,MaxKeyLength,MaxPayloadLength,FileName);

        ht->File = CreateFileA(FileName,GENERIC_READ | GENERIC_WRITE,
        0,NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
        if(ht->File == INVALID_HANDLE_VALUE)
        {
            cout << "Create: file creation failed. " << GetLastError(ht) << "\n";
            delete ht;
            return NULL;
        }
        cout << "Create: file created" << "\n";

        int elementSize = ht->MaxKeyLength+ht->MaxPayloadLength;
        int dwMaxSizeLow = elementSize*ht->Capacity;

        ht->FileMapping = CreateFileMappingA(ht->File,NULL,
        PAGE_READWRITE,0,dwMaxSizeLow,"htFileMapping");

        if(ht->FileMapping == NULL || ht->FileMapping == INVALID_HANDLE_VALUE)
        {
            cout << "Create: FileMapping error "<< GetLastError(ht) << "\n";
            delete ht;
            return NULL;
        }

        ht->Addr = MapViewOfFile(ht->FileMapping,FILE_MAP_ALL_ACCESS,0,0,0);

        if(ht->Addr == NULL || ht->Addr == INVALID_HANDLE_VALUE)
        {
            cout << "Create: MapViewOfFile error " << GetLastError(ht) << "\n";
            delete ht;
            return NULL;
        }
        return ht;
    }
    HTHANDLE* Open(const char FileName[512])
    {
        HTHANDLE* ht = new HTHANDLE();

        ht->File = CreateFileA(FileName,GENERIC_READ | GENERIC_WRITE,
        0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
        if(ht->File == INVALID_HANDLE_VALUE)
        {
            cout << "Open: file creation failed. " << GetLastError(ht) << "\n";
            delete ht;
            return NULL;
        }
        cout << "Open: file created" << "\n";

        int elementSize = ht->MaxKeyLength+ht->MaxPayloadLength;
        int dwMaxSizeLow = elementSize*ht->Capacity;

        ht->FileMapping = CreateFileMappingA(ht->File,NULL,
        PAGE_READWRITE,0,dwMaxSizeLow,"htFileMapping");

        if(ht->FileMapping == NULL || ht->FileMapping == INVALID_HANDLE_VALUE)
        {
            cout << "Open: FileMapping error "<< GetLastError(ht) << "\n";
            delete ht;
            return NULL;
        }

        ht->Addr = MapViewOfFile(ht->FileMapping,FILE_MAP_ALL_ACCESS,0,0,0);

        if(ht->Addr == NULL || ht->Addr == INVALID_HANDLE_VALUE)
        {
            cout << "Open: MapViewOfFile error " << GetLastError(ht) << "\n";
            delete ht;
            return NULL;
        }
        return ht;

    }
    BOOL Snap(const HTHANDLE* hthandle)
    {
        if(hthandle == NULL)
        {
            cout << "Snap: hthandle is null " << "\n";
            return 0;
        }
        


    }
}