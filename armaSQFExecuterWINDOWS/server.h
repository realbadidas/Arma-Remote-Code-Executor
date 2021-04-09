#pragma once
#include <string>



void startServer();
void stopServer();


std::string sendCommand();
void setResponse(std::string str);

//std::string returnBuffer;
//std::string commandBuffer;