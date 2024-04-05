#pragma once

#include "dll_api.h"
#include <cstdio>

namespace vgui
{
	class VGUI_DLL_API InputStream
	{
	public:
		virtual void seekStart(bool *const success) = 0;
		virtual void seekRelative(int count, bool *const success) = 0;
		virtual void seekEnd(bool *const success) = 0;
		virtual int getAvailable(bool *const success) = 0;
		virtual unsigned char readUChar(bool *const success) = 0;
		virtual void readUChar(unsigned char *buf, int count, bool *const success) = 0;
		virtual void close(bool *const success) = 0;
	};

	class VGUI_DLL_API FileInputStream : InputStream
	{
	public:
		FileInputStream(const char *fileName, bool textMode);

		virtual void seekStart(bool *const success);
		virtual void seekRelative(int count, bool *const success);
		virtual void seekEnd(bool *const success);
		virtual int getAvailable(bool *const success);
		virtual unsigned char readUChar(bool *const success);
		virtual void readUChar(unsigned char *buf, int count, bool *const success);
		virtual void close(bool *const success);
		virtual void close();
	private:
		FILE* _fp;
	};
}