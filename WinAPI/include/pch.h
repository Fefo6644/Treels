#pragma once

#ifndef UNICODE
#define UNICODE
#endif // !UNICODE

// Windows header files
#include <Windows.h>
#include <windowsx.h>
#include <ShObjIdl.h>
#include <Uxtheme.h>
#pragma comment(lib, "UxTheme")
#include <d2d1.h>
#pragma comment(lib, "d2d1")
#include <dwrite.h>
#pragma comment(lib, "dwrite.lib")

// C++ STL header files
#define _USE_MATH_DEFINES
#include <thread>
#include <cmath>

// Other header files
#include "objs.h"
