// SYCORE
#include "syscore.h"

#define SYSCORE_BENCHMARK_IMPLEMENTATION 1
#include "sc_benchmark.h"

// COMPONENTS
#define KO_CLIENT_IMPLEMENTATION 1
#include "../ko_client/ko_client.h"

namespace global
{
    KO_CLIENT ko_client;
}
#include "../dynamic/dynamic.cpp"



int main()
{
    while (1)
    {
        dynamic_loop();
    }

    return 0;
}
