#pragma once
#ifndef _PLAYER_H_
#define _PLAYER_H_
#include "chessboard.h"

/***************
* [����] ����
***************/
const int BUFFER_LENGTH = 128;	// ��������С

/***************
* [ö��] ����
***************/
enum class Operation {
	NEWBLACK = 0,	// ���� AI�ֺ����� 
	NEWWHITE = 1,	// ���� AI�ְ׺���
	MOVE = 2,		// ����
	WITHDRAW = 3,	// ����
	TIPS = 4,		// ��ʾ
	EXIT = 5,		// �˳�
	RECORD = 6,		// �����ּ�¼
};

/***************
* [��] ���
* ��������ҽ������
***************/
class Player {
public:
	Player();						// ���캯��
	~Player();						// ��������
	void getUserInput(Chess chess);	// [�ӿ�] ��ȡ�û�����
	inline Operation getUserOperation() {
		return operation;			// [�ӿ�] ��ȡ����
	}
	inline Move getUserMove() {
		return move;				// [�ӿ�] ��ȡ�߷�
	}
private:
	Operation operation;			// ��������
	Move move;						// �߷�����
	char buffer[128];				// ������
	char icon[16], order[16];		// ��ʱ�ַ���

};


#endif