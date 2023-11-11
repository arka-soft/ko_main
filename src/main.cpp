#define KO_CLIENT_IMPLEMENTATION 1
#include "ko_client/ko_client.h"
#include <chrono>
#include <iostream>
#include <thread>

KO_CLIENT g_ko_client;

void Spike()
{
    while (true)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        std::cout << g_ko_client.get_spike_cooldown() << std::endl;
    }
}

int main()
{

    g_ko_client.print_info();
    std::thread t1(Spike);
    t1.join();

    return 0;
}