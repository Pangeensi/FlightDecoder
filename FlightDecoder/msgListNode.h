#pragma once
#ifndef _MSG_LIST_NODE_H_
#define _MSG_LIST_NODE_H_
#define Posi(T) ListNode<T>*
template <typename T>
struct ListNode
{
	T _elem;		//�б�ڵ�Ԫ��
	Posi(T) pred;	//ǰ��λ��
	Posi(T)	succ;	//���λ��
	ListNode() {}									//��header��trailer�Ĺ���
	ListNode(T e, Posi(T) p=NULL, Posi(T) s=NULL)	//��һ���б�ڵ�Ĺ���
		:_elem(e),pred(p), succ(s) {}

	Posi(T) insertAsPred(T const& e);	//��Ϊǰ������
	Posi(T) insertAsSucc(T const& e);	//��Ϊ��̲���
};
/*============================================

�����½ڵ㲢��Ϊǰ������
����ֵ������Ľڵ�Ԫ��
���ֵ�������ڵ�ĵ�ַ

=============================================*/
template <typename T>
Posi(T) ListNode<T>::insertAsPred(T const& e)
{
	ListNode Node(e, this->pred, this);
	return this->pred = &Node;

}
/*============================================

�����½ڵ㲢��Ϊ��̲���
����ֵ������Ľڵ�Ԫ��
���ֵ�������ڵ�ĵ�ַ

=============================================*/
template <typename T>
Posi(T) ListNode<T>::insertAsSucc(T const& e)
{
	ListNode Node(e, this, this->succ);
	return this->succ = &Node;
}
#endif