#include <vector>
#include <algorithm> // transform

#include "Helper.h"
using namespace Helper;

struct Slope
{
  int right;
  int down;
};

struct Position
{
  int x;
  int y;
  bool tree;

  Position() : x(-1), y(-1), tree(false){}

  Position(int x_in, int y_in, bool tree_in) : x(x_in), y(y_in), tree(tree_in) {}

  // Position + Slope = Position
  Position operator+(Slope const& s) const
  {
    return Position(x + s.right, y + s.down, true);
  }

  bool operator==(Position const& other) const
  {
    return x == other.x && y == other.y;
  }

  bool operator!=(Position const& other) const
  {
    return !(*this == other);
  }

  static const Position end;
};

const struct Position Position::end = Position::Position(); // initialize static member

class Terrain
{
  public:
    Terrain(std::vector<std::string> const& strings)
  {
    w = strings[0].length();
    h = strings.size();
    terrain.reserve(h);

    for (size_t i = 0; i < strings.size(); i++)
    {
      std::vector<bool> line;
      line.reserve(w);
      for (size_t j= 0; j < w; j++)
      {
        line.push_back(strings[i][j] == '#');
      }
      terrain.push_back(line);
    }
  }
  
  private:
    std::vector<std::vector<bool>> terrain;
    size_t w;
    size_t h;

  public:
    int height()
    {
      return terrain.size();
    }

    Position Get(size_t x, size_t y) const
    {
      if (x >= 0 && y >=0 && x < w && y < h)
      {
        return Position(x, y, terrain[y][x]);

      }
      return Position::end;
    }

    Position Next(Position const &p, Slope const &s)
    {
      Position newPos = p + s;

      if (newPos.y >= (int)h)
      {
        return Position::end;
      }

      newPos.x %= w;
      newPos.tree = terrain[newPos.y][newPos.x];

      return newPos;
    }
};

int Slide(Terrain t, Position const& p_in, Slope const& s)
{
  Position p = p_in;
  int trees_hit = 0;

  do {
    trees_hit += p.tree ? 1 : 0;
    p = t.Next(p, s);
  } while (p != Position::end);

  return trees_hit;
}

// Starting at the top-left corner of your map and following a slope of right 3 and down 1, how many trees would you encounter?
int Do3A(Terrain t)
{
  PRINT("Part A");

  Slope s = {3,1};
  Position p = t.Get(0,0);
  return Slide(t,p,s);
}

long long unsigned Do3B(Terrain t)
{
  PRINT("Part B");

  std::vector<Slope> slopes;
  slopes.push_back({1,1}); //  Right 1, down 1. 90
  slopes.push_back({3,1}); //  Right 3, down 1. 244
  slopes.push_back({5,1}); //  Right 5, down 1. 97
  slopes.push_back({7,1}); //  Right 7, down 1. 92
  slopes.push_back({1,2}); //  Right 1, down 2. 48

  Position p = t.Get(0, 0);

  long long unsigned result = Slide(t, p, slopes[0]);

  // What do you get if you multiply together the number of trees encountered on each of the listed slopes?
  for (size_t i = 01; i < slopes.size(); i++)
  {
    int slide = Slide(t, p, slopes[i]);
    result *= slide;
  }
  return result;
}

int main()
{
  PRINT("Day 3");
  std::string dataFileName = "Data/input.txt";
  std::string outFileName = "Data/Out.txt";
  std::vector<std::string> lines_str = ParseFile(dataFileName);

  // Data to trees
  Terrain t = Terrain(lines_str);

  // Data to strings
  std::vector<std::string> lines_out;
  lines_out.push_back(std::to_string(Do3A(t)));
  lines_out.push_back(std::to_string(Do3B(t)));

  PRINT("Result: " );
  PRINT(lines_out[0]);
  PRINT(lines_out[1]);

  WriteToFile(lines_out, outFileName);
}