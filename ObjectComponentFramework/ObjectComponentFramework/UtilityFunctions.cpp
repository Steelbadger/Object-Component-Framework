#include "UtilityFunctions.h"
#include <time.h>
#include <sstream>

void Error(std::string output)
{
	HANDLE h = GetStdHandle(STD_ERROR_HANDLE);
	SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_INTENSITY);
	std::cerr << "Error:\t" << output << std::endl;
	SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);
}

void Warning(std::string output)
{
	HANDLE h = GetStdHandle(STD_ERROR_HANDLE);
	SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	std::cerr << "Warning:\t" << output << std::endl;
	SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);
}

void Info(std::string output)
{
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(h, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	std::cout << output << std::endl;
	SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);
}

void TimerMark(bool output)
{
	static double mark(0);
	double oldMark = mark;
	mark = clock();
	if (output) {
		std::stringstream out;
		out << "Time between Marks: " << mark-oldMark << "ms";
		Info(out.str());
	}
}
