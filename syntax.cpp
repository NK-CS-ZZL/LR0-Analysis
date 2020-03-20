#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "token.h"
#include "cal.h"

using namespace std;
// Record tokens info
Token tokens[MAX_TOKEN_NUM];
int num;

//It's used to show if some errors occur during calculation.
static bool err_flag = false;

// Locate colomn No. by current token 
static int table_entry(int type) {
	switch (type)
	{
	case TK_INT:
		return 0;
	case TK_LOR:
		return 1;
	case TK_LAND:
		return 2;
	case TK_GT:
	case TK_LT:
	case TK_NLT:
	case TK_NGT:
	case TK_EQU:
	case TK_NEQ:
		return 3;
	case TK_PLUS:
	case TK_MINUS:
		return 4;
	case TK_MUL:
	case TK_DIV:
		return 5;
	case TK_LP:
		return 6;
	case TK_RP:
		return 7;
	case TK_EOF:
		return 8;
	case TK_UMINUS:
	case TK_LNOT:
		return 9;
	default:
		// for TK_A~TK_F
		return 10 + (type - TK_A); 
	}
}

// It records the length of each rule.
static int rule_lens[] = { 1, 3, 1, 3, 1,
                           3, 1, 3, 1, 3, 
	                       1, 3, 1, 2 };

// Postive number means shift action. 
// e.g. state_table(0, 0) means current token is TK_INT(see in "table_entry") and we should go to state 9
// Negative number means reduct action
// e.g. state_table(2, 1) means current token is TK_LOR and we should reduct according to Rule 9.
// Zero means no action, which indicates some errors ocurr.
// Macro "ac" means parse is completed successfully.
static int state_table[23][16] = {
{9,    0,    0,    0,    0,    0,    10,   0,    0,    8,    2,    3,    4,    5,    6,    7},
{0,    11,   0,    0,    0,    0,    0,    0,    ac,   0,    0,    0,    0,    0,    0,    0},
{0,    -2,   12,   0,    0,    0,    0,   -2,   -2,    0,    0,    0,    0,    0,    0,    0},
{0,    -4,   -4,   13,   0,    0,    0,   -4,   -4,    0,    0,    0,    0,    0,    0,    0},
{0,    -6,   -6,   -6,   14,   0,    0,   -6,   -6,    0,    0,    0,    0,    0,    0,    0},	//5
{0,    -8,   -8,   -8,   -8,   15,   0,   -8,   -8,    0,    0,    0,    0,    0,    0,    0},
{0,    -10,  -10,  -10,  -10,  -10,  0,   -10,  -10,   0,    0,    0,    0,    0,    0,    0},
{9,    0,    0,    0,    0,    0,    10,   0,    0,    8,    0,    0,    0,    0,    0,    16},
{0,    -12,  -12,  -12,  -12,  -12,  0,   -12,  -12,   0,    0,    0,    0,    0,    0,    0},
{9,    0,    0,    0,    0,    0,    10,   0,    0,    8,    17,   3,    4,    5,    6,    7},	//10
{9,    0,    0,    0,    0,    0,    10,   0,    0,    8,    0,    18,   4,    5,    6,    7},
{9,    0,    0,    0,    0,    0,    10,   0,    0,    8,    0,    0,    19,   5,    6,    7},
{9,    0,    0,    0,    0,    0,    10,   0,    0,    8,    0,    0,    0,    20,   6,    7},
{9,    0,    0,    0,    0,    0,    10,   0,    0,    8,    0,    0,    0,    0,    21,   7},
{9,    0,    0,    0,    0,    0,    10,   0,    0,    8,    0,    0,    0,    0,    0,    22},	//15
{0,    -13,  -13,  -13,  -13,  -13,  0,    -13,  -13,  0,    0,    0,    0,    0,    0,    0},
{0,    11,   0,    0,    0,    0,    0,    23,   0,    0,    0,    0,    0,    0,    0,    0},
{0,    -1,   12,   0,    0,    0,    0,    -1,   -1,   0,    0,    0,    0,    0,    0,    0},
{0,    -3,   -3,   13,   0,    0,    0,    -3,   -3,   0,    0,    0,    0,    0,    0,    0},
{0,    -5,   -5,   -5,   14,   0,    0,    -5,   -5,   0,    0,    0,    0,    0,    0,    0},	//20
{0,    -7,   -7,   -7,   -7,   15,   0,    -7,   -7,   0,    0,    0,    0,    0,    0,    0},
{0,    -9,   -9,   -9,   -9,   -9,   0,    -9,   -9,   0,    0,    0,    0,    0,    0,    0},
{0,    -11,  -11,  -11,  -11,  -11,  0,    -11,  -11,  0,    0,    0,    0,    0,    0,    0},
};

// Convert integer to c style string
char* my_itoa(int num, char* str, int radix)
{
	char index[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	unsigned unum;
	int i = 0, j, k;

	if (radix == 10 && num < 0)
	{
		unum = (unsigned)-num;
		str[i++] = '-';
	}
	else unum = (unsigned)num;

	do
	{
		str[i++] = index[unum % (unsigned)radix];
		unum /= radix;

	} while (unum);

	str[i] = '\0';

	if (str[0] == '-') k = 1;
	else k = 0;

	char temp;
	for (j = k; j <= (i - 1) / 2; j++)
	{
		temp = str[j];
		str[j] = str[i - 1 + k - j];
		str[i - 1 + k - j] = temp;
	}

	return str;
}

// Check if overflow occurs when str converts to integer
inline int strtol_wrapper(const char* str, char** endptr, int base) {
	int result = strtol(str, endptr, base);
	if (errno == ERANGE) {
		cerr << "overflow" << endl;
		err_flag = true;
		return 0;
	}
	return result;
}

// Check if expr overflows
inline bool overflow_check(int a, int b, type op) {
	switch (op) {
	case TK_PLUS:
		if (a > 0 && b > 0) {
			if (INT_MAX - b < a) {
				err_flag = true;
				return true;
			}
		}
		else if (a < 0 && b < 0) {
			if (INT_MIN - b > a) {
				err_flag = true;
				return true;
			}
		}
	case TK_MINUS:
		if (a > 0 && b < 0) {
			if (INT_MAX + b < a) {
				err_flag = true;
				return true;
			}
		}
		else if (a < 0 && b > 0) {
			if (INT_MIN + b > a) {
				err_flag = true;
				return true;
			}
		}
	case TK_MUL:
		if (a * b / b != a) {
			err_flag = true;
			return true;
		}
	// Divide overflows when "INT_MIN/-1"
	case TK_DIV:
		if (a == INT_MIN && b == -1) {
			err_flag = true;
			return true;
		}
	case TK_UMINUS:
		if (a == INT_MIN) {
			err_flag = true;
			return true;
		}
	}
	return false;
}

// Calculating function
int eval() {
	err_flag = false;
	Token stack[MAX_TOKEN_NUM];
	Token origin;
	origin.token_type = 1;
	stack[0] = origin;
	int stack_top = 1;

	int curr = 0;
	while (curr <= num) {
		Token* cur_token = (tokens + curr);
		int action = state_table[stack[stack_top - 1].token_type - 1][table_entry(cur_token->token_type)];
		if (action > 0) {
			if (action == ac) {
				return strtol_wrapper(stack[1].str, NULL, 0);
			}
			stack[stack_top].token_type = cur_token->token_type;
			strcpy(stack[stack_top].str, cur_token->str);
			stack_top++;

			stack[stack_top].token_type = action;
			stack_top++;
			curr++;
		}
		else {
			int rule_len = rule_lens[-action];
			stack_top -= rule_len * 2 - 1;
			int state = stack[stack_top - 2].token_type;
			switch (-action)
			{
			case 1: {
				stack[stack_top - 1].token_type = TK_A;
				int operand1 = strtol_wrapper(stack[stack_top - 1].str, NULL, 0);
				int operand2 = strtol_wrapper(stack[stack_top + 3].str, NULL, 0);
				operand1 = operand1 || operand2;
				my_itoa(operand1, stack[stack_top - 1].str, 10);
				break;
			}
			case 2:
				stack[stack_top - 1].token_type = TK_A;
				break;
			case 3: {
				stack[stack_top - 1].token_type = TK_B;
				int operand1 = strtol_wrapper(stack[stack_top - 1].str, NULL, 0);
				int operand2 = strtol_wrapper(stack[stack_top + 3].str, NULL, 0);
				operand1 = operand1 && operand2;
				my_itoa(operand1, stack[stack_top - 1].str, 10);
				break;
			}
			case 4:
				stack[stack_top - 1].token_type = TK_B;
				break;
			case 5:{
				stack[stack_top - 1].token_type = TK_C;
				int operand1 = strtol_wrapper(stack[stack_top - 1].str, NULL, 0);
				int operand2 = strtol_wrapper(stack[stack_top + 3].str, NULL, 0);
				int op = stack[stack_top + 1].token_type;
				switch (op) {
				case TK_GT:
					operand1 = operand1 > operand2;
					break;
				case TK_LT:
					operand1 = operand1 < operand2;
					break;
				case TK_NLT:
					operand1 = operand1 >= operand2;
					break;
				case TK_NGT:
					operand1 = operand1 <= operand2;
					break;
				case TK_EQU:
					operand1 = operand1 == operand2;
					break;
				case TK_NEQ:
					operand1 = operand1 != operand2;
					break;
				default:
					cerr << "unknown op(compare operator)" << endl;
					exit(UNKNOWN_OP_ERR);
				}
				my_itoa(operand1, stack[stack_top - 1].str, 10);
				break;
			}
			case 6:
				stack[stack_top - 1].token_type = TK_C;
				break;
			case 7: {
				stack[stack_top - 1].token_type = TK_D;
				int operand1 = strtol_wrapper(stack[stack_top - 1].str, NULL, 0);
				int operand2 = strtol_wrapper(stack[stack_top + 3].str, NULL, 0);
				int op = stack[stack_top + 1].token_type;
				if (op == TK_PLUS) {
					if (overflow_check(operand1, operand2, TK_PLUS)) {
						cerr << "overflow" << endl;
						return 0;
					}
					operand1 += operand2;
				}
				else if (op == TK_MINUS) {
					if (overflow_check(operand1, operand2, TK_MINUS)) {
						cerr << "overflow" << endl;
						return 0;
					}
					operand1 -= operand2;
				}
				else {
					cerr << "unknown op(plus&minus)" << endl;
					exit(UNKNOWN_OP_ERR);
				}
				my_itoa(operand1, stack[stack_top - 1].str, 10);
				break;
			}
			case 8:
				stack[stack_top - 1].token_type = TK_D;
				break;
			case 9: {
				stack[stack_top - 1].token_type = TK_E;
				int operand1 = strtol_wrapper(stack[stack_top - 1].str, NULL, 0);
				int operand2 = strtol_wrapper(stack[stack_top + 3].str, NULL, 0);
				int op = stack[stack_top + 1].token_type;
				if (op == TK_MUL) {
					if (overflow_check(operand1, operand2, TK_MUL)) {
						cerr << "overflow" << endl;
						return 0;
					}
					operand1 *= operand2;
				}
				else if (op == TK_DIV) {
					if (operand2 == 0) {
						cerr << "div zero fault" << endl;
						err_flag = true;
						return 0;
					}
					if (overflow_check(operand1, operand2, TK_DIV)) {
						cerr << "overflow" << endl;
						return 0;
					}
					operand1 /= operand2;
				}
				else {
					cerr << "unknown op(mul&div)" << endl;
					exit(UNKNOWN_OP_ERR);
				}
				my_itoa(operand1, stack[stack_top - 1].str, 10);
				break;
			}
			case 10:
				stack[stack_top - 1].token_type = TK_E;
				break;
			case 11: {
				stack[stack_top - 1].token_type = TK_F;
				int val = strtol_wrapper(stack[stack_top + 1].str, NULL, 0);
				my_itoa(val, stack[stack_top - 1].str, 10);
				break;
			}
			case 12:
				stack[stack_top - 1].token_type = TK_F;
				break;
			case 13: {
				int op = stack[stack_top - 1].token_type;
				stack[stack_top - 1].token_type = TK_F;
				int val = strtol_wrapper(stack[stack_top + 1].str, NULL, 0);
				if (op == TK_UMINUS) {
					if (overflow_check(val, 0, TK_UMINUS)) {
						cerr << "overflow" << endl;
						return 0;
					}
					val = -val;
				}
				else if (op == TK_LNOT) {
					val = !val;
				}
				my_itoa(val, stack[stack_top - 1].str, 10);
				break;
			}
			default:
			{
				cerr << "reduction error" << endl;
				exit(SYNTAX_ERR);
			}
			}
			// stack_top++;
			// GOTO ACTION
			stack[stack_top].token_type = state_table[state - 1][table_entry(stack[stack_top - 1].token_type)];
			stack_top++;
		}
	}
	cerr << "unknown error" << endl;
	exit(UNKNOWN_ERR);
	return 0;
}

// Interface
void cal(const char expr[]) {
	make_token(expr);
	int result = eval();
	if (!err_flag)
		cout << result << endl;
}

// Testing function
void test(){
	cout << "1>=1" << endl;
	cal("1>=1");
	cout << endl;
	
	cout << "30*(14 + 6)/100 == 6" << endl;
	cal("30*(14 + 6)/100 == 6");
	cout << endl;
	
	cout << "!(1 && 0)" << endl;
	cal("!(1 && 0)");
	cout << endl;
	
	cout << "-2147483647-1(-INT_MAX-1=INT_MIN)" << endl;
	cal("-2147483647-1");
	cout << endl;
	
	cout << "2147483647+1(overflow)" << endl;
	cal("2147483647+1");
	cout << endl;
	
	cout << "1024*1024*1024*1024(overflow)" << endl;
	cal("1024*1024*1024*1024");
	cout << endl;

	cout << "1/(1-1)(divide zero fault)" << endl;
	cal("1/(1-1)");
	cout << endl;
}


