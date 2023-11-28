#ifndef MESSAGE_H
#define MESSAGE_H

#include <string>
#include <vector>
#include "User.h"

class Message {
public://发送者User，接收用户vector<User*>,内容string
    Message();
    Message(User sender, std::vector<User*> receiver, std::string content);
    ~Message();
    //获取和设置信息
    User getSender() const;
    void setSender(User sender);
    //获取和设置接收用户
    std::vector<User*> getReceiver() const;
    void setReceiver(std::vector<User*>& receiver);
    //获取和设置发送内容
    std::string getContent() const;
    void setContent(std::string content);

private:
    User sender;                    // 发送者
    std::vector<User *> receiver;   // 接收用户，空容器时是所有人
    std::string content;            // 消息内容
};

#endif // MESSAGE_H
