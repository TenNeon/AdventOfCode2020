#include <vector>
#include <algorithm> // transform
#include <sstream> // istringstream
#include <regex>   // regex_token_iterator
#include <exception> // invalid argument
#include "Helper.h"
using namespace Helper;

struct ID
{
  map_str_str fields;

  ID(map_str_str map_in) : fields(map_in){}

  bool IsValid() const
  {
    for (std::map<std::string, IDValidationFunction>::const_iterator field = validation_map.begin();
      field != validation_map.end(); field++)
    {
      bool valid = (this->*(field->second))();
      if (!valid)
      {
        return false;
      }
    }
    return true;
  }

  bool SimpleIsValid() const
  {
    for (std::map<std::string, IDValidationFunction>::const_iterator field = validation_map.begin();
      field != validation_map.end(); field++)
    {
      std::string const& fieldName = field->first;
      if (fieldName != "cid")
      {
        bool invalid = fields.find(fieldName) == fields.end();
        if (invalid) 
          return false;
      }
    }
    return true;
  }

  //  byr(Birth Year) - four digits; at least 1920 and at most 2002.
  bool ValidBirthYear() const
  {
    return StrDateBetween("byr", 1920, 2002);
  }

  //  iyr(Issue Year) - four digits; at least 2010 and at most 2020.
  bool ValidIssueYear() const
  {
    return StrDateBetween("iyr", 2010, 2020);
  }

  //  eyr(Expiration Year) - four digits; at least 2020 and at most 2030.
  bool ValidExpirationYear() const
  {
    return StrDateBetween("eyr", 2020, 2030);
  }

  //  hgt(Height) - a number followed by either cm or in:
  //                If cm, the number must be at least 150 and at most 193.
  //                If in, the number must be at least 59 and at most 76.
  bool ValidHeight() const
  {
    map_str_str::const_iterator found = fields.find("hgt");
    if (found == fields.end()) return false; // reject not-found

    std::string const& found_str = found->second;
    if (found_str.length() < 4) return false; // reject too short

    std::string unit = found_str.substr(found_str.size() - 2, 2);
    try
    {
      int value = std::stoi(found_str.substr(0, found_str.size() - 2));
      if (unit == "cm") return Between(value, 150, 193); // reject bad value
      if (unit == "in") return Between(value, 59, 76); // reject bad value
    }
    catch (std::invalid_argument e) { return false; } // reject doesn't parse to int
    return false; // reject bad unit
  }

  //  hcl(Hair Color) - a # followed by exactly six characters 0 - 9 or a - f.
  bool ValidHairColor() const
  {
    map_str_str::const_iterator found = fields.find("hcl");
    if (found == fields.end()) return false; // reject not-found

    std::string const& found_str = found->second;
    std::regex regex("#[a-zA-Z0-9]{6}");

    bool valid = std::regex_match(found_str,regex);
    return valid;
  }

  //  ecl(Eye Color) - exactly one of : amb blu brn gry grn hzl oth.
  bool ValidEyeColor() const
  {
    map_str_str::const_iterator found = fields.find("ecl");
    if (found == fields.end()) return false; // reject not-found

    std::string const& found_str = found->second;
    return (found_str == "amb"
      || found_str == "blu"
      || found_str == "brn"
      || found_str == "gry"
      || found_str == "grn"
      || found_str == "hzl"
      || found_str == "oth");
  }

  //  pid(Passport ID) - a nine - digit number, including leading zeroes.
  bool ValidPassportID() const
  {
    map_str_str::const_iterator found = fields.find("pid");
    if (found == fields.end()) return false; // reject not-found

    std::string const& found_str = found->second;
    if (found_str.length() != 9) return false; // reject too short

    try{std::stoi(found_str);}
    catch (std::invalid_argument e) { return false; } // reject doesn't parse to int

    return true;
  }

  //  cid(Country ID) - ignored, missing or not.
  bool ValidCountryID() const
  {
    return true;
  }

private:
  typedef bool (ID::*IDValidationFunction)(void) const; // function pointer type

  std::map<std::string, ID::IDValidationFunction> validation_map =
  {
    {"byr", &ID::ValidBirthYear     },
    {"iyr", &ID::ValidIssueYear     },
    {"eyr", &ID::ValidExpirationYear},
    {"hgt", &ID::ValidHeight        },
    {"hcl", &ID::ValidHairColor     },
    {"ecl", &ID::ValidEyeColor      },
    {"pid", &ID::ValidPassportID    },
    {"cid", &ID::ValidCountryID     }
  };

  bool StrDateBetween(std::string const& str_date, int min, int max) const
  {
    map_str_str::const_iterator found = fields.find(str_date);

    if (found != fields.end())
    {
      std::string found_str = found->second;
      if (found_str.length() != 4) return false; // not 4 digits

      try
      {
        int year_int = std::stoi(found_str);
        return Between(year_int, min, max);
      }
      catch (std::invalid_argument e) { return false; }
    }
    return false;
  }

};

std::vector<map_str_str> ParseToIDFields(std::vector<std::string> raw)
{
  std::vector<map_str_str> result;
  map_str_str tokenized;
  std::istringstream in_stream;
  std::string buffer;

  for (size_t i = 0; i < raw.size(); i++)
  {
    std::string& line = raw[i];
    if (line == "") // push complete set
    {
      result.push_back(tokenized);
      tokenized.clear();
    }
    else // process line
    {
      in_stream = std::istringstream(line);
      while (in_stream >> buffer)
      {
        int colon = buffer.find(':');
        std::string key = buffer.substr(0, colon);
        std::string value = buffer.substr(colon+1);
        tokenized[key] = value;
      }
    }
  }
  return result;
}


int Do4A(std::vector<ID> const& p)
{
  PRINT("A:");
  int result = 0;
  for (size_t i = 0; i < p.size(); i++)
  {
    result += p[i].SimpleIsValid() ? 1 : 0;
  }
  return result;
}

int Do4B(std::vector<ID> const& p)
{
  PRINT("B:");
  int result = 0;
  for (size_t i = 0; i < p.size(); i++)
  {
    result += p[i].IsValid() ? 1 : 0;
  }
  return result;
}

int main()
{
  PRINT("Day 4");
  std::string dataFileName = "Data/input.txt";
  std::string outFileName = "Data/Out.txt";
  std::vector<std::string> lines_str = ParseFile(dataFileName);

  std::vector<map_str_str> raw_ID = ParseToIDFields(lines_str);

  // Data to passports
  std::vector<ID> passports;
  std::transform(raw_ID.begin(), raw_ID.end(), std::back_inserter(passports),
    [](map_str_str a) { return ID(a); });

  // Passports to coutns of valid
  std::vector<std::string> lines_out;
  lines_out.push_back(std::to_string(Do4A(passports)));
  PRINT(lines_out[0]);
  lines_out.push_back(std::to_string(Do4B(passports)));
  PRINT(lines_out[1]);

  WriteToFile(lines_out, outFileName);
}