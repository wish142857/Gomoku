#include "timer.h"

/***************
* [����] ���캯��
***************/
Timer::Timer(double sec) {
	limitClock = clock_t(sec * double(CLOCKS_PER_SEC));
	return;
}

/***************
* [����] ��������
***************/
Timer::~Timer() {
	return;
}

/***************
* [����] ����ʱ������
***************/
void Timer::reset(double sec) {
	limitClock = clock_t(sec * double(CLOCKS_PER_SEC));
	return;
}

/***************
* [����] �趨��ʱ��ʼ��
***************/
void Timer::start() {
	startClock = clock();
	return;
}

/***************
* [����] �жϼ�ʱ����
* ����ֵ ��ʱ���� false�����򷵻� true
***************/
bool Timer::check() {
	checkClock = clock();
	return ((checkClock - startClock) <= limitClock);
}

/***************
* [����] ���ؾ���ʱ��
* ����ֵ ����ʱ��(s)
***************/
double Timer::time() {
	checkClock = clock();
	return double(double(checkClock - startClock) / double(CLOCKS_PER_SEC));
}
