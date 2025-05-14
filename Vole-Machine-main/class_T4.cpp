#include "class_T4.h"
#include <bitset>
#include <cmath>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <cctype>

std::unordered_map<std::string, char> ALU::lookupTableToHex = {{"0000", '0'},
        {"0001", '1'}, {"0010", '2'}, {"0011", '3'}, {"0100", '4'}, {"0101", '5'},
        {"0110", '6'}, {"0111", '7'}, {"1000", '8'}, {"1001", '9'}, {"1010", 'A'},
        {"1011", 'B'}, {"1100", 'C'}, {"1101", 'D'}, {"1110", 'E'}, {"1111", 'F'}};


std::unordered_map<char, std::string> ALU::lookupTableToDec = {{'0', "0000"},
        {'1', "0001"}, {'2', "0010"}, {'3', "0011"}, {'4', "0100"}, {'5', "0101"},
        {'6', "0110"}, {'7', "0111"}, {'8', "1000"}, {'9', "1001"}, {'A', "1010"},
        {'B', "1011"}, {'C', "1100"}, {'D', "1101"}, {'E', "1110"}, {'F', "1111"}};

std::string Machine::screenASCII = "";
std::string Machine::screenHex = "";

bool CPU::isHalt = false;

std::string ALU::DecToHex(int num)
{
    std::bitset<8> b(num);
    std::string temp = "";
    std::string hex = "";

    for (int i = 7; i >= 4; --i)
    {
        temp += (b[i] ? '1' : '0');
    }

    hex += ALU::lookupTableToHex[temp];
    temp = "";

    for (int i = 3; i >= 0; --i)
    {
        temp += (b[i] ? '1' : '0');
    }

    hex += ALU::lookupTableToHex[temp];

    return hex;
}

int ALU::HexToDec(const std::string& num)
{
    std::string bits = "";
    bits += ALU::lookupTableToDec[num[0]];
    bits += ALU::lookupTableToDec[num[1]];

    std::bitset<8> b(bits);
    int dec = b.to_ulong();

    if (b[7] == 1)
    {
        dec -= (1 << 8);
    }

    return dec;
}

std::string ALU::addHexInt(const std::string& n1, const std::string& n2)
{
    int a = ALU::HexToDec(n1);
    int b = ALU::HexToDec(n2);
    return ALU::DecToHex(a + b);
}

std::string ALU::FloatToHex(float num)
{
    int S = (num < 0.0) ? 1 : 0;
    num = (num < 0.0) ? -num : num;
    int E = 0;
    while (num >= 2.0)
    {
        num /= 2.0;
        ++E;
    }
    while (num < 1.0)
    {
        num *= 2.0;
        --E;
    }
    num -= 1.0;
    E += 4;
    int M = static_cast<int>(num * 16);
    int ans = (S << 7) | (E << 4) | M;
    return ALU::DecToHex(ans);
}

float ALU::HexToFloat(const std::string& num)
{
    std::string bits = "";
    bits += ALU::lookupTableToDec[num[0]];
    bits += ALU::lookupTableToDec[num[1]];

    std::bitset<8> b(bits);
    int S = b[7] ? -1 : 1;
    int E = (b[6] << 2) | (b[5] << 1) | b[4];
    E -= 4;

    float M = 1.0;
    for (int i = 3; i >= 0; --i)
    {
        M += b[i] * std::pow(2, i - 4);
    }

    return S * M * std::pow(2, E);
}

std::string ALU::addHexFloat(const std::string& n1, const std::string& n2)
{
    float a = ALU::HexToFloat(n1);
    float b = ALU::HexToFloat(n2);
    return ALU::FloatToHex(a + b);
}

std::string ALU::OR(const std::string& n1, const std::string& n2)
{
    int a = HexToDec(n1);
    int b = HexToDec(n2);
    return DecToHex(a | b);
}

std::string ALU::AND(const std::string& n1, const std::string& n2)
{
    int a = HexToDec(n1);
    int b = HexToDec(n2);
    return DecToHex(a & b);
}

std::string ALU::XOR(const std::string& n1, const std::string& n2)
{
    int a = HexToDec(n1);
    int b = HexToDec(n2);
    return DecToHex(a ^ b);
}

std::string ALU::rotate(const std::string& n1, const std::string& n2)
{
    int num = ALU::HexToDec(n1);
    int x = ALU::HexToDec(n2);

    return ALU::DecToHex((num >> x) | (num << (8 - x)));
}

/////////////////////////////////////////////////////////////////////////////////

void Register::setValue(const int& address, const std::string& value)
{
    if (address < 0 || address > 15)
    {
        throw std::out_of_range("Register address is out of range");
    }
    registers[address] = value;
}

std::string Register::getValue(const int& address)
{
    if (address < 0 || address > 15)
    {
        throw std::out_of_range("Register address is out of range");
    }
    return registers[address];
}

////////////////////////////////////////////////////////////////////////////////////////////////////

std::string Memory::getCell(const int& address)
{
    if (address < 0 || address > 255)
    {
        throw std::out_of_range("Memory address is out of range");
    }
    return cells[address];
}

void Memory::setCell(const int& address, const std::string& value)
{
    if (address < 0 || address > 255)
    {
        throw std::out_of_range("Memory address is out of range");
    }
    if (address == 0)
    {
        Machine::screenASCII += char(ALU::HexToDec(value));
        Machine::screenHex += value + ' ';
    }
    cells[address] = value;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

void CU::loadRegMem(Register& reg, const int& regIdx, Memory& mem, const int& memIdx)
{
    reg.setValue(regIdx, mem.getCell(memIdx));
}

void CU::loadRegVal(Register& reg, const int& regIdx, const int& val)
{
    reg.setValue(regIdx, ALU::DecToHex(val));
}

void CU::store(Register& reg, const int& regIdx, Memory& mem, const int& memIdx)
{
    mem.setCell(memIdx, reg.getValue(regIdx));
}

void CU::move(Register& reg, const int& regIdx1, const int& regIdx2)
{
    reg.setValue(regIdx2, reg.getValue(regIdx1));
}

void CU::jump(Register& reg, const int& regIdx, const int& memIdx, int& PC)
{
    if (reg.getValue(0) == reg.getValue(regIdx))
    {
        PC = memIdx;
    }
}

void CU::halt()
{
    CPU::isHalt = true;
}

///////////////////////////////////////////////////////////////////////////////////////////////

void CPU::fetch(Memory& mem)
{
    instruction = mem.getCell(programCounter) + mem.getCell(programCounter+1);
    //std::cout << "\npc: " << programCounter << "->" << instruction << "\n\n";
    programCounter += 2;
    if (programCounter > 255)
    {
        throw std::overflow_error("The program counter overflowed the memory");
    }
}

void CPU::decode(Memory& mem, Register& reg)
{
    if (instruction[0] == '1')
        {
        int regIdx = ALU::HexToDec(instruction.substr(1, 1));
        std::string XY = instruction.substr(2, 2);
        int memIdx = ALU::HexToDec(XY);
        CU::loadRegMem(reg, regIdx, mem, memIdx);
        }
    else if (instruction[0] == '2')
        {
        int regIdx = ALU::HexToDec(instruction.substr(1, 1));
        int val = ALU::HexToDec(instruction.substr(2, 2));
        CU::loadRegVal(reg, regIdx, val);
        }
    else if (instruction[0] == '3')
        {
        int regIdx = ALU::HexToDec(instruction.substr(1, 1));
        std::string XY = instruction.substr(2, 2);
        int memIdx = ALU::HexToDec(XY);
        CU::store(reg, regIdx, mem, memIdx);
        }
    else if (instruction[0] == '4')
        {
        int regIdx1 = ALU::HexToDec(instruction.substr(2, 1));
        int regIdx2 = ALU::HexToDec(instruction.substr(3, 1));
        CU::move(reg, regIdx1, regIdx2);
        }
    else if (instruction[0] == '5')
        {
        int R_index = ALU::HexToDec(instruction.substr(1, 1));
        int S_index = ALU::HexToDec(instruction.substr(2, 1));
        int T_index = ALU::HexToDec(instruction.substr(3, 1));
        std::string addition = ALU::addHexInt(reg.getValue(S_index), reg.getValue(T_index));
        reg.setValue(R_index, addition);
        }
   else if (instruction[0] == '6')
        {
        int R_index = ALU::HexToDec(instruction.substr(1, 1));
        int S_index = ALU::HexToDec(instruction.substr(2, 1));
        int T_index = ALU::HexToDec(instruction.substr(3, 1));
        std::string addition = ALU::addHexFloat(reg.getValue(S_index), reg.getValue(T_index));
        reg.setValue(R_index, addition);
        }
    else if (instruction[0] == '7')
        {
        int R_index = ALU::HexToDec(instruction.substr(1, 1));
        int S_index = ALU::HexToDec(instruction.substr(2, 1));
        int T_index = ALU::HexToDec(instruction.substr(3, 1));
        std :: string result = ALU::OR(reg.getValue(S_index), reg.getValue(T_index));
        reg.setValue(R_index, result);
        }
        else if (instruction[0] == '8')
        {
        int R_index = ALU::HexToDec(instruction.substr(1, 1));
        int S_index = ALU::HexToDec(instruction.substr(2, 1));
        int T_index = ALU::HexToDec(instruction.substr(3, 1));
        std :: string result = ALU::AND(reg.getValue(S_index), reg.getValue(T_index));
        reg.setValue(R_index, result);
        }
        else if (instruction[0] == '9')
        {
        int R_index = ALU::HexToDec(instruction.substr(1, 1));
        int S_index = ALU::HexToDec(instruction.substr(2, 1));
        int T_index = ALU::HexToDec(instruction.substr(3, 1));
        std :: string result = ALU::XOR(reg.getValue(S_index), reg.getValue(T_index));
        reg.setValue(R_index, result);
        }
        else if (instruction[0] == 'A')
        {
        int R_index = ALU::HexToDec(instruction.substr(1, 1));
        std :: string result = ALU::rotate(reg.getValue(R_index),instruction.substr(3, 1));
        reg.setValue(R_index, result);
        }
        else if (instruction[0] == 'B')
        {
        int memIdx = ALU::HexToDec(instruction.substr(2));
        int regIdx = ALU::HexToDec(instruction.substr(1, 1));
        CU::jump(reg, regIdx, memIdx, programCounter);
        }
        else if (instruction[0] == 'C')
        {
            CU::halt();
        }
        else if (instruction[0] == 'D')
        {
            if (reg.getValue(0) < reg.getValue(ALU::HexToDec(instruction.substr(1,1))))
            {
                programCounter = ALU::HexToDec(instruction.substr(2));
            }
        }
        else
        {
            throw std::runtime_error("Unknown instruction code");
        }

}

void CPU::setPC(int p)
{
    this->programCounter = p;
}

void CPU::runInstruction(Memory& mem)
{
    fetch(mem);
    decode(mem , reg);
}

std::string CPU::getFromReg(const int& address)
{
    return reg.getValue(address);
}

////////////////////////////////////////////////////////////////////////////////////

void Machine::loadProgram(const std::string& fileName, const std::string& startMem = "10")
{
    std::ifstream fin{fileName};

    if (!fin.is_open())
    {
        throw std::runtime_error("Faild to open file");
    }
    std::string inst;
    int address = ALU::HexToDec(startMem);
    cpu.setPC(address);
    for (int i = 0; i < 16; ++i)
    {
        cpu.reg.setValue(i, "00");
    }
    for (int i = 0; i < 256; ++i)
    {
        memory.setCell(i, "00");
    }
    screenASCII = "";
    screenHex = "";
    while (fin >> inst)
    {
        if (address + 1 >= 256)
        {
            throw std::overflow_error("Program length exceeds machine's memory");
        }
        for (char& c : inst)
        {
            c = std::toupper(c);
        }
        memory.setCell(address, inst.substr(0, 2));
        memory.setCell(address+1, inst.substr(2));
        address += 2;
    }
    fin.close();
    std::cout << "Filed loaded successfully\n";
}

void Machine::printState()
{
    std::cout << "=== Registers ===\n";
    for (int i = 0; i < 16; ++i)
    {
       std::cout << cpu.reg.getValue(i) << "  ";
            if((i + 1) % 4 == 0){
                std::cout << "\n";
            }
    }
    std::cout << '\n' << "=== Memory ===\n";
    for (int i = 0; i < 256; ++i)
    {
        std::cout << memory.getCell(i) << "  ";
            if((i + 1) % 16 == 0){
                std::cout << "\n";
            }
    }
    std::cout << '\n' << "=== Screen ===\n" << "ASCII: " << screenASCII << '\n' << "HEX: " << screenHex << '\n';
}

////////////////////////////////////////////////////////////////////////////////////////////////////

std::string choice;
void VoleMain::init(Memory& mem , Register& reg)
{
    VoleMain::printMenu();
    VoleMain::takeInput();
    VoleMain::handleChoice(choice,mem, reg);
}

void VoleMain::printMenu()
{
    std:: cout<< "Choose one of the following choices : \n a)Read file and Load program \n b)Excute the program \n c)Run step \n d)Display the content \n e)Exit" << std :: endl;

}

void VoleMain::takeInput()
{
    std :: cin >> choice;
    choice = tolower(choice[0]);
    while (choice != "a" && choice != "b" && choice != "c" && choice != "d" && choice != "e")
    {
        std :: cout << "Please enter a valid input either a or b or c or d : \n a)Read file and Load program \n b)Excute the program \n c)Run step \n d)Display the content \n e)Exit" << std :: endl;
        std :: cin >> choice;
    }

}

void VoleMain::handleChoice(std::string& choice, Memory& mem, Register& reg)
{
    Machine machine;
    std::string fileName;
    std::string address;
    while (choice != "e")
    {
        if(choice == "a")
            {
                std::cout << "Enter the filename \n";
                std::cin >> fileName;
                std::cout << "Enter the memory cell address to store the program at ";
                std::cin >> address;
                address[0] = std::toupper(address[0]);
                address[1] = std::toupper(address[1]);
                try
                {
                    machine.loadProgram(fileName, address);
                }
                catch(const std::exception& e)
                {
                    std::cerr << e.what() << '\n';
                }
            }
        if(choice == "b")
        {
            machine.cpu.setPC(ALU::HexToDec(address));
            Machine::screenASCII = "";
            Machine::screenHex = "";
            for (int i = 0; i < 256; i += 2)
            {
                machine.cpu.fetch(machine.memory);
                try
                {
                    machine.cpu.decode(machine.memory, machine.cpu.reg);
                }
                catch (const std::exception& e)
                {
                    std::cerr << e.what() << "\n\n";
                    break;
                }
                if (CPU::isHalt)
                {
                    CPU::isHalt = false;
                    break;
                }
            }
            machine.printState();
        }
        if(choice == "c")
        {
            machine.cpu.fetch(machine.memory);
            try
            {
                machine.cpu.decode(machine.memory, machine.cpu.reg);
            }
            catch (const std::exception& e)
            {
                std::cerr << e.what() << "\n\n";
            }
            if (CPU::isHalt)
            {
                CPU::isHalt = false;
                break;
            }
            machine.printState();
        }
        if (choice == "d")
        {
            machine.printState();
        }
        VoleMain::printMenu();
        VoleMain::takeInput();
    }
}



