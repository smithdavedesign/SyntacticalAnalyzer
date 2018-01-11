/********************************************************************************/
/* Project: Project 2 								*/
/* Author: Dave Smith								*/
/* Date: 4/16/2017								*/
/* Description: phase 2 of compiler						*/
/********************************************************************************/
#include <iostream>
#include <iomanip>
#include <cstring>
#include <fstream>
#include "SyntacticalAnalyzer.h"

using namespace std;
/*based off LexicalAnalyzer.h
enum token_type
1. LAMBDA
2. IDENT_T
3. NUMLIT_T
4. CONS_T
5. IF_T
6. DISPLAY_T
7. NEWLINE_T
8. LISTOP_T
9. AND_T
10. OR_T
11. NOT_T
12. DEFINE_T
13. NUMBERP_T
14. SYMBOLP_T
15. LISTP_T
16. ZEROP_T
17. NULLP_T
18. CHARP_T
19. STRINGP_T
20. PLUS_T
21. MINUS_T
22. DIV_T
23. MULT_T
24. EQUALTO_T
25. GT_T
26. LT_T
27. GTE_T
28. LTE_T
29. LPAREN_T
30. RPAREN_T
31. QUOTE_T
32. ERROR_T
33. EOF_T
34. MAX_TOKENS
*/

static int gram_rules[12][34] =
  {{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,1,-1,-1,-1,-1,-1},/*<program>*/
   {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,2,-1,-1,-1,-1,-1},/*<define>*/
   {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,3,-1,-1,-1,4,-1},/*<more_defines>*/
   {-1,5,5,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,5,6,5,-1,-1,-1},/*<stmt_list>*/
   {-1,8,7,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,9,-1,7,-1,-1,-1},/*<stmt>*/
   {-1,-1,10,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,11,-1,-1,-1},/*<literal>*/
   {-1,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,-1,12,-1,-1,-1},/*<quoted_lit>*/
   {-1,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,14,13,-1,-1,-1},/*<more_tokens>*/
   {-1,15,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,16,-1,-1,-1,-1},/*<param_list>*/
   {-1,17,17,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,17,18,17,-1,-1,-1},/*<else_part>*/
   {-1,41,-1,21,19,42,43,20,22,23,24,-1,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,-1,-1,-1,-1,-1,-1},/*<action>*/
   {-1,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,44,-1,72,-1,-1,-1}};/*<any_other_token>*/

/********************************************************************************/
/* This function will: creat new instance of class, init my file. gets first token/
/********************************************************************************/
SyntacticalAnalyzer::SyntacticalAnalyzer (char * filename)
{
	lex = new LexicalAnalyzer (filename);
	int fnlength = strlen (filename);
	filename[fnlength-2] = 'p';
	filename[fnlength-1] = '2';
	p2file.open (filename);
	token = lex->GetToken();
	int errors = program ();
}

/********************************************************************************/
/* This function will: deconstructor                                            */
/********************************************************************************/
SyntacticalAnalyzer::~SyntacticalAnalyzer ()
{
	delete lex;
	p2file.close();
}

/*
The following functions will call each other untill they all return, making a large tree of non terminals
using the rules given by the functions. It will parse through are guile program, and make sure that there 
are no syntax errors. 
*/
/********************************************************************************/
/* This function will: rule 1.
/********************************************************************************/
int SyntacticalAnalyzer::program ()
{//0

  lex->debug << "program function called\n";
  p2file << "Starting <program>. ";
  int errors = 0;

  // token should be in the firsts of program
  // if true... keep going
  // if false...
  // Error message -
  // then keep going or keep getting token until token is
  // in the firsts of program

  while (gram_rules[0][token] == -1) {
    lex->ReportError("Not in firsts of program\n");
    errors++;
    token = lex->GetToken();
  }
  
  p2file << "Current token = " << lex->GetTokenName(token) << endl;
  p2file << "Using rule " << gram_rules[0][token] << endl;
  //grabing wrong token?
  //token = lex->GetToken();

  
  errors += define();

  errors += more_defines();
  

  if (token != EOF_T)
    {
      lex->ReportError ("Expected end of file; " + lex->GetLexeme ());
      errors++;
    }
  
  p2file << "Ending <program>. Current token = " << lex->GetTokenName(token)
	 << ". Errors = " << errors << endl;
  
  // token should be in the follows of program
  // if true... keep going if false... Error message -
  // then keep going or keep getting token until token is
  // in the follows of program
  
  lex->debug << "program function returning " << errors << " errors\n";
  return errors;
}


/********************************************************************************/
/* This function will: rule 2, */
/********************************************************************************/
int SyntacticalAnalyzer::define()
{//1
  //define = row1;
  //write to debug
  int errors = 0;
  p2file << "Starting <define>. ";
  lex->debug << "define function called\n";
  p2file << "Current token = " << lex->GetTokenName(token) << endl;
  
  while(gram_rules[1][token] == -1)
    {
      lex->ReportError("Not in firsts of define\n");
      errors++;
      token = lex->GetToken();//grab next token
    }


  p2file << "Using rule " << gram_rules[1][token] << endl;

  while(token != LPAREN_T)
    {
      lex->ReportError ("Expected LPAREN_T; " + lex->GetLexeme ());
      errors++;
      token = lex->GetToken();
    }
  token = lex->GetToken();
  while(token != DEFINE_T)
    {
      lex->ReportError ("Expected DEFINE_T; " + lex->GetLexeme ());
      errors++;
      token = lex->GetToken();
    }
  token = lex->GetToken();
  while(token != LPAREN_T)
    {
      lex->ReportError ("Expected LPAREN; " + lex->GetLexeme ());
      errors++;
      token = lex->GetToken();
    }
        token = lex->GetToken();
  while(token != IDENT_T)
    {
      lex->ReportError ("Expected LPAREN; " + lex->GetLexeme ());
      errors++;
      token = lex->GetToken();
    }
  token = lex->GetToken();
  
  errors += param_list();
  
  while(token != RPAREN_T)
    {
      lex->ReportError ("Expected RPAREN; " + lex->GetLexeme ());
      errors++;
      token = lex->GetToken();
    }
  token = lex->GetToken();

  errors += stmt();
  errors += stmt_list();
  
  while(token != RPAREN_T)
    {
      lex->ReportError ("Expected RPAREN; " + lex->GetLexeme ());
      errors++;
      token = lex->GetToken();
    }
  token = lex->GetToken();
  
  p2file << "Ending <define>. Current token = " << lex->GetTokenName(token)
	 << ". Errors = " << errors << endl;
  
  lex->debug << "define function returning " << errors << " errors\n" << endl;
  return errors;
}  


/********************************************************************************/
/* This function will: rules 3-4*/
/********************************************************************************/

int SyntacticalAnalyzer::more_defines()
{//2
  p2file << "Starting <more_defines>. ";
  lex->debug << "more_defines function called\n";

  int errors = 0;

  p2file << "Current token = " << lex->GetTokenName(token) << endl;
  p2file << "Using rule " << gram_rules[2][token] << endl;
  if (gram_rules[2][token] == 3)
    {
      errors += define();
      errors += more_defines();
    }
  
  if (token != EOF_T)
    {
      lex->ReportError ("Expected end of file; " + lex->GetLexeme());
      errors++;
    }
  p2file << "Ending <more_defines>. Current token = " << lex->GetTokenName(token)
	 << ". Errors = " << errors << endl;
  
  lex->debug << "more_defines function returning " << errors << " errors\n";
  return errors;
}

/********************************************************************************/
/* This function will: rules 5-6 */
/********************************************************************************/
int SyntacticalAnalyzer::stmt_list()
{//3
  int errors = 0;
  p2file << "Starting <stmt_list>. ";
  lex->debug << "stmt function called\n";

  p2file << "Current token = " << lex->GetTokenName(token) << endl;
  p2file << "Using rule " << gram_rules[3][token] << endl;
  
  while(gram_rules[3][token] == -1)
    {
      lex->ReportError("Not in firsts of stmt\n");
      errors++;
      token = lex->GetToken();//grab next token
    }

  if(gram_rules[3][token] == 5)
    {
      errors += stmt();
      errors+= stmt_list();
    }

  //LAMDA should be on a RPAREN
  if(gram_rules[3][token] == 6)
    {
    }

  p2file << "Ending <stmt_list>. Current token = " << lex->GetTokenName(token)
	 << ". Errors = " << errors << endl;

  lex->debug << "stmt_list function returning " << errors << " errors\n" << endl;
  return errors;
}

/********************************************************************************/
/* This function will: rules 7-9*/
/********************************************************************************/
int SyntacticalAnalyzer::stmt()
{//4
  int errors = 0;
  p2file << "Starting <stmt>. ";
  lex->debug << "stmt function called\n";
  
  
  while(gram_rules[4][token] == -1)
    {
      lex->ReportError("Not in firsts of stmt\n");
      errors++;
      token = lex->GetToken();//grab next token
    }
  p2file << "Current token = " << lex->GetTokenName(token) << endl;
  p2file << "Using rule " << gram_rules[4][token] << endl;
  //rule 8 
  if (gram_rules[4][token]== 8)
    {
      token = lex->GetToken();
    }
 //rule 9 is LPAREN
  else if (gram_rules[4][token] == 9)
    {
      token = lex->GetToken();
      errors += action();
      while(token != RPAREN_T)
	{
	  lex->ReportError ("Expected RPAREN_T " + lex->GetLexeme ());
	  token = lex->GetToken();
	  errors++;
	}
      token = lex->GetToken();
    }      
  //rule 7
  else if (gram_rules[4][token] == 7)
    {
      errors +=literal();
   }
  
  p2file << "Ending <stmt>. Current token = " << lex->GetTokenName(token)
	 << ". Errors = " << errors << endl;
  
  lex->debug << "stmt function returning " << errors << " errors\n" << endl;
  return errors;
 
}

/********************************************************************************/
/* This function will: rukes 10-11*/
/********************************************************************************/
int SyntacticalAnalyzer::literal()
{
  //5
  int errors = 0;
  p2file << "Starting <literal>. ";
  lex->debug << "literal function called\n";


  p2file << "Current token = " << lex->GetTokenName(token) << endl;
  p2file << "Using rule " << gram_rules[5][token] << endl;

  //COVERS RULE 10 
  while(gram_rules[5][token] == -1)
    {
      lex->ReportError("Not in firsts of literal\n");
      errors++;
      token = lex->GetToken();//grab next token
    }
  
  if (gram_rules[5][token] == 10)
    {
      token = lex->GetToken();
    }
  
  else if (gram_rules[5][token] == 11)
    {
      token = lex->GetToken();
      errors += quoted_lit();
    }
  
  p2file << "Ending <literal>. Current token = " << lex->GetTokenName(token)
	 << ". Errors = " << errors << endl;
  lex->debug << "literal function returning " << errors << " errors\n" << endl;
  return errors;
}

/********************************************************************************/
/* This function will: rule 12*/
/********************************************************************************/
int SyntacticalAnalyzer::quoted_lit()
{
  //6
  p2file << "Starting <quoted_lit>. ";
  int errors = 0;
  lex->debug << "quoted_lit function called\n";
  
  p2file << "Current token = " << lex->GetTokenName(token) << endl;
  p2file << "Using rule " << gram_rules[6][token] << endl;
  
  errors += any_other_token();
  
  p2file << "Ending <quoted_lit>. Current token = " << lex->GetTokenName(token)
	 << ". Errors = " << errors << endl;
  lex->debug << "quoted_lit function returning " << errors << " errors\n" << endl;
  return errors;
}
/********************************************************************************/
/* This function will: rules 13-14*/
/********************************************************************************/
int SyntacticalAnalyzer::more_tokens()
{//7
  p2file << "Starting <more_tokens>. ";
  int errors = 0;
  lex->debug << "more_tokens function called\n";


  p2file << "Current token = " << lex->GetTokenName(token) << endl;
  p2file << "Using rule " << gram_rules[7][token] << endl;

  if(gram_rules[7][token] == 13)
    {
      errors += any_other_token();
      errors += more_tokens();
    }
  
  p2file << "Ending <more_tokens>. Current token = " << lex->GetTokenName(token)
	 << ". Errors = " << errors << endl;
  lex->debug << "more_tokens function returning " << errors << " errors\n" << endl;
  return errors;

}
/********************************************************************************/
/* This function will: rules 15-16*/
/********************************************************************************/
int SyntacticalAnalyzer::param_list()
{//8
  int errors = 0;
  p2file << "Starting <param_list>. ";
  lex->debug << "param function called\n";

  while (gram_rules[8][token] == -1)
    {
      lex->ReportError("Not in firsts of else_part: " + lex->GetLexeme());
      errors++;
      token = lex->GetToken();
    }
  p2file << "Current token = " << lex->GetTokenName(token) << endl;
  p2file << "Using rule " << gram_rules[8][token] << endl;
  
  
  if(token == IDENT_T)
    {
      token = lex->GetToken();
      errors += param_list();
    }
  
  while (token != RPAREN_T)
    {
      lex->ReportError("Not in follows of param_list: " + lex->GetLexeme());
      errors++;
      token = lex->GetToken();
    }
  p2file << "Ending <param_list>. Current token = " << lex->GetTokenName(token)
	 << ". Errors = " << errors << endl;
  lex->debug << "param_list function returning " << errors << " errors\n" << endl;
  return errors;  

}  
/********************************************************************************/
/* This function will: rules 17-18*/
/********************************************************************************/
int SyntacticalAnalyzer::else_part()
{//9
  int errors = 0;  p2file << "Starting <else_part>. ";
  lex->debug << "else function called\n";

  p2file << "Current token = " << lex->GetTokenName(token) << endl;
  p2file << "Using rule " << gram_rules[9][token] << endl;

  if(gram_rules[9][token] == 17)
    {
      errors += stmt();
    }

  p2file << "Ending <else_part>. Current token = " << lex->GetTokenName(token)
	 << ". Errors = " << errors << endl;
  lex->debug << "else_part function returning " << errors << " errors\n" << endl;
  return errors;
}
/********************************************************************************/
/* This function will: rules 20-43*/
/********************************************************************************/
int SyntacticalAnalyzer::action()
{//10
  int errors = 0;
  p2file << "Starting <action>. ";
  lex->debug << "action function called\n";
  
  p2file << "Current token = " << lex->GetTokenName(token) << endl;
  p2file << "Using rule " << gram_rules[10][token] << endl;
  
  while (gram_rules[10][token] == -1)
    {
      lex->ReportError("Not in firsts of program\n");
      errors++;
      token = lex->GetToken();
    }
  //rule 19 
  if(gram_rules[10][token] == 19)
    {
      token = lex->GetToken();//grab next token
      errors += stmt();
      errors += stmt();
      errors += else_part();
    }
  //rule 21
  if(gram_rules[10][token] == 21)
    {
      token = lex->GetToken();
      errors += stmt();
      errors += stmt();
    }
  //rules 20 && 24-31
  if(gram_rules[10][token] == 20 || (gram_rules[10][token] >= 24 && gram_rules[10][token] <= 31) || gram_rules[10][token] == 42)
    {
      token = lex->GetToken();
      errors += stmt();
    }
  //rules 35-41 & 32 & 22-23
  if(gram_rules[10][token] == 32 || (gram_rules[10][token] >= 35 && gram_rules[10][token] <= 41) || gram_rules[10][token] == 22 || gram_rules[10][token] == 23)
    {
      token = lex->GetToken();
      errors += stmt_list();
    }
  //rules 33 and 34
  if(gram_rules[10][token] == 33 || gram_rules[10][token] == 34)
    {
      token = lex->GetToken();
      errors +=stmt();
      errors += stmt_list();
    }
  if(gram_rules[10][token] == 43)
    {
      token = lex->GetToken();
    }
  
  p2file << "Ending <action>. Current token = " << lex->GetTokenName(token)
	 << ". Errors = " << errors << endl;
  lex->debug << "action function returning " << errors << " errors\n" << endl;
  return errors;
}
/********************************************************************************/
/* This function will: rules 44-72*/
/********************************************************************************/
int SyntacticalAnalyzer::any_other_token()
{//11
  p2file << "Starting <any_other_token>. ";
  int errors = 0;
  lex->debug << "any_other_token function called\n";
  p2file << "Current token = " << lex->GetTokenName(token) << endl;
  p2file << "Using rule " << gram_rules[11][token] << endl;
  
  if(gram_rules[11][token] == 44)
    {
      token = lex->GetToken();//grab next token     
      errors += more_tokens(); 
      while(token != RPAREN_T)
	{
	  lex->ReportError("Not in firsts of any_other_token\n");
	  errors++;
	  token = lex->GetToken();//grab next token     
	}
    }
  token = lex->GetToken();
  
  p2file << "Ending <any_other_token>. Current token = " << lex->GetTokenName(token)
	 << ". Errors = " << errors << endl;
  lex->debug << "any_other_token function returning " << errors << " errors\n" << endl;
  return errors;
}

//not used
void SyntacticalAnalyzer::find_NonT(int rule)
{
}
