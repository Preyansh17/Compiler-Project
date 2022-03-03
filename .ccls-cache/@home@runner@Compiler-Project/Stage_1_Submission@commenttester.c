#include<stdio.h>

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