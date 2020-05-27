#pragma once
#ifndef _ROBOT_H_
#define _ROBOT_H_
#include "chessboard.h"
#include "timer.h"

/***************
* [����] ����
***************/
const int MAX_VALUE = 0x3f3f3f3f;			// ����ֵ
const int MIN_VALUE = -0x3f3f3f3f;			// ��Сֵ
const int MIN_DEPTH = 2;					// ��С��������
const int MAX_DEPTH = 6;					// �����������
const double THRESHOLD_TIME = 3.0;			// �����ٽ�ʱ��(��)
const bool ITERATIVE_DEEPENING_MODE = true;	// ��������ģʽ
// ��Ϊ false���򽫸��� MAX_DEPTH �̶���������


/***************
* [����] ����
***************/
void compute_prefix(char* P, int *pi, int m);
int KMP_matcher(char* P, int *pi, char* T, int m, int n);

/***************
* [�ṹ] ����
***************/
struct pattern {
	char P[7];
	int m;
	int pi[8];
	pattern() {}
	pattern(pattern& pattern_) { memcpy(P, pattern_.P, 7); m = pattern_.m; memcpy(pi, pattern_.pi, 8); }
	pattern(const char *P_, int m_) { memcpy(P, P_, 7); m = m_; compute_prefix(P, pi, m); }
};

// PointRate 
const int FIVE_OURS = 10000000;
const int FIVE_OPPO = 1000000;
const int ALIVE_FOUR_OURS = 100000;
const int ALIVE_FOUR_OPPO = 10000;
const int DOUBLE_FOUR_OURS = 100000;
const int DOUBLE_FOUR_OPPO = 10000;
const int THREE_OR_DEAD_FOUR_OURS = 1000;
const int DEAD_FOUR_OPPO = 1000;
const int WEAK_OURS = 100;

/***************
* [��] ����
*+ ������������㷨
***************/
class Robot {
public:
	Robot();					// ���캯��
	~Robot();					// ��������
	Move getRobotDecision(Chessboard& chessboard);	// [�ӿ�] ��ȡ��������
	
private:
	std::vector<Move> createMoves(Chessboard& chessboard);			// ��ȡ�Ϸ����ӵ�
	int evaluate(Chessboard& chessboard);							// ��ֵ�㷨
	int evaluatePoint(Chessboard& chessboard, Move mv, Chess cur, int min = 0);
	Move searchMove(Chessboard& chessboard);						// �����㷨
	int maxValue(Chessboard& chessboard, int depth, int a, int b);	// minimax�Կ����� - MAX
	int minValue(Chessboard& chessboard, int depth, int a, int b);	// minimax�Կ����� - MIN
	Chess chess;					// ��ִ��ɫ
	pattern black_p[20];
	pattern white_p[20];
	int cost_self[20];				// ��������
	int cost_opp[20];				// ��������
	int bar[16] = {12, 12, 11, 7, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	int pat_num = 12;
	// int bar[16] = {19, 19, 18, 7, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	// int pat_num = 19;
	Timer timer;					// ��ʱ��ʵ��
};

#endif
