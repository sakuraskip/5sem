#include <windows.h>
#include <stdio.h>

int main()
{
    SYSTEMTIME systemtime_local, systemtime_system;
    

    GetLocalTime(&systemtime_local);
    GetSystemTime(&systemtime_system);
    
    

    int diff = systemtime_local.wHour - systemtime_system.wHour;

    if(diff < 0)
    {
        printf("%04d-%02d-%02dT%02d:%02d:%02d%-03d\n",
           systemtime_local.wYear,
           systemtime_local.wMonth,
           systemtime_local.wDay,
           systemtime_local.wHour,
           systemtime_local.wMinute,
           systemtime_local.wSecond,
           diff);
    }
    else
    {
         printf("%04d-%02d-%02dT%02d:%02d:%02d%+03d\n",
           systemtime_local.wYear,
           systemtime_local.wMonth,
           systemtime_local.wDay,
           systemtime_local.wHour,
           systemtime_local.wMinute,
           systemtime_local.wSecond,
           diff);
    }

}