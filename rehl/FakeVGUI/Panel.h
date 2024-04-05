#pragma once

#include "dll_api.h"
#include "MouseCode.h"
#include "KeyCode.h"
#include "Scheme.h"


namespace vgui
{
	class BuildGroup;
	class Border;
	class Cursor;
	class SurfaceBase;
	class FocusChangeSignal;
	class FocusNavGroup;
	class Layout;
	class LayoutInfo;
	class InputSignal;
	class Scheme;
	class SchemeFont;
	class SchemeColor;
	class RepaintSignal;
	class Color;
	class App;

	class VGUI_DLL_API Panel
	{
	public:

		Panel(int x, int y, int wide, int tall);

		virtual void  setPos(int x, int y);
		virtual void  getPos(int *const x, int *const y);
		virtual void  setSize(int wide, int tall);
		virtual void  getSize(int *const wide, int *const tall);
		virtual void  setBounds(int x, int y, int wide, int tall);
		virtual void  getBounds(int *const x, int *const y, int *const wide, int *const tall);
		virtual int  getWide();
		virtual int  getTall();
		virtual vgui::Panel * getParent();
		virtual void  setVisible(bool state);
		virtual bool  isVisible();
		virtual bool  isVisibleUp();
		virtual void  repaint();
		virtual void  repaintAll();
		virtual void  getAbsExtents(int *const x0, int *const y0, int *const x1, int *const y1);
		virtual void  getClipRect(int *const x0, int *const y0, int *const x1, int *const y1);
		virtual void  setParent(vgui::Panel *newParent);
		virtual void  addChild(vgui::Panel *child);
		virtual void  insertChildAt(vgui::Panel *child, int index);
		virtual void  removeChild(vgui::Panel *child);
		virtual bool  wasMousePressed(vgui::MouseCode code);
		virtual bool  wasMouseDoublePressed(vgui::MouseCode code);
		virtual bool  isMouseDown(vgui::MouseCode code);
		virtual bool  wasMouseReleased(vgui::MouseCode code);
		virtual bool  wasKeyPressed(vgui::KeyCode code);
		virtual bool  isKeyDown(vgui::KeyCode code);
		virtual bool  wasKeyTyped(vgui::KeyCode code);
		virtual bool  wasKeyReleased(vgui::KeyCode code);
		virtual void  addInputSignal(vgui::InputSignal *s);
		virtual void  removeInputSignal(vgui::InputSignal *s);
		virtual void  addRepaintSignal(vgui::RepaintSignal *s);
		virtual void  removeRepaintSignal(vgui::RepaintSignal *s);
		virtual bool  isWithin(int x, int y);
		virtual vgui::Panel * isWithinTraverse(int x, int y);
		virtual void  localToScreen(int *const x, int *const y);
		virtual void  screenToLocal(int *const x, int *const y);
		virtual void  setCursor(vgui::Cursor *cursor);
		virtual void  setCursor(vgui::Scheme::SchemeCursor scu);
		vgui::Cursor * getCursor();
		virtual void  setMinimumSize(int wide, int tall);
		virtual void  getMinimumSize(int *const wide, int *const tall);
		virtual void  requestFocus();
		virtual bool  hasFocus();
		virtual int  getChildCount();
		vgui::Panel * getChild(int index);
		virtual void  setLayout(vgui::Layout *layout);
		virtual void  invalidateLayout(bool layoutNow);
		virtual void  setFocusNavGroup(vgui::FocusNavGroup *focusNavGroup);
		virtual void  requestFocusPrev();
		virtual void  requestFocusNext();
		virtual void  addFocusChangeSignal(vgui::FocusChangeSignal *s);
		virtual bool  isAutoFocusNavEnabled();
		virtual void  setAutoFocusNavEnabled(bool state);
		virtual void  setBorder(vgui::Border *border);
		virtual void  setPaintBorderEnabled(bool state);
		virtual void  setPaintBackgroundEnabled(bool state);
		virtual void  setPaintEnabled(bool state);
		virtual void  getInset(int *const left, int *const top, int *const right, int *const bottom);
		virtual void  getPaintSize(int *const wide, int *const tall);
		virtual void  setPreferredSize(int wide, int tall);
		virtual void  getPreferredSize(int *const wide, int *const tall);
		virtual vgui::SurfaceBase * getSurfaceBase();
		virtual bool  isEnabled();
		virtual void  setEnabled(bool state);
		virtual void  setBuildGroup(vgui::BuildGroup *buildGroup, const char *panelPersistanceName);
		virtual bool  isBuildGroupEnabled();
		virtual void  removeAllChildren();
		virtual void  repaintParent();
		virtual vgui::Panel * createPropertyPanel();
		virtual void  getPersistanceText(char *buf, int bufLen);
		virtual void  applyPersistanceText(const char *buf);
		virtual void  setFgColor(vgui::Scheme::SchemeColor sc);
		virtual void  setBgColor(vgui::Scheme::SchemeColor sc);
		virtual void  setFgColor(int r, int g, int b, int a);
		virtual void  setBgColor(int r, int g, int b, int a);
		virtual void  getFgColor(int *const r, int *const g, int *const b, int *const a);
		virtual void  getBgColor(int *const r, int *const g, int *const b, int *const a);
		virtual void  setBgColor(vgui::Color *p_color);
		virtual void  setFgColor(vgui::Color *p_color);
		virtual void  getBgColor(vgui::Color *const color);
		virtual void  getFgColor(vgui::Color *const color);
		virtual void  setAsMouseCapture(bool state);
		virtual void  setAsMouseArena(bool state);
		virtual vgui::App * getApp();
		virtual void  getVirtualSize(int *const wide, int *const tall);
		virtual void  setLayoutInfo(vgui::LayoutInfo *layoutInfo);
		vgui::LayoutInfo * getLayoutInfo();
		virtual bool  isCursorNone();
		virtual void  solveTraverse();
		virtual void  paintTraverse();
		virtual void  setSurfaceBaseTraverse(vgui::SurfaceBase *surfaceBase);
		virtual void  performLayout();
		virtual void  internalPerformLayout();
		virtual void  drawSetColor(vgui::Scheme::SchemeColor sc);
		virtual void  drawSetColor(int r, int g, int b, int a);
		virtual void  drawFilledRect(int x0, int y0, int x1, int y1);
		virtual void  drawOutlinedRect(int x0, int y0, int x1, int y1);
		virtual void  drawSetTextFont(vgui::Scheme::SchemeFont sf_0);
		virtual void  drawSetTextFont(vgui::Font *font);
		virtual void  drawSetTextColor(vgui::Scheme::SchemeColor sc);
		virtual void  drawSetTextColor(int r, int g, int b, int a);
		virtual void  drawSetTextPos(int x, int y);
		virtual void  drawPrintText(const char *str, int strlen);
		virtual void  drawPrintText(int x, int y, const char *str, int strlen);
		virtual void  drawPrintChar(char ch_0);
		virtual void  drawPrintChar(int x, int y, char ch_0);
		virtual void  drawSetTextureRGBA(int id, const char *rgba, int wide, int tall);
		virtual void  drawSetTexture(int id);
		virtual void  drawTexturedRect(int x0, int y0, int x1, int y1);
		virtual void  solve();
		virtual void  paintTraverse(bool repaint);
		virtual void  paintBackground();
		virtual void  paint();
		virtual void  paintBuildOverlay();
		virtual void  internalCursorMoved(int x, int y);
		virtual void  internalCursorEntered();
		virtual void  internalCursorExited();
		virtual void  internalMousePressed(vgui::MouseCode code);
		virtual void  internalMouseDoublePressed(vgui::MouseCode code);
		virtual void  internalMouseReleased(vgui::MouseCode code);
		virtual void  internalMouseWheeled(int delta);
		virtual void  internalKeyPressed(vgui::KeyCode code);
		virtual void  internalKeyTyped(vgui::KeyCode code);
		virtual void  internalKeyReleased(vgui::KeyCode code);
		virtual void  internalKeyFocusTicked();
		virtual void  internalFocusChanged(bool lost);
		virtual void  internalSetCursor();
	};
}