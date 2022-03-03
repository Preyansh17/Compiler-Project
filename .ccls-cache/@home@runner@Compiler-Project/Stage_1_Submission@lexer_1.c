/*
Group:33
2016A7PS0036P Megh Thakkar
2016A7PS0103P Sahil Singla
2016A7PS0110P Sankalp Sangle
2016A7PS0150P Patel Parth
*/

#include "lexer.h"
#include "hashTableDef.h"
#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<fcntl.h>
#define BUFFERSIZE 100

#define TK_ID_LEXICAL_ERROR 5
#define TK_FUNID_LEXICAL_ERROR 6
#define TK_RNUM_LEXICAL_ERROR 7
// Twin buffers
char* buffer_prev;
char* buffer_curr;

int file_over_flag = 0;
int next_ptr = 0;
int line_no = 1;

int length = 0;
char* tokenMap3[] = {
"TK_ASSIGNOP",
"TK_COMMENT",
"TK_FIELDID",
"TK_ID",
"TK_NUM",
"TK_RNUM",
"TK_FUNID",
"TK_RUID",
"TK_WITH",
"TK_PARAMETERS",
"TK_END",
"TK_WHILE",
"TK_UNION",
"TK_ENDUNION",
"TK_DEFINETYPE",
"TK_AS",
"TK_TYPE",
"TK_MAIN",
"TK_GLOBAL",
"TK_PARAMETER",
"TK_LIST",
"TK_SQL",
"TK_SQR",
"TK_INPUT",
"TK_OUTPUT",
"TK_INT",
"TK_REAL",
"TK_COMMA",
"TK_SEM",
"TK_COLON",
"TK_DOT",
"TK_ENDWHILE",
"TK_OP",
"TK_CL",
"TK_IF",
"TK_THEN",
"TK_ENDIF",
"TK_READ",
"TK_WRITE",
"TK_RETURN",
"TK_PLUS",
"TK_MINUS",
"TK_MUL",
"TK_DIV",
"TK_CALL",
"TK_RECORD",
"TK_ENDRECORD",
"TK_ELSE",
"TK_AND",
"TK_OR",
"TK_NOT",
"TK_LT",
"TK_LE",
"TK_EQ",
"TK_GT",
"TK_GE",
"TK_NE",
"EPSILON",
"DOLLAR",
"TK_ERROR",
};
char* keywords[] = {NULL,
NULL,
NULL,
NULL,
NULL,
NULL,
NULL,
NULL,
"with",
"parameters",
"end",
"while",
"union",
"endunion",
"definetype",
"as",
"type",
"_main",
"global",
"parameter",
"list",
NULL,
NULL,
"input",
"output",
"int",
"real",
NULL,
NULL,
NULL,
NULL,
"endwhile",
NULL,
NULL,
"if",
"then",
"endif",
"read",
"write",
"return",
NULL,
NULL,
NULL,
NULL,
"call",
"record",
"endrecord",
"else",
NULL,
NULL,
NULL,
NULL,
NULL,
NULL,
NULL,
NULL,
NULL,
NULL,
NULL,
NULL};

void appendNewLine(FILE* fp) {
    fputs("\n", fp);
    fclose(fp);
}

void addToken(Token* token, Tokentype type, char* lexeme, int vtype, Value* value){
	token->line_no = line_no;
	token->type = type;
    token->lexeme = (char*)malloc(200*sizeof(char));
	strcpy(token->lexeme, lexeme);
    Value* val = (Value*)malloc(sizeof(Value));
    if (value != NULL) {
        val->num = value->num;
        val->r_num = value->r_num;
        free(value);
    }
    token->value = val;
    token->vtype = vtype;

}

int checkForRefillBuffer(FILE** fp) {
    if(buffer_curr[next_ptr]=='\0'|| buffer_curr[next_ptr]==-1) { 
        next_ptr = 0;
        *fp = getStream(*fp);
        if(*fp == NULL) {
            file_over_flag = 1;						
        } 
	}
    return file_over_flag;
}

FILE* getStream(FILE* fp) {
    int bytesRead;
    char* tmp = buffer_prev;
    buffer_prev = buffer_curr;
    buffer_curr = tmp;

    memset(buffer_curr, 0 , sizeof(char) * BUFFERSIZE + 1);
    bytesRead = fread(buffer_curr, sizeof(char), BUFFERSIZE, fp);
	
    if(bytesRead<=0) {
		fclose(fp);
 	    return NULL;
 	}
    
    buffer_curr[bytesRead]='\0';
 	return fp;

}

int add(char * lex, char * lexeme, Token * token, Tokentype tk_type) {
    strcpy(lex, lexeme);
    addToken(token, tk_type, lex, -1, NULL);	
    return 1;
}

Token* getNextToken(FILE** fp) {
    
    char lex[200];

    Token* token = (Token*)malloc(sizeof(Token));

	// if(feof(*fp)) {
	// 	fclose(*fp);
 	// 	return NULL;
 	// }
    if(file_over_flag==1) {

		if(*fp != NULL)
			fclose(*fp);

		return NULL;
	}

	//Loading Buffer
	if (checkForRefillBuffer(fp) == 1) {
        return NULL;
    }
    
	int final_state_flag = 0;
    int state = 1;
	
	int lexeme_ptr = 0;
    
    while(1) {
        if(checkForRefillBuffer(fp) == 1) {
                    return NULL;
        }
        switch(state) {
            case 1: 
                //switch char read
                memset(lex, 0, sizeof(char) * 50);
                lexeme_ptr = 0;
            
                if (buffer_curr[next_ptr] == '[') {
                    final_state_flag = add(lex, "[", token, TK_SQL);
                }
                else if (buffer_curr[next_ptr] == ']') {
                    final_state_flag = add(lex, "]", token, TK_SQR);
                }
                else if (buffer_curr[next_ptr] == ',') {
                    final_state_flag = add(lex, ",", token, TK_COMMA);
                }
                else if (buffer_curr[next_ptr] == ';') {
                    final_state_flag = add(lex, ";", token, TK_SEM);
                }
                else if (buffer_curr[next_ptr] == ':') {
                    final_state_flag = add(lex, ":", token, TK_COLON);
                }
                else if (buffer_curr[next_ptr] == '.') {
                    final_state_flag = add(lex, ".", token, TK_DOT);
                }
                else if (buffer_curr[next_ptr] == '(') {
                    final_state_flag = add(lex, "(", token, TK_OP);
                }
                else if (buffer_curr[next_ptr] == ')') {
                    final_state_flag = add(lex, ")", token, TK_CL);
                }
                else if (buffer_curr[next_ptr] == '+') {
                    final_state_flag = add(lex, "+", token, TK_PLUS);
                }
                else if (buffer_curr[next_ptr] == '-') {
                    final_state_flag = add(lex, "-", token, TK_MINUS);
                }
                else if (buffer_curr[next_ptr] == '*') {
                    final_state_flag = add(lex, "*", token, TK_MUL);
                }
                else if (buffer_curr[next_ptr] == '/') {
                    final_state_flag = add(lex, "/", token, TK_DIV);
                }
                else if (buffer_curr[next_ptr] == '~') {
                    final_state_flag = add(lex, "~", token, TK_NOT);
                }
                else if (buffer_curr[next_ptr] == '&') {
                    lex[lexeme_ptr++] = buffer_curr[next_ptr];
                    state = 15;
                }
                else if (buffer_curr[next_ptr] == '@') {
                    lex[lexeme_ptr++] = buffer_curr[next_ptr];
                    state = 18;
                }
                else if (buffer_curr[next_ptr] == '=') {
                    lex[lexeme_ptr++] = buffer_curr[next_ptr];
                    state = 21;
                }
                else if (buffer_curr[next_ptr] == '!') {
                    lex[lexeme_ptr++] = buffer_curr[next_ptr];
                    state = 23;
                }
                else if (buffer_curr[next_ptr] == '<') {
                    lex[lexeme_ptr++] = buffer_curr[next_ptr];
                    state = 25;
                }
                else if (buffer_curr[next_ptr] == '>') {
                    lex[lexeme_ptr++] = buffer_curr[next_ptr];
                    state = 31;
                }
                else if (buffer_curr[next_ptr] == '%') {
                    lex[lexeme_ptr++] = buffer_curr[next_ptr];
                    state = 34;
                }
                else if (buffer_curr[next_ptr] == '#') {
                    lex[lexeme_ptr++] = buffer_curr[next_ptr];
                    state = 36;
                }
                else if (buffer_curr[next_ptr] == '_') {
                    lex[lexeme_ptr++] = buffer_curr[next_ptr];
                    length++;
                    state = 39;
                }
                else if (buffer_curr[next_ptr] == ' ' || buffer_curr[next_ptr] == '\r' || buffer_curr[next_ptr] == '\t') {
                    state = 1;
                }
                else if (buffer_curr[next_ptr] == '\n') {
                    state = 1;
                    line_no++;
                }
                else if (buffer_curr[next_ptr] >= 'a' 
                    && buffer_curr[next_ptr] <= 'z'
                    && buffer_curr[next_ptr] != 'b'
                    && buffer_curr[next_ptr] != 'c'
                    && buffer_curr[next_ptr] != 'd'
                    ) {
                    state = 43;
                    lex[lexeme_ptr++] = buffer_curr[next_ptr];
                } 
                else if (buffer_curr[next_ptr] >= 'b' && buffer_curr[next_ptr] <= 'd') {
                    state = 45;
                    lex[lexeme_ptr++] = buffer_curr[next_ptr];
                    length++;
                }
                else if (buffer_curr[next_ptr] >= '0' && buffer_curr[next_ptr] <= '9') {
                    state = 49;
                    lex[lexeme_ptr++] = buffer_curr[next_ptr];
                }
                else {
                    lex[lexeme_ptr++] = buffer_curr[next_ptr];
                    lex[lexeme_ptr] = '\0';
                    fprintf(stderr, "Line No: %d : Unknown pattern %s at line\n", line_no, lex);
                    addToken(token, TK_ERROR, lex, -2, NULL);
                    final_state_flag = 1;
                }
                next_ptr++;
                break;
            case 15:
                if ( buffer_curr[next_ptr] == '&') {
                    lex[lexeme_ptr++] = buffer_curr[next_ptr++];
                    state = 16;
                }
                else {
                    lex[lexeme_ptr] = '\0';
                    fprintf(stderr, "Line No: %d : Unknown pattern %s at line\n", line_no, lex);
                    addToken(token, TK_ERROR, lex, -2, NULL);
                    final_state_flag = 1;
                }
                break;
            case 16:
                if ( buffer_curr[next_ptr] == '&') {
                    lex[lexeme_ptr++] = buffer_curr[next_ptr++];
                    state = 17;
                }
                else {
                    lex[lexeme_ptr] = '\0';
                    fprintf(stderr, "Line No: %d : Unknown pattern %s at line\n", line_no, lex);
                    addToken(token, TK_ERROR, lex, -2, NULL);
                    final_state_flag = 1;
                }
                break;
            case 17:
                lex[next_ptr] = '\0';
                addToken(token, TK_AND, lex, -1, NULL);
                final_state_flag = 1;
                break;
            case 18:
                if ( buffer_curr[next_ptr] == '@') {
                    lex[lexeme_ptr++] = buffer_curr[next_ptr++];
                    state = 19;
                }
                else {
                    lex[lexeme_ptr] = '\0';
                    fprintf(stderr, "Line No: %d : Unknown pattern %s at line\n", line_no, lex);
                    addToken(token, TK_ERROR, lex, -2, NULL);
                    final_state_flag = 1;
                }
                break;
            case 19:
                if ( buffer_curr[next_ptr] == '@') {
                    lex[lexeme_ptr++] = buffer_curr[next_ptr++];
                    state = 20;
                }
                else {
                    lex[lexeme_ptr] = '\0';
                    fprintf(stderr, "Line No: %d : Unknown pattern %s at line\n", line_no, lex);
                    addToken(token, TK_ERROR, lex, -2, NULL);
                    final_state_flag = 1;
                }
                break;
            case 20:
                lex[next_ptr] = '\0';
                addToken(token, TK_OR, lex, -1, NULL);
                final_state_flag = 1;
                break;
            case 21:
                if ( buffer_curr[next_ptr] == '=') {
                    lex[lexeme_ptr++] = buffer_curr[next_ptr++];
                    state = 22;
                }
                else {
                    lex[lexeme_ptr] = '\0';
                    fprintf(stderr, "Line No: %d : Unknown pattern %s at line\n", line_no, lex);
                    addToken(token, TK_ERROR, lex, -2, NULL);
                    final_state_flag = 1;
                }
                break;
            case 22:
                lex[lexeme_ptr] = '\0';
                addToken(token, TK_EQ, lex, -1, NULL);
                final_state_flag = 1;
                break;
            case 23:
                if ( buffer_curr[next_ptr] == '=') {
                    lex[lexeme_ptr++] = buffer_curr[next_ptr++];
                    state = 24;
                }
                else {
                    lex[lexeme_ptr] = '\0';
                    fprintf(stderr, "Line No: %d : Unknown pattern %s at line\n", line_no, lex);
                    addToken(token, TK_ERROR, lex, -2, NULL);
                    final_state_flag = 1;
                }
                break;
            case 24:
                lex[lexeme_ptr] = '\0';
                addToken(token, TK_NE, lex, -1, NULL);
                final_state_flag = 1;
                break;
            case 25:
                if ( buffer_curr[next_ptr] == '=') {
                    lex[lexeme_ptr++] = buffer_curr[next_ptr++];
                    state = 26;
                }
                else if (buffer_curr[next_ptr] == '-') {
                    lex[lexeme_ptr++] = buffer_curr[next_ptr++];
                    state = 27;
                }
                else {
                    state = 30;
                }
                break;
            case 26:
                lex[lexeme_ptr] = '\0';
                addToken(token, TK_LE, lex, -1, NULL);
                final_state_flag = 1;
                break;
            case 27:
                if(buffer_curr[next_ptr] == '-') {
                    lex[lexeme_ptr++] = buffer_curr[next_ptr++];
                    state = 28;
                }
                else {
                    lex[lexeme_ptr] = '\0';
                    fprintf(stderr, "Line No: %d : Unknown pattern %s at line\n", line_no, lex);
                    addToken(token, TK_ERROR, lex, -2, NULL);
                    final_state_flag = 1;
                }
                break;
            case 28:
                if(buffer_curr[next_ptr] == '-') {
                    lex[lexeme_ptr++] = buffer_curr[next_ptr++];
                    state = 29;
                }
                else {
                    lex[lexeme_ptr] = '\0';
                    fprintf(stderr, "Line No: %d : Unknown pattern %s at line\n", line_no, lex);
                    addToken(token, TK_ERROR, lex, -2, NULL);
                    final_state_flag = 1;
                }
                break;
            case 29:
                lex[next_ptr] = '\0';
                addToken(token, TK_ASSIGNOP, lex, -1, NULL);
                final_state_flag = 1;
                break;
            case 30:
                lex[lexeme_ptr] = '\0';
                addToken(token, TK_LT, lex, -1, NULL);
                final_state_flag = 1;
                break;
            case 31:
                if ( buffer_curr[next_ptr] == '=') {
                    lex[lexeme_ptr++] = buffer_curr[next_ptr++];
                    state = 32;
                }
                else {
                    state = 33;
                }
                break;
            case 32:
                lex[lexeme_ptr] = '\0';
                addToken(token, TK_GE, lex, -1, NULL);
                final_state_flag = 1;
                break;
            case 33:
                lex[lexeme_ptr] = '\0';
                addToken(token, TK_GT, lex, -1, NULL);
                final_state_flag = 1;
                break;
            case 34:
                if ( buffer_curr[next_ptr] != '\n') {
                    state = 34;
                }
                else {
                    state = 35;
                }
                next_ptr++;
                break;
            case 35:
                lex[lexeme_ptr] = '\0';
                addToken(token, TK_COMMENT, lex, -1, NULL);
                line_no++;
                final_state_flag = 1;
                break;
            case 36:
                if(buffer_curr[next_ptr] >= 'a' && buffer_curr[next_ptr] <= 'z') {
                    lex[lexeme_ptr++] = buffer_curr[next_ptr++];
                    state = 37;
                }
                else {
                    lex[lexeme_ptr] = '\0';
                    fprintf(stderr, "Line No: %d : Unknown pattern %s at line\n", line_no, lex);
                    addToken(token, TK_ERROR, lex, -2, NULL);
                    final_state_flag = 1;
                }
                break;
            case 37:
                if(buffer_curr[next_ptr] >= 'a' && buffer_curr[next_ptr] <= 'z') {
                    lex[lexeme_ptr++] = buffer_curr[next_ptr++];
                    state = 37;
                }
                else {
                    state = 38;
                }

                break;
            case 38:
                lex[lexeme_ptr] = '\0';
                addToken(token, TK_RUID, lex, -1, NULL);
                final_state_flag = 1;
                break;
            case 39:
                if ( (buffer_curr[next_ptr] >= 'a' && buffer_curr[next_ptr] <= 'z') || (buffer_curr[next_ptr] >= 'A' && buffer_curr[next_ptr] <= 'Z')) {
                    lex[lexeme_ptr++] = buffer_curr[next_ptr++];
                    length++;
                    state = 40;
                }
                else {
                    lex[lexeme_ptr] = '\0';
                    fprintf(stderr, "Line No: %d : Unknown pattern %s at line\n", line_no, lex);
                    addToken(token, TK_ERROR, lex, -2, NULL);
                    final_state_flag = 1;
                }
                break;
            case 40:
                if((buffer_curr[next_ptr] >= 'a' && buffer_curr[next_ptr] <= 'z') || (buffer_curr[next_ptr] >= 'A' && buffer_curr[next_ptr] <= 'Z')) {
                    lex[lexeme_ptr++] = buffer_curr[next_ptr++];
                    length++;
                    state = 40;
                }
                else if (buffer_curr[next_ptr] >= '0' && buffer_curr[next_ptr] <= '9') {
                    lex[lexeme_ptr++] = buffer_curr[next_ptr++];
                    length++;
                    state = 41;
                }
                else {
                    state = 42;
                }
                break;
            case 41:
                if(buffer_curr[next_ptr] >= '0' && buffer_curr[next_ptr] <= '9') {
                    lex[lexeme_ptr++] = buffer_curr[next_ptr++];
                    length++;
                    state = 41;
                }
                else {
                   state = 42;
                }
                break;
            case 42:
                lex[lexeme_ptr] = '\0';
                int type;
                if( (type = lookUp(lex) )== -1) {
                    if (length > 30){
                        fprintf(stderr, "Line No: %d : Function Identifier is longer than the prescribed length of 30 characters(Length = %d of %s)\n", line_no, length, lex);
                        addToken(token, TK_ERROR, lex, TK_FUNID_LEXICAL_ERROR, NULL); // for error recovery due to increased length
                        final_state_flag = 1;
                    } else {
                        addToken(token, TK_FUNID, lex, -1, NULL);
                    }
                    
                }
                else {
                    addToken(token, type, lex, -1, NULL);
                }
                final_state_flag = 1;
                break;
            case 43:
                if ( buffer_curr[next_ptr] >= 'a' && buffer_curr[next_ptr] <= 'z') {
                    lex[lexeme_ptr++] = buffer_curr[next_ptr++];
                    state = 43;
                }
                else {
                    state = 44;
                }
                break;
            case 44:
                lex[lexeme_ptr] = '\0';
                int type1;
                if( (type1 = lookUp(lex) )== -1)
                    addToken(token, TK_FIELDID, lex, -1, NULL);
                else 
                    addToken(token, type1, lex, -1, NULL);
                final_state_flag = 1;
                break;
            case 45:
                if ( buffer_curr[next_ptr] >= 'a' && buffer_curr[next_ptr] <= 'z') {
                    lex[lexeme_ptr++] = buffer_curr[next_ptr++];
                    state = 43;
                }
                else if (buffer_curr[next_ptr] >= '2' && buffer_curr[next_ptr] <= '7') {
                    length++;
                    lex[lexeme_ptr++] = buffer_curr[next_ptr++];
                    state = 46;
                }
                else {
                    state = 44;
                }
                break;
            case 46:
                if ( buffer_curr[next_ptr] >= 'b' && buffer_curr[next_ptr] <= 'd') {
                    length++;
                    lex[lexeme_ptr++] = buffer_curr[next_ptr++];
                    state = 46;
                }
                else if (buffer_curr[next_ptr] >= '2' && buffer_curr[next_ptr] <= '7') {
                    lex[lexeme_ptr++] = buffer_curr[next_ptr++];
                    length++;
                    state = 47;
                }
                else {
                    state = 48;
                }
                break;
            case 47:
                if ( buffer_curr[next_ptr] >= '2' && buffer_curr[next_ptr] <= '7') {
                    length++;
                    lex[lexeme_ptr++] = buffer_curr[next_ptr++];
                    state = 47;
                }

                else if (buffer_curr[next_ptr] >= 'b' && buffer_curr[next_ptr] <= 'd') {
                    lex[lexeme_ptr] = '\0';
                    fprintf(stderr, "Line No: %d : Unknown pattern %s at line\n", line_no, lex);
                    addToken(token, TK_ERROR, lex, -2, NULL);
                    final_state_flag = 1;
                }
                else {
                    state = 48;
                }
                break;
            case 48:
                lex[lexeme_ptr] = '\0';
                if (length > 20){
                    fprintf(stderr, "Line No: %d : Identifier is longer than the prescribed length of 20 characters(Length = %d of %s)\n", line_no, length, lex);
                    addToken(token, TK_ERROR, lex, TK_ID_LEXICAL_ERROR, NULL); // for error recovery due to increased length
                } else {
                    addToken(token, TK_ID, lex, -1, NULL);
                }
                final_state_flag = 1;
                break;
            case 49:
                if ( buffer_curr[next_ptr] >= '0' && buffer_curr[next_ptr] <= '9') {
                    lex[lexeme_ptr++] = buffer_curr[next_ptr++];
                    state = 49;
                }
                else if (buffer_curr[next_ptr] == '.') {
                    lex[lexeme_ptr++] = buffer_curr[next_ptr++];
                    state = 50;
                }
                else {
                    state = 57;
                }
                break;
            case 50:
                if ( buffer_curr[next_ptr] >= '0' && buffer_curr[next_ptr] <= '9') {
                    lex[lexeme_ptr++] = buffer_curr[next_ptr++];
                    state = 51;
                }
                else {
                    lex[lexeme_ptr] = '\0';
                    fprintf(stderr, "Line No: %d : Unknown pattern %s at line\n", line_no, lex);
                    addToken(token, TK_ERROR, lex, -2, NULL);
                    final_state_flag = 1;
                }
                break;
            case 51:
                if ( buffer_curr[next_ptr] >= '0' && buffer_curr[next_ptr] <= '9') {
                    lex[lexeme_ptr++] = buffer_curr[next_ptr++];
                    state = 52;
                }
                else {
                    lex[lexeme_ptr] = '\0';
                    fprintf(stderr, "Line No: %d : Unknown pattern %s at line\n", line_no, lex);
                    addToken(token, TK_ERROR, lex, -2, NULL);
                    final_state_flag = 1;
                }
                break;
            case 52:
                if ( buffer_curr[next_ptr] == 'E') {
                    lex[lexeme_ptr++] = buffer_curr[next_ptr++];
                    state = 53;
                }
                else {
                    state = 58;
                }
                break;
            case 53:
                if (buffer_curr[next_ptr] == '+' || buffer_curr[next_ptr] == '-') {
                    lex[lexeme_ptr++] = buffer_curr[next_ptr++];
                    state = 54;
                }
                else if (buffer_curr[next_ptr] >= '0' && buffer_curr[next_ptr] <= '9') {
                    lex[lexeme_ptr++] = buffer_curr[next_ptr++];
                    state = 55;
                }
                else {
                    lex[lexeme_ptr] = '\0';
                    fprintf(stderr, "Line No: %d : Unknown pattern %s at line\n", line_no, lex);
                    addToken(token, TK_ERROR, lex, TK_RNUM_LEXICAL_ERROR, NULL);
                    final_state_flag = 1;
                }
                break;
            case 54:
                if (buffer_curr[next_ptr] >= '0' && buffer_curr[next_ptr] <= '9') {
                    lex[lexeme_ptr++] = buffer_curr[next_ptr++];
                    state = 55;
                }
                else {
                    lex[lexeme_ptr] = '\0';
                    fprintf(stderr, "Line No: %d : Unknown pattern %s at line\n", line_no, lex);
                    addToken(token, TK_ERROR, lex, TK_RNUM_LEXICAL_ERROR, NULL);
                    final_state_flag = 1;
                }
                break;
            case 55:
                if (buffer_curr[next_ptr] >= '0' && buffer_curr[next_ptr] <= '9') {
                    lex[lexeme_ptr++] = buffer_curr[next_ptr++];
                    state = 56;
                }
                else {
                    lex[lexeme_ptr] = '\0';
                    fprintf(stderr, "Line No: %d : Unknown pattern %s at line\n", line_no, lex);
                    addToken(token, TK_ERROR, lex, TK_RNUM_LEXICAL_ERROR, NULL);
                    final_state_flag = 1;
                }
                break;
            case 56:
                lex[lexeme_ptr] = '\0';
                addToken(token, TK_RNUM, lex, -1, NULL);
                final_state_flag = 1;
                break;
            case 57:
                lex[lexeme_ptr] = '\0';
                int value = atoi(lex);
                Value* val = (Value*)malloc(sizeof(Value));
                val->num = value;

                addToken(token, TK_NUM, lex, 0, val);
                final_state_flag = 1;
                break;
            case 58:
                lex[lexeme_ptr] = '\0';
                float value1 = atof(lex);
                value1 = (int)(value1*100) / 100.0;
                Value* val1 = (Value*)malloc(sizeof(Value));
                val1->r_num = value1;
                addToken(token, TK_RNUM, lex, 1, val1);
                final_state_flag = 1;
                break;
        }
        if(final_state_flag == 1) {
            length = 0;
            break;    
        }
    }

    return token;
}

void initializeLexer() {
    lookUpTable = createAndPopulateLookUpTable(keywords, TOTAL_TERMINALS_INCLUDING_ERROR, NO_SLOTS);
    buffer_curr = (char*) malloc(sizeof(char) * BUFFERSIZE + 1);
    buffer_prev = (char*) malloc(sizeof(char) * BUFFERSIZE + 1);
    memset(buffer_curr, 0, BUFFERSIZE+1);
    memset(buffer_prev, 0, BUFFERSIZE+1);
    return;
}

void unSetOverFlag() {
    file_over_flag = 0;
    line_no = 1;
}