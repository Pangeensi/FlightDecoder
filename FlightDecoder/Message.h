#pragma once
#include <iostream>
#include "HTTP.h"
#include "TCP.h"
#include "Stack.h"
#ifndef _MESSAGE_H_
#define _MESSAGE_H_SynCodeArray
#define SYN_SIZE 33		//同步帧大小
#define CK_SIZE	 5		//TCP校验字大小
#define HTTP_PTSYN	17	//HTTP帧协议区帧结尾码大小
#define TCP_PTSYN	17	//TCP帧协议期帧结尾码大小
#define MSG_STATUS_SEARCH	0x01	//搜索状态标志位，表示当前读到的数据为乱码
#define MSG_STATUS_HTTP		0x02	//HTTP状态标志位，表示当前读到的数据为HTTP帧的数据或负载
#define MSG_STATUS_TCP		0x03	//TCP状态标志位，表示当前读到的数据为TCP帧的数据或负载
typedef unsigned char uchar;
uchar frameBlockSynCodeArray[SYN_SIZE] = "0edd242b37edb827eb9e60c508004500";	//帧块同步数据
uchar tcpCkSynCodeArray[CK_SIZE] = "05dc";										//TCP帧校验字数据
uchar httpFinSynCodeArray[HTTP_PTSYN] = "50180ff5xxxx0000";						//HTTP帧协议区帧结尾码
uchar tcpFinSynCodeArray[TCP_PTSYN] = "50100ff5xxxx0000";						//TCP帧协议区帧结尾码
class Message
{
private:
	int _TCPNum = 10;																//默认TCP帧的容量
	int _TCPPosi = -1;																//TCP帧的规模
	Stack<uchar> _synCache = Stack<uchar>(SYN_SIZE);								//各类帧同步缓存
	Stack<uchar> _frameBlockSynCode = Stack<uchar>(frameBlockSynCodeArray, 0, SYN_SIZE);	//帧块同步校验栈
	Stack<uchar> _tcpCkSynCode = Stack<uchar>(tcpCkSynCodeArray, 0, 5);						//TCP校验栈
	Stack<uchar> _httpFinSynCode = Stack<uchar>(httpFinSynCodeArray, 0, HTTP_PTSYN);		//HTTP帧协议区帧结尾识别栈
	Stack<uchar> _tcpFinSynCode = Stack<uchar>(tcpFinSynCodeArray, 0, TCP_PTSYN);			//TCP帧协议区帧结尾识别栈
	ADSB_HTTP _HTTP = ADSB_HTTP();				//HTTP帧
	ADSB_TCP* _TCP = new ADSB_TCP[_TCPNum];		//TCP帧
	uchar	_msgStatus = MSG_STATUS_SEARCH;									//帧标志位
	int i = 0;
protected:
	//帧底层的重新装填和同步
	void frameReload(Stack<uchar>*, int, uchar*);								//数据帧重新装填函数
	bool frameSyn(Stack<uchar>*, Stack<uchar>*, uchar, void (Message::*) ());	//数据帧同步函数
	//Message块帧的重新装填和同步
	void frameBlockReload(void);			//帧块同步重新装填函数
	bool frameBlockSyn(Stack<uchar>*);		//每个Message中的帧块同步函数
	//TCP校验字的重新装填和同步
	void tcpCkReload(void);					//TCP校验帧帧重新装填函数
	bool tcpCkSyn(Stack<uchar>*);			//对已捕获帧同步的帧进行TCP校验
	//TCP帧中帧结尾码的重新装填和同步
	void tcpFinReload(void);				//TCP帧中帧结尾码的重新装填函数
	bool tcpFinSyn(Stack<uchar>*);			//TCP帧中帧结尾码的结尾捕捉函数
	//HTTP帧中帧结尾码的重新装填和同步
	void httpFinReload(void);				//HTTP帧中帧结尾码的重新装填函数
	bool httpFinSyn(Stack<uchar>*);			//HTTP帧中帧结尾码的结尾捕捉函数
public:
	Message() { frameBlockReload(); tcpCkReload(); }	//Message类的构造函数
	~Message() { delete[] _TCP; }						//Message类的析构函数
	bool HTTPClassify(Stack<uchar>*);					//仅含HTTP的数据分类函数
	bool dataClassify(Stack<uchar>*);					//数据分类函数
	void traverse(void);								//Message的遍历函数
};
/*========================================

数据帧重新装填函数
输入：指定的数据帧、数据帧的指定规模和未封装时的数组数据

=========================================*/
void Message::frameReload(Stack<uchar>* data,int dataSize,uchar* codeArray)
{
	while (!data->empty())				//重新装填帧同步栈
		data->pop();
	for (int i = dataSize - 2; i >= 0; i--)
		data->push(codeArray[i]);
}
/*========================================

数据帧同步函数
输入：数据栈，指定的数据帧，捕获成功后Message的状态，重新装填函数
输出：在数据栈结束前是否捕捉到帧同步

注：此函数目前不可用

=========================================*/
bool  Message::frameSyn(Stack<uchar>* data, Stack<uchar>* synCode, uchar status, void(Message::* reload)())	//数据帧同步函数
{
	while ((synCode->pop() == _synCache.push(data->pop())))
	{
		if (!synCode->size())//只要帧同步栈清空，则说明捕捉到帧同步
		{
			reload;
			_msgStatus = status;			//帧同步成功，从搜寻状态变为HTTP状态
			//while (!_synCache.empty())
				//_synCache.pop();			//缓存中为帧同步或乱码，丢弃
			return true;
		}
	}
	reload;									//不是帧同步，数据为乱码或信息
	return false;
}
/*========================================

每个Message中的帧块同步函数
输入：数据栈
输出：在数据栈结束前是否捕捉到TCP或HTTP帧同步

=========================================*/
bool Message::frameBlockSyn(Stack<uchar>* data)
{
	
	while ((_frameBlockSynCode.pop() == _synCache.push(data->pop())))
	{
		if (!_frameBlockSynCode.size())//只要帧同步栈清空，则说明捕捉到帧同步
		{
			frameBlockReload();
			_msgStatus = MSG_STATUS_HTTP;	//帧同步成功，从搜寻状态变为HTTP状态
			//while (!_synCache.empty())
				//_synCache.pop();			//缓存中为帧同步或乱码，丢弃
			return true;
		}
	}
	frameBlockReload();						//不是帧同步，数据为乱码或信息
	return false;
	
	//return frameSyn(data, &_frameBlockSynCode, MSG_STATUS_HTTP, &Message::frameBlockReload);
}
/*========================================

整个Message中的帧块同步码重新装填函数

=========================================*/
void Message::frameBlockReload(void)
{
	frameReload(&_frameBlockSynCode, SYN_SIZE, frameBlockSynCodeArray);
}
/*========================================

对已捕获帧同步的帧进行TCP校验
返回值：当前帧是否为TCP帧

=========================================*/
bool Message::tcpCkSyn(Stack<uchar>* data)
{
	while ((_tcpCkSynCode.pop() == _synCache.push(data->pop())))
	{
		if (!_tcpCkSynCode.size())//只要帧同步栈清空，则说明捕捉到帧同步
		{
			tcpCkReload();
			_msgStatus = MSG_STATUS_TCP;
			while (!_synCache.empty())
				_synCache.pop();			//缓存中为校验或乱码，丢弃
			return true;
		}
	}
	tcpCkReload();							//不是校验，帧结束或是HTTP
	_msgStatus = MSG_STATUS_SEARCH;
	return false;
}
/*========================================

TCP校验帧重新装填函数

=========================================*/
void Message::tcpCkReload(void)
{
	frameReload(&_tcpCkSynCode, CK_SIZE, tcpCkSynCodeArray);
}
/*========================================

仅对进行同步、缓存、查询状态，调用指定端口，不涉及TCP校验
输入：数据栈
返回值：是否找到HTTP帧

=========================================*/
bool Message::HTTPClassify(Stack<uchar>* data)
{
	if (frameBlockSyn(data))
	{
		if (_msgStatus == MSG_STATUS_HTTP)	//HTTP状态无数据
		{
			//获取帧协议
			std::cout << std::endl << "HTTP" << std::endl;
			return true;
		}
	}
	else								//乱码
	{
		while (!_synCache.empty())
		{
			printf("%c", _synCache.pop());
			i++;
			if (i > 3)
			{
				printf(" ");
				i = 0;
			}
		}
		return false;
	}
}
/*========================================

将数据进行同步、缓存、查询状态，调用指定端口
输入：数据栈
返回值：是否完成本Message帧

=========================================*/
bool Message::dataClassify(Stack<uchar>* data)	//数据分类函数
{
	if(frameBlockSyn(data))
		if (tcpCkSyn(data))	//TCP状态无数据
		{
			//获取协议帧
			std::cout << std::endl << "TCP" << std::endl;
			_TCPPosi++;		//捕获新的TCP，位置自增
			return false;
		}
		else				//此帧结束
		{
			std::cout << std::endl << "FRAME END" << std::endl;
			while (!_synCache.empty())
				data->push(_synCache.pop());			//检查到段错误以后把数据吐回去
			return true;
		}
	else	//通过标志位分析其状态
	{
		if (_msgStatus == MSG_STATUS_HTTP)	//HTTP状态
		{
			while (!_synCache.empty())
			{
				_HTTP.cpDataFramePbit(&_synCache);
				i++;
				if (i > 3)
				{
					printf(" ");
					i = 0;
				}
			}
			return false;
		}
		else								//TCP状态
		{
			while (!_synCache.empty())
			{
				_TCP[_TCPPosi].cpDataFramePbit(&_synCache);
				//printf("%d", _TCPPosi);
				i++;
				if (i > 3)
				{
					printf(" ");
					i = 0;
				}
			}
			return false;
		}
	}

}
/*========================================

Message的遍历函数

=========================================*/
void Message::traverse(void)
{
	std::cout << std::endl << "HTTP Traverse" << std::endl;
	_HTTP.traverse();
	for (int i = 0; i < _TCPPosi + 1; i++)
	{
		std::cout << std::endl << "TCP Traverse" << std::endl;
		_TCP[i].traverse();
	}
}
#endif