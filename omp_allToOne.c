#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

int parseArgs(int argc, char * argv[]);
void InitializeData();
void printData();
void writeToFile();
int collatz(int number);


int arraySize = 1;
int rep = 1;
int *seed = NULL;
int *input, *output;

int main(int argc, char *argv[])
{
  int i, j, cur, counter, correct;
  double start, stop;

  if (parseArgs(argc, argv))
  {
    return 0;
  }

  start = omp_get_wtime();
  InitializeData();

  correct = 0;

  for(int i = 0; i < arraySize; i++)
  {
      if(input[i] >= 1){
          printf("%d ",input[i]);
      }
  }
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

 //printData();

  // Validate our results
  correct = 0;

  for(int i = 0; i < arraySize; i++)
  {
      if(output[i] >= 0){
          correct++;
          //printf("%d ",output[i]);
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
    int sizeSet = 0;
    for(int i=0;i<argc;i++){
        int num = atoi(argv[i]+1);
        switch( argv[i][0] ) 
        {
        case 's':
            seed = calloc(1,sizeof(int));
            *seed=num;
            break;
        case 'n':
            arraySize=num;
            sizeSet=1;
            break;
        case 'r':
            rep=num;
            repSet=1;
            break;
        default :
            break;
        }
    }
    if(!sizeSet || !repSet || arraySize<=0 || rep<=0){
        printf("Invalid arguments, expected 2 or 3 args:\n");
        printf("r - number of repititions greater than 0(to increase runtime)\n");
        printf("n - numbers to compute between 1 and 524288\n");
        printf("s - random seed (optional leave out to seed by time)\n");
        printf("Example execution ./openCL-allToOne r3 n524288 s3 > output.txt\n");
        return 1;
    }
    return 0;
}

void InitializeData()
{
  int i;
  input = (int *) malloc (arraySize*sizeof(int));
  output = (int *) calloc (arraySize, sizeof(int));
  
  if(seed==NULL)
  {
    srand(time(NULL));
  }
  else
  {
    srand(*seed);
  }

  for (i = 0; i < arraySize; i++)
  {
      input[i] = (rand()%100000)+1;
  }
}

void printData()
{
  int i;

  for (i = 0; i < arraySize; i++)
  {
    printf("%d ", output[i]);
  }
  printf("\n");
}

/*
void writeToFile()
{
  int i, j;

  FILE *f = fopen("redblue.txt", "w");
  if (f == NULL)
  {
    printf("Error opening file!\n");
    exit(1);
  }

  for (i = 0; i < bN; i++)
  {
    for (j = 0; j < bN; j++)
    { 
      switch(board[i][j])
      {
        case 0:
          fprintf(f, "%c", ' ');
          break;
        case 1:
          fprintf(f, "%c", '>');
          break;
        case 2:
          fprintf(f, "%c", 'V');
      }
    }
    fprintf(f, "\n");
  }

  fclose(f);
}*/