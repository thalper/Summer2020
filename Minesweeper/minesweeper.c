#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>

int SIZE;
int NUMMINES;

// functions for creating the grids
void chooseSize(); // user selects the SIZE of the game
void chooseMines(); // user selects number of mines
void setPlayer(char playerGrid[][SIZE]); // initialize player view grid
void setKey(char keyGrid[][SIZE]); // set key grid
void addMine(char keyGrid[][SIZE]); // adds a mine to key grid
void fillKey(char keyGrid[][SIZE], int row, int col); // fills in number of adjacent mines

// functions for playing minesweeper
void play(char playerGrid[][SIZE], char keyGrid[][SIZE]); // driver for playing the game
void chooseSpace(int *x, int *y); // choose space to modify
void markSpace(char playerGrid[][SIZE], int *marked); // mark the chosen space as a bomb
void revealSpace(char playerGrid[][SIZE], char keyGrid[][SIZE], int *moves, bool *lost); // reveal the chosen space
void revealBombs(char playerGrid[][SIZE], char keyGrid[][SIZE]); // reveals all bombs and the game is over
void Reveal(char playerGrid[][SIZE], char keyGrid[][SIZE], int *moves, bool *lost, int x, int y); // reveals the chosen space




void printGrid(char grid[][SIZE]); // prints the desired grid

////////////////// Main
int main()
{
  srand(time(NULL));

  system("clear");
  chooseSize();
  int temp = SIZE;
  while (temp <= 1)
  {
    printf("The SIZE of the grid must be an integer larger than 1.\n\n");
    chooseSize();
  }

  chooseMines();
  while(NUMMINES > (SIZE * SIZE) / 2)
  {
    printf("The number of mines must be less than or equal to half the number of total spaces.\n\n");
    chooseMines();
  }

  char playerGrid[SIZE][SIZE];
  char keyGrid[SIZE][SIZE];

  setPlayer(playerGrid);
  setKey(keyGrid);

  play(playerGrid, keyGrid);

  return EXIT_SUCCESS;
}


void printGrid(char grid[][SIZE])
{
  printf("\n   ");
  for (int i= 0;  i < SIZE; i++)
    printf("%-2d", i);
  printf("\n\n");
  for (int j = 0; j < SIZE; j++)
  {
    printf("%-3d", j);
    for (int k = 0; k < SIZE; k++)
    {
      if (grid[j][k] == '#')
      {
        printf("\033[0;36m");
        printf("%-2c", grid[j][k]);
        printf("\033[0m");

      }
      else if (grid[j][k] == '*')
      {
        printf("\033[0;31m");
        printf("%-2c", grid[j][k]);
        printf("\033[0m");

      }
      else
        printf("%-2c", grid[j][k]);
    }
    printf("\n");
  }

  return;
}




////////////////////////////////////////// functions for playing minesweeper

void play(char playerGrid[][SIZE], char keyGrid[][SIZE])
{
  int moves = ((SIZE*SIZE) - NUMMINES);
  bool lost = false;
  char reveal = '.';
  int marked = 0;



  do {
    system("clear");
    printGrid(playerGrid);
    printf("\n");

    if (reveal != '0' && reveal != '1')
    {
      printf("\nTo reveal a space, enter '0'\nTo mark a bomb, enter '1'\n");
      scanf(" %c", &reveal);
    }
    if (reveal == '0')
    {
      revealSpace(playerGrid, keyGrid, &moves, &lost);
      reveal = '.';
    }
    else if (reveal == '1')
    {
      markSpace(playerGrid, &marked);
      reveal = '.';
    }
    else
    {
      while (reveal != '0' && reveal != '1')
      {
        printf("Please enter a '0' or a '1'\n");
        scanf(" %c", &reveal);
      }
    }
  } while(((moves > 0) || (marked != NUMMINES)) && (lost == false));

  system("clear");
  printGrid(playerGrid);

  if (lost == false)
    printf("\nCongratulations! You Won!\n");
  else
    printf("\nSorry, you lost.\n");

  return;
}




void revealBombs(char playerGrid[][SIZE], char keyGrid[][SIZE])
{
  for (int i = 0; i < SIZE; i++)
  {
    for (int j = 0; j < SIZE; j++)
    {
      if (keyGrid[i][j] == '*' && playerGrid[i][j] != '#')
        playerGrid[i][j] = '*';
    }
  }
}



void revealSpace(char playerGrid[][SIZE], char keyGrid[][SIZE], int *moves, bool *lost)
{
  int x;
  int y;
  chooseSpace(&x, &y);

  int i = x;
  int j = y;


  if (playerGrid[i][j] == '#')
  {
    printf("You can't reveal a space that has been marked as a bomb.\n");
    revealSpace(playerGrid, keyGrid, moves, lost);
    return;
  }

  Reveal(playerGrid, keyGrid, moves, lost, x, y);

  return;
}



void Reveal(char playerGrid[][SIZE], char keyGrid[][SIZE], int *moves, bool *lost, int x, int y)
{
  if (keyGrid[x][y] == '*')
  {
    revealBombs(playerGrid, keyGrid);
    *lost = true;
    return;
  }

  (*moves)--;

  playerGrid[x][y] = keyGrid[x][y];

  if (keyGrid[x][y] == '0')
  {
    for (int dx = -1; dx < 2; dx++)
    {
      for (int dy = -1; dy < 2; dy++)
      {
        if ((x+dx >= 0) && (x+dx < SIZE) && (y+dy >= 0) && (y+dy < SIZE))
        {
          if (playerGrid[x+dx][y+dy] == '-')
            Reveal(playerGrid, keyGrid, moves, lost, x+dx, y+dy);
        }
      }
    }
  }

  return;
}



void markSpace(char playerGrid[][SIZE], int *marked)
{
  int x;
  int y;
  chooseSpace(&x, &y);

  if (playerGrid[x][y] == '-')
    {
      playerGrid[x][y] = '#';
      (*marked)++;
      return;
    }

  playerGrid[x][y] = '-';
  (*marked)--;
  return;
}



void chooseSpace(int *x, int *y)
{
  printf("Select the row:\n");
  scanf(" %d", x);
  printf("Select the column:\n");
  scanf(" %d", y);

  return;
}

//////////////////////////////////////// Functions for building grids

void fillKey(char keyGrid[][SIZE], int row, int col)
{
  // Top-left
  if ((row > 0) && (col > 0))
  {
    if (keyGrid[row-1][col-1] != '*')
      keyGrid[row-1][col-1]++;
  }

  // Top-Middle
  if (row > 0)
  {
    if (keyGrid[row-1][col] != '*')
      keyGrid[row-1][col]++;
  }

  // Top-Right
  if ((row > 0) && (col < SIZE - 1))
  {
    if (keyGrid[row-1][col+1] != '*')
      keyGrid[row-1][col+1]++;
  }

  // Middle-left
  if (col > 0)
  {
    if (keyGrid[row][col-1] != '*')
      keyGrid[row][col-1]++;
  }

  // Middle-Right
  if (col < SIZE - 1)
  {
    if (keyGrid[row][col+1] != '*')
      keyGrid[row][col+1]++;
  }

  // Bottom-left
  if ((row < SIZE - 1) && (col > 0))
  {
    if (keyGrid[row+1][col-1] != '*')
      keyGrid[row+1][col-1]++;
  }

  // Bottom-Middle
  if (row < SIZE - 1)
  {
    if (keyGrid[row+1][col] != '*')
      keyGrid[row+1][col]++;
  }

  // Bottom-Right
  if ((row < SIZE - 1) && (col < SIZE - 1))
  {
    if (keyGrid[row+1][col+1] != '*')
      keyGrid[row+1][col+1]++;
  }

  return;
}




void addMine(char keyGrid[][SIZE])
{
  int numel = SIZE * SIZE;

  int location = rand() % (numel);
  int row = location / SIZE;
  int col = location % SIZE;

  if (keyGrid[row][col] == '*')
  {
    addMine(keyGrid);
  }
  else
  {
    keyGrid[row][col] = '*';
    fillKey(keyGrid, row, col);
  }
  return;
}



void setKey(char keyGrid[][SIZE])
{
  for (int i = 0; i < SIZE; i++)
  {
    for (int j = 0; j < SIZE; j++)
      keyGrid[i][j] = '0';
  }

  for (int k = 0; k < NUMMINES; k++)
    addMine(keyGrid);

}



void setPlayer(char playerGrid[][SIZE])
{
  for (int i = 0; i < SIZE; i++)
  {
    for (int j = 0; j < SIZE; j++)
    {
      playerGrid[i][j] = '-';
    }
  }

  return;
}




void chooseSize()
{
  printf("Enter size of grid as a single integer that will\nrepresent the dimention of each side\n");
  scanf(" %d", &SIZE);
  printf("\n");

  if (SIZE < 3)
  {
    printf("The grid must be at least 3 by 3.\n");
    chooseSize();
    return;
  }

  return;
}




void chooseMines()
{
  printf("Enter number of mines\n");
  scanf(" %d", &NUMMINES);
  printf("\n");

  if (NUMMINES < 1)
  {
    printf("There must be at least 1 mine.\n");
    chooseMines();
    return;
  }

  return;
}
