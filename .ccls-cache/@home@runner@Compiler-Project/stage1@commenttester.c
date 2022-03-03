#include <stdio.h>
#include<stdlib.h>
void removeComments(char *testcaseFile, char *cleanFile){
  FILE *fptr1,*fptr2;
  char c;
  fptr1 = fopen(testcaseFile,"r");
  
  if ((fptr1 = fopen(testcaseFile,"r")) == NULL){
       printf("Error! opening file");
       exit(1);
   }

  fptr2 = fopen(cleanFile,"w");

   if(fptr2 == NULL)
   {
      printf("Error!");   
      exit(1);             
   }

  printf("""Yes")
  
   while(fscanf(fptr1,"%c", &c))
     {
       if (c!='%')
       {
         printf("Value of character=%c", c);
         fprintf(fptr2,"%c", c);
       }
       else{
         while(fscanf(fptr1,"%s",&c)&&c!='\n');
         fprintf(fptr2,"%s","\n");
         
       }
       
     }

   fclose(fptr1);
   fclose(fptr2);

  
  
}


void main() {

  removeComments("t2.txt","commentst2.txt");
  
}