#include <stdio.h>
#include <string>
#include <thread>

#include "assembler.h"
#include "memory.h"
#include "cpu.h"

int main(int argc, char* argv[])
{
	std::string sourceFileName = "";
	if (argc < 2)
	{
		sourceFileName = "source.8085";
		//printf("Usage: %s source [out]\n", argv[0]);
		//std::cin.get();
		//return 1;
	}
	else
	{
		sourceFileName = argv[1];
	}

	std::string outFileName = "out.bin";

	if (argc > 2)
	{
		outFileName = argv[2];
	}

	uint8_t* memory_data = Assembler::GetAssembledMemory(sourceFileName);

	CPU* cpu = new CPU(memory_data, 0xffff);
	

	/*for (int i = 0x0900; i < 0x09FF; i++)
	{
		printf("%s: %s\n", IntToHex(i).c_str(), IntToHex(cpu->GetMemory()->GetDataAtAddr(i), 2).c_str());
	}*/

	std::thread t = cpu->Run();

	t.join();
	
	for (int i = 0x0900; i < 0x090f; i++)
	{
		printf("%c",	cpu->GetMemory()->GetDataAtAddr(i));
	}

	printf("\n");

	return 0;
}