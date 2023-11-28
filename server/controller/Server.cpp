#include "Server.h"
#include "ClientConnection.h"

#include <json/json.h>
#include <string>
#include <iostream>
#include <algorithm>

Server::Server(std::string ipAddress, int port, int serverSocket):
    ipAddress(ipAddress),port(port),serverSocket(serverSocket)
{
    pthread_mutex_init(&this->mutex, NULL);//初始化互斥锁
    pthread_cond_init(&cond, NULL);//初始化条件变量

    pthread_mutex_init(&this->mutex_cout, NULL);//初始化互斥锁
    
}

Server::~Server(){

    pthread_mutex_destroy(&this->mutex_cout);
    // 释放clientConnections
    for(auto elem : clientConnections){
        delete elem;
    }

    // 释放users
    for(auto elem : users){
        delete elem;
    }
}

void Server::start()
{
    //1.创建监听套接字
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1)
    {
        std::cerr << "socket\n";
        return;
    }
    //2.绑定本地IP和port
    struct sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(8888);
    if (inet_pton(AF_INET, "192.168.216.128", &(saddr.sin_addr.s_addr)) <= 0) {
        std::cout << "Invalid IP address" << std::endl;
        return;
    }
    int ret = bind(fd, (struct sockaddr*)&saddr, sizeof(saddr));
    if (ret == -1)
    {
        std::cerr << "bind失败\n";
        return;
    }
    //3.设置监听
    ret = listen(fd, 128);//128为同时处理的最大连接请求
    if (ret == -1)
    {
        std::cerr << "listen\n";
        return;
    }
    char ip[16] = "";
    inet_ntop(AF_INET, &saddr.sin_addr.s_addr, ip, 16);
    std::cout << "server: ip: " 
        << ip
        << " port: " 
        << ntohs(saddr.sin_port)
        << std::endl;

    //4.循环等待客户端的连接
    int flag = 0;
    while (true)
    {
        struct sockaddr_in caddr;
        socklen_t  addrlen = sizeof(caddr);
        int cfd = accept(fd, (struct sockaddr*)&caddr, &addrlen);//等待客户端的连接
        if (cfd == -1)//连接失败
        {
            std::cerr << "accept\n";
            continue;
        }
        inet_ntop(AF_INET, &caddr.sin_addr.s_addr, ip, 16);
        std::cout << "client: ip: " 
            << ip
            << " port: " 
            << ntohs(caddr.sin_port)
            << std::endl;

        ClientConnection * clientConnection = new ClientConnection(cfd,this->users, this->mutex_cout);//封装ClientConnection类
        clientConnection->setIpAddress(inet_ntop(AF_INET, &caddr.sin_addr.s_addr, ip, 16));
        clientConnection->setPort(ntohs(caddr.sin_port));
        this->addClientConnection(clientConnection);//调用方法，方法内部创建线程去处理与客户端的通信
        
        if (flag == 0)
        {
            pthread_t pid;
            pthread_create(&pid, NULL,checkIfCloseConnection, this);
        }

        //delete clientConnection;      
    }
}

void Server::stop()
{
}

void Server::addClientConnection(ClientConnection *clientConnection){
    // 检查clientConnection是否为nullptr
    if (clientConnection == nullptr) {

        return;
    }
    // 检查客户端连接是否已存在
    auto it = std::find(clientConnections.begin(), clientConnections.end(), clientConnection);
    if (it == clientConnections.end()) { // 如果连接不在列表中
        clientConnections.push_back(clientConnection); // 添加到列表

        // 创建新线程
        pthread_t thread;
        int result = pthread_create(&thread, NULL, ClientConnection::threadFunc, (void*)clientConnection);
        if (result != 0) {
            // 处理创建线程失败的情况
            // 记录错误日志或抛出异常
        } else {
            // 成功创建线程
            pthread_detach(thread); // 分离线程，让它独立运行
        }
    }
}


void Server::removeClientConnection(ClientConnection *clientConnection) {
    if (clientConnection == nullptr) {
        return;
    }

    // 不需要显式关闭连接，因为这应该在ClientConnection的析构函数中处理

    // 查找并移除连接
    auto it = std::find(clientConnections.begin(), clientConnections.end(), clientConnection);
    if (it != clientConnections.end()) {
        clientConnections.erase(it);
        delete clientConnection; // 删除对象并调用析构函数
    }
}

void* checkIfCloseConnection(void* s)
{
    Server* server = (Server*)s;//传入的void*是一个Server对象，先做转换
    while (true)//不断循环检测Server里的std::vector<ClientConnection*> clientConnections;
    {
   
        for (int i = 0; i < server->clientConnections.size(); i++)
        {
            if (server->clientConnections[i]->getShouldClosed())
            {
                pthread_mutex_lock(&server->mutex);//加锁
                server->removeClientConnection(server->clientConnections[i]);//移除一个客户端连接
                pthread_mutex_unlock(&server->mutex);//解锁
            }       
        }
        
    }
    
    return nullptr;
}
