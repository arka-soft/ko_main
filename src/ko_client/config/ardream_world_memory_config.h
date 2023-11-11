#pragma once
#include <array>
#include <cstdint>
#include <stdint.h>
#include <vector>
#include <windows.h>

typedef uint8_t *KO_MEM_ADR;
typedef uint32_t KO_MEM_SIZE; //KO is 32-bit

/*
 * @struct Skills
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

struct KO_MEMORY_MAP 
{
    const char *base_module_name_for_all_skills{"ARTY.Guard"}; // This module name persist through all skills presumably
                                                               // some others too. Subject to change if it is also
                                                               // related to pointers other than skills.

    const std::vector<KO_MEM_SIZE> spike_offsets{0xD0, 0x04, 0x08, 0x00, 0x00, 0x14, 0xA0};
    const KO_MEM_SIZE spike_secondary_base_address{0x002B0D04};
};
