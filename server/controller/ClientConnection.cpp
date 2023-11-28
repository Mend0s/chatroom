#include "ClientConnection.h"
#include "../model/User.h"
#include "../model/UserManager.h"
#include "../model/Message.h"
#include "../model/MessageManager.h"
#include "../tools/JsonTools.h"

#include <iostream>
#include <string>
#include <sys/socket.h>
#include <unistd.h>
#include <pthread.h>

// 静态成员变量定义和初始化
UserManager ClientConnection::userManager; 
MessageManager ClientConnection::messageManager; 
JsonTools ClientConnection::jsonTools; 


// ClientConnection::ClientConnection(int clientSocket, std::vector<User*>& users)
//     :clientSocket(clientSocket), users(users)
// {}

ClientConnection::ClientConnection(int clientSocket, std::vector<User*>& users, pthread_mutex_t& mutex_cout)
    :clientSocket(clientSocket), users(users), mutex_cout(mutex_cout)
{}

ClientConnection::ClientConnection(int clientSocket, std::string ipAddress, int port, std::vector<User*>& users, pthread_mutex_t& mutex_cout)
    :clientSocket(clientSocket), ipAddress(ipAddress), port(port), users(users), mutex_cout(mutex_cout)
{}

// ClientConnection::ClientConnection(int clientSocket, std::vector<User*>& users, pthread_mutex_t& mutex, pthread_cond_t& cond)
//     :clientSocket(clientSocket),users(users),mutex(mutex)
// {}

// ClientConnection::ClientConnection(int clientSocket = -1, std::string ipAddress = "127.0.0.1", int port, std::vector<User*>& users, pthread_mutex_t& mutex, pthread_cond_t& cond)
// : users(users)
// {}


ClientConnection::~ClientConnection(){}

void ClientConnection::handleCommunication()
{
    //接收客户端数据
    char buff[1024] = "";
    int totalBytesReceived = 0;
    int len = 0;
    std::string jsonString;

    
    while (true)//循环与客户端通信
    {
        totalBytesReceived = 0;
        len = 0;
        while ((len = recv(this->clientSocket, buff + totalBytesReceived, sizeof(buff) - totalBytesReceived, 0)) > 0) 
        {
            totalBytesReceived += len;
            pthread_mutex_lock(&mutex_cout);
            std::cout << "接受了" << len << "个数据\n";
            pthread_mutex_unlock(&mutex_cout);
        }

        // int len = recv(this->clientSocket, buff, sizeof(buff), 0);

        if (totalBytesReceived > 0)
        {
            // std::cerr << "len: "<<len << "\n" 
            //     << "buff" << buff <<"\n";

            pthread_mutex_lock(&mutex_cout);

            jsonString.assign(buff);
            std::cout << jsonString << std::endl;
            Json::Value jsonValue = acceptParameters(jsonString);
            // std::cerr << jsonValue["account"].asString();

            pthread_mutex_unlock(&mutex_cout);

            distribute(jsonValue);  
        }
        else if (totalBytesReceived == 0){
            this->shouldClosed = true;
        }
        else
        {
            pthread_mutex_lock(&mutex_cout);
            std::cerr << "recv\n";
            std::cout << "接受错误\n";
            pthread_mutex_unlock(&mutex_cout);
        }
        
    }
    
   
}

bool ClientConnection::sendMessage(Message message)
{

    while (!message.getReceiver().empty())
    {
        //pthread_cond_wait(&this->cond, &mutex);

        return true;
    }
    //{"sender": "user1"},
    //{ "message": "hello" }

    for (int i = 0; i < users.size(); i++)
    {
        Json::Value value;
        value["sender"] = message.getSender().getNickname();
        value["message"] = message.getContent();
        std::string jsonString = jsonTools.jsonToString(value);
        int len = send(users[i]->getClientSocketFd(), jsonString.c_str(), jsonString.size(), 0);
        if (len == -1)
        {
            std::cerr << "send\n";
            return false;
        }
            
    }

    return true;
}



// 解析 JSON 字符串
Json::Value ClientConnection::acceptParameters(std::string jsonString){  
    return jsonTools.stringToJson(jsonString);
}


void ClientConnection::distribute(Json::Value jsonValue){
    // 给此客户端返回的Json信息
    Json::Value returnValue;

    if(jsonValue["type"].asString() == "logic"){
        // 判断账号是否输入正确
        if(this->isUserExist(jsonValue) == false){
            returnValue["logic"] = "false: 账号输入错误";
        }
        // 判断账号与密码正确
        else if(this->verifyUser(jsonValue) == false)
        {
            // 给客户端返回显示登录成功
            returnValue["logic"] = "false: 密码输入错误";
        }
        else{
            // 给客户端返回显示账号或密码错误
            returnValue["logic"] = "true";
        }
        // 给此客户端返回Json信息
        this->sendReturnJson(returnValue);
    }
    else if(jsonValue["type"].asString() == "register"){
        if(this->isUserExist(jsonValue)){
            // 用户已经存在
            returnValue["register"] = "false";
        }
        else{
            returnValue["register"] = "true";
        }
        this->sendReturnJson(returnValue);
    }
    else if(jsonValue["type"].asString() == "queryNumber"){
        returnValue["queryNumber"] = std::to_string(this->users.size()); 
        this->sendReturnJson(returnValue);
    }
    else if(jsonValue["type"].asString() == "changPw"){
        if(this->changePw(jsonValue)){
            returnValue["changPw"] = "true";
        }
        else{
            returnValue["changPw"] = "false";
        }
        this->sendReturnJson(returnValue);
    }
    else if(jsonValue["type"].asString() == "changeNickName"){
        if(this->changeNickName(jsonValue)){
            returnValue["changPw"] = "true";
        }
        else{
            returnValue["changPw"] = "false";
        }
        this->sendReturnJson(returnValue);
    }
    else if(jsonValue["type"].asString() == "text"){
        // 需要分发给所有客户端聊天信息
        Message message;
        // 设置需要发送的消息
        message.setContent(jsonValue["text"].asString());

        // 设置发送者
        std::string account = jsonValue["account"].asString();
        User user = this->getSender(account);
        message.setSender(user);

        // 设置接收者
        // 此vector为空，代表给所有客户端发送消息
        std::vector<User *> receiver;
        message.setReceiver(receiver);

        // 发送消息
        this->sendMessage(message);
    }
    else{
        // 不符合要求，直接舍弃
        // 不需要返回给客户端信息
    }

    return;
}

User ClientConnection::getSender(std::string account){
    User user;
    for(User *elemUser : users){
        // 一定找得到，因为account就是客户端发送者发来的
        if(elemUser->getAccount() == account){
            user = *elemUser;
            break;
        }
    }
    return user;
}

bool ClientConnection::verifyUser(Json::Value jsonValue){
    std::string account = jsonValue["account"].asString();
    std::string password = jsonValue["password"].asString();
    User user;
    user.setAccount(account);
    user.setPassword(password);
    return userManager.verifyUser(user);
}

bool ClientConnection::isUserExist(Json::Value jsonValue){
    std::string account = jsonValue["account"].asString();
    User user;
    user.setAccount(account);
    return userManager.isUserExist(user);
}

bool ClientConnection::registerUser(Json::Value jsonValue){
    std::string account = jsonValue["account"].asString();
    std::string nickname = jsonValue["nickname"].asString();
    std::string password = jsonValue["password"].asString();
    User user;
    user.setAccount(account);
    user.setNickname(nickname);
    user.setPassword(password);
    return userManager.addUser(user);
}

bool ClientConnection::changePw(Json::Value jsonValue){
    std::string account = jsonValue["account"].asString();
    std::string password = jsonValue["password"].asString();
    std::string newPw = jsonValue["newPw"].asString();
    User user;
    user.setAccount(account);
    user.setPassword(password);
    return userManager.changePassword(user, newPw);
}

bool ClientConnection::changeNickName(Json::Value jsonValue){
    std::string account = jsonValue["account"].asString();
    std::string password = jsonValue["password"].asString();
    std::string newNickName = jsonValue["newNickName"].asString();
    User user;
    user.setAccount(account);
    user.setPassword(password);
    return userManager.changeNickname(user, newNickName);
}

std::string ClientConnection::getIpAddress(){
    return this->ipAddress;
}

void ClientConnection::setIpAddress(std::string ipAddress){
    this->ipAddress = ipAddress;
}

int ClientConnection::getPort(){
    return port;
}

void ClientConnection::setPort(int port){
    this->port = port;
}

int ClientConnection::getClientSocketFd(){
    return this->clientSocket;
}

void ClientConnection::setClientSocketFd(int clientSocketFd){
    this->clientSocket = clientSocket;
}

bool ClientConnection::getShouldClosed(){
    return this->shouldClosed;
}

void ClientConnection::setShouldClosed(bool shouldClosed){
    this->shouldClosed = shouldClosed;
}

void ClientConnection::sendReturnJson(Json::Value value)
{
    
    std::string sendString = jsonTools.jsonToString(value);
    int len = send(this->clientSocket, sendString.c_str(), sendString.size(), 0);
    if (len == -1)
        std::cerr << "send\n";
}
