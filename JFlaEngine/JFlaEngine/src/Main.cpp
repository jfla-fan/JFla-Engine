#include "Application/Application.h"
#include <iostream>


int main(int argc, char* argv[])
{
	Engine::Application app("My app", 640, 580);

	app.Run();
	
	return 0;
}

#undef main