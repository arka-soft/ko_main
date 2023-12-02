#pragma once
#include <array>
#include <cstdint>
#include <stdint.h>
#include <vector>
#include <windows.h>

// A pointer to an address in the address space of Knight Online.
// It is defined as uint8_t so that + operator increments it in bytes.
// But it is never meant to be dereferenced in the host process !
// It can only be dereferenced using ReadProcessMemory function.
typedef uint8_t* KO_MEM_ADR;

// A byte value in the address space of Knight Online.
typedef uint8_t  KO_MEM_BYTE;

// An offset of bytes in the address space of Knight Online.
// KnightOnline.exe is a 32-bit process, so its address space goes from 0 to maximum of uint32_t.
// But an offset in byte can be directional (going backward / forward in memory).
// Hence, we need an int64_t for this.
typedef int64_t  KO_MEM_OFFSET; 

/**
 * @brief KO_BYTE_PATTERNS contains byte patterns for various elements such as skills from Knight Online Client.
 */
struct KO_MEMORY_CONFIG{
  // Constants
  // -------------------------------
  const static uint8_t KO_STRING_LENGTH_IN_BYTES = 40;

  // Skill Patterns
  // -------------------------------
  // Skill patterns consistently yield two distinct addresses—-one for humans and another for karus. This means that skills and other elements have nations.
  // The determination of which corresponds to each nation occurs during the examination of the nation offset.
  // The nation identification result must manifest as Karus: 62 or Human: 38.
  // It's important to note that these values are universal across all skills and require no modification for other skill sets.

  const static KO_MEM_BYTE skill_nation_human = 62;
  const static KO_MEM_BYTE skill_nation_karus = 38;

  // It seems that Karus is always the first address that is found when searching. 
  KO_MEM_OFFSET skill_nation_identification_offset_from_pattern = 0x78; // When added to the address, it points to the nation of the skill. 
  KO_MEM_OFFSET skill_cooldown_offset_from_pattern              = 0x9C; // When added to the address, it point to the cooldown of the skill. 

  // Tested
  // Raw: 53 70 69 6B 65 00 69 63 20 74 6F 75 63 68 00 00 05 00 00 00 0F 00 00 00 53 70 69 6B 65 00 69 63 20 74 6F 75 63 68 00 72
  KO_MEM_BYTE spike_byte_pattern[KO_STRING_LENGTH_IN_BYTES] = {0x53, 0x70, 0x69, 0x6B, 0x65, 0x00, 0x69, 0x63, 0x20, 0x74, 0x6F, 0x75, 0x63, 0x68, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x0F, 0x00, 0x00, 0x00, 0x53, 0x70, 0x69, 0x6B, 0x65, 0x00, 0x69, 0x63, 0x20, 0x74, 0x6F, 0x75, 0x63, 0x68, 0x00, 0x72};
  
  // Tested
  // Raw: 54 68 72 75 73 74 00 6E 00 69 6E 00 3F 3F 3F 00 06 00 00 00 0F 00 00 00 74 68 72 75 73 74 20 00 00 69 6E 00 6E 74 65 72
  KO_MEM_BYTE thrust_byte_pattern[KO_STRING_LENGTH_IN_BYTES] = {0x54, 0x68, 0x72, 0x75, 0x73, 0x74, 0x00, 0x6E, 0x00, 0x69, 0x6E, 0x00, 0x3F, 0x3F, 0x3F, 0x00, 0x06, 0x00, 0x00, 0x00, 0x0F, 0x00, 0x00, 0x00, 0x74, 0x68, 0x72, 0x75, 0x73, 0x74, 0x20, 0x00, 0x00, 0x69, 0x6E, 0x00, 0x6E, 0x74, 0x65, 0x72};
  
  // Tested 
  // Raw: 50 69 65 72 63 65 00 72 61 69 6E 00 3F 3F 3F 00 06 00 00 00 0F 00 00 00 50 69 65 72 63 65 00 72 61 69 6E 00 6E 74 65 72
  KO_MEM_BYTE pierce_byte_pattern[KO_STRING_LENGTH_IN_BYTES] = {0x50, 0x69, 0x65, 0x72, 0x63, 0x65, 0x00, 0x72, 0x61, 0x69, 0x6E, 0x00, 0x3F, 0x3F, 0x3F, 0x00, 0x06, 0x00, 0x00, 0x00, 0x0F, 0x00, 0x00, 0x00, 0x50, 0x69, 0x65, 0x72, 0x63, 0x65, 0x00, 0x72, 0x61, 0x69, 0x6E, 0x00, 0x6E, 0x74, 0x65, 0x72};
 
  // Tested
  // Raw: 43 75 74 00 73 74 00 6E 00 69 6E 00 3F 3F 3F 00 03 00 00 00 0F 00 00 00 43 75 74 00 73 74 20 00 00 69 6E 00 6E 74 65 72
  KO_MEM_BYTE cut_byte_pattern[KO_STRING_LENGTH_IN_BYTES] = {0x43, 0x75, 0x74, 0x00, 0x73, 0x74, 0x00, 0x6E, 0x00, 0x69, 0x6E, 0x00, 0x3F, 0x3F, 0x3F, 0x00, 0x03, 0x00, 0x00, 0x00, 0x0F, 0x00, 0x00, 0x00, 0x43, 0x75, 0x74, 0x00, 0x73, 0x74, 0x20, 0x00, 0x00, 0x69, 0x6E, 0x00, 0x6E, 0x74, 0x65, 0x72};
  
  // Tested
  // Raw: 73 68 6F 63 6B 00 00 72 61 69 6E 00 3F 3F 3F 00 05 00 00 00 0F 00 00 00 73 68 6F 63 6B 00 00 72 61 69 6E 00 6E 74 65 72
  KO_MEM_BYTE shock_byte_pattern[KO_STRING_LENGTH_IN_BYTES] = {0x73, 0x68, 0x6F, 0x63, 0x6B, 0x00, 0x00, 0x72, 0x61, 0x69, 0x6E, 0x00, 0x3F, 0x3F, 0x3F, 0x00, 0x05, 0x00, 0x00, 0x00, 0x0F, 0x00, 0x00, 0x00, 0x73, 0x68, 0x6F, 0x63, 0x6B, 0x00, 0x00, 0x72, 0x61, 0x69, 0x6E, 0x00, 0x6E, 0x74, 0x65, 0x72};
 
  // Tested
  // Raw: 4A 61 62 00 3F 00 20 3F 3F 3F 3F 3F 3F 3F 3F 00 03 00 00 00 0F 00 00 00 4A 61 62 00 74 75 6D 5D 20 43 6F 75 6E 74 65 72
  KO_MEM_BYTE jab_byte_pattern[KO_STRING_LENGTH_IN_BYTES] = {0x4A, 0x61, 0x62, 0x00, 0x3F, 0x00, 0x20, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x00, 0x03, 0x00, 0x00, 0x00, 0x0F, 0x00, 0x00, 0x00, 0x4A, 0x61, 0x62, 0x00, 0x74, 0x75, 0x6D, 0x5D, 0x20, 0x43, 0x6F, 0x75, 0x6E, 0x74, 0x65, 0x72};
  
  // Tested
  // Raw: 73 74 61 62 00 72 79 00 6B 69 6E 00 00 00 00 66 04 00 00 00 0F 00 00 00 53 74 61 62 32 00 79 00 6B 69 6E 00 00 00 69 6E
  KO_MEM_BYTE stab2_byte_pattern[KO_STRING_LENGTH_IN_BYTES] = {0x73, 0x74, 0x61, 0x62, 0x00, 0x72, 0x79, 0x00, 0x6B, 0x69, 0x6E, 0x00, 0x00, 0x00, 0x00, 0x66, 0x04, 0x00, 0x00, 0x00, 0x0F, 0x00, 0x00, 0x00, 0x53, 0x74, 0x61, 0x62, 0x32, 0x00, 0x79, 0x00, 0x6B, 0x69, 0x6E, 0x00, 0x00, 0x00, 0x69, 0x6E};
  
  // Tested
  // Raw: 73 74 61 62 00 72 79 00 6B 69 6E 00 00 00 00 66 04 00 00 00 0F 00 00 00 53 74 61 62 00 72 79 00 6B 69 6E 00 00 00 69 6E
  KO_MEM_BYTE stab_byte_pattern[KO_STRING_LENGTH_IN_BYTES] = {0x73, 0x74, 0x61, 0x62, 0x00, 0x72, 0x79, 0x00, 0x6B, 0x69, 0x6E, 0x00, 0x00, 0x00, 0x00, 0x66, 0x04, 0x00, 0x00, 0x00, 0x0F, 0x00, 0x00, 0x00, 0x53, 0x74, 0x61, 0x62, 0x00, 0x72, 0x79, 0x00, 0x6B, 0x69, 0x6E, 0x00, 0x00, 0x00, 0x69, 0x6E};
  
  // Tested
  // Raw: 73 74 72 6F 6B 65 00 73 6B 69 6E 00 00 00 00 66 06 00 00 00 0F 00 00 00 73 74 72 6F 6B 65 00 73 6B 69 6E 00 00 00 69 6E
  KO_MEM_BYTE stroke_byte_pattern[KO_STRING_LENGTH_IN_BYTES]= {0x73, 0x74, 0x72, 0x6F, 0x6B, 0x65, 0x00, 0x73, 0x6B, 0x69, 0x6E, 0x00, 0x00, 0x00, 0x00, 0x66, 0x06, 0x00, 0x00, 0x00, 0x0F, 0x00, 0x00, 0x00, 0x73, 0x74, 0x72, 0x6F, 0x6B, 0x65, 0x00, 0x73, 0x6B, 0x69, 0x6E, 0x00, 0x00, 0x00, 0x69, 0x6E};
  
  //Tested
  // Raw: 56 61 6D 70 69 72 69 63 20 74 6F 75 63 68 00 00 0E 00 00 00 0F 00 00 00 56 61 6D 70 69 72 69 63 20 74 6F 75 63 68 00 72 
  KO_MEM_BYTE vampiric_byte_pattern[KO_STRING_LENGTH_IN_BYTES] = {0x56, 0x61, 0x6D, 0x70, 0x69, 0x72, 0x69, 0x63, 0x20, 0x74, 0x6F, 0x75, 0x63, 0x68, 0x00, 0x00, 0x0E, 0x00, 0x00, 0x00, 0x0F, 0x00, 0x00, 0x00, 0x56, 0x61, 0x6D, 0x70, 0x69, 0x72, 0x69, 0x63, 0x20, 0x74, 0x6F, 0x75, 0x63, 0x68, 0x00, 0x72};
  
  // Tested
  // Raw: 42 6C 6F 6F 64 20 64 72 61 69 6E 00 3F 3F 3F 00 0B 00 00 00 0F 00 00 00 42 6C 6F 6F 64 20 64 72 61 69 6E 00 6E 74 65 72
  KO_MEM_BYTE blood_byte_pattern[KO_STRING_LENGTH_IN_BYTES] = {0x42, 0x6C, 0x6F, 0x6F, 0x64, 0x20, 0x64, 0x72, 0x61, 0x69, 0x6E, 0x00, 0x3F, 0x3F, 0x3F, 0x00, 0x0B, 0x00, 0x00, 0x00, 0x0F, 0x00, 0x00, 0x00, 0x42, 0x6C, 0x6F, 0x6F, 0x64, 0x20, 0x64, 0x72, 0x61, 0x69, 0x6E, 0x00, 0x6E, 0x74, 0x65, 0x72};

  //Tested
  // Raw: 53 74 65 61 6C 74 68 00 00 69 6E 00 3F 3F 3F 00 07 00 00 00 0F 00 00 00 53 74 65 61 6C 74 68 00 00 69 6E 00 6E 74 65 72
  KO_MEM_BYTE stealth_byte_pattern[KO_STRING_LENGTH_IN_BYTES] = {0x53, 0x74, 0x65, 0x61, 0x6C, 0x74, 0x68, 0x00, 0x00, 0x69, 0x6E, 0x00, 0x3F, 0x3F, 0x3F, 0x00, 0x07, 0x00, 0x00, 0x00, 0x0F, 0x00, 0x00, 0x00, 0x53, 0x74, 0x65, 0x61, 0x6C, 0x74, 0x68, 0x00, 0x00, 0x69, 0x6E, 0x00, 0x6E, 0x74, 0x65, 0x72};

  //Tested
  // Raw: 4C 75 70 69 6E 65 20 45 79 65 73 00 67 00 00 00 0B 00 00 00 0F 00 00 00 4C 75 70 69 6E 65 20 45 79 65 73 00 67 00 61 6C
  KO_MEM_BYTE lupin_eyes_byte_pattern[KO_STRING_LENGTH_IN_BYTES] = {0x4C, 0x75, 0x70, 0x69, 0x6E, 0x65, 0x20, 0x45, 0x79, 0x65, 0x73, 0x00, 0x67, 0x00, 0x00, 0x00, 0x0B, 0x00, 0x00, 0x00, 0x0F, 0x00, 0x00, 0x00, 0x4C, 0x75, 0x70, 0x69, 0x6E, 0x65, 0x20, 0x45, 0x79, 0x65, 0x73, 0x00, 0x67, 0x00, 0x61, 0x6C};

  //Tested
  // Raw: 43 75 72 65 20 63 75 72 73 65 00 00 67 00 00 00 0A 00 00 00 0F 00 00 00 43 75 72 65 20 63 75 72 73 65 00 00 67 00 61 6C
  KO_MEM_BYTE cure_curse_byte_pattern[KO_STRING_LENGTH_IN_BYTES] = {0x43, 0x75, 0x72, 0x65, 0x20, 0x63, 0x75, 0x72, 0x73, 0x65, 0x00, 0x00, 0x67, 0x00, 0x00, 0x00, 0x0A, 0x00, 0x00, 0x00, 0x0F, 0x00, 0x00, 0x00, 0x43, 0x75, 0x72, 0x65, 0x20, 0x63, 0x75, 0x72, 0x73, 0x65, 0x00, 0x00, 0x67, 0x00, 0x61, 0x6C};

  //Tested
  // Raw: 4D 61 67 69 63 20 53 68 69 65 6C 64 00 00 00 00 0C 00 00 00 0F 00 00 00 4D 61 67 69 63 20 53 68 69 65 6C 64 00 00 61 6C
  KO_MEM_BYTE magic_shield_byte_pattern[KO_STRING_LENGTH_IN_BYTES] = {0x4D, 0x61, 0x67, 0x69, 0x63, 0x20, 0x53, 0x68, 0x69, 0x65, 0x6C, 0x64, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x0F, 0x00, 0x00, 0x00, 0x4D, 0x61, 0x67, 0x69, 0x63, 0x20, 0x53, 0x68, 0x69, 0x65, 0x6C, 0x64, 0x00, 0x00, 0x61, 0x6C};



  // Character Patterns
  // -------------------------------
  // This byte pattern signifies the player's nation.
  // The expected outcome is either Human: 69 or Karus: 75.
  static const KO_MEM_BYTE player_nation_human = 69;
  static const KO_MEM_BYTE player_nation_karus = 75;

  // Tested 
  // Raw: 54 65 78 74 5F 4E 61 74 69 6F 6E 00 00 00 00 00 0B 00 00 00 0F 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  KO_MEM_BYTE player_nation_identification_byte_pattern[KO_STRING_LENGTH_IN_BYTES] = {0x54, 0x65, 0x78, 0x74, 0x5F, 0x4E, 0x61, 0x74, 0x69, 0x6F, 0x6E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0B, 0x00, 0x00, 0x00, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  KO_MEM_OFFSET player_nation_identification_offset_from_pattern                     = 0xC4; 

 // Mana and HP is incoming. (Currently, there is nothing that I can anchor. Still investigating.)
 // Raw: 3C 00 00 00 05 00 00 00 5A 00 00 00 2C 00 00 00 ED 00 00 00 77 00 00 00 32 00 00 00 00 00 00 00 32
 // Needs more testing but seems stable enough. 
  KO_MEM_BYTE mana_hp_anchor_byte_pattern[33] = {0x3C, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x5A, 0x00, 0x00, 0x00, 0x2C, 0x00, 0x00, 0x00, 0xED, 0x00, 0x00, 0x00, 0x77, 0x00, 0x00, 0x00, 0x32, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x32};
  KO_MEM_OFFSET max_mana_offset_from_pattern = -0x38;
  KO_MEM_OFFSET current_mana_offset_from_pattern = -0x34;
  KO_MEM_OFFSET max_hp_offset_from_pattern = -0x510;
  KO_MEM_OFFSET current_hp_offset_from_pattern = -0x50C;

  //Percentage Healths, from same pattern.
  KO_MEM_BYTE percentage_hp_mana_byte_pattern[13] = {0x70, 0x72, 0x6F, 0x5F, 0x68, 0x6F, 0x72, 0x4D, 0x70, 0x00, 0x00, 0x00, 0x00};
  KO_MEM_OFFSET percentage_mp_offset_from_pattern = 0xD8; // should be interpreted as float
  KO_MEM_OFFSET percentage_hp_offset_from_pattern = 0x220;  // should be interpreted as float

  // Whisper and Chat State Pattern
  // Raw: 00 00 00 00 00 00 00 00 A6 1A 5A F1 FC 7F 00 00 7F 00 32 40 00 00 00 00
  // This should always be 173 (uint8_t) otherwise either enter is pressed or whisper panel is open and ready to write.
  // Appereantly this is not on the heap so the memory location is always the same. 
  // Needs to be checked, if it does not return any value press enter in the game, I guess?
  // This does not work at the moment for some reason. So, when needed this will be adjusted. 
  static const KO_MEM_BYTE no_communication_is_open = 173;
  KO_MEM_BYTE whisper_chat_byte_pattern[24] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xA6, 0x1A, 0x5A, 0xF1, 0xFC, 0x7F, 0x00, 0x00, 0x7F, 0x00, 0x32, 0x40, 0x00, 0x00, 0x00, 0x00};
  int32_t whisper_chat_offset_from_pattern = 0x28;

};

