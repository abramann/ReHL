#include "panel.h"

using namespace vgui;

Panel::Panel(int x, int y, int wide, int tall)
{
}

void Panel::setPos(int x, int y)
{
}

void Panel::getPos(int * const x, int * const y)
{
}

void Panel::setSize(int wide, int tall)
{
}

void Panel::getSize(int * const wide, int * const tall)
{
}

void Panel::setBounds(int x, int y, int wide, int tall)
{
}

void Panel::getBounds(int * const x, int * const y, int * const wide, int * const tall)
{
}

int Panel::getWide()
{
	return 0;
}

int Panel::getTall()
{
	return 0;
}

Panel * Panel::getParent()
{
	return nullptr;
}

void Panel::setVisible(bool state)
{
}

bool Panel::isVisible()
{
	return false;
}

bool Panel::isVisibleUp()
{
	return false;
}

void Panel::repaint()
{
}

void Panel::repaintAll()
{
}

void Panel::getAbsExtents(int * const x0, int * const y0, int * const x1, int * const y1)
{
}

void Panel::getClipRect(int * const x0, int * const y0, int * const x1, int * const y1)
{
}

void Panel::setParent(Panel * newParent)
{
}

void Panel::addChild(Panel * child)
{
}

void Panel::insertChildAt(Panel * child, int index)
{
}

void Panel::removeChild(Panel * child)
{
}

bool Panel::wasMousePressed(MouseCode code)
{
	return false;
}

bool Panel::wasMouseDoublePressed(MouseCode code)
{
	return false;
}

bool Panel::isMouseDown(MouseCode code)
{
	return false;
}

bool Panel::wasMouseReleased(MouseCode code)
{
	return false;
}

bool Panel::wasKeyPressed(KeyCode code)
{
	return false;
}

bool Panel::isKeyDown(KeyCode code)
{
	return false;
}

bool Panel::wasKeyTyped(KeyCode code)
{
	return false;
}

bool Panel::wasKeyReleased(KeyCode code)
{
	return false;
}

void Panel::addInputSignal(InputSignal * s)
{
}

void Panel::removeInputSignal(InputSignal * s)
{
}

void Panel::addRepaintSignal(RepaintSignal * s)
{
}

void Panel::removeRepaintSignal(RepaintSignal * s)
{
}

bool Panel::isWithin(int x, int y)
{
	return false;
}

Panel * Panel::isWithinTraverse(int x, int y)
{
	return nullptr;
}

void Panel::localToScreen(int * const x, int * const y)
{
}

void Panel::screenToLocal(int * const x, int * const y)
{
}

void Panel::setCursor(Cursor * cursor)
{
}

void Panel::setCursor(Scheme::SchemeCursor scu)
{
}

Cursor * Panel::getCursor()
{
	return nullptr;
}

void Panel::setMinimumSize(int wide, int tall)
{
}

void Panel::getMinimumSize(int * const wide, int * const tall)
{
}

void Panel::requestFocus()
{
}

bool Panel::hasFocus()
{
	return false;
}

int Panel::getChildCount()
{
	return 0;
}

Panel * Panel::getChild(int index)
{
	return nullptr;
}

void Panel::setLayout(Layout * layout)
{
}

void Panel::invalidateLayout(bool layoutNow)
{
}

void Panel::setFocusNavGroup(FocusNavGroup * focusNavGroup)
{
}

void Panel::requestFocusPrev()
{
}

void Panel::requestFocusNext()
{
}

void Panel::addFocusChangeSignal(FocusChangeSignal * s)
{
}

bool Panel::isAutoFocusNavEnabled()
{
	return false;
}

void Panel::setAutoFocusNavEnabled(bool state)
{
}

void Panel::setBorder(Border * border)
{
}

void Panel::setPaintBorderEnabled(bool state)
{
}

void Panel::setPaintBackgroundEnabled(bool state)
{
}

void Panel::setPaintEnabled(bool state)
{
}

void Panel::getInset(int * const left, int * const top, int * const right, int * const bottom)
{
}

void Panel::getPaintSize(int * const wide, int * const tall)
{
}

void Panel::setPreferredSize(int wide, int tall)
{
}

void Panel::getPreferredSize(int * const wide, int * const tall)
{
}

SurfaceBase * Panel::getSurfaceBase()
{
	return nullptr;
}

bool Panel::isEnabled()
{
	return false;
}

void Panel::setEnabled(bool state)
{
}

void Panel::setBuildGroup(BuildGroup * buildGroup, const char * panelPersistanceName)
{
}

bool Panel::isBuildGroupEnabled()
{
	return false;
}

void Panel::removeAllChildren()
{
}

void Panel::repaintParent()
{
}

Panel * Panel::createPropertyPanel()
{
	return nullptr;
}

void Panel::getPersistanceText(char * buf, int bufLen)
{
}

void Panel::applyPersistanceText(const char * buf)
{
}

void Panel::setFgColor(Scheme::SchemeColor sc)
{
}

void Panel::setBgColor(Scheme::SchemeColor sc)
{
}

void Panel::setFgColor(int r, int g, int b, int a)
{
}

void Panel::setBgColor(int r, int g, int b, int a)
{
}

void Panel::getFgColor(int * const r, int * const g, int * const b, int * const a)
{
}

void Panel::getBgColor(int * const r, int * const g, int * const b, int * const a)
{
}

void Panel::setBgColor(vgui::Color * p_color)
{
}

void Panel::setFgColor(vgui::Color * p_color)
{
}

void Panel::getBgColor(vgui::Color * const color)
{
}

void Panel::getFgColor(vgui::Color * const color)
{
}

void Panel::setAsMouseCapture(bool state)
{
}

void Panel::setAsMouseArena(bool state)
{
}

App * Panel::getApp()
{
	return nullptr;
}

void Panel::getVirtualSize(int * const wide, int * const tall)
{
}

void Panel::setLayoutInfo(LayoutInfo * layoutInfo)
{
}

LayoutInfo * Panel::getLayoutInfo()
{
	return nullptr;
}

bool Panel::isCursorNone()
{
	return false;
}

void Panel::solveTraverse()
{
}

void Panel::paintTraverse()
{
}

void Panel::setSurfaceBaseTraverse(SurfaceBase * surfaceBase)
{
}

void Panel::performLayout()
{
}

void Panel::internalPerformLayout()
{
}

void Panel::drawSetColor(Scheme::SchemeColor sc)
{
}

void Panel::drawSetColor(int r, int g, int b, int a)
{
}

void Panel::drawFilledRect(int x0, int y0, int x1, int y1)
{
}

void Panel::drawOutlinedRect(int x0, int y0, int x1, int y1)
{
}

void Panel::drawSetTextFont(Scheme::SchemeFont sf_0)
{
}

void Panel::drawSetTextFont(Font * font)
{
}

void Panel::drawSetTextColor(Scheme::SchemeColor sc)
{
}

void Panel::drawSetTextColor(int r, int g, int b, int a)
{
}

void Panel::drawSetTextPos(int x, int y)
{
}

void Panel::drawPrintText(const char * str, int strlen)
{
}

void Panel::drawPrintText(int x, int y, const char * str, int strlen)
{
}

void Panel::drawPrintChar(char ch_0)
{
}

void Panel::drawPrintChar(int x, int y, char ch_0)
{
}

void Panel::drawSetTextureRGBA(int id, const char * rgba, int wide, int tall)
{
}

void Panel::drawSetTexture(int id)
{
}

void Panel::drawTexturedRect(int x0, int y0, int x1, int y1)
{
}

void Panel::solve()
{
}

void Panel::paintTraverse(bool repaint)
{
}

void Panel::paintBackground()
{
}

void Panel::paint()
{
}

void Panel::paintBuildOverlay()
{
}

void Panel::internalCursorMoved(int x, int y)
{
}

void Panel::internalCursorEntered()
{
}

void Panel::internalCursorExited()
{
}

void Panel::internalMousePressed(MouseCode code)
{
}

void Panel::internalMouseDoublePressed(MouseCode code)
{
}

void Panel::internalMouseReleased(MouseCode code)
{
}

void Panel::internalMouseWheeled(int delta)
{
}

void Panel::internalKeyPressed(KeyCode code)
{
}

void Panel::internalKeyTyped(KeyCode code)
{
}

void Panel::internalKeyReleased(KeyCode code)
{
}

void Panel::internalKeyFocusTicked()
{
}

void Panel::internalFocusChanged(bool lost)
{
}

void Panel::internalSetCursor()
{
}
