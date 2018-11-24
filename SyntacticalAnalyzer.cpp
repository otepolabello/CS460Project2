#include <iostream>
#include <iomanip>
#include <fstream>
#include "SyntacticalAnalyzer.h"

using namespace std;

SyntacticalAnalyzer::SyntacticalAnalyzer (char * filename)
{
  lex = new LexicalAnalyzer (filename);
  token = lex->GetToken();
  program ();
}

SyntacticalAnalyzer::~SyntacticalAnalyzer ()
{
  delete lex;
}

int Program(){
  int errors = 0;
  if(token == LPAREN){
    errors += Defines();
  }
  else{
    errors++;
    errors += Define();
  }
  return errors;
}

int MoreDefines(){
  int errors = 0;
  token = lex->GetToken();
  if(token == IDENT){
    errors += StmtList();
  }
  else if(token == DEFINE){
    errors += Define();
  }
  else{
    errors++;
  }
  return errors;
}

int Define(){
  int errors = 0;
  token = lex->GetToken();
  if(token == LPAREN){
    token = lex->GetToken();
  }
  else{
    errors++;
  }
  if(token == IDENT){
    errors += ParamList();
  }
  else{
    errors++;
  }
  token = lex->GetToken();
  if(token == RPAREN){
    errors += Stmt();
    errors += StmtList();
  }
  else{
    errors++;
  }
  if(token == RPAREN){
  }
  else{
    errors++;
  }
  return errors;
}

int StmtList(){
  int errors = 0;
  token = lex->GetToken();
  if(token == RPAREN){
    return errors;
  }
  else{
    errors++;
  }
  errors += Stmt();
  errors += StmtList();
  return errors;
}

int Stmt(){
  int errors = 0;
  if(token == IDENT){
    token = lex->GetToken();
    return errors;
  }
  else if(token == LPAREN){
    errors += Action();
  }
  else if(token == NUMLIT || token == STRLIT || token == SQUOTE){
    errors += Literal();
  }
  else{
    errors++;
  }
  return errors;
}

int Literal(){
  int errors = 0;
  if(token == STRLIT || token == NUMLIT){
    token = lex->GetToken();
    return errors;
  }
  if(token == SQUOTE){
    errors += QuotedLit();
  }
  else{
    errors++;
  }
  return errors;
}

int MoreTokens(){
  int errors = 0;
  if(token == RPAREN || token == IDENT || token == NUMLIT || token == STRLIT ||
     token == CONS || token == IF || token == DISPLAY || token == NEWLINE ||
     token == LISTOP || token == AND || token == OR || token == NOT || token == DEFINE ||
     token == NUMBERP || token == LISTP || token == ZEROP || token == NULLP ||
     token == STRINGP || token == PLUS || token == MINUS || token == DIV ||
     token == MULT || token == MODULO || token == ROUND || token == EQUALTO ||
     token == GT || token == LT || token == GTE || token == LTE || token == SQUOTE ||
     token == COND || token == ELSE){
    errors += AnyOtherTokens();
  }
  else if(token == RPAREN){
    token = lex->GetToken();
    return errors;
  }
  else{
    errors++;
  }
  return errors;
}

int ParamList(){
  int errors = 0;
  token = lex->GetToken();
  if(token == IDENT){
    errors += ParamList();
  }
  else if(token == RPAREN){
    token = lex->GetToken();
    return errors;
  }
  else{
    errors++;
  }
  return errors;
}

int ElsePart(){
  int errors = 0;
  if(token == IDENT){
    token = lex->GetToken();
    return errors;
  }
  else if(token == LPAREN){
    errors += Action();
  }
  else if(token == NUMLIT || token == STRLIT || token == SQUOTE){
    errors += Literal();
  }
  else{
    errors++;
  }
  return errors;
}

int StmtPair(){
  int errors = 0;
  if(token == LPAREN){
    errors += StmtPairBody();
  }
  else{
    errors++;
  }
  if(token == RPAREN){
    token = lex->GetToken();
  }
  else{
    errors++;
  }
  return errors;
}

int StmtPairBody(){
  int errors = 0;
  if(token = ELSE){
    errors += Stmt();
  }
  else if(token == IDENT || token == LPAREN || token == NUMLIT || token == STRLIT ||
	  token == SQUOTE){
    token = lex->GetToken();
    errors += Stmt();
    errors += Stmt();
  }
  else{
    errors++;
  }
  return errors;
}

int Action(){
  int errors = 0;
  if(token == IF){
    errors += Stmt();
    errors += Stmt();
    errors += ElsePart();
  }
  else if(token == COND){
    token = lex->GetToken();
    errors += StmtPairBody();
  }
  else if(token == LISTOP || token == NOT || token == NUMBERP || token == LISTP ||
	  token == ZEROP || token == NULLP || token == STRINGP || token == ROUND ||
	  token == DISPLAY){
    errors += Stmt();
  }
  return errors;
}

int AnyOtherToken(){
  return 0;
}

int QuotedList(){
  return 0;
}
