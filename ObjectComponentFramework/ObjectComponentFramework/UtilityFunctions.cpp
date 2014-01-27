#include "UtilityFunctions.h"

void Error(std::string output)
{
	HANDLE h = GetStdHandle(STD_ERROR_HANDLE);
	SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_INTENSITY);
	std::cerr << "Error: " << output << std::endl;
	SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);
}

void Warning(std::string output)
{
	HANDLE h = GetStdHandle(STD_ERROR_HANDLE);
	SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	std::cerr << "Warning: " << output << std::endl;
	SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);
}