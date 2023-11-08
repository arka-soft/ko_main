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
 * @struct KO_MEMORY_MAP
 * @brief A struct representing a collection of skills data.
 *
 * see @resolve_pointer_chain function note to understand the naming convention
 * in detail.
 * @note In summary, each initial pointer in the chain references another
 * pointer containing a module name and a secondary base address (e.g.,
 * "Arty.Guard" + Secondary Base Address). After the first pointer, we must
 * apply offsets to the second pointer to locate the subsequent pointer. This
 * process repeats iteratively.
 *
 * Each skill has its own struct and relevant data.
 */

struct KO_MEMORY_MAP {
  /*This module name persist through all skills presumably some others too.
   * Subject to change if it is also related to pointers other than skills.*/
  const char *base_module_name_for_all_skills{"ARTY.Guard"};
  const std::vector<KO_MEM_SIZE> spike_offsets{0xD0, 0x04, 0x08, 0x00,
                                               0x00, 0x14, 0xA0};
  const KO_MEM_SIZE spike_secondary_base_address{0x002B0D04};
};

/**
 * @struct KO_BYTE_PATTERNS
 * @brief This struct contains byte patterns, ignore masks and offsets for
 * various skills.
 *
 * @note X_byte_patterns contains the patterns for a specific element in the
 * game. X_ignore_masks contains the masks that within the byte pattern but
 * needs to be ignored because they might change. X_offset_from_pattern is the
 * actual value we are looking for.
 *
 * @example We look for a specific skill cooldown, we can get the name of the
 * skill from the game process as a byte pattern. Inside that byte pattern some
 * bytes are subject to change for that, we can use the ignore masks the
 * identify those bytes to be ignored. Then, we can add an offset to the byte
 * patterns address to get the actual cooldown.
 */
struct SKILL_BYTE_PATTERNS {

  const static int KO_SKILL_NAME_LENGTH{0x40};
  const static int KARUS{0x62};
  const static int HUMAN{0x38};
  KO_MEM_BYTE skill_race_idenfitication_offset = 0x78;

  KO_MEM_BYTE spike_byte_patterns[40];
  KO_MEM_BYTE spike_ignore_masks[40];
  KO_MEM_BYTE spike_cooldown_offset_from_pattern;
};

struct PLAYER_BYTE_PATTERNS {

  const static int KO_NATION_NAME_LENGTH{40};
  const static int KARUS{69};
  const static int HUMAN{75};

  KO_MEM_BYTE nation_identification_offset{0xC4};

  KO_MEM_BYTE nation_identification_pattern[KO_NATION_NAME_LENGTH] = {
      0x54, 0x65, 0x78, 0x74, 0x5F, 0x4E, 0x61, 0x74, 0x69, 0x6F,
      0x6E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0B, 0x00, 0x00, 0x00,
      0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
};
