
#include "server.h"

//#include "crow.h"
//#include <string>
#include "RSJparser.tcc"
#include "picosha2.h"
#include <iostream>

using namespace std;





std::string returnBuffer = "no-message-yet";
std::string commandBuffer = "no-message-yet";

crow::SimpleApp app;

    std::ifstream my_fstream("@sqfremoteexecute\\config.json");
    RSJresource JSON(my_fstream);


    vector<std::string> splitString(std::string s, char del) {
        std::replace(s.begin(), s.end(), del, ' ');

        vector<std::string> ret;
        stringstream ss(s);

        std::string tmp;





        while (ss >> tmp) {
            ret.push_back(tmp);
        }
        return ret;
    }

    std::string urlDecode(std::string& SRC) {
        std::string ret;
        char ch;
        int i, ii;
        for (i = 0; i < SRC.length(); i++) {
            if (int(SRC[i]) == 37) {
                sscanf_s(SRC.substr(i + 1, 2).c_str(), "%x", &ii);
                ch = static_cast<char>(ii);
                ret += ch;
                i = i + 2;
            }
            else {
                ret += SRC[i];
            }
        }
        return (ret);
    }



    std::string urlParamsParser(std::string URLparams, std::string keyWord) {
        vector<std::string> keyWordsWithValue = splitString(URLparams, '&');
        for (std::string st : keyWordsWithValue) {
            if (st.rfind(keyWord, 0) == 0) {
                return splitString(st, '=')[1];
            }
        }
        return "";
    }

    bool checkIfValidUser(std::string user, std::string pass) {
        //THIS IS SUPER GHETTO BUT WORKS FOR NOW
        for (int i = 0; i <= 1337; i++) {
            if (JSON["users"][i]["user"].as<std::string>("") == user && JSON["users"][i]["pass"].as<std::string>("") == pass && user != "" && pass != "") {
                return true;
            }
        }
        return false;
    }
    std::string sendCommand() {
        std::string ret = commandBuffer;
        commandBuffer = "no-message-yet";
        return ret;
    }

    void setResponse(std::string str) {
        returnBuffer = str;
    }


    void startServer() {


        std::string host = JSON["host"].as<std::string>("192.168.178.66");
        int port = JSON["port"].as<int>(6601);

        std::string SSL_Key = JSON["SSL_Key"].as<std::string>("ssl.key");
        std::string SSL_Cert = JSON["SSL_Cert"].as<std::string>("ssl.cert");


        commandBuffer = "no-message-yet";
       
        
        returnBuffer = "no-message-yet";

        CROW_ROUTE(app, "/send")
            .methods("POST"_method)
            ([](const crow::request& req)
                {
                    std::string str = req.body;

                    std::string user = urlParamsParser(str, "user");
                    std::string password = urlParamsParser(str, "pass");
                    std::string execCommand = urlParamsParser(str, "execute");


                    if (user == "" || password == "" || execCommand == "") {
                        return finishUp(false, "NOTGIVEN", "NOTGIVEN", 7, "No return; CHECK BODY PARAMS");
                    }

                    std::string hash_hex_str;
                    picosha2::hash256_hex_string(password, hash_hex_str);


                    bool isAuthed = checkIfValidUser(user, hash_hex_str);


                    if (!isAuthed) {
                        return finishUp(false, user+"@"+ hash_hex_str, execCommand, 6, "Wrong password or username");

                    }


                        int i = 0;
                        commandBuffer = urlDecode(execCommand);

                        while (returnBuffer == "no-message-yet" && i < 300) { std::this_thread::sleep_for(10ms); i++; }

                        if (returnBuffer == "no-message-yet") {
                            return finishUp(true, user + "@" + hash_hex_str, clearBufferAndReturnOld(&commandBuffer), 5, "NO RESPONSE FROM SCRIPT");
                        }
                        else {
                            return finishUp(true, user + "@" + hash_hex_str, commandBuffer, 0, clearBufferAndReturnOld(&returnBuffer));
                        }
                });

        std::cout << "Webserver started:" << std::endl;
        std::cout << "PORT " << port << std::endl;
        std::cout << "HOST " << host << "\r\n" << std::endl;


        if (JSON["https"].as<bool>(false))
            app.port(port).bindaddr(host).multithreaded().ssl_file(SSL_Cert, SSL_Key).run();
        else
            app.port(port).bindaddr(host).multithreaded().run();
    }
    crow::json::wvalue finishUp(bool authenticated, std::string userPassCombo, std::string execute, int returnCode, std::string returnStr) {
        crow::json::wvalue toReturn;

        toReturn["code"] = returnCode;
        toReturn["return"] = returnStr;


        std::cout << "\r\n+==================================+" << endl;
        std::cout << "Incoming Request" << endl;
        std::cout << "Authenticated? >> ";
        if (authenticated) {
            std::cout << "YES";
        }
        else {
            std::cout << "NO";
        }
        std::cout << " >> " << userPassCombo << endl;
        std::cout << "SCRIPT EXEC: " << execute << endl;
        std::cout << "CODE: " << returnCode << endl;
        std::cout << "RETURN: " << returnStr << endl;

        return toReturn;
    }
    std::string clearBufferAndReturnOld(std::string *buffer) {
        std::string oldBuffer = *buffer;
        *buffer = "no-message-yet";
        return oldBuffer;
    }
    void stopServer() {
        app.stop();
    }
