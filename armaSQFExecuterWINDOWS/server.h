#pragma once

#include <string>
#ifndef crow_h
	#define CROW_ENABLE_SSL
	#include "crow.h"
	#define crow_h
#endif

void startServer();
void stopServer();


std::string sendCommand();
void setResponse(std::string str);
std::string clearBufferAndReturnOld(std::string* buffer);
crow::json::wvalue finishUp(bool authenticated, std::string userPassCombo, std::string execute, int returnCode, std::string returnStr);
//std::string returnBuffer;
//std::string commandBuffer;