#include <iostream>
#include "../syscore/syscore.h"

#define printfR() printf("\033[31m");
#define printfB() printf("\033[34m");
#define printfK() printf("\033[0m");

void dynamic_loop()
{
    TICTOC loop_timer;
    loop_timer.tic();

    // This is some temporary code that implements a terminal based display
    int bar_scale = 2;
    
    int32_t cur_hp = global::ko_client.get_player_cur_hp();
    int32_t max_hp = global::ko_client.get_player_max_hp();
    double  per_hp = (double) cur_hp / max_hp;
    int hp_bar =  per_hp * 10 * bar_scale;

    int32_t cur_mp = global::ko_client.get_player_cur_mp();
    int32_t max_mp = global::ko_client.get_player_max_mp();
    double  per_mp = (double) cur_mp / max_mp;
    int mp_bar = per_mp * 10 * bar_scale;

    printfR();
    printf("HP:[");
    for(int i = 0; i < hp_bar * bar_scale; ++i)
        printf("#");
    for(int i = 0; i < (10*bar_scale-hp_bar); ++i)
        printf("  ");

    printf("] [%d / %d]\n", cur_hp, max_hp);
    printfK();

    printfB();
    printf("MP:[");
    for(int i = 0; i < mp_bar * bar_scale; ++i)
        printf("#");
    for(int i = 0; i < (10*bar_scale-mp_bar); ++i)
        printf("  ");

    printf("] [%d / %d]\n", cur_mp, max_mp);
    printfK();



    printf("\n--------------------------------\n");
    printf("SPIKE : %.2f\n", global::ko_client.get_spike_cooldown());
    printf("THRUST: %.2f\n", global::ko_client.get_thrust_cooldown());
    printf("PIERCE: %.2f\n", global::ko_client.get_pierce_cooldown());


    loop_timer.toc();
    double elapsed_time = loop_timer.elapsed_time_in_ms();
    uint32_t fps = 1000 / elapsed_time;
    printf("Running at %d FPS.\n",fps);
    std::cout << "\x1B[2J\x1B[H"; // Clear the screen.
}

