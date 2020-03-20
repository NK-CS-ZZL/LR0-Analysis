#include <regex>
#include "token.h"
using namespace std;

// Lexical analysis


extern Token tokens[MAX_TOKEN_NUM];
extern int num;

static regex rules[RULES_NUMBER] = {
	regex("([ |\t]+)"),				// space or tab
	regex("[1-9][0-9]+|[0-9]"),		// integer
	regex("<="),					// not great than
	regex(">="),					// not less than
	regex(">"),						// great than
	
	regex("<"),						// less than
	regex("!="),					// not equal
	regex("=="),					// equal
	regex("!"),						// logic not
	regex("&&"),					// logic and
	
	regex("\\|\\|"),				// logic or
	regex("\\+"),					// plus
	regex("\\-"),					// minus
	regex("\\*"),					// multiply
	regex("/"),						// divide
	
	regex("\\("),					// left parenthesis
	regex("\\)"),					// right parenthesis
};
// Judge if it matches the first token of remaining sentence
inline bool is_header(const smatch& smh) {
	if (smh.empty() || smh.prefix().str().length() != 0)
		return false;
	return true;
}
// Push a token into "tokens"
inline void push_tk(const type tk_type, const char* str) {
	tokens[num].token_type = tk_type;
	strcpy(tokens[num].str, str);
	num++;
}
inline void push_tk(const type tk_type) {
	tokens[num].token_type = tk_type;
	num++;
}
// It's used to distinguish unary minus with binary minus.
static bool unary_flag = true;

bool match(string& input) {
	for (int i = 0; i < RULES_NUMBER; i++) {
		smatch smh;
		regex_search(input, smh, rules[i]);
		if (is_header(smh)) {
			if (i != 0) {
				if (i == 1) { 
					push_tk(type(i + TK_NOTYPE), smh.str().c_str()); 
					unary_flag = false;
				}
				else if (i == 12) {
					if (unary_flag)
						push_tk(type(TK_UMINUS));
					else
						push_tk(type(i + TK_NOTYPE));
					unary_flag = true;
				}
				else {
					push_tk(type(i + TK_NOTYPE));
					unary_flag = true;
				}
			}
			input = input.substr(smh.str().length(), input.length());
			return true;
		}
	}
	cerr << "Unrecongnized token." << endl;
	return false;
}

void make_token(const char* str) {
	memset(tokens, 0, sizeof(Token) * MAX_TOKEN_NUM);
	num = 0;
	string input(str);
	while (!input.empty()) {
		if (!match(input))
			exit(UNKNOWN_TOKEN_ERR);
	}
	tokens[num].token_type = TK_EOF;
}

