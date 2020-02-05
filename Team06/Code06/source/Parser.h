#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <stdexcept>

using namespace std;

namespace Parser {

	class Parser {
	public:
		Parser(string source);
		void parse();
	private:
		string src;
		size_t pos;

		void expect(string s);

		void program();
		void procedure();

	};
	int analyse(string& src);
}
#endif
