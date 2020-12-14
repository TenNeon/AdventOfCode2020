// Day1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <vector>
#include <algorithm> // transform

#include "Helper.h"
using namespace Helper;

//#include "Helper.h"
//using namespace Helper;

int Do1A(std::vector<int> data)
{
  std::sort(data.begin(), data.end());

  int smallest = data[0];
  int first_large = data.back();

  // Find the second half of the data
  for (size_t i = 0; i < data.size(); i++)
  {
    if (data[i] + smallest >= 2020)
    {
      first_large = data[i];
      break;
    }
  }

  // Compare the first half to the second half
  for (int i = 0; i < first_large; i++)
  {
    for (size_t j = 0; j < data.size(); j++)
    {
      if (data[i] + data[j] == 2020)
      {
        return data[i] * data[j];
      }
      if (data[i] + data[j] > 2020)
      {
        break;
      }
    }
  }

  return 0;
}

int Do1B(std::vector<int> data)
{
  for (size_t i = 0; i < data.size()-1; i++)
  {
    for (size_t j = 1; j < data.size()-2; j++)
    {
      if (i != j)
      {
        int A = data[i];
        int B = data[j];
        int sumAB = A + B;
        if (sumAB < 2020)
        {
          for (size_t k = 2; k < data.size() - 3; k++)
          {
            if (j != k)
            {
              int C = data[k];
              int sum = A + B + C;
              if (sum == 2020)
              {
                return A * B * C;
              }
            }
          }
        }
      }
    }
  }
  return 0;
}

int main()
{
  std::string dataFileName = "Data/Data.txt";
  std::string outFileName = "Data/Out_A.txt";
  std::vector<std::string> lines_str = ParseFile(dataFileName);

  // Data to ints
  std::vector<int> lines_int;
  std::transform(lines_str.begin(), lines_str.end(), std::back_inserter(lines_int), [](std::string a) { return std::stoi(a); });


  // Data to strings
  std::vector<std::string> lines_out;
  lines_out.push_back(std::to_string(Do1A(lines_int)));
  lines_out.push_back(std::to_string(Do1B(lines_int)));
  //std::transform(lines_int.begin(), lines_int.end(), std::back_inserter(lines_out), [](int a) {return std::to_string(a); });

  WriteToFile(lines_out, outFileName);
}
