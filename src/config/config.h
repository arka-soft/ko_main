#pragma once
#include <iostream>
#include "../syscore/syscore.h"

extern "C" __declspec(dllexport) void config_setup(NODE_SHARED_MEMORY& node_shared_mem);
extern "C" __declspec(dllexport) void config_loop(NODE_SHARED_MEMORY& node_shared_mem);
