#include "config.h"

void config_setup(NODE_SHARED_MEMORY& node_shared_mem)
{
    std::cout << "Running: Setup" << std::endl;
    std::cout << "X : " << node_shared_mem.x << std::endl;
    node_shared_mem.x = 0;
    node_shared_mem.y = 0;
}

void config_loop(NODE_SHARED_MEMORY& node_shared_mem)
{
    std::cout << "Running: Loop" << std::endl;
    std::cout << "X : " << node_shared_mem.x-- << std::endl;
    std::cout << "Y : " << node_shared_mem.y++ << std::endl;
}

