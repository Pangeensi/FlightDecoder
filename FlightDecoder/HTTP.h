#pragma once
#ifndef _ADSB_HTTP_H_
#define _ADSB_HTTP_H_
#include "Vector.h"
#include "Stack.h"
typedef unsigned char uchar;
class ADSB_HTTP
{
private:
	Vector<uchar>	_ProData = Vector<uchar>();						//���õ�Э������ͷ
	Vector<uchar>	_DataFrame = Vector<uchar>();					//������
	int _httpSize = 0;												//HTTP֡�Ĺ�ģ
protected:
	void copyFrom(Stack<uchar>*, int,Vector<uchar>*);			//������װ����
public:
	ADSB_HTTP() {}												//HTTP֡�Ĺ��캯��
	~ADSB_HTTP() {}
	void cpProData(Stack<uchar>*);								//����Э���װ����
	void cpDataFrame(Stack<uchar>* data, int r);				//���ݷ�װ����
	void cpDataFramePbit(Stack<uchar>* data);					//��λ���ݷ�װ����
	void traverse(void);										//HTTP��������
};
/*========================================

�����������ݶ�ջ��ָ�����ַ�װ��HTTP��������
���룺����ջ����װԪ��������Ŀ��������

=========================================*/
void ADSB_HTTP::copyFrom(Stack<uchar>* data, int num, Vector<uchar>* vtc)
{
	for (int i = 0; i < num; i++)
		vtc->insert(data->pop(), i);
}
/*========================================

��HTTP֡�ı���Э�鲿�ַ�װ����������
���룺����ջ

=========================================*/
void ADSB_HTTP::cpProData(Stack<uchar>* data)
{
	copyFrom(data, 76, &_ProData);
}
/*========================================

����ؽ�HTTP֡�ı���Э�鲿�ַ�װ���������У����ϲ����
���룺����ջ������λ��

=========================================*/
void ADSB_HTTP::cpDataFrame(Stack<uchar>* data, int r)
{
	_DataFrame.insert(data->pop(), r);
}
/*========================================

����ؽ�HTTP֡�ı���Э�鲿�ַ�װ����������
���룺����ջ

=========================================*/
void ADSB_HTTP::cpDataFramePbit(Stack<uchar>* data)
{
	_DataFrame.insert(data->pop(), _httpSize++);
}
/*========================================

HTTP�ı�������

=========================================*/
void ADSB_HTTP::traverse(void)
{
	trav(&_DataFrame);
}
#endif