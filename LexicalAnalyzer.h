#ifndef LEX_H
#define LEX_H

#include <iostream>
#include <fstream>

using namespace std;

enum token_type {NONE = -1, IDENT = 100, NUMLIT, STRLIT, LISTOP, CONS, IF,
                 COND, ELSE, DISPLAY, NEWLINE, AND, OR, NOT, DEFINE,
		 NUMBERP, LISTP, ZEROP, NULLP, STRINGP, PLUS, MINUS, DIV,
		 MULT, MODULO, ROUND, EQUALTO, GT, LT, GTE, LTE, LPAREN,
                 RPAREN, SQUOTE, BU, GD, ER, EOF_T, NUM_TOKENS};

class LexicalAnalyzer 
{
    public:
	LexicalAnalyzer (char * filename);
	~LexicalAnalyzer ();
	token_type GetToken ();
	string GetTokenName (token_type t) const;
	string GetLexeme () const;
	void ReportError (const string & msg);

	// functions added
	int parseInput();
	string getLine ();
	int getLineNum ();
	int nextState (int currentState, char currentChar);
	void SetToken(int state);
	void FindKeywords(int currentState);
	void FindPredicates();
	void FindOtherTypes (int currentState);
	bool readNewLine();

    private:
	ifstream input;
	ofstream listingFile;
	ofstream tokenFile;
	ofstream debugFile;
	token_type token;
	string line;
	int lineNum;
	int pos;
	string lexeme;
	int errors;
	bool newLine;
	bool stringLitInProgress; //used when evaluating a STRLIT type

};
	
#endif
