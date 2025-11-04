#pragma once

#include <Windows.h>
#include <mutex>
#include <iostream>
#include <cstring>
#include <thread>
#include <string>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

#ifdef _WIN32
#ifdef HT_EXPORTS
#define HT_API __declspec(dllexport)
#else
#define HT_API __declspec(dllimport)
#endif
#else
#define HT_API
#endif

namespace HT    // HT API
{

    struct HT_API HTHANDLE
    {
        HTHANDLE();
        HTHANDLE(int Capacity, int SecSnapshotInterval, int MaxKeyLength, int MaxPayloadLength, const char FileName[512], int elementCount);
        int     Capacity;
        int     elementCount;             // текущий элемент в таблице
        int     SecSnapshotInterval;    // интервал снапшота в сек. 
        int     MaxKeyLength;           // максимальная длина ключа
        int     MaxPayloadLength;       // максимальная длина значения
        char    FileName[512];          // имя файла 
        HANDLE  File;                   // File HANDLE != 0, если файл открыт
        HANDLE  FileMapping;            // Mapping File HANDLE != 0, если mapping открыт  
        LPVOID  Addr;                   // Addr != NULL, если mapview получен  
        char    LastErrorMessage[512];  // сообщение об ошибке для 0x00  
        time_t  lastsnaptime;           // время последнего snap (time()) 
        BYTE* DataAddr;
    };

    struct HT_API Element  
    {
        Element();
        Element(const void* key, int keylength);                                            
        Element(const void* key, int keylength, const void* payload, int  payloadlength);    
        Element(Element* oldelement, const void* newpayload, int  newpayloadlength);     
        const void* key;                 // ключ
        int             keylength;           // длина ключа
        const void* payload;             // данные
        int             payloadlength;       // длина данных
    };

    extern HT_API HANDLE globalMutex;


    HT_API bool InitMutex();

    HT_API HTHANDLE* Create   //  создание HT             
    (
        int   Capacity,                     // размер хранилища
        int   SecSnapshotInterval,          // интервал снапшота в сек.
        int   MaxKeyLength,                 // максимальная длина ключа
        int   MaxPayloadLength,             // максимальная длина значения
        const char  FileName[512],
        int elementCount          // число элементов
    );  // != NULL - успешное создание  

    HT_API HTHANDLE* Open     //  открытие HT             
    (
        const char    FileName[512]         // имя файла 
    );  // != NULL - успешное открытие  

    HT_API BOOL Snap         // создание Snapshot
    (
        HTHANDLE* hthandle           // указатель на HT (File, FileMapping)
    );

    HT_API BOOL Close        // Snap и закрытие HT и освобождение HTHANDLE
    (
        HTHANDLE* hthandle           // указатель на HT (File, FileMapping)
    );  // == TRUE - успешное закрытие   

    HT_API BOOL Insert      // вставка элемента в хранилище
    (
        HTHANDLE* hthandle,            // указатель на HT
        const Element* element              // элемент
    );  // == TRUE - успешная вставка 

    HT_API BOOL Delete      // удаление элемента из хранилища
    (
        HTHANDLE* hthandle,            // указатель на HT (файл)
        const Element* element              // элемент 
    );  // == TRUE - успешное удаление 

    HT_API Element* Get     // получение элемента из хранилища
    (
        HTHANDLE* hthandle,            // указатель на HT
        const Element* element              // элемент 
    );  // != NULL - успешное получение 

    HT_API BOOL Update     // обновление элемента в хранилище
    (
        HTHANDLE* hthandle,            // указатель на HT
        const Element* oldelement,          // старый элемент (ключ, старое значение)
        const void* newpayload,          // новые данные  
        int             newpayloadlength     // длина новых данных
    );  // != NULL - успешное обновление 

    HT_API extern HANDLE globalMutex;

    HT_API void print                               // печать элемента 
    (
        const Element* element              // элемент 
    );

}