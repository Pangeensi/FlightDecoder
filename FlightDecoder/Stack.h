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
	void reverse(void);					//ջ��ת����
public:
	//ջ�Ĺ���
	Stack(Rank c = DEFAULT_CAPACITY)	//�չ��죬ջ�ڲ�û��Ĭ��Ԫ��
	{
		this->_elem = new T[this->_capacity = c];
		this->_size = 0;
	}
	Stack(T* data, Rank lo, Rank hi)	//���ڰ�װ�Ĺ��죬�������װΪջ
	{
		this->_size = hi - lo;
		this->_elem = new T[this->_capacity = 2 * this->_size];
		for (int i = 0; i < this->_size; i++)
			this->_elem[i] = data[i];
		reverse();						//����������ṹ��ת��������β����Ϊջ�����Լӿ���������ٶ�
	}
	//ջ�Ļ�������
	Rank size(void);		//��ȡջ�Ĺ�ģ
	bool empty(void);		//�ж�ջ�Ƿ�Ϊ��
	T push(T const& e);		//��ջ����
	T pop(void);			//��ջ����
	T& top(void);			//����ջ��Ԫ��
	//ջ����������
};
#endif
/*===========================================

��ջ��������������ת

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

��ȡջ�Ĺ�ģ
����ֵ����ǰջ���д洢��Ԫ�ظ���

============================================*/
template<typename T>
inline Rank Stack<T>::size(void)
{
	return this->_size;
}
/*===========================================

��ȡջ�Ƿ�Ϊ�յ���Ϣ
����ֵ��ջΪ���򷵻�true�����򷵻�false

============================================*/
template<typename T>
inline bool Stack<T>::empty(void)
{
	return !this->_size;
}
/*===========================================

��ջ����
����ֵ������ջ��Ԫ��
����ֵ����Ԫ�ص�ֵ������ʶ�𱸷ݵ�

============================================*/
template<typename T>
inline T Stack<T>::push(T const& e)
{
	this->insert(e, this->_size);
	return e;
}
/*===========================================

��ջ����
����ֵ����ջԪ��

============================================*/
template<typename T>
inline T Stack<T>::pop(void)
{
	return this->remove(size() - 1);
}
/*===========================================

ȡ��ջ��Ԫ�����ڵĵ�ַ
����ֵ��ջ��Ԫ�����ڵĵ�ַ

============================================*/
template<typename T>
inline T& Stack<T>::top(void)
{
	return (*this)[size() - 1];
}
