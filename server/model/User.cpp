#include "User.h"


User::User()
{
	this->nickname = "\0";
	this->account = "\0";
	this->password = "\0";
	this->clientSocketFd = -1;
}

User::User(std::string nickname, std::string account, std::string password)
	:nickname(nickname),account(account),password(password)
{
	clientSocketFd = 0;
}

User::~User()
{
}

std::string User::getNickname()
{
	return nickname;
}

void User::setNickname(std::string nickname)
{
	this->nickname = nickname;
}

std::string User::getAccount()
{
	return account;
}

void User::setAccount(std::string account)
{
	this->account = account;
}



std::string User::getPassword()
{
	return password;
}

void User::setPassword(std::string password)
{
	this->password = password;
}

int User::getClientSocketFd()
{
	return clientSocketFd;
}

void User::setClientSocketFd(int clientSocketFd)
{
	this->clientSocketFd = clientSocketFd;
}
