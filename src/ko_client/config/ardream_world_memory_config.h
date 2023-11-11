#pragma once
#include <array>
#include <cstdint>
#include <stdint.h>
#include <vector>
#include <windows.h>

typedef uint8_t *KO_MEM_ADR;
typedef uint32_t KO_MEM_SIZE;
typedef uint8_t KO_MEM_BYTE;


/**
 * @brief KO_BYTE_PATTERNS contains byte patterns for various elements such as skills from Knight Online Client.
 */
struct KO_BYTE_PATTERNS{
  // Constants
  // -------------------------------
  const static uint8_t KO_STRING_LENGTH_IN_BYTES = 40;

  // Skill Patterns
  // -------------------------------
  // Skill patterns consistently yield two distinct addresses—-one for humans and another for karus. This means that skills have nations.
  // The determination of which corresponds to each nation occurs during the examination of the nation offset.
  // Consequently, numerous skills have dual offsets: one for nation identification and another for skill cooldown.
  // The nation identification result must manifest as Karus: 62 or Human: 38.
  // It's important to note that these values are universal across all skills and require no modification for other skill sets. 
  const static KO_MEM_BYTE skill_nation_human = 62;
  const static KO_MEM_BYTE skill_nation_karus = 38;

  KO_MEM_BYTE skill_nation_identification_offset_from_pattern = 0x78;
  KO_MEM_BYTE skill_cooldown_offset_from_pattern              = 0x9C;

  KO_MEM_BYTE spike_byte_pattern[KO_STRING_LENGTH_IN_BYTES]     = {0x53, 0x70, 0x69, 0x6B, 0x65, 0x00, 0x69, 0x63, 0x20, 0x74, 0x6F, 0x75, 0x63, 0x68, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x0F, 0x00, 0x00, 0x00, 0x53, 0x70, 0x69, 0x6B, 0x65, 0x00, 0x69, 0x63, 0x20, 0x74, 0x6F, 0x75, 0x63, 0x68, 0x00, 0x72};
  KO_MEM_BYTE thrust_byte_pattern[KO_STRING_LENGTH_IN_BYTES]    = {0x54, 0x68, 0x72, 0x75, 0x73, 0x74, 0x00, 0x6E, 0x00, 0x69, 0x6E, 0x00, 0x3F, 0x3F, 0x3F, 0x00, 0x06, 0x00, 0x00, 0x00, 0x0F, 0x00, 0x00, 0x00, 0x74, 0x68, 0x72, 0x75, 0x73, 0x74, 0x20, 0x00, 0x00, 0x69, 0x6E, 0x00, 0x6E, 0x74, 0x65, 0x72};
  KO_MEM_BYTE priece_byte_pattern[KO_STRING_LENGTH_IN_BYTES]    = {0x50, 0x69, 0x65, 0x72, 0x63, 0x65, 0x00, 0x72, 0x61, 0x69, 0x6E, 0x00, 0x3F, 0x3F, 0x3F, 0x00, 0x06, 0x00, 0x00, 0x00, 0x0F, 0x00, 0x00, 0x00, 0x50, 0x69, 0x65, 0x72, 0x63, 0x65, 0x00, 0x72, 0x61, 0x69, 0x6E, 0x00, 0x6E, 0x74, 0x65, 0x72};
  KO_MEM_BYTE cut_byte_pattern[KO_STRING_LENGTH_IN_BYTES]       = {0x43, 0x75, 0x74, 0x00, 0x73, 0x74, 0x00, 0x6E, 0x00, 0x69, 0x6E, 0x00, 0x3F, 0x3F, 0x3F, 0x00, 0x03, 0x00, 0x00, 0x00, 0x0F, 0x00, 0x00, 0x00, 0x43, 0x75, 0x74, 0x00, 0x73, 0x74, 0x20, 0x00, 0x00, 0x69, 0x6E, 0x00, 0x6E, 0x74, 0x65, 0x72};
  KO_MEM_BYTE shock_byte_pattern[KO_STRING_LENGTH_IN_BYTES]     = {0x73, 0x68, 0x6F, 0x63, 0x6B, 0x00, 0x00, 0x72, 0x61, 0x69, 0x6E, 0x00, 0x3F, 0x3F, 0x3F, 0x00, 0x05, 0x00, 0x00, 0x00, 0x0F, 0x00, 0x00, 0x00, 0x73, 0x68, 0x6F, 0x63, 0x6B, 0x00, 0x00, 0x72, 0x61, 0x69, 0x6E, 0x00, 0x6E, 0x74, 0x65, 0x72};
  KO_MEM_BYTE jab_byte_pattern[KO_STRING_LENGTH_IN_BYTES]       = {0x4A, 0x61, 0x62, 0x00, 0x3F, 0x00, 0x20, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x00, 0x03, 0x00, 0x00, 0x00, 0x0F, 0x00, 0x00, 0x00, 0x4A, 0x61, 0x62, 0x00, 0x74, 0x75, 0x6D, 0x5D, 0x20, 0x43, 0x6F, 0x75, 0x6E, 0x74, 0x65, 0x72};
  KO_MEM_BYTE stab2_byte_pattern[KO_STRING_LENGTH_IN_BYTES]     = {0x73, 0x74, 0x61, 0x62, 0x00, 0x72, 0x79, 0x00, 0x6B, 0x69, 0x6E, 0x00, 0x00, 0x00, 0x00, 0x66, 0x04, 0x00, 0x00, 0x00, 0x0F, 0x00, 0x00, 0x00, 0x53, 0x74, 0x61, 0x62, 0x32, 0x00, 0x79, 0x00, 0x6B, 0x69, 0x6E, 0x00, 0x00, 0x00, 0x69, 0x6E};
  KO_MEM_BYTE stab_byte_pattern[KO_STRING_LENGTH_IN_BYTES]      = {0x73, 0x74, 0x61, 0x62, 0x00, 0x72, 0x79, 0x00, 0x6B, 0x69, 0x6E, 0x00, 0x00, 0x00, 0x00, 0x66, 0x04, 0x00, 0x00, 0x00, 0x0F, 0x00, 0x00, 0x00, 0x53, 0x74, 0x61, 0x62, 0x00, 0x72, 0x79, 0x00, 0x6B, 0x69, 0x6E, 0x00, 0x00, 0x00, 0x69, 0x6E};
  KO_MEM_BYTE stroke_byte_pattern[KO_STRING_LENGTH_IN_BYTES]    = {0x73, 0x74, 0x72, 0x6F, 0x6B, 0x65, 0x00, 0x73, 0x6B, 0x69, 0x6E, 0x00, 0x00, 0x00, 0x00, 0x66, 0x06, 0x00, 0x00, 0x00, 0x0F, 0x00, 0x00, 0x00, 0x73, 0x74, 0x72, 0x6F, 0x6B, 0x65, 0x00, 0x73, 0x6B, 0x69, 0x6E, 0x00, 0x00, 0x00, 0x69, 0x6E};
  KO_MEM_BYTE vampiric_byte_pattern[KO_STRING_LENGTH_IN_BYTES]  = {0x56, 0x61, 0x6D, 0x70, 0x69, 0x72, 0x69, 0x63, 0x20, 0x74, 0x6F, 0x75, 0x63, 0x68, 0x00, 0x00, 0x0E, 0x00, 0x00, 0x00, 0x0F, 0x00, 0x00, 0x00, 0x56, 0x61, 0x6D, 0x70, 0x69, 0x72, 0x69, 0x63, 0x20, 0x74, 0x6F, 0x75, 0x63, 0x68, 0x00, 0x72};
  KO_MEM_BYTE blood_byte_pattern[KO_STRING_LENGTH_IN_BYTES]     = {0x42, 0x6C, 0x6F, 0x6F, 0x64, 0x20, 0x64, 0x72, 0x61, 0x69, 0x6E, 0x00, 0x3F, 0x3F, 0x3F, 0x00, 0x0B, 0x00, 0x00, 0x00, 0x0F, 0x00, 0x00, 0x00, 0x42, 0x6C, 0x6F, 0x6F, 0x64, 0x20, 0x64, 0x72, 0x61, 0x69, 0x6E, 0x00, 0x6E, 0x74, 0x65, 0x72};

  /* Pseudo-algorithm for clarity:
  *  Locate byte patterns for both player skills and player information.
  *  Identify two addresses for skills and one for player nation.
  *  Verify the player's nation using (player_nation_identification_byte_pattern + player_nation_identification_offset_from_pattern). The expected result is either Human: 69 or Karus: 75.
  *  Examine the first skill address using (spike_byte_pattern + skill_nation_identification_offset_from_pattern). The anticipated outcome is Human: 38 or Karus: 62.
  *  If the player's nation is human and the skill's nation is also human:
  *  Determine the cooldown using (spike_byte_pattern + skill_cooldown_offset_from_pattern) (interpreted as a float).
  *  Acknowledge that while this process may seem intricate, it serves to streamline future efforts, especially in the event of game patches.
  */

  // Character Patterns
  // -------------------------------
  // This byte pattern signifies the player's nation.
  // The expected outcome is either Human: 69 or Karus: 75.
  static const KO_MEM_BYTE player_nation_human = 69;
  static const KO_MEM_BYTE player_nation_karus = 75;

  KO_MEM_BYTE player_nation_identification_byte_pattern[KO_STRING_LENGTH_IN_BYTES] = {0x54, 0x65, 0x78, 0x74, 0x5F, 0x4E, 0x61, 0x74, 0x69, 0x6F, 0x6E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0B, 0x00, 0x00, 0x00, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  KO_MEM_BYTE player_nation_identification_offset_from_pattern                     = 0xC4; 

 // This is Karus. 
  KO_MEM_BYTE current_hp_byte_pattern_human[KO_STRING_LENGTH_IN_BYTES] = {0x00, 0x00, 0xA0, 0x40, 0x00, 0x2D, 0xEC, 0xF2, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0xD5, 0xCF, 0xAD, 0xD2, 0x06, 0xB9, 0xF5, 0x19, 0x93, 0x7B, 0x48, 0x1B, 0x43, 0x01, 0xE0, 0xEC, 0x32, 0x41, 0x96, 0x41, 0xFC, 0xEB, 0x9C, 0x5B, 0xAD, 0x56};
  KO_MEM_BYTE current_hp_offset_from_pattern                     = 0x70; // This should be interpreted as 4 bytes (uint32_t). 
 // This is Human. 

  KO_MEM_BYTE current_hp_byte_patten_karus[KO_STRING_LENGTH_IN_BYTES] = {0x00, 0x00, 0x00, 0x00, 0x00, 0xF8, 0x37, 0xED, 0xA8, 0x82, 0xDC, 0x56, 0xE6, 0x30, 0x78, 0x06, 0xA8, 0x8C, 0x2C, 0x2B, 0xF4, 0xD4, 0x9F, 0x5D, 0x8E, 0x8D, 0x73, 0x86, 0xFA, 0xFE, 0x5C, 0x08, 0x1D, 0x61, 0xB4, 0x0D, 0x4B, 0x56, 0x6B, 0xE9};

  KO_MEM_BYTE current_mana_byte_pattern[KO_STRING_LENGTH_IN_BYTES] = {0x00, 0x00, 0xA0, 0x40, 0x00, 0x2D, 0xEC, 0xF2, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0xD5, 0xCF, 0xAD, 0xD2, 0x06, 0xB9, 0xF5, 0x19, 0x93, 0x7B, 0x48, 0x1B, 0x43, 0x01, 0xE0, 0xEC, 0x32, 0x41, 0x96, 0x41, 0xFC, 0xEB, 0x9C, 0x5B, 0xAD, 0x56};
  uint16_t current_mana_offset_from_pattern                        = 0x548; // This should be interpreted as 4 bytes (uint32_t). 548 is big for uint8_t...

};

/**
 * @struct KO_BYTE_PATTERNS
 * @brief This struct contains byte patterns, ignore masks and offsets for
 * various skills.
 *
 * @note x_byte_patterns contains the patterns for a specific element in the
 * game. x_ignore_masks contains the masks that within the byte pattern but
 * needs to be ignored because they might change. x_offset_from_pattern is the
 * actual value we are looking for.
 *
 * @example We look for a specific skill cooldown, we can get the name of the
 * skill from the game process as a byte pattern. Inside that byte pattern some
 * bytes are subject to change, for that, we can use the ignore masks to
 * identify those changing bytes. Then, we can add an offset to the byte
 * patterns' address to get the actual cooldown.
 */
 
struct KO_BYTE_PATTERNS{
  KO_MEM_BYTE spike_byte_patterns;
  KO_MEM_BYTE spike_ingore_masks;
  KO_MEM_BYTE spike_offset_from_pattern;

};
