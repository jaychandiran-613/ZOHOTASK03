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
		DWORD dwWritten,dwRead;
		char buffer[1024], sample[50];
	public:
	void createpipe()
		{
			hPipe = CreateFile(TEXT("\\\\.\\pipe\\MYFIFO"), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
		}

	bool checkfile()
	{
		return hPipe;
	}

	void *write()
	{
        cout << "enter your message   : ";
		while(1)
        {
            //mt.lock();
	    	sleep(1);
		    cin >> sample;
		    WriteFile(hPipe, sample, sizeof(sample), &dwWritten, NULL);
		    //mt.unlock();
        }
	}

	int checksample()
	{
		return strcmp(buffer, "end");
	}

	void read()
	{
        while(1)
        {
            
	    	if(ReadFile(hPipe, buffer, sizeof(buffer) - 1, &dwRead, NULL) == TRUE)
            {   cout<<"The message is received on pipe.\n";
				//mt.lock();
                buffer[dwRead] = '\0';
	        	cout << "The received message : " << buffer << endl;
				sleep(1);
		        //mt.unlock();
            }
			else
			{
				cout<<"The message is not received on the pipe.\n";
			}
			if(checksample() == 0)
			{
				break;
			}
	    	
        }

	}

	int closeconnection()
	{
		return CloseHandle(hPipe);
	}
};
int main()
{
	// for (int i = 0; i < 10; i++)
	// {
	// 	hPipe = CreateFile(TEXT("\\\\.\\pipe\\MYFIFO"), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
	// 	if (hPipe != INVALID_HANDLE_VALUE)
	// 	{
	// 		break;
	// 	}
	// 	Sleep(1000);
	// }
    CLIENT c;
	c.createpipe();
	if (c.checkfile() != 0)	
	{
        cout<<"The Server end is connected successfully.\n";
		//thread pt2 (&CLIENT::write, &c);
		thread pt3 (&CLIENT::read, &c);
		//pt2.join();
		pt3.join();
	}
	else
	{
		cout<<"Unable to create the name of the file.\n";
		return 0;
	}

	cout << "The connection is broken.\n";

	if(c.closeconnection() == 0)
	{
		cout<<"Unable to close an open object handle.\n";
	}
	else
	{
		cout<<"Closes an open object handle.\n";
	}

	return 0;
}