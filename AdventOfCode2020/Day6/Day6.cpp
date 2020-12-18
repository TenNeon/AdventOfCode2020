#include "Helper.h"
using namespace Helper;

#include <algorithm> // transform

typedef vec_str Party;

vec_str ConcatLines(vec_str const& lines)
{
  vec_str result;
  str line;
  for (size_t i = 0; i < lines.size(); i++)
  {
    if (lines[i] == "")
    {
      result.push_back(line);
      line.clear();
    }
    else
    {
      line += lines[i];
    }
  }
  if (line.length() > 0)
  {
    result.push_back(line);
  }

  return result;
}

std::map<char, int> ToCountingMap(str const& char_string)
{
  std::map<char, int> result;

  for (size_t i = 0; i < char_string.size(); i++)
  {
    result[char_string[i]] += 1;
  }

  return result;
}

int Do6A(vec_str const& parties)
{
  int result = 0;
  // convert each party into a counting map
  for (size_t i = 0; i < parties.size(); i++)
  {
    str const& party = parties[i];

    std::map<char, int> countingMap = ToCountingMap(party);
    int size = countingMap.size();
    result += countingMap.size();
  }

  return result;
}

int CountCommonValues(Party const& party)
{
  int result = 0;
  int count = party.size();

  str concat_party = *ConcatLines(party).begin();

  std::map<char, int> countingMap = ToCountingMap(concat_party);

  for (std::map<char, int>::iterator itr = countingMap.begin(); 
    itr != countingMap.end(); itr++)
  {
    result += itr->second == count ? 1 : 0;
  }

  return result;
}

int Do6B(vec_str const& parties)
{

  Party party;
  int result = 0;
  for (size_t i = 0; i < parties.size(); i++)
  {
    if (parties[i] == "")
    {
      // Process party
      result += CountCommonValues(party);
      party.clear();
    }
    else
    {
      party.push_back( parties[i] );
    }
  }
  if (party.size() > 0) // handle last element
  {
    result += CountCommonValues(party);
  }
  return result;
}

int main()
{
  PRINT("Day 6");
  std::string dataFileName = "Data/input.txt";
  std::string outFileName = "Data/Out.txt";
  std::vector<std::string> lines_str = ParseFile(dataFileName);

  vec_str parties = ConcatLines(lines_str);
  int Result6A = Do6A(parties);
  int Result6B = Do6B(lines_str);

  std::vector<std::string> lines_out;
  lines_out.push_back(std::to_string(Result6A));
  PRINT(lines_out[0]);
  lines_out.push_back(std::to_string(Result6B));
  PRINT(lines_out[1]);

  WriteToFile(lines_out, outFileName);
}