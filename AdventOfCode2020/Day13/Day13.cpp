#include "Helper.h"
using namespace Helper;

#include <map>
typedef std::map<int, vec_int> schedule;

ll_int NextBus(int ID, ll_int time)
{
  if (ID == 1 || time % ID == 0) return 0; // skip extra math

  // find the smallest multiple of ID that is greater than time

  return ((time / ID) + 1) * ID - time;
}

typedef std::map<int, int> map_int_int;
typedef std::vector < std::pair<int, int>> vec_pair_int_int;

bool isBusDepartureRun(ll_int time, vec_pair_int_int const& routes)
{
  for (size_t i = 0; i < routes.size(); i++)
  {
    int const& index = routes[i].first;
    int const& ID = routes[i].second;
    int next = NextBus(ID, time);
    if (next != index) // wait should be equal to index
    {
      return false;
    } 
  }
  return true;
}


// Get next bus
str DoPartA(vec_str const& input_raw)
{
  // ID = period

  int waitTime = INT_MAX;
  int busID = -1;

  int time = std::stoi(input_raw[0]);
  vec_str tokens = Tokenize(input_raw[1]);
  
  for (vec_str::iterator itr = tokens.begin(); itr != tokens.end(); itr++)
  {
    str const& token = *itr;
    // walk through the tokens, trying to parse each to int
    int ID = -1;
    if (token != "x")
    {
      try { ID = std::stoi(token); }
      catch (std::invalid_argument e) { PRINT("Couldn't parse " << token << " to int"); continue; } // doesn't parse to int
    }
    if (ID <= 0) { continue; }

    int thisWait = NextBus(ID, time);
    if (thisWait < waitTime)
    {
      busID = ID;
      waitTime = thisWait;
    }
  }

  if (busID > 0 && waitTime < INT_MAX)
  {
    return std::to_string(busID * waitTime);
  }

  return "incomplete";
}

str DoPartB(vec_str const& input_raw)
{
  vec_str tokens = Tokenize(input_raw[1]);
  map_int_int routes;


  for (size_t i = 0; i < tokens.size(); i++)
  {
    str const& token = tokens[i];

    if (token != "x")
    {
      try { routes[i] = std::stoi(token); }
      catch (std::invalid_argument e) { PRINT("Couldn't parse " << token << " to int"); continue; } // doesn't parse to int
    }
  }

  // Find time such that
  // for i in tokens
  // NextBus((int)tokens[i], time) == i || tokens[i] == 'x'

  // answer must be a multiple of
  // ???

  vec_pair_int_int routes_vec = vec_pair_int_int(routes.begin(), routes.end());
  ll_int time1 = routes.begin()->second;
  ll_int time = time1;
  while (!isBusDepartureRun(time, routes_vec))
  {
    if (time % 1000000000 <= time1)
    {
      PRINT(time);
    }
    time += time1;
  }

  return std::to_string(time);
}
int main()
{
  //AoCDay(13, DoPartA, DoPartB, "Data/test.txt");
  AoCDay(13, DoPartA, DoPartB, "Data/input.txt");
}