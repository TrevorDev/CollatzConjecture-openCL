/********
omp_oneToAll.c
summary: Collatz Conjecture problem with one to all approach
author: Heesung Ahn, Trevor Baron
date: Nov 28 2013
********/

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>
#include <inttypes.h>
#define DATA_SIZE (524288)

int parseArgs(int argc, char * argv[]);
void InitializeData();

int depth = 1;
int rep = 1;
int pN = 8;
int *input, *output;

int main(int argc, char *argv[])
{
  int i, j, k, altPos;
  double start, stop;
  unsigned int cur;
  if (parseArgs(argc, argv))
  {
    return 0;
  }

  start = omp_get_wtime();

  omp_set_num_threads(pN);
  
  for (k = 0; k < rep; k++)
  {
    InitializeData();
    for (j = 0; j < depth; j++)
    {
      #pragma omp parallel for private(altPos, cur, i)
      for (i = 0; i < input[0]; i++)
      {
        altPos = input[0]+i;
        cur = input[i];
        if (cur != 0)
        {
          if(cur!=1&&(cur-1)%3==0)
          {
            if((cur-1)/3!=1)
            {
              output[altPos]=(cur-1)/3;
            }       
          }
          output[i] = cur*2;
        }
      }
      for (i = 0; i < input[0]; i++)
      {
        input[i] = output[i];
      }
    }
  }
  stop = omp_get_wtime();
  
  for(int i = 0; i < output[0]; i++)
  {
      if(output[i] >= 0)
      {
          if(i==0)
          {
             printf("%d",output[i]);
          }
          else
          {
             printf(",%d",output[i]);
          }
      }
  }
  printf("\n");
  printf("TIME- %f\n",(double)(stop - start));

  return 0;
}

int parseArgs(int argc, char * argv[]){
    int repSet = 0;
    int depthSet = 0;
    for(int i=0;i<argc;i++){
        int num = atoi(argv[i]+1);
        switch( argv[i][0] ) 
        {
        case 'd':
            depth=num;
            depthSet=1;
            break;
        case 'r':
            rep=num;
            repSet=1;
            break;
        case 'p':
            pN=num;
            break;
        default :
            break;
        }
    }
    if(!depthSet || !repSet || rep<=0||depth<0||depth>19){
        printf("Invalid arguments, expected 2 args:\n");
        printf("r - number of repititions greater than 0(to increase runtime)\n");
        printf("d - depth of reverse collatz tree to print out between 1 and 19\n");
        printf("p - number of processors\n");
        printf("Example execution ./openCL-allToOne r1 d8 > output.txt\n");
        return 1;
    }
    return 0;
}

void InitializeData()
{
  input = (int *) calloc (DATA_SIZE, sizeof(int));
  output = (int *) calloc (DATA_SIZE, sizeof(int));

  input[0] = 1;
}