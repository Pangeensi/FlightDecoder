#pragma once
#include <iostream>
#include "HTTP.h"
#include "TCP.h"
#include "Stack.h"
#ifndef _MESSAGE_H_
#define _MESSAGE_H_
typedef unsigned char uchar;
uchar SynCodeArray[42] = "0edd 242b 37ed b827 eb9e 60c5 0800 4500\r\n";	//帧同步数据
uchar ckWordArray[5] = "05dc";										//TCP帧校验字
class Message
{
private:
	int _TCPNum = 5;														//默认TCP帧的个数

	Stack<uchar> _synCache = Stack<uchar>(39);								//帧同步缓存
	Stack<uchar> _ckWord = Stack<uchar>(ckWordArray, 0, 5);					//TCP校验栈
	ADSB_HTTP _HTTP = ADSB_HTTP();				//HTTP帧
	ADSB_TCP* _TCP = new ADSB_TCP[_TCPNum];		//TCP帧
public:
	Message() {}					//Message类的构造函数
	~Message() { delete[] _TCP; }	//Message类的析构函数
	bool synFrame(Stack<uchar>*);	//帧同步函数
	void synReload(void);			//帧同步帧重新装填函数
	bool TCPCk(Stack<uchar>* data);	//对已捕获帧同步的帧进行TCP校验
	void TCPCkReload(void);			//TCP校验帧帧重新装填函数
	Stack<uchar> _synCode = Stack<uchar>(SynCodeArray, 0, 40);				//帧同步栈
};
/*========================================

帧同步函数
输入：数据栈
输出：在数据栈结束前是否捕捉到帧同步

=========================================*/
bool Message::synFrame(Stack<uchar>* data)
{
	while ((_synCode.pop() == _synCache.push(data->pop())))
	{
		//std::cout << _synCode.size()<<std::endl;
		if (!_synCode.size())//只要帧同步栈清空，则说明捕捉到帧同步
		{
			synReload();
			return true;
		}
		printf("match data:%c syn:%c size:%d empty:%c\n", data->push(data->pop()), _synCode.push(_synCode.pop()), _synCode.size(), _synCode.empty() ? 'Y' : 'N');
	
		/*
		while (!_synCache.empty())
			data->push(_synCache.pop());		//发现不同步，数据回灌
			*/ //错误数据暂改为丢弃
	}
	synReload();
	return false;
}
/*========================================

帧同步帧重新装填函数

=========================================*/
void Message::synReload(void)
{
	while (!_synCode.empty())				//重新装填帧同步栈
		_synCode.pop();
	for (int i = 40; i >= 0; i--)
		_synCode.push(SynCodeArray[i]);
}
/*========================================

对已捕获帧同步的帧进行TCP校验
返回值：当前帧是否为TCP帧

=========================================*/
bool Message::TCPCk(Stack<uchar>* data)
{
	while ((_ckWord.pop() == _synCache.push(data->pop())))
	{
		//std::cout << _synCode.size()<<std::endl;
		if (!_ckWord.size())//只要帧同步栈清空，则说明捕捉到帧同步
		{
			TCPCkReload();
			return true;
		}
		printf("match data:%c syn:%c size:%d empty:%c\n", data->push(data->pop()), _ckWord.push(_ckWord.pop()), _ckWord.size(), _ckWord.empty() ? 'Y' : 'N');

		/*
		while (!_synCache.empty())
			data->push(_synCache.pop());		//发现不同步，数据回灌
			*/ //错误数据暂改为丢弃
	}
	TCPCkReload();
	return false;
}
/*========================================

TCP校验帧重新装填函数

=========================================*/
void Message::TCPCkReload(void)
{
	while (!_ckWord.empty())				//重新装填帧TCP校验栈
		_ckWord.pop();
	for (int i = 0; i < 5; i++)
		_ckWord.push(ckWordArray[i]);
}
#endif