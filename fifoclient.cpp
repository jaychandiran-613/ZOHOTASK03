/*#include <iostream>#include <windows.h > 
#include <string.h>
using namespace std;

#define FIFO_FILE "MYFIFO"
int main(void)
{
    HANDLE hPipe;
    DWORD dwWritten,dwRead;
    char sample[50],buffer[1024];
    int option = 0;

    for(int i=0;i < 10;i++)
    {
    	hPipe = CreateFile(TEXT("\\\\.\\pipe\\MYFIFO"), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
        if(hPipe!=INVALID_HANDLE_VALUE)
        {
            break;
        }

        Sleep(1000);
    }

    if (hPipe != INVALID_HANDLE_VALUE)
    {
        while(1)
        {
        if(option == 0)
        {
        	cout<<"enter your message   : ";
            cin>>sample;
            WriteFile(hPipe, sample,sizeof(sample), &dwWritten, NULL);  
            option = 1;  
        }

        if(strcmp(sample,"end")==0)
        {
            break;
        }

        if(ReadFile(hPipe, buffer, sizeof(buffer) - 1, &dwRead, NULL) != FALSE && option == 1)
        {
            buffer[dwRead] = '\0';
            cout<<"The received message : "<<buffer<<endl;
            option = 0;
        }
        }
    }

    cout<<"The connection is broken.\n";
    CloseHandle(hPipe);
    return (0);
}

*/
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
		return strcmp(sample, "end");
	}

	void *read()
	{
        while(1)
        {
            
	    	if(1 == 1)
            {
				ReadFile(hPipe, buffer, sizeof(buffer) - 1, &dwRead, NULL);
				//mt.lock();
                buffer[dwRead] = '\0';
	        	cout << "The received message : " << buffer << endl;
				sleep(1);
		        //mt.unlock();
            }
	    	
        }

	}

	void closeconnection()
	{
		CloseHandle(hPipe);
	}
};
int main()
{
	CLIENT c;
	// for (int i = 0; i < 10; i++)
	// {
	// 	hPipe = CreateFile(TEXT("\\\\.\\pipe\\MYFIFO"), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
	// 	if (hPipe != INVALID_HANDLE_VALUE)
	// 	{
	// 		break;
	// 	}

	// 	Sleep(1000);
	// }

	c.createpipe();
	if (c.checkfile() != 0)	//hPipe != INVALID_HANDLE_VALUE
	{
			thread pt2 (&CLIENT::write, &c);
			thread pt3 (&CLIENT::read, &c);
			pt2.join();
			pt3.join();
	}
	cout << "The connection is broken.\n";
	c.closeconnection();
	return (0);
}