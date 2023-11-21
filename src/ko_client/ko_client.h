#ifndef KO_CLIENT_H
#define KO_CLIENT_H
#include "config/ardream_world_memory_config.h"
#include <iostream>
#include <iterator>
#include <stdint.h>
#include <tchar.h>
#include <tlhelp32.h>
#include <cassert>
#include "kc_memutils.h"

enum class PLAYER_RACE : uint8_t 
{
    KARUS = 62, // KARUS identification byte.
    EL_MORAD = 38 // EL MORAD identification byte.
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
    DWORD process_id;

    PLAYER_RACE player_race;

    KO_MEM_ADR spike_cooldown_ptr;
    KO_MEM_ADR thrust_cooldown_ptr;
    KO_MEM_ADR pierce_cooldown_ptr;

    KO_MEM_ADR player_max_hp_ptr;
    KO_MEM_ADR player_cur_hp_ptr;
    KO_MEM_ADR player_max_mp_ptr;
    KO_MEM_ADR player_cur_mp_ptr;

    // Method Section
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
    KO_CLIENT();

    ~KO_CLIENT();

    [[nodiscard]] DWORD get_process_id() const noexcept{return process_id;}
    [[nodiscard]] HANDLE get_process_handle() const noexcept{return process_handle;}
    [[nodiscard]] PLAYER_RACE get_player_race() const noexcept{return player_race;}


    /**
     * @brief A utility macro to define getter functions that read addresses from the KO memory and returns them as float.
     */
    #define DEFINE_FLOAT_GETTER_FUNC(function_name, variable_name) [[nodiscard]] inline float function_name() const noexcept \
    {float f; ReadProcessMemory(process_handle, variable_name, &f, sizeof(f), NULL); return f;}

    /**
     * @brief A utility macro to define getter functions that read addresses from the KO memory and returns them as uint32.
     */
    #define DEFINE_UINT32_GETTER_FUNC(function_name, variable_name) [[nodiscard]] inline uint32_t function_name() const noexcept \
    {uint32_t i; ReadProcessMemory(process_handle, variable_name, &i, sizeof(i), NULL); return i;}

    // Getter functions 
    DEFINE_FLOAT_GETTER_FUNC(get_spike_cooldown, spike_cooldown_ptr); 
    DEFINE_FLOAT_GETTER_FUNC(get_pierce_cooldown, pierce_cooldown_ptr); 
    DEFINE_FLOAT_GETTER_FUNC(get_thrust_cooldown, thrust_cooldown_ptr); 

    DEFINE_UINT32_GETTER_FUNC(get_player_max_hp, player_max_hp_ptr);
    DEFINE_UINT32_GETTER_FUNC(get_player_cur_hp, player_cur_hp_ptr);
    DEFINE_UINT32_GETTER_FUNC(get_player_max_mp, player_max_mp_ptr);
    DEFINE_UINT32_GETTER_FUNC(get_player_cur_mp, player_cur_mp_ptr);

    inline void print_info() const noexcept;

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
    DWORD get_process_id_by_client_name(const char *process_name);

    /**
     * @brief Finds the player race from the KO memory and returns it.
     * 
     * @param ko_memory_ref Reference to the mapped KO memory object
     * @param conf Reference to the KO memory config
     * @return PLAYER_RACE
     */
    PLAYER_RACE find_player_race(PROCESS_MEMORY& ko_memory_ref, KO_MEMORY_CONFIG& conf);


    /**
     * @brief A generic function that finds the skill cooldown by searching a skill byte pattern.
     * 
     * @param ko_memory_ref Reference to the mapped KO memory object
     * @param conf Reference to the KO memory config
     * @param skill_byte_pattern Pointer to the beginning of the byte pattern to search
     * @param byte_pattern_size Size of the pattern to search for
     * @return KO_MEM_ADR A pointer to the first match to the pattern, expressed in the address space of KnighOnline.
     */
    KO_MEM_ADR find_skill_cooldown_ptr_generic(PROCESS_MEMORY& ko_memory_ref, KO_MEMORY_CONFIG& conf, KO_MEM_BYTE* skill_byte_pattern, size_t byte_pattern_size);

    /**
     * @brief A utility macro to call find_skill_cooldown_ptr_generic for a skill name.
     */
    #define find_skill_cooldown_ptr(ko_memory_ref, conf, skill_name) find_skill_cooldown_ptr_generic(ko_memory_ref, conf, conf.skill_name##_byte_pattern, sizeof(conf.skill_name##_byte_pattern));

    /**
     * @brief  Finds the patterns for the player health and mana information in the KO memory, and assings them to member variables.
     * 
     * @param ko_memory_ref 
     * @param conf 
     */
    void assign_player_health_and_mana_ptr(PROCESS_MEMORY& ko_memory_ref, KO_MEMORY_CONFIG& conf);
};

#endif

#ifdef KO_CLIENT_IMPLEMENTATION
#pragma once

#define KC_MEMUTILS_IMPLEMENTATION 1
#include "kc_memutils.h"

KO_CLIENT::KO_CLIENT()
{
    process_id = get_process_id_by_client_name("KnightOnLine.exe");

    // TODO: Add Safety Features
    process_handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, process_id);

    // Map the process memory to search for patterns in it
    double const ko_address_space_heap_starts_at = 0.2; // GB (via manual inspection using vmmap)
    double const ko_address_space_heap_size = 1; // GB (via manual inspection using vmmap)
    KO_MEM_ADR heap_base_address = (KO_MEM_ADR) GB_TO_BYTES(ko_address_space_heap_starts_at);
    uint64_t bytes_to_map = GB_TO_BYTES(ko_address_space_heap_size);

    PROCESS_MEMORY ko_memory{process_handle, heap_base_address, bytes_to_map};
    KO_MEMORY_CONFIG ko_memory_config;

    // Assign the pointers and stuff
    player_race = find_player_race(ko_memory, ko_memory_config);

    spike_cooldown_ptr = find_skill_cooldown_ptr(ko_memory, ko_memory_config, spike);
    thrust_cooldown_ptr = find_skill_cooldown_ptr(ko_memory, ko_memory_config, thrust);
    pierce_cooldown_ptr = find_skill_cooldown_ptr(ko_memory, ko_memory_config, pierce);

    assign_player_health_and_mana_ptr(ko_memory, ko_memory_config);

}

KO_CLIENT::~KO_CLIENT()
{
    CloseHandle(process_handle);
}

DWORD KO_CLIENT::get_process_id_by_client_name(const char *process_name)
{
    // TODO: Add Safety Features
    HANDLE snapshot_handle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapshot_handle == INVALID_HANDLE_VALUE)
    {
        return 0;
    }

    PROCESSENTRY32 process_entry{};
    process_entry.dwSize = sizeof(PROCESSENTRY32);

    DWORD result = 0;
    while (Process32Next(snapshot_handle, &process_entry))
    {
        if (_stricmp(process_entry.szExeFile, process_name) == 0)
        {
            result = process_entry.th32ProcessID;
            break;
        }
    }

    CloseHandle(snapshot_handle);
    return result;
}

PLAYER_RACE KO_CLIENT::find_player_race(PROCESS_MEMORY &ko_memory_ref, KO_MEMORY_CONFIG &conf)
{
    KO_MEM_ADR result = ko_memory_ref.find_pattern_in_memory(conf.player_nation_identification_byte_pattern,
                                                             conf.KO_STRING_LENGTH_IN_BYTES);

    result += conf.player_nation_identification_offset_from_pattern;

    KO_MEM_BYTE nation_byte;
    ReadProcessMemory(process_handle, result, &nation_byte, sizeof(nation_byte), NULL);

    switch (nation_byte)
    {
    case conf.player_nation_human:
        return PLAYER_RACE::EL_MORAD;
    case conf.player_nation_karus:
        return PLAYER_RACE::KARUS;
    default:
        assert(1); // TODO: do something about this.
        return PLAYER_RACE::KARUS;
    }

}

KO_MEM_ADR KO_CLIENT::find_skill_cooldown_ptr_generic(PROCESS_MEMORY &ko_memory_ref, KO_MEMORY_CONFIG &conf, KO_MEM_BYTE *skill_byte_pattern, size_t byte_pattern_size)
{
    KO_MEM_ADR result = ko_memory_ref.find_pattern_in_memory(skill_byte_pattern, byte_pattern_size);

    KO_MEM_ADR nation_byte_adr = result + conf.skill_nation_identification_offset_from_pattern;
    KO_MEM_BYTE nation_byte;
    ReadProcessMemory(process_handle, nation_byte_adr, &nation_byte, sizeof(nation_byte), NULL);

    // If not the first match, then it's the second match.
    if (nation_byte != (KO_MEM_BYTE)player_race)
        result = ko_memory_ref.find_pattern_in_memory(skill_byte_pattern, byte_pattern_size, result + 1);

    return result + conf.skill_cooldown_offset_from_pattern;
}

void KO_CLIENT::assign_player_health_and_mana_ptr(PROCESS_MEMORY &ko_memory_ref, KO_MEMORY_CONFIG &conf)
{
    KO_MEM_ADR result = ko_memory_ref.find_pattern_in_memory(conf.mana_hp_anchor_byte_pattern, sizeof(conf.mana_hp_anchor_byte_pattern));

    player_max_hp_ptr = result + conf.max_hp_offset_from_pattern;
    player_cur_hp_ptr = result + conf.current_hp_offset_from_pattern;

    player_max_mp_ptr = result + conf.max_mana_offset_from_pattern;
    player_cur_mp_ptr = result + conf.current_mana_offset_from_pattern;
}

inline void KO_CLIENT::print_info() const noexcept
{
    std::cout << "Knight Online PID: " << process_id << "\nKnight Online Handle:  " << process_handle << std::endl;
}

#endif
