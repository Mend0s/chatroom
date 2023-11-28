#include"UserManager.h"
#include<iostream>
#include<fstream>

bool UserManager::addUser(User& u)
{
	std::ofstream ofs;//创建流对象
	ofs.open("Userdata.csv", std::ios::out | std::ios::app);//以写文件和追加的方式打开文件
	if (ofs.fail())//判断是否打开文件失败
	{
		std::cerr << "Failed to open the Userdata.csv.\n";
		return false;
	}
	ofs << u.getAccount() << ","//向文件中写入数据
		<< u.getPassword() << ","
		<< u.getNickname() << std::endl;
	ofs.close();//关闭文件
	return true;
}

bool UserManager::removeUser(User& u)
{//思路是创建一个新的临时文件，然后逐行读取原文件的内容并写入到临时文件中，遇到需要删除的内容则跳过
	std::string toRemove = u.getAccount() + "," 
		+ u.getPassword() + "," + u.getNickname();//这是需要删除的行的内容

	std::ifstream ifs("Userdata.csv");//打开原始数据文件
	std::ofstream ofs("temp.csv");//创建并打开临时文件

	std::string line;//用于存储逐行读的原始数据内容
	while (std::getline(ifs, line)) {
		if (line != toRemove)//如果不是要删除的行，则跳过写入
			ofs << line << std::endl;
	}
	ifs.close();//关闭原始数据文件
	ofs.close();//关闭临时文件

	std::remove("Userdata.csv");//删除原始数据文件
	std::rename("temp.csv", "Userdata.csv");//将临时文件改名为原始数据文件名
	return true;
}

bool UserManager::verifyUser(User& u)
{//只需查验账号密码【验证用户是登录功能】
	int flag = 0;//用于判断数据中是否有改用户标志位
	std::ifstream ifs("Userdata.csv", std::ios::in);
	if (ifs.fail())//判断是否打开文件失败
	{
		std::cerr << "Failed to open the Userdata.csv.\n";
		return false;
	}
	std::string checkstr = u.getAccount() + ","
		+ u.getPassword();//这是查验的行的内容

	std::string line;//用于存储每行的内容
	std::string validstr;//存储每行中的账号和密码，格式为：账号，密码
	while (std::getline(ifs, line)) {
		int times = 0;
		for (int i = 0; i < line.size(); i++)//根据数据格式，找到第2个逗号的下标，截取之前的子串赋给validstr
		{
			if (line[i] == ',')
				times++;
			if (times == 2)
			{
				validstr = line.substr(0, i);
				break;
			}
						}
		if (validstr == checkstr)//如果有一行是和需要查验的相等则flag置1
			flag = 1;
	}

	return flag==1?true:false;//flag为1返回true，否则返回false
}

bool UserManager::isUserExist(User& u)
{//判断账号是否存在，仅在文本文件中产看传入的User账号是否存在
	int flag = 0;//用于判断数据中是否有改用户标志位
	std::ifstream ifs("Userdata.csv", std::ios::in);
	if (ifs.fail())//判断是否打开文件失败
	{
		std::cerr << "Failed to open the Userdata.csv.\n";
		return false;
	}
	std::string checkstr = u.getAccount();//这是查验的行的内容

	std::string line;//用于存储每行的内容
	std::string validstr;//存储每行中的账号
	while (std::getline(ifs, line)) {
		int times = 0;
		for (int i = 0; i < line.size(); i++)//根据数据格式，找到第1个逗号的下标，截取之前的子串赋给validstr
		{
			if (line[i] == ',')
				times++;
			if (times == 1)
			{
				validstr = line.substr(0, i);
				break;
			}
		}
		if (validstr == checkstr)//如果有一行是和需要查验的相等则flag置1
			flag = 1;
	}

	return flag == 1 ? true : false;//flag为1返回true，否则返回false
}

//void UserManager::writeUsers(User& u)
//{
//	//std::ofstream ofs("Userdata.csv", std::ios::out | std::ios::app);
//}
//
//void UserManager::readUsers()
//{
//
//}

bool UserManager::changePassword(User& u, std::string newpw)
{//修改密码，查验账号和原密码是否对得上
	int flag = 0;//成功与否标志位
	std::string needchangestr = u.getAccount() + ","
		+ u.getPassword();//这是需要删除的行的内容
	std::string afterchangestr = u.getAccount() + ","
		+ newpw + ",";//对应行修改过后的内容

	std::ifstream ifs("Userdata.csv");//打开原始数据文件
	std::ofstream ofs("temp.csv");//创建并打开临时文件

	std::string line;//用于存储逐行读的原始数据内容
	std::string validstr;//存储每行中的账号和密码，格式为：账号，密码
	std::string nickname;//存放原始数据的昵称
	while (std::getline(ifs, line)) {//逐行读取到line
		int times = 0;
		for (int i = 0; i < line.size(); i++)//根据数据格式，找到第2个逗号的下标，截取之前的子串赋给validstr
		{
			if (line[i] == ',')
				times++;
			if (times == 2)
			{
				validstr = line.substr(0, i);//找到第二个逗号，将line字符串从0开始的i长度的子串给validstr【也就是账号，密码】
				nickname = line.substr(i + 1);//将第二个逗号后面第一个字符作起始字符，一直到line结束【也就是昵称】
				break;
			}
		}
		if (validstr == needchangestr && flag==0)//当line是需要修改的那一行
		{
			ofs << afterchangestr + nickname << std::endl;//就写入修改后的数据
			flag = 1;
		}
		else//若不是，则正常写入前一个文件的数据
			ofs << line << std::endl;
	}
	ifs.close();//关闭原始数据文件
	ofs.close();//关闭临时文件

	std::remove("Userdata.csv");//删除原始数据文件
	std::rename("temp.csv", "Userdata.csv");//将临时文件改名为原始数据文件名
	return flag==1?true:false;
}

bool UserManager::changeNickname(User& u, std::string newNickname)
{//修改昵称，查询账号是否一致
	int flag = 0;//改变成功与否标志位
	std::string needchangestr = u.getAccount();//这是需要删除的行的内容

	std::ifstream ifs("Userdata.csv");//打开原始数据文件
	std::ofstream ofs("temp.csv");//创建并打开临时文件

	std::string line;//用于存储逐行读的原始数据内容
	std::string account;//存储原数据账号
	std::string password;//存储原数据密码
	while (std::getline(ifs, line)) {//逐行读取到line
		int times = 0;
		int test = 0;//用于保证times==1条件只进入一次
		for (int i = 0; i < line.size(); i++)//根据数据格式，找到第1个逗号的下标，截取之前的子串赋给account
		{
			if (line[i] == ',')
				times++;
			if (times == 1 && test==0)
			{
				account = line.substr(0, i);//找到第1个逗号，将line字符串从0开始的i长度的子串给account【也就是账号】
				test = 1;
			}
			else if (times == 2)
			{
				password = line.substr(account.size()+1,i-account.size()-1);//将第1个逗号后面第一个字符作起始字符，一直到第二个逗号前结束【也就是密码】
				break;
			}
		}
		if (account == needchangestr && flag==0)//当line是需要修改的那一行
		{
			ofs << u.getAccount() + "," + password + "," + newNickname << std::endl;//就写入修改后的数据
			flag = 1;
		}
		else//若不是，则正常写入前一个文件的数据
			ofs << line << std::endl;
	}
	ifs.close();//关闭原始数据文件
	ofs.close();//关闭临时文件

	std::remove("Userdata.csv");//删除原始数据文件
	std::rename("temp.csv", "Userdata.csv");//将临时文件改名为原始数据文件名
	return flag == 1 ? true : false;
}
