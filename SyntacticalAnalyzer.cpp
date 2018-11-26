#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstdlib>
#include "SyntacticalAnalyzer.h"

using namespace std;

SyntacticalAnalyzer::SyntacticalAnalyzer (char * filename)
{
  string name = filename;
  lex = new LexicalAnalyzer (filename);
  string p2name = name.substr (0, name.length()-3) + ".p2";
  p2file.open(p2name);
  token_type t;
  int errors = program();
  p2file << "This file has " << errors << " errors.\n";
}

SyntacticalAnalyzer::~SyntacticalAnalyzer ()
{
  delete lex;
}

int SyntacticalAnalyzer::program()
{
  int errors = 0;
  token = lex->GetToken();
  string tok = lex->GetTokenName(token), lexeme = lex->GetLexeme();
  p2file << "Entering Program Function; current token is: " << tok << ", lexeme: " << lexeme << endl;
  // apply rule 1
  if (token == 31)
    {
      p2file << "Using Rule 1\n";
      token = lex->GetToken();
      errors += define();
      if (token == 31)
	{
	  token = lex->GetToken();
	  errors += more_defines();
	}
      else
	{
	  lex->ReportError ("left parenthesis expected, '" + lex->GetTokenName(token) + "' found.");
	  errors++;
	}
      if (token == 35)
	{
	  // do nothing 
	}
      else
	{
	  lex->ReportError ("EOF token expected, '" + lex->GetTokenName(token) + "' found.");
          errors++;
	}
    }
  else
    {
      lex->ReportError ("left parenthesis expected, '" + lex->GetTokenName(token) + "' found.");
      errors++;
    }
  tok = lex->GetTokenName(token);
  p2file << "Exiting Program Function; current token is: " << tok << endl;
  return errors;
}

int SyntacticalAnalyzer::more_defines()
{
  string tok = lex->GetTokenName(token), lexeme = lex->GetLexeme();
  p2file << "Entering More_Defines Function; current token is: " << tok << ", lexeme: " << lexeme << endl;
  int errors = 0;
  if (token == 13)
    { // apply rule 2
      p2file << "Using Rule 2\n";
      errors += define();
      if (token == 31)
	{
	  token = lex->GetToken();
	  errors += more_defines();
	}
      else
	{
	  lex->ReportError ("left parenthesis expected, '" + lex->GetTokenName(token) + "' found.");
          errors++;
	}
    }
 
  else if (token == 1)
    { // apply rule 3
      p2file << "Using Rule 3\n";
      token = lex->GetToken();
      errors += stmt_list();
      if (token == 32)
	token = lex->GetToken();
      else
	{
	  lex->ReportError ("right parenthesis expected, '" + lex->GetTokenName(token) + "' found.");
          errors++;
	}
    }

  if (token == 35)
    { // apply rule 1
    }
  else
    {
      lex->ReportError ("IDENT_T or DEFINE_T expected, '" + lex->GetTokenName(token) + "' found.");
      errors++;
      }
  tok = lex->GetTokenName(token);
  p2file << "Exiting More_Defines Funtion; current token is: " << tok << endl;
  return errors;
}

int SyntacticalAnalyzer::define()
{
  string tok = lex->GetTokenName(token), lexeme = lex->GetLexeme();
  p2file << "Entering Define Function; current token is: " << tok << ", lexeme: " << lexeme << endl;
  int errors = 0;
  if (token == 13)
    { // apply rule 4
      p2file << "Using Rule 4\n";
      token = lex->GetToken();
      if (token == 31)
	{
	  token = lex->GetToken();
	  if (token == 1)
	    {
	      token = lex->GetToken();
	      errors += param_list();
	      if (token == 32)
		{
		  token = lex->GetToken();
		  errors += stmt();
		  errors += stmt_list();
		  if (token == 32)
		    token = lex->GetToken();
		  else
		    {
		      lex->ReportError ("right parenthesis expected, '" + lex->GetTokenName(token) + "' found.");
		      errors++;
		    }
		}
	      else
		{
		  lex->ReportError ("right parenthesis expected, '" + lex->GetTokenName(token) + "' found.");
		  errors++;
		}
	    }
	  else
	    {
	      lex->ReportError ("IDENT_T token expected, '" + lex->GetTokenName(token) + "' found.");
	      errors++;
	    }
	}
      else
	{
	  lex->ReportError ("left parenthesis expected, '" + lex->GetTokenName(token) + "' found.");
          errors++;
	}
    }
  else
    {
      lex->ReportError ("DEFINE_T token expected, '" + lex->GetTokenName(token) + "' found.");
      errors++;
    }
  tok = lex->GetTokenName(token);
  p2file << "Exiting Define Function; current token is: " << tok << endl;
  return errors;
}

int SyntacticalAnalyzer::stmt_list()
{
  string tok = lex->GetTokenName(token), lexeme = lex->GetLexeme();
  p2file << "Entering Stmt_List Function; current token is: " << tok << ", lexeme: " << lexeme << endl;
  int errors = 0;
  if (token == 2 || token == 3 || token == 33 || token == 1 || token == 31)
    { // apply rule 5
      p2file << "Using Rule 5\n";
      errors += stmt();
      errors += stmt_list();
    }
  else if (token == 32)
    { // apply rule 6
      p2file << "Using Rule 6\n";
    }
  else
    {
      lex->ReportError ("right parenthesis expected, '" + lex->GetTokenName(token) + "' found.");
      errors++;
    }
  tok = lex->GetTokenName(token);
  p2file << "Exiting Stmt_List Function; current token is: " << tok << endl;
  return errors;
}

int SyntacticalAnalyzer::stmt()
{
  string tok = lex->GetTokenName(token), lexeme = lex->GetLexeme();
  p2file << "Entering Stmt Function; current token is: " << tok << ", lexeme: " << lexeme << endl;
  int errors = 0;
  if (token == 2 || token == 3 || token == 33)
    { // apply rule 7
      p2file << "Using Rule 7\n";
      errors += literal();
    }
  else if (token == 1)
    { // apply rule 8
      p2file << "Using Rule 8\n";
      token = lex->GetToken();
    }
  else if (token == 31)
    { // apply rule 9
      p2file << "Using Rule 9\n";
      token = lex->GetToken();
      errors += action();
      if (token == 32)
	token = lex->GetToken();
      else
	{
	  lex->ReportError ("right parenthesis expected, '" + lex->GetTokenName(token) + "' found.");
          errors++;
	}
    }
  else
    {
      lex->ReportError ("NUMLIT_T, STRLIT_T, SQUOTE_T, IDENT_T, or LPAREN_T expected, '" + lex->GetTokenName(token) + "' found.");
      errors++;
    }
  tok = lex->GetTokenName(token);
  p2file << "Exiting Stmt Function; current token is: " << tok << endl;
  return errors;
}

int SyntacticalAnalyzer::literal()
{
  string tok = lex->GetTokenName(token), lexeme = lex->GetLexeme();
  p2file << "Entering Literal Function; current token is: " << tok << ", lexeme: " << lexeme << endl;
  int errors = 0;
  if (token == 2)
    { // apply rule 10
      p2file << "Using Rule 10\n";
      token = lex->GetToken();
    }
  else if (token == 3)
    { // apply rule 11
      p2file << "Using Rule 11\n";
      token = lex->GetToken();
    }
  else if (token == 33)
    { // apply rule 12
      p2file << "Using Rule 12\n";
      token = lex ->GetToken();
      errors += quoted_lit();
    }
  else
    {
      lex->ReportError ("NUMLIT_T, STRLIT_T or SQUOTE_T expected, '" + lex->GetTokenName(token) + "' found.");
      errors++;
    }
  tok = lex->GetTokenName(token);
  p2file << "Exiting Literal Function; current token is: " << tok << endl;
  return errors;
}

int SyntacticalAnalyzer::quoted_lit()
{
  string tok = lex->GetTokenName(token), lexeme = lex->GetLexeme();
  p2file << "Entering Quoted_Lit Function; current token is: " << tok << ", literal: " << lexeme << endl;
  int errors = 0;
  if (token >= 1 && token <= 31 || token == 33)
    { // apply rule 13
      p2file << "Using Rule 13\n";
      errors += any_other_token();
    }
  else
    {
      lex->ReportError ("any other token expected, '" + lex->GetTokenName(token) + "' found.");
      errors++;
    }
  tok = lex->GetTokenName(token);
  p2file << "Exiting Quoted_Lit Function; current token is: " << tok << endl;
  return errors;
}

int SyntacticalAnalyzer::more_tokens()
{
  string tok = lex->GetTokenName(token), lexeme = lex->GetLexeme();
  p2file << "Entering More_Tokens Function; current token is: " << tok << ", lexeme: " << lexeme << endl;
  int errors = 0;
  if (token >= 1 && token <= 31 || token == 33)
    { // apply rule 14
      p2file << "Using Rule 14\n";
      errors += any_other_token();
      errors += more_tokens();
    }
  else if (token == 32)
    { // apply rule 15
      p2file << "Using Rule 15\n";
    }
  else
    {
      lex->ReportError ("any other token expected, '" + lex->GetTokenName(token) + "' found.");
      errors++;
    }
  tok = lex->GetTokenName(token);
  p2file << "Exiting More_Tokens Function; current token is: " << tok << endl;
  return errors;
}

int SyntacticalAnalyzer::param_list()
{
  string tok = lex->GetTokenName(token), lexeme = lex->GetLexeme();
  p2file << "Entering Param_List Function; current token is: " << tok << ", lexeme: " << lexeme << endl;
  int errors = 0;
  if (token == 1)
    { // apply rule 16
      p2file << "Using Rule 16\n";
      token = lex->GetToken();
      errors += param_list();
    }
  else if (token == 32)
    { // apply rule 17
      p2file << "Using Rule 17\n";
    }
  else
    {
      lex->ReportError ("right parenthesis expected, '" + lex->GetTokenName(token) + "' found.");
      errors++;
    }
  tok = lex->GetTokenName(token);
  p2file << "Exiting Param_List Function; current token is: " << tok << endl;
  return errors;
}

int SyntacticalAnalyzer::else_part()
{
  string tok = lex->GetTokenName(token), lexeme = lex->GetLexeme();
  p2file << "Entering Else_Part Function; current token is: " << tok << ", lexeme: " << lexeme << endl;
  int errors = 0;
  if (token == 2 || token == 3 || token == 33 || token == 1 || token == 31)
    { // apply rule 18
      p2file << "Using Rule 18\n";
      errors += stmt();
    }
  else if (token == 32)
    { // apply rule 19
      p2file << "Using Rule 19\n";
    }
  else
    {
      lex->ReportError ("right parenthesis expected, '" + lex->GetTokenName(token) + "' found.");
      errors++;
    }
  tok = lex->GetTokenName(token);
  p2file << "Exiting Else_Part Function; current token is: " << tok << endl;
  return errors;
}

int SyntacticalAnalyzer::stmt_pair()
{
  string tok = lex->GetTokenName(token), lexeme = lex->GetLexeme();
  p2file << "Entering Stmt_Pair Function; current token is: " << tok << ", lexeme: " << lexeme << endl;
  int errors = 0;
  if (token == 31)
    { // apply rule 20
      p2file<< "Using Rule 20\n";
      token = lex->GetToken();
      errors += stmt_pair_body();
    }
  else if (token == 32)
    { // apply rule 21
      p2file << "Using Rule 21\n";
    }
  else
    {
      lex->ReportError ("right parenthesis expected, '" + lex->GetTokenName(token) + "' found.");
      errors++;
    }
  tok = lex->GetTokenName(token);
  p2file << "Exiting Stmt_Pair Function; current token is: " << tok << endl;
  return errors;
}

int SyntacticalAnalyzer::stmt_pair_body()
{
  string tok = lex->GetTokenName(token), lexeme = lex->GetLexeme();
  p2file << "Entering Stmt_Pair_Body Function; current token is: " << tok << ", lexeme: " << lexeme << endl;
  int errors = 0;
  if (token == 2 || token == 3 || token == 33 || token == 1 || token == 31)
    { // apply rule 22
      p2file << "Using Rule 22\n";
      errors += stmt();
      if (token == 2 || token == 3 || token == 33 || token == 1 || token == 31)
	{
	  errors += stmt();
	}
      else
	{
	  lex->ReportError ("NUMLIT_T, STRLIT_T, SQUOTE_T, IDENT_T, or LPAREN_T expected, '" + lex->GetTokenName(token) + "' found.");
	  errors++;
	}
    }
  else if (token == 21)
    { // apply rule 23
      p2file << "Using Rule 23\n";
      token == lex->GetToken();
      errors += stmt();
      if (token == 32)
	{
	  token = lex->GetToken();
	}
    }
  else
    {
      lex->ReportError ("NUMLIT_T, STRLIT_T, SQUOTE_T, IDENT_T, LPAREN_T or ELSE_T expected, '" + lex->GetTokenName(token) + "' found.");
      errors++;
    }
  tok = lex->GetTokenName(token);
  p2file << "Exiting Stmt_Pair_Body Function; current token is: " << tok << endl;
  return errors;
}

int SyntacticalAnalyzer::action()
{
  string tok = lex->GetTokenName(token), lexeme = lex->GetLexeme();
  p2file << "Entering Action Function; current token is: " << tok << ", lexeme: " << lexeme << endl;
  int errors = 0;
  switch(token)
    {

    case 5:
      // apply rule 24
      p2file << "Using Rule 24\n";
      token = lex->GetToken();
      errors += stmt();
      errors += stmt();
      errors += else_part();
      break;

    case 6:
      // apply rule 25
      p2file << "Using Rule 25\n";
      token = lex->GetToken();
      if (token == 31)
	{
	  token = lex->GetToken();
	  errors += stmt_pair_body();
	}
      else
	{
	  lex->ReportError ("right parenthesis expected, '" + lex->GetTokenName(token) + "' found.");
	  errors++;
	}
      break;

    // apply rules 26, 30-35, 41, 48
    case 9:
      p2file << "Using Rule 26\n";
      token = lex->GetToken();
      errors += stmt();
      break;
    case 12:
      p2file << "Using Rule 30\n";
      token = lex->GetToken();
      errors += stmt();
      break;
    case 14:
      p2file << "Using Rule 31\n";
      token = lex->GetToken();
      errors += stmt();
      break;
    case 15:
      p2file << "Using Rule 32\n";
      token = lex->GetToken();
      errors += stmt();
      break;
    case 16:
      p2file << "Using Rule 33\n";
      token = lex->GetToken();
      errors += stmt();
      break;
    case 17:
      p2file << "Using Rule 34\n";
      token = lex->GetToken();
      errors += stmt();
      break;
    case 18:
      p2file << "Using Rule 35\n";
      token = lex->GetToken();
      errors += stmt();
      break;
    case 20:
      p2file << "Using Rule 41\n";
      token = lex->GetToken();
      errors += stmt();
      break;
    case 7:
      p2file << "Using Rule 48\n";
      token = lex->GetToken();
      errors += stmt();
      break;

    // apply rules 27, 40
    case 4:
      p2file << "Using Rule 27\n";
      token = lex->GetToken();
      errors += stmt();
      errors += stmt();
      break;
    case 19:
      p2file << "Using Rule 40\n";
      token = lex->GetToken();
      errors += stmt();
      errors += stmt();
      break;

    // apply rules 28, 29, 36, 39, 42-47
    case 10:
      p2file << "Using Rule 28\n";
      token = lex->GetToken();
      errors += stmt_list();
      break;
    case 11:
      p2file << "Using Rule 29\n";
      token = lex->GetToken();
      errors += stmt_list();
      break;
    case 22:
      p2file << "Using Rule 36\n";
      token = lex->GetToken();
      errors += stmt_list();
      break;
    case 25:
      p2file << "Using Rule 39\n";
      token = lex->GetToken();
      errors += stmt_list();
      break;
    case 26:
      p2file << "Using Rule 42\n";
      token = lex->GetToken();
      errors += stmt_list();
      break;
    case 27:
      p2file << "Using Rule 43\n";
      token = lex->GetToken();
      errors += stmt_list();
      break;
    case 28:
      p2file << "Using Rule 44\n";
      token = lex->GetToken();
      errors += stmt_list();
      break;
    case 29:
      p2file << "Using Rule 45\n";
      token = lex->GetToken();
      errors += stmt_list();
      break;
    case 30:
      p2file << "Using Rule 46\n";
      token = lex->GetToken();
      errors += stmt_list();
      break;
    case 1:
      p2file << "Using Rule 47\n";
      token = lex->GetToken();
      errors += stmt_list();
      break;

    // apply rules 37, 38
    case 23:
      p2file << "Using Rule 37\n";
      token = lex->GetToken();
      errors += stmt();
      errors += stmt_list();
      break;
    case 24:
      p2file << "Using Rule 38\n";
      token = lex->GetToken();
      errors += stmt();
      errors += stmt_list();
      break;

    case 8:
      // apply rule 49
      p2file << "Using Rule 49\n";
      token = lex->GetToken();
      break;

    default:
      lex->ReportError ("action token expected, '" + lex->GetTokenName(token) + "' found.");
      errors++;
    }
  tok = lex->GetTokenName(token);
  p2file << "Exiting Action Function; current token is: " << tok << endl;
  return errors;
}

int SyntacticalAnalyzer::any_other_token()
{
  string tok = lex->GetTokenName(token), lexeme = lex->GetLexeme();
  p2file << "Entering Any_Other_Token Function; current token is: " << tok << ", lexeme: " << lexeme << endl;
  int errors = 0;
  if (token == 31)
    { // apply rule 50
      p2file << "Using Rule 50\n";
      token = lex->GetToken();
      errors += more_tokens();
      if (token == 32)
	{
	  token = lex->GetToken();
	}
      else
	{
	  lex->ReportError ("right parenthesis expected, '" + lex->GetTokenName(token) + "' found.");
	  errors++;
	}
    }
  else if (token >= 1 && token <= 30)
    { // apply rules 51-78, 80, 81
      if(token >= 1 && token <= 5)
	p2file << "Using Rule " << token + 50 << endl;
      else if(token == 6)
	p2file << "Using Rule 80\n";
      else if(token >= 7 && token <= 18)
	p2file << "Using Rule " << token + 49 << endl;
      else if(token == 19 || token == 20)
	p2file << "Using Rule " << token + 53 << endl;
      else if(token == 21)
	p2file << "Using Rule 81\n";
      else if(token >= 22 && token <= 25)
	p2file << "Using Rule " << token + 46 << endl;
      else if(token >= 26 && token <= 30)
	p2file << "Using Rule " << token + 48 << endl;
      token = lex->GetToken();
    }
  else if (token == 33)
    { // apply rule 79
      p2file << "Using Rule 79\n";
      token = lex->GetToken();
      errors += any_other_token();
    }
  else
    {
      lex->ReportError ("any other token expected, '" + lex->GetTokenName(token) + "' found.");
      errors++;
    }
  tok = lex->GetTokenName(token);
  p2file << "Exiting Any_Other_Token Function; current token is: " << tok << endl;
  return errors;
}
