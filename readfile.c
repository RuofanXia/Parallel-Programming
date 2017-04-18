#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define GENESIZE 1000

int main()
{
   char buffer[6000] ;
   char *record;
   char *line;
   
   char *path = "./data/";
    char str[20];
    strcpy(str, path);
    strcat(str, "1.genome");

   FILE *fstream = fopen(path,"r");

   int *arr1 = (int *)malloc(GENESIZE*sizeof(int));
   if(fstream == NULL)
   {
      printf("\n file opening failed ");
      return -1 ;
   }
     
   char *ptr = "C:";
   int index = 0;
   while((line=fgets(buffer,sizeof(buffer),fstream))!=NULL)
   {
      record = strtok(line," ");
      //printf("record: %s\n", record);
      if (!strcmp(ptr, record))
      {
        while(record != NULL)
        {
          record = strtok(NULL, " ");
          if (record != NULL && *record != '\n')
          {
              *(arr1+index) = atoi(record);
              index++;
          }
        }
      }     
   }

   for (int i=0; i<GENESIZE; i++)
   {
      printf("Value: %d ", *(arr1+i));
   }
}