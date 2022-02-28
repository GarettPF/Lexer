#pragma once
#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include "Token.h"

using namespace std;

class Lexer {
	private:
		ifstream *programFile;
		string statement;
		vector<Token*> tokens;

	public:
		Lexer(ifstream *file) : programFile(file) {};

		/**
		 * @brief	Parses the current statement that was read in
		 *			will indentify and seperate the statement into different
		 *			Tokens and verify the type and value of each. These tokens
		 *			are stored into the tokens array.
		*/
		void parse() {
			string current;
			Token::Kind longest = Token::Kind::null;
			Token::Kind potential = Token::Kind::null;
			int count = 0, attempts = 0;

			while (!statement.empty()) {
				while (statement[0] == ' ' || statement[0] == '\t')
					statement.erase(statement.begin());
				current = statement.substr(0, count);

				// check for any matches
				if (isKeyword(current))
					longest = Token::Kind::keyword;
				else if (isId(current))
					longest = Token::Kind::id;
				else if (isString(current))
					longest = Token::Kind::string;
				else if (isInteger(current)) {
					longest = Token::Kind::integer;
					potential = Token::Kind::decimal;
				} else if (isDecimal(current))
					longest = Token::Kind::decimal;
				else if (isComment(current)) {
					statement.erase(0, count);
					count = 0;
					longest = Token::Kind::null;
					attempts = 0;
				} else if (isOp(current))
					longest = Token::Kind::op;
				else if (isEos(current) && count == 1)
					longest = Token::Kind::eos;
				else {
					if (longest == Token::Kind::comment) {
						statement.erase(0, count);
						count = 0;
						longest = Token::Kind::null;
						attempts = 0;
					}
					if (longest != Token::Kind::null &&
						potential != Token::Kind::decimal) {
						// no more matches
						count--;
						if (longest != Token::Kind::comment && longest != Token::Kind::string) { 
							// ignore comments and strings
							Token *t = new Token(longest, current.substr(0, count));
							tokens.push_back(t);
						}
						statement.erase(0, count);
						count = 0;
						longest = Token::Kind::null;
						attempts = 0;
					} else
						if (attempts++ == 100) exit(-1); 
							// statement has an error since it passed the limit
					potential = Token::Kind::null;
				}
				count++;
			}
		}

		/**
		 * @brief	Gets the next string up until the ; character
		 *			newlines and tabs are ignored
		 * @return	true if file is empty, false otherwise
		*/
		bool getNext() {
			char c;
			statement = "";
			while (programFile->get(c)) {
				if (c == '\n' || programFile->eof())
					break;
				statement.push_back(c);
			}
			return !programFile->eof();
		}

		/**
		 * @brief	Prints to the console the array of tokens that we have
		 *			in a nice and orderly fashion
		*/
		void print() {
			cout.width(20); cout << left << "Token type" << "Token Value" << endl;
			cout << "--------------------------------" << endl;
			for (auto it = tokens.begin(); it != tokens.end(); it++) {
				cout.width(20);
				cout << left;
				cout << (*it)->getKind() << (*it)->getLexeme() << endl;
			}
		}

		// ############# BOOLEAN FUNCTIONS ################ //

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
			if (s.empty()) return false;
			if (s.front() == 34 && s.back() == 34 && s.length() > 1) { // 34 = "
				string text = s.substr(1, s.length() - 2);
				if (isText(text)) {
					Token *str = new Token(Token::Kind::string, s);
					tokens.push_back(str);
					return true;
				}
			} else if (s.front() == 39 && s.back() == 39 && s.length() > 1) { // 39 = '
				string text = s.substr(1, s.length() - 2);
				if (!isText(text)) return false;

				// check for interpolation
				int beg = 0;
				for (int i = 0; i < s.length(); i++) {
					if (s[i] == 92) {
						Token *str = new Token(Token::Kind::string, s.substr(beg, i-beg+1));
						tokens.push_back(str);
						beg = i++;

						for (int j = 1; j < s.length() - i; j++) {
							if (!isId(s.substr(i, j))) {
								Token *id = new Token(Token::Kind::id, s.substr(i, j-1));
								tokens.push_back(id);
								beg = i + j - 2;
								i = beg;
								break;
							}
						}
					}
				}
				Token *str = new Token(Token::Kind::string, s.substr(beg, s.length()-beg));
				tokens.push_back(str);
				return true;
			}
			return false;
		}

		bool isInteger(string s) {
			if (isNonZero(s[0])) {
				s.erase(s.begin());
				for (char c : s) {
					if (!isDigit(c)) return false;
				}
				return true;
			}
			return false;
		}

		bool isDecimal(string s) {
			if (isDigit(s[0])) {
				for (int i = 1; i < s.length(); i++) {
					if (s[i] == '.') {
						if (isDigit(s[i + 1])) {
							for (int j = i + 2; j < s.length(); j++) {
								if (!isDigit(s[j])) return false;
							}
							return true;
						}
					} else if (!isDigit(s[i])) return false;
				}
			}
			return false;
		}

		bool isComment(string s) {
			if (s.empty()) return false;
			if (s.front() == '#' && s.back() == '#' && s.length() > 1)
				return true;
			return false;
		}

		bool isText(string s) {
			for (char c : s) {
				try {
					if (isLetter(c) || isDigit(c) || isSymbol(c) || c == ' ');
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
				case '=':
				case '+':
				case '-':
				case '/':
				case '\'':
					return true;
			}
			return false;
		}

		bool isOp(string s) {
			if (s.length() == 1) {
				switch (s[0]) {
					case '=':
					case '+':
					case '-':
					case '*':
					case '/':
						return true;
				}
			}
			if (s == "==")
				return true;
			return false;
		}

		bool isEos(string s) {return s[0] == ';'; }
};