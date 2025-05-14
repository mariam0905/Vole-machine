#pragma once

#include <string>
#include <unordered_map>

class ALU
{
private:
    static std::unordered_map<std::string, char> lookupTableToHex;
    static std::unordered_map<char, std::string> lookupTableToDec;
public:
    static std::string DecToHex(int num);
    static int HexToDec(const std::string& num);
    static std::string addHexInt(const std::string& n1, const std::string& n2);

    static std::string FloatToHex(float num);
    static float HexToFloat(const std::string& num);
    static std::string addHexFloat(const std::string& n1, const std::string& n2);

    static std::string OR(const std::string& n1, const std::string& n2);
    static std::string AND(const std::string& n1, const std::string& n2);
    static std::string XOR(const std::string& n1, const std::string& n2);
    static std::string rotate(const std::string& n1, const std::string& n2);
};

class Register
{
private:
    std::string registers[16];
public:
    Register()
    {
        for (int i = 0; i < 16; ++i)
        {
            registers[i] = "00";
        }
    }
    void setValue(const int& address, const std::string& value);
    std::string getValue(const int& address);
};

class Memory
{
private:
    std::string cells[256];
public:
    Memory()
    {
        for (int i = 0; i < 256; ++i)
        {
            cells[i] = "00";
        }
    }
    std::string getCell(const int& address);
    void setCell(const int& address, const std::string& value);
};

class CU
{
public:
    CU() {}

    static void loadRegMem(Register& reg, const int& regIdx, Memory& mem, const int& memIdx);

     /**
     * @brief Loads the value of the memory at the cell `memIdx` into the register at the index `regIdx`
     * @param reg The register of the CPU
     * @param regIdx The index of the register
     * @param mem The memory of the machine
     * @param memIdx The index of the memory cell
     */
    static void loadRegVal(Register& reg, const int& regIdx, const int& val);

    /**
     * @brief Loads the value `val` in the register at the index `regIdx`
     * @param reg The resgister of the CPU
     * @param regIdx The index of the register
     * @param val The value to be stored in the register at index `regIdx`
     */
    static void store(Register& reg, const int& regIDX, Memory& mem, const int& memIdx);

    /**
     * @brief Stores the value of the register at index `regIDX` into the memory at the index `memIdx`
     * @param reg The register of the CPU
     * @param mem The memory of the machine
     * @param regIdx The index of the register
     * @param memIdx The index of the memory cell
     */
    static void move(Register& reg, const int& regIdx1, const int& regIdx2);

   /**
     * @brief Moves the value of the register at index `regIdx1` to the register at index `regIdx2`
     * @param reg The register of the CPU
     * @param regIdx1 The index of the first register
     * @param regIdx2 The index of the second register
     */
    static void jump(Register& reg, const int& regIdx, const int& memIdx, int& PC);
/**
     * @brief Checks the value of the register at index `regIdx` with the register index 0, if they are equal the
     *        execution of the program continues from the memory cell at index `memIdx`
     * @param reg The register of the CPU
     * @param regIdx The index of the register to be comapred with the register at index 0
     * @param memIdx The index of the memory cell that will be executed if the 2 registers are equal
     * @param PC The program counter of the CPU
     */
    static void halt();
};

class CPU
{
private:
    ALU alu;
    CU cu;
    std::string instruction;
    int programCounter;
public:
    Register reg;
    static bool isHalt;
    CPU() : programCounter(0) {}
    /**
     * @brief Gets the instruction to be executed from the memory using the
     *        program counter and puts it in the instruction register
     * @param mem The memory of the machine
     */
    void fetch(Memory& mem);

    /**
     * @brief Decodes the instruction in the instruction register to be executed
     */
    void decode(Memory& mem, Register& reg);
    void setPC(int p);
    void runInstruction(Memory& mem);
    std::string getFromReg(const int& address);
};

class Machine
{
public:
    CPU cpu;
    Memory memory;
    static std::string screenASCII;
    static std::string screenHex;
    Machine() {}

    void loadProgram(const std::string& fileName, const std::string& startMem);
    void printState();
};

class VoleMain
{
private:
    void printMenu();
    void takeInput();
    void handleChoice(std::string& choice ,Memory& mem, Register& reg);

public:
    Machine machine;
    VoleMain() {}

    /**
     * @brief This starts the whole machine
     */
    void init(Memory& mem , Register& reg);
};
