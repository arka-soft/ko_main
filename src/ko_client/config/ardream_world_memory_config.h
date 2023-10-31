#pragma once
#include <array>
#include <cstdint>
#include <stdint.h>
#include <vector>
#include <windows.h>

typedef uint8_t *KO_MEM_ADR;
typedef uint32_t KO_MEM_SIZE; // KO is 32-bit

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

struct KO_MEMORY_MAP {
  const char *base_module_name_for_all_skills{"ARTY.Guard"};

  const std::vector<KO_MEM_SIZE> spike_offsets{0xD0, 0x04, 0x08, 0x00,
                                               0x00, 0x14, 0xA0};
  const KO_MEM_SIZE spike_secondary_base_address{0x002B0D04};

  const std::vector<KO_MEM_SIZE> thrust_offsets{0x04,  0x08, 0x08, 0x28,
                                                0x354, 0x60, 0xA0};
  const KO_MEM_SIZE thrust_secondary_base_address{0x002EC4E0};

  const std::vector<KO_MEM_SIZE> pierce_offsets{0x04,  0x08, 0x08, 0x28,
                                                0x348, 0x60, 0xA0};
  const KO_MEM_SIZE pierce_secondary_base_address{0x002EC4E0};

  const std::vector<KO_MEM_SIZE> cut_offsets{0x04,  0x08, 0x08, 0x28,
                                             0x358, 0x60, 0xA0};
  const KO_MEM_SIZE cut_secondary_base_address{0x002EC4E0};

  const std::vector<KO_MEM_SIZE> shock_offsets{0x04,  0x08, 0x08, 0x28,
                                               0x34C, 0x60, 0xA0};
  const KO_MEM_SIZE shock_secondary_base_address{0x002EC4E0};

  const std::vector<KO_MEM_SIZE> jab_offsets{0x04,  0x08, 0x08, 0x28,
                                             0x340, 0x60, 0xA0};
  const KO_MEM_SIZE jab_secondary_base_address{0x002EC4E0};

  const std::vector<KO_MEM_SIZE> stab_offsets{0x04,  0x08, 0x08, 0x28,
                                              0x18C, 0x60, 0xA0};
  const KO_MEM_SIZE stab_secondary_base_address{0x002EC4E0};

  const std::vector<KO_MEM_SIZE> stab2_offsets{0x04,  0x08, 0x08, 0x28,
                                               0x190, 0x60, 0xA0};
  const KO_MEM_SIZE stab2_secondary_base_address{0x002EC4E0};

  const std::vector<KO_MEM_SIZE> stroke_offsets{0x04,  0x08, 0x08, 0x28,
                                                0x180, 0x60, 0xA0};
  const KO_MEM_SIZE stroke_secondary_base_address{0x002EC4E0};

  const std::vector<KO_MEM_SIZE> lightfeet_offsets{0x04,  0x08, 0x08, 0x28,
                                                   0x430, 0x60, 0xA0};
  const KO_MEM_SIZE lightfeet_secondary_base_address{0x002EC4E0};
};
