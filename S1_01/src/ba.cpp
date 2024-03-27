#include "ba.h"


/* Début de BA */


int bapple::main() {
	size_t fileSize = 0;
	u8 *data = loadFileAsByteArray("ba.bin", &fileSize);

	if(data) {
		clearConsole();
		COORD p;
		getConsolePos(&p);

		for(u32 frame = 0; frame < (fileSize / 0xA8); frame++) {
			takeConsoleToPos(p);
			std::string lines = "";
			for(int y = 0; y < 32; y++) {
				for(int x = 0; x < 42; x++) {
					int i = (y * 42) + x;
					int idx = (i / 8) + (frame * 0xA8);
					int num = (7 - (i % 8));

					u8 val = (data[idx] >> num) & 1;

					if(val == 0) lines += "  ";
					else		 lines += "* ";
				}
				lines += "\n";
			}
			std::cout << lines;

			Sleep(33); // 30 fps -> 1 / 30 = 0.033s = 33ms
		}

		delete[] data;
	}
	else {
		std::cout << "Impossible de trouver \"ba.bin\"" << std::endl;
	}

	Sleep(1000);


	return 0;
}