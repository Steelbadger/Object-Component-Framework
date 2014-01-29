#include "Application.h"


int main(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	Application program;

	// Initialize and run the program
	bool result = program.Initialize();
	if(result)
	{
		program.Run();
	}

	// Shutdown
	program.Shutdown();
	return 0;
}
