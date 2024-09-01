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

	class App
	{
	public:
		VGUI_DLL_API App();
		//~App() = delete;

		VGUI_DLL_API App(bool externalMain);

		static VGUI_DLL_API App * getInstance(void);

		virtual VGUI_DLL_API void start();
		virtual VGUI_DLL_API void stop();
		virtual VGUI_DLL_API void externalTick();
		virtual VGUI_DLL_API bool wasMousePressed(vgui::MouseCode code, vgui::Panel * panel);
		virtual VGUI_DLL_API  bool wasMouseDoublePressed(vgui::MouseCode code, vgui::Panel * panel);
		virtual VGUI_DLL_API  bool isMouseDown(vgui::MouseCode code, vgui::Panel * panel);
		virtual VGUI_DLL_API  bool wasMouseReleased(vgui::MouseCode code, vgui::Panel * panel);
		virtual VGUI_DLL_API  bool wasKeyPressed(vgui::KeyCode key, vgui::Panel * panel);
		virtual VGUI_DLL_API  bool isKeyDown(vgui::KeyCode key, vgui::Panel * panel);
		virtual VGUI_DLL_API  bool wasKeyTyped(vgui::KeyCode key, vgui::Panel * panel);
		virtual VGUI_DLL_API  bool wasKeyReleased(vgui::KeyCode key, vgui::Panel * panel);
		virtual VGUI_DLL_API  void addTickSignal(vgui::TickSignal * s);
		virtual VGUI_DLL_API  void  setCursorPos(int x, int y);
		virtual VGUI_DLL_API  void getCursorPos(int& x, int& y);
		virtual VGUI_DLL_API  void setMouseCapture(vgui::Panel * panel);
		virtual VGUI_DLL_API  void setMouseArena(int x0, int y0, int x1, int y1, bool enabled);
		virtual VGUI_DLL_API  void setMouseArena(vgui::Panel *);
		virtual VGUI_DLL_API  void requestFocus(vgui::Panel * panel);
		virtual VGUI_DLL_API  vgui::Panel* getFocus(void);
		virtual VGUI_DLL_API  void repaintAll(void);
		virtual VGUI_DLL_API  void setScheme(vgui::Scheme * scheme);
		virtual VGUI_DLL_API  vgui::Scheme * getScheme(void);
		virtual VGUI_DLL_API  void enableBuildMode(void);
		virtual VGUI_DLL_API  long getTimeMillis(void);
		virtual VGUI_DLL_API  char getKeyCodeChar(vgui::KeyCode code, bool shifted);
		virtual VGUI_DLL_API  void getKeyCodeText(vgui::KeyCode code, char * buf, int buflen);
		virtual VGUI_DLL_API  int getClipboardTextCount(void);
		virtual VGUI_DLL_API  void setClipboardText(const char *text, int textLen);
		virtual VGUI_DLL_API  int getClipboardText(int offset, char *buf, int bufLen);
		virtual VGUI_DLL_API  void reset(void);
		virtual VGUI_DLL_API  void internalSetMouseArena(int x0, int y0, int x1, int y1, bool enabled);
		virtual VGUI_DLL_API  bool setRegistryString(const char *key, const char *value);
		virtual VGUI_DLL_API  bool getRegistryString(const char *key, char *value, int valueLen);
		virtual VGUI_DLL_API  bool setRegistryInteger(const char *key, int value);
		virtual VGUI_DLL_API  bool getRegistryInteger(const char *key, int& value);
		virtual VGUI_DLL_API  void setCursorOveride(vgui::Cursor *cursor);
		virtual VGUI_DLL_API  vgui::Cursor * getCursorOveride(void);
		virtual VGUI_DLL_API  void setMinimumTickMillisInterval(int interval);
		virtual VGUI_DLL_API  void main(int argc, char* argv[]) = 0;
		virtual VGUI_DLL_API  void run(void);
		virtual VGUI_DLL_API  void internalCursorMoved(int x, int y, vgui::SurfaceBase *surfaceBase);
		virtual VGUI_DLL_API  void internalMousePressed(vgui::MouseCode code, vgui::SurfaceBase *surfaceBase);
		virtual VGUI_DLL_API  void internalMouseDoublePressed(vgui::MouseCode code, vgui::SurfaceBase *surfaceBase);
		virtual VGUI_DLL_API  void internalMouseReleased(vgui::MouseCode code, vgui::SurfaceBase *surfaceBase);
		virtual VGUI_DLL_API  void internalMouseWheeled(int delta, vgui::SurfaceBase *surfaceBase);
		virtual VGUI_DLL_API  void internalKeyPressed(vgui::KeyCode code, vgui::SurfaceBase *surfaceBase);
		virtual VGUI_DLL_API  void internalKeyTyped(vgui::KeyCode code, vgui::SurfaceBase *surfaceBase);
		virtual VGUI_DLL_API  void internalKeyReleased(vgui::KeyCode code, vgui::SurfaceBase *surfaceBase);

	private:
		virtual VGUI_DLL_API void init(void);
		virtual VGUI_DLL_API void updateMouseFocus(int x, int y, vgui::SurfaceBase *surfaceBase);
		virtual VGUI_DLL_API void setMouseFocus(vgui::Panel *panel);

	protected:
		virtual VGUI_DLL_API void surfaceBaseCreated(vgui::SurfaceBase *surfaceBase);
		virtual VGUI_DLL_API void surfaceBaseDeleted(vgui::SurfaceBase *surfaceBase);

	public:
		virtual VGUI_DLL_API void platTick(void);

	protected:
		virtual VGUI_DLL_API void internalTick(void);

		static VGUI_DLL_API App * _instance;
	private:
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
	};
}