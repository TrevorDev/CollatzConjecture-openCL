#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

int parseArgs(int argc, char * argv[]);
void InitializeData();

int arraySize = 1;
int rep = 1;
int pN = 8;
int *seed = NULL;
int *input, *output;

int main(int argc, char *argv[])
{
  int i, j, counter, correct;
  double start, stop;
  unsigned int cur;
  if (parseArgs(argc, argv))
  {
    return 0;
  }

  start = omp_get_wtime();
  InitializeData();

  correct = 0;
  omp_set_num_threads(pN);
    
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
        if (counter > 100000)
        {
          counter = -1;
          break;
        } 
      }
      output[j] = counter;
    }
  }

  stop = omp_get_wtime();
  // Validate our results
  correct = 0;
  for(int i = 0; i < arraySize; i++)
  {
      if(output[i] >= 0)
      {
          correct++;
          /*if(i==0)
          {
             printf("%d",output[i]);
          }
          else
          {
             printf(",%d",output[i]);
          }*/
      }
  }
  printf("\n");
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
        case 'p':
            pN=num;
            break;
        default :
            break;
        }
    }
    if(!sizeSet || !repSet || arraySize<=0 || rep<=0){
        printf("Invalid arguments, expected 2 or 3 args:\n");
        printf("r - number of repititions greater than 0(to increase runtime)\n");
        printf("p - number of processors\n");
        printf("s - random seed (optional leave out to seed by time)\n");
        printf("Example execution ./openCL-allToOne r3 n524288 s3 p4 > output.txt\n");
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