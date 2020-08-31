#pragma once
#ifndef _ADSB_TCP_H_
#define _ADSB_TCP_H_
#include "Vector.h"
#include "Stack.h"
class ADSB_TCP
{
private:
	Vector<unsigned char>	_ProData = Vector<unsigned char>();						//备用的协议数据头
	Vector<unsigned char>	_DataFrame = Vector<unsigned char>();					//数据区
	int _tcpSize = 0;																//TCP帧的规模
protected:
	void copyFrom(Stack<uchar>*, int, Vector<uchar>*);			//批量封装函数
public:
	ADSB_TCP() {}								//TCP帧的构造函数
	~ADSB_TCP() {}
	void cpProData(Stack<uchar>*);								//备用协议封装函数
	void cpDataFrame(Stack<uchar>* data, int r);				//数据封装函数
	void cpDataFramePbit(Stack<uchar>* data);					//逐位数据封装函数
};
/*========================================

将待处理数据堆栈的指定部分封装在HTTP数据区中
输入：数据栈，封装元素数量，目标数据区

=========================================*/
void ADSB_TCP::copyFrom(Stack<uchar>* data, int num, Vector<uchar>* vtc)
{
	for (int i = 0; i < num; i++)
		vtc->insert(data->pop(), i);
}
/*========================================

将TCP帧的备用协议部分封装在数据区中
输入：数据栈

=========================================*/
void ADSB_TCP::cpProData(Stack<uchar>* data)
{
	copyFrom(data, 76, &_ProData);
}
/*========================================

逐比特将TCP帧的备用协议部分封装在数据区中，供上层调用
输入：数据栈，数据位置

=========================================*/
void ADSB_TCP::cpDataFrame(Stack<uchar>* data, int r)
{
	_DataFrame.insert(data->pop(), r);
}
/*========================================

逐比特将TCP帧的备用协议部分封装在数据区中
输入：数据栈

=========================================*/
void ADSB_TCP::cpDataFramePbit(Stack<uchar>* data)
{
	_DataFrame.insert(data->pop(), _tcpSize++);
}
#endif