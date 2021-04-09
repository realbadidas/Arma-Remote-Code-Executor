
#define CROW_ENABLE_SSL

#include "server.h"

#include "crow.h"
#include <string>
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
                    std::cout << "New Post Request" << std::endl;
                    std::cout << "Script execute: " << std::endl;

                    std::string returnStr = "Error";
                    std::string returnCode = "1";

                    std::string str = req.body;

                    CROW_LOG_INFO << "New Message: " << str;


                    std::string user = urlParamsParser(str, "user");
                    std::string password = urlParamsParser(str, "pass");
                    std::string execCommand = urlParamsParser(str, "execute");

                    std::string hash_hex_str;
                    picosha2::hash256_hex_string(password, hash_hex_str);


                    if (checkIfValidUser(user, hash_hex_str)) {
                        int i = 0;

                        commandBuffer = urlDecode(execCommand);
                        while (returnBuffer == "no-message-yet" && i < 300) { std::this_thread::sleep_for(10ms); i++; }

                        if (returnBuffer == "no-message-yet") {
                            returnCode = "2";
                            returnStr = "NO RESPONSE FROM SCRIPT";
                            commandBuffer = "no-message-yet";
                        }
                        else {
                            returnCode = "0";
                            returnStr = returnBuffer;
                            returnBuffer = "no-message-yet";
                        }
                    }
                    else {
                        returnCode = "3";
                        returnStr = "Access denied for " + user + "@" + hash_hex_str;
                        std::cout << "Invalid password or username: " << user + "@" << hash_hex_str << std::endl;
                    }
                    crow::json::wvalue x;

                    x["code"] = returnCode;
                    x["return"] = returnStr;
                    std::cout << "Code " << returnCode << std::endl;
                    std::cout << "----" << std::endl;
                    return x;
                });

        std::cout << "Webserver started." << std::endl;

        if (JSON["https"].as<bool>(false))
            app.port(port).bindaddr(host).multithreaded().ssl_file(SSL_Cert, SSL_Key).run();
        else
            app.port(port).bindaddr(host).multithreaded().run();
    }
    void stopServer() {
        app.stop();
    }
