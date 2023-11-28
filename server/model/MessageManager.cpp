#include "MessageManager.h"
#include <fstream>
#include <iostream>

// 构造函数 需要一个用户类指针
MessageManager::MessageManager() {
    readMessage();
}

// 析构函数
MessageManager::~MessageManager() {
    writeMessage();
}

    // 新增一个聊天信息
    // 将新增的聊天信息写入txt文件中     
    // 指定好message的格式：
    // 例如： 用户名-说的话
void MessageManager::addMessage(Message message) {
    log.push_back(message.getSender().getNickname()+"-"+
    message.getContent());
    if (log.size() > maxMessages) {
        removeMessage();
    }
    writeMessage();
}

    // 将一个string封装成一个Message并返回
Message MessageManager::packageMessage(User sender,std::vector<User*> receivers,std::string message) {
    return Message(sender ,receivers ,message);
}

    // 删除聊天信息
    // 将多余的聊天信息从txt文件中删除   
    // 只保存300条聊天信息
void MessageManager::removeMessage() {
        log.erase(log.begin());
        writeMessage();
}

    // 将聊天记录写入到文件中，每个信息后加'\n'
void MessageManager::writeMessage() {
    std::ofstream outFile("../messages.txt", std::ios::out | std::ios::trunc);
    if (outFile.is_open()) {
        for (const auto& message : log) {
            outFile << message << "\n";
        }
        outFile.close();
    } else {
        std::cerr << "Unable to open file for writing.\n";
    }
}

    // 将聊天记录读取到log里
void MessageManager::readMessage() {
    std::ifstream inFile("../messages.txt", std::ios::in);
    std::string line;
    if (inFile.is_open()) {
        while (getline(inFile, line)) {
            log.push_back(line);
        }
        inFile.close();
    } else {
        // std::cerr << "Unable to open file for reading.\n";
    }
}
