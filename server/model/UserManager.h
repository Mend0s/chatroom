#ifndef __USERMANAGER_H_
#define __USERMANAGER_H_



#include <string>
#include <vector>
#include "User.h"

/*
    author: 蒋恒新
*/

/*
     1.用户在文本中存储的格式：
    【account,password,nickname\n】

    2.各功能需要的数据：
    1.登录
    发送账号，密码，其余为默认值
    2.注册
    发送账号，昵称，密码，其余为默认值
    3.修改密码
    发送账号，密码，新密码，其余为默认值
    4.修改昵称
    发送账号，昵称，新昵称，其余为默认值

*/
/*
    change:
    1.std::vector<std::string ,std::string> users->std::vector<User*> users;
    2.各方法参数修改为User对象
*/

// 对用户进行管理
// 主要是对用户的用户名与密码进行操作，而不涉及到套接字标识符
class UserManager {
public:
    UserManager() {};
    ~UserManager() {};

    // 新增一个用户
    // 将新增的用户的用户名与密码写入txt文件中     
    //void addUser(std::string username, std::string password);
    bool addUser(User& u);

    // 删除一个用户
    // 将指定的用户从txt文件中删除       
    //User removeUser(std::string username);
    bool removeUser(User& u);

    // 判断输入的用户名与密码是否正确
    //bool verifyUser(const std::string username, const std::string password);
    bool verifyUser(User& u);

    // 判断用户是否存在
    //bool isUserExist(const std::string username, const std::string password); 
    bool isUserExist(User& u);

    // 将用户的信息写入到文件中
    //void writeUsers();

    // 将用户的信息读取到users里,默认读文本文件最新加入的用户
    //void readUsers();

    /*new*/
    //修改账号密码
    bool changePassword(User& u,std::string newpw);

    //修改账号昵称
    bool changeNickname(User& u, std::string newNickname);


private:
    std::vector<User*> users; // 保存用户名和密码信息的映射

    
};
#endif // !__USERMANAGER_H_
