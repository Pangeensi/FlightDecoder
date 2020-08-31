#pragma once
#ifndef _STACK_H_
#define _STACK_H_
#include "Vector.h"
#define DEFAULT_CAPACITY 10
typedef int Rank;
template <typename T>
class Stack :protected Vector<T>
{
protected:
	void reverse(void);					//栈翻转操作
public:
	//栈的构造
	Stack(Rank c = DEFAULT_CAPACITY)	//空构造，栈内部没有默认元素
	{
		this->_elem = new T[this->_capacity = c];
		this->_size = 0;
	}
	Stack(T* data, Rank lo, Rank hi)	//基于包装的构造，将数组包装为栈
	{
		this->_size = hi - lo;
		this->_elem = new T[this->_capacity = 2 * this->_size];
		for (int i = 0; i < this->_size; i++)
			this->_elem[i] = data[i];
		reverse();						//将整个数组结构翻转，将数组尾部作为栈顶，以加快程序运行速度
	}
	//栈的基本操作
	Rank size(void);		//获取栈的规模
	bool empty(void);		//判断栈是否为空
	T push(T const& e);		//入栈操作
	T pop(void);			//出栈操作
	T& top(void);			//返回栈顶元素
	//栈的批量操作
};
#endif
/*===========================================

将栈的整个数据区翻转

============================================*/
template<typename T>
inline void Stack<T>::reverse(void)
{
	T* newElem = new T[this->_capacity];
	for (int i = this->_size - 1; i >= 0; i--)
		newElem[i] = this->_elem[this->_size - 1 - i];
	delete[] this->_elem;
	this->_elem = newElem;
}
/*===========================================

获取栈的规模
返回值：当前栈当中存储的元素个数

============================================*/
template<typename T>
inline Rank Stack<T>::size(void)
{
	return this->_size;
}
/*===========================================

获取栈是否为空的信息
返回值：栈为空则返回true，否则返回false

============================================*/
template<typename T>
inline bool Stack<T>::empty(void)
{
	return !this->_size;
}
/*===========================================

入栈操作
输入值：新入栈的元素
返回值：本元素的值，用于识别备份等

============================================*/
template<typename T>
inline T Stack<T>::push(T const& e)
{
	this->insert(e, this->_size);
	return e;
}
/*===========================================

出栈操作
返回值：出栈元素

============================================*/
template<typename T>
inline T Stack<T>::pop(void)
{
	return this->remove(size() - 1);
}
/*===========================================

取出栈顶元素所在的地址
返回值：栈顶元素所在的地址

============================================*/
template<typename T>
inline T& Stack<T>::top(void)
{
	return (*this)[size() - 1];
}
