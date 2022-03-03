#include "lexer.h"
#include "hashTableDef.h"
#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<fcntl.h>
#define BUFFERSIZE 200
#define LEXSIZE 200
#define TK_ID_LEXICAL_ERROR 5
#define TK_FUNID_LEXICAL_ERROR 6
#define TK_RNUM_LEXICAL_ERROR 7

// Twin buffers
char* buffer_prev;
char* buffer_curr;

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

int file_over_flag = 0;
int length = 0;

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

int next_ptr = 0;
int line_no = 1;

void addToken(Token* token, Tokentype type, char* lexeme, int vtype, Value* value){
	
    Value* val = (Value*)malloc(sizeof(Value));
    if (value != NULL) {
        val->num = value->num;
        val->r_num = value->r_num;
        free(value);
    }

    token->lexeme = (char*)malloc(LEXSIZE*sizeof(char));
	strcpy(token->lexeme, lexeme);
    
    token->line_no = line_no;
	token->type = type;
    
    token->value = val;
    token->vtype = vtype;

}

int checkForRefillBuffer(FILE** fp) {
    
    if(buffer_curr[next_ptr] !='\0' && buffer_curr[next_ptr] !=-1) 
       return file_over_flag; 
    
    next_ptr = 0;
    
    *fp = getStream(*fp);
    if(*fp != NULL) {
        return file_over_flag;   					
    } 
    file_over_flag = 1;	
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

int between2to7(char c) {
    return (c >= '2' && c <= '7') ? 1 : 0;
}

int between0to9(char c) {
    return (c >= '0' && c <= '9') ? 1 : 0;
}

int betweenBtoD(char c) {
    return (c >= 'b' && c <='d') ? 1 : 0;
}

int betweenAtoZ(char c) {
    return (c >= 'a' && c <='z') ? 1 : 0;
}

int betweenCapitalAtoZ(char c) {
    return (c >= 'A' && c <='Z') ? 1 : 0;

}

tokenInfo getNextToken(FILE** fp) {
    
    int lexeme_ptr = 0;
    int final_state_flag = 0;
    int state = 1;
	
	Token* token = (Token*)malloc(sizeof(Token));
    char lex[LEXSIZE];

    //Loading Buffer
	if (checkForRefillBuffer(fp) == 1) {
        return NULL;
    }
    

    if(file_over_flag==1) {

		if(*fp != NULL)
			fclose(*fp);

		return NULL;
	}

    
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
                else if (betweenAtoZ(buffer_curr[next_ptr]) && !betweenBtoD(buffer_curr[next_ptr])) {
                    state = 43;
                    lex[lexeme_ptr++] = buffer_curr[next_ptr];
                } 
                else if (betweenBtoD(buffer_curr[next_ptr])) {
                    state = 45;
                    lex[lexeme_ptr++] = buffer_curr[next_ptr];
                    length++;
                }
                else if (between0to9(buffer_curr[next_ptr])) {
                    state = 49;
                    lex[lexeme_ptr++] = buffer_curr[next_ptr];
                }
                else {
                    lex[lexeme_ptr++] = buffer_curr[next_ptr];
                    lex[lexeme_ptr] = '\0';
                    fprintf(stderr, "Line No: %d : Error : Unknown symbol <%s>\n", line_no, lex);
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
                    final_state_flag = 1;
                    lex[lexeme_ptr] = '\0';
                    fprintf(stderr, "Line No: %d : Error: Unknown pattern <%s>\n", line_no, lex);
                    addToken(token, TK_ERROR, lex, -2, NULL);
                }
                break;
            case 16:
                if ( buffer_curr[next_ptr] == '&') {
                    lex[lexeme_ptr++] = buffer_curr[next_ptr++];
                    state = 17;
                }
                else {
                    final_state_flag = 1;
                    lex[lexeme_ptr] = '\0';
                    fprintf(stderr, "Line No: %d : Error: Unknown pattern <%s>\n", line_no, lex);
                    addToken(token, TK_ERROR, lex, -2, NULL);
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
                    final_state_flag = 1;
                    lex[lexeme_ptr] = '\0';
                    fprintf(stderr, "Line No: %d : Error: Unknown pattern <%s>\n", line_no, lex);
                    addToken(token, TK_ERROR, lex, -2, NULL);
                }
                break;
            case 19:
                if ( buffer_curr[next_ptr] == '@') {
                    lex[lexeme_ptr++] = buffer_curr[next_ptr++];
                    state = 20;
                }
                else {
                    final_state_flag = 1;
                    lex[lexeme_ptr] = '\0';
                    fprintf(stderr, "Line No: %d : Unknown pattern %s at line\n", line_no, lex);
                    addToken(token, TK_ERROR, lex, -2, NULL);
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
                    final_state_flag = 1;
                    lex[lexeme_ptr] = '\0';
                    fprintf(stderr, "Line No: %d : Error: Unknown symbol <%s>\n", line_no, lex);
                    addToken(token, TK_ERROR, lex, -2, NULL);
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
                    final_state_flag = 1;
                    lex[lexeme_ptr] = '\0';
                    fprintf(stderr, "Line No: %d : Error: Unknown pattern <%s>\n", line_no, lex);
                    addToken(token, TK_ERROR, lex, -2, NULL);
                }
                break;
            case 24:
                lex[lexeme_ptr] = '\0';
                addToken(token, TK_NE, lex, -1, NULL);
                final_state_flag = 1;
                break;
            case 25:
                if ( buffer_curr[next_ptr] == '=') {
                    state = 26;
                }
                else if (buffer_curr[next_ptr] == '-') {
                    state = 27;
                }
                else {
                    state = 30;
                    break;
                }
                lex[lexeme_ptr++] = buffer_curr[next_ptr++];
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
                    final_state_flag = 1;
                    lex[lexeme_ptr] = '\0';
                    fprintf(stderr, "Line No: %d : Error: Unknown pattern <%s>\n", line_no, lex);
                    addToken(token, TK_ERROR, lex, -2, NULL);
                }
                break;
            case 28:
                if(buffer_curr[next_ptr] == '-') {
                    lex[lexeme_ptr++] = buffer_curr[next_ptr++];
                    state = 29;
                }
                else {
                    final_state_flag = 1;
                    lex[lexeme_ptr] = '\0';
                    fprintf(stderr, "Line No: %d : Error: Unknown pattern <%s>\n", line_no, lex);
                    addToken(token, TK_ERROR, lex, -2, NULL);
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
                if(betweenAtoZ(buffer_curr[next_ptr])) {
                    lex[lexeme_ptr++] = buffer_curr[next_ptr++];
                    state = 37;
                }
                else {
                    final_state_flag = 1;
                    lex[lexeme_ptr] = '\0';
                    fprintf(stderr, "Line No: %d : Error: Unknown pattern <%s>\n", line_no, lex);
                    addToken(token, TK_ERROR, lex, -2, NULL);
                }
                break;
            case 37:
                if(betweenAtoZ(buffer_curr[next_ptr])) {
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
                if (betweenAtoZ(buffer_curr[next_ptr]) || betweenCapitalAtoZ(buffer_curr[next_ptr])) {
                    lex[lexeme_ptr++] = buffer_curr[next_ptr++];
                    length++;
                    state = 40;
                }
                else {
                    final_state_flag = 1;
                    lex[lexeme_ptr] = '\0';
                    fprintf(stderr, "Line No: %d : Error: Unknown pattern <%s>\n", line_no, lex);
                    addToken(token, TK_ERROR, lex, -2, NULL);
                }
                break;
            case 40:
                if(betweenAtoZ(buffer_curr[next_ptr]) || betweenCapitalAtoZ(buffer_curr[next_ptr])) {
                    length++;
                    state = 40;
                }
                else if (between0to9(buffer_curr[next_ptr])) {
                    length++;
                    state = 41;
                }
                else {
                    state = 42;
                    break;
                }
                lex[lexeme_ptr++] = buffer_curr[next_ptr++];
                break;
            case 41:
                if(between0to9(buffer_curr[next_ptr])) {
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
                        fprintf(stderr, "Line No: %d : Error: Function Identifier is longer than the prescribed length of 30 characters\n", line_no);
                        addToken(token, TK_ERROR, lex, TK_FUNID_LEXICAL_ERROR, NULL); // for error recovery due to increased length
                    } 
                    else
                        addToken(token, TK_FUNID, lex, -1, NULL);
                    
                }
                else {
                    addToken(token, type, lex, -1, NULL);
                }
                final_state_flag = 1;
                break;
            case 43:
                if (betweenAtoZ(buffer_curr[next_ptr])) {
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
                if (between2to7(buffer_curr[next_ptr])) {
                    length++; 
                    state = 46;
                }
                else if (betweenAtoZ(buffer_curr[next_ptr])) {
                    state = 43;
                }
                else {
                    state = 44;
                    break;
                }
                lex[lexeme_ptr++] = buffer_curr[next_ptr++];
                break;
            case 46:
                if (between2to7(buffer_curr[next_ptr])) {
                    state = 47;
                    length++;
                }
                else if (betweenBtoD(buffer_curr[next_ptr])) {
                    state = 46;
                    length++;
                }
                else {
                    state = 48;
                    break;
                }
                lex[lexeme_ptr++] = buffer_curr[next_ptr++];
                break;
            case 47:
                if (betweenBtoD(buffer_curr[next_ptr])) {
                    final_state_flag = 1;
                    lex[lexeme_ptr] = '\0';
                    addToken(token, TK_ERROR, lex, -2, NULL);
                    fprintf(stderr, "Line No: %d : Error: Unknown pattern <%s>\n", line_no, lex);
                }
                else if (between2to7(buffer_curr[next_ptr])) {
                    state = 47;
                    lex[lexeme_ptr++] = buffer_curr[next_ptr++];
                    length++;
                }
                else
                    state = 48;
                break;
            case 48:
                lex[lexeme_ptr] = '\0';
                if (length > 20){
                    fprintf(stderr, "Line No: %d : Error: Variable Identifier is longer than the prescribed length of 20 characters\n", line_no);
                    addToken(token, TK_ERROR, lex, TK_ID_LEXICAL_ERROR, NULL); // for error recovery due to increased length
                } else {
                    addToken(token, TK_ID, lex, -1, NULL);
                }
                final_state_flag = 1;
                break;
            case 49:
                if (between0to9(buffer_curr[next_ptr])) {
                    state = 49;
                }
                else if (buffer_curr[next_ptr] == '.') {
                    state = 50;
                }
                else {
                    state = 57;
                    break;
                }
                lex[lexeme_ptr++] = buffer_curr[next_ptr++];
                break;
            case 50:
                if (between0to9(buffer_curr[next_ptr])) {
                    state = 51;
                    lex[lexeme_ptr++] = buffer_curr[next_ptr++];
                }
                else {
                    final_state_flag = 1;
                    lex[lexeme_ptr] = '\0';
                    fprintf(stderr, "Line No: %d : Error: Unknown pattern <%s>\n", line_no, lex);
                    addToken(token, TK_ERROR, lex, -2, NULL);
                }
                break;
            case 51:
                if (between0to9(buffer_curr[next_ptr])) {
                    lex[lexeme_ptr++] = buffer_curr[next_ptr++];
                    state = 52;
                }
                else {
                    final_state_flag = 1;
                    lex[lexeme_ptr] = '\0';
                    fprintf(stderr, "Line No: %d : Error: Unknown pattern <%s>\n", line_no, lex);
                    addToken(token, TK_ERROR, lex, -2, NULL);
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
                    state = 54;
                }
                else if (between0to9(buffer_curr[next_ptr])) {
                    state = 55;
                }
                else {
                    final_state_flag = 1;
                    lex[lexeme_ptr] = '\0';
                    fprintf(stderr, "Line No: %d : Error: Unknown pattern <%s>\n", line_no, lex);
                    addToken(token, TK_ERROR, lex, TK_RNUM_LEXICAL_ERROR, NULL);
                    break;
                }
                lex[lexeme_ptr++] = buffer_curr[next_ptr++];
                break;
            case 54:
                if (between0to9(buffer_curr[next_ptr])) {
                    lex[lexeme_ptr++] = buffer_curr[next_ptr++];
                    state = 55;
                }
                else {
                    final_state_flag = 1;
                    lex[lexeme_ptr] = '\0';
                    fprintf(stderr, "Line No: %d : Error: Unknown pattern <%s>\n", line_no, lex);
                    addToken(token, TK_ERROR, lex, TK_RNUM_LEXICAL_ERROR, NULL);
                }
                break;
            case 55:
                if (between0to9(buffer_curr[next_ptr])) {
                    lex[lexeme_ptr++] = buffer_curr[next_ptr++];
                    state = 56;
                }
                else {
                    final_state_flag = 1;
                    lex[lexeme_ptr] = '\0';
                    fprintf(stderr, "Line No: %d : Error: Unknown pattern <%s>\n", line_no, lex);
                    addToken(token, TK_ERROR, lex, TK_RNUM_LEXICAL_ERROR, NULL);
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

void removeComments(char *testcaseFile, char *cleanFile)
{
  FILE *fptr1;
  FILE *fptr2;
  char c;
  fptr2 = fopen(cleanFile,"w");

   if(fptr2 == NULL)
   {
      printf("Error!");              
   }

   if ((fptr1 = fopen(testcaseFile,"r")) == NULL){
       printf("Error! opening file");
   }

   while(fscanf(fptr1,"%c", &c)!=EOF)
  {
    if(c!='%')
      fprintf(fptr2,"%c",c);
    else
      {
        while(fscanf(fptr1,"%c",&c)!=EOF && c!='\n');
        fprintf(fptr2,"%s","\n");
      }
  }

   fclose(fptr1); 
   fclose(fptr2);

   
}