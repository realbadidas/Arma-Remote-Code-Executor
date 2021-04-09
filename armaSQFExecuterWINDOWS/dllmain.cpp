#include <windows.h>
#include <string>
#include "server.h"
#include <thread>
#include <iostream>
#include <boost/thread.hpp>

using namespace std;



BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)

{



	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		AllocConsole();
		freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);
		cout << "Started Extension: SQFExecutor | Compiled: " << __DATE__ << " | " << __TIME__ << endl;
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

void init() {
	boost::thread thd(startServer);

}



extern "C"
{
	__declspec(dllexport) void __stdcall RVExtension(char* output, int outputSize, const char* function);
};

void __stdcall RVExtension(char* container, int outputSize, const char* function)
{
	outputSize -= 1;
	std::string input = function;
	std::string output = "NO";
	size_t io = atoi(input.substr(0, 1).c_str());
	input = input.substr(1);
	boost::thread thd;
	switch (io)
	{
	case 0: // Init
		output = "OK";
		break;
	case 1: // Test
		output = input;
		break;
	case 2:
		init();
		output = "OK";
		break;
	case 3:
		stopServer();
		break;
	case 4: //Arma Server instantly responds with an answer if you sent the server a command"
		setResponse(input);
		output = "OK";
		break;
	case 5: //Arma Server queries this in a constant loop
		output = sendCommand();
		break;
	}


	strncpy_s(container, outputSize + 1, output.c_str(), outputSize);

}