#include <iostream>
#include <fstream>
#include <vector>
#include <bitset>
#include <iomanip>

//Registers struct
struct Registers { 
	uint8_t A = 0; // 4bits
	uint8_t B = 0; // 4bits
	uint8_t PC = 0; //command counter(4bit)
	bool carry = false; //carry flag
	uint8_t IN = 0;
	uint8_t OUT = 0;
};

std::vector<uint8_t> getInstructions(const std::string& path) { 
	std::ifstream file(path, std::ios::binary);	//input file stream as a binary
	if(!file){	//check if opened properly
		std::cout<<"Unable to load file."<<std::endl;
		return;
	}

	std::vector<uint8_t> instr_cons; //initiate vector to store consequence of instructions
	
	char read_byte;	//init 1 byte for reading
	while (file.read(&read_byte, 1)) { instr_cons.push_back(static_cast<uint8_t>(read_byte)); }
	return instr_cons;
}

void printRegs(const Registers )

void printRegisters(const Registers& cpu) { 
	std::cout << "A: " <<std::bitset<4>(cpu.A) << "(" <<int(cpu.A) << ") " 
	<< "B: " <<std::bitset<4>(cpu.B) << "(" <<int(cpu.B) << ") "
	<< "PC: " <<std::bitset<4>(cpu.PC) << "(" <<int(cpu.PC) << ") "
	<< "Carry: " << cpu.carry <<  " "
	<< "IN: " <<std::bitset<4>(cpu.IN) << " "
	<< "OUT: " <<std::bitset<4>(cpu.OUT) << "\n";
}

void executeInstruction(Registers& cpu, const std::vector<uint8_t>& program) {
	uint8_t instruction = program[cpu.PC];
	uint8_t operation_code = (instruction >> 4) & 0xF; //get first 4 bits are stand for operation code
	uint8_t operand = instruction & 0xF; // last 4 bits are stand for operand
	
	std::cout << "Command to execute: " << std::bitset<8>(instruction) << "\n";
	
	switch (opcode) {
		case 0x0: // MOV A, im
			cpu.A = operand;
			std::cout << "MOV A, " << std::bitset<4>(operand) <<std::endl;
			break;
		case 0x1: // MOV B, im
			cpu.A = operand;
			std::cout << "MOV B, " << std::bitset<4>(operand) << std::endl;
			break;
		case 0x2: // ADD A, B
	        uint8_t result = cpu.A + cpu.B;
	        cpu.carry = (result > 0xF);
	        cpu.A = result & 0xF;
		    std::cout << "ADD A, B"<<std::endl;
			break;
		case 0x3: // JMP im
		    cpu.PC = operand;
		    std::cout << "JMP " << std::bitset<4>(operand) <<std::endl;
		    return;
		case 0x4: // JMC im
		    if (!cpu.carry){
		        cpu.PC = operand;
		        std::cout << "JMC " << std::bitset<4>(operand) << ". No carry"<<std::endl;
		        return;
		    } else { std::cout << "JMC " << std::bitset<4>(operand) << ". Carry"<<std::endl; }
		    break;
		case 0x5: // IN A
		    cpu.A = cpu.IN;
		    std::cout << "IN A"<<std::endl;
		    break;
		case 0x6: // OUT A
		    cpu.OUT = cpu.A;
		    std::cout << "OUT A"<<std::endl;
		    break;
		default:
		    std::cout<<"Unknown command!"<<std::endl;
		    break;
		}
		
		cpu.PC = (cpu.PC + 1) & 0xF; // Счетчик команд (4бита)
	}
	
	int main() {
		Registers cpu;
		std::string filename;
		
		std::cout << "Type a path to the binary file: ";
		try
		{
      std::cin >> filename;
		
		  //Program load
		  auto program = getProgram(filename);
		  if(program.empty()) {return 1;}
		
		  std::cout << "Program's loaded. Size: " << program.size() << " bites.\n";
		
		  // Registers operations
		  while (cpu.PC < program.size()) {
			  printRegisters(cpu);
			  std::cout << "Press ENTER to execute a new command";
			  std::cin.get();
			  executeInstruction(cpu, program);
		  }
		}
    catch(std::exception& e){std::cout<<"An error occured. Info: "<<e.what()<<std::endl;}
		catch(...){"Unknown error occured.";}
		return 0;
	}
	
	
	
	







