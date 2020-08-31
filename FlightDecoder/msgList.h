#pragma once
#ifndef _MSG_LIST_H_
#define _MSG_LIST_H_
#include "msgListNode.h"
template <typename T>
class msgList
{
private:
	Posi(T) header;					//ͷԪ�غ�βԪ��
	Posi(T) trailer;
	int _size;						//�б�Ĺ�ģ
protected:
	void initialize(void);			//һ���б��ʼ������
public:
	msgList() { initialize(); }		//�б��캯��
	~msgList()						//�б���������
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
	Posi(T) insertAsPred(Posi(T) p, T const& e);	//��Ϊǰ������Ԫ��
	Posi(T) insertAsSucc(Posi(T) p, T const& e);	//��Ϊ��̲���Ԫ��
	Posi(T) firstNode(T const& e);					//��ȡ�б��׽ڵ�λ��
};
/*============================================

һ���б��ʼ������������һ�����б�

=============================================*/
template <typename T>
void msgList<T>::initialize()
{
	header = new ListNode<T>;		//ͷβ�ڵ�ʵ����
	trailer = new ListNode<T>;
	header->pred = NULL;			//��β����
	header->succ = trailer;
	trailer->pred = header;
	trailer->succ = NULL;
	_size = 0;						//��ģ����
}
/*============================================

��ָ��Ԫ��ǰ�����µ��б�ڵ�
����ֵ��ָ��Ԫ��λ�ú�Ԫ��ֵ
���ֵ������Ԫ��λ��

=============================================*/
template<typename T>
Posi(T) msgList<T>::insertAsPred(Posi(T) p, T const& e)
{
	return p->insertAsPred(e);
}
/*============================================

��ָ��Ԫ�غ�����µ��б�ڵ�
����ֵ��ָ��Ԫ��λ�ú�Ԫ��ֵ
���ֵ������Ԫ��λ��

=============================================*/
template<typename T>
Posi(T) msgList<T>::insertAsSucc(Posi(T) p, T const& e)
{
	return p->insertAsSucc(e);
}
/*============================================

��ȡ�б��׽ڵ�λ�ã�����б�Ϊ�գ����Զ������׽ڵ�
����ֵ�����ܵĲ���Ԫ�أ�����б�Ϊ�գ���������Ч
���ֵ���׽ڵ��λ��

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