#pragma once
#ifndef _MSG_LIST_NODE_H_
#define _MSG_LIST_NODE_H_
#define Posi(T) ListNode<T>*
template <typename T>
struct ListNode
{
	T _elem;		//列表节点元素
	Posi(T) pred;	//前驱位置
	Posi(T)	succ;	//后继位置
	ListNode() {}									//对header和trailer的构造
	ListNode(T e, Posi(T) p=NULL, Posi(T) s=NULL)	//对一般列表节点的构造
		:_elem(e),pred(p), succ(s) {}

	Posi(T) insertAsPred(T const& e);	//作为前驱插入
	Posi(T) insertAsSucc(T const& e);	//作为后继插入
};
/*============================================

创建新节点并作为前驱插入
输入值：插入的节点元素
输出值：新增节点的地址

=============================================*/
template <typename T>
Posi(T) ListNode<T>::insertAsPred(T const& e)
{
	ListNode Node(e, this->pred, this);
	return this->pred = &Node;

}
/*============================================

创建新节点并作为后继插入
输入值：插入的节点元素
输出值：新增节点的地址

=============================================*/
template <typename T>
Posi(T) ListNode<T>::insertAsSucc(T const& e)
{
	ListNode Node(e, this, this->succ);
	return this->succ = &Node;
}
#endif