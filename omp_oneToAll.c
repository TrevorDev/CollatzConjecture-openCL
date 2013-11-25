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
int *input, *output;

int main(int argc, char *argv[])
{
  int i, j, counter, correct;
  double start, stop;
  uint64_t cur;
  if (parseArgs(argc, argv))
  {
    return 0;
  }

  start = omp_get_wtime();
  InitializeData();

  correct = 0;
  omp_set_num_threads(16);
    
  for (i = 0; i < rep; i++)
  {
    #pragma omp parallel for private(j, counter, cur)
    for (j = 0; j < arraySize; j++)
    {
      counter=0;
      cur = input[j];
      while(cur != 1)
      {
        counter++;
        if(cur % 2 == 0) 
        {
          cur = cur / 2;
        }
        else 
        {
          cur = (cur * 3) + 1;
        }
        if (counter > 10000)
        {
          counter = -1;
          printf("input:%d\n",input[j]);
          break;
        } 
        /*if (counter > 1000000)
        {
          printf("input:%d %d\n",input[j], cur);
        } */
      }
      output[j] = counter;
    }
  }


  // Validate our results
  correct = 0;

  for(int i = 0; i < arraySize; i++)
  {
      if(output[i] >= 0)
      {
          correct++;
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
  stop = omp_get_wtime();
  // Print a brief summary detailing the results
  printf("Computed '%d/%d' values to 1!\n", correct, arraySize);
  printf("TIME- %f\n",(double)(stop - start));
  //writeToFile();
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
        default :
            break;
        }
    }
    if(!depthSet || !repSet || rep<=0||depth<0||depth>19){
        printf("Invalid arguments, expected 2 args:\n");
        printf("r - number of repititions greater than 0(to increase runtime)\n");
        printf("d - depth of reverse collatz tree to print out between 1 and 19\n");
        printf("Example execution ./openCL-allToOne r1 d8 > output.txt\n");
        return 1;
    }
    return 0;
}

void InitializeData()
{
  int i;
  input = (int *) calloc (DATA_SIZE, sizeof(int));
  output = (int *) calloc (DATA_SIZE, sizeof(int));

  input[0] = 1;
}