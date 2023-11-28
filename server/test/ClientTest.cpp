#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<arpa/inet.h>
#include<json/json.h>
#include <iostream>
#include "../tools/JsonTools.h"

int main()
{
	//1.创建通信的套接字
	int fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd == -1)
	{
		perror("socket");
		return -1;
	}
	//2.连接服务器IP port
	struct sockaddr_in saddr;
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(8888);
	inet_pton(AF_INET, "192.168.216.128", &saddr.sin_addr.s_addr);
	int ret = connect(fd, (struct sockaddr*)&saddr, sizeof(saddr));
	if (ret == -1)
	{
		perror("connect");
		return -1;
	}
	int number = 0;
	//3.通信
	JsonTools jsonTools;
	//发送数据
		//char buff[1024];
	Json::Value value;
	value["type"] = "register";
	value["account"] = "123456789";
	value["nickname"] = "ronaldo";
	value["password"] = "qweasdzxc";
	std::string sendstr = jsonTools.jsonToString(value);
	//sprintf(buff, "你好,hello world,%d...\n", number++);
	send(fd, sendstr.c_str(), sizeof(sendstr), 0);
	std::cout << sendstr << '\n' << sendstr.c_str() << '\n';
	while (1)
	{
		

		//接收数据
		//memset(buff, 0, sizeof(buff));
		char buff[1024] = "";
		int len = recv(fd, buff, sizeof(buff), 0);
		if (len > 0)
		{
			printf("server say:%s\n", buff);
		}
		else if (len == 0)
		{
			printf("服务器已经断开了连接...\n");
			break;
		}
		else
		{
			perror("recv");
			break;
		}
		sleep(1);

	}
	//.关闭文件描述符
	close(fd);

	return 0;
}
