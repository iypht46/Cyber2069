#pragma once

#include <memory>

#define UNIQUE_HANDLE(TYPE)	std::unique_ptr<TYPE>;
#define SHARED_HANDLE(TYPE) std::shared_ptr<TYPE>;
#define WEAK_HANDLE(TYPE) std::weak_ptr<TYPE>;

#define MAKE_UNIQUE(TYPE) std::make_unique<TYPE>;
#define MAKE_SHARED(TYPE) std::make_shared<TYPE>;