#pragma once
#ifndef _MSG_LIST_H_
#define _MSG_LIST_H_
#include "msgListNode.h"
typedef int Rank;
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
		/*
		for (int i = 0; i < _size; i++)
		{
			Posi(T) del = header->succ;
			header->succ = header->succ->succ;
			delete del;
		}
		*/
		while (0 < _size)
			remove(header->succ);
		delete[] header;
		delete[] trailer;
	}
	Posi(T) insertAsPred(Posi(T) p, T const& e);	//作为前驱插入元素
	Posi(T) insertAsSucc(Posi(T) p, T const& e);	//作为后继插入元素
	Posi(T) firstNode(T const& e);					//获取列表首节点位置
	bool empty(void);								//返回列表是否为空
	Rank size(void);								//获取列表的规模
	T remove(Posi(T));								//删除节点
	int clear(void);								//清除所有节点
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
	_size++;
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
	_size++;
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
/*============================================

返回列表是否为空
输出值：列表为空与否的标志位

=============================================*/
template<typename T>
inline bool msgList<T>::empty(void)
{
	return !_size;
}
/*===========================================

获取列表的规模
返回值：当前栈当中存储的元素个数

============================================*/
template<typename T>
inline Rank msgList<T>::size(void)
{
	return this->_size;
}
/*===========================================

删除列表节点
输入值：列表节点的位置
返回值：被删除节点的节点元素
============================================*/
template <typename T>
T msgList<T>::remove(Posi(T) p)
{
	T e = p->_elem;
	p->pred->succ = p->succ;
	p->succ->pred = p->pred;
	delete p;
	this->_size--;
	return e;
}
#endif