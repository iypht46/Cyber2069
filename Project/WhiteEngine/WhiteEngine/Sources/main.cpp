#include "Core/World.hpp"

int main(int argc, char * argv[])
{
	World::Init();

	World::Loop();

	World::Terminate();

	return 0;
}