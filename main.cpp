#include <iostream>
#include <fstream>
#include <vector>
#include <bitset>
#include <iomanip>

//RegistersStack struct
struct RegistersStack {
	uint8_t A = 0; // 4bits
	uint8_t B = 0; // 4bits
	uint8_t PC = 0; //program counter(4bit)
	bool carry = false; //carry flag
	uint8_t IN = 0;
	uint8_t OUT = 0;
};

std::vector<uint8_t> getInstructions(const std::string& path) {
	std::ifstream file(path, std::ios::binary);	//input file stream as a binary
	std::vector<uint8_t> instr_cons; //initiate vector to store consequence of instructions
	if (!file) {	//check if opened properly
		std::cout << "Unable to load file." << std::endl;
		return instr_cons;
	}
	char read_byte;	//init 1 byte for reading
  int count = 0;
	while (file.read(&read_byte, 1)) { 
    instr_cons.push_back(static_cast<uint8_t>(read_byte)); }
	return instr_cons;
}

void printRegsStack(const RegistersStack& instr) {
	std::cout << "Registers stack info\n\t-A: " << std::bitset<4>(instr.A) <<
		"\n\t-B: " << std::bitset<4>(instr.B) <<
		"\n\t-PC: " << std::bitset<4>(instr.PC) <<
		"\n\t-Carry: " << std::bitset<4>(instr.carry) <<
		"\n\t-Input: " << std::bitset<4>(instr.IN) <<
		"\n\t-Output: " << std::bitset<4>(instr.OUT);
}

void instrExecution(RegistersStack& proc, const std::vector<uint8_t>& program) {
	uint8_t instruction = program[proc.PC];
	uint8_t operation_code = (instruction >> 4) & 0xF; //get first 4 bits are stand for operation code
	uint8_t operand = instruction & 0xF; // last 4 bits are stand for operand
	uint8_t res;
	std::cout << "\nCommand " << std::bitset<8>(instruction) << " is being executed" << std::endl;

	switch (operation_code) {
		//Add section
	case 0b0000:	//Add A Im
		res = proc.A + operand;
		proc.carry = res > 0xF;
		proc.A = res & 0xF;
		std::cout << "ADD A, " << std::bitset<4>(operand) << std::endl;
		break;
	case 0b0101:	//Add B Im
		res = proc.B + operand;
		proc.carry = res > 0xF;
		proc.B = res & 0xF;
		std::cout << "ADD B, " << std::bitset<4>(operand) << std::endl;
		break;
	case 0b1000: // ADD A, B
		res = proc.A + proc.B;
		proc.carry = res > 0xF;	//true if bigger than 15
		proc.A = res & 0xF;	//0101 & 1111
		std::cout << "ADD A, B" << std::endl;
		break;
	case 0b1101: // ADD B, A
		res = proc.B + proc.A;
		proc.carry = (res > 0xF);	//true if bigger than 15
		proc.B = res & 0xF;	//0101 & 1111
		std::cout << "ADD B, A" << std::endl;
		break;

		//MOV section
	case 0b0001:	//MOV A, B
		proc.A = proc.B;
		std::cout << "MOV A, B" << std::endl;
		break;
	case 0b0100:	//MOV B, A
		proc.B = proc.A;
		std::cout << "MOV B, A" << std::endl;
		break;
	case 0b0011: // MOV A, im
		proc.A = operand;
		std::cout << "MOV A, " << std::bitset<4>(operand) << std::endl;
		break;
	case 0b0111: // MOV B, im
		proc.B = operand;
		std::cout << "MOV B, " << std::bitset<4>(operand) << std::endl;
		break;

		//jump section
	case 0b1111: // JMP
		proc.PC = operand;
		std::cout << "JMP " << std::bitset<4>(operand) << std::endl;
		return; //to not inc PC
	case 0b1110: // Jump if no carry
		if (!proc.carry) {
			proc.PC = operand;
			std::cout << "JMC " << std::bitset<4>(operand) << ". Jumping, no carry" << std::endl;
			return;
		}
		else { std::cout << "JMC " << std::bitset<4>(operand) << ". No jump, there is carry" << std::endl; }
		break;
		//case 0b1010:	//JZ

		//	break;

			//I/O section
	case 0b0010: // IN A
    proc.A = proc.IN;
		std::cout << "IN A" << std::endl;
		break;


	case 0b0110: // IN B
    proc.B = proc.IN;
		std::cout << "IN B" << std::endl;
		break;

	case 0b1100: // OUT A
		proc.OUT = proc.A;
		std::cout << "OUT A" << std::endl;
		break;
	case 0b1001: // OUT B
		proc.OUT = proc.B;
		std::cout << "OUT A" << std::endl;
		break;
	case 0b1011: // OUT Im
		proc.OUT = operand;
		std::cout << "OUT Im" << std::endl;
		break;
	default:
		std::cout << "Unknown command!" << std::endl;
		break;
	}

	proc.PC = (proc.PC + 1) & 0xF; // Inc PC
}

int main() {
	RegistersStack proc;
	std::string filename;

	std::cout << "Type a path to the binary file: ";
	try {
		std::cin >> filename;

		//Program load
		auto program = getInstructions(filename);
		if (program.empty()) { std::cout<<"File is empty."<<std::endl; return 1; }

		std::cout << "Program's loaded" << std::endl;

		// RegistersStack operations
		while (proc.PC < program.size()) {
			printRegsStack(proc);
			std::cout << "\nPress ENTER to execute a new command";
			std::cin.get();
			instrExecution(proc, program);
		}
	}
	catch (std::exception& e) { std::cout << "An error occured. Info: " << e.what() << std::endl; }
	catch (...) { "Unknown error occured."; }
	return 0;
}
