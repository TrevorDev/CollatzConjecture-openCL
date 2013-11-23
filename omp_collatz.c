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
bN: width of board >1, so board size is N x N cells
tN: width of one overlay tile, each N x N cells (b mod t=0)
cN: max. colour density in integer percent, 1-100 (stopping condition)
mN: max. no. of full steps (additional stopping condition)
sN: optional random seed
iMode: optional interactive mode switch 1 - on / 0 - off
*/

int pN, sN;
int *data;

int main(int argc, char *argv[])
{
  int i;
  double start, stop;

  int size, counter;
  int nthreads, tid;

  if (argc == 1)
  {
    printf("please provide correct arguments (p for process s for seed)\n");
    printf("eg: pN sN aN        (Replace N with number)\n");
    exit(0);
  }

  for (i = 1; i < argc; i++)
  {
    if (argv[i][0] == 'p')
    {
      pN = atoi(++argv[i]);
      printf("pN %d\n",pN);
      continue;
    }

    if (argv[i][0] == 's')
    {
      sN = atoi(++argv[i]);
      printf("sN %d\n",sN);
      continue;
    }

    if (argv[i][0] == 'a')
    {
      size = atoi(++argv[i]);
      printf("size %d\n",size);
      continue;
    }
  }

  start = omp_get_wtime();
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

/*  #pragma omp parallel private(nthreads, tid)
  {
    tid = omp_get_thread_num();
    printf("Hello World from thread = %d\n", tid);

    if (tid == 0) 
    {
      nthreads = omp_get_num_threads();
      printf("Number of threads = %d\n", nthreads);
    }
  }*/  /* All threads join master thread and disband */

/*
  do
  {
    // red movement
    #pragma omp parallel for collapse(2) private(i, j) firstprivate(bN) shared(board, tempBoard)
    for (i = 0; i < bN; i++)
    {
      for (j = 0; j < bN; j++)
      {
        if (board[i][j] == 2)
        {
          tempBoard[i][j] = 2;
        }
        else if (board[i][j] == 1 && board[i][(j+1)%bN] == 0)
        {
          tempBoard[i][(j+1)%bN] = 1;
        }
        else if (board[i][j] == 1)
        {
          tempBoard[i][j] = 1;
        }
      }
    }

    #pragma omp parallel for collapse(2) private(i, j) firstprivate(bN) shared(board)
    for (i = 0; i < bN; i++)
    {
      for (j = 0; j < bN; j++)
      {
        board[i][j] = 0;
      }
    }

    // blue movement 
    #pragma omp parallel for collapse(2) private(i, j) firstprivate(bN) shared(board, tempBoard)
    for (i = 0; i < bN; i++)
    {
      for (j = 0; j < bN; j++)
      {
        if (tempBoard[i][j] == 1)
        {
          board[i][j] = 1;
        }
        else if (tempBoard[i][j] == 2 && tempBoard[(i+1)%bN][j] == 0)
        {
          board[(i+1)%bN][j] = 2;
        }
        else if (tempBoard[i][j] == 2)
        {
          board[i][j] = 2;
        }
      }
    }

    #pragma omp parallel for collapse(2) private(i, j) firstprivate(bN) shared(tempBoard)
    for (i = 0; i < bN; i++)
    {
      for (j = 0; j < bN; j++)
      {
        tempBoard[i][j] = 0;
      }
    }

    #pragma omp parallel for private(i, j, k, myPercent, rCount, bCount, myRow, myCol) firstprivate(tN) shared(board, percent, limit)
    for (i = 0; i < numOfBoard; i++)
    {
      int localLimit = 0;
      if (localLimit == 0)
      {
        myPercent = 0;
        rCount = 0;
        bCount = 0;
        myRow = i / (bN / tN) * tN;
        myCol = (i * tN) % bN;
      
        for (j = myRow; j < myRow+tN; j++)
        {
          for (k = myCol; k < myCol+tN; k++)
          {
            if (board[j][k] == 1)
              rCount++;
            else if (board[j][k] == 2)
              bCount++;
          }
        }
      
        if (rCount >= bCount)
        {
          myPercent = (int) ((float) rCount / (tN*tN) * 100);
        }
        else
        {
          myPercent = (int) ((float) bCount / (tN*tN) * 100);
        }
      
        if (myPercent >= cN)
        {
          percent = myPercent;
          localLimit = 1;
          limit = localLimit;
        }
      }
    }
    steps++;
  } while (steps < mN && limit == 0);
*/
  // writing
  /*
  for (i = 1; i < argc; i++)
    printf("%s ",--argv[i]);
  printf(" ");*/
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