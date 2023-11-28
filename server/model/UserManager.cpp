#include"UserManager.h"
#include<iostream>
#include<fstream>

bool UserManager::addUser(User& u)
{
	std::ofstream ofs;//����������
	ofs.open("Userdata.csv", std::ios::out | std::ios::app);//��д�ļ���׷�ӵķ�ʽ���ļ�
	if (ofs.fail())//�ж��Ƿ���ļ�ʧ��
	{
		std::cerr << "Failed to open the Userdata.csv.\n";
		return false;
	}
	ofs << u.getAccount() << ","//���ļ���д������
		<< u.getPassword() << ","
		<< u.getNickname() << std::endl;
	ofs.close();//�ر��ļ�
	return true;
}

bool UserManager::removeUser(User& u)
{//˼·�Ǵ���һ���µ���ʱ�ļ���Ȼ�����ж�ȡԭ�ļ������ݲ�д�뵽��ʱ�ļ��У�������Ҫɾ��������������
	std::string toRemove = u.getAccount() + "," 
		+ u.getPassword() + "," + u.getNickname();//������Ҫɾ�����е�����

	std::ifstream ifs("Userdata.csv");//��ԭʼ�����ļ�
	std::ofstream ofs("temp.csv");//����������ʱ�ļ�

	std::string line;//���ڴ洢���ж���ԭʼ��������
	while (std::getline(ifs, line)) {
		if (line != toRemove)//�������Ҫɾ�����У�������д��
			ofs << line << std::endl;
	}
	ifs.close();//�ر�ԭʼ�����ļ�
	ofs.close();//�ر���ʱ�ļ�

	std::remove("Userdata.csv");//ɾ��ԭʼ�����ļ�
	std::rename("temp.csv", "Userdata.csv");//����ʱ�ļ�����Ϊԭʼ�����ļ���
	return true;
}

bool UserManager::verifyUser(User& u)
{//ֻ������˺����롾��֤�û��ǵ�¼���ܡ�
	int flag = 0;//�����ж��������Ƿ��и��û���־λ
	std::ifstream ifs("Userdata.csv", std::ios::in);
	if (ifs.fail())//�ж��Ƿ���ļ�ʧ��
	{
		std::cerr << "Failed to open the Userdata.csv.\n";
		return false;
	}
	std::string checkstr = u.getAccount() + ","
		+ u.getPassword();//���ǲ�����е�����

	std::string line;//���ڴ洢ÿ�е�����
	std::string validstr;//�洢ÿ���е��˺ź����룬��ʽΪ���˺ţ�����
	while (std::getline(ifs, line)) {
		int times = 0;
		for (int i = 0; i < line.size(); i++)//�������ݸ�ʽ���ҵ���2�����ŵ��±꣬��ȡ֮ǰ���Ӵ�����validstr
		{
			if (line[i] == ',')
				times++;
			if (times == 2)
			{
				validstr = line.substr(0, i);
				break;
			}
						}
		if (validstr == checkstr)//�����һ���Ǻ���Ҫ����������flag��1
			flag = 1;
	}

	return flag==1?true:false;//flagΪ1����true�����򷵻�false
}

bool UserManager::isUserExist(User& u)
{//�ж��˺��Ƿ���ڣ������ı��ļ��в��������User�˺��Ƿ����
	int flag = 0;//�����ж��������Ƿ��и��û���־λ
	std::ifstream ifs("Userdata.csv", std::ios::in);
	if (ifs.fail())//�ж��Ƿ���ļ�ʧ��
	{
		std::cerr << "Failed to open the Userdata.csv.\n";
		return false;
	}
	std::string checkstr = u.getAccount();//���ǲ�����е�����

	std::string line;//���ڴ洢ÿ�е�����
	std::string validstr;//�洢ÿ���е��˺�
	while (std::getline(ifs, line)) {
		int times = 0;
		for (int i = 0; i < line.size(); i++)//�������ݸ�ʽ���ҵ���1�����ŵ��±꣬��ȡ֮ǰ���Ӵ�����validstr
		{
			if (line[i] == ',')
				times++;
			if (times == 1)
			{
				validstr = line.substr(0, i);
				break;
			}
		}
		if (validstr == checkstr)//�����һ���Ǻ���Ҫ����������flag��1
			flag = 1;
	}

	return flag == 1 ? true : false;//flagΪ1����true�����򷵻�false
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
{//�޸����룬�����˺ź�ԭ�����Ƿ�Ե���
	int flag = 0;//�ɹ�����־λ
	std::string needchangestr = u.getAccount() + ","
		+ u.getPassword();//������Ҫɾ�����е�����
	std::string afterchangestr = u.getAccount() + ","
		+ newpw + ",";//��Ӧ���޸Ĺ��������

	std::ifstream ifs("Userdata.csv");//��ԭʼ�����ļ�
	std::ofstream ofs("temp.csv");//����������ʱ�ļ�

	std::string line;//���ڴ洢���ж���ԭʼ��������
	std::string validstr;//�洢ÿ���е��˺ź����룬��ʽΪ���˺ţ�����
	std::string nickname;//���ԭʼ���ݵ��ǳ�
	while (std::getline(ifs, line)) {//���ж�ȡ��line
		int times = 0;
		for (int i = 0; i < line.size(); i++)//�������ݸ�ʽ���ҵ���2�����ŵ��±꣬��ȡ֮ǰ���Ӵ�����validstr
		{
			if (line[i] == ',')
				times++;
			if (times == 2)
			{
				validstr = line.substr(0, i);//�ҵ��ڶ������ţ���line�ַ�����0��ʼ��i���ȵ��Ӵ���validstr��Ҳ�����˺ţ����롿
				nickname = line.substr(i + 1);//���ڶ������ź����һ���ַ�����ʼ�ַ���һֱ��line������Ҳ�����ǳơ�
				break;
			}
		}
		if (validstr == needchangestr && flag==0)//��line����Ҫ�޸ĵ���һ��
		{
			ofs << afterchangestr + nickname << std::endl;//��д���޸ĺ������
			flag = 1;
		}
		else//�����ǣ�������д��ǰһ���ļ�������
			ofs << line << std::endl;
	}
	ifs.close();//�ر�ԭʼ�����ļ�
	ofs.close();//�ر���ʱ�ļ�

	std::remove("Userdata.csv");//ɾ��ԭʼ�����ļ�
	std::rename("temp.csv", "Userdata.csv");//����ʱ�ļ�����Ϊԭʼ�����ļ���
	return flag==1?true:false;
}

bool UserManager::changeNickname(User& u, std::string newNickname)
{//�޸��ǳƣ���ѯ�˺��Ƿ�һ��
	int flag = 0;//�ı�ɹ�����־λ
	std::string needchangestr = u.getAccount();//������Ҫɾ�����е�����

	std::ifstream ifs("Userdata.csv");//��ԭʼ�����ļ�
	std::ofstream ofs("temp.csv");//����������ʱ�ļ�

	std::string line;//���ڴ洢���ж���ԭʼ��������
	std::string account;//�洢ԭ�����˺�
	std::string password;//�洢ԭ��������
	while (std::getline(ifs, line)) {//���ж�ȡ��line
		int times = 0;
		int test = 0;//���ڱ�֤times==1����ֻ����һ��
		for (int i = 0; i < line.size(); i++)//�������ݸ�ʽ���ҵ���1�����ŵ��±꣬��ȡ֮ǰ���Ӵ�����account
		{
			if (line[i] == ',')
				times++;
			if (times == 1 && test==0)
			{
				account = line.substr(0, i);//�ҵ���1�����ţ���line�ַ�����0��ʼ��i���ȵ��Ӵ���account��Ҳ�����˺š�
				test = 1;
			}
			else if (times == 2)
			{
				password = line.substr(account.size()+1,i-account.size()-1);//����1�����ź����һ���ַ�����ʼ�ַ���һֱ���ڶ�������ǰ������Ҳ�������롿
				break;
			}
		}
		if (account == needchangestr && flag==0)//��line����Ҫ�޸ĵ���һ��
		{
			ofs << u.getAccount() + "," + password + "," + newNickname << std::endl;//��д���޸ĺ������
			flag = 1;
		}
		else//�����ǣ�������д��ǰһ���ļ�������
			ofs << line << std::endl;
	}
	ifs.close();//�ر�ԭʼ�����ļ�
	ofs.close();//�ر���ʱ�ļ�

	std::remove("Userdata.csv");//ɾ��ԭʼ�����ļ�
	std::rename("temp.csv", "Userdata.csv");//����ʱ�ļ�����Ϊԭʼ�����ļ���
	return flag == 1 ? true : false;
}
