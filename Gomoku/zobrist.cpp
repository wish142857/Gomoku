#include <random>
#include <iostream>
#include "zobrist.h"

BoardHash hashTable[TABLE_SIZE_MASK + 1];

/*
初始化随机数矩阵
*/
void Zobrist::init()
{
	for (int i = 0; i < 2; i++) {
		for (int j = 1; j <= GRID_NUM_; j++) {
			for (int k = 1; k <= GRID_NUM_; k++) {
				board[i][j][k] = rand_64();
			}
		}
	}
	key = rand_64();
}

/*
异或运算更新key
*/
void Zobrist::update(int x, int y, int role) {
	key ^= board[role][x][y];
}

/*
64位随机数生成器
*/
u64 Zobrist::rand_64() {
	return rand() ^ ((u64)rand() << 15) ^ ((u64)rand() << 30) ^ ((u64)rand() << 45) ^ ((u64)rand() << 60);
}

/*
查询当前局面是否在哈希表中
*/
int Cache::getCache(int depth, int alpha, int beta) {
	BoardHash *hsh = &hashTable[zobrist.getKey() & TABLE_SIZE_MASK];
	if (hsh->key == zobrist.getKey()) {
		if (hsh->depth >= depth) {
			if (hsh->flags == hashfEXACT) {
				return hsh->val;
			}
			if ((hsh->flags == hashfALPHA) && (hsh->val <= alpha)) {
				return alpha;
			}
			if ((hsh->flags == hashfBETA) && (hsh->val >= beta)) {
				return beta;
			}
		}
	}
	return VAL_UNKNOWN;
}

/*
更改哈希表
*/
void Cache::setCache(int val, int flags, int depth) {
	BoardHash *hsh = &hashTable[zobrist.getKey() & TABLE_SIZE_MASK];
	hsh->key = zobrist.getKey();
	hsh->val = val;
	hsh->flags = flags;
	hsh->depth = depth;
}

/*
重置哈希表
*/
void Cache::resetCache() {
	memset(hashTable, 0, sizeof(hashTable));
}