#include <iostream>
#include <iomanip>
#include <fstream>
#include "SyntacticalAnalyzer.h"

using namespace std;

SyntacticalAnalyzer::SyntacticalAnalyzer (char * filename)
{
  lex = new LexicalAnalyzer (filename);
  token_type t;
  int errors = program();
  cout << "This file has " << errors << " errors.\n";
}

SyntacticalAnalyzer::~SyntacticalAnalyzer ()
{
  delete lex;
}

int SyntacticalAnalyzer::program()
{
  int errors = 0;
  token = lex->GetToken();
  // apply rule 1
  if (token == 31)
    {
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
  return errors;
}

int SyntacticalAnalyzer::more_defines()
{
  int errors = 0;
  if (token == 13)
    { // apply rule 2
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

  else if (token == 35)
    { // apply rule 1
    }
  else
    {
      lex->ReportError ("IDENT_T or DEFINE_T expected, '" + lex->GetTokenName(token) + "' found.");
      errors++;
    }
  return errors;
}

int SyntacticalAnalyzer::define()
{
  int errors = 0;
  if (token == 13)
    { // apply rule 4 
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
  return errors;
}

int SyntacticalAnalyzer::stmt_list()
{
  int errors = 0;
  if (token == 2 || token == 3 || token == 33 || token == 1 || token == 31)
    { // apply rule 5
      token = lex->GetToken();
      errors += stmt();
      errors += stmt_list();
    }
  else
    { // apply rule 6
    }
  return errors;
}

int SyntacticalAnalyzer::stmt()
{
  int errors = 0;
  if (token == 2 || token == 3 || token == 33)
    { // apply rule 7
      errors += literal();
    }
  else if (token == 1)
    { // apply rule 8
      token = lex->GetToken();
    }
  else if (token == 31)
    { // apply rule 9
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
  return errors;
}

int SyntacticalAnalyzer::literal()
{
  int errors = 0;
  if (token == 2)
    { // apply rule 10
      token = lex->GetToken();
    }
  else if (token == 3)
    { // apply rule 11
      token = lex->GetToken();
    }
  else if (token == 33)
    { // apply rule 12
      token = lex ->GetToken();
      errors += quoted_lit();
    }
  else
    {
      lex->ReportError ("NUMLIT_T, STRLIT_T or SQUOTE_T expected, '" + lex->GetTokenName(token) + "' found.");
      errors++;
    }
  return errors;;
}

int SyntacticalAnalyzer::quoted_lit()
{
  int errors = 0;
  if (token >= 1 && token <= 31 || token == 33)
    { // apply rule 13
      errors += any_other_token();
    }
  else
    {
      lex->ReportError ("any other token expected, '" + lex->GetTokenName(token) + "' found.");
      errors++;
    }
  return errors;
}

int SyntacticalAnalyzer::more_tokens()
{
  int errors = 0;
  if (token >= 1 && token <= 31 || token == 33)
    { // apply rule 14
      errors += any_other_token();
      errors += more_tokens();
    }
  else if (token == 32)
    { // apply rule 15
    }
  else
    {
      lex->ReportError ("any other token expected, '" + lex->GetTokenName(token) + "' found.");
      errors++;
    }
  return errors;
}

int SyntacticalAnalyzer::param_list()
{
  int errors = 0;
  if (token == 1)
    { // apply rule 16
      token = lex->GetToken();
      errors += param_list();
    }
  else if (token == 32)
    { // apply rule 17
    }
  else
    {
      lex->ReportError ("right parenthesis expected, '" + lex->GetTokenName(token) + "' found.");
      errors++;
    }
  return errors;
}

int SyntacticalAnalyzer::else_part()
{
  int errors = 0;
  if (token == 2 || token == 3 || token == 33 || token == 1 || token == 31)
    { // apply rule 18
      errors += stmt();
    }
  else if (token == 32)
    { // apply rule 19
    }
  else
    {
      lex->ReportError ("right parenthesis expected, '" + lex->GetTokenName(token) + "' found.");
      errors++;
    }
  return errors;
}

int SyntacticalAnalyzer::stmt_pair()
{
  int errors = 0;
  if (token == 31)
    { // apply rule 20
      token = lex->GetToken();
      errors += stmt_pair_body();
    }
  else if (token == 32)
    { // apply rule 21
    }
  else
    {
      lex->ReportError ("right parenthesis expected, '" + lex->GetTokenName(token) + "' found.");
      errors++;
    }
  return errors;
}

int SyntacticalAnalyzer::stmt_pair_body()
{
  int errors = 0;
  if (token == 2 || token == 3 || token == 33 || token == 1 || token == 31)
    { // apply rule 22
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
  return errors;
}

int SyntacticalAnalyzer::action()
{
  int errors = 0;
  switch(token)
    {

    case 5:
      // apply rule 24
      token = lex->GetToken();
      errors += stmt();
      errors += stmt();
      errors += else_part();
      break;

    case 6:
      // apply rule 25
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
      
    case 9:
    case 12:
    case 14:
    case 15:
    case 16:
    case 17:
    case 18:
    case 20:
    case 7:
      // apply rules 26, 30-35, 41, 48
      token = lex->GetToken();
      errors += stmt();
      break;

    case 4:
    case 19:
      // apply rules 27, 40
      token = lex->GetToken();
      errors += stmt();
      errors += stmt();
      break;

    case 10:
    case 11:
    case 22:
    case 25:
    case 26:
    case 27:
    case 28:
    case 29:
    case 30:
    case 1:
      // apply rules 28, 29, 36, 39, 42-47
      token = lex->GetToken();
      errors += stmt_list();
      break;

    case 23:
    case 24:
      // apply rules 37, 38
      token = lex->GetToken();
      errors += stmt();
      errors += stmt_list();
      break;

    case 8:
      // apply rule 49
      token = lex->GetToken();
      break;

    default:
      lex->ReportError ("action token expected, '" + lex->GetTokenName(token) + "' found.");
      errors++;
    }
  return errors;
}

int SyntacticalAnalyzer::any_other_token()
{
  int errors = 0;
  if (token == 31)
    { // apply rule 50
      token = lex->GetToken();
      errors += more_tokens();
      if (token == 32)
	{
	  // do nothing
	}
      else
	{
	  lex->ReportError ("right parenthesis expected, '" + lex->GetTokenName(token) + "' found.");
	  errors++;
	}
    }
  else if (token >= 1 && token <= 30)
    { // apply rules 51-78, 80, 81
      token = lex->GetToken();
    }
  else if (token == 33)
    { // apply rule 79
      token = lex->GetToken();
      errors += any_other_token();
    }
  else
    {
      lex->ReportError ("any other token expected, '" + lex->GetTokenName(token) + "' found.");
      errors++;
    }
  return errors;
}
