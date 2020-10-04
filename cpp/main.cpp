#include "system.h"

int userMain(int argc, char* argv[]);

int main(int argc, char* argv[]) {
	System::load();
	int ret = userMain(argc, argv);
	System::unload();
	return ret;
}
