#include <iostream>
#include <unistd.h>
#include <windows.h>
#include <string>
#include <thread>
#include <mutex>
using namespace std;
#define FIFO_FILE "MYFIFO"
class SERVER
{
	private:
	mutex mt;
	HANDLE hPipe;
	char buffer[1024];
	char sample[50];
	DWORD dwRead, dwWritten;
	int option = 0;
	bool check = TRUE;

	public:

	SERVER(DWORD dwOpenMode, DWORD dwPipeMode1, DWORD dwPipeMode2,DWORD dwPipeMode3, DWORD nMaxInstances, DWORD nOutBufferSize, DWORD nInBufferSize, DWORD nDefaultTimeOut)
		{
			hPipe = CreateNamedPipe(TEXT("\\\\.\\pipe\\MYFIFO"), dwOpenMode, dwPipeMode1 | dwPipeMode2 | dwPipeMode3, nMaxInstances, nOutBufferSize, nInBufferSize, nDefaultTimeOut, NULL);
		}

	bool checkpipe()
	{
		return hPipe;
	}

	bool checkconnection()
	{
		return ConnectNamedPipe(hPipe, NULL);
	}

	void *checkread()
	{
		while (1)
		{
			
			if(1 == 1)
            {
				ReadFile(hPipe, buffer, sizeof(buffer) - 1, &dwRead, NULL);
				//mt.lock();
		    	sleep(1);
                buffer[dwRead] = '\0';
		    	cout << "\nThe received message : " << buffer << endl;
				//mt.unlock();
            }
			
		}
	}

	void checkwrite()
	{   
		while (1)
		{
			cout << "enter your message   : ";
			//mt.lock();
			cin >> sample;

			if(WriteFile(hPipe, sample, sizeof(sample), &dwWritten, NULL) == TRUE)
			{
				cout<<"The message written on the pipe.\n";
			}
			else
			{
				cout<<"The message unable to write on pipe.\n";
			}

			if(checksample() == 0)
			{
				break;
			}

			sleep(1);
			//mt.unlock();
		}
	}

	int checksample()
	{
		return strcmp(sample, "end");
	}

	int closehandle()
	{
		return CloseHandle(hPipe);
	}

	int connection()
	{
		return DisconnectNamedPipe(hPipe);
	}

};
int main(void)
{
	
	SERVER s(PIPE_ACCESS_DUPLEX, PIPE_TYPE_BYTE, PIPE_READMODE_BYTE, PIPE_WAIT, 1, 1024 * 16, 1024 * 16, NMPWAIT_USE_DEFAULT_WAIT);
	if(s.checkpipe() != 0)
	{   cout<<"The namedpipe is created successfully.\n";
		if(s.checkconnection() == TRUE)
		{
			cout<<"The Client is connected successfully.\n";
			//thread pt1(&SERVER::checkread, &s);
	    	thread pt(&SERVER::checkwrite, &s);
		    //pt1.join();
		    pt.join();
		}
		else
		{
			cout<<"Unable to connect to the client.\n";
			return 0;
		}
	}
	else
	{
		cout<<"Unable to Create the pipe\n";
		return 0;
	}
    
	cout << "The connection is broken.\n";
	
	if(s.connection() == 0)
	{   cout<<GetLastError();
		cout<<"Unable to disconnect the namedpipe.\n";
	}
	else
	{
		cout<<"The namedpipe disconnected successfull.\n";
	}
	
	if(s.closehandle() == 0)
	{
		cout<<"Unable to close an open object handle.\n";
	}
	else
	{
		cout<<"Closes an open object handle.\n";
	}
	
	return 0;
}