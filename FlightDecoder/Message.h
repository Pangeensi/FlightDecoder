#pragma once
#include <iostream>
#include "HTTP.h"
#include "TCP.h"
#include "Stack.h"
#ifndef _MESSAGE_H_
#define _MESSAGE_H_
#define SYN_SIZE 33		//同步帧大小
#define CK_SIZE	 5		//TCP校验字大小
#define MSG_STATUS_SEARCH	0x01
#define MSG_STATUS_HTTP		0x02
#define MSG_STATUS_TCP		0x03
typedef unsigned char uchar;
uchar SynCodeArray[SYN_SIZE] =	"0edd242b37edb827eb9e60c508004500";		//帧同步数据
uchar ckWordArray[CK_SIZE] =	"05dc";									//TCP帧校验字
class Message
{
private:
	int _TCPNum = 10;														//默认TCP帧的容量
	int _TCPPosi = -1;														//TCP帧的规模
	Stack<uchar> _synCache = Stack<uchar>(SYN_SIZE);						//帧同步缓存
	Stack<uchar> _synCode = Stack<uchar>(SynCodeArray, 0, SYN_SIZE);		//帧同步栈
	Stack<uchar> _ckWord = Stack<uchar>(ckWordArray, 0, 5);					//TCP校验栈
	ADSB_HTTP _HTTP = ADSB_HTTP();				//HTTP帧
	ADSB_TCP* _TCP = new ADSB_TCP[_TCPNum];		//TCP帧
	uchar	_msgStatus = MSG_STATUS_SEARCH;									//帧标志位
	int i = 0;
protected:
	void frameReload(Stack<uchar>*, int, uchar*);								//数据帧重新装填函数
	bool frameSyn(Stack<uchar>*, Stack<uchar>*, uchar, void (Message::*) ());	//数据帧同步函数
public:
	Message() { synReload(); TCPCkReload(); }					//Message类的构造函数
	~Message() { delete[] _TCP; }	//Message类的析构函数
	bool synFrame(Stack<uchar>*);	//帧同步函数
	void synReload(void);			//帧同步帧重新装填函数
	bool TCPCk(Stack<uchar>*);		//对已捕获帧同步的帧进行TCP校验
	void TCPCkReload(void);				//TCP校验帧帧重新装填函数
	bool HTTPClassify(Stack<uchar>*);	//仅含HTTP的数据分类函数
	bool dataClassify(Stack<uchar>*);	//数据分类函数
	void traverse(void);				//Message的遍历函数
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

注：目前不可用

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

帧同步函数
输入：数据栈
输出：在数据栈结束前是否捕捉到帧同步

=========================================*/
bool Message::synFrame(Stack<uchar>* data)
{
	
	while ((_synCode.pop() == _synCache.push(data->pop())))
	{
		if (!_synCode.size())//只要帧同步栈清空，则说明捕捉到帧同步
		{
			synReload();
			_msgStatus = MSG_STATUS_HTTP;	//帧同步成功，从搜寻状态变为HTTP状态
			//while (!_synCache.empty())
				//_synCache.pop();			//缓存中为帧同步或乱码，丢弃
			return true;
		}
	}
	synReload();							//不是帧同步，数据为乱码或信息
	return false;
	
	//return frameSyn(data, &_synCode, MSG_STATUS_HTTP, &Message::synReload);
}
/*========================================

帧同步帧重新装填函数

=========================================*/
void Message::synReload(void)
{
	frameReload(&_synCode, SYN_SIZE, SynCodeArray);
}
/*========================================

对已捕获帧同步的帧进行TCP校验
返回值：当前帧是否为TCP帧

=========================================*/
bool Message::TCPCk(Stack<uchar>* data)
{
	while ((_ckWord.pop() == _synCache.push(data->pop())))
	{
		if (!_ckWord.size())//只要帧同步栈清空，则说明捕捉到帧同步
		{
			TCPCkReload();
			_msgStatus = MSG_STATUS_TCP;
			while (!_synCache.empty())
				_synCache.pop();			//缓存中为校验或乱码，丢弃
			return true;
		}
	}
	TCPCkReload();							//不是校验，帧结束或是HTTP
	_msgStatus = MSG_STATUS_SEARCH;
	return false;
}
/*========================================

TCP校验帧重新装填函数

=========================================*/
void Message::TCPCkReload(void)
{
	frameReload(&_ckWord, CK_SIZE, ckWordArray);
}
/*========================================

仅对进行同步、缓存、查询状态，调用指定端口，不涉及TCP校验
输入：数据栈
返回值：是否找到HTTP帧

=========================================*/
bool Message::HTTPClassify(Stack<uchar>* data)
{
	if (synFrame(data))
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
	if(synFrame(data))
		if (TCPCk(data))	//TCP状态无数据
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