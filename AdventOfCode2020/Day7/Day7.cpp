
#include "Helper.h"
using namespace Helper;

#include <regex>   // regex_token_iterator
#include <unordered_set> // unordered set

struct BagRule
{
  str color;
  str contentColor;
  int contentCount;
  
  BagRule(str color_in, str raw = "", int count = 0) 
    : color(color_in), contentColor(raw), contentCount(count)
  {
    if (raw.length() > 0)
    {
      // Count
      int firstSpace = raw.find(' ');
      try { contentCount = std::stoi(raw.substr(0,firstSpace)); }
      catch (std::invalid_argument e) { PRINT("Couldn't parse int");  return; } // doesn't parse to int

      // Color
      std::regex bag_end_regex(" bag*");
      std::smatch match;
      str color_raw = raw.substr(firstSpace+1);
      if (regex_search(color_raw, match, bag_end_regex))
      { contentColor = match.prefix(); }
    }
  }
};

typedef std::vector<BagRule> vec_rule;
typedef std::unordered_set<std::string> set_str;

void ParseBagRegulationLine(str const& line, vec_rule &result)
{
  str outerBag;
  std::regex bags_contain_regex(" bags contain ");

  //striped indigo bags contain 5 dull plum bags, 2 light lavender bags, 3 light red bags, 1 muted olive bag.
  //bright gold bags contain no other bags.

  std::smatch match;
  if (regex_search(line, match, bags_contain_regex))
  {
    outerBag = match.prefix();
    const std::string suffix = match.suffix();

    if (suffix == "no other bags.")
    {
      result.push_back(BagRule(outerBag));
      return;
    }

    const std::regex suffix_regex(", ");
    std::sregex_token_iterator itr = std::sregex_token_iterator(suffix.begin(), suffix.end(), suffix_regex, -1);
    std::sregex_token_iterator end = std::sregex_token_iterator();
    for (; itr != end; itr++)
    {
      result.push_back(BagRule(outerBag, *itr));
    }
  }
}

void ParseBagRegulations(vec_str lines, vec_rule& result)
{
  for (size_t i = 0; i < lines.size(); i++)
  {
    ParseBagRegulationLine(lines[i], result);
  }
}

void BagsThatCanContain(vec_rule const& rules, set_str & matches, str bagColor)
{
  set_str this_level_matches;

  // Named bag
  for (size_t i = 0; i < rules.size(); i++)
  {
    BagRule const& rule = rules[i];
    if (rule.contentColor == bagColor)
    {
      if (matches.count(rule.color) == 0)
      {
        this_level_matches.insert(rule.color);
        matches.insert(rule.color);
      }
    }
  }

  // Bags that contain named bag
  for (set_str::iterator itr = this_level_matches.begin(); itr != this_level_matches.end(); itr++)
  {
    BagsThatCanContain(rules, matches, *itr);
  }
}

int Do7A(vec_rule const& rules)
{
  PRINT("A");

  set_str matches;
  BagsThatCanContain(rules, matches, "shiny gold");

  return matches.size();
}


struct BagStack
{
  str bag;
  int count;
  BagStack(BagRule r): bag(r.contentColor), count(r.contentCount){}

};

int MustContainBags(vec_rule const& rules, str bagColor, int depth)
{
  int count = 0;
  for (size_t i = 0; i < rules.size(); i++)
  {
    BagRule const& rule = rules[i];
    if (rule.color == bagColor)
    {
      if (rule.contentCount > 0)
      {
        int newBags = MustContainBags(rules, rule.contentColor, depth + 1) * rule.contentCount;
        count += newBags;
      }
    }
  }
  return count+1;
}

int Do7B(vec_rule const& rules)
{
  PRINT("B");

  return MustContainBags(rules, "shiny gold", 0) - 1;
}

int main()
{
  PRINT("Day 7");
  str dataFileName = "Data/input.txt";
  str outFileName = "Data/Out.txt";
  vec_str lines_str = ParseFile(dataFileName);

  vec_rule rules;
  rules.reserve(lines_str.size());
  ParseBagRegulations(lines_str, rules);

  int Result7A = Do7A(rules);
  PRINT(Result7A);
  int Result7B = Do7B(rules);
  PRINT(Result7B);

  std::vector<std::string> lines_out;
  lines_out.push_back(std::to_string(Result7A));
  lines_out.push_back(std::to_string(Result7B));

  WriteToFile(lines_out, outFileName);
}