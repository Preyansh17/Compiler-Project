/*
Group:33
2016A7PS0036P Megh Thakkar
2016A7PS0103P Sahil Singla
2016A7PS0110P Sankalp Sangle
2016A7PS0150P Patel Parth
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "parser.h"
#include "stack.h"
#include "NaryTree.h"
#include "hashTable.h"
#include "lexer.h"

char* tokenMap2[] = {"TK_ASSIGNOP",
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
char* non_terminals2[] = {"primitiveDatatype", "relationalOp", "assignmentStmt","fieldDefinitions", "declaration", "moreFields", "singleOrRecId", "elsePart", "output_par", "arithmeticExpression", "mainFunction", "remaining_list", "more_ids", "returnStmt", "idList", "ioStmt", "global_or_not", "conditionalStmt", "stmts", "input_par", "allVar", "program", "booleanExpression", "highPrecedenceOperators", "inputParameters", "fieldDefinition", "iterativeStmt", "function", "all", "factor", "logicalOp", "dataType", "outputParameters", "typeDefinition", "stmt", "otherStmts", "var", "constructedDatatype", "optionalReturn", "termPrime", "term", "lowPrecedenceOperators", "temp", "parameter_list", "new_24", "declarations", "funCallStmt", "typeDefinitions", "otherFunctions", "expPrime", "var_mid"};
char* keywords2[] = {NULL,
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
int initialized_plook_up = 0;
int initialized_look_up = 0;
int created_parse_table = 0;
int root_node_created = 0;
int initialized_lexer = 0;
int grammar_file_closed = 0;

int main(int argc, char* argv[]){

	if(argc != 3){
		printf("Check command: ./compiler testcase(x).txt outputParseTree.txt\n");
		return 0;
	}
	clock_t start_time, end_time;

	printf("***************************Information of implementation details:***************************\n\n");
	printf("(a) FIRST and FOLLOW set automated\n");
	printf("(b) Both lexical and syntax analysis modules implemented\n");
	printf("(c) Module works with all given test cases\n");
	printf("(d) Parse Tree constructed\n\n");

	printf("***************************End of implementation details***************************\n\n");

	double total_CPU_time, total_CPU_time_in_seconds;
    FILE * fp1 = fopen(argv[1], "a");
    fputs("\n", fp1);
    fclose(fp1);

	printf("----------------------------- Execution started -----------------------------\n");

	if (initialized_lexer == 0){
		initializeLexer();
		initialized_lexer = 1;
	}
	int break_loop = 0;
	char test_file_name[100];
	strcpy(test_file_name, argv[1]);
	/*************************** Interface ***************************/
	while(1){

		printf("\nEnter the number corresponding to the functionality:\n\n");
		printf("0: Exit the interface\n");
		printf("1: For removal of comments of the file\n");
		printf("2: For printing the token list generated by the lexer\n");
		printf("3: To verify the syntactic correctness of the input source code and print the parse tree\n");
		printf("4: To print the time taken to find syntactic correctness\n");

		printf("\n\nYour Input: ");

		int choice;
		scanf("%d",&choice);
		printf("Choice : %d\n", choice);
		switch(choice){

			case 0:{
				break_loop = 1;
			}break;

			case 1:{
				printf("Remove comments of %s\n", test_file_name);
				if (initialized_lexer == 0){
					initializeLexer();
					initialized_lexer = 1;
				}
				char temp_file_name[100];
				strcpy(temp_file_name, test_file_name);
				FILE* test_case = fopen(test_file_name, "r");
				FILE* cleaned = fopen("cleaned_test_file.txt", "w");
				if (test_case == NULL) {
					printf("%s\n", test_file_name);
					printf("Error opening test file\n");
					continue;
				}
				if (cleaned == NULL) {
					printf("Error opening cleaned file\n");
					continue;
				}
				removeComments(test_case, cleaned);
				strcpy(test_file_name, temp_file_name);
			}break;

			case 2:{
				printf("Lexical Tokens of %s\n", test_file_name);
				if (initialized_lexer == 0){
					initializeLexer();
					initialized_lexer = 1;
				}
				printf("Lexer init\n");
				char temp_file_name[100];
				strcpy(temp_file_name, test_file_name);
				FILE* fp = fopen(temp_file_name, "r");
				unSetOverFlag();
				Token* token;
				while((token = getNextToken(&fp)) != NULL) {
					printf("_______________________________________________________\n");
					printf("Token type : %s\nToken line number : %d\nToken lexeme : %s\nToken value : %f\nToken vtype : %d\n", tokenMap2[token->type], token->line_no, token->lexeme, token->value->r_num, token->vtype);
					printf("_______________________________________________________\n");
					if (fp == NULL) {
						printf("File pointer null\n");
					}
				}
				if (fp != NULL)
					fclose(fp);
				printf("Over...");
				strcpy(test_file_name, temp_file_name);
			}break;

			case 3:{
				// printf("In 3\n");
				char temp_file_name[100];
				strcpy(temp_file_name, test_file_name);
				printf("Syntax analysis of %s\n", test_file_name);
				unSetOverFlag();
				start_time = clock();
				if (initialized_lexer == 0) {
					initializeLexer();
					initialized_lexer = 1;
				}
				if (initialized_plook_up == 0){
					plookupTable = createLookUpTable(NUMBER_OF_SLOTS);
					plookupTable = PopulateLookUpTable(plookupTable, tokenMap2, TOTAL_TERMINALS, NUMBER_OF_SLOTS);
					plookupTable = PopulateLookUpTable(plookupTable, non_terminals2, TOTAL_NON_TERMINALS, NUMBER_OF_SLOTS);
					initialized_plook_up = 1;
				}
				FirstAndFollow* first_and_follow = ComputeFirstAndFollowSets(NUMBER_OF_SLOTS);
				
				printFirstAndFollow(first_and_follow);
				if (created_parse_table == 0){
					parse_table = createParseTable(first_and_follow);
					created_parse_table = 1;
				}
				// printParseTable(parse_table);
				int errors;

				if (root_node_created == 0){
					rootNode = parseInputSourceCode(temp_file_name, parse_table, &errors);
					root_node_created = 1;
				}

				FILE* outputfile = fopen(argv[2], "w");
				fprintf(outputfile, "%-25s %-10s %-15s %-15s %-10s %-5s %s\n\n\n", "LEXEME","LINE","TOKEN","VALUE","PARENT","IS LEAF?","NODE SYMBOL");

				printParseTreeUtility(rootNode, tokenMap2, non_terminals2, outputfile);

				fclose(outputfile);
				// printf("-------Parsing Table Created-------\n\n");
				strcpy(test_file_name, temp_file_name);
				end_time = clock();
				total_CPU_time = (double) (end_time - start_time);
				total_CPU_time_in_seconds = total_CPU_time / CLOCKS_PER_SEC;
			}break;

			case 4:{
				printf("Total CPU time : %lf\n", total_CPU_time);
				printf("Total CPU time in seconds : %lf\n", total_CPU_time_in_seconds);
			}break;

		}

		if(break_loop)
			break;

	}
	printf("----------------------------- Execution complete -----------------------------\n");

	return 0;
}

