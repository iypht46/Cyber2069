#include "Sources/Tools.hpp"

int main(int argc, char * argv[])
{
	Tools::Init();

	Tools::Loop();

	Tools::Terminate();

	return 0;
}