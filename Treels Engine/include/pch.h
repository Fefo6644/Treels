#ifndef _PRECOMPILED_HEADER_
#define _PRECOMPILED_HEADER_

#ifndef UNICODE
#define UNICODE
#endif // UNICODE

using byte = unsigned char;
using word = unsigned short;
using dword = unsigned int;
using qword = unsigned long long;

// Windows header files
#include <Windows.h>
#include <windowsx.h>
#include <d2d1_3.h>
#include <objbase.h>

// C++ STL header files
#define _USE_MATH_DEFINES
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <vector>
#include <sstream>
#include <string>
#include <iomanip>
#include <thread>
#include <atomic>
#include <functional>
#include <algorithm>

#endif // !_PRECOMPILED_HEADER_
