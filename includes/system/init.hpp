#pragma once
#include <base/common.hpp>

bool initSDL(std::string title);
void cleanup(void);
int throw_sdl_err(const char* fmt);