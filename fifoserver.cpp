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
	string a;

	public:
		static int check;

	SERVER(LPCSTR lpName, DWORD dwOpenMode, DWORD dwPipeMode1, DWORD dwPipeMode2, DWORD dwPipeMode3, DWORD nMaxInstances, DWORD nOutBufferSize, DWORD nInBufferSize, DWORD nDefaultTimeOut)
	{
		hPipe = CreateNamedPipe(lpName, dwOpenMode, dwPipeMode1 | dwPipeMode2 | dwPipeMode3, nMaxInstances, nOutBufferSize, nInBufferSize, nDefaultTimeOut, NULL);
	}

	bool checkpipe()
	{
		return hPipe;
	}

	bool checkconnection()
	{
		return ConnectNamedPipe(hPipe, NULL);
	}

	void checkread()
	{
		while (1)
		{
			//	//char a[4]="p↨\0";
			// strncpy(buffer,a,256);
			bool check = ReadFile(hPipe, buffer, sizeof(buffer) - 1, &dwRead, NULL);

			if (check == FALSE)	// buffer.find("p↨") != string::npos ||
			{
				cout << "\nThe message is not received on the pipe.\n";
				cout << "Server connection might went off.\n";
				check = 0;
				break;
			}

			else if (check == TRUE)
			{
				cout << "\nThe message is received on pipe.\n";
				//mt.lock();
				buffer[dwRead] = '\0';
				cout << "The received message : " << buffer << "\n\n";
				sleep(1);
				//mt.unlock();

			}

			// if(checksample() == 0)
			// { 	terminate();
			// 	break;
			// }
		}
	}

	void checkwrite()
	{
		while (1)
		{
			// cout << "enter your message   : ";
			//mt.lock();
			// if(check == 0)
			// {
			// 	cin.ignore();
			// 	break;
			// }

			cin >> sample;

			if (WriteFile(hPipe, sample, sizeof(sample), &dwWritten, NULL) == TRUE)
			{
				cout << "The message written on the pipe.\n\n";
			}
			else
			{
				cout << "The message unable to write on pipe.\n";
				break;
			}

			if (checksample() == 0)
			{
				cout << "\nThe program stopped.\n";
				abort();
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

	int disconnection()
	{
		return DisconnectNamedPipe(hPipe);
	}
};
int SERVER::check = 1;
int main(void)
{
	LPCSTR filename = TEXT("\\\\.\\pipe\\MYFIFO");
	LPCSTR filename1 = TEXT("\\\\.\\pipe\\MYFIFO1");

	SERVER server1(filename, PIPE_ACCESS_DUPLEX, PIPE_TYPE_BYTE, PIPE_READMODE_BYTE, PIPE_WAIT, 1, 1024 * 16, 1024 * 16, NMPWAIT_USE_DEFAULT_WAIT);
	SERVER server2(filename1, PIPE_ACCESS_DUPLEX, PIPE_TYPE_BYTE, PIPE_READMODE_BYTE, PIPE_WAIT, 1, 1024 * 16, 1024 * 16, NMPWAIT_USE_DEFAULT_WAIT);
	
	if (server1.checkpipe() != 0 && server2.checkpipe() != 0)
	{
		cout << "The namedpipe is created successfully.\n";
		if (server1.checkconnection() == TRUE && server2.checkconnection() == TRUE)
		{
			cout << "The Client is connected successfully.\n\n";
			thread pt1(&SERVER::checkread, &server2);
			thread pt(&SERVER::checkwrite, &server1);
			pt1.join();
			pt.join();
		}
		else
		{
			cout << "Unable to connect to the client.\n";
			return 0;
		}
	}
	else
	{
		cout << "Unable to Create the pipe\n";
		return 0;
	}

	cout << "The connection is broken.\n";

	if (server1.disconnection() != 0 && server2.disconnection() != 0)
	{
		cout << "The namedpipe disconnected successfully.\n";
	}
	else
	{
		cout << "Unable to disconnect the namedpipe.\n";
	}

	if (server1.closehandle() != 0 && server2.closehandle() != 0)
	{
		cout << "Closes an open object handle.\n\n";
	}
	else
	{
		cout << "Unable to close an open object handle.\n";
	}

	return 0;
}