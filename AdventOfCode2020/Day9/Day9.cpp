#include "Helper.h"
using namespace Helper;

#include <algorithm> // transform
#include <list>
typedef long long unsigned ll_int;
typedef std::list<ll_int> list_int;


bool NumberIsValid(list_int::const_iterator begin, list_int::const_iterator end, int number)
{
  for (list_int::const_iterator itrA = begin; itrA != end; itrA++)
  {
    int a = *itrA;
    for (list_int::const_iterator itrB = begin; itrB != end; itrB++)
    {
      int b = *itrB;
      if (itrA != itrB && ((a + b) == number))
      {
        //PRINT(number << " is valid: " << a << " + " << b << " = " << number);
        return true;
      }
    }
  }
  //PRINT(number << " is invalid");
  return false;
}

str DoPartA(vec_str const& input_raw)
{
  list_int input;

  try{
    std::transform(input_raw.begin(), input_raw.end(), std::back_inserter(input), [](str a) { return std::stoll(a); });
  }
  catch (std::invalid_argument e) { PRINT("Couldn't parse int");  return "bad parse"; } // doesn't parse to int

  // Copy the preamble into recent
  int preamble_length = 25;
  list_int::iterator begin_recent = input.begin();
  list_int::iterator end_recent = input.begin();
  std::advance(end_recent, preamble_length);

  for (list_int::const_iterator itr = end_recent; itr != input.end(); itr++)
  {
    int newVal = *itr;
    if (NumberIsValid(begin_recent, end_recent, newVal))
    {
      begin_recent++;
      end_recent++;
    }
    else
    {
      return std::to_string(newVal);
    }
  }

  return "no answer";
}

str DoPartB(vec_str const& input)
{
  ll_int targetSum;
  try {targetSum = std::stoll(DoPartA(input));}
  catch (std::invalid_argument e) { PRINT("Couldn't parse int");  return "bad parse"; } // doesn't parse to int

  // Find target
  // result will be a contiguous set of integers that sum to target
  // answer is the first of that set plus the last

  return "no answer";
}

int main()
{
  AoCDay(9, DoPartA, DoPartB, "Data/input.txt");
}