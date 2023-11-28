#ifndef MESSAGE_MANAGER_H
#define MESSAGE_MANAGER_H

#include <string>
#include <vector>
#include "Message.h"

/*
    author: 杨竞博
*/
class MessageManager{
public:
    MessageManager();
    ~MessageManager();

    // 新增一个聊天信息
    // 将新增的聊天信息写入txt文件中     
    // 指定好message的格式：
    // 例如： 时间-用户名-说的话
    void addMessage(Message message);

    // 将一个string封装成一个Message并返回
    Message packageMessage(User sender,std::vector<User*> receiver,std::string message);

    // 删除聊天信息
    // 将多余的聊天信息从txt文件中删除   
    // 只保存300条聊天信息
    void removeMessage();

    // 将聊天记录写入到文件中
    void writeMessage();
    // 将聊天记录读取到log里
    void readMessage();

public:
    std::vector<std::string> log; //消息容器
    const int maxMessages = 300; // 最大保存信息数量

};

#endif // MESSAGE_MANAGER_H