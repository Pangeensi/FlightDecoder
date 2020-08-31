#pragma once
#ifndef _VECTOR_H_
#define _VECTOR_H_
#define DEFAULT_CAPACITY 10
typedef int Rank;
template <typename T>
class Vector
{
protected:
	T* _elem;				//������������
	Rank _size;				//�����Ĺ�ģ
	Rank _capacity;			//����������
public:
	void expand(void);					//������չ
	void shrink(void);					//��������
	Rank insert(T const& e, Rank r);	//����Ԫ��
	T remove(Rank lo, Rank hi);			//����ɾ��Ԫ��
	T remove(Rank r);					//����ɾ��Ԫ��
	Rank size(void);					//��ȡ�����Ĺ�ģ
	bool empty(void);					//�ж������Ƿ�Ϊ��
	Vector(Rank c = DEFAULT_CAPACITY)	//�չ��죬�����ڲ�û��Ĭ��Ԫ��
	{
		_elem = new T[_capacity = c];
		_size = 0;
	}
	~Vector() { delete[] _elem; }		//����������
};
/*===========================================

�����������䣬Ĭ��Ϊ���䵽ԭ�ȵ�������

============================================*/
template<typename T>
void Vector<T>::expand(void)
{
	T* _newElem = new T[_capacity * 2];		//�����µ��ڴ��
	for (int i = 0; i < _size; i++)
		_newElem[i] = _elem[i];				//��������
	delete[] _elem;							//ɾ��ԭ�ڴ��
	_elem = _newElem;						//������ڴ���ַ
	_capacity *= 2;
}
/*===========================================

��������������Ĭ��Ϊ��С��ԭ�ȵĶ���֮һ

============================================*/
template<typename T>
void Vector<T>::shrink(void)
{
	T* _newElem = new T[_capacity / 2];		//�����µ��ڴ��
	for (int i = 0; i < _size; i++)
		_newElem[i] = _elem[i];				//��������
	delete[] _elem;							//ɾ��ԭ�ڴ��
	_elem = _newElem;						//������ڴ���ַ
	_capacity /= 2;
}
/*===========================================

����Ԫ��
����ֵ���������Ԫ�����ݺͲ����λ��
����ֵ��������Ԫ�ص���

============================================*/
template<typename T>
Rank Vector<T>::insert(T const& e, Rank r)
{
	if (_size + 1 > _capacity)
		expand();							//����ǰ����������������ͽ�������
	int i = _size;
	for (; i > r; i--)
		_elem[i] = _elem[i - 1];			//Ԫ�غ���
	_elem[i] = e;							//����Ԫ��
	_size++;
	return r;
}
/*===========================================

����ɾ��Ԫ��
����ֵ����ɾ����Ԫ������λ��
����ֵ����ɾ�������λԪ��

============================================*/
template<typename T>
T Vector<T>::remove(Rank lo, Rank hi)
{
	T eBak = _elem[hi - 1];
	_size = _size - (hi - lo);					//�ȼ��ٹ�ģ��ʶ����������ɸ��Ǻ������õ�����	
	for (int i = lo; i < _size; i++)
		_elem[i] = _elem[i + hi - lo];			//ʹ�ú��Ԫ�ضԱ��Ƴ�Ԫ�ؽ��и���
	/*
	if (_size < (_capacity / 2))
		shrink();								//���������������������������
		*/
	return eBak;
}
/*===========================================

ɾ��Ԫ��
����ֵ����ɾ����Ԫ������λ��
����ֵ����ɾ����Ԫ��

============================================*/
template<typename T>
inline T Vector<T>::remove(Rank r)
{
	return remove(r, r + 1);
}
/*===========================================

��ȡջ�Ĺ�ģ
����ֵ����ǰջ���д洢��Ԫ�ظ���

============================================*/
template<typename T>
inline Rank Vector<T>::size(void)
{
	return this->_size;
}
/*===========================================

��ȡջ�Ƿ�Ϊ�յ���Ϣ
����ֵ��ջΪ���򷵻�true�����򷵻�false

============================================*/
template<typename T>
inline bool Vector<T>::empty(void)
{
	return !this->_size;
}
#endif