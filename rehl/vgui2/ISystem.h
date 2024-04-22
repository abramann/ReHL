#pragma once

#include "interface.h"


namespace vgui2
{
	class ISystem : public IBaseInterface
	{
	public:
		virtual ~ISystem() {}

		virtual void Shutdown() = 0;
		virtual void RunFrame() = 0;
		virtual void ShellExecute(const char *command, const char *file) = 0;
		virtual double GetFrameTime() = 0;
		virtual double GetCurrentTime() = 0;
		virtual int GetTimeMillis() = 0;
		virtual int GetClipboardTextCount() = 0;
		virtual void SetClipboardText(const char *text, int textLen) = 0;
		virtual void SetClipboardText(const wchar_t *text, int textLen) = 0;
		virtual int GetClipboardText(int offset, char *buf, int bufLen) = 0;
		virtual int GetClipboardText(int offset, wchar_t *buf, int bufLen) = 0;
		virtual bool SetRegistryString(const char *key, const char *value) = 0;
		virtual bool GetRegistryString(const char *key, char *value, int valueLen) = 0;
		virtual bool SetRegistryInteger(const char *key, int value) = 0;
		virtual bool GetRegistryInteger(const char *key, int *const value) = 0;
		virtual KeyValues* GetUserConfigFileData(const char *dialogName, int dialogID) = 0;
		virtual void SetUserConfigFile(const char *fileName, const char *pathName) = 0;
		virtual void SaveUserConfigFile() = 0;
		virtual bool SetWatchForComputerUse(bool state) = 0;
		virtual double GetTimeSinceLastUse() = 0;
		virtual int GetAvailableDrives(char *buf, int bufLen) = 0;
		virtual bool CommandLineParamExists(const char *paramName) = 0;
		virtual const char* GetFullCommandLine() = 0;
		virtual bool GetCurrentTimeAndDate(int *year, int *month, int *dayOfWeek, int *day, int *hour, int *minute, int *second) = 0;
		virtual double GetFreeDiskSpace(const char *path) = 0;
		virtual bool CreateShortcut(const char *linkFileName, const char *targetPath, const char *arguments, const char *workingDirectory, const char *iconFile) = 0;
		virtual bool GetShortcutTarget(const char *linkFileName, char *targetPath, char *arguments, int destBufferSizes) = 0;
		virtual bool ModifyShortcutTarget(const char *linkFileName, const char *targetPath, const char *arguments, const char *workingDirectory) = 0;
		virtual bool GetCommandLineParamValue(const char *paramName, char *value, int valueBufferSize) = 0;
		virtual bool DeleteRegistryKey(const char *key) = 0;
		virtual const char* GetDesktopFolderPath() = 0;
		virtual vgui2::KeyCode KeyCode_VirtualKeyToVGUI(int keyCode) = 0;
		virtual int KeyCode_VGUIToVirtualKey(vgui2::KeyCode keyCode) = 0;
		virtual const char* GetStartMenuFolderPath() = 0;
		virtual const char* GetAllUserDesktopFolderPath() = 0;
		virtual const char* GetAllUserStartMenuFolderPath() = 0;
	};
}

#define VGUI_SYSTEM_INTERFACE_VERSION "VGUI_System009"
