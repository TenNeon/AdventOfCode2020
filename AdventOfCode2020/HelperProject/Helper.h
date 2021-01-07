#pragma once

#include <iostream>
#include <vector>
#include <ostream> // ostream
#include <fstream> // istream
#include <string>  // getline
#include <map>     // map
#include <regex>   // sregex_token_iterator 
#include <numeric> // limits


#ifdef _DEBUG
#define DEBUG(x) std::cerr << x << std::endl //!< DEBUG print
#else
#define DEBUG(x)
#endif
#define PRINT(x) std::cout << x << std::endl;

constexpr float PI = 3.14159f;
constexpr float HALF_PI = 1.57079632679f;

namespace Helper
{
  typedef std::vector < std::vector<std::string>> vec_vec_str;
  typedef std::string str;
  typedef std::vector<std::string> vec_str;
  typedef std::vector<int> vec_int;
  typedef long long unsigned ll_int;
  typedef std::map<std::string, std::string> map_str_str;
  
  struct vec2
  {
    float x = 0;
    float y = 0;

    vec2(float a = 0, float b = 0) : x(a), y(b) {}

    vec2 operator+(vec2 const& other) const { return vec2(x+other.x, y+other.y); }
    vec2 operator-(vec2 const& other) const { return vec2(x-other.x, y-other.y); }
    vec2 operator+=(vec2 const& other) { x += other.x; y += other.y; return *this; }
    vec2 operator-=(vec2 const& other) { x -= other.x; y -= other.y; return *this; }
    bool operator==(vec2 const& other) const { return other.x == x && other.y == y; }
    vec2 operator*(vec2 const& other) const { return vec2(x * other.x, y * other.y); }
    vec2 operator*(int scale) const { return vec2(x * scale, y * scale); }

    float Length() const
    {
      return std::sqrt(x*x + y*y);
    }

    vec2& Normalize()
    {
      float len = Length();
      if (len != 0)
      {
        x /= len;
        y /= len;
      }
      else
      {
        x = 0;
        y = 0;
      }
      return *this;
    }

    vec2 Normalized() const
    {
      float len = Length();
      return vec2(x / len, y / len);
    }
  };

  vec2 operator*(int scale, vec2 const& a) { return vec2(a.x * scale, a.y * scale); }

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

  // Simple strtok wrapper
  vec_str Tokenize(str string, const std::regex regex = std::regex(R"([\s|,]+)")) // defaults to space and comma
  {
    std::sregex_token_iterator itr{ string.begin(), string.end(), regex, -1 };
    std::vector<std::string> tokenized{ itr, {} };
    return tokenized;
  }


  typedef str AoCPartFunc(vec_str const&);
  void AoCDay(int day, AoCPartFunc a, AoCPartFunc b, str inputFileName = "Data/input.txt")
  {
    PRINT("Day " << day);
    str outFileName = "Data/Out.txt";
    vec_str lines_str = ParseFile(inputFileName);
    std::vector<std::string> lines_out;

    lines_out.push_back(a(lines_str));
    PRINT("A: " << lines_out[0]);

    lines_out.push_back(b(lines_str));
    PRINT("B: " << lines_out[1]);

    WriteToFile(lines_out, outFileName);
  }
}