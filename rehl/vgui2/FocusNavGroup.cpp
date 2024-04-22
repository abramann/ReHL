#include "precompiled.h"
#include "FocusNavGroup.h"
#include "vgui2\VGui.h"

vgui2::FocusNavGroup::FocusNavGroup(Panel * panel)
{
	_mainPanel = panel;
	_currentFocus.Set(0);
	_topLevelFocus = 0;
	_defaultButton.Set(0);
	_currentDefaultButton.Set(0);
}

vgui2::Panel * vgui2::FocusNavGroup::GetDefaultPanel()
{
	NOT_IMPLEMENTED;
	return nullptr;
}

void vgui2::FocusNavGroup::SetDefaultButton(vgui2::Panel * panel)
{
	NOT_IMPLEMENTED;

}

vgui2::VPANEL vgui2::FocusNavGroup::GetDefaultButton()
{
	NOT_IMPLEMENTED;

	return VPANEL();
}

vgui2::VPANEL vgui2::FocusNavGroup::GetCurrentDefaultButton()
{
	NOT_IMPLEMENTED;

	return VPANEL();
}

vgui2::Panel * vgui2::FocusNavGroup::FindPanelByHotkey(wchar_t key)
{
	NOT_IMPLEMENTED;

	return nullptr;
}

bool vgui2::FocusNavGroup::RequestFocusPrev(vgui2::VPANEL panel)
{
	NOT_IMPLEMENTED;

	return false;
}

bool vgui2::FocusNavGroup::RequestFocusNext(vgui2::VPANEL panel)
{
	NOT_IMPLEMENTED;

	return false;
}

vgui2::Panel * vgui2::FocusNavGroup::GetCurrentFocus()
{
	NOT_IMPLEMENTED;

	return nullptr;
}

vgui2::VPANEL vgui2::FocusNavGroup::SetCurrentFocus(vgui2::VPANEL focus, vgui2::VPANEL defaultPanel)
{
	return VPANEL();
}

void vgui2::FocusNavGroup::SetFocusTopLevel(bool state)
{
	NOT_IMPLEMENTED;

}

void vgui2::FocusNavGroup::SetCurrentDefaultButton(vgui2::VPANEL panel, bool sendCurrentDefaultButtonMessage)
{
	NOT_IMPLEMENTED;

}
