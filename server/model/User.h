#ifndef __USER_H_
#define __USER_H_



#include <string>
/*
    author: 蒋恒新
*/

/*
    question:
    1.clientSocketFd是否是服务端与该客户端通信的描述符?
    2.账号的数据与客户端登录该账号的端口和IP地址的关系，设计到私聊用户时服务器分发消息给对应用户的选择问题
*/
/*
    change:

*/
class User {
public:
    User();
    User(std::string nickname,std::string account, std::string password);
    ~User();

    std::string getNickname();
    void setNickname(std::string nickname);

    std::string getAccount();
    void setAccount(std::string account);

    std::string getPassword();
    void setPassword(std::string password);

    int getClientSocketFd();
    void setClientSocketFd(int clientSocketFd);

private:
    std::string nickname; // 用户昵称
    std::string account;  // 账号
    std::string password; // 密码
    int clientSocketFd;   // 客户端Socket,套接字描述符 ，默认为0
};

#endif // !__USER_H_
