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
