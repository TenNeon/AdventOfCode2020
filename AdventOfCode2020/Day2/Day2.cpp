
#include "Helper.h"
using namespace Helper;

#include <algorithm>

struct Password
{
  char letter;
  int min;
  int max;
  std::string password;

  // 13-15 c: cqbhncccjsncqcc
  Password(std::string raw)
  {
    // find min, max
    int dash = raw.find_first_of('-', 0);
    int colon = raw.find_first_of(':', 0);
    int space = raw.find_first_of(' ', 0);
    min = std::stoi( raw.substr(0, dash) );
    max = std::stoi( raw.substr(dash+1, space-dash) );
    letter = raw.substr(space + 1, 1).c_str()[0];
    password = raw.substr(colon+2);
  }

};

int Count(std::string str, char c)
{
  int index = 0;
  int count = 0;
  while (index != str.size())
  {
    if (str[index++] == c)
    {
      count++;
    }
  }
  return count;
}

bool IsValidPassword(Password const& p)
{
  int first = p.min-1;
  int second = p.max-1;
  char c = p.letter;
  std::string const& pass = p.password;

  if ((int)pass.length() < first || (int)pass.length() < second)
  {
    return false;
  }

  if ((pass[first] == c && pass[second] != c) ||
      (pass[first] != c && pass[second] == c))
  {
    return true;
  }
  return false;
}

int Do2A(std::vector<Password> passwords)
{
  int goodPasswordCount = 0;

  for (size_t i = 0; i < passwords.size(); i++)
  {
    Password& p = passwords[i];
    int count = Count(p.password,p.letter);
    goodPasswordCount += count >= p.min && count <= p.max ? 1 : 0;
  }
  return goodPasswordCount;
}

int Do2B(std::vector<Password> passwords)
{
  int goodPasswordCount = 0;
  for (size_t i = 0; i < passwords.size(); i++)
  {
    goodPasswordCount += IsValidPassword(passwords[i]) ? 1 : 0;
  }
  return goodPasswordCount;
}

int main()
{
  std::string dataFileName = "Data/input.txt";
  std::string outFileName = "Data/Out.txt";
  std::vector<std::string> lines_str = ParseFile(dataFileName);

  // 13-15 c: cqbhncccjsncqcc
  std::vector<Password> passwords;
  std::transform(lines_str.begin(), lines_str.end(), std::back_inserter(passwords), [](std::string a) { return Password(a); });

  std::vector<std::string> lines_out;
  lines_out.push_back(std::to_string(Do2A(passwords)));
  lines_out.push_back(std::to_string(Do2B(passwords)));


  WriteToFile(lines_out, outFileName);

}