#pragma once
#include <iostream>
#include "HTTP.h"
#include "TCP.h"
#include "Stack.h"
#ifndef _MESSAGE_H_
#define _MESSAGE_H_
#define SYN_SIZE 33		//同步帧大小
#define CK_SIZE	 5		//TCP校验字大小
typedef unsigned char uchar;
uchar SynCodeArray[SYN_SIZE] =	"0edd242b37edb827eb9e60c508004500";		//帧同步数据
uchar ckWordArray[CK_SIZE] =	"05dc";									//TCP帧校验字
class Message
{
private:
	int _TCPNum = 5;														//默认TCP帧的个数

	Stack<uchar> _synCache = Stack<uchar>(SYN_SIZE);						//帧同步缓存
	Stack<uchar> _synCode = Stack<uchar>(SynCodeArray, 0, SYN_SIZE);		//帧同步栈
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
	void HTTPPack(void);			//将一帧HTTP帧进行封装
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
		if (!_synCode.size())//只要帧同步栈清空，则说明捕捉到帧同步
		{
			synReload();
			return true;
		}
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
	for (int i = SYN_SIZE - 2; i >= 0; i--)
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
		if (!_ckWord.size())//只要帧同步栈清空，则说明捕捉到帧同步
		{
			TCPCkReload();
			return true;
		}
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
	for (int i = CK_SIZE - 2; i >= 0; i--)
		_ckWord.push(ckWordArray[i]);
}
/*========================================

HTTP帧封装函数

=========================================*/
void HTTPPack(void)
{

}
#endif