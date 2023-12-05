#ifndef SC_KEYS_H
#define SC_KEYS_H

#include "windows.h"

#include <stdint.h>
#include <utility>

/**
 * @brief Virtual key codes for common alphanumeric keys.
 *
 * These constants represent virtual key codes for common alphanumeric keys.
 * Each constant is associated with a specific key on the keyboard.
 * This is added on top of "windows.h" because they were not defined before. (Expansion)
 *
 * @note These constants are defined as compile-time constants (constexpr).
 */

constexpr int VK_0 = 0x30;
constexpr int VK_1 = 0x31;
constexpr int VK_2 = 0x32;
constexpr int VK_3 = 0x33;
constexpr int VK_4 = 0x34;
constexpr int VK_5 = 0x35;
constexpr int VK_6 = 0x36;
constexpr int VK_7 = 0x37;
constexpr int VK_8 = 0x38;
constexpr int VK_9 = 0x39;

constexpr int VK_A = 0x41;
constexpr int VK_B = 0x42;
constexpr int VK_C = 0x43;
constexpr int VK_D = 0x44;
constexpr int VK_E = 0x45;
constexpr int VK_F = 0x46;
constexpr int VK_G = 0x47;
constexpr int VK_H = 0x48;
constexpr int VK_I = 0x49;
constexpr int VK_J = 0x4A;
constexpr int VK_K = 0x4B;
constexpr int VK_L = 0x4C;
constexpr int VK_M = 0x4D;
constexpr int VK_N = 0x4E;
constexpr int VK_O = 0x4F;
constexpr int VK_P = 0x50;
constexpr int VK_Q = 0x51;
constexpr int VK_R = 0x52;
constexpr int VK_S = 0x53;
constexpr int VK_T = 0x54;
constexpr int VK_U = 0x55;
constexpr int VK_V = 0x56;
constexpr int VK_W = 0x57;
constexpr int VK_X = 0x58;
constexpr int VK_Y = 0x59;
constexpr int VK_Z = 0x5A;

/**
 * @brief Virtual key codes for various skills.
 *
 * These constants represent virtual key codes for different in-game skills.
 * Each constant is associated with a specific skill action in the game.
 * These will be read from a config file when implemented.
 *
 * @note These constants are defined as compile-time constants (constexpr).
 */
constexpr int spike_key   = VK_2;
constexpr int spike_page  = VK_F1;
constexpr int thrust_key  = VK_3;
constexpr int thrust_page = VK_F1;
constexpr int pierce_key  = VK_4;
constexpr int pierce_page = VK_F1;
constexpr int cut_key     = VK_5;
constexpr int cut_page    = VK_F1;
constexpr int shock_key   = VK_6;
constexpr int shock_page  = VK_F1;
constexpr int jab_key     = VK_7;
constexpr int jab_page    = VK_F1;
constexpr int stab_key    = VK_8;
constexpr int stab_page   = VK_F1;
constexpr int stab2_key   = VK_9;
constexpr int stab2_page  = VK_F1;
constexpr int stroke_key  = VK_0;
constexpr int stroke_page = VK_F1;

/**
 * @brief Simulates the press of multiple keys using the `send_raw_key` function.
 *
 * This variadic template function, `press_keys`, allows you to simulate the pressing
 * of a sequence of keys by invoking the `send_raw_key` function for each key provided
 * as arguments.
 *
 * @tparam Keys Types of keys to be pressed.
 * @param keys Keys to be pressed.
 */
template<typename... Keys>
void send_multiple_keys(Keys&&... keys);

/**
 * @brief Checks if a specific key is currently pressed.
 *
 * The function utilizes the GetAsyncKeyState function to determine whether
 * the specified key is currently in a pressed state.
 *
 * @param key_code The virtual key code to check for pressed state.
 * @return `true` if the key is pressed, `false` otherwise.
 */
bool is_key_pressed(const int& key_code);

/**
 * @brief Simulates the press and release of a key using the Windows API's `SendInput`.
 *
 * This function translates a virtual key code to a scan code, simulates the key press,
 * introduces a delay for the specified duration, and then simulates the key release.
 *
 * @param key The virtual key code to be simulated.
 * @param key_press_release_delay_in_ms The duration in milliseconds between key press and release.
 */
void send_raw_key(const uint16_t& key, const uint8_t& key_press_release_delay_in_ms = 10);

/**
 * @brief Enumeration representing the hit report of a skill.
 *
 * The `SKILL_HIT_REPORT` enum is used to indicate the readiness or cooldown status
 * of a skill. It consists of two values: READY, indicating that the skill is ready
 * for use, and COOLDOWN, indicating that the skill is currently in cooldown.
 */
enum SKILL_HIT_REPORT
{
     READY,   /* The skill is ready for use. */
     COOLDOWN /* The skill is in cooldown.   */
};
#endif

#ifdef SYSCORE_KEYS_IMPLEMENTATION
bool is_key_pressed(const int& key_code)
{
     // Check if the key is pressed by using bitwise AND with the high-order bit
     return (GetAsyncKeyState(key_code) & 0x8000) != 0;
}

template<typename... Keys>
void send_multiple_keys(Keys&&... keys)
{
     // Simulate pressing each key
     (send_raw_key(std::forward<Keys>(keys)), ...);
}

void send_raw_key(const uint16_t& key, const uint8_t& key_press_release_delay_in_ms)
{

     //Translating virtual-code to scan code for Knight Online.
     uint16_t scan_code = MapVirtualKey(key, 0);

     INPUT keyboard;

     // Simulate key press
     keyboard.type = INPUT_KEYBOARD;

     // Keyboard input settings
     KEYBDINPUT key_down;
     key_down.wScan   = scan_code;
     key_down.dwFlags = KEYEVENTF_SCANCODE;
     keyboard.ki      = key_down;

     //Send the key down
     bool is_send_input_successful = SendInput(1, &keyboard, sizeof(INPUT));
     if(!is_send_input_successful)
     {
          // SYSLOG_ERROR("Key_down event failed with error code: " << GetLastError() << "\nFailed Key: " << scan_code << "\nFailed Virtual Key: " << key);
          return;
     }

     //Time it takes to relase the key
     Sleep(key_press_release_delay_in_ms);

     // Simulate key release
     KEYBDINPUT key_up;

     // Keyboard input settings
     key_up.wScan   = scan_code;
     key_up.dwFlags = KEYEVENTF_KEYUP | KEYEVENTF_SCANCODE;
     keyboard.ki    = key_up;

     //Send the key up
     is_send_input_successful = SendInput(1, &keyboard, sizeof(INPUT));
     if(!is_send_input_successful)
     {
          // SYSLOG_ERROR("Key_up event failed with error code: " << GetLastError() << "\nFailed Scan Code: " << scan_code << "\nFailed Virtual Key: " << key);
          return;
     }
     Sleep(1);
}
#endif