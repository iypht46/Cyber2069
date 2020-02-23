#pragma once

#include <memory>

#define UNIQUE_HANDLE(TYPE)	std::unique_ptr<TYPE>;
#define SHARED_HANDLE(TYPE) std::shared_ptr<TYPE>;
#define WEAK_HANDLE(TYPE) std::weak_ptr<TYPE>;