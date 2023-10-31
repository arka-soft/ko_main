#pragma once
#include "config/ardream_world_memory_config.h"
#include <Windows.h>
#include <iostream>
#include <iterator>
#include <stdint.h>
#include <tchar.h>
#include <tlhelp32.h>

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
class KO_CLIENT {
  // Data Section
public:
private:
  // Game Process
  HANDLE process_handle;
  DWORD process_id;

  // Skill Pointers
  KO_MEM_ADR spike_pointer;
  KO_MEM_ADR thrust_pointer;
  KO_MEM_ADR pierce_pointer;
  KO_MEM_ADR cut_pointer;
  KO_MEM_ADR shock_pointer;
  KO_MEM_ADR jab_pointer;
  KO_MEM_ADR stab2_pointer;
  KO_MEM_ADR stab_pointer;
  KO_MEM_ADR stroke_pointer;
  KO_MEM_ADR lightfeet_pointer;

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

  [[nodiscard]] DWORD get_process_id() const noexcept;
  [[nodiscard]] HANDLE get_process_handle() const noexcept;
  [[nodiscard]] inline float get_spike_cooldown() const noexcept;
  [[nodiscard]] inline float get_thrust_cooldown() const noexcept;
  [[nodiscard]] inline float get_pierce_cooldown() const noexcept;
  [[nodiscard]] inline float get_cut_cooldown() const noexcept;
  [[nodiscard]] inline float get_shock_cooldown() const noexcept;
  [[nodiscard]] inline float get_jab_cooldown() const noexcept;
  [[nodiscard]] inline float get_stab2_cooldown() const noexcept;
  [[nodiscard]] inline float get_stab_cooldown() const noexcept;
  [[nodiscard]] inline float get_stroke_cooldown() const noexcept;
  [[nodiscard]] inline float get_lightfeet_cooldown() const noexcept;

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
   * @brief Get the base address of a module in a specified process.
   *
   * This function retrieves the base address of a module with the given name
   * in a target process by creating a snapshot of the current threads and
   * iterating through them to find the desired process
   *
   * @param process_id
   * @param module_name
   * @return KO_MEM_ADR
   */
  static KO_MEM_ADR get_game_module_base_address(const DWORD process_id,
                                                 const char *module_name);

  /**
   * @brief Resolve a pointer chain to retrieve the final memory address in a
   * remote process.
   *
   * This function calculates the memory address by following a chain of
   * offsets from a base address within a remote process, allowing for data
   * retrieval.
   *
   * @note Here's how it works:
   * In gaming, we often encounter the challenge of locating dynamic values
   * within the game's memory, which change every time we open or close the
   * game. To overcome this, we rely on static memory locations, also known as
   * pointers, to access the desired values. This process involves utilizing a
   * game module, a secondary address, and a series of offsets, which together
   * guide us to the target value.
   *
   * Start with the base address, "GameModule," and add the secondary address
   * to it: "GameModule" + SecondaryAddress. From this point, we have a
   * pointer points to another pointer: ("GameModule" + SecondaryAddress)  ->
   * PTR1. PTR1, to which we apply the second offset: PTR1 + Second Offset ->
   * PTR 2. The process continues with subsequent offsets until the last
   * pointer ultimately directs us to the desired value. It's important to
   * note that all of this information is typically obtained using tools like
   * Cheat Engine, which help us navigate and manipulate the game's memory
   * effectively. This method allows us to find and interact with the in-game
   * data we're interested in.
   *
   *
   * @param process_handle
   * @param process_id
   * @param base_module_name
   * @param second_base_offset
   * @param offsets
   * @return KO_MEM_ADR
   */

  KO_MEM_ADR static const get_static_memory_address(
      HANDLE process_handle, DWORD process_id, const char *base_module_name,
      uint32_t second_base_offset, const std::vector<KO_MEM_SIZE> &offsets);
};

#ifdef KO_CLIENT_IMPLEMENTATION
KO_CLIENT::KO_CLIENT() {
  KO_MEMORY_MAP ko_memory_map;
  process_id = get_process_id_by_client_name("KnightOnLine.exe");

  // TODO: Add Safety Features
  process_handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, process_id);

  // Skill pointers
  spike_pointer = get_static_memory_address(
      process_handle, process_id, ko_memory_map.base_module_name_for_all_skills,
      ko_memory_map.spike_secondary_base_address, ko_memory_map.spike_offsets);

  thrust_pointer = get_static_memory_address(
      process_handle, process_id, ko_memory_map.base_module_name_for_all_skills,
      ko_memory_map.thrust_secondary_base_address,
      ko_memory_map.thrust_offsets);

  pierce_pointer = get_static_memory_address(
      process_handle, process_id, ko_memory_map.base_module_name_for_all_skills,
      ko_memory_map.pierce_secondary_base_address,
      ko_memory_map.pierce_offsets);

  cut_pointer = get_static_memory_address(
      process_handle, process_id, ko_memory_map.base_module_name_for_all_skills,
      ko_memory_map.cut_secondary_base_address, ko_memory_map.cut_offsets);

  shock_pointer = get_static_memory_address(
      process_handle, process_id, ko_memory_map.base_module_name_for_all_skills,
      ko_memory_map.shock_secondary_base_address, ko_memory_map.shock_offsets);

  jab_pointer = get_static_memory_address(
      process_handle, process_id, ko_memory_map.base_module_name_for_all_skills,
      ko_memory_map.jab_secondary_base_address, ko_memory_map.jab_offsets);

  stab2_pointer = get_static_memory_address(
      process_handle, process_id, ko_memory_map.base_module_name_for_all_skills,
      ko_memory_map.stab2_secondary_base_address, ko_memory_map.stab2_offsets);

  stab_pointer = get_static_memory_address(
      process_handle, process_id, ko_memory_map.base_module_name_for_all_skills,
      ko_memory_map.stab_secondary_base_address, ko_memory_map.stab_offsets);

  stroke_pointer = get_static_memory_address(
      process_handle, process_id, ko_memory_map.base_module_name_for_all_skills,
      ko_memory_map.stroke_secondary_base_address,
      ko_memory_map.stroke_offsets);

  lightfeet_pointer = get_static_memory_address(
      process_handle, process_id, ko_memory_map.base_module_name_for_all_skills,
      ko_memory_map.lightfeet_secondary_base_address,
      ko_memory_map.lightfeet_offsets);
}

KO_CLIENT::~KO_CLIENT() { CloseHandle(process_handle); }

DWORD KO_CLIENT::get_process_id_by_client_name(const char *process_name) {
  // TODO: Add Safety Features
  HANDLE snapshot_handle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
  if (snapshot_handle == INVALID_HANDLE_VALUE) {
    return 0;
  }

  PROCESSENTRY32 process_entry{};
  process_entry.dwSize = sizeof(PROCESSENTRY32);

  DWORD result = 0;
  while (Process32Next(snapshot_handle, &process_entry)) {
    if (_stricmp(process_entry.szExeFile, process_name) == 0) {
      result = process_entry.th32ProcessID;
      break;
    }
  }

  CloseHandle(snapshot_handle);
  return result;
}

KO_MEM_ADR KO_CLIENT::get_game_module_base_address(const DWORD process_id,
                                                   const char *module_name) {
  // Takes a snapshot of the specified processes, as well as the modules
  // only used by these processes.
  // TODO: Add Safety Features
  HANDLE snapshot_handle = CreateToolhelp32Snapshot(
      TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, process_id);

  if (snapshot_handle == INVALID_HANDLE_VALUE) {
    std::cerr << "Retrieving snapshot failed in "
                 "'get_game_module_base_address()'\n ";
    return 0;
  }

  MODULEENTRY32 module_entry{};
  module_entry.dwSize = sizeof(MODULEENTRY32);

  KO_MEM_ADR result = nullptr;
  while (Module32Next(snapshot_handle, &module_entry)) {
    if (_tcsicmp(module_entry.szModule, module_name) == 0) {
      result = module_entry.modBaseAddr;
      break;
    }
  }
  CloseHandle(snapshot_handle);
  return result;
}

const KO_MEM_ADR KO_CLIENT::get_static_memory_address(
    HANDLE process_handle, DWORD process_id, const char *base_module_name,
    uint32_t second_base_offset, const std::vector<KO_MEM_SIZE> &offsets) {
  // TODO: Add Safety Features
  KO_MEM_ADR game_module_base_address =
      get_game_module_base_address(process_id, base_module_name);
  KO_MEM_ADR game_module_address =
      game_module_base_address + second_base_offset;

  const size_t bytes_to_read_in_ko_memory = sizeof(KO_MEM_SIZE);

  KO_MEM_ADR next_memory_address = game_module_address;
  for (int i = 0; i < offsets.size(); i++) {
    if (ReadProcessMemory(process_handle, next_memory_address,
                          &next_memory_address, bytes_to_read_in_ko_memory,
                          NULL) == 0) {
      std::cerr << "Resolving a pointer has failed in "
                   "'get_static_memory_address()'\n";
      return 0;
    }

    next_memory_address += offsets[i];
  }

  return next_memory_address;
}

[[nodiscard]] DWORD KO_CLIENT::get_process_id() const noexcept {
  return process_id;
}

[[nodiscard]] HANDLE KO_CLIENT::get_process_handle() const noexcept {
  return process_handle;
}

// TODO: Add safety features to all skill getters.
[[nodiscard]] inline float KO_CLIENT::get_spike_cooldown() const noexcept {
  float cooldown;

  ReadProcessMemory(process_handle, spike_pointer, &cooldown, sizeof(cooldown),
                    0);
  return cooldown;
}

[[nodiscard]] inline float KO_CLIENT::get_thrust_cooldown() const noexcept {
  float cooldown;
  ReadProcessMemory(process_handle, thrust_pointer, &cooldown, sizeof(cooldown),
                    0);
  return cooldown;
}

[[nodiscard]] inline float KO_CLIENT::get_pierce_cooldown() const noexcept {
  float cooldown;
  ReadProcessMemory(process_handle, pierce_pointer, &cooldown, sizeof(cooldown),
                    0);
  return cooldown;
}

[[nodiscard]] inline float KO_CLIENT::get_cut_cooldown() const noexcept {
  float cooldown;
  ReadProcessMemory(process_handle, cut_pointer, &cooldown, sizeof(cooldown),
                    0);
  return cooldown;
}

[[nodiscard]] inline float KO_CLIENT::get_shock_cooldown() const noexcept {
  float cooldown;
  ReadProcessMemory(process_handle, shock_pointer, &cooldown, sizeof(cooldown),
                    0);
  return cooldown;
}

[[nodiscard]] inline float KO_CLIENT::get_jab_cooldown() const noexcept {
  float cooldown;
  ReadProcessMemory(process_handle, jab_pointer, &cooldown, sizeof(cooldown),
                    0);
  return cooldown;
}

[[nodiscard]] inline float KO_CLIENT::get_stab2_cooldown() const noexcept {
  float cooldown;
  ReadProcessMemory(process_handle, stab2_pointer, &cooldown, sizeof(cooldown),
                    0);
  return cooldown;
}

[[nodiscard]] inline float KO_CLIENT::get_stab_cooldown() const noexcept {
  float cooldown;
  ReadProcessMemory(process_handle, stab_pointer, &cooldown, sizeof(cooldown),
                    0);
  return cooldown;
}

[[nodiscard]] inline float KO_CLIENT::get_stroke_cooldown() const noexcept {
  float cooldown;
  ReadProcessMemory(process_handle, stroke_pointer, &cooldown, sizeof(cooldown),
                    0);
  return cooldown;
}

[[nodiscard]] inline float KO_CLIENT::get_lightfeet_cooldown() const noexcept {
  float cooldown;
  ReadProcessMemory(process_handle, lightfeet_pointer, &cooldown,
                    sizeof(cooldown), 0);
  return cooldown;
}

inline void KO_CLIENT::print_info() const noexcept {
  std::cout << "Knight Online PID: " << process_id
            << "\nKnight Online Handle:  " << process_handle << std::endl;
}

#endif
