#include "Message.h"

Message::Message() {
}

// Constructor
Message::Message(User sender, std::vector<User*> receiver, std::string content)
    : sender(sender), receiver(receiver), content(content) {}

// Destructor
Message::~Message() {
}

// Getter for sender
User Message::getSender() const {
    return sender;
}

// Setter for sender
void Message::setSender(User newSender) {
    sender = newSender;
}

// Getter for receiver
std::vector<User*> Message::getReceiver() const {
    return receiver;
}

// Setter for receiver
void Message::setReceiver(std::vector<User*>& newReceiver) {
    receiver = newReceiver;
}

// Getter for content
std::string Message::getContent() const {
    return content;
}

// Setter for content
void Message::setContent(std::string newContent) {
    content = newContent;
}
