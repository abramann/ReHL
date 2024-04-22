#pragma once

#include "vgui2//IInput.h"

#define VGUI_INPUTINTERNAL_INTERFACE_VERSION "VGUI_InputInternal001"



namespace vgui2
{

	enum MouseCodeState_t
	{
		BUTTON_RELEASED = 0,
		BUTTON_PRESSED,
		BUTTON_DOUBLECLICKED,
	};

	typedef int HInputContext;

	class IInputInternal : public IInput
	{
	public:
		virtual ~IInputInternal();

		// processes input for a frame
		virtual void RunFrame() = 0;

		virtual void UpdateMouseFocus(int x, int y) = 0;

		// called when a panel becomes invalid
		virtual void PanelDeleted(VPANEL panel) = 0;

		// inputs into vgui input handling 
		virtual bool InternalCursorMoved(int x, int y) = 0; //expects input in surface space
		virtual bool InternalMousePressed(MouseCode code) = 0;
		virtual bool InternalMouseDoublePressed(MouseCode code) = 0;
		virtual bool InternalMouseReleased(MouseCode code) = 0;
		virtual bool InternalMouseWheeled(int delta) = 0;
		virtual bool InternalKeyCodePressed(KeyCode code) = 0;
		virtual void InternalKeyCodeTyped(KeyCode code) = 0;
		virtual void InternalKeyTyped(wchar_t unichar) = 0;
		virtual bool InternalKeyCodeReleased(KeyCode code) = 0;

		// Creates/ destroys "input" contexts, which contains information
		// about which controls have mouse + key focus, for example.
		virtual HInputContext CreateInputContext() = 0;
		virtual void DestroyInputContext(HInputContext context) = 0;

		// Associates a particular panel with an input context
		// Associating NULL is valid; it disconnects the panel from the context
		virtual void AssociatePanelWithInputContext(HInputContext context, VPANEL pRoot) = 0;

		// Activates a particular input context, use DEFAULT_INPUT_CONTEXT
		// to get the one normally used by VGUI
		virtual void ActivateInputContext(HInputContext context) = 0;

		//virtual VPANEL GetMouseCapture() = 0;
		//virtual bool IsChildOfModalPanel(VPANEL panel) = 0;
	};
}