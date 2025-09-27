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
    HTHANDLE::HTHANDLE(int Capacity,int SecSnapshotInterval,int MaxKeyLength,int MaxPayloadLength,const char FileName[512],int elementCount):
    Capacity(Capacity),SecSnapshotInterval(SecSnapshotInterval),MaxKeyLength(MaxKeyLength),MaxPayloadLength(MaxPayloadLength),
    File(NULL),FileMapping(NULL),Addr(NULL),elementCount(elementCount)
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
        HTHANDLE* ht = new HTHANDLE(Capacity,SecSnapshotInterval,MaxKeyLength,MaxPayloadLength,FileName,0);

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
    BOOL Snap(HTHANDLE* hthandle)
    {
        if(hthandle == NULL)
        {
            cout << "Snap: hthandle is null " << "\n";
            return 0;
        }
        hthandle->lastsnaptime = time(0);

        HANDLE snapshot = CreateFileA(
            "snapshotHELP.htSNAP",
            GENERIC_READ | GENERIC_WRITE,0,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);

        if(snapshot == INVALID_HANDLE_VALUE)
        {
            cout << "\nSnap: createfileA error"; 
        }

        int elementSize = hthandle->MaxKeyLength + hthandle->MaxPayloadLength;
        int HTsize = hthandle->elementCount * elementSize;
        DWORD writtenBytesAmount =0;
        int writeResult = WriteFile(
            snapshot,
            hthandle->Addr,
            (DWORD)HTsize,
            &writtenBytesAmount,
            NULL
        );
        CloseHandle(snapshot);
        if(!writeResult || writtenBytesAmount != HTsize)
        {
            cout << "\nwrittenBytesAmount: " + to_string(writtenBytesAmount) + "\nHTsize: "
            + to_string(HTsize) + "snap: write ERROR";
            return false;
        }
        cout << "\nsnapshot created";
        return TRUE;
        


    }
    BOOL Close(HTHANDLE* hthandle)
    {
        if(hthandle == NULL)
        {
            cout << "\nClose: hthandle is null";
            return false;
        }
        bool closeResult = true;

        if(hthandle->Addr != NULL)
        {
            if(UnmapViewOfFile(hthandle->Addr) == false)
            {
                closeResult = false;
            }
            hthandle->Addr = NULL;
        }

        if(hthandle->FileMapping != NULL && hthandle->FileMapping != INVALID_HANDLE_VALUE)
        {
            if(CloseHandle(hthandle->FileMapping) == false)
            {
                closeResult = false;
            }
            hthandle->FileMapping = NULL;
        }

        if(hthandle->File !=NULL && hthandle->File!=INVALID_HANDLE_VALUE)
        {
            if(CloseHandle(hthandle->File) == false)
            {
                closeResult=false;
            }
            hthandle->File = NULL;
        }
        cout << "\nClosed hthandle";
        return closeResult;
    }

    BOOL Insert(HTHANDLE* hthandle,const Element* element)
    {
        bool closeResult = true;
        if(hthandle == NULL || element == NULL || hthandle->Addr == NULL || hthandle->elementCount >=hthandle->Capacity)
        {
            cout << "\nInsert: something is null";
                       return false;

        }
        if(element->keylength > hthandle->MaxKeyLength || element->payloadlength > hthandle->MaxPayloadLength)
        {
            cout << "\nInsert: key/payload length > maxlength";
           return false;
        }
        int newIndex = hthandle->elementCount;

        int emptyItemSize = hthandle->MaxKeyLength + hthandle->MaxPayloadLength;
        char* freeSlotStart = (char*)hthandle->Addr+(hthandle->elementCount*emptyItemSize);

        if(element->keylength>0 && element->payloadlength>0)
        {
            memcpy(freeSlotStart,element->key,element->keylength);
            memset(freeSlotStart+element->keylength,0,hthandle->MaxKeyLength-element->keylength);//очистка памяти для следующего элемента
            
            memcpy(freeSlotStart+hthandle->MaxKeyLength,element->payload,element->payloadlength);
            memset(freeSlotStart+hthandle->MaxKeyLength+element->payloadlength,0,hthandle->MaxPayloadLength-element->payloadlength);
        }
        else
        {
            cout << "\nInsert: key/payload length are null";
                      return false;

        }
        
        hthandle->elementCount++;
        cout << "\nInsert completed";
        return true;
    }
    BOOL Delete(HTHANDLE* hthandle, const Element* element)
    {
        if(hthandle == NULL)
        {
            cout << "\nDelete: hthandle is null";
            return false;
        }
        if(hthandle->Addr == NULL)
        {
            cout << "\nDelete: addr is null";
            return false;
        }
        if(element == NULL || element->keylength == NULL || element->payloadlength == NULL)
        {
            cout <<"\nDelete: element is null";
            return FALSE;
        }
        bool deleteFlag = false;
        int deleteIndex = 0;
        int emptyItemSize = hthandle->MaxKeyLength + hthandle->MaxPayloadLength;

        for(int i=0;i<hthandle->elementCount;i++)
        {
            char* Start = (char*)hthandle->Addr+(i*emptyItemSize);

            if(memcmp(Start,element->key,element->keylength) == 0)
            {
                deleteIndex = i;
                deleteFlag = true;
                break;
            }
        }

        if(!deleteFlag)
        {
            cout << "\nDelete: item not found";
            return false;
        }

        if(deleteIndex != hthandle->elementCount-1)
        {
            char* lastElement = (char*)hthandle->Addr + (hthandle->elementCount-1)*emptyItemSize;
            char* deleteItem = (char*)hthandle->Addr+(deleteIndex*emptyItemSize);

            memcpy(deleteItem,lastElement,emptyItemSize);
        }
        hthandle->elementCount--;

        cout << "\nDelete: item deleted";
        return true;
    }
    Element* Get(const HTHANDLE* hthandle, const Element* element)
    {
       if(hthandle == NULL)
        {
            cout << "\nDelete: hthandle is null";
            return NULL;
        }
        if(hthandle->Addr == NULL)
        {
            cout << "\nDelete: addr is null";
            return NULL;
        }
        if(element == NULL || element->keylength == NULL || element->payloadlength == NULL)
        {
            cout <<"\nDelete: element is null";
            return NULL;
        }
        int emptyItemSize = hthandle->MaxKeyLength + hthandle->MaxPayloadLength;

        for(int i=0;i<hthandle->elementCount;i++)
        {
            char* Start = (char*)hthandle->Addr+(i*emptyItemSize);

            if(memcmp(Start,element->key,element->keylength)==0)
            {
                Element* searched = new Element(Start,element->keylength,Start+hthandle->MaxKeyLength,hthandle->MaxPayloadLength);
                return searched;
            }

        }
        cout << "\nNotFOUND";
        return NULL;
    }
    BOOL Update(const HTHANDLE* hthandle,           
	const Element*  oldelement,         
	const void*     newpayload,           
	int             newpayloadlength )
    {
         if(hthandle == NULL)
        {
            cout << "\nUpdate: hthandle is null";
            return false;
        }
        if(hthandle->Addr == NULL)
        {
            cout << "\nUpdate: addr is null";
            return false;
        }
        if(oldelement == NULL || oldelement->keylength == NULL || oldelement->payloadlength == NULL)
        {
            cout <<"\nUpdate: element is null";
            return false;
        }
        if(newpayload == NULL || newpayloadlength == NULL || newpayloadlength > hthandle->MaxPayloadLength)
        {
            cout <<"\nUpddate: newpayload is null";
            return false;
        }
        int emptyItemSize = hthandle->MaxKeyLength + hthandle->MaxPayloadLength;

        for(int i=0;i<hthandle->elementCount;i++)
        {
            char* Start = (char*)hthandle->Addr+(i*emptyItemSize);
            if(memcmp(Start,oldelement->key,oldelement->keylength)==0)
            {
                memcpy(Start+hthandle->MaxKeyLength,newpayload,newpayloadlength);
                cout << "\nUpdate: element updated";
                return true;
            }
        }
        cout <<"\nUpdate: FAILED";
        return false;
    }
}