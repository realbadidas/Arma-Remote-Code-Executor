#include <string>
#include "server.h"
#include <windows.h>
#include <thread>
#include <iostream>
#include <boost/thread.hpp>
#include "RSJparserD.tcc"


using namespace std;
bool initialized = false;

std::ifstream mstrm("@sqfremoteexecute\\config.json");
BETRSJREsource JSON(mstrm);


BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)

{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		if (JSON["consoleWindow"].as<bool>(true)) {

			AllocConsole();
			freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);
			cout << "Started Extension SQFExecutor & successfully hooked | Compiled: " << __DATE__ << " | " << __TIME__ << endl;
			cout << "To hide this window, edit the config and disable ConsoleWindow." << endl;
			cout << "Made with love from badidas" << endl;
			cout << "Github Repo: https://github.com/realbadidas/Arma-Remote-Code-Executor \r\n" << endl;
		}

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
		if (!initialized) {
			init();
			initialized = true;
			output = "OK";
		}
		else {
			output = "ERR";
		}

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