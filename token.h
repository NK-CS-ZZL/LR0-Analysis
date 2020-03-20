#pragma once
#pragma warning(disable:4091)
#pragma warning(disable:6385)
#pragma warning(disable:26812)
#include <iostream>
#define MAX_TOKEN_NUM 64
#define MAX_TOKEN_LEN 32

#define UNKNOWN_TOKEN_ERR -1
#define UNKNOWN_OP_ERR -2
#define SYNTAX_ERR -3
#define UNKNOWN_ERR -4


#define RULES_NUMBER 17
#define ac 128

enum type {
	TK_NOTYPE = 256,
	TK_INT,
	TK_NGT, TK_NLT, TK_GT, TK_LT, TK_NEQ, TK_EQU,
	TK_LNOT, TK_LAND, TK_LOR,
	TK_PLUS, TK_MINUS, TK_MUL, TK_DIV,
	TK_LP, TK_RP,
	TK_UMINUS, TK_EOF,
	TK_A, TK_B, TK_C, TK_D, TK_E, TK_F
};

typedef struct Token {
	int token_type;
	char str[MAX_TOKEN_LEN];
	Token() :token_type(TK_NOTYPE) {
		memset(str, 0, MAX_TOKEN_LEN);
	}
};

void make_token(const char* str);