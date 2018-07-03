#include <iostream>
#include <istream>
#include <fstream>
#include <iomanip>
#include <limits>
#include <cstring>
#include <sstream>
#include "struct.h"
#include "avl.h"
#include <cstdlib>
#include <exception>

#ifdef __gnu_linux__
    #include <unistd.h>
#endif

#ifdef _WIN32
    #define NOMINMAX
    #include <windows.h>
#endif

