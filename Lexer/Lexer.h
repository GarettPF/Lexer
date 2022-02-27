#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include "Token.h"

using namespace std;

class Lexer {
	private:
		ifstream *programFile;
		string current;
		vector<Token> tokens;

	public:
		Lexer(ifstream *file) : programFile(file) {};

		void parse() {

		}

		/**
		 * @brief gets the next string up until the ; character is encountered
		 * any whitespaces, newlines, and tabs are ignored
		*/
		void getNext() {
			char c;
			current = "";
			while (programFile->get(c)) {
				if (c != ' ' && c != '\n' && c != '\t')
					current.push_back(c);
				if (c == ';')
					break;
			}
			cout << current << endl;
		}

		bool isKeyword(string s) {
			const string keywords[] = {
				"integer", "decimal", "string", "print", "define", "read"
			};
			for (int i = 0; i < 6; i++) {
				if (s == keywords[i])
					return true;
			}
			return false;
		}

		bool isId(string s) {
			if (isLetter(s[0]) || s[0] == '_') {
				for (char c : s) {
					try {
						if (isLetter(c) || isDigit(c) || c == '_');
						else return false;
					} catch (exception e) {
					}
				}
				return true;
			}
			return false;
		}

		bool isString(string s) {
			if (s.front() == 34 && s.back() == 34) { // 34 = "
				string text = s.substr(1, s.length() - 2);
				return isText(text);
			} else if (s.front() == 39 && s.back() == 39) { // 39 = '
				// TODO
			}
		}

		bool isText(string s) {
			for (char c : s) {
				try {
					if (isLetter(c) || isDigit(c) || isSymbol(c));
					else return false;
				} catch (exception e) { }
			}
			return true;
		}

		bool isLetter(char c) {
			bool result = false;
			if (c >= 65 && c <= 90)
				result = true;
			else if (c >= 97 && c <= 122)
				result = true;
			return result;
		}

		bool isNonZero(char c) {
			bool result = false;
			if (c >= 49 && c <= 57)
				result = true;
			return result;
		}

		bool isDigit(char c) {
			return (c == 48 || isNonZero(c));
		}

		bool isSymbol(char c) {
			switch (c) {
				case '!':
				case '@':
				case '#':
				case '$':
				case '%':
				case '^':
				case '&':
				case '*':
				case ';':
				case ':':
				case '\\':
					return true;
			}
			return false;
		}

		bool isOp(char *s) {
			switch (*s) {
				case '=':
				case '+':
				case '-':
				case '*':
				case '/':
					return true;
			}
			if (strcmp(s, "==") == 0)
				return true;
			return false;
		}

		bool isEos(char c) {return c == ';';}
};