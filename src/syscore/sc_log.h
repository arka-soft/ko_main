#include <iostream>
#include <sstream>



 /**
 * @brief Custom logging macros with color formatting for enhanced console output.
 *
 * These macros provide a simple logging mechanism with different log levels,
 * each associated with a distinct color for better visibility. The logging
 * levels include INFO, WARN, ERROR, DEBUG, and SUCCESS. The logging level can
 * be controlled by defining the LOGGING_LEVEL macro before including this file.
 * Additionally, a DEBUG macro can be defined to enable or disable debug logs.
 *
 * Logging Levels and Macros Availability:
 * - DEBUG defined: SYSLOG_DEBUG macro is available.
 * - LOGGING_LEVEL 1: SYSLOG_SUCCESS, SYSLOG_INFO, SYSLOG_WARN and SYSLOG_ERROR macros are available.
 * - LOGGING_LEVEL 2: SYSLOG_SUCCESS, SYSLOG_WARN, and SYSLOG_ERROR macros are available.
 * - LOGGING_LEVEL 3: SYSLOG_SUCCESS, and SYSLOG_ERROR macros are available.
 *
 * Example usage:
 * @code
 *   #define LOGGING_LEVEL 2  // Set desired logging level
 *   #include "syscore.h"
 *
 *   int main() {
 *       SYSLOG_INFO("This is an informational message." << std::endl); // (With new line.)
 *       SYSLOG_WARN("This is a warning message.");
 *       SYSLOG_ERROR("This is an error message.");
 *       SYSLOG_DEBUG("This is a debug message.");
 *       SYSLOG_SUCCESS("This is a success message.");
 *       return 0;
 *   }
 * @endcode
 *
 * The output will be formatted with ANSI escape codes for colors, making it
 * visually distinguishable based on the log level.
 */

#define ANSI_COLOR_RESET   "\x1b[0m"
#define ANSI_BOLD          "\x1b[1m"
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_BLUE    "\x1b[36m"

#define LOG_FORMAT(level, colour, ...) \
do { \
    std::ostringstream oss; \
    oss << colour ANSI_BOLD level << __VA_ARGS__; \
    std::cout << oss.str() << ANSI_COLOR_RESET; \
} while(0)


#ifndef LOGGING_LEVEL
#define LOGGING_LEVEL 0
#endif

#if LOGGING_LEVEL <= 1
    #define SYSLOG_INFO(...)  LOG_FORMAT("[INFO] ", ANSI_COLOR_MAGENTA, __VA_ARGS__)
#else
    #define SYSLOG_INFO(...) {}
#endif

#if LOGGING_LEVEL <= 2
    #define SYSLOG_WARN(...)  LOG_FORMAT("[WARN] ", ANSI_COLOR_YELLOW, __VA_ARGS__ )
#else
    #define SYSLOG_WARN(...) {}
#endif

#if LOGGING_LEVEL <= 3
    #define SYSLOG_ERROR(...) LOG_FORMAT("[ERROR] ", ANSI_COLOR_RED, __VA_ARGS__)
#else
    #define SYSLOG_ERROR(...) {}
#endif


#ifdef DEBUG
     #define SYSLOG_DEBUG(...) LOG_FORMAT("[DEBUG] ", ANSI_COLOR_BLUE, __VA_ARGS__)
#else
     #define SYSLOG_DEBUG(...) {}
#endif


#define SYSLOG_SUCCESS(...) LOG_FORMAT("[OK] ", ANSI_COLOR_GREEN, __VA_ARGS__)
