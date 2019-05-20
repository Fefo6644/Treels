#pragma once

#ifndef UNICODE
#define UNICODE
#endif // UNICODE

typedef unsigned char byte;
typedef unsigned short word;

// Windows header files
#include <Windows.h>
#include <windowsx.h>
#include <ShObjIdl.h>
#include <Uxtheme.h>
#pragma comment(lib, "Uxtheme.lib")
#include <d2d1_3.h>
#pragma comment(lib, "d2d1.lib")

// C++ STL header files
#define _USE_MATH_DEFINES
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <exception>
#include <system_error>
#include <thread>
#include <atomic>
#include <condition_variable>
#include <mutex>
