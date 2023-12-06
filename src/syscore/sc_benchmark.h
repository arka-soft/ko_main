#ifndef SYSCORE_BENCHMARK_H
#define SYSCORE_BENCHMARK_H
#include "windows.h"
/**
 * @class TICTOC
 *
 * @brief A class to create high resolution benchmark timers.
 * 
 * tic starts the timer, toc stops it. There are methods to access the time elapsed between them.
 */
class TICTOC
{
                                                                           private:
     LARGE_INTEGER freq;                   // Frequency of the QuerryPerformanceCounter. Fixed at computer boot.
     LARGE_INTEGER tic_count;              // The initial count of the QuerryPerformanceCounter.
     LARGE_INTEGER toc_count;              // The final count of the QuerryPerformanceCounter.
     double        time_difference_ms;     // Time difference between the initial and final count.

                                                                           public:
     /**
         * @brief Construct a new TICTOC timer. Timer doesn't start till the tic method is run.
         */
     TICTOC( );

     /**
         * @brief Start the timer.
         */
     void tic( );

     /**
         * @brief End the timer.
         */
     void toc( );

     /**
         * @brief Returns the elapsed time in ms.
         */
     double elapsed_time_in_ms( );

     /**
         * @brief Returns the elapsed time in ns.
         */
     double elapsed_time_in_ns( );
};

#endif

#ifdef SYSCORE_BENCHMARK_IMPLEMENTATION
TICTOC::TICTOC( )
{
     if(!QueryPerformanceFrequency(&freq))
     {
          // Log an error if QueryPerformanceFrequency fails
          SYSLOG_ERROR("QueryPerformanceFrequency failed.\n");
          // Handle the error as appropriate
     }
}

void inline TICTOC::tic( )
{
     if(!QueryPerformanceCounter(&tic_count))
     {
          // Log an error if QueryPerformanceCounter fails
          SYSLOG_ERROR("QueryPerformanceCounter (tic) failed.\n");
          // Handle the error as appropriate
     }
}

void inline TICTOC::toc( )
{
     if(!QueryPerformanceCounter(&toc_count))
     {
          // Log an error if QueryPerformanceCounter fails
          SYSLOG_ERROR("QueryPerformanceCounter (toc) failed.\n");
          // Handle the error as appropriate
     }

     time_difference_ms = ((double) (toc_count.QuadPart - tic_count.QuadPart) / freq.QuadPart * 1000);
}

double inline TICTOC::elapsed_time_in_ms( ) { return time_difference_ms; }

double inline TICTOC::elapsed_time_in_ns( ) { return time_difference_ms * 1000000; }

#endif
