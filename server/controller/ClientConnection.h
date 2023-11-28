#ifndef __CLIENTCONNECTION_H   // 避免重复包含
#define __CLIENTCONNECTION_H


#include "../model/Message.h"
#include "../tools/JsonTools.h"
#include "../model/MessageManager.h"
#include "../model/UserManager.h"

#include <string>
#include <json/json.h>
#include <unistd.h>
#include <pthread.h>


/*
    author: 侯顺智
*/
// 表示一个客户端的连接，负责处理与单个客户端的通信。
class ClientConnection {
public:
    // ClientConnection(int clientSocket, std::vector<User*>& users);
    ClientConnection(int clientSocket, std::vector<User*>& users, pthread_mutex_t& mutex_cout);
    ClientConnection(int clientSocket, std::string ipAddress, int port, std::vector<User*>& users,pthread_mutex_t& mutex_cout);
    // ClientConnection(int clientSocket = -1, std::vector<User*>& users, pthread_mutex_t &mutex, pthread_cond_t &cond);
    // ClientConnection(int clientSocket = -1, std::string ipAddress = "127.0.0.1", int port, std::vector<User*>& users, pthread_mutex_t& mutex, pthread_cond_t& cond);
    ~ClientConnection();

    /*
        更新思路
        1、接受客户端发来的信息，将信息传给acceptParameters
        2、根据Json::Value的type，调用distribute进行分发。相关处理都在distribute方法里。
    */
    void handleCommunication(); 

    //"type": "text"
    // 分发给指定客户端信息   ★★★★★
    // distribute方法里判断type如果是聊天信息，调用此函数
    // 方法一：将刚刚接受的信息保存在一个全局变量/文件里（需要读写锁），然后通知所有的线程去读
    // 方法二：将刚刚接受的信息保存在一个数组/string（局部变量）里，然后遍历users,让每个用户所关联的线程去读取。（这样就不需要读写锁了）
    bool sendMessage(Message message);

    static void* threadFunc(void* arg)
    {
        ClientConnection* instance = static_cast<ClientConnection*>(arg);
        instance->handleCommunication();
        return nullptr;
    }

    /**
     * @brief 接受Qt传来的参数
     *
     * 解析客户端传来的JSON格式的字符串，并返回解析后的Json::Value
     *
     * @param jsonString std::string 客户端传来的字符串，字符串的内容是JSON格式的数据
     *
     * @return 解析后的Json::Value，如果解析失败则返回nullptr
     * @author 侯顺智
     */
    Json::Value acceptParameters(std::string jsonString);

    /**
     * @brief 分发请求
     *
     * 根据JSON里的type的值进行分发，选择具体的解析函数   ★★★★★
     *
     * @param jsonValue Json::Value
     *
     * @return void
     * @author 侯顺智
     */
    void distribute(Json::Value jsonValue);

//"type": "logic"
    /**
     * @brief 判断用户名账号与密码是否正确
     *
     * 判断用户名账号与密码是否正确
     *
     * @param jsonValue Json::Value
     *
     * @return 检查结果，如果用户名账号与密码都正确，返回true,否则返回false
     * @author 侯顺智
     */
    bool verifyUser(Json::Value jsonValue);

//"type": "register"
    /**
     * @brief 判断用户是否存在
     *
     * 判断用户是否存在
     *
     * @param jsonValue Json::Value
     *
     * @return bool  如果用户存在则返回true,否则返回false
     * @author 侯顺智
     */
    bool isUserExist(Json::Value jsonValue); 



    /**
     * @brief 根据传来的用户名与密码注册一个账号
     *
     * 根据传来的用户名与密码注册一个账号
     *
     * @param jsonValue Json::Value
     *
     * @return bool  注册用户成功则返回true,否则返回false
     * @author 侯顺智
     */
    bool registerUser(Json::Value jsonValue);

    bool changePw(Json::Value jsonValue);
    bool changeNickName(Json::Value jsonValue);
    

    std::string getIpAddress();
    void setIpAddress(std::string ipAddress);

    int getPort();
    void setPort(int port);

    int getClientSocketFd();
    void setClientSocketFd(int clientSocketFd);

    bool getShouldClosed();


private:
    std::string ipAddress;      // 该客户端的ip地址
    int port;                   // 该客户端的端口
    int clientSocket;           // 客户端Socket
    std::vector<User*> &users; // 服务器所含的users
    bool shouldClosed;          // 是否应该关闭连接
    pthread_mutex_t mutex;//保存Server类中的互斥锁变量
    pthread_cond_t cond;//条件变量
    pthread_mutex_t &mutex_cout;

    void setShouldClosed(bool shouldClosed);
    User getSender(std::string account);
    // 给此客户端返回Json信息
    void sendReturnJson(Json::Value value);

    static UserManager userManager;    // 方便执行对用户的操作
    static MessageManager messageManager;    // 方便执行对信息的操作
    static JsonTools jsonTools;
};

#endif