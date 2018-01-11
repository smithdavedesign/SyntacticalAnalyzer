/********************************************************************************/
/* Project: P2 									*/
/* Author: Dave Smith									*/
/* Date: April 4th									*/
/* Description:	SyntacicalAnalyzer.h								*/
/********************************************************************************/

#ifndef SYN_H
#define SYN_H

#include <map>
#include <iostream>
#include <fstream>
#include "LexicalAnalyzer.h"

using namespace std;

class SyntacticalAnalyzer 
{
    public:
	SyntacticalAnalyzer (char * filename);
	~SyntacticalAnalyzer ();
    private:
      	LexicalAnalyzer * lex;
	ofstream p2file;
	token_type token;

	int program ();
	int define();
	int more_defines();
	int stmt_list();
	int stmt();
	int literal();
	int quoted_lit();
	int more_tokens();
	int param_list();
	int else_part();
	int action();
	int any_other_token();
	void find_NonT(int);
};
#endif
