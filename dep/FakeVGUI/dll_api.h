#pragma once

#ifdef BUILD_VGUI
#define VGUI_DLL_API __declspec(dllexport)
#else
#define VGUI_DLL_API __declspec(dllimport)
#endif