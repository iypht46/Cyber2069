#pragma once
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

#define LogCustomType_DC(TYPE)	\
template<typename OStream>	\
friend OStream& operator<<(OStream& os, const TYPE& obj)

#define LogCustomType_DF(TYPE)	\
template<typename OStream>	\
OStream& operator<<(OStream& os, const TYPE& obj)