#include <curses.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define BOMB  1
#define MARKED  2

int SIZE;
int NUMMINES;

void printDone(char grid[][SIZE]); // prints the desired grid
void printGrid(char grid[][SIZE]); // prints the desired grid

// functions for creating the grids
void chooseSize(); // user selects the SIZE of the game
void chooseMines(); // user selects number of mines
void setPlayer(char playerGrid[][SIZE]); // initialize player view grid
void setKey(char keyGrid[][SIZE]); // set key grid
void addMine(char keyGrid[][SIZE]); // adds a mine to key grid
void fillKey(char keyGrid[][SIZE], int row, int col); // fills in number of adjacent mines

// functions for playing minesweeper
void markSpace(char playerGrid[][SIZE], int *marked, int mousex, int mousey); // mark the chosen space as a bomb
void revealSpace(char playerGrid[][SIZE], char keyGrid[][SIZE], int *moves, bool *lost, int mousex, int mousey); // reveal the chosen space
void revealBombs(char playerGrid[][SIZE], char keyGrid[][SIZE]); // reveals all bombs and the game is over
void Reveal(char playerGrid[][SIZE], char keyGrid[][SIZE], int *moves, bool *lost, int x, int y); // reveals the chosen space


int main()
{
  int x;
  int y;
  int ch;

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

  int moves = ((SIZE*SIZE) - NUMMINES);
  bool lost = false;
  int marked = 0;

  initscr();
  start_color();

  init_pair(BOMB, COLOR_RED, COLOR_BLACK);
  init_pair(MARKED, COLOR_CYAN, COLOR_BLACK);

  printGrid(playerGrid);

  keypad(stdscr, TRUE);

  mousemask(BUTTON1_CLICKED | BUTTON3_CLICKED, NULL);

  cbreak();
  noecho();

  clear();

  printGrid(playerGrid);
  printw("\nTo reveal a space, left click.\nTo mark a bomb, right click.\n");

  do {
    refresh();
    ch = wgetch(stdscr);

    if (ch == KEY_MOUSE)
    {
      MEVENT event;
      if (getmouse(&event) == OK)
      {
        int mousex = event.x;
        int mousey = event.y;

        switch (event.bstate)
        {
          case BUTTON1_CLICKED:
          {
            revealSpace(playerGrid, keyGrid, &moves, &lost, mousex, mousey);
            clear();
            printGrid(playerGrid);
            printw("\nTo reveal a space, left click.\nTo mark a bomb, right click.\n");
          } break;
          case BUTTON3_CLICKED:
          {
            markSpace(playerGrid, &marked, mousex, mousey);
            clear();
            printGrid(playerGrid);
            printw("\nTo reveal a space, left click.\nTo mark a bomb, right click.\n");
          } break;
          default:
            break;
        }
      }
    }
  } while(((moves > 0) || (marked != NUMMINES)) && (lost == false));


  endwin();

  system("clear");
  printDone(playerGrid);

  if (lost == false)
    printf("\nCongratulations! You Won!\n");
  else
    printf("\nSorry, you lost.\n");

  return EXIT_SUCCESS;


}



void printGrid(char grid[][SIZE])
{
  printw("\n    ");
  for (int i= 0;  i < SIZE; i++)
    printw("%-3d", i);
  printw("\n\n");
  for (int j = 0; j < SIZE; j++)
  {
    printw("%-4d", j);
    for (int k = 0; k < SIZE; k++)
    {
      if (grid[j][k] == '#')
      {
        attron(COLOR_PAIR(MARKED));
        printw("%-3c", grid[j][k]);
        attroff(COLOR_PAIR(MARKED));

      }
      else if (grid[j][k] == '*')
      {
        attron(COLOR_PAIR(BOMB));
        printw("%-3c", grid[j][k]);
        attroff(COLOR_PAIR(BOMB));
      }
      else
        printw("%-3c", grid[j][k]);
    }
    printw("\n");
  }

  return;
}



void printDone(char grid[][SIZE])
{
  printf("\n    ");
  for (int i= 0;  i < SIZE; i++)
    printf("%-3d", i);
  printf("\n\n");
  for (int j = 0; j < SIZE; j++)
  {
    printf("%-4d", j);
    for (int k = 0; k < SIZE; k++)
    {
      if (grid[j][k] == '#')
      {
        printf("\033[0;36m");
        printf("%-3c", grid[j][k]);
        printf("\033[0m");

      }
      else if (grid[j][k] == '*')
      {
        printf("\033[0;31m");
        printf("%-3c", grid[j][k]);
        printf("\033[0m");

      }
      else
        printf("%-3c", grid[j][k]);
    }
    printf("\n");
  }

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
  printf("Enter size of grid as a single integer that will\nrepresent the dimension of each side\n");
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

////////////////////////////////////////// functions for playing minesweeper


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



void revealSpace(char playerGrid[][SIZE], char keyGrid[][SIZE], int *moves, bool *lost, int mousey, int mousex)
{
  int i = (mousex - 3);
  int j = (mousey - 4) / 3;


  if (playerGrid[i][j] == '#')
  {
    printw("You can't reveal a space that has been marked as a bomb.\n");
    return;
  }

  Reveal(playerGrid, keyGrid, moves, lost, i, j);

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

  if (playerGrid[x][y] == '-')
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



void markSpace(char playerGrid[][SIZE], int *marked, int mousey, int mousex)
{
  int x = (mousex - 3);
  int y = (mousey - 4) / 3;

  if (playerGrid[x][y] == '-')
  {
    playerGrid[x][y] = '#';
    (*marked)++;
    return;
  }
  else if (playerGrid[x][y] == '#')
  {
    playerGrid[x][y] = '-';
    (*marked)--;
    return;
  }

  return;
}
