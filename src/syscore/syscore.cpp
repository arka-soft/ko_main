
// SYCORE
#include "syscore.h"

#define SYSCORE_BENCHMARK_IMPLEMENTATION 1
#include "sc_benchmark.h"

#define SYSCORE_KEYS_IMPLEMENTATION 1
#include "sc_keys.h"
#include "sc_log.h"

// COMPONENTS
#define KO_CLIENT_IMPLEMENTATION 1
#include "../ko_client/ko_client.h"

namespace global
{
     KO_CLIENT ko_client;
}

#include "../dynamic/dynamic.cpp"

int main( )
{
     while(1)
     {
          global::ko_client.send_spike_until_in_cooldown( );
          global::ko_client.send_thrust_until_in_cooldown( );
          global::ko_client.send_pierce_until_in_cooldown( );
          global::ko_client.send_cut_until_in_cooldown( );
          global::ko_client.send_shock_until_in_cooldown( );
          global::ko_client.send_jab_until_in_cooldown( );

          Sleep(50);
     }
     return 0;
}
