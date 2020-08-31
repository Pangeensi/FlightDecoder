#pragma once
#ifndef _VECTOR_H_
#define _VECTOR_H_
#define DEFAULT_CAPACITY 10
typedef int Rank;
template <typename T>
class Vector
{
protected:
	T* _elem;				//向量的数据域
	Rank _size;				//向量的规模
	Rank _capacity;			//向量的容量
public:
	void expand(void);					//容量扩展
	void shrink(void);					//容量缩减
	Rank insert(T const& e, Rank r);	//插入元素
	T remove(Rank lo, Rank hi);			//批量删除元素
	T remove(Rank r);					//单独删除元素
	Rank size(void);					//获取向量的规模
	bool empty(void);					//判断向量是否为空
	Vector(Rank c = DEFAULT_CAPACITY)	//空构造，向量内部没有默认元素
	{
		_elem = new T[_capacity = c];
		_size = 0;
	}
	~Vector() { delete[] _elem; }		//向量的析构
};
/*===========================================

向量容量扩充，默认为扩充到原先的两倍大

============================================*/
template<typename T>
void Vector<T>::expand(void)
{
	T* _newElem = new T[_capacity * 2];		//申请新的内存块
	for (int i = 0; i < _size; i++)
		_newElem[i] = _elem[i];				//复制数据
	delete[] _elem;							//删除原内存块
	_elem = _newElem;						//标记新内存块地址
	_capacity *= 2;
}
/*===========================================

向量容量缩减，默认为缩小到原先的二分之一

============================================*/
template<typename T>
void Vector<T>::shrink(void)
{
	T* _newElem = new T[_capacity / 2];		//申请新的内存块
	for (int i = 0; i < _size; i++)
		_newElem[i] = _elem[i];				//复制数据
	delete[] _elem;							//删除原内存块
	_elem = _newElem;						//标记新内存块地址
	_capacity /= 2;
}
/*===========================================

插入元素
输入值：所插入的元素数据和插入的位置
返回值：被插入元素的秩

============================================*/
template<typename T>
Rank Vector<T>::insert(T const& e, Rank r)
{
	if (_size + 1 > _capacity)
		expand();							//插入前检查容量，如果不足就进行扩充
	int i = _size;
	for (; i > r; i--)
		_elem[i] = _elem[i - 1];			//元素后移
	_elem[i] = e;							//插入元素
	_size++;
	return r;
}
/*===========================================

批量删除元素
输入值：被删除的元素数据位置
返回值：被删除的最高位元素

============================================*/
template<typename T>
T Vector<T>::remove(Rank lo, Rank hi)
{
	T eBak = _elem[hi - 1];
	_size = _size - (hi - lo);					//先减少规模标识，以免在完成覆盖后做无用的运算	
	for (int i = lo; i < _size; i++)
		_elem[i] = _elem[i + hi - lo];			//使用后继元素对被移除元素进行覆盖
	/*
	if (_size < (_capacity / 2))
		shrink();								//缩减完后，如果容量过大则进行缩减
		*/
	return eBak;
}
/*===========================================

删除元素
输入值：被删除的元素数据位置
返回值：被删除的元素

============================================*/
template<typename T>
inline T Vector<T>::remove(Rank r)
{
	return remove(r, r + 1);
}
/*===========================================

获取栈的规模
返回值：当前栈当中存储的元素个数

============================================*/
template<typename T>
inline Rank Vector<T>::size(void)
{
	return this->_size;
}
/*===========================================

获取栈是否为空的信息
返回值：栈为空则返回true，否则返回false

============================================*/
template<typename T>
inline bool Vector<T>::empty(void)
{
	return !this->_size;
}
#endif