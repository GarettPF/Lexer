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
			text,
			letter,
			nonZero,
			number,
			symbol,
			op,
			eos
		};

		Token(Kind k, string v) : kind(k), value(v) {};

	private:
		Kind kind;
		string value;
};