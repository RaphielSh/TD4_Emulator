#include <iostream>
#include <fstream>
#include <vector>
#include <bitset>
#include <iomanip>

//Registers struct
struct TD4 { 
	uint8_t A = 0; // 4bits
	uint8_t B = 0; // 4bits
	uint8_t PC = 0; //command counter(4bit)
	bool carry = false; //carry flag
	uint8_t IN = 0;
	uint8_t OUT = 0;
};

std::vector<uint8_t> loadProgram(const std::string& filename) { 
	std::ifstream file(filename, std::ios::binary);
	std::vector<uint8_t> program;
	
	if(!file){
		throw(std::error)
		return program;
	}
	
	char byte;
	while (file.read(&byte, 1)) {
		program.push_back(static_cast<uint8_t>(byte));
	}
	
	return program;
}

//Функция для вывода состояния регистров
void printRegisters(const TD4& cpu) { 
	std::cout << "A: " <<std::bitset<4>(cpu.A) << "(" <<int(cpu.A) << ") " 
	<< "B: " <<std::bitset<4>(cpu.B) << "(" <<int(cpu.B) << ") "
	<< "PC: " <<std::bitset<4>(cpu.PC) << "(" <<int(cpu.PC) << ") "
	<< "Carry: " << cpu.carry <<  " "
	<< "IN: " <<std::bitset<4>(cpu.IN) << " "
	<< "OUT: " <<std::bitset<4>(cpu.OUT) << "\n";
}

//Выполнение одной инструкции
void executeInstruction(TD4& cpu, const std::vector<uint8_t>& program) {
	uint8_t instruction = program[cpu.PC];
	uint8_t opcode = (instruction >> 4) & 0xF; //Первые 4 бита - код операции
	uint8_t operand = instruction & 0xF; // Последние 4 бита - операнд
	
	std::cout << "Выполняется команда: " << std::bitset<8>(instruction) << "\n";
	
	switch (opcode) {
		case 0x0: // MOV A, im
			cpu.A = operand;
			std::cout << "MOV A, " << std::bitset<4>(operand) << "\n";
			break;
		case 0x1: // MOV B, im
			cpu.A = operand;
			std::cout << "MOV A, " << std::bitset<4>(operand) << "\n";
			break;
		case 0x2: // MOV A, B
		    {
		        uint8_t result = cpu.A + cpu.B;
		        cpu.carry = (result > 0xF);
		        cpu.A = result & 0xF; // Результат 4 бита
		        std::cout << "ADD A, B\n";
		    }
		break;
		case 0x3: // JMP im
		    cpu.PC = operand;
		    std::cout << "JMP " << std::bitset<4>(operand) << "\n";
		    return; // Прерывает увеличение PC, так ка произошел переход
		case 0x4: // JMC im
		    if (!cpu.carry){
		        cpu.PC = operand;
		        std::cout << "JMC " << std::bitset<4>(operand) << "(переноса нет, переход)\n";
		        return;
		    } else {
		        std::cout << "JMC " << std::bitset<4>(operand) << "(перенос есть, нет перехода)\n";
		    }
		    break;
		case 0x5: // IN A
		    cpu.A = cpu.IN;
		    std::cout << "IN A\n";
		    break;
		case 0x6: // OUT A
		    cpu.OUT = cpu.A;
		    std::cout << "OUT A\n";
		    break;
		default:
		    std::cerr << "Неизвестная команда!\n";
		    break;
		}
		
		cpu.PC = (cpu.PC + 1) & 0xF; // Счетчик команд (4бита)
	}
	
	int main() {
		TD4 cpu;
		std::string filename;
		
		std::cout << "Type a path to the binary file: ";
		try
		{std::cin >> filename;
		
		//Program load
		auto program = loadProgram(filename);
		if(program.empty()) {return 1;}
		
		std::cout << "Program's loaded. Size: " << program.size() << " bites.\n";
		
		// TD4 operations
		while (cpu.PC < program.size()) {
			printRegisters(cpu);
			std::cout << "Press ENTER to execute a new command";
			std::cin.get();
			executeInstruction(cpu, program);
		}
		}catch(std::exception& e){std::cout<<"An error occured. Info: "<<e.what()<<std::endl;}
		catch(...){"Unknown error occured.";}
		return 0;
	}
	
	
	
	







