#include<iostream>
#include <unistd.h>
#include <windows.h> 
#include<string>
#include<thread>
#include<mutex>
using namespace std;
mutex mt;
#define FIFO_FILE "MYFIFO"
class SERVER{
    private:
    HANDLE hPipe;
    char buffer[1024];
    char sample[50];
    DWORD dwRead,dwWritten;
    int option = 0;
    bool check = TRUE;

    public:

    SERVER(string a,string b,string c,string d)
    {
        hPipe = CreateNamedPipe(TEXT("\\\\.\\pipe\\MYFIFO"),PIPE_ACCESS_DUPLEX, PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT, 1, 1024 * 16, 1024 * 16, NMPWAIT_USE_DEFAULT_WAIT, NULL);
    }

    bool checkpipe()
    {
        return hPipe;
    }
    
    bool checkconnection()
    {
        return ConnectNamedPipe(hPipe, NULL);    
    }
    void* checkread()
    {
        // mt.lock();
        // sleep(1);
        ReadFile(hPipe, buffer, sizeof(buffer) - 1, &dwRead, NULL);
        buffer[dwRead] = '\0';
        cout<<"\nThe received message : "<<buffer<<endl;
        return 0;
        //mt.unlock();
    }
    void* checkwrite()
    {
        // mt.lock();
        cout<<"enter your message   : ";
        cin>>sample;
        WriteFile(hPipe,sample,sizeof(sample), &dwWritten, NULL);
        return 0;
        // sleep(1);
        // mt.unlock();
    }
    int checksample()
    {
        return strcmp(sample,"end");
    }
    void closehandle()
    {
        CloseHandle(hPipe);
    }
    void connection()
    {
        DisconnectNamedPipe(hPipe);
    }

};
int main(void)
{   int option =0;
    string buffer;
    SERVER s("PIPE_ACCESS_DUPLEX","PIPE_TYPE_BYTE", "PIPE_READMODE_BYTE", "PIPE_WAIT");
           if(s.checkpipe() != 0 )//&& sleep(10) && ConnectNamedPipe(hPipe, NULL) != FALSE
    {
            for(int i=0;i<5;i++)
            {
                if(s.checkconnection() == TRUE)
                {
                    cout<<"server is active\n";
                    break;
                }
                sleep(1000);
                cout<<i<<endl;
            }  
            while (1)
            {   
                thread pt1 (&SERVER::checkread,&s);
                thread pt (&SERVER::checkwrite,&s);
                pt1.join();
                pt.join();

                // if(option == 0)
                // {
                //     s.checkread();
                //     option = 1;
                // }
                // if(option == 1)
                // {
                //     s.checkwrite();
                //     option = 0;
                // }
                // if(s.checksample()==0)
                // {   
                //     break;
                // }
            }
            
    }
        cout<<"The connection is broken.\n";
        s.closehandle();
        s.connection();
    return 0;
}