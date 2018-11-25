#include <iomanip>
#include <cstdlib>
#include <string>
#include "LexicalAnalyzer.h"

using namespace std;

static string token_names[] = { "IDENT_T","NUMLIT_T","STRLIT_T","LISTOP_T","CONS_T","IF_T", "COND_T","ELSE_T","DISPLAY_T","NEWLINE_T","AND_T","OR_T", "NOT_T","DEFINE_T","NUMBERP_T","LISTP_T","ZEROP_T","NULLP_T", "STRINGP_T","PLUS_T","MINUS_T","DIV_T","MULT_T","MODULO_T", "ROUND_T","EQUALTO_T","GT_T","LT_T","GTE_T","LTE_T","LPAREN_T", "RPAREN_T","SQUOTE_T","ERROR_T","EOF_T" };

bool testing = false;

// notice the initializer list style used for input...This is how we
// deal with initializing a private ifstream variable.
LexicalAnalyzer::LexicalAnalyzer (char * filename): input(filename)
{
  // check if input file exists
  if (input.fail())
    {
      cerr << "File: " << filename << " not found.\n";
      exit (2);
    }

  pos = 0; // set pos to 0
  lineNum = 0; // set lineNum to 0
  newLine = false;
  stringLitInProgress = false;

}

LexicalAnalyzer::~LexicalAnalyzer ()
{
	// This function will complete the execution of the lexical analyzer class
  listingFile.close();
  debugFile.close();
  tokenFile.close();
  //inputFile.close();

}

token_type LexicalAnalyzer::GetToken ()
{
	// This function will find the next lexeme in the input file and return
	// the token_type value associated with that lexeme

  newLine = false;

  /* So, this if condition is to determine whether we do or don't need to grab the
     next line and load it into our line variable.
     First, if the line is empty (which only happens with the very first call to
     GetToken() ) then get the first line and load into line variable.
     After that, the only other times we'll be loading up another line again is when
     pos becomes >= the length of the line (in other words, when we've incremented
     to the end/read all the tokens in our line). */
  if (line.empty() || pos >= line.length())
    {
      getline (input, line);
      newLine = true;	    
      if (input.eof()) // check if EOF, return if so
	return EOF_T;
      pos = 0; // everytime we get a new line, we need to reset our pos variable to 0
      lineNum++; // also, increment our lineNum variable (which starts at 0 initially)
      cout << lineNum << ": " << line << endl; // output the line number and contents

      //int msg;
      //ReportError (getLine, msg);
      listingFile << errors << " errors found in input file" << endl;
      return token;

    }

  /* Here is where we start parsing the line.  parseInput() will set the value of
     lexeme, and return the state it ended in */
  int currentState;
  currentState = parseInput();
  // if the lexeme isn't empty, and, if the line isn't blank, set the token type value
  if (lexeme != "" && lexeme != " " && !line.empty())
    {
      SetToken(currentState);
      cout << '\t' << GetTokenName(token) << endl;
    }

  // for a STRLIT that hasn't found a closing double-quote, we'll hit this point where
  // stringLitInProgress will be true and return an ERROR token
  if (stringLitInProgress)
    {
      stringLitInProgress = false;
      currentState = ER;
      SetToken(currentState);
      return ER;
    }
  
  return token;
}

string LexicalAnalyzer::GetTokenName (token_type t) const
{
	// The GetTokenName function returns a string containing the name of the
	// token passed to it.

  if (t == IDENT || t == -IDENT)
    return token_names[0];
  else if (t == NUMLIT || t == -NUMLIT)
    return token_names[1];
  else if (t == STRLIT || t == -STRLIT)
    return token_names[2];
  else if (t == LISTOP || t == -LISTOP)
    return token_names[3];
  else if (t == CONS || t == -CONS)
    return token_names[4];
  else if (t == IF || t == -IF)
    return token_names[5];
  else if (t == COND || t == -COND)
    return token_names[6];
  else if (t == ELSE || t == -ELSE)
    return token_names[7];
  else if (t == DISPLAY || t == -DISPLAY)
    return token_names[8];
  else if (t == NEWLINE || t == -NEWLINE)
    return token_names[9];
  else if (t == AND || t == -AND)
    return token_names[10];
  else if (t == OR || t == -OR)
    return token_names[11];
  else if (t == NOT || t == -NOT)
    return token_names[12];
  else if (t == DEFINE || t == -DEFINE)
    return token_names[13];
  else if (t == NUMBERP || t == -NUMBERP)
    return token_names[14];
  else if (t == LISTP || t == -LISTP)
    return token_names[15];
  else if (t == ZEROP || t == -ZEROP)
    return token_names[16];
  else if (t == NULLP || t == -NULLP)
    return token_names[17];
  else if (t == STRINGP || t == -STRINGP)
    return token_names[18];
  else if (t == PLUS || t == -PLUS)
    return token_names[19];
  else if (t == MINUS || t == -MINUS)
    return token_names[20];
  else if (t == DIV || t == -DIV)
    return token_names[21];
  else if (t == MULT || t == -MULT)
    return token_names[22];
  else if (t == MODULO || t == -MODULO)
    return token_names[23];
  else if (t == ROUND || t == -ROUND)
    return token_names[24];
  else if (t == EQUALTO || t == -EQUALTO)
    return token_names[25];
  else if (t == GT || t == -GT)
    return token_names[26];
  else if (t == LT || t == -LT)
    return token_names[27];
  else if (t == GTE || t == -GTE)
    return token_names[28];
  else if (t == LTE || t == -LTE)
    return token_names[29];
  else if (t == LPAREN || t == -LPAREN)
    return token_names[30];
  else if (t == RPAREN || t == -RPAREN)
    return token_names[31];
  else if (t == SQUOTE || t == -SQUOTE)
    return token_names[32];
  else if (t == ER || t == -ER)
    return token_names[33];
  else if (t == EOF || t == -EOF)
    return token_names[34];

}

void LexicalAnalyzer::SetToken(int state)
{
  int LexLength = lexeme.length();
  if (state == IDENT || state == -IDENT || state == LISTOP)
    {
      if(lexeme[LexLength -1] == '?')
	FindPredicates();
      else
	FindKeywords(state);
    }
  else
    FindOtherTypes(state);
}

void LexicalAnalyzer::FindPredicates ()
{
  /*This function handles the 5 predicates*/ 
  
  if ("number?" == lexeme)
    token = NUMBERP;
  else if ("list?" == lexeme)
    token = LISTP;
  else if ("zero?" == lexeme)
    token = ZEROP;
  else if ("null?" == lexeme)
    token = NULLP;
  else if ("string?" == lexeme)
    token = STRINGP;
  else
    token = ER;
  
}

void LexicalAnalyzer::FindKeywords(int state)
{
  /* everything coming into this function has been identified as an IDENT,
     if, it doesn't match any of the keywords, then it remains an IDENT */
  if (state == LISTOP)
    token = LISTOP;
  else if(lexeme == "cons")
      token = CONS;
  else if(lexeme == "if")
      token = IF;
  else if(lexeme == "cond")
      token  = COND;
  else if(lexeme == "else")
      token = ELSE;
  else if(lexeme == "display")
      token = DISPLAY;
  else if(lexeme == "newline")
      token = NEWLINE;
  else if(lexeme == "and")
      token  = AND;
  else if (lexeme == "or")
      token = OR;
  else if(lexeme == "not")
      token = NOT;
  else if(lexeme == "define")
      token = DEFINE;
  else if (lexeme == "modulo")
      token = MODULO; 
  else if(lexeme == "round")
      token = ROUND;
  else 
    token = IDENT;
}

void LexicalAnalyzer::FindOtherTypes(int state)
{
  /* everything coming into this function has been identified as something
     OTHER than an IDENT, and so, of the remaining token types it could be
     we evaluate what it is below */
  if (state == NUMLIT || state == -NUMLIT)
    token = NUMLIT;
  else if (state == STRLIT || state == -STRLIT)
    token = STRLIT;
  else if (state == ER || state == -ER || lexeme == "$" || lexeme == "?")
    token = ER;
  else if (state == PLUS || state == -PLUS)
    token = PLUS;
  else if (state == MINUS || state == -MINUS)
    token = MINUS;
  else if (state == DIV || state == -DIV)
    token = DIV;
  else if (state == MULT || state == -MULT)
    token = MULT;
  else if (state == EQUALTO || state == -EQUALTO)
    token = EQUALTO;
  else if (state == GT || state == -GT)
    token = GT;
  else if (state == LT || state == -LT)
    token = LT;
  else if (state == GTE || state == -GTE)
    token = GTE;
  else if (state == LTE || state == -LTE)
    token = LTE;
  else if (state == LPAREN || state == -LPAREN)
    token = LPAREN;
  else if (state == RPAREN || state == -RPAREN)
    token = RPAREN;
  else if (state == SQUOTE || state == -SQUOTE)
    token = SQUOTE;
}

string LexicalAnalyzer::GetLexeme () const
{
  // This function will return the lexeme found by the most recent call to 
  // the get_token function
  return lexeme;
}

void LexicalAnalyzer::ReportError (const string & msg)
{
  // This function will be called to write an error message to a file
  
  //Takes a string

  //if (token == lexeme)

  // string code = getLine();

  //If there's an error while the line is being parsed then print out error statement

  if (token == ER | token == -ER)
    //if (msg != "")
  cout << msg << endl;
    //else if
  listingFile << "Error at " << lineNum << "," << pos << ": Invalid character found: "
	     << lexeme << endl;
  debugFile << "Error at " << lineNum << "," << pos << ": Invalid character found: "
	     << lexeme << endl;
    //errors need to be kept track of to print to getToken()
  errors++;
}

string LexicalAnalyzer::getLine ()
{
  return line;
}

int LexicalAnalyzer::getLineNum()
{
  return lineNum;
}

bool LexicalAnalyzer::readNewLine()
{
  return newLine;
}

int LexicalAnalyzer::parseInput ()
{
  string temp;
  string code = getLine();
   while (pos < code.length())
    {
      if (testing)
	cout << ">> TOP OF WHILE: pos = " << pos << " code.length() = " << code.length() << endl;

      int state = 0;
      // read in characters one at a time, add to temp, evaluate new state
      while (state > -1 && state < 100)
        {
	  if (testing)
	    cout << ">> Current position: " << pos << endl;
          temp += code[pos];
          if (testing)
            cout << ">> CURRENT STATE: " << state << ", looking at " << code[pos] << endl;
          state = nextState(state, code[pos]);
          if (testing)
            cout << ">> TRANSITIONED TO STATE: " << state << endl;
          pos++;
        }
      if (testing)
        cout << ">> END STATE: " << state << endl;
      // if we need to back up...
      if (state < -1 || state == BU)
        {
	  if (testing)
	    cout << ">> BACKING UP AND ERASING LAST CHAR: " << endl;
          temp.erase(temp.length()-1, 1); // erase the last character
          pos--; // decrement position
        }
      // check that temp isn't empty or just whitespace, then print
      if (temp != "" && temp != " ")
	{
	  cout << '\t' << temp << '\t';//GetTokenName(token) << endl;
	  lexeme = temp; // set the vale of lexeme
	  return state; /// return state value
	}
      else // if the string turns out to be blank, full of whitespace, etc
	temp = ""; // reset the string value
    }
}

int LexicalAnalyzer::nextState (int currentState, char currentChar)
{
  int charColumn;
  // set col to appropriate value based on transition matrix
  if (isalpha(currentChar) && currentChar != 'c' && currentChar != 'a' && currentChar != 'd' && currentChar != 'r')
    charColumn = 0;
  
  else if (isdigit(currentChar))
    charColumn = 6;

  else
    switch (currentChar)
      {
      case 'c':
	charColumn = 1;
	break;
	
      case 'a':
	charColumn = 2;
	break;
	
      case 'd':
	charColumn = 3;
	break;
	
      case 'r':
	charColumn = 4;
	break;

      case '_':
	charColumn = 5;
	break;
	
      case '.':
	charColumn = 7;
	break;
	
      case '+':
	charColumn = 8;
	break;
	
      case '-':
	charColumn = 9;
	break;
	
      case '/':
	charColumn = 10;
	break;

      case '*':
	charColumn = 11;
	break;
	
      case '>':
	charColumn = 12;
	break;
      
      case '=':
	charColumn = 13;
	break;
	
      case '<':
	charColumn = 14;
	break;
      
      case '(':
	charColumn = 15;
	break;
	
      case ')':
	charColumn = 16;
	break;
	
      case '\'':
	charColumn = 17;
	break;
	
      case '"':
	charColumn = 18;
	stringLitInProgress = true;
	break;
	
      case ' ':
	charColumn = 19;
	break;
      
      case '?':
	charColumn = 20;
	break;
	/* removed for now, didn't appear to be useful
      case '\n':
	charColumn = 21;
	break;
	*/
      case '\0': // the null terminator however continues to show up from time to time
	charColumn = 21;
	break;
	
      default:
	return ER;
      }

int states[12][22] = {
/*      alpha     c       a       d       r       _      #   .      +       -       /       *       >       =        <       (       )       '       "       ws      ?      \0      */
/*       0        1       2       3       4       5      6   7      8       9       10      11      12      13       14      15      16      17      18      19      20     21      */
/*      ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
  /*0*/ {1,       2,      1,      1,      1,      ER,    5,  11,     4,      7,      DIV,    MULT,   8,      EQUALTO, 9,      LPAREN, RPAREN, SQUOTE, 10,     GD,     ER,    -IDENT},
  /*1*/ {1,       1,      1,      1,      1,      1,     1, -IDENT,-IDENT, -IDENT, -IDENT, -IDENT, -IDENT, -IDENT,  -IDENT, -IDENT, -IDENT, -IDENT, -IDENT, -IDENT,  IDENT, -IDENT},
  /*2*/ {1,       1,      3,      3,      1,      1,     1, -IDENT,-IDENT, -IDENT, -IDENT, -IDENT, -IDENT, -IDENT,  -IDENT, -IDENT, -IDENT, -IDENT, -IDENT, -IDENT,  IDENT, -IDENT},
  /*3*/ {1,       1,      1,      3,      LISTOP, 1,     1, -IDENT,-IDENT, -IDENT, -IDENT, -IDENT, -IDENT, -IDENT,  -IDENT, -IDENT, -IDENT, -IDENT, -IDENT, -IDENT,  IDENT, -IDENT},
  /*4*/ {-PLUS,  -PLUS,  -PLUS,  -PLUS,  -PLUS,  -PLUS,  5,  6,    -PLUS,  -PLUS,  -PLUS,  -PLUS,  -PLUS,  -PLUS,   -PLUS,  -PLUS,  -PLUS,  -PLUS,  -PLUS,  -PLUS,  -PLUS,  -PLUS},
  /*5*/ {-NUMLIT,-NUMLIT,-NUMLIT,-NUMLIT,-NUMLIT,-NUMLIT,5,  11,   -NUMLIT,-NUMLIT,-NUMLIT,-NUMLIT,-NUMLIT,-NUMLIT, -NUMLIT,-NUMLIT,-NUMLIT,-NUMLIT,-NUMLIT,-NUMLIT,-NUMLIT,-NUMLIT},
  /*6*/ {-ER,    -ER,    -ER,    -ER,    -ER,    -ER,    11, -ER,  -ER,    -ER,    -ER,    -ER,    -ER,    -ER,     -ER,    -ER,    -ER,    -ER,    -ER,    -NUMLIT,    -ER,    -ER},
  /*7*/ {-MINUS, -MINUS, -MINUS, -MINUS, -MINUS, -MINUS, 5,  6,    -MINUS, -MINUS, -MINUS, -MINUS, -MINUS, -MINUS,   MINUS, -MINUS, -MINUS, -MINUS, -MINUS, -MINUS, -MINUS, -MINUS},
  /*8*/ {-GT,    -GT,    -GT,    -GT,    -GT,    -GT,   -GT,-GT,   -GT,    -GT,    -GT,    -GT,    -GT,     GTE,    -GT,    -GT,    -GT,    -GT,    -GT,    -GT,    -GT,    -GT},
  /*9*/ {-LT,    -LT,    -LT,    -LT,    -LT,    -LT,   -LT,-LT,   -LT,    -LT,    -LT,    -LT,    -LT,     LTE,    -LT,    -LT,    -LT,    -LT,    -LT,    -LT,    -GT,    -GT},
  /*10*/{10,      10,     10,     10,     10,     10,    10, 10,    10,     10,     10,     10,     10,     10,      10,     10,     10,     10,     STRLIT, 10,     10,    ER},
  /*11*/{-NUMLIT,-NUMLIT,-NUMLIT,-NUMLIT,-NUMLIT,-NUMLIT,11,-NUMLIT,-NUMLIT,-NUMLIT,-NUMLIT,-NUMLIT,-NUMLIT,-NUMLIT,-NUMLIT,-NUMLIT,-NUMLIT,-NUMLIT,-NUMLIT,-NUMLIT,-NUMLIT,-NUMLIT}};
 
  // return the transition state value
  return states[currentState][charColumn];
}
