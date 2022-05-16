#include <iostream>
#include <unistd.h>
#include <windows.h>
#include <string>
#include <thread>
#include <mutex>
using namespace std;

#define FIFO_FILE "MYFIFO"
class CLIENT
{
	private:
	mutex mt;
	HANDLE hPipe;
	DWORD dwWritten, dwRead;
	char buffer[1024], sample[50];
	public:
		
	static int check;
	CLIENT(LPCSTR lpName, DWORD dwDesiredAccess1, DWORD dwDesiredAccess2, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile)
	{
		int count = 0;
		hPipe = CreateFile(lpName, dwDesiredAccess1 | dwDesiredAccess2, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
		while (hPipe == INVALID_HANDLE_VALUE)
		{
			hPipe = CreateFile(lpName, dwDesiredAccess1 | dwDesiredAccess2, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
			if (count++ == 5)
			{
				break;
			}

			sleep(1);
		}
	}

	HANDLE checkfile()
	{
		return hPipe;
	}

	void write()
	{
		while (1)
		{
			//cout << "enter your message   : ";
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
		}
	}

	int checksample()
	{
		return strcmp(buffer, "end");
	}

	void read()
	{
		while (1)
		{
			if (ReadFile(hPipe, buffer, sizeof(buffer) - 1, &dwRead, NULL) == FALSE)
			{
				cout << "\nThe message is not received on the pipe.\n";
				cout << "Server connection might went off.\n";
				check = 0;
				abort();
				break;
			}
			else
			{
				cout << "\nThe message is received on pipe.\n";
				//mt.lock();
				buffer[dwRead] = '\0';
				cout << "The received message : " << buffer << "\n\n";
				sleep(1);
				//mt.unlock();
			}
		}
	}

	int closeconnection()
	{
		return CloseHandle(hPipe);
	}
};
int CLIENT::check = 1;
int main()
{
	LPCSTR filename = TEXT("\\\\.\\pipe\\MYFIFO");
	LPCSTR filename1 = TEXT("\\\\.\\pipe\\MYFIFO1");

	CLIENT client1(filename, GENERIC_READ, GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
	CLIENT client2(filename1, GENERIC_READ, GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);

	int count = 0;

	if (client1.checkfile() != INVALID_HANDLE_VALUE && client2.checkfile() != INVALID_HANDLE_VALUE)
	{
		cout << "The Server end is connected successfully.\n\n";

		thread pt2(&CLIENT::write, &client2);
		thread pt3(&CLIENT::read, &client1);

		pt2.join();
		pt3.join();
	}
	else
	{
		cout << "Unable to create the name of the file.\n";
		return 0;
	}

	cout << "The connection is broken.\n";

	if (client1.closeconnection() != 0 && client2.closeconnection() != 0)
	{
		cout << "Closes an open object handle.\n\n";
	}
	else
	{
		cout << "Unable to close an open object handle.\n";
	}

	return 0;
}