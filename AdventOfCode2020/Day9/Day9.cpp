#include "Helper.h"
using namespace Helper;

#include <algorithm> // transform
#include <numeric>  // accumulate
#include <list>
typedef long long unsigned ll_int;
typedef std::list<ll_int> list_int;


bool NumberIsValid(list_int::const_iterator begin, list_int::const_iterator end, ll_int number)
{
  for (list_int::const_iterator itrA = begin; itrA != end; itrA++)
  {
    ll_int a = *itrA;
    for (list_int::const_iterator itrB = begin; itrB != end; itrB++)
    {
      ll_int b = *itrB;
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

list_int FindSequenceSummingTo(list_int const& list, size_t length, ll_int number)
{
  list_int::const_iterator begin_sequence = list.begin();
  list_int::const_iterator end_sequence = list.begin();
  std::advance(end_sequence, length);
  ll_int local_sum = std::accumulate(begin_sequence, end_sequence, (ll_int)0, [](ll_int a, ll_int b) {return a + b; });

  // Find a sequence of length that sums to number
  for (size_t i = 0; i < list.size() - length; i++)
  {
    if (local_sum == number)
    {
      return list_int(begin_sequence, end_sequence);
    }
    // Otherwise eject the last number and append the next
    local_sum -= *begin_sequence;
    local_sum += *end_sequence;
    begin_sequence++;
    end_sequence++;
  }
  return list_int();
}

str DoPartA(vec_str const& input_raw)
{
  list_int input;

  try{
    std::transform(input_raw.begin(), input_raw.end(), std::back_inserter(input), [](str a) { return std::stoll(a); });
  }
  catch (std::invalid_argument e) { PRINT("Couldn't parse int");  return "bad parse"; } // doesn't parse to int

  // Copy the preamble into recent
  int preamble_length = 25; // 25
  list_int::iterator begin_recent = input.begin();
  list_int::iterator end_recent = input.begin();
  std::advance(end_recent, preamble_length);

  for (list_int::const_iterator itr = end_recent; itr != input.end(); itr++)
  {
    ll_int newVal = *itr;
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

str DoPartB(vec_str const& input_raw)
{
  ll_int targetSum;
  try {targetSum = std::stoll(DoPartA(input_raw));}
  catch (std::invalid_argument e) { PRINT("Couldn't parse int");  return "bad parse"; } // doesn't parse to int

  list_int input;
  try {
    std::transform(input_raw.begin(), input_raw.end(), std::back_inserter(input), [](str a) { return std::stoll(a); });
  }
  catch (std::invalid_argument e) { PRINT("Couldn't parse int");  return "bad parse"; } // doesn't parse to int

  // Find target
  // result will be a contiguous set of integers that sum to target
  // answer is the first of that set plus the last

  // Sequence is at least 2 long
  // Should definitely end before getting all the way through o_o
  // Worst case is last number is the sum of all previous, but we know it's not quite that bad
  for (size_t i = 2; i < input.size(); i++)
  {
    PRINT("Trying seq length " << i);
    list_int result = FindSequenceSummingTo(input, i, targetSum);
    if (result.size() > 0)
    {
      result.sort();
      try { return std::to_string(result.front() + result.back()); }
      catch (std::invalid_argument e) { PRINT("Couldn't parse int");  return "bad parse"; } // doesn't parse to int
    }
  }

  return "no answer";
}

int main()
{
  AoCDay(9, DoPartA, DoPartB, "Data/input.txt");
}