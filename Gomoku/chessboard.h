#pragma once
#ifndef _CHESSBOARD_H_
#define _CHESSBOARD_H_
#include <iostream>
#include <string.h>
#include <vector>
#include <regex>
#include "zobrist.h"

/***************
* [����] ����
***************/
const int GRID_NUM = 15;	// ���̹�ģ
const int EFFECTIVE_DIAGONAL_NUM = 29; // �Խ�������

/***************
* [ö��] ��/״̬
***************/
enum class Chess {
	BLANK = 0,		// �հ�
	BLACK = 1,		// ����
	WHITE = 2, 		// ����
	POSSIBLE = 3	// ��ѡ��������
};
enum class Status {
	S_OK = 0,		// ִ�гɹ�
	F_OUTSIDE = 1,	// ִ��ʧ�� - ����
	F_NOBLANK = 2,	// ִ��ʧ�� - �ǿ�
	F_NOLAST = 3,	// ִ��ʧ�� - ����һ��
	G_BLACK = 4,	// ��Ϸ���� - ���ӻ�ʤ
	G_WHITE = 5,	// ��Ϸ���� - ���ӻ�ʤ
	G_DRAW = 6,		// ��Ϸ���� - ����
	G_CONTINUE = 7,	// ��Ϸ���� - �޷��ж�ʤ��
};

/***************
* [�ṹ] �߷�
***************/
struct Move {
	int x;			// X ����
	int y;			// Y ����
	Move(int x = 0, int y = 0) : x(x), y(y) { }
};

/***************
* [��] ����
***************/
class Player;
class Robot;

/***************
* [��] ���
* ���������������غ���
***************/
class Chessboard {
public:
	Chessboard();										// ���캯��
	~Chessboard();										// ��������
	void initMove();									// ��ֳ�ʼ��
	Status makeMove(const int x, const int y);			// ���ִ���߷�
	Status unMakeMove();								// ��ֳ����߷�
	Status gameOver();									// �ж����ʤ�����
	void printChessboard();								// ��ӡ����
	bool printChessRecord(int step);					// ��ӡ��ּ�¼
protected:
	// �ж�λ���Ƿ�Ϊ��
	inline bool isBlank(const int x, const int y) {
		return chessboard[x][y] == Chess::BLANK;
	}
	// ������ּ�¼��ȡ��ǰ��һ����
	inline int getCurrentStep() {
		return int(chessRecord.size() + 1);
	}
	// ������ּ�¼��ȡ��ǰ��һ��ɫ
	inline Chess getCurrentChess() {
		return ((chessRecord.size() & 1) ? Chess::WHITE : Chess::BLACK);
	}
	// ��ȡ��num������λ��
	inline Move getMove(int num) {
		return chessRecord[num - 1];
	}
private:
	Chess chessboard[GRID_NUM + 1][GRID_NUM + 1];		// ���̱���
	std::vector<Move> chessRecord;						// ��ּ�¼
	int possibleMoves[GRID_NUM + 1][GRID_NUM + 1];		// �������� ��ѡ����λ��
	char horizontals[GRID_NUM][GRID_NUM + 3];			// �������� ˮƽ������
	char verticals[GRID_NUM][GRID_NUM + 3];				// �������� ��ֱ������
	char up_diagonals[EFFECTIVE_DIAGONAL_NUM][GRID_NUM + 3];	// �������� ����-���Ϸ���Խ�������
	char down_diagonals[EFFECTIVE_DIAGONAL_NUM][GRID_NUM + 3];	// �������� ����-���Ϸ���Խ�������
	
	int horizontal_piece_count[2][GRID_NUM + 1];
	int vertical_piece_count[2][GRID_NUM + 1];
	int updiagonal_piece_count[2][EFFECTIVE_DIAGONAL_NUM + 1];
	int downdiagonal_piece_count[2][EFFECTIVE_DIAGONAL_NUM + 1];

	int fx[16] = {-1, -1, -1, 0, 0, 1, 1, 1, -2, -2, -2, 0, 0, 2, 2, 2};
	int fy[16] = {-1, 0, 1, -1, 1, -1, 0, 1, -2, 0, 2, -2, 2, -2, 0, 2};
	int fnum = 16;

	Cache cache;

	// �����з��Ƿ���������
	inline bool inChessboard(const int x, const int y) {
		return (x >= 1 && x <= GRID_NUM && y >= 1 && y <= GRID_NUM);	
	}
	// �������
	inline void clearChessboard() {
		memset(chessboard, 0, sizeof(chessboard));
		memset(possibleMoves, 0, sizeof(possibleMoves));
		memset(horizontal_piece_count, 0, sizeof(horizontal_piece_count));
		memset(vertical_piece_count, 0, sizeof(vertical_piece_count));
		memset(updiagonal_piece_count, 0, sizeof(updiagonal_piece_count));
		memset(downdiagonal_piece_count, 0, sizeof(downdiagonal_piece_count));
	}
	// ����ת�ַ�
	inline char chess2char(Chess x) {
		return '0' + int(x);
	}
	// ��Ԫ������
	friend class Player;
	friend class Robot;

};
#endif