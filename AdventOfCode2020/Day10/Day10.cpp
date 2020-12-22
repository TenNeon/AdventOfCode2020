#include "Helper.h"
using namespace Helper;

#include <algorithm> // transform
//#include <numeric>  // accumulate
//#include <list>
//typedef long long unsigned ll_int;
//typedef std::list<ll_int> list_int;
typedef std::map<int, int> map_int_int;
map_int_int CountJumps(vec_int const & input)
{
  map_int_int jumps;

  for (size_t i = 0; i < input.size()-1; i++)
  {
    jumps[input[i + 1] - input[i]]++;
  }

  return jumps;
}

str DoPartA(vec_str const& input_raw) 
{ 
  vec_int input;

  try { std::transform(input_raw.begin(), input_raw.end(), std::back_inserter(input), [](str a) { return std::stoi(a); }); }
  catch (std::invalid_argument e) { PRINT("Couldn't parse int");  return "bad parse"; } // doesn't parse to int

  std::sort(input.begin(), input.end());

  input.insert(input.begin(), 0); // outlet
  input.push_back(input.back()+3); // adapter

  map_int_int jumps = CountJumps(input);



  return std::to_string(jumps[1] * jumps[3]); 
}

ll_int countPaths(vec_int const& nodes, std::vector<ll_int> & counts, int nodeIndex)
{
  if (nodeIndex >= nodes.size())
  {
    return 0;
  }

  if (nodeIndex == nodes.size() - 1)
  {
    counts[nodeIndex] = 1;
    return 1;
  }

  ll_int this_count = 0;
  int thisValue = nodes[nodeIndex];
  for (size_t i = 3; i > 0; --i)
  {
    int nextValue = thisValue + i;
    // child n can be up to n away
    for (size_t j = i; j > 0; --j)
    {
      int nextIndex = nodeIndex + j;
      bool hasNextValue = nextIndex < nodes.size() && nodes[nextIndex] == nextValue;
      if (hasNextValue)
      {
        this_count += counts[nextIndex] > 0 ? counts[nextIndex] : countPaths(nodes, counts, nextIndex);
        break;
      }
    }
  }

  counts[nodeIndex] = this_count;

  return this_count;
}

str DoPartB(vec_str const& input_raw)
{
  vec_int input;

  try { std::transform(input_raw.begin(), input_raw.end(), std::back_inserter(input), [](str a) { return std::stoi(a); }); }
  catch (std::invalid_argument e) { PRINT("Couldn't parse int");  return "bad parse"; } // doesn't parse to int
  std::sort(input.begin(), input.end());
  input.insert(input.begin(), 0); // outlet
  input.push_back(input.back() + 3); // adapter

  std::vector<ll_int> counts(input.size(), 0);
  ll_int result = countPaths(input, counts, 0);

  return std::to_string(result);
}

int main()
{
  AoCDay(10, DoPartA, DoPartB, "Data/input.txt");
}