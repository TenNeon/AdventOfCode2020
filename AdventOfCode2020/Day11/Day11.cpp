#include "Helper.h"
using namespace Helper;

#include <algorithm> // transform

typedef std::vector<char> vec_char;
typedef std::vector<vec_char> chair_grid;
constexpr char OCCUPIED = '#';
constexpr char FREE = 'L';
constexpr char EMPTY = '.';

void PrintGrid(chair_grid const& grid)
{
  std::cout << std::endl;
  for (int i = 0; i < (int)grid.size(); i++)
  {
    for (int j = 0; j < (int)grid[0].size(); j++)
    {
      std::cout << grid[i][j];
    }
    std::cout << std::endl;
  }
  std::cout << std::endl;
}

chair_grid ParseInput(vec_str const& raw)
{
  chair_grid result;
  for (size_t i = 0; i < raw.size(); i++) // rows
  {
    vec_char row = vec_char(raw[i].begin(), raw[i].end());
    if (row.size() > 0)
    {
      result.push_back(row);
    }
  }
  return result;
}

bool isZero(int x, int y) { return (x == 0 && y == 0); }
bool isInBounds(chair_grid const& grid, int x, int y){return x >= 0 && x < (int)grid.size() && y >= 0 && y < (int)grid[x].size();}
bool isOccupied(chair_grid const& grid, int x, int y) { return grid[x][y] == OCCUPIED; }
bool isChair(chair_grid const& grid, int x, int y) { return grid[x][y] == OCCUPIED || grid[x][y] == FREE; }

bool OccupiedInDirection(chair_grid const& grid, int x, int y, int dx, int dy)
{
  if (isZero(dx,dy)) return false; // don't count self

  // step in direction until a seat is found or an edge is found
  // if seat return isOccupied
  // else return false;

  x += dx;
  y += dy;

  while(isInBounds(grid, x, y))
  {
    if (isChair(grid, x, y)) return isOccupied(grid, x, y);
    x += dx;
    y += dy;
  }
  return false;
}

typedef int NeighborCountFunc(chair_grid const&, int, int);

int NumNeighborsInSight(chair_grid const& grid, int x, int y)
{
  int count = 0;
  for (int i = -1; i < 2; i++) // rows
  {
    for (int j = -1; j < 2; j++) // cols
    {
      bool occupied = OccupiedInDirection(grid, x, y, i, j);
      count += occupied ? 1 : 0;
    }
  }
  return count;
}

// count of occupied adjacent seats
int NumNeighbors(chair_grid const& grid, int x, int y)
{
  int count = 0;
  for (int i = -1; i < 2; i++) // rows
  {
    for (int j = -1; j < 2; j++) // cols
    {
      int X = x + i;
      int Y = y + j;
      if (isInBounds(grid,X,Y)
       && !isZero(i,j)
       && isOccupied(grid, X, Y))
      {
        count++;
      }
    }
  }
  return count;
}

void StepSeat(char seat, const chair_grid& grid, int i, int j, char& newSeat, int exitThreshold, NeighborCountFunc CountNeighbors)
{
  if (seat != EMPTY)
  {
    int neighbors = CountNeighbors(grid, i, j);

    if (seat == FREE && neighbors == 0)
    {
      newSeat = OCCUPIED;
    }
    else if (seat == OCCUPIED && neighbors >= exitThreshold)
    {
      newSeat = FREE;
    }
    //std::cout << std::to_string(neighbors).c_str()[0];
  }
  //else
  //{
  //  std::cout << ".";
  //}
}

/*
    If a seat is empty (L) and there are no occupied seats adjacent to it, the seat becomes occupied.
    If a seat is occupied (#) and four or more seats adjacent to it are also occupied, the seat becomes empty.
    Otherwise, the seat's state does not change.
*/
chair_grid GetNextStep(chair_grid const& grid, int exitThreshold, NeighborCountFunc CountNeighbors)
{
  chair_grid result;

  for (int i = 0; i < (int)grid.size(); i++)
  {
    vec_char row;
    for (int j = 0; j < (int)grid[i].size(); j++)
    {
      char seat = grid[i][j];
      char newSeat = seat;
      StepSeat(seat, grid, i, j, newSeat, exitThreshold, CountNeighbors);
      row.push_back(newSeat);
    }
    //std::cout << std::endl;
    result.push_back(row);
  }
  return result;
}

int CountOccupied(chair_grid grid)
{
  int count = 0;
  for (int i = 0; i < (int)grid.size(); i++)
  {
    for (int j = 0; j < (int)grid[i].size(); j++)
    {
      count += grid[i][j] == OCCUPIED ? 1 : 0;
    }
  }
  return count;
}

int RunUntilStable(chair_grid grid, int exitThreshold = 4, NeighborCountFunc CountNeighbors = NumNeighbors)
{
  chair_grid oldGrid;
  chair_grid newGrid = grid;
  //PrintGrid(newGrid);
  int step = 0;
  do
  {
    PRINT("Step " << ++step);
    oldGrid = newGrid;
    newGrid = GetNextStep(oldGrid, exitThreshold, CountNeighbors);
    //PrintGrid(newGrid);
  } while (newGrid != oldGrid);
  PRINT("Stable");

  return CountOccupied(newGrid);
}

str DoPartA(vec_str const& input_raw)
{
  //return "skip";
  chair_grid grid = ParseInput(input_raw);
  int occupied = RunUntilStable(grid);
  return std::to_string(occupied);
}

str DoPartB(vec_str const& input_raw)
{
  //return "skip";

  chair_grid grid = ParseInput(input_raw);
  int occupied = RunUntilStable(grid, 5, NumNeighborsInSight);
  return std::to_string(occupied);
}
int main()
{
  //AoCDay(11, DoPartA, DoPartB, "Data/test.txt");
  AoCDay(11, DoPartA, DoPartB, "Data/input.txt");
}