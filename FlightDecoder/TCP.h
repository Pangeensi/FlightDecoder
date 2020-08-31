#pragma once
#ifndef _ADSB_TCP_H_
#define _ADSB_TCP_H_
#include "Vector.h"
#include "Stack.h"
class ADSB_TCP
{
private:
	Vector<unsigned char>	_ProData = Vector<unsigned char>();						//���õ�Э������ͷ
	Vector<unsigned char>	_DataFrame = Vector<unsigned char>();					//������
	int _tcpSize = 0;																//TCP֡�Ĺ�ģ
protected:
	void copyFrom(Stack<uchar>*, int, Vector<uchar>*);			//������װ����
public:
	ADSB_TCP() {}								//TCP֡�Ĺ��캯��
	~ADSB_TCP() {}
	void cpProData(Stack<uchar>*);								//����Э���װ����
	void cpDataFrame(Stack<uchar>* data, int r);				//���ݷ�װ����
	void cpDataFramePbit(Stack<uchar>* data);					//��λ���ݷ�װ����
};
/*========================================

�����������ݶ�ջ��ָ�����ַ�װ��HTTP��������
���룺����ջ����װԪ��������Ŀ��������

=========================================*/
void ADSB_TCP::copyFrom(Stack<uchar>* data, int num, Vector<uchar>* vtc)
{
	for (int i = 0; i < num; i++)
		vtc->insert(data->pop(), i);
}
/*========================================

��TCP֡�ı���Э�鲿�ַ�װ����������
���룺����ջ

=========================================*/
void ADSB_TCP::cpProData(Stack<uchar>* data)
{
	copyFrom(data, 76, &_ProData);
}
/*========================================

����ؽ�TCP֡�ı���Э�鲿�ַ�װ���������У����ϲ����
���룺����ջ������λ��

=========================================*/
void ADSB_TCP::cpDataFrame(Stack<uchar>* data, int r)
{
	_DataFrame.insert(data->pop(), r);
}
/*========================================

����ؽ�TCP֡�ı���Э�鲿�ַ�װ����������
���룺����ջ

=========================================*/
void ADSB_TCP::cpDataFramePbit(Stack<uchar>* data)
{
	_DataFrame.insert(data->pop(), _tcpSize++);
}
#endif