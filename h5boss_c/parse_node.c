#include "parse_node.h"
#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include <assert.h>
#include<libgen.h>
//structure for maintaining key value pair
int bufi=0;
//split a string by delimiter, return a char **
char** str_split(char* a_str, const char a_delim)
{
    char** result    = 0;
    size_t count     = 0;
    char* tmp        = a_str;
    char* last_comma = 0;
    char delim[2];
    delim[0] = a_delim;
    delim[1] = 0;

    /* Count how many elements will be extracted. */
    while (*tmp)
    {
        if (a_delim == *tmp)
        {
            count++;
            last_comma = tmp;
        }
        tmp++;
    }

    /* Add space for trailing token. */
    count += last_comma < (a_str + strlen(a_str) - 1);

    /* Add space for terminating null string so caller
       knows where the list of returned strings ends. */
    count++;

    result = malloc(sizeof(char*) * count);

    if (result)
    {
        size_t idx  = 0;
        char* token = strtok(a_str, delim);

        while (token)
        {
            assert(idx < count);
            *(result + idx++) = strdup(token);
            token = strtok(0, delim);
        }
        assert(idx == count - 1);
        *(result + idx) = 0;
    }

    return result;
}
char ** path_split(char* path) {
    char **token=(char **)malloc(2*sizeof(char *));
    char * dict=strdup(path);
    char * basec=strdup(path);
    char * d=strdup(dirname(dict));
    char * b=strdup(basename(basec));
    token[0]=d;
    token[1]=b;
    free(dict);
    free(basec);
    //printf("ori:%s\n",path);
    //printf("dir:%s\n",dirname(dict));
    //printf("base:%s\n",basename(basec));
    return token;
}
char ** parse_nodes(char * file,int numline){
  char ** buf=(char **)malloc(sizeof(char *)*(numline+1));
  FILE * fp;
  char * line = (char *)malloc(sizeof(char)*100);
  size_t len = 0;
  size_t read;
  fp = fopen(file,"r");
  if (fp == NULL)
     exit(EXIT_FAILURE);
  while ((read = getline(&line,&len,fp))!= -1 ){
    //printf("line of length %zu:\n",read);
    //printf("%s",line);
    if('\n'!=line[0]){
     buf[bufi] = (char *)malloc(strlen(line)+1);
     line[strcspn(line,"\n")]=0;
     strcpy(buf[bufi],line);
     bufi++;
    }
  }
  fclose(fp);
  if (line!=NULL)
    free(line);
  return buf;
}
struct Nodes_pair * dataset_list (char * file,const char sep,int numline){
    struct Nodes_pair * dl= malloc(sizeof(struct Nodes_pair));
    
    char ** lines=parse_nodes(file,numline);
    if(lines==NULL){
     printf("lines parsing error\n");
    }
    char ** dl_keys;
    char ** dl_values;
    dl_keys=(char **)malloc(sizeof(char *)*numline);
    if (dl_keys==NULL)  {
     printf("dl_keys allocation error\n");
    }
    dl_values=(char **)malloc(sizeof(char *)*numline);
    if (dl_values==NULL)  {
     printf("dl_values allocation error\n");
    }
    dl->count=bufi;
    int i;
    for (i=0;i<bufi;i++){
      char ** tokens=str_split(lines[i],sep);
      dl_keys[i]=(char *)malloc(strlen(tokens[0])+1);
      dl_values[i]=(char *)malloc(strlen(tokens[1])+1);
      strcpy(dl_keys[i],tokens[0]);
      strcpy(dl_values[i],tokens[1]);
    }
    dl->keys=dl_keys;
    dl->values=dl_values;
    return dl;
}
/*
void main(int argc, char ** argv){
   if(argc!=2) {
     printf("usage: %s filename\n",argv[0]); 
     exit(EXIT_FAILURE);
   }
   int j;
   const char sep=':';
   struct Nodes_pair * dl=dataset_list(argv[1],sep);
   //parse_nodes(argv[1]);
   for(j=0;j<dl->count;j++){
    printf("parsed line %d: %s, %s",j,dl->keys[j],dl->values[j]);
   }
   exit(EXIT_SUCCESS);
}
*/
