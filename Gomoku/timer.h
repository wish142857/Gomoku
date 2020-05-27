#pragma once
#ifndef _TIMER_H_
#define _TIMER_H_
#include <ctime>

/***************
* [��] ��ʱ��
* �������е�������
***************/
class Timer {
public:
	Timer(double sec = 0);	// ���캯��
	~Timer();				// ��������
	void reset(double sec);	// ����ʱ������
	void start();			// �趨��ʱ��ʼ��
	bool check();			// �жϼ�ʱ����
	double time();			// ���ؾ���ʱ��
private:
	clock_t limitClock;		// ʱ������
	clock_t startClock;		// ��ʼʱ��
	clock_t checkClock;		// ���ʱ��
};





#endif
