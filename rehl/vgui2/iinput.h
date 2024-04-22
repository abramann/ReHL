//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//===========================================================================//

#pragma once

#include "vgui2\VGui.h"
#include "vgui2\MouseCode.h"
#include "vgui2\KeyCode.h"

#define VGUI_GCS_COMPREADSTR                 0x0001
#define VGUI_GCS_COMPREADATTR                0x0002
#define VGUI_GCS_COMPREADCLAUSE              0x0004
#define VGUI_GCS_COMPSTR                     0x0008
#define VGUI_GCS_COMPATTR                    0x0010
#define VGUI_GCS_COMPCLAUSE                  0x0020
#define VGUI_GCS_CURSORPOS                   0x0080
#define VGUI_GCS_DELTASTART                  0x0100
#define VGUI_GCS_RESULTREADSTR               0x0200
#define VGUI_GCS_RESULTREADCLAUSE            0x0400
#define VGUI_GCS_RESULTSTR                   0x0800
#define VGUI_GCS_RESULTCLAUSE                0x1000
// style bit flags for WM_IME_COMPOSITION
#define VGUI_CS_INSERTCHAR                   0x2000
#define VGUI_CS_NOMOVECARET                  0x4000

#define MESSAGE_CURSOR_POS -1
#define MESSAGE_CURRENT_KEYFOCUS -2

namespace vgui2
{

	class Cursor;
	typedef unsigned long HCursor;

#define VGUI_INPUT_INTERFACE_VERSION "VGUI_Input004"

	class IInput : public IBaseInterface
	{
	public:
		virtual ~IInput();

		virtual void SetMouseFocus(VPANEL newMouseFocus) = 0;
		virtual void SetMouseCapture(VPANEL panel) = 0;

		// returns the string name of a scan code
		virtual void GetKeyCodeText(KeyCode code, char *buf, int buflen) = 0;

		// focus
		virtual VPANEL GetFocus() = 0;
		virtual VPANEL GetMouseOver() = 0;		// returns the panel the mouse is currently over, ignoring mouse capture

		// mouse state
		virtual void SetCursorPos(int x, int y) = 0;
		virtual void GetCursorPos(int &x, int &y) = 0;
		virtual bool WasMousePressed(MouseCode code) = 0;
		virtual bool WasMouseDoublePressed(MouseCode code) = 0;
		virtual bool IsMouseDown(MouseCode code) = 0;

		// cursor override
		virtual void SetCursorOveride(HCursor cursor) = 0;
		virtual HCursor GetCursorOveride() = 0;

		// key state
		virtual bool WasMouseReleased(MouseCode code) = 0;
		virtual bool WasKeyPressed(KeyCode code) = 0;
		virtual bool IsKeyDown(KeyCode code) = 0;
		virtual bool WasKeyTyped(KeyCode code) = 0;
		virtual bool WasKeyReleased(KeyCode code) = 0;

		virtual VPANEL GetAppModalSurface() = 0;
		// set the modal dialog panel.
		// all events will go only to this panel and its children.
		virtual void SetAppModalSurface(VPANEL panel) = 0;
		// release the modal dialog panel
		// do this when your modal dialog finishes.
		virtual void ReleaseAppModalSurface() = 0;

		virtual void GetCursorPosition(int &x, int &y) = 0;
	};

	IInput* input();

} // namespace vgui2
