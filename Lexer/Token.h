#pragma once
#include <string>

using namespace std;

class Token {
	public:
		enum class Kind {
			keyword,
			id,
			string,
			integer,
			decimal,
			comment,
			op,
			eos,
			null
		};

		Token(Kind k, string l) : kind(k), lexeme(l) {};

		string getKind() {
			string kindString[] = {
				"keyword", "identifier", "string", "integer", "decimal",
				"comment", "operator", "eos"
			};
			return kindString[(int)kind];
		}

		string getLexeme() {
			if (kind == Kind::string)
				return lexeme.substr(1, lexeme.length() - 2);
			return lexeme;
		}

	private:
		Kind kind;
		string lexeme;
};