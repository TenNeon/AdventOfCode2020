// Day1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
//#include <istream>
#include <ostream> // ostream
#include <fstream> // istream
#include <string> // getline

#include <algorithm>

std::vector<std::string> ParseFile(std::string filename)
{
  std::ifstream inFile(filename);
  std::string line;
  std::vector<std::string> lines;
  if (inFile.is_open())
  {
    while (std::getline(inFile,line))
    {
      lines.push_back(line);
    }
  }
  else
  {
    std::cout << "Can't open" << filename << std::endl;
  }

  return lines;
}

void WriteToFile(std::vector<std::string> data, std::string filename)
{
  std::ofstream outFile;
  outFile.open(filename, std::ios::out);

  if (outFile.is_open())
  {
    for (size_t i = 0; i < data.size(); i++)
    {
      outFile << data[i] << std::endl;
    }
  }
  else
  {
    std::cout << "Can't open" << filename << std::endl;
  }
}

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
  for (size_t i = 0; i < first_large; i++)
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
  //std::transform(lines_int.begin(), lines_int.end(), std::back_inserter(lines_out), [](int a) {return std::to_string(a); });

  WriteToFile(lines_out, outFileName);
}
