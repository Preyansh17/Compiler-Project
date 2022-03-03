#include <stdio.h>
#include <stdlib.h>

#include "lexerDef.h"

//Function Declarations

tokenInfo getNextToken(FILE** file_ptr);
int checkForRefillBuffer(FILE** fp);
FILE* getStream(FILE* file_ptr);
int add(char * lex, char * lexeme, Token * token, Tokentype tk_type);
int between2to7(char c);
int between0to9(char c);
int betweenBtoD(char c);
int betweenAtoZ(char c);
int betweenCapitalAtoZ(char c);

void addToken(Token* token, Tokentype type, char* lexeme, int valueType, Value* value);
void initializeLexer();
void removeComments(char* test_case_file, char* cleaned_test_file);
void unSetOverFlag();