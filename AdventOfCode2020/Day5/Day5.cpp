// Day5.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "Helper.h"
using namespace Helper;

#include <algorithm> // transform

#define rowCount 128
#define columnCount 8
#define rowInfoLength 7
#define columnInfoLength 3

class SeatNumber
{
public:
  int GetRow() const { return row; }
  int GetColumn() const { return column; }
  int GetID() const { return ID; }

  SeatNumber(std::string const& raw)
  {
    int rowSectionMin = 0;
    int rowSectionMax = rowCount - 1;

    int columnSectionMin = 0;
    int columnSectionMax = columnCount - 1;

    // Find row
    int rowSectionSize = rowCount / 2;
    for (size_t i = 0; i < rowInfoLength; i++)
    {
      char currentChar = raw[i];
      if (currentChar == 'F') // lower half
      {
        rowSectionMax -= rowSectionSize;
      }
      else if (currentChar == 'B') // upper half
      {
        rowSectionMin += rowSectionSize;
      }
      else { PRINT(currentChar << ": bad entry?"); continue; }

      rowSectionSize /= 2;
    }
    if (rowSectionMin == rowSectionMax)
    {
      row = rowSectionMin;
    }
    else
    {
      PRINT("Couldn't resolve column");
    }


    // Find column
    int columnSectionSize = columnCount / 2;
    for (size_t i = rowInfoLength; i < raw.size(); i++)
    {
      char currentChar = raw[i];
      if (currentChar == 'L') // left half
      {
        columnSectionMax -= columnSectionSize;
      }
      else if (currentChar == 'R') // right half
      {
        columnSectionMin += columnSectionSize;
      }
      else { PRINT(currentChar << ": bad entry?"); continue; }

      columnSectionSize /= 2;
    }
    if (columnSectionMin == columnSectionMax)
    {
      column = columnSectionMin;
    }
    else
    {
      PRINT("Couldn't resolve column");
    }

    ID = (8 * row) + column;
    //PRINT(raw << ": row " << row << ", column " << column << ", seat ID " << ID);
  }
private:
  int row = -1;
  int column = -1;
  int ID = -1; // 8*row + column

};

int Do5B(std::vector<SeatNumber> const& seats)
{
  for (size_t i = 0; i < seats.size()-1; i++)
  {
    int thisID = seats[i].GetID();
    int nextID = seats[i+1].GetID();
    if (nextID > thisID+1)
    {
      return thisID + 1;
    }
  }
  return -1;
}

int main()
{
  PRINT("Day 5");
  std::string dataFileName = "Data/input.txt";
  std::string outFileName = "Data/Out.txt";
  std::vector<std::string> lines_str = ParseFile(dataFileName);

  // Data to SeatNumbers
  std::vector<SeatNumber> seats;
  std::transform(lines_str.begin(), lines_str.end(), std::back_inserter(seats),
    [](str a) { return SeatNumber(a); });

  std::sort(seats.begin(), seats.end(), [](SeatNumber a, SeatNumber b) { return a.GetID() < b.GetID(); });

  int max = seats.back().GetID(); // part A

  int mySeat = Do5B(seats);

  std::vector<std::string> lines_out;
  lines_out.push_back(std::to_string(max));
  PRINT(lines_out[0]);
  lines_out.push_back(std::to_string(mySeat));
  PRINT(lines_out[1]);

  WriteToFile(lines_out, outFileName);
}