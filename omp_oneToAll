#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

void InitializeData(int size);
void printData(int size);
void writeToFile();
int collatz(int number);

int steps = 0, limit = 0, percent = 0;

/*
pN: no. of processors (threads) to use
sN: optional random seed
aN: size of array

*/

int pN, sN;
int *data;

int main(int argc, char *argv[])
{
  int i;
  double start, stop;
  int size, counter, findNum;
  int nthreads, tid;
  int position;
  findNum = 0;
  size = 0;
  if (argc == 1)
  {
    printf("please provide correct arguments\n");
    printf("'p' for process, 'f' for aiming number to find, 's' for seed, 'a' for array size\n");
    printf("eg: pN fN or (aN and sN) (Replace N with number)\n");
    exit(0);
  }

  for (i = 1; i < argc; i++)
  {
    if (argv[i][0] == 'p')
    {
      pN = atoi(++argv[i]);
      continue;
    }

    if (argv[i][0] == 'f')
    {
      findNum = atoi(++argv[i]);
      continue;
    }

    if (argv[i][0] == 's')
    {
      sN = atoi(++argv[i]);
      continue;
    }

    if (argv[i][0] == 'a')
    {
      size = atoi(++argv[i]);
      continue;
    }
  }
  start = omp_get_wtime();
  size = 1024;
  data = (int *) malloc (size*sizeof(int));
  data[0] = 2;
  position = 1;
  for (i = 0; i < size; i++)
  {
    if (data[i] != 0)
    {
      if(data[i] % 2 == 0) 
      {
        data[position++] = data[i]*2;
      }
      if(((data[i] - 1) / 3 ) % 2 != 0)
      {
        data[position++] = (data[i] - 1) / 3;
      }
    }
  }
  for (i = 0; i < size; i++)
  {
    printf("%d ",data[i]);
  }
  printf("\n%d\n",position);

  /*
  if (findNum == 0)
  {
    InitializeData(size);
    printData(size);
    omp_set_num_threads(pN);
    
    #pragma omp parallel for private(i, counter)
    for (i = 0; i < size; i++)
    {
      counter=0;
      while(data[i] != 1)
      {
        counter++;
        if(data[i] % 2 == 0) 
        {
          data[i] = data[i] / 2;
        }
        else 
        {
          data[i] = (data[i] * 3) + 1;
        }
        if (counter > 25)
        {
          data[i] = 0;
          break;
        } 
      }
    }

    printData(size);
  }
  else
  {
    size = 1024;
    data = (int *) malloc (size*sizeof(int));
    data[0] = 2;
    position = 1;
    for (i = 0; i < size; i++)
    {
      if(data[i] % 2 == 0) 
      {
        data[position++] = data[i]*2;
      }
      else
      {
        data[position++] = (data[i] - 1) / 3;
      }
    }

  }
  */
  // writing
  
  for (i = 1; i < argc; i++)
    printf("%s ",--argv[i]);
  printf(" ");
  stop = omp_get_wtime();
  //printf("Iterations: %d Termination Percent: %d Time: %f\n", steps, percent, (double)(stop - start));
  //writeToFile();
  return 0;
}

void InitializeData(int size)
{
  int i, r;
  data = (int *) malloc (size*sizeof(int));
  
  if (sN != -1)
  {
    srand(sN);
  }
  else
  {
    srand((unsigned)time(NULL));
  }

  for (i = 0; i < size; i++)
  {
      r = rand()%10000;
      data[i] = r;
  }
}

void printData(int size)
{
  int i;

  for (i = 0; i < size; i++)
  {
    printf("%d ", data[i]);
  }
  printf("\n");
}

int collatz(int number)
{
  int counter=0;
  while(number!=1)
  {
    counter++;
    if(number%2 == 0) 
    {
      number = number / 2;
    }
    else 
    {
      number = (number*3) + 1;
    }
    if (counter > 25)
    {
      counter = 0;
      break;
    } 
  }

  return counter;
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