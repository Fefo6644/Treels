#ifndef _PRECOMPILED_HEADER_
#define _PRECOMPILED_HEADER_

#ifndef UNICODE
#define UNICODE
#endif // UNICODE

typedef unsigned char byte;
typedef unsigned short word;
typedef unsigned int dword;
typedef unsigned long qword;

// Windows header files
#include <Windows.h>
#include <d2d1_3.h>

// C++ STL header files
#define _USE_MATH_DEFINES
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <exception>
#include <system_error>
#include <vector>
#include <sstream>
#include <string>
#include <iomanip>
#include <thread>
#include <atomic>
#include <condition_variable>
#include <mutex>

#endif // !_PRECOMPILED_HEADER_
