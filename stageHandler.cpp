#include "structs.h"

static class StageHandler {
private:

	Stage stages[];
public:

	void loadAllMaps(char* filepath) {
		// Loop through maps in the filepath, call readFile
	}

	char *readFile(const char *filename) {
		/*char *buffer = 0;
		unsigned long length;
		FILE *file = fopen(filename, "rb");

		if (file) {
			fseek(file, 0, SEEK_END);
			length = ftell(file);
			fseek(file, 0, SEEK_SET);
			
			buffer = malloc(length + 1);
			memset(buffer, 0, length + 1);
			fread(buffer, 1, length, file);
			
			fclose(file);

			buffer[length] = '\0';
		}*/
	}

	Stage getMap(int index) {
		return stages[index];
	}
};
