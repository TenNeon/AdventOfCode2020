#include "Helper.h"
using namespace Helper;

#include <unordered_set>
#include <algorithm> // transform

struct Instruction
{
  enum class Operation { acc, jmp, nop };
  int arg = 0;
  Operation op = Operation::nop;
  Instruction(str const& raw)
  {
    int space = raw.find(' ');
    str raw_op = raw.substr(0, space);
    if (raw_op == "nop")
    {
      op = Operation::nop;
    }
    else if (raw_op == "acc")
    {
      op = Operation::acc;
    }
    else if (raw_op == "jmp")
    {
      op = Operation::jmp;
    }
    str raw_arg = raw.substr(space+1);

    arg = std::stoi(raw_arg);
  }
};

struct InstructionInfo
{
  int line;
  Instruction instruction;
};

struct InstructionContext
{
  InstructionInfo info;
  int accumulator;
};

class OpComputer
{
public:
  OpComputer(std::vector<Instruction> const& instr) : instructions(instr){}

  enum class ReturnCode {running, exit, error};

  // Execute current instruction and advance
  // returns whether it is still running
  ReturnCode Step()
  {
    if (currentInstruction < 0 )
    {
      PRINT("Current instruction not valid: " << currentInstruction);
      return ReturnCode::error;
    }

    Instruction const& instr = instructions[currentInstruction];
    switch (instr.op)
    {
    case Instruction::Operation::acc:
      accumulator += instr.arg;
      break;
    case Instruction::Operation::jmp:
      --currentInstruction += instr.arg;
      break;
    case Instruction::Operation::nop:
      break;
    default:
      break;
    }
    currentInstruction++;

    if (currentInstruction >= (int)instructions.size())
    {
      PRINT("OpComputer Exiting normally");
      return ReturnCode::exit;
    }

    return ReturnCode::running;
  }

  InstructionContext GetState()
  {
    return { {currentInstruction, instructions[currentInstruction] }, accumulator };
  }

private:
  std::vector<Instruction> instructions;
  int currentInstruction = 0;
  int accumulator = 0;
};

class OpComputerDebugger
{
  OpComputer client;

public:
  OpComputerDebugger(OpComputer clientIn): client(clientIn), executedLines(), lastGoodContext(clientIn.GetState()){}

  // returns true if successful, false if done
  OpComputer::ReturnCode Step()
  {
    lastGoodContext = client.GetState();
    OpComputer::ReturnCode returnCode = client.Step();
    if (returnCode == OpComputer::ReturnCode::exit) return returnCode;

    InstructionContext end = client.GetState();
    Instruction::Operation op = lastGoodContext.info.instruction.op;
    str opstr = op == Instruction::Operation::acc ? "acc" : op == Instruction::Operation::jmp ? "jmp" : "nop";
    //PRINT(opstr << ": "
    //  << lastGoodContext.info.instruction.arg << ", line: "
    //  << lastGoodContext.info.line << "-->" << end.info.line << ", acc: "
    //  << lastGoodContext.accumulator << "==>" << end.accumulator);

    if (executedLines.find(end.info.line) != executedLines.end() )
    {
      //PRINT("Attempted to repeat line " << end.info.line);
      return OpComputer::ReturnCode::error;
    }
    executedLines.insert(lastGoodContext.info.line);
    return returnCode;
  }

  InstructionContext GetContext() const { return lastGoodContext; }

private:
  std::unordered_set<int> executedLines;
  InstructionContext lastGoodContext;
};

std::vector<Instruction> Repair(std::vector<Instruction> const& instructions, int changeIndex)
{
  std::vector<Instruction> copy = instructions;
  Instruction::Operation& op = copy[changeIndex].op;

  if (op == Instruction::Operation::nop)
  {
    op = Instruction::Operation::jmp;
  }
  else if (op == Instruction::Operation::jmp)
  {
    op = Instruction::Operation::nop;
  }

  return copy;
}

int DoPartA(std::vector<Instruction> const& instructions)
{
  OpComputerDebugger debugger = OpComputerDebugger(instructions);
  OpComputer::ReturnCode returnCode;
  do {
    returnCode = debugger.Step();
  } while (returnCode == OpComputer::ReturnCode::running);
  return debugger.GetContext().accumulator;
}

int DoPartB(std::vector<Instruction> const& instructions)
{
  for (size_t i = 0; i < instructions.size(); i++)
  {
    Instruction::Operation const& op = instructions[i].op;
    if (op == Instruction::Operation::acc) continue;
    
    OpComputerDebugger debugger = OpComputerDebugger(Repair(instructions, i));
    OpComputer::ReturnCode returnCode;
    PRINT("Testing " << i);
    do {
      returnCode = debugger.Step();
    } while (returnCode == OpComputer::ReturnCode::running);

    if (returnCode == OpComputer::ReturnCode::exit)
    {
      return debugger.GetContext().accumulator;
    }
  }
  return -1;
}

int main()
{
  PRINT("Day 8");
  str dataFileName = "Data/input.txt";
  str outFileName = "Data/Out.txt";
  vec_str lines_str = ParseFile(dataFileName);

  std::vector<Instruction> instructions;
  std::transform(lines_str.begin(), lines_str.end(), std::back_inserter(instructions),
    [](str a) { return Instruction(a); });

  PRINT("A");
  int ResultA = DoPartA(instructions);
  PRINT(ResultA);

  PRINT("B");
  int ResultB = DoPartB(instructions);
  PRINT(ResultB);

  std::vector<std::string> lines_out;
  lines_out.push_back(std::to_string(ResultA));
  lines_out.push_back(std::to_string(ResultB));

  WriteToFile(lines_out, outFileName);
}