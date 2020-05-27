#include "player.h"
#include <iostream>
using namespace std;

/***************
* [����] ���캯��
***************/
Player::Player() {
	operation = Operation::NEWBLACK;
	memset(buffer, 0, sizeof(buffer));
	return;
}

/***************
* [����] ��������
***************/
Player::~Player() {
	return;
}

/***************
* [����] ��ȡ�û����� --- �ӿ�
***************/
void Player::getUserInput(Chess chess) {
	if (chess == Chess::BLACK)
		strcpy_s(icon, sizeof(icon), "[��]");
	else
		strcpy_s(icon, sizeof(icon), "[��]");
	printf_s("%s ���Ļغϣ����������:", icon);
	while (true) {
		// ��ȡ�û�����
		move.x = move.y = 0;
		cin.getline(buffer, BUFFER_LENGTH);
		cin.clear();
		sscanf_s(buffer, "%s %X %X", order, sizeof(order), &move.x, &move.y);
		// ƥ�����
		if (strcmp(order, "newblack") == 0) {
			operation = Operation::NEWBLACK;
			return;
		}
		if (strcmp(order, "newwhite") == 0) {
			operation = Operation::NEWWHITE;
			return;
		}
		if (strcmp(order, "move") == 0) {
			operation = Operation::MOVE;
			return;
		}
		if (strcmp(order, "withdraw") == 0) {
			operation = Operation::WITHDRAW;
			return;
		}
		if (strcmp(order, "tips") == 0) {
			operation = Operation::TIPS;
			return;
		}
		if (strcmp(order, "exit") == 0) {
			operation = Operation::EXIT;
			return;
		}
		if (strcmp(order, "record") == 0) {
			operation = Operation::RECORD;
			return;
		}
		// ��������
		printf_s("%s �Ƿ���ʽ�����������:", icon);
	}
	return;
}
