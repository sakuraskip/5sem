#include <iostream>
#include <windows.h>
#include <string.h>
using namespace std;

int main(int argc, char** argv)
{
    HWND window =  GetConsoleWindow();
   char title[BUFSIZ]  = {0};
   GetConsoleTitleA(title,BUFSIZ);

   cout << "text: " << title << endl;
   Sleep(5000);
    
}