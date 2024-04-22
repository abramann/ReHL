#pragma once

#include "VGui.h"


namespace vgui2
{
	class Panel;

	class FocusNavGroup
	{
	public:
		FocusNavGroup(Panel *panel);

		virtual Panel* GetDefaultPanel();
		virtual void SetDefaultButton(vgui2::Panel *panel);
		virtual VPANEL GetDefaultButton();
		virtual VPANEL GetCurrentDefaultButton();
		virtual Panel* FindPanelByHotkey(wchar_t key);
		virtual bool RequestFocusPrev(vgui2::VPANEL panel);
		virtual bool RequestFocusNext(vgui2::VPANEL panel);
		virtual Panel* GetCurrentFocus();
		virtual VPANEL SetCurrentFocus(vgui2::VPANEL focus, vgui2::VPANEL defaultPanel);
		virtual void SetFocusTopLevel(bool state);
		virtual void SetCurrentDefaultButton(vgui2::VPANEL panel, bool sendCurrentDefaultButtonMessage);

	private:
		VPanelHandle _defaultButton;
		VPanelHandle _currentDefaultButton;
		VPanelHandle _currentFocus;
		Panel *_mainPanel;
		bool _topLevelFocus;
	};
}