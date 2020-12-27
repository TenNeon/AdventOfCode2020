#include "Helper.h"
using namespace Helper;

#include <cmath> // abs

vec2 North = vec2(0,1);
vec2 South = vec2(0,-1);
vec2 East = vec2(1,0);
vec2 West = vec2(-1,0);

struct Instruction
{
  char instruction = 0;
  int distance = 0;
  Instruction(char i, int dist) : instruction(i), distance(dist) 
  { }
};

struct Orientation
{
  vec2 pos;
  vec2 dir;
  Orientation(vec2 p_in, vec2 d_in) : pos(p_in), dir(d_in) {}
};

vec2 RotateDir(vec2 const& dir, char LR)
{
  if (dir == East) // E
  {
    if (LR == 'L')
    {
      return North;
    }
    else
    {
      return South;
    }
  }
  if (dir == West) // W
  {
    if (LR == 'R')
    {
      return North;
    }
    else
    {
      return South;
    }
  }
  if (dir == North) // N
  {
    if (LR == 'R')
    {
      return East;
    }
    else
    {
      return West;
    }
  }
  if (dir == South) // S
  {
    if (LR == 'L')
    {
      return East;
    }
    else
    {
      return West;
    }
  }
  return vec2();
}

Orientation RotateAround(Orientation const& ori, char LR, int times = 1)
{
  Orientation result = ori;
  vec2& dir = result.dir;
  vec2& pos = result.pos; // doesn't change
  vec2 wayp0 = dir;

  vec2 offset = dir;
  float th = times * HALF_PI;

  if (LR == 'R'){th *= -1;} // reverse for right rotation

  // rotate

  dir.x = wayp0.x * cos(th) - wayp0.y * sin(th);
  dir.y = wayp0.x * sin(th) + wayp0.y * cos(th);

  return result;
}

class Ship
{
protected:
  Orientation orientation;
public:
  Ship() : orientation(vec2(0,0),vec2(1,0)) {}
  Ship(Orientation const& o) : orientation(o) {}

  void Step(Instruction instruction)
  {
    int rotTimes = instruction.distance / 90;
    vec2& pos = orientation.pos;
    vec2& dir = orientation.dir;
    //vec2 newPos;
    switch (instruction.instruction)
    {
    case 'N':
      pos += North * instruction.distance;
      break;
    case 'S':
      pos += South * instruction.distance;
      break;
    case 'E':
      pos += East * instruction.distance;
      break;
    case 'W':
      pos += West * instruction.distance;
      break;
    case 'L': // rotate left
      for (size_t i = 0; (int)i < rotTimes; i++)
      {
        dir = RotateDir(dir, 'L');
      }
      break;
    case 'R': // rotate right
      for (size_t i = 0; (int)i < rotTimes; i++)
      {
        dir = RotateDir(dir, 'R');
      }
      break;
    case 'F': // forward
      pos += dir*instruction.distance;
    default:
      break;
    }

    PRINT(pos.x << " " << pos.y);

  }

  vec2 GetPos() { return orientation.pos; }
  vec2 GetDir() { return orientation.dir; }
};

class WayShip : public Ship
{
public:
  WayShip(Orientation o) : Ship(o){}

  void Step(Instruction instruction)
  {
    int rotTimes = instruction.distance / 90;
    vec2& pos = orientation.pos;
    vec2& waypoint = orientation.dir;
    //vec2 newPos;
    switch (instruction.instruction)
    {
    case 'N':
      waypoint += North * instruction.distance;
      break;
    case 'S':
      waypoint += South * instruction.distance;
      break;
    case 'E':
      waypoint += East * instruction.distance;
      break;
    case 'W':
      waypoint += West * instruction.distance;
      break;
    case 'L': // rotate left
      orientation = RotateAround(orientation, 'L', rotTimes);
      break;
    case 'R': // rotate right
      orientation = RotateAround(orientation, 'R', rotTimes);
      break;
    case 'F': // forward
      //PRINT("F" << instruction.distance);
      //std::cout << "Stepped from " << pos.x << ", " << pos.y << " to ";
      pos += waypoint * instruction.distance;

      //std::cout << pos.x << ", " << pos.y << std::endl;

    default:
      break;
    }
  }
};

typedef std::vector<Instruction> vec_instr;
vec_instr ParseInstructions(vec_str const& raw)
{
  vec_instr result;

  for (size_t i = 0; i < raw.size(); i++)
  {
    result.push_back(Instruction( raw[i].c_str()[0], std::stoi(raw[i].substr(1)) ));
  }

  return result;
}

str DoPartA(vec_str const& input_raw)
{
  vec_instr instructions = ParseInstructions(input_raw);

  Ship ship = Ship();

  for (size_t i = 0; i < instructions.size(); i++)
  {
    ship.Step(instructions[i]);
  }
  vec2 pos = ship.GetPos();
  return std::to_string((int)(std::abs(pos.x) + std::abs(pos.y)));
}

str DoPartB(vec_str const& input_raw)
{
  vec_instr instructions = ParseInstructions(input_raw);

  WayShip ship = WayShip(Orientation(vec2(0), vec2(10,1)));

  for (size_t i = 0; i < instructions.size(); i++)
  {
    ship.Step(instructions[i]);
  }
  vec2 pos = ship.GetPos();
  return std::to_string((int)(std::abs(pos.x) + std::abs(pos.y)));
}
int main()
{
  //AoCDay(12, DoPartA, DoPartB, "Data/test.txt");
  AoCDay(12, DoPartA, DoPartB, "Data/input.txt");
}