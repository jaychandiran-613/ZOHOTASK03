#include<iostream>
#include <windows.h> 
#include<string.h>
using namespace std;
#define FIFO_FILE "MYFIFO"
int main(void)
{
    HANDLE hPipe;
    DWORD dwWritten,dwRead;
    char sample[50],buffer[1024];
    int option = 0;



    hPipe = CreateFile(TEXT("\\\\.\\pipe\\MYFIFO"), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
    if (hPipe != INVALID_HANDLE_VALUE)
    {
        while(1)
        {
        if(option == 0)
        {   cout<<"enter your message   : ";
            cin>>sample;
            WriteFile(hPipe, sample,sizeof(sample), &dwWritten, NULL);  
            option = 1;  
        }
        if(ReadFile(hPipe, buffer, sizeof(buffer) - 1, &dwRead, NULL) != FALSE && option == 1)
        {
            buffer[dwRead] = '\0';
            cout<<"The received message : "<<buffer<<endl;
            option = 0;
        }
        }

    }
    
    CloseHandle(hPipe);
    return (0);
}