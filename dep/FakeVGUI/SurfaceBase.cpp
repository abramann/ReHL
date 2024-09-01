#include "SurfaceBase.h"


using namespace vgui;

SurfaceBase::~SurfaceBase()
{
}

void vgui::SurfaceBase::addModeInfo(int wide, int tall, int bpp)
{
}

vgui::SurfaceBase::SurfaceBase(vgui::Panel * pPanel)
{
}

vgui::SurfaceBase::SurfaceBase(const SurfaceBase & surfaceBase)
{
}

vgui::Panel * vgui::SurfaceBase::getPanel(void)
{
	return nullptr;
}

void vgui::SurfaceBase::requestSwap(void)
{
}

void vgui::SurfaceBase::resetModeInfo(void)
{
}

int vgui::SurfaceBase::getModeInfoCount(void)
{
	return 0;
}

bool vgui::SurfaceBase::getModeInfo(int index, int & wide, int & tall, int & bpp)
{
	return false;
}

vgui::App * vgui::SurfaceBase::getApp(void)
{
	return nullptr;
}

void vgui::SurfaceBase::setEmulatedCursorVisible(bool state)
{
}

void vgui::SurfaceBase::setEmulatedCursorPos(int x, int y)
{
}
