/*
*
*    This program is free software; you can redistribute it and/or modify it
*    under the terms of the GNU General Public License as published by the
*    Free Software Foundation; either version 2 of the License, or (at
*    your option) any later version.
*
*    This program is distributed in the hope that it will be useful, but
*    WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
*    General Public License for more details.
*
*    You should have received a copy of the GNU General Public License
*    along with this program; if not, write to the Free Software Foundation,
*    Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*
*    In addition, as a special exception, the author gives permission to
*    link the code of this program with the Half-Life Game Engine ("HL
*    Engine") and Modified Game Libraries ("MODs") developed by Valve,
*    L.L.C ("Valve").  You must obey the GNU General Public License in all
*    respects for all of the code used other than the HL Engine and MODs
*    from Valve.  If you modify this file, you may extend this exception
*    to your version of the file, but you are not obligated to do so.  If
*    you do not wish to do so, delete this exception statement from your
*    version.
*
*/

#include "precompiled.h"

#ifdef _WIN32

// DLL entry point
BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	if (fdwReason == DLL_PROCESS_ATTACH)
	{
		if (MessageBoxA(NULL, "Use the built engine", "Notice", MB_OKCANCEL) == 2)
		{
			if (!LoadLibraryA("GameHW.dll"))
			{
				MessageBoxA(NULL, "Failed to load GameHW.dll", "ERROR", MB_OK);
			}
		}
		else
		{
			// chromhtml.dll is MSVC10 compiled and has CRT issue with MSVC14, so will fail if building with MTd - Wefaq
#if defined(UNRESOLVED_ISSUE) && defined(_DEBUG)
			LoadLibraryA("chromehtml.dll");
#endif
		}
		g_RehldsRuntimeConfig.parseFromCommandLine(GetCommandLineA());
#ifdef _WIN32
		Module hlds_exe;
		if (!FindModuleByName("hlds.exe", &hlds_exe))
		{
			printf("%s: launcher is not hlds.exe, tests playing/recording disabled!\n", __func__);
		}
		else {
			TestSuite_Init(NULL, &hlds_exe, NULL);
		}
#endif
	}

	return TRUE;
}

#else // _WIN32

void __attribute__((constructor)) DllMainLoad()
{

}

void __attribute__((destructor)) DllMainUnload()
{

}

#endif // _WIN32