#ifndef KO_CLIENT_H
#define KO_CLIENT_H
#include "../syscore/syscore.h"
#include "config/ardream_world_memory_config.h"
#include "kc_memutils.h"

#include <cassert>
#include <chrono>
#include <iostream>
#include <iterator>
#include <stdint.h>
#include <tchar.h>
#include <tlhelp32.h>

enum class PLAYER_RACE : uint8_t
{
     KARUS    = 62,     // KARUS identification byte.
     EL_MORAD = 38      // EL MORAD identification byte.
};

/**
 * @class KnightOnline
 *
 * @brief A class representing interactions with the Knight Online game
 * process.
 *
 * The `KnightOnline` class encapsulates functionality related to interacting
 * with the Knight Online game process, including process identification,
 * memory access, and resolving memory addresses for game-related data.
 */
class KO_CLIENT
{
     // Data Section
                                                                           public:
                                                                           private:
     HANDLE process_handle;
     DWORD  process_id;

     PLAYER_RACE player_race;

     KO_MEM_ADR spike_cooldown_ptr;
     KO_MEM_ADR thrust_cooldown_ptr;
     KO_MEM_ADR pierce_cooldown_ptr;
     KO_MEM_ADR cut_cooldown_ptr;
     KO_MEM_ADR shock_cooldown_ptr;
     KO_MEM_ADR jab_cooldown_ptr;
     KO_MEM_ADR stab_cooldown_ptr;
     KO_MEM_ADR stab2_cooldown_ptr;
     KO_MEM_ADR stroke_cooldown_ptr;

     KO_MEM_ADR player_max_hp_ptr;
     KO_MEM_ADR player_cur_hp_ptr;
     KO_MEM_ADR player_max_mp_ptr;
     KO_MEM_ADR player_cur_mp_ptr;

     // Method Section
                                                                           private:
     /**
   * @brief Get the process ID by matching the process name.
   *
   * This function retrieves the process ID of a running process by matching
   * its name with the provided process_name. Returns 0 if it fails.
   *
   * @param process_name
   * @return DWORD
   */
     DWORD get_process_id_by_client_name(const char* process_name);

     /**
   * @brief Finds the player race from the KO memory and returns it.
   *
   * @param ko_memory_ref Reference to the mapped KO memory object
   * @param conf Reference to the KO memory config
   * @return PLAYER_RACE
   */
     PLAYER_RACE find_player_race(PROCESS_MEMORY& ko_memory_ref, KO_MEMORY_CONFIG& conf);

     /**
   * @brief A generic function that finds the skill cooldown by searching a
   * skill byte pattern.
   *
   * @param ko_memory_ref Reference to the mapped KO memory object
   * @param conf Reference to the KO memory config
   * @param skill_byte_pattern Pointer to the beginning of the byte pattern to
   * search
   * @param byte_pattern_size Size of the pattern to search for
   * @return KO_MEM_ADR A pointer to the first match to the pattern, expressed
   * in the address space of KnighOnline.
   */
     KO_MEM_ADR find_skill_cooldown_ptr_generic(PROCESS_MEMORY& ko_memory_ref, KO_MEMORY_CONFIG& conf, KO_MEM_BYTE* skill_byte_pattern, size_t byte_pattern_size);

/**
 * @brief A utility macro to call find_skill_cooldown_ptr_generic for a skill
 * name.
 */
#define find_skill_cooldown_ptr(ko_memory_ref, conf, skill_name) find_skill_cooldown_ptr_generic(ko_memory_ref, conf, conf.skill_name##_byte_pattern, sizeof(conf.skill_name##_byte_pattern));

     /**
   * @brief  Finds the patterns for the player health and mana information in
   * the KO memory, and assings them to member variables.
   *
   * @param ko_memory_ref
   * @param conf
   */
     void assign_player_health_and_mana_ptr(PROCESS_MEMORY& ko_memory_ref, KO_MEMORY_CONFIG& conf);

     /**
    * @brief Sends the specified skill with a retry mechanism to handle cooldown
    * inconsistencies.
    *
    * This function monitors and handles cooldown inconsistencies, automatically
    * retrying if necessary.
    *
    * @tparam skill The skill to be sent.
    *
    * @warning Assumes the existence of `float get_##skill##_cooldown()` and
    * `void send_raw_key(int key)`.
    *
    * @note Keeps track of the last x amount of previous cooldowns to ensure a
    * consistent decrease.
    *
    * This mechanism is designed to address issues when attempting to use a skill
    * while the previous skill's action is still ongoing. In such cases, the skill
    * activation may initially fail, causing the cooldown to spike to a maximum
    * value (e.g., 10 seconds), only to rapidly decrease to a negligible value
    * (epsilon) once the action is completed in a split second.
    *
    * Fast execution of this process results in a series of cooldown values, for
    * instance:
    *   - 10.9293
    *   - 10.9495
    *   - 10.0291
    *
    * The algorithm systematically examines the last x cooldown values to verify if
    * they exhibit a consistent decrease. A consistent decrease indicates
    * successful skill activation, allowing the system to proceed to the next skill
    * confidently.
    */
#define DEFINE_SEND_SKILL_UNTIL_IN_COOLDOWN_FUNC(skill)                                                                                                                                                \
     bool send_##skill##_until_in_cooldown( ) const noexcept                                                                                                                                           \
     {                                                                                                                                                                                                 \
          const float epsilon                       = 1e-6; /* Tolerance for floating-point number comparison */                                                                                       \
          const int   previous_cooldowns_count      = 3;    /* Number of previous cooldowns to consider */                                                                                             \
          const int   input_overwhelm_protection_ms = 50;   /* Protect against input lag */                                                                                                            \
          const int   max_duration_ms               = 3000; /* Timeout in ms */                                                                                                                        \
                                                                                                                                                                                                       \
          float current_cooldown = get_##skill##_cooldown( );                                                                                                                                          \
          float previous_cooldown;                                                                                                                                                                     \
          int   previous_decreasing_cooldown_count = 0; /* Track previous cooldowns */                                                                                                                 \
          auto  start_time                         = std::chrono::high_resolution_clock::now( );                                                                                                       \
                                                                                                                                                                                                       \
          if(current_cooldown > epsilon)                                                                                                                                                               \
          {                                                                                                                                                                                            \
               return false;                                                                                                                                                                           \
          } /* If skill is in cooldown, return */                                                                                                                                                      \
                                                                                                                                                                                                       \
          while(true)                                                                                                                                                                                  \
          {                                                                                                                                                                                            \
               send_multiple_keys(skill##_page, skill##_key); /* Attempt to activate the skill */                                                                                                      \
               previous_cooldown = current_cooldown;                                                                                                                                                   \
               current_cooldown  = get_##skill##_cooldown( ); /* Get current cooldown */                                                                                                               \
                                                                                                                                                                                                       \
               Sleep(input_overwhelm_protection_ms);                                                                                                                                                   \
                                                                                                                                                                                                       \
               const bool is_previous_cooldown_bigger_than_current_cooldown = previous_cooldown - current_cooldown > epsilon;                                                                          \
                                                                                                                                                                                                       \
               if(is_previous_cooldown_bigger_than_current_cooldown) /* Skill is active and cooldown is decreasing */                                                                                  \
               {                                                                                                                                                                                       \
                    previous_decreasing_cooldown_count++; /* Track decreasing cooldown count */                                                                                                        \
               }                                                                                                                                                                                       \
               else                                                                                                                                                                                    \
               {                                                                                                                                                                                       \
                    previous_decreasing_cooldown_count = 0; /* If the trail fails, start again */                                                                                                      \
                    continue;                                                                                                                                                                          \
               }                                                                                                                                                                                       \
                                                                                                                                                                                                       \
               bool is_cooldown_consistently_decreasing = previous_decreasing_cooldown_count == previous_cooldowns_count;                                                                              \
                                                                                                                                                                                                       \
               if(is_cooldown_consistently_decreasing) /* X amount of previous cooldowns is decreasing */                                                                                              \
               {                                                                                                                                                                                       \
                    send_raw_key(VK_R);                                                                                                                                                                \
                    return true;                                                                                                                                                                       \
               }                                                                                                                                                                                       \
                                                                                                                                                                                                       \
               auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now( ) - start_time);                                                     \
                                                                                                                                                                                                       \
               if(elapsed_time.count( ) >= max_duration_ms)                                                                                                                                            \
               {                                                                                                                                                                                       \
                    return false;                                                                                                                                                                      \
               } /* Timeout */                                                                                                                                                                         \
          }                                                                                                                                                                                            \
     }

/**
 * @brief A utility macro to define getter functions that read addresses from
 * the KO memory and returns them as float.
 */
#define DEFINE_FLOAT_GETTER_FUNC(function_name, variable_name)                                                                                                                                         \
     [[nodiscard]] inline float function_name( ) const noexcept                                                                                                                                        \
     {                                                                                                                                                                                                 \
          float f;                                                                                                                                                                                     \
          ReadProcessMemory(process_handle, variable_name, &f, sizeof(f), NULL);                                                                                                                       \
          return f;                                                                                                                                                                                    \
     }

/**
 * @brief A utility macro to define getter functions that read addresses from
 * the KO memory and returns them as uint32.
 */
#define DEFINE_UINT32_GETTER_FUNC(function_name, variable_name)                                                                                                                                        \
     [[nodiscard]] inline uint32_t function_name( ) const noexcept                                                                                                                                     \
     {                                                                                                                                                                                                 \
          uint32_t i;                                                                                                                                                                                  \
          ReadProcessMemory(process_handle, variable_name, &i, sizeof(i), NULL);                                                                                                                       \
          return i;                                                                                                                                                                                    \
     }

/**
 * @brief Defines getter and sender functions for a specific skill.
 *
 * This macro creates a pair of functions for a given skill: a getter to retrieve
 * the cooldown value and a sender to report skill hits based on cooldown status.
 * The getter is generated using DEFINE_FLOAT_GETTER_FUNC, and the sender using
 * DEFINE_SEND_SKILL_FUNC.
 *
 * @param skill The skill's name for which to define functions.
 *
 * @note The getter retrieves cooldown using the skill's associated variable,
 * and the sender uses the getter to determine hit reports (COOLDOWN or READY)
 * based on the current cooldown status.
 */
#define DEFINE_SKILL_FUNCTIONS(skill)                                                                                                                                                                  \
     DEFINE_FLOAT_GETTER_FUNC(get_##skill##_cooldown, skill##_cooldown_ptr); /*ie. defines get_spike_cooldown*/                                                                                        \
     DEFINE_SEND_SKILL_UNTIL_IN_COOLDOWN_FUNC(skill)                         /*ie. defines send_spike_until_in_cooldown*/

                                                                           public:
     /**
   * @brief Construct a new KnightOnline object and initialize process-related
   * data.
   *
   * This constructor creates a new instance of the `KnightOnline` class and
   * initializes essential process-related data. It first attempts to find the
   * "KnightOnLine.exe" process and waits for its identification, with a
   * maximum retry interval of 1 second between attempts. Once the process is
   * identified, it opens a handle to the process, and resolves the memory
   * address for pointers, process handle and ID.
   */
     KO_CLIENT( );

     ~KO_CLIENT( );

     [[nodiscard]] DWORD       get_process_id( ) const noexcept { return process_id; }
     [[nodiscard]] HANDLE      get_process_handle( ) const noexcept { return process_handle; }
     [[nodiscard]] PLAYER_RACE get_player_race( ) const noexcept { return player_race; }

     DEFINE_SKILL_FUNCTIONS(spike);
     DEFINE_SKILL_FUNCTIONS(thrust);
     DEFINE_SKILL_FUNCTIONS(pierce);
     DEFINE_SKILL_FUNCTIONS(jab);
     DEFINE_SKILL_FUNCTIONS(cut);
     DEFINE_SKILL_FUNCTIONS(shock);
     DEFINE_SKILL_FUNCTIONS(stab);
     DEFINE_SKILL_FUNCTIONS(stab2);
     DEFINE_SKILL_FUNCTIONS(stroke);

     // Player (Maybe later we can expand this to have a macro called
     // DEFINE_PLAYER_FUNCTIONS)
     DEFINE_UINT32_GETTER_FUNC(get_player_max_hp, player_max_hp_ptr);
     DEFINE_UINT32_GETTER_FUNC(get_player_cur_hp, player_cur_hp_ptr);
     DEFINE_UINT32_GETTER_FUNC(get_player_max_mp, player_max_mp_ptr);
     DEFINE_UINT32_GETTER_FUNC(get_player_cur_mp, player_cur_mp_ptr);

     inline void print_info( ) const noexcept;
};

#endif

#ifdef KO_CLIENT_IMPLEMENTATION
#pragma once

#define KC_MEMUTILS_IMPLEMENTATION 1
#include "kc_memutils.h"

KO_CLIENT::KO_CLIENT( )
{
     process_id = get_process_id_by_client_name("KnightOnLine.exe");

     // TODO: Add Safety Features
     process_handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, process_id);

     // Map the process memory to search for patterns in it
     double const ko_address_space_heap_starts_at = 0.2;     // GB (via manual inspection using vmmap)
     double const ko_address_space_heap_size      = 1;       // GB (via manual inspection using vmmap)
     KO_MEM_ADR   heap_base_address               = (KO_MEM_ADR) GB_TO_BYTES(ko_address_space_heap_starts_at);
     uint64_t     bytes_to_map                    = GB_TO_BYTES(ko_address_space_heap_size);

     PROCESS_MEMORY   ko_memory {process_handle, heap_base_address, bytes_to_map};
     KO_MEMORY_CONFIG ko_memory_config;

     // Assign the pointers
     player_race = find_player_race(ko_memory, ko_memory_config);

     spike_cooldown_ptr  = find_skill_cooldown_ptr(ko_memory, ko_memory_config, spike);
     thrust_cooldown_ptr = find_skill_cooldown_ptr(ko_memory, ko_memory_config, thrust);
     pierce_cooldown_ptr = find_skill_cooldown_ptr(ko_memory, ko_memory_config, pierce);
     cut_cooldown_ptr    = find_skill_cooldown_ptr(ko_memory, ko_memory_config, cut);
     shock_cooldown_ptr  = find_skill_cooldown_ptr(ko_memory, ko_memory_config, shock);
     jab_cooldown_ptr    = find_skill_cooldown_ptr(ko_memory, ko_memory_config, jab);
     stab2_cooldown_ptr  = find_skill_cooldown_ptr(ko_memory, ko_memory_config, stab2);
     stab_cooldown_ptr   = find_skill_cooldown_ptr(ko_memory, ko_memory_config, stab);
     stroke_cooldown_ptr = find_skill_cooldown_ptr(ko_memory, ko_memory_config, stroke);

     assign_player_health_and_mana_ptr(ko_memory, ko_memory_config);
}

KO_CLIENT::~KO_CLIENT( ) { CloseHandle(process_handle); }

DWORD KO_CLIENT::get_process_id_by_client_name(const char* process_name)
{
     // TODO: Add Safety Features
     HANDLE snapshot_handle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
     if(snapshot_handle == INVALID_HANDLE_VALUE)
     {
          return 0;
     }

     PROCESSENTRY32 process_entry { };
     process_entry.dwSize = sizeof(PROCESSENTRY32);

     DWORD result = 0;
     while(Process32Next(snapshot_handle, &process_entry))
     {
          if(_stricmp(process_entry.szExeFile, process_name) == 0)
          {
               result = process_entry.th32ProcessID;
               break;
          }
     }

     CloseHandle(snapshot_handle);
     return result;
}

PLAYER_RACE KO_CLIENT::find_player_race(PROCESS_MEMORY& ko_memory_ref, KO_MEMORY_CONFIG& conf)
{
     KO_MEM_ADR result = ko_memory_ref.find_pattern_in_memory(conf.player_nation_identification_byte_pattern, conf.KO_STRING_LENGTH_IN_BYTES);

     result += conf.player_nation_identification_offset_from_pattern;

     KO_MEM_BYTE nation_byte;
     ReadProcessMemory(process_handle, result, &nation_byte, sizeof(nation_byte), NULL);

     switch(nation_byte)
     {
          case conf.player_nation_human: return PLAYER_RACE::EL_MORAD;
          case conf.player_nation_karus: return PLAYER_RACE::KARUS;
          default:
               assert(1);     // TODO: do something about this.
               return PLAYER_RACE::KARUS;
     }
}

KO_MEM_ADR KO_CLIENT::find_skill_cooldown_ptr_generic(PROCESS_MEMORY& ko_memory_ref, KO_MEMORY_CONFIG& conf, KO_MEM_BYTE* skill_byte_pattern, size_t byte_pattern_size)
{
     KO_MEM_ADR result = ko_memory_ref.find_pattern_in_memory(skill_byte_pattern, byte_pattern_size);

     KO_MEM_ADR  nation_byte_adr = result + conf.skill_nation_identification_offset_from_pattern;
     KO_MEM_BYTE nation_byte;
     ReadProcessMemory(process_handle, nation_byte_adr, &nation_byte, sizeof(nation_byte), NULL);

     // If not the first match, then it's the second match.
     if(nation_byte != (KO_MEM_BYTE) player_race)
          result = ko_memory_ref.find_pattern_in_memory(skill_byte_pattern, byte_pattern_size, result + 1);

     return result + conf.skill_cooldown_offset_from_pattern;
}

void KO_CLIENT::assign_player_health_and_mana_ptr(PROCESS_MEMORY& ko_memory_ref, KO_MEMORY_CONFIG& conf)
{
     KO_MEM_ADR result = ko_memory_ref.find_pattern_in_memory(conf.mana_hp_anchor_byte_pattern, sizeof(conf.mana_hp_anchor_byte_pattern));

     player_max_hp_ptr = result + conf.max_hp_offset_from_pattern;
     player_cur_hp_ptr = result + conf.current_hp_offset_from_pattern;

     player_max_mp_ptr = result + conf.max_mana_offset_from_pattern;
     player_cur_mp_ptr = result + conf.current_mana_offset_from_pattern;
}

inline void KO_CLIENT::print_info( ) const noexcept { std::cout << "Knight Online PID: " << process_id << "\nKnight Online Handle:  " << process_handle << std::endl; }

#endif
