
#include <iostream>
#include <thread>
#include "GetMediaInfo.h"
#include "Utils.h"


int main(int argc, char *argv[]){
	
	if (argc < 2) {
		std::cout << "Usage : [program] [videofile]" << std::endl;
		return -1;
	}

	char *exec = remove_exe_suffix(argv[0]);

	for (int i = 1; i < argc; i++) {
		char logName[256] = { 0 };
		sprintf(logName, "%s_%d_info.log", exec, i);

		std::thread getMediaInfoThread(getMediaThreadInfo, argv, i, logName);
		getMediaInfoThread.join();
	}

	getchar();
	return 0;
}


