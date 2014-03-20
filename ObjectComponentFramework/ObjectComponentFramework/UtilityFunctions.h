#pragma once
#include <iostream>
#include <string>
#include <Windows.h>

void Error(std::string output);
void Warning(std::string output);
void Info(std::string output);
void TimerMark(bool output = false);