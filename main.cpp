#include "cal.h"
#include<iostream>
#include<string>
using namespace std;

int main() {
	char buffer[512] = { 0 };
	while (buffer[0] != 'q') {
		cin >> buffer;
		if (strcmp(buffer, "help") == 0) {
			cout << "Enter a expression and I'll give you the answer" << endl;
			cout << "It supports +, -, *, /, >, <, >=, <=, !=, ==, &&, ||, !" << endl;
		}
		else if (strcmp(buffer, "test") == 0) {
			cout << "testing:" << endl;
			test();
		}
		else {
			cal(buffer);
		}
		memset(buffer, 0, sizeof(buffer));
	}
	return 0;
} 