#include "robot.h"
#include <regex>
using namespace std;



/***************
* [����] ���캯��
***************/
Robot::Robot() {
	// ģʽ��ʼ��
	black_p[0] = pattern(".11111", 5);
	black_p[1] = pattern(".011110", 6);
	black_p[2] = pattern(".11110", 5);
	black_p[3] = pattern(".01111", 5);
	black_p[4] = pattern(".11101", 5);
	black_p[5] = pattern(".10111", 5);
	black_p[6] = pattern(".11011", 5);
	black_p[7] = pattern(".001110", 6);
	black_p[8] = pattern(".011100", 6);
	black_p[9] = pattern(".011010", 6);
	black_p[10] = pattern(".010110", 6);
	black_p[11] = pattern(".001100", 6);
	
	/*black_p[11] = pattern(".11100", 5);
	black_p[12] = pattern(".00111", 5);
	black_p[13] = pattern(".01101", 5);
	black_p[14] = pattern(".01011", 5);
	black_p[15] = pattern(".11010", 5);
	black_p[16] = pattern(".10110", 5);
	black_p[17] = pattern(".01110", 5);
	black_p[18] = pattern(".001100", 6);*/
	
	white_p[0] = pattern(".22222", 5);
	white_p[1] = pattern(".022220", 6);
	white_p[2] = pattern(".22220", 5);
	white_p[3] = pattern(".02222", 5);
	white_p[4] = pattern(".22202", 5);
	white_p[5] = pattern(".20222", 5);
	white_p[6] = pattern(".22022", 5);
	white_p[7] = pattern(".002220", 6);
	white_p[8] = pattern(".022200", 6);
	white_p[9] = pattern(".022020", 6);
	white_p[10] = pattern(".020220", 6);
	white_p[11] = pattern(".002200", 6);
	
	/*white_p[11] = pattern(".22200", 5);
	white_p[12] = pattern(".00222", 5);
	white_p[13] = pattern(".02202", 5);
	white_p[14] = pattern(".02022", 5);
	white_p[15] = pattern(".22020", 5);
	white_p[16] = pattern(".20220", 5);
	white_p[17] = pattern(".02220", 5);
	white_p[18] = pattern(".002200", 6);*/
	

	// ģʽ��ֵ
	cost_self[0] = 300000; // ��
	cost_self[1] = 10000; // ����
	cost_self[2] = 1000; // ����
	cost_self[3] = 1000; // ����
	cost_self[4] = 1000; // ����
	cost_self[5] = 1000; // ����
	cost_self[6] = 1000; // ����
	cost_self[7] = 1000; // ����
	cost_self[8] = 1000; // ����
	cost_self[9] = 1000; // ����
	cost_self[10] = 1000; // ����
	cost_self[11] = 100; // ���

	cost_opp[0] = 300000;
	cost_opp[1] = 10000;
	cost_opp[2] = 1000;
	cost_opp[3] = 1000;
	cost_opp[4] = 1000;
	cost_opp[5] = 1000;
	cost_opp[6] = 1000;
	cost_opp[7] = 1000;
	cost_opp[8] = 1000;
	cost_opp[9] = 1000;
	cost_opp[10] = 1000;
	cost_opp[11] = 100;

	// ���ö�ʱ��
	timer.reset(THRESHOLD_TIME);
	return;
}

/***************
* [����] ��������
***************/
Robot::~Robot() {
	return;
}

/***************
* [����] ��ȡ�������� --- �ӿ�
* ����  Chessboard& chessboard: ����, Chess chess: ��ɫ
***************/
Move Robot::getRobotDecision(Chessboard& chessboard) {
	// ��ʾ��Ϣ
	chess = chessboard.getCurrentChess();	// ��¼��ִ��ɫ
	if (chess == Chess::BLACK)
		printf_s("[��] AI���ھ��ߣ����Ե�...\n");
	else
		printf_s("[��] AI���ھ��ߣ����Ե�...\n");
	// Ԥ���·�
	if (chessboard.getCurrentStep() == 1)
		return Move((GRID_NUM + 1) >> 1, (GRID_NUM + 1) >> 1);	// ��Ԫ
	if (chessboard.getCurrentStep() == 3) {
		Move second_move = chessboard.getMove(2);
		if (second_move.x == 8 && abs(second_move.y - 8) == 1 || second_move.y == 8 && abs(second_move.x - 8) == 1) {
			if (second_move.x == 8)
				return Move(7, second_move.y);
			if (second_move.y == 8)
				return Move(second_move.x, 7);
		}
		if (second_move.x <= 7 && second_move.y <= 7)
			return Move(7, 9);
		if (second_move.x <= 7 && second_move.y >= 9)
			return Move(7, 7);
		if (second_move.x >= 9 && second_move.y <= 7)
			return Move(7, 7);
		if (second_move.x >= 9 && second_move.y >= 9)
			return Move(7, 9);
		return Move(7, 7);
	}
	if (chessboard.getCurrentStep() == 2) {
		Move first_move = chessboard.getMove(1);
		int x = first_move.x, y = first_move.y;
		if (x <= 8 && y <= 8)
			return Move(x + 1, y + 1);
		if (x <= 8 && y > 8)
			return Move(x + 1, y - 1);
		if (x > 8 && y <= 8)
			return Move(x - 1, y + 1);
		else
			return Move(x - 1, y - 1);
	}
	// ��������
	return searchMove(chessboard);
}

/***************
* [����] ��ȡ�Ϸ����ӵ�
***************/
vector<Move> Robot::createMoves(Chessboard& chessboard) {
	vector<Move> moves;
	for (int i = 1; i <= GRID_NUM; i++)
		for (int j = GRID_NUM; j >= 1; j--)
			if (chessboard.possibleMoves[i][j] > 0 && chessboard.possibleMoves[i][j] <= chessboard.fnum)
				moves.push_back(Move(i, j));

	vector<int> evals;
	Move best;
	Chess cur = chessboard.getCurrentChess();
	int max = 0;
	for (const auto mv : moves) {
		int eval = evaluatePoint(chessboard, mv, cur); // ��������
		if (eval > max) {
			max = eval;
			best = mv;
		}
		evals.push_back(eval);
	}

	// ������
	if (max >= ALIVE_FOUR_OPPO) {
		vector<Move> res;
		res.push_back(best);
		return res;
	}

	// ����
	for (int i = 0; i < int(moves.size()); i++) {
		for (int j = 0; j < int(moves.size()) - i - 1; j++) {
			if (evals[j] < evals[j + 1]) {
				Move tmp = moves[j + 1];
				moves[j + 1] = moves[j];
				moves[j] = tmp;
				int tmp2 = evals[j + 1];
				evals[j + 1] = evals[j];
				evals[j] = tmp2;
			}
		}
	}

	// ���������غ�֮�ڵ�ɱ�壬�������С������Χ
	if (max >= (THREE_OR_DEAD_FOUR_OURS << 1)) {
		for (int i = (int)moves.size() - 1; i >= 0; i--) {
			if (evals[i] < THREE_OR_DEAD_FOUR_OURS)
				moves.pop_back();
		}
	}
	return moves;
}

int Robot::evaluatePoint(Chessboard& chessboard, Move mv, Chess cur, int min)
{
	if (chessboard.getCurrentStep() < 6) {
		return 0;
	}
	char same, oppo;
	if (cur == Chess::BLACK) {
		same = '1';
		oppo = '2';
	}
	else {
		same = '2';
		oppo = '1';
	}
	int x = mv.x, y = mv.y;

	// �������������ж����͵�����
	bool left_block[4] = { true, true, true, true }; //  ����ͬɫ���Ƿ�Ϊ��ɫ��߽磨һ�ࣩ
	bool right_block[4] = { true, true, true, true }; //  ����ͬɫ���Ƿ�Ϊ��ɫ��߽磨��һ�ࣩ
	bool left_block_second[4] = { true, true, true, true }; //  ����ͬɫ-�հ�-����ͬɫ���Ƿ�Ϊ��ɫ��߽磨һ�ࣩ
	bool right_block_second[4] = { true, true, true, true }; //  ����ͬɫ-�հ�-����ͬɫ���Ƿ�Ϊ��ɫ��߽磨��һ�ࣩ
	int left_first[4] = { 0, 0, 0, 0 };  //  ��һ������ͬɫ����һ�ࣩ
	int left_second[4] = { 0, 0, 0, 0 };  //  �ڶ�������ͬɫ����һ�ࣩ
	int right_first[4] = { 0, 0, 0, 0 };  //  ��һ������ͬɫ������һ�ࣩ
	int right_second[4] = { 0, 0, 0, 0 };  //  �ڶ�������ͬɫ������һ�ࣩ

	// �Է����ӵ���������
	int oppo_whole[4] = { 0, 0, 0, 0 };
	int oppo_second_left[4] = { 0, 0, 0, 0 };
	int oppo_second_right[4] = { 0, 0, 0, 0 };
	int oppo_both_open[4] = { 2, 2, 2, 2 };

	// ���¾�Ϊ��Щ����ļ���
	// ˮƽ��
	char *c = &chessboard.horizontals[x - 1][y - 1];
	char *d = c + 2;
	char *e = c, *f = d;
	while (*c == same) {
		++left_first[0];
		--c;
	}
	if (*c == '0') {
		left_block[0] = false;
		--c;
		while (*c == same) {
			++left_second[0];
			--c;
		}
		if (*c == '0') {
			left_block_second[0] = false;
		}
	}
	while (*d == same) {
		++right_first[0];
		++d;
	}
	if (*d == '0') {
		right_block[0] = false;
		++d;
		while (*d == same) {
			++right_second[0];
			++d;
		}
		if (*d == '0') {
			right_block_second[0] = false;
		}
	}
	while (*e == oppo) {
		++oppo_whole[0];
		--e;
	}
	if (*e != '0') {
		oppo_both_open[0]--;
	}
	else {
		--e;
		while (*e == oppo) {
			++oppo_second_left[0];
			--e;
		}
	}
	while (*f == oppo) {
		++oppo_whole[0];
		++f;
	}
	if (*f != '0') {
		oppo_both_open[0]--;
	}
	else {
		++f;
		while (*f == oppo) {
			++oppo_second_right[0];
			++f;
		}
	}

	// ��ֱ��
	c = &chessboard.verticals[y - 1][x - 1];
	d = c + 2;
	e = c, f = d;
	while (*c == same) {
		++left_first[1];
		--c;
	}
	if (*c == '0') {
		left_block[1] = false;
		--c;
		while (*c == same) {
			++left_second[1];
			--c;
		}
		if (*c == '0') {
			left_block_second[1] = false;
		}
	}
	while (*d == same) {
		++right_first[1];
		++d;
	}
	if (*d == '0') {
		right_block[1] = false;
		++d;
		while (*d == same) {
			++right_second[1];
			++d;
		}
		if (*d == '0') {
			right_block_second[1] = false;
		}
	}
	while (*e == oppo) {
		++oppo_whole[1];
		--e;
	}
	if (*e != '0') {
		oppo_both_open[1]--;
	}
	else {
		--e;
		while (*e == oppo) {
			++oppo_second_left[1];
			--e;
		}
	}
	while (*f == oppo) {
		++oppo_whole[1];
		++f;
	}
	if (*f != '0') {
		oppo_both_open[1]--;
	}
	else {
		++f;
		while (*f == oppo) {
			++oppo_second_right[1];
			++f;
		}
	}

    // ����-���϶Խ���
	if (x + y <= GRID_NUM + 1) {
		c = &chessboard.up_diagonals[x + y - 2][y - 1];
	}
	else {
		c = &chessboard.up_diagonals[x + y - 2][GRID_NUM - x];
	}
	d = c + 2;
	e = c, f = d;
	while (*c == same) {
		++left_first[2];
		--c;
	}
	if (*c == '0') {
		left_block[2] = false;
		--c;
		while (*c == same) {
			++left_second[2];
			--c;
		}
		if (*c == '0') {
			left_block_second[2] = false;
		}
	}
	while (*d == same) {
		++right_first[2];
		++d;
	}
	if (*d == '0') {
		right_block[2] = false;
		++d;
		while (*d == same) {
			++right_second[2];
			++d;
		}
		if (*d == '0') {
			right_block_second[2] = false;
		}
	}
	while (*e == oppo) {
		++oppo_whole[2];
		--e;
	}
	if (*e != '0') {
		oppo_both_open[2]--;
	}
	else {
		--e;
		while (*e == oppo) {
			++oppo_second_left[2];
			--e;
		}
	}
	while (*f == oppo) {
		++oppo_whole[2];
		++f;
	}
	if (*f != '0') {
		oppo_both_open[2]--;
	}
	else {
		++f;
		while (*f == oppo) {
			++oppo_second_right[2];
			++f;
		}
	}

	// ����-���¶Խ���
	if (y - x <= 0) {
		c = &chessboard.down_diagonals[y - x + GRID_NUM - 1][y - 1];
	}
	else {
		c = &chessboard.down_diagonals[y - x + GRID_NUM - 1][x - 1];
	}
	d = c + 2;
	e = c, f = d;
	while (*c == same) {
		++left_first[3];
		--c;
	}
	if (*c == '0') {
		left_block[3] = false;
		--c;
		while (*c == same) {
			++left_second[3];
			--c;
		}
		if (*c == '0') {
			left_block_second[3] = false;
		}
	}
	while (*d == same) {
		++right_first[3];
		++d;
	}
	if (*d == '0') {
		right_block[3] = false;
		++d;
		while (*d == same) {
			++right_second[3];
			++d;
		}
		if (*d == '0') {
			right_block_second[3] = false;
		}
	}
	while (*e == oppo) {
		++oppo_whole[3];
		--e;
	}
	if (*e != '0') {
		oppo_both_open[3]--;
	}
	else {
		--e;
		while (*e == oppo) {
			++oppo_second_left[3];
			--e;
		}
	}
	while (*f == oppo) {
		++oppo_whole[3];
		++f;
	}
	if (*f != '0') {
		oppo_both_open[3]--;
	}
	else {
		++f;
		while (*f == oppo) {
			++oppo_second_right[3];
			++f;
		}
	}

	bool priority[4] = { false, false, false, false };  // ��������ѡ�������
	int d4_opp = 0;  // �Է�����
	int d4 = 0;  // ��������
	int a3 = 0;  // ��������
	int weak = 0;  // �������
	for (int i = 0; i < 4; i++) {
		int seq_same = left_first[i] + right_first[i];
		int seq_gap1_same_left = seq_same + left_second[i];
		int seq_gap1_same_right = seq_same + right_second[i];
		int seq_gap2_same = seq_gap1_same_left + right_second[i];

		bool either_block = left_block[i] || right_block[i];
		bool both_block = left_block[i] && right_block[i];

		if (seq_same == 4) return FIVE_OURS; // ����5
		else if (oppo_whole[i] == 4) priority[1] = true; // �Է�5
		else if (seq_same == 3 && !either_block) priority[2] = true; // ������4
		else if (seq_gap1_same_left > 2 && seq_gap1_same_right > 2 && !either_block) priority[2] = true;  // ����ͬһֱ��˫��4
		else if (oppo_whole[i] == 3 && oppo_both_open[i] == 2) priority[3] = true; // �Է���4
		else if (oppo_whole[i] == 3 && oppo_both_open[i] == 1) ++d4_opp;  // �Է���4
		else if (oppo_whole[i] + oppo_second_left[i] == 3 || oppo_whole[i] + oppo_second_right[i] == 3) ++d4_opp;  // �Է���4
		else if (seq_same == 3 && !both_block && either_block) ++d4; // ������4
		else if (seq_same == 2 && (seq_gap1_same_left > 2 || seq_gap1_same_right > 2))  ++d4; // ������4
		else if (seq_same == 2 && !either_block) ++a3; // ������3
		else if (seq_same < 2 && (seq_gap1_same_left > 1 && !left_block_second[i] && !right_block[i]
			|| seq_gap1_same_right > 1 && !right_block_second[i] && !left_block[i])) ++a3; //������3

		else if (seq_gap2_same > 0 && !left_block_second[i] && !right_block_second[i]) ++weak; // ������2
		
	}
	if (priority[1]) return FIVE_OPPO;
	if (priority[2]) return ALIVE_FOUR_OURS;
	if (d4 > 1) return DOUBLE_FOUR_OURS;
	if (d4 && a3) return DOUBLE_FOUR_OURS;
	if (priority[3]) return ALIVE_FOUR_OPPO;
	if (d4_opp > 1) return DOUBLE_FOUR_OPPO;
	if (a3 || d4) return (a3 + d4) * THREE_OR_DEAD_FOUR_OURS;
	if (d4_opp) return DEAD_FOUR_OPPO;
	return weak * WEAK_OURS;
}

/***************
* [����] ��ֵ�㷨
* ��������� chess, ȷ����ǰ��ɫ
***************/
int Robot::evaluate(Chessboard& chessboard) {
	int black_value = 0;
	int white_value = 0;
	int cost_black[20];
	int cost_white[20];
	// ���ݴ�ʱ�����巽��ֵ��������
	if (chess == Chess::BLACK) {
		memcpy(cost_black, cost_self, 20 * sizeof(int));
		memcpy(cost_white, cost_opp, 20 * sizeof(int));
	}
	else {
		memcpy(cost_black, cost_opp, 20 * sizeof(int));
		memcpy(cost_white, cost_self, 20 * sizeof(int));
	}
	
	// ����KMP�㷨��ģʽƥ��
	for (int i = 0; i < GRID_NUM; i++) {
		for (int j = bar[chessboard.horizontal_piece_count[0][i + 1]]; j < pat_num; j++)
		{
			black_value += KMP_matcher(black_p[j].P, black_p[j].pi, chessboard.horizontals[i], black_p[j].m, GRID_NUM) * cost_black[j];
		}
		for (int j = bar[chessboard.horizontal_piece_count[1][i + 1]]; j < pat_num; j++)
		{
			white_value += KMP_matcher(white_p[j].P, white_p[j].pi, chessboard.horizontals[i], white_p[j].m, GRID_NUM) * cost_white[j];
		}
	}
	for (int i = 0; i < GRID_NUM; i++) {
		for (int j = bar[chessboard.vertical_piece_count[0][i + 1]]; j < pat_num; j++)
		{
			black_value += KMP_matcher(black_p[j].P, black_p[j].pi, chessboard.verticals[i], black_p[j].m, GRID_NUM) * cost_black[j];
		}
		for (int j = bar[chessboard.vertical_piece_count[1][i + 1]]; j < pat_num; j++)
		{
			white_value += KMP_matcher(white_p[j].P, white_p[j].pi, chessboard.verticals[i], white_p[j].m, GRID_NUM) * cost_white[j];
		}	
	}
	for (int i = 0; i < EFFECTIVE_DIAGONAL_NUM; i++) {
		int len = 15 - abs(14 - i);
		for (int j = bar[chessboard.updiagonal_piece_count[0][i + 1]]; j < pat_num; j++)
		{
			black_value += KMP_matcher(black_p[j].P, black_p[j].pi, chessboard.up_diagonals[i], black_p[j].m, len) * cost_black[j];
		}
		for (int j = bar[chessboard.updiagonal_piece_count[1][i + 1]]; j < pat_num; j++)
		{
			white_value += KMP_matcher(white_p[j].P, white_p[j].pi, chessboard.up_diagonals[i], white_p[j].m, len) * cost_white[j];
		}
	}
	for (int i = 0; i < EFFECTIVE_DIAGONAL_NUM; i++) {
		int len = 15 - abs(14 - i);
		for (int j = bar[chessboard.downdiagonal_piece_count[0][i + 1]]; j < pat_num; j++)
		{
			black_value += KMP_matcher(black_p[j].P, black_p[j].pi, chessboard.down_diagonals[i], black_p[j].m, len) * cost_black[j];
		}
		for (int j = bar[chessboard.downdiagonal_piece_count[1][i + 1]]; j < pat_num; j++)
		{
			white_value += KMP_matcher(white_p[j].P, white_p[j].pi, chessboard.down_diagonals[i], white_p[j].m, len) * cost_white[j];
		}
	}
	if (chess == Chess::BLACK) {
		return (black_value - white_value);
	}
	else {
		return (white_value - black_value);
	}
}
//����������������֧�Ӵ�ͨ���޷�ֱ��������ʤ���վ֣���������һ�����ʱ��Ҫ���ݾ��淵�����������
//�ο����ϣ�
//������, �⿡, ���. �������������ϵͳ�������������[J]. �����Ӧ��, 2012, 32(07):1969-1972.
//�Ჩ��. �������˹�����Ȩ�ع�ֵ�㷨[J]. ���Ա�̼�����ά��, 2008(6):69-75.
//https://www.cnblogs.com/maxuewei2/p/4825520.html

/***************
* [����] �����㷨
* ʹ�æ�-�¼�֦
***************/
Move Robot::searchMove(Chessboard& chessboard)  {
	bool flag = true;
	int depth = 0, a = MIN_VALUE, b = MAX_VALUE;
	int max_value = MIN_VALUE, tmp_value = 0;
	Move move(0, 0), result(0, 0);
	vector<Move> moves = createMoves(chessboard);
	if (moves.size() == 1) {
		return moves.back();
	}
	// *** �����̶� **
	if (!ITERATIVE_DEEPENING_MODE) {
		depth = MAX_DEPTH;
		for (auto m : moves) {
			// �ݹ�����
			if (chessboard.makeMove(m.x, m.y) != Status::S_OK) {
				printf_s("[��] AI��������(makeMove)����������ֹ��\n");
				exit(1);
			}
			tmp_value = minValue(chessboard, depth - 1, a, b);
			if (chessboard.unMakeMove() != Status::S_OK) {
				printf_s("[��] AI��������(unMakeMove)����������ֹ��\n");
				exit(1);
			}
			// ��������
			if (tmp_value > max_value) {
				max_value = tmp_value;
				move = m;
			}
			if (max_value > a)
				a = max_value;
		}
		result = move;
		return result;
	}
	// *** �������� ***
	timer.start();	// �趨��ʱ����ʼ��
	while (flag) {
		depth += 2;			// ��������
		a = MIN_VALUE;
		b = MAX_VALUE;
		max_value = MIN_VALUE;
		flag = true;		// �������
		for (auto m : moves) {
			// ����ʱ����
			if ((depth > MAX_DEPTH) || (depth > MIN_DEPTH && !timer.check()))  {
				flag = false;
				break;
			}
			// �ݹ�����
			if (chessboard.makeMove(m.x, m.y) != Status::S_OK) {
				printf_s("[��] AI��������(makeMove)����������ֹ��\n");
				exit(1);
			}
			tmp_value = minValue(chessboard, depth - 1, a, b);
			if (chessboard.unMakeMove() != Status::S_OK) {
				printf_s("[��] AI��������(unMakeMove)����������ֹ��\n");
				exit(1);
			}
			// ��������
			if (tmp_value > max_value) {
				max_value = tmp_value;
				move = m;
			}
			if (max_value > a)
				a = max_value;
		}
		if (flag)
			result = move;
	}
	return result;
}

/***************
* [����] minimax�Կ����� - MAX
* ʹ�æ�-�¼�֦
* ѭ������ʽ: ����ʱ����ʱ�ľ����ڴ��ҷ�����
* ѭ������ʽ: ����ǰ��chessboard״̬����
***************/
int Robot::maxValue(Chessboard& chessboard, int depth, int a, int b) {
	int val;
	Cache& cache = chessboard.cache;
	if ((val = cache.getCache(depth, a, b)) != VAL_UNKNOWN) {
		return val;
	}
	if (depth == 0) {
		val = evaluate(chessboard);
		cache.setCache(val, hashfEXACT, depth);
		return val;
	}
	int max_value = MIN_VALUE, tmp_value = 0;
	int hashf = hashfALPHA;
	vector<Move> moves = createMoves(chessboard);
	for (auto m : moves) {
		if (chessboard.makeMove(m.x, m.y) != Status::S_OK) {
			printf_s("[��] AI��������(makeMove)����������ֹ��\n");
			exit(1);
		}
		tmp_value = minValue(chessboard, depth - 1, a, b);
		if (chessboard.unMakeMove() != Status::S_OK) {
			printf_s("[��] AI��������(unMakeMove)����������ֹ��\n");
			exit(1);
		}
		if (tmp_value > max_value)
			max_value = tmp_value;
		if (max_value >= b) {
			cache.setCache(max_value, hashfBETA, depth);
			return max_value;
		}
		if (max_value > a) {
			hashf = hashfEXACT;
			a = max_value;
		}
	}
	cache.setCache(max_value, hashf, depth);
	return max_value;
}

/***************
* [����] minimax�Կ����� - MIN
* ʹ�æ�-�¼�֦
* ѭ������ʽ: ����ʱ����ʱ�ľ����ڴ��Է�����
* ѭ������ʽ: ����ǰ��chessboard״̬����
***************/
int Robot::minValue(Chessboard& chessboard, int depth, int a, int b) {
	int val;
	Cache& cache = chessboard.cache;
	if ((val = cache.getCache(depth, a, b)) != VAL_UNKNOWN) {
		return val;
	}
	if (depth == 0) {
		val = evaluate(chessboard);
		cache.setCache(val, hashfEXACT, depth);
		return val;
	}
	int min_value = MAX_VALUE, tmp_value = 0;
	int hashf = hashfBETA;
	vector<Move> moves = createMoves(chessboard);
	for (auto m : moves) {
		if (chessboard.makeMove(m.x, m.y) != Status::S_OK) {
			printf_s("[��] AI��������(makeMove)����������ֹ��\n");
			exit(1);
		}
		tmp_value = maxValue(chessboard, depth - 1, a, b);
		if (chessboard.unMakeMove() != Status::S_OK) {
			printf_s("[��] AI��������(unMakeMove)����������ֹ��\n");
			exit(1);
		}
		if (tmp_value < min_value)
			min_value = tmp_value;
		if (min_value <= a) {
			cache.setCache(min_value, hashfALPHA, depth);
			return min_value;
		}
		if (min_value < b) {
			hashf = hashfEXACT;
			b = min_value;
		}
	}
	cache.setCache(min_value, hashf, depth);
	return min_value;
}
