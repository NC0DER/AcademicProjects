#ifndef TIMER_H
#define TIMER_H

#ifdef _WIN32
    #include <chrono>
    #define start_timer auto start = std::chrono::system_clock::now();
    #define stop_timer  auto stop = std::chrono::system_clock::now();
#else
    #include <sys/time.h> //Linux/Unix systems with no C++11 support.
    struct timeval start, stop;
    #define start_timer gettimeofday(&start, NULL);
    #define stop_timer  gettimeofday(&stop, NULL);
#endif
#endif