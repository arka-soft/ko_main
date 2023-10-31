#define KO_CLIENT_IMPLEMENTATION 1
#include "ko_client/ko_client.h"
#include <chrono>
#include <iostream>
#include <thread>

KO_CLIENT g_ko_client;

// Only for testing purpose, this does not belong to the real base.
void TestSkills() {
  while (true) {
    Sleep(100);

    if (g_ko_client.get_spike_cooldown() > 0) {
      std::cout << "Spike: " << g_ko_client.get_spike_cooldown() << std::endl;
    }

    if (g_ko_client.get_thrust_cooldown() > 0) {
      std::cout << "Thrust: " << g_ko_client.get_thrust_cooldown() << std::endl;
    }

    if (g_ko_client.get_pierce_cooldown() > 0) {
      std::cout << "Pierce: " << g_ko_client.get_pierce_cooldown() << std::endl;
    }

    if (g_ko_client.get_cut_cooldown() > 0) {
      std::cout << "Cut: " << g_ko_client.get_cut_cooldown() << std::endl;
    }

    if (g_ko_client.get_shock_cooldown() > 0) {
      std::cout << "Shock: " << g_ko_client.get_shock_cooldown() << std::endl;
    }

    if (g_ko_client.get_jab_cooldown() > 0) {
      std::cout << "Jab: " << g_ko_client.get_jab_cooldown() << std::endl;
    }

    if (g_ko_client.get_stab2_cooldown() > 0) {
      std::cout << "Stab2: " << g_ko_client.get_stab2_cooldown() << std::endl;
    }

    if (g_ko_client.get_stab_cooldown() > 0) {
      std::cout << "Stab: " << g_ko_client.get_stab_cooldown() << std::endl;
    }

    if (g_ko_client.get_stroke_cooldown() > 0) {
      std::cout << "Stroke: " << g_ko_client.get_stroke_cooldown() << std::endl;
    }

    if (g_ko_client.get_lightfeet_cooldown() > 0) {
      std::cout << "Lightfeet: " << g_ko_client.get_lightfeet_cooldown()
                << std::endl;
    }
  }
}
int main() {

  g_ko_client.print_info();
  std::thread t1(TestSkills);
  t1.join();

  return 0;
}