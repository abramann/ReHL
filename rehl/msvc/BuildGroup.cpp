#include "precompiled.h"
#include "BuildGroup.h"

vgui2::BuildGroup::BuildGroup(vgui2::Panel * parentPanel, vgui2::Panel * contextPanel)
{
	m_hBuildDialog.Set(0);
	m_pParentPanel = parentPanel;
	m_pBuildContext = contextPanel;
}

void vgui2::BuildGroup::SetEnabled(bool state)
{
	NOT_IMPLEMENTED;

}

bool vgui2::BuildGroup::IsEnabled()
{
	NOT_IMPLEMENTED;

	return false;
}

vgui2::Panel * vgui2::BuildGroup::GetCurrentPanel()
{
	NOT_IMPLEMENTED;

	return nullptr;
}

void vgui2::BuildGroup::LoadControlSettings(const char * controlResourceName, const char * pathID)
{
	NOT_IMPLEMENTED;

}

bool vgui2::BuildGroup::SaveControlSettings()
{
	NOT_IMPLEMENTED;

	return false;
}

void vgui2::BuildGroup::ApplySettings(KeyValues * resourceData)
{
	NOT_IMPLEMENTED;

}

void vgui2::BuildGroup::GetSettings(KeyValues * resourceData)
{
	NOT_IMPLEMENTED;

}

void vgui2::BuildGroup::RemoveSettings()
{
	NOT_IMPLEMENTED;

}

void vgui2::BuildGroup::SetContextPanel(vgui2::Panel * contextPanel)
{
	NOT_IMPLEMENTED;

}

vgui2::Panel * vgui2::BuildGroup::GetContextPanel()
{
	NOT_IMPLEMENTED;

	return nullptr;
}

const char * vgui2::BuildGroup::GetResourceName()
{
	NOT_IMPLEMENTED;

	return nullptr;
}

void vgui2::BuildGroup::PanelAdded(vgui2::Panel * panel)
{
	NOT_IMPLEMENTED;

}

void vgui2::BuildGroup::MousePressed(vgui2::MouseCode code, vgui2::Panel * panel)
{
	NOT_IMPLEMENTED;

}

void vgui2::BuildGroup::MouseReleased(vgui2::MouseCode code, vgui2::Panel * panel)
{
	NOT_IMPLEMENTED;

}

CUtlVector<vgui2::PHandle>* vgui2::BuildGroup::GetControlGroup()
{
	NOT_IMPLEMENTED;
	return nullptr;
}

CUtlVector<vgui2::PHandle>* GetControlGroup()
{
	NOT_IMPLEMENTED;

	return nullptr;
}

void vgui2::BuildGroup::ToggleRulerDisplay()
{
	NOT_IMPLEMENTED;

}

void vgui2::BuildGroup::SetRulerLabelsVisible(bool state)
{
	NOT_IMPLEMENTED;

}

bool vgui2::BuildGroup::HasRulersOn()
{
	NOT_IMPLEMENTED;

	return false;
}

void vgui2::BuildGroup::DrawRulers()
{
	NOT_IMPLEMENTED;

}

void vgui2::BuildGroup::CursorMoved(int x, int y, vgui2::Panel * panel)
{
	NOT_IMPLEMENTED;

}

void vgui2::BuildGroup::MouseDoublePressed(vgui2::MouseCode code, vgui2::Panel * panel)
{
	NOT_IMPLEMENTED;

}

void vgui2::BuildGroup::KeyCodeTyped(vgui2::KeyCode code, vgui2::Panel * panel)
{
	NOT_IMPLEMENTED;

}

void vgui2::BuildGroup::ApplySchemeSettings(vgui2::IScheme * pScheme)
{
	NOT_IMPLEMENTED;

}

vgui2::HCursor vgui2::BuildGroup::GetCursor(vgui2::Panel * panel)
{
	NOT_IMPLEMENTED;

	return HCursor();
}
