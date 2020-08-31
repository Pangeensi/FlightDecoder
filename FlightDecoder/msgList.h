#pragma once
#ifndef _MSG_LIST_H_
#define _MSG_LIST_H_
#include "msgListNode.h"
template <typename T>
class msgList
{
private:
	Posi(T) header;					//头元素和尾元素
	Posi(T) trailer;
	int _size;						//列表的规模
protected:
	void initialize(void);			//一般列表初始化函数
public:
	msgList() { initialize(); }		//列表构造函数
	~msgList()						//列表析构函数
	{
		for (int i = 0; i < _size; i++)
		{
			Posi(T) del = header->succ;
			header->succ = header->succ->succ;
			delete[] del;
		}
		delete[] header;
		delete[] trailer;
	}
	Posi(T) insertAsPred(Posi(T) p, T const& e);	//作为前驱插入元素
	Posi(T) insertAsSucc(Posi(T) p, T const& e);	//作为后继插入元素
	Posi(T) firstNode(T const& e);					//获取列表首节点位置
};
/*============================================

一般列表初始化函数，创建一个空列表

=============================================*/
template <typename T>
void msgList<T>::initialize()
{
	header = new ListNode<T>;		//头尾节点实例化
	trailer = new ListNode<T>;
	header->pred = NULL;			//首尾相连
	header->succ = trailer;
	trailer->pred = header;
	trailer->succ = NULL;
	_size = 0;						//规模清零
}
/*============================================

在指定元素前插入新的列表节点
输入值：指定元素位置和元素值
输出值：插入元素位置

=============================================*/
template<typename T>
Posi(T) msgList<T>::insertAsPred(Posi(T) p, T const& e)
{
	return p->insertAsPred(e);
}
/*============================================

在指定元素后插入新的列表节点
输入值：指定元素位置和元素值
输出值：插入元素位置

=============================================*/
template<typename T>
Posi(T) msgList<T>::insertAsSucc(Posi(T) p, T const& e)
{
	return p->insertAsSucc(e);
}
/*============================================

获取列表首节点位置，如果列表为空，则自动创建首节点
输入值：可能的插入元素，如果列表不为空，则输入无效
输出值：首节点的位置

=============================================*/
template <typename T>
Posi(T) msgList<T>::firstNode(T const& e)
{
	if (header->succ == trailer)
		return insertAsSucc(header, e);
	else
		return header->succ;
}
#endif