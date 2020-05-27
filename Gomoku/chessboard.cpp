#include "chessboard.h"
#include <string.h> 

using namespace std;

/***************
* [����] ���캯��
***************/
Chessboard::Chessboard() {
	this->initMove();
	return;
}

/***************
* [����] ��������
***************/
Chessboard::~Chessboard() {
	return;
}

/***************
* [����] ��ֳ�ʼ��
***************/
void Chessboard::initMove() {
	this->clearChessboard();	// �������
	chessRecord.clear();		// ��ռ�¼
	// ���ø�������
	for (int i = 0; i < GRID_NUM; i++) {
		for (int j = 1; j <= GRID_NUM; j++) {
			horizontals[i][j] = '0';
			verticals[i][j] = '0';
		}
		horizontals[i][0] = horizontals[i][GRID_NUM + 1] = '#';
		verticals[i][0] = verticals[i][GRID_NUM + 1] = '#';
		horizontals[i][GRID_NUM + 2] = verticals[i][GRID_NUM + 2] = '\0';
	}
	for (int i = 0; i < EFFECTIVE_DIAGONAL_NUM; i++) {
		int len = 15 - abs(14 - i);
		for (int j = 1; j <= len; j++) {
			up_diagonals[i][j] = '0';
			down_diagonals[i][j] = '0';
		}
		up_diagonals[i][0] = up_diagonals[i][len + 1] = '#';
		down_diagonals[i][0] = down_diagonals[i][len + 1] = '#';
		up_diagonals[i][len + 2] = down_diagonals[i][len + 2] = '\0';
	}
	return;
}

/***************
* [����] ���ִ���߷�
* ����  S_OK |  F_OUTSIDE | F_NOBLANK
***************/
Status Chessboard::makeMove(const int x, const int y) {
	// �ж��߷��Ƿ�Ϸ�
	if (!inChessboard(x, y))
		return Status::F_OUTSIDE;
	if (chessboard[x][y] != Chess::BLANK)
		return Status::F_NOBLANK;
	// ִ���߷�
	Chess cur = getCurrentChess();
	chessboard[x][y] = cur;					// ��������
	chessRecord.push_back(Move(x, y));		// ���¼�¼
	// ���¸�������
	char c = chess2char(cur);
	horizontals[x - 1][y] = c;
	verticals[y - 1][x] = c;
	if (x + y <= GRID_NUM + 1)
		up_diagonals[x + y - 2][y] = c;
	else
		up_diagonals[x + y - 2][GRID_NUM + 1 - x] = c;
	if (y - x <= 0)
		down_diagonals[y - x + GRID_NUM - 1][y] = c;
	else
		down_diagonals[y - x + GRID_NUM - 1][x] = c;

	int color = 1;
	if (cur == Chess::BLACK) {
		color = 0;
	}
	horizontal_piece_count[color][x]++;
	vertical_piece_count[color][y]++;
	updiagonal_piece_count[color][x + y - 1]++;
	downdiagonal_piece_count[color][y - x + GRID_NUM]++;

	for (int i = 0; i < fnum; i++)
			if (inChessboard(x + fx[i], y + fy[i]))
				possibleMoves[x + fx[i]][y + fy[i]]++;
	possibleMoves[x][y] += fnum + 1;

	cache.update(x, y, color);

	return Status::S_OK;
}

/***************
* [����] ��ֳ����߷�
* ����  S_OK | F_NOLAST
***************/
Status Chessboard::unMakeMove() {
	// �ж��ܷ���
	if (chessRecord.empty())
		return Status::F_NOLAST;
	// �����߷�
	Move move = chessRecord.back();
	int x = move.x, y = move.y;
	chessboard[x][y] = Chess::BLANK;	// ��������
	chessRecord.pop_back();				// ���¼�¼
	// ���¸�������
	horizontals[x - 1][y] = '0';
	verticals[y - 1][x] = '0';
	if (x + y <= GRID_NUM + 1)
		up_diagonals[x + y - 2][y] = '0';
	else
		up_diagonals[x + y - 2][GRID_NUM + 1 - x] = '0';
	if (y - x <= 0)
		down_diagonals[y - x + GRID_NUM - 1][y] = '0';
	else
		down_diagonals[y - x + GRID_NUM - 1][x] = '0';

	int color = 1;
	if (getCurrentChess() == Chess::BLACK) {
		color = 0;
	}
	horizontal_piece_count[color][x]--;
	vertical_piece_count[color][y]--;
	updiagonal_piece_count[color][x + y - 1]--;
	downdiagonal_piece_count[color][y - x + GRID_NUM]--;

	possibleMoves[x][y] -= fnum + 1;
	for (int i = 0; i < fnum; i++)
			if (inChessboard(x + fx[i], y + fy[i]))
				possibleMoves[x + fx[i]][y + fy[i]]--;
	cache.update(x, y, color);

	return Status::S_OK;
}

/***************
* [����] ���ʤ���ж�
* ����  G_BLACK | G_WHITE | G_DRAW | G_CONTINUE
***************/
Status Chessboard::gameOver() {
	if (chessRecord.empty())
		return Status::G_CONTINUE;
	Move last = chessRecord.back();
	int x = last.x, y = last.y;

	// ͨ���������һ����ĸ��������������ڵ�ͬɫ���������ж��Ƿ���Ϸ����
	int adjacent_same_color[4] = { 0, 0, 0, 0 };
	char same;
	Status res;
	if (getCurrentChess() == Chess::BLACK) {
		same = '2';
		res = Status::G_WHITE;
	}
	else {
		same = '1';
		res = Status::G_BLACK;
	}

	char *c = &horizontals[x - 1][y - 1];
	char *d = c + 2;
	while (*c == same) {
		++adjacent_same_color[0];
		--c;
	}
	while (*d == same) {
		++adjacent_same_color[0];
		++d;
	}

	c = &verticals[y - 1][x - 1];
	d = c + 2;
	while (*c == same) {
		++adjacent_same_color[1];
		--c;
	}
	while (*d == same) {
		++adjacent_same_color[1];
		++d;
	}

	if (x + y <= GRID_NUM + 1) {
		c = &up_diagonals[x + y - 2][y - 1];
	}
	else {
		c = &up_diagonals[x + y - 2][GRID_NUM - x];
	}
	d = c + 2;
	while (*c == same) {
		++adjacent_same_color[2];
		--c;
	}
	while (*d == same) {
		++adjacent_same_color[2];
		++d;
	}

	if (y - x <= 0) {
		c = &down_diagonals[y - x + GRID_NUM - 1][y - 1];
	}
	else {
		c = &down_diagonals[y - x + GRID_NUM - 1][x - 1];
	}
	d = c + 2;
	while (*c == same) {
		++adjacent_same_color[3];
		--c;
	}
	while (*d == same) {
		++adjacent_same_color[3];
		++d;
	}

	for (int i = 0; i < 4; i++) {
		if (adjacent_same_color[i] > 3)
			return res;
	}

	if (getCurrentStep() > GRID_NUM * GRID_NUM) {
		return Status::G_DRAW; // �;�
	}
	return Status::G_CONTINUE;
}

/***************
* [����] ��ӡ����
***************/
void Chessboard::printChessboard() {
	// ��ӡ������
	printf_s("   ");
	for (int i = 1; i <= 15; i++)
		printf_s("%2X", i);
	printf_s("\n");
	for (int i = 1; i <= 15; i++) {
		printf_s(" %2X", i);	// ��ӡ������
		for (int j = 1; j <= 15; j++) {
			if (chessboard[i][j] == Chess::BLACK)
				printf_s("��");		// ��ӡ����
			else if (chessboard[i][j] == Chess::WHITE)
				printf_s("��");		// ��ӡ����
			else {
				// �߽��ӡ
				if (i == 1 && j == 1)
					printf_s("�X ");
				else if (i == 1 && j == 15)
					printf_s("�[");
				else if (i == 15 && j == 1)
					printf_s("�^ ");
				else if (i == 15 && j == 15)
					printf_s("�a");
				else if (i == 1 && j != 15 && j != 1)
					printf_s("�� ");
				else if (i == 15 && j != 15 && j != 1)
					printf_s("�� ");
				else if (j == 1 && i != 15 && i != 1)
					printf_s("�� ");
				else if (j == 15 && i != 15 && i != 1)
					printf_s("��");
				else
					printf_s("�� ");
			}
		}
		printf_s("\n");
	}
	printf_s("\n");
	return;
}

/***************
* [����] ��ӡ���̼�¼
* ����  ��������������������0�����У�
* ����  true - ��ӡ�ɹ�, false - ��ӡʧ��
***************/
bool Chessboard::printChessRecord(int step) {
	char label[20];
	// ������м�¼
	if (step == 0) {
		for (int i = 1; i <= chessRecord.size(); i++) {
			sprintf_s(label, "����%d����", i);
			if (i & 1)
				printf("%-12s �� �� (%X, %X)\n", label, chessRecord[i - 1].x, chessRecord[i - 1].y);
			else
				printf("%-12s �� �� (%X, %X)\n", label, chessRecord[i - 1].x, chessRecord[i - 1].y);
		}
		return true;
	}
	// ����ض�������¼
	if (step < 0)
		step += int(chessRecord.size() + 1);
	if (step < 1 || step > int(chessRecord.size()))
		return false;
	sprintf_s(label, "����%d����", step);
	if (step & 1)
		printf("%-12s �� �� (%X, %X)\n", label, chessRecord[step - 1].x, chessRecord[step - 1].y);
	else
		printf("%-12s �� �� (%X, %X)\n", label, chessRecord[step - 1].x, chessRecord[step - 1].y);
	return true;
}
