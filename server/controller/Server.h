#ifndef __SERVER_H   // 避免重复包含
#define __SERVER_H


#include "ClientConnection.h"
#include "../model/UserManager.h"

#include <vector>
#include <json/json.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>


/*
    除了：
        start(): 蒋恒新
        stop(): 蒋恒新
    

        void addClientConnection(ClientConnection *clientConnection);
        removeClientConnection
        : 杨竞博

    author: 侯顺智
*/

// 服务器类
// 负责底层的网络通信、处理连接请求等服务器核心功能
class Server {
public:
    Server(std::string ipAddress = "127.0.0.1", int port = 8888, int serverSocket = -1);
    ~Server();

    // 启动服务器
    // 创建服务器线程
    void start();
    // 停止服务器
    void stop();


  
    /**
     * @brief 增加一个客户端连接
     *
     * 增加一个客户端连接,一个线程对应一个ClientConnection，让一个线程去与客户端通信
     *
     * @param clientConnection ClientConnection *
     *
     * @return void
     * @author 杨竞博
     */
    void addClientConnection(ClientConnection *clientConnection);

//"type": "exit"
    // 
    /**
     * @brief 断开此客户端连接: 删除一个客户端连接
     *
     * 断开此客户端连接: 删除一个指定的客户端连接
     *
     * @param jsonValue Json::Value
     *
     * @return void  
     * @author 杨竞博
     */
    void removeClientConnection(ClientConnection *clientConnection);

    friend void* checkIfCloseConnection(void* s);
    //void* checkIfCloseConnection(void* s);
private:
    std::string ipAddress;      // 该服务器的ip地址
    int port;                   // 该服务器的端口
    int serverSocket;           // 服务器Socket，用来监听连接请求的Socket
    std::vector<ClientConnection*> clientConnections; // 保存连接此服务器的客户端
    std::vector<User*> users; // 已连接用户
    pthread_mutex_t mutex;
    pthread_cond_t cond;//条件变量

    pthread_mutex_t mutex_cout;
};
/**
     * @brief 循环检测，看是否需要断开客户端链接
     *
     * 断开此客户端连接，检测过程中加锁
     *
     * @param void
     *
     * @return void
     * @author 蒋恒新
     */
void* checkIfCloseConnection(void* s);
    
#endif