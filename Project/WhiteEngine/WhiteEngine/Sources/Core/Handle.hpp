#pragma once

#include <memory>

#define UNIQUE_HANDLE(TYPE)	std::unique_ptr<TYPE>;
#define MAKE_UNIQUE_HANDLE(TYPE)	std::make_unique<TYPE>;