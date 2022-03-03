#include "lexerDef.h"

tokenInfo getNextToken(twinBuffer B);

FILE *getStream(FILE *fp);

void removeComments(char *testcaseFile, char *cleanFile);