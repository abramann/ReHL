#pragma once
#pragma once

#include "dll_api.h"
#include "MouseCode.h"
#include "KeyCode.h"
#include "Dar.h"


namespace vgui
{
	class Panel;
	class TickSignal;
	class Scheme;
	class Cursor;
	class SurfaceBase;

	class VGUI_DLL_API App
	{
	public:
		App();
		//~App() = delete;

		App(bool externalMain);

		static App * getInstance(void);

		virtual void start();
		virtual void stop();
		virtual void externalTick();
		virtual bool wasMousePressed(vgui::MouseCode code, vgui::Panel * panel);
		virtual  bool wasMouseDoublePressed(vgui::MouseCode code, vgui::Panel * panel);
		virtual  bool isMouseDown(vgui::MouseCode code, vgui::Panel * panel);
		virtual  bool wasMouseReleased(vgui::MouseCode code, vgui::Panel * panel);
		virtual  bool wasKeyPressed(vgui::KeyCode key, vgui::Panel * panel);
		virtual  bool isKeyDown(vgui::KeyCode key, vgui::Panel * panel);
		virtual  bool wasKeyTyped(vgui::KeyCode key, vgui::Panel * panel);
		virtual  bool wasKeyReleased(vgui::KeyCode key, vgui::Panel * panel);
		virtual  void addTickSignal(vgui::TickSignal * s);
		virtual  void  setCursorPos(int x, int y);
		virtual  void getCursorPos(int& x, int& y);
		virtual  void setMouseCapture(vgui::Panel * panel);
		virtual  void setMouseArena(int x0, int y0, int x1, int y1, bool enabled);
		virtual  void setMouseArena(vgui::Panel *);
		virtual  void requestFocus(vgui::Panel * panel);
		virtual  vgui::Panel* getFocus(void);
		virtual  void repaintAll(void);
		virtual  void setScheme(vgui::Scheme * scheme);
		virtual  vgui::Scheme * getScheme(void);
		virtual  void enableBuildMode(void);
		virtual  long getTimeMillis(void);
		virtual  char getKeyCodeChar(vgui::KeyCode code, bool shifted);
		virtual  void getKeyCodeText(vgui::KeyCode code, char * buf, int buflen);
		virtual  int getClipboardTextCount(void);
		virtual  void setClipboardText(const char *text, int textLen);
		virtual  int getClipboardText(int offset, char *buf, int bufLen);
		virtual  void reset(void);
		virtual  void internalSetMouseArena(int x0, int y0, int x1, int y1, bool enabled);
		virtual  bool setRegistryString(const char *key, const char *value);
		virtual  bool getRegistryString(const char *key, char *value, int valueLen);
		virtual  bool setRegistryInteger(const char *key, int value);
		virtual  bool getRegistryInteger(const char *key, int& value);
		virtual  void setCursorOveride(vgui::Cursor *cursor);
		virtual  vgui::Cursor * getCursorOveride(void);
		virtual  void setMinimumTickMillisInterval(int interval);
		virtual  void main(int argc, char* argv[]) = 0;
		virtual  void run(void);
		virtual  void internalCursorMoved(int x, int y, vgui::SurfaceBase *surfaceBase);
		virtual  void internalMousePressed(vgui::MouseCode code, vgui::SurfaceBase *surfaceBase);
		virtual  void internalMouseDoublePressed(vgui::MouseCode code, vgui::SurfaceBase *surfaceBase);
		virtual  void internalMouseReleased(vgui::MouseCode code, vgui::SurfaceBase *surfaceBase);
		virtual  void internalMouseWheeled(int delta, vgui::SurfaceBase *surfaceBase);
		virtual  void internalKeyPressed(vgui::KeyCode code, vgui::SurfaceBase *surfaceBase);
		virtual  void internalKeyTyped(vgui::KeyCode code, vgui::SurfaceBase *surfaceBase);
		virtual  void internalKeyReleased(vgui::KeyCode code, vgui::SurfaceBase *surfaceBase);

	private:
		virtual  void init(void);
		virtual  void updateMouseFocus(int x, int y, vgui::SurfaceBase *surfaceBase);
		virtual  void setMouseFocus(vgui::Panel *panel);

	protected:
		virtual  void surfaceBaseCreated(vgui::SurfaceBase *surfaceBase);
		virtual  void surfaceBaseDeleted(vgui::SurfaceBase *surfaceBase);

	public:
		virtual  void platTick(void);

	protected:
		virtual  void internalTick(void);

	private:
		/*
		bool _running;
		bool _externalMain;
		vgui::Dar<vgui::SurfaceBase*> _surfaceBaseDar;
		vgui::Panel *_keyFocus;
		vgui::Panel *_oldMouseFocus;
		vgui::Panel *_mouseFocus;
		vgui::Panel *_mouseCapture;
		vgui::Panel *_wantedKeyFocus;
		bool _mousePressed[3];
		bool _mouseDoublePressed[3];
		bool _mouseDown[3];
		bool _mouseReleased[3];
		bool _keyPressed[103];
		bool _keyTyped[103];
		bool _keyDown[103];
		bool _keyReleased[103];
		vgui::Dar<vgui::TickSignal*> _tickSignalDar;
		vgui::Scheme *_scheme;
		bool _buildMode;
		bool _wantedBuildMode;
		vgui::Panel *_mouseArenaPanel;
		vgui::Cursor *_cursor[14];
		vgui::Cursor *_cursorOveride;
		int _nextTickMillis;
		int _minimumTickMillisInterval;
		*/
	};
}