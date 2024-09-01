#include "InputStream.h"


using namespace vgui;

FileInputStream::FileInputStream(const char *fileName, bool textMode)
{
}

void FileInputStream::seekStart(bool *const success)
{
}

void FileInputStream::seekRelative(int count, bool *const success)
{
}

void FileInputStream::seekEnd(bool *const success)
{
}

int FileInputStream::getAvailable(bool *const success)
{
	return 0;
}

unsigned char FileInputStream::readUChar(bool *const success)
{
	return 0;
}

void FileInputStream::readUChar(unsigned char *buf, int count, bool *const success)
{
}

void FileInputStream::close(bool *const success)
{
}

void FileInputStream::close()
{
}