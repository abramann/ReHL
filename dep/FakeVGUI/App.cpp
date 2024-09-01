#include "App.h"


using namespace vgui;

App::App()
{
}

App::App(bool externalMain)
{
}

App * App::getInstance(void)
{
	return nullptr;
}

void App::start()
{
}

void App::stop()
{
}

void App::externalTick()
{
}

bool App::wasMousePressed(MouseCode code, Panel * panel)
{
	return false;
}

bool App::wasMouseDoublePressed(MouseCode code, Panel * panel)
{
	return false;
}

bool App::isMouseDown(MouseCode code, Panel * panel)
{
	return false;
}

bool App::wasMouseReleased(MouseCode code, Panel * panel)
{
	return false;
}

bool App::wasKeyPressed(KeyCode key, Panel * panel)
{
	return false;
}

bool App::isKeyDown(KeyCode key, Panel * panel)
{
	return false;
}

bool App::wasKeyTyped(KeyCode key, Panel * panel)
{
	return false;
}

bool App::wasKeyReleased(KeyCode key, Panel * panel)
{
	return false;
}

void App::addTickSignal(TickSignal * s)
{
}

void App::setCursorPos(int x, int y)
{
}

void App::getCursorPos(int & x, int & y)
{
}

void App::setMouseCapture(Panel * panel)
{
}

void App::setMouseArena(int x0, int y0, int x1, int y1, bool enabled)
{
}

void App::setMouseArena(Panel *)
{
}

void App::requestFocus(Panel * panel)
{
}

Panel * App::getFocus(void)
{
	return nullptr;
}

void App::repaintAll(void)
{
}

void App::setScheme(Scheme * scheme)
{
}

Scheme * App::getScheme(void)
{
	return nullptr;
}

void App::enableBuildMode(void)
{
}

long App::getTimeMillis(void)
{
	return 0;
}

char App::getKeyCodeChar(KeyCode code, bool shifted)
{
	return 0;
}

void App::getKeyCodeText(KeyCode code, char * buf, int buflen)
{
}

int App::getClipboardTextCount(void)
{
	return 0;
}

void App::setClipboardText(const char * text, int textLen)
{
}

int App::getClipboardText(int offset, char * buf, int bufLen)
{
	return 0;
}

void App::reset(void)
{
}

void App::internalSetMouseArena(int x0, int y0, int x1, int y1, bool enabled)
{
}

bool App::setRegistryString(const char * key, const char * value)
{
	return false;
}

bool App::getRegistryString(const char * key, char * value, int valueLen)
{
	return false;
}

bool App::setRegistryInteger(const char * key, int value)
{
	return false;
}

bool App::getRegistryInteger(const char * key, int & value)
{
	return false;
}

void App::setCursorOveride(Cursor * cursor)
{
}

Cursor * App::getCursorOveride(void)
{
	return nullptr;
}

void App::setMinimumTickMillisInterval(int interval)
{
}

void App::main(int argc, char * argv[])
{
}

void App::run(void)
{
}

void App::internalCursorMoved(int x, int y, SurfaceBase * surfaceBase)
{
}

void App::internalMousePressed(MouseCode code, SurfaceBase * surfaceBase)
{
}

void App::internalMouseDoublePressed(MouseCode code, SurfaceBase * surfaceBase)
{
}

void App::internalMouseReleased(MouseCode code, SurfaceBase * surfaceBase)
{
}

void App::internalMouseWheeled(int delta, SurfaceBase * surfaceBase)
{
}

void App::internalKeyPressed(KeyCode code, SurfaceBase * surfaceBase)
{
}

void App::internalKeyTyped(KeyCode code, SurfaceBase * surfaceBase)
{
}

void App::internalKeyReleased(KeyCode code, SurfaceBase * surfaceBase)
{
}

void App::init(void)
{
}

void App::updateMouseFocus(int x, int y, SurfaceBase * surfaceBase)
{
}

void App::setMouseFocus(Panel * panel)
{
}

void App::surfaceBaseCreated(SurfaceBase * surfaceBase)
{
}

void App::surfaceBaseDeleted(SurfaceBase * surfaceBase)
{
}

void App::platTick(void)
{
}

void App::internalTick(void)
{
}