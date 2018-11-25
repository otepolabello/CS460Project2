#ifndef SYN_H
#define SYN_H

#include <iostream>
#include <fstream>
#include "LexicalAnalyzer.h"

using namespace std;

class SyntacticalAnalyzer 
{
    public:
	SyntacticalAnalyzer (char * filename);
	~SyntacticalAnalyzer ();
	int Program();
	int MoreDefines();
	int Define();
	int StmtList();
	int Stmt();
	int Literal();
	int MoreTokens();
	int ParamList();
	int ElsePart();
	int StmtPair();
	int StmtPairBody();
	int Action();
	int AnyOtherToken();
	int QuotedLit();
    private:
	LexicalAnalyzer * lex;
	token_type token;
};
	
#endif
