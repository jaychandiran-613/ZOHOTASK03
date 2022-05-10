#include<iostream>
#include <windows.h> 
#include<string>
using namespace std;
#define FIFO_FILE "MYFIFO"
int main(void)
{
    HANDLE hPipe;
    char buffer[1024];
    DWORD dwRead,dwWritten;
    int option = 0;
    char sample[50];

    hPipe = CreateNamedPipe(TEXT("\\\\.\\pipe\\MYFIFO"),PIPE_ACCESS_DUPLEX, PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT, 1, 1024 * 16, 1024 * 16, NMPWAIT_USE_DEFAULT_WAIT, NULL);
    while (hPipe != INVALID_HANDLE_VALUE)
    {
        if (ConnectNamedPipe(hPipe, NULL) != FALSE)   // wait for someone to connect to the pipe
        {
            while (1)
            {
                if(ReadFile(hPipe, buffer, sizeof(buffer) - 1, &dwRead, NULL) != FALSE && option == 0)
                {
                    buffer[dwRead] = '\0';
                    cout<<"The received message : "<<buffer<<endl;
                    option = 1;
                }

                if(option == 1)
                {
                    cout<<"enter your message   : ";
                    cin>>sample;
                    WriteFile(hPipe,sample,sizeof(sample), &dwWritten, NULL);
                    option = 0;
                }
            }
        }

        DisconnectNamedPipe(hPipe);
    }

    return 0;
}