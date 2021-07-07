// demo code by J. Kaltenmark


#include "GameEngine.h"
#include <iostream>

int main()
{
    std::cout << " -- Easer Engine --\n";

	GameEngine engine;
	while (engine.isStarted())
	{
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
}
