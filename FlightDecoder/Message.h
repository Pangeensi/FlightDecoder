#pragma once
#include <iostream>
#include "HTTP.h"
#include "TCP.h"
#include "Stack.h"
#ifndef _MESSAGE_H_
#define _MESSAGE_H_
typedef unsigned char uchar;
uchar SynCodeArray[41] = "0edd 242b 37ed b827 eb9e 60c5 0800 4500";	//帧同步数据
class Message
{
private:
	int _TCPNum = 5;														//默认TCP帧的个数
	Stack<uchar> _synCode = Stack<uchar>(SynCodeArray, 0, 41);				//帧同步栈
	Stack<uchar> _synCache = Stack<uchar>(41);								//帧同步缓存
	ADSB_HTTP _HTTP = ADSB_HTTP();				//HTTP帧
	ADSB_TCP* _TCP = new ADSB_TCP[_TCPNum];		//TCP帧
public:
	Message() {}					//Message类的构造函数
	~Message() { delete[] _TCP; }	//Message类的析构函数
	bool synFrame(Stack<uchar>*);	//帧同步函数
	void synReload(void);			//帧同步帧重新装填函数

};
/*========================================

帧同步函数
输入：数据栈
输出：在数据栈结束前是否捕捉到帧同步

=========================================*/
bool Message::synFrame(Stack<uchar>* data)
{
	if (!_synCode.empty())		//只要帧同步栈清空，则说明捕捉到帧同步
	{
		if (!(_synCode.pop() == _synCache.push(data->pop())))
		{
			/*
			while (!_synCache.empty())
				data->push(_synCache.pop());		//发现不同步，数据回灌
				*/ //错误数据暂改为丢弃
			synReload();
			return false;
		}		
	}
	return true;
}
/*========================================

帧同步帧重新装填函数

=========================================*/
void Message::synReload(void)
{
	while (!_synCode.empty())				//重新装填帧同步栈
		_synCode.pop();
	for (int i = 0; i < 16; i++)
		_synCode.push(SynCodeArray[i]);
}
#endif