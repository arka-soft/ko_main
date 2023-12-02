#ifndef KC_MEMUTILS_H
#define KC_MEMUTILS_H
#include <windows.h>
#include <stdint.h>

/**
 * @brief kc_memutils.h
 * 
 * Header only library for working with process memories on Windows.
 * Used internally by the KO Client.
 * 
 */

#define GB_TO_BYTES(gb) ((uint64_t)(1073741824 * gb))
#define MB_TO_BYTES(mb) ((uint64_t)(1048576 * mb))
#define KB_TO_BYTES(kb) ((uint64_t)(1024 * kb))

#define BYTES_TO_GB(bytes) (bytes / 1073741824.0)
#define BYTES_TO_MB(bytes) (bytes / 1048576.0)
#define BYTES_TO_KB(bytes) (bytes / 1024.0)

/**
 * @brief 
 * 
 * memmem is a well-known C function that comes as a 'GNU extension', which means it is not something in the standard,
 * but provided by the compilers on different platform. Unfortunately, it is not available on Windows with g++.
 * 
 * This is a copy of the Cygwin's memmem implementation, taken from:
 * https://github.com/Alexpux/Cygwin/blob/master/newlib/libc/string/memmem.c
 * 
 * TODO: This is the 'small-size' version of the algorithm. There's also a larger but faster version.
 * It is longer and requires more includes, so I didn't copy it directly. We may switch to it in the future.
 * There's a ticket for it at https://arka-soft.atlassian.net/browse/SW-41
 * 
 * @param haystack pointer to the start of search space
 * @param hs_len length of the search space in bytes
 * @param needle pointer to the pattern of bytes to search in the haystack
 * @param ne_len length of the pattern of bytes
 * @return void* returns a pointer to the beginning of the pattern found in the haystack
 * 
 */
void* memmem(const void *haystack, size_t hs_len, const void *needle, size_t ne_len);

// This type points to an address in the address space of the other process.
// The data pointed by the OTHER_PROCESS_PTR is meant to be read using ReadProcessMemory function.
typedef uint8_t *OTHER_PROCESS_PTR; 

// This type points to an address in the address space of our own process.
// The data pointed by the HOST_PROCESS_PTR can be accessed as usual using the asterisk (*) operator.
typedef uint8_t *HOST_PROCESS_PTR;  

/**
 * @brief  PROCESS_MEMORY
 * 
 * This is a utility class that can copy large chunks of memory from another process
 * into a heap-allocated memory within the address space of the current process.
 * 
 * If there are regions marked with NO_ACCESS within the target memory, they are set to 0.
 * 
 * It comes with utility methods to search within the address space,
 * as well as means by which pointers can be translated between the process space of the host process,
 * and the other process.
 * 
 */
class PROCESS_MEMORY
{
    // DATA:
private:
    HOST_PROCESS_PTR mapped_memory = nullptr; // pointer to a region in our heap that will hold a copy of the other process' memory.
    OTHER_PROCESS_PTR map_base_address = nullptr; // the address in the address space of the other process at which we started copying.
    SIZE_T map_num_bytes = 0; // number of bytes to copy from the other process

    // METHODS:
    /**
     * @brief Construct PROCESS_MEMORY, copies num_bytes from the base_address into heap.
     * 
     * @param process_handle handle to the process
     * @param base_address base address in the address space of the external process to start the map from
     * @param num_bytes number of bytes to copy
     */
public:
    explicit PROCESS_MEMORY(HANDLE process_handle, OTHER_PROCESS_PTR base_address, SIZE_T num_bytes);
    /**
     * @brief Destroy PROCESS_MEMORY. Deallocates all the copied memory.
     * 
     */
    ~PROCESS_MEMORY();

    // No copy constructor or copy assignment operator.
    PROCESS_MEMORY(const PROCESS_MEMORY&) = delete;
    PROCESS_MEMORY& operator=(const PROCESS_MEMORY&) = delete;


    /**
     * @brief Translates a pointer within the internal mapped_memory to a pointer in the address space of the external process.
     * 
     * @param ptr A pointer pointing somewhere within the internal mapped_memory byte array.
     * @return OTHER_PROCESS_PTR A pointer to the same address, expressed in the address space of the external process.
     */
    inline OTHER_PROCESS_PTR host_ptr_to_other(HOST_PROCESS_PTR ptr) { return (OTHER_PROCESS_PTR)((uint8_t *)map_base_address + ((uint64_t)ptr - (uint64_t)mapped_memory)); }


    /**
     * @brief Translates a pointer within the address space of the external process to an internal pointer to the mapped_memory.
     * 
     * @param ptr A pointer pointing to an address in the address space of the external process.
     * @return HOST_PROCESS_PTR A pointer to the same address, expressed in the the internal memory map.
     */
    inline HOST_PROCESS_PTR other_ptr_to_host(OTHER_PROCESS_PTR ptr) { return (HOST_PROCESS_PTR)&mapped_memory[(uint64_t)ptr - (uint64_t)map_base_address]; }

    /**
     * @brief Finds a pattern of bytes in the mapped memory and returns its original address in the external process memory.
     * 
     * 
     * @param pattern_ptr Pointer to the beginning of the byte pattern to search for.
     * @param pattern_size Size of the byte pattern in size.
     * @param search_start_addr_in_process_space  (Optional) Used for starting the search from an offset.
     * @return OTHER_PROCESS_PTR A pointer to the first match of the pattern, in the address space of the external process.
     */
    OTHER_PROCESS_PTR find_pattern_in_memory(BYTE *pattern_ptr, size_t pattern_size, OTHER_PROCESS_PTR search_start_addr_in_process_space = nullptr);
};

#endif

#ifdef KC_MEMUTILS_IMPLEMENTATION
#pragma once 
    void* memmem(const void *haystack, size_t hs_len, const void *needle, size_t ne_len)
    {
    const char* hs = (char*) haystack;
    const char* ne = (char*) needle;

    if (ne_len == 0)
        return (void *)hs;
    int i;
    int c = ne[0];
    const char *end = hs + hs_len - ne_len;

    for ( ; hs <= end; hs++)
    {
        if (hs[0] != c)
        continue;
        for (i = ne_len - 1; i != 0; i--)
        if (hs[i] != ne[i])
        break;
        if (i == 0)
        return (void *)hs;
    }

    return NULL;
    }

    PROCESS_MEMORY::PROCESS_MEMORY(HANDLE process_handle, OTHER_PROCESS_PTR base_address, SIZE_T num_bytes)
    {
        this->map_num_bytes = num_bytes;
        //assert(process_handle != nullptr); //TODO: SHA, whoever uses this constructor must open the process themselves.

        // Remark: This is also guaranteed to initialize the whole memory to 0.
        // memset(mapped_memory, 0, bytes_to_map) is implied.
        this->mapped_memory = (HOST_PROCESS_PTR) VirtualAlloc(NULL, map_num_bytes, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

        MEMORY_BASIC_INFORMATION first_region_info;
        auto accessed_memory_region = VirtualQueryEx(process_handle, base_address, &first_region_info, sizeof(first_region_info));
        this->map_base_address = (OTHER_PROCESS_PTR)first_region_info.BaseAddress;

        // Iterate over the memory regions
        uint64_t current_byte_offset = 0;
        OTHER_PROCESS_PTR address = base_address;

        // The idea is to simply iterate over the memory regions, copy them using ReadProcessMemory if we have access to them.
        // And skip them if they are guarded.
        bool reached_the_end = false;
        while (!reached_the_end) {
            MEMORY_BASIC_INFORMATION region_info;
            auto accessed_memory_region = VirtualQueryEx(process_handle, address, &region_info, sizeof(region_info));
            if (!accessed_memory_region) {break;} //TODO: SHA, throw exception or assert.

            uint64_t bytes_to_read_from_this_segment;
            if (current_byte_offset + region_info.RegionSize >= map_num_bytes)
            {
                reached_the_end = true;
                bytes_to_read_from_this_segment = map_num_bytes - current_byte_offset; 
            }
            else
            {
                bytes_to_read_from_this_segment = region_info.RegionSize;
            }

            const bool can_read_region = region_info.Protect != PAGE_NOACCESS &&
                                   region_info.Protect != PAGE_EXECUTE_WRITECOPY &&
                                   region_info.Protect != PAGE_EXECUTE &&
                                   region_info.Protect != PAGE_WRITECOPY &&
                                   region_info.Protect != PAGE_TARGETS_INVALID;

            if(can_read_region)
            {
                bool succeed = ReadProcessMemory(process_handle,region_info.BaseAddress, &mapped_memory[current_byte_offset], bytes_to_read_from_this_segment, NULL);
            }

            current_byte_offset += bytes_to_read_from_this_segment;
            address = (OTHER_PROCESS_PTR) region_info.BaseAddress + region_info.RegionSize;
        }
    }
    PROCESS_MEMORY::~PROCESS_MEMORY()
    {
        if(this->mapped_memory) VirtualFree(this->mapped_memory, 0, MEM_RELEASE);
    }

    OTHER_PROCESS_PTR PROCESS_MEMORY::find_pattern_in_memory(BYTE* pattern_ptr, size_t pattern_size, OTHER_PROCESS_PTR search_start_addr_in_process_space)
    {
        // TODO: Do something if this function fails to find. 
        HOST_PROCESS_PTR search_start_address_in_map;
        SIZE_T search_size;

        if(search_start_addr_in_process_space)
        {
            uint64_t start_offset = (uint64_t) search_start_addr_in_process_space - (uint64_t) this->map_base_address;
            search_start_address_in_map = &(this->mapped_memory[start_offset]);
            search_size = map_num_bytes - start_offset;
        }
        else
        {
            search_start_address_in_map = this->mapped_memory;
            search_size = map_num_bytes;
        }

        const HOST_PROCESS_PTR result = (HOST_PROCESS_PTR) memmem((void*) search_start_address_in_map, search_size, pattern_ptr, pattern_size);

        uint64_t offset = (uint64_t) result -  (uint64_t) this->mapped_memory;
        return host_ptr_to_other(result);
    }

#endif