#pragma once

#include <iostream>
#include <vector>
#include <ostream> // ostream
#include <fstream> // istream
#include <string>  // getline
#include <map>     // map

#ifdef _DEBUG
#define DEBUG(x) std::cerr << x << std::endl //!< DEBUG print
#else
#define DEBUG(x)
#endif
#define PRINT(x) std::cout << x << std::endl;

namespace Helper
{
  typedef std::vector < std::vector<std::string>> vec_vec_str;
  typedef std::string str;
  typedef std::vector<std::string> vec_str;
  typedef std::map<std::string, std::string> map_str_str;

  std::vector<std::string> ParseFile(std::string filename)
  {
    std::ifstream inFile(filename);
    std::string line;
    std::vector<std::string> lines;
    if (inFile.is_open())
    {
      while (std::getline(inFile, line))
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

  template <class T>
  bool Between(T const& value, T const& min, T const& max)
  {
    return (value >= min && value <= max);
  }


}