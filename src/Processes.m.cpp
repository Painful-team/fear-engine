#include <core/Engine.hpp>

int main()
{
	FearEngine::Engine engine;
	if (engine.init() != 0)
	{
		return -1;
	}

	engine.run();

	return 0;
}
