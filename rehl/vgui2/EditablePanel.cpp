#include "precompiled.h"
#include "EditablePanel.h"
#include "MessageMap.h"
#include "KeyValues.h"
#include "vgui2\IPanel.h"
#include "MessageMap.h"

bool vgui2::EditablePanel::Unknown::chained = false;
bool vgui2::EditablePanel::Unknown::OnDefaultButtonSet_InitVar_bAdded = false;
bool vgui2::EditablePanel::Unknown::OnCurrentDefaultButtonSet_InitVar_bAdded = false;
bool vgui2::EditablePanel::Unknown::OnFindDefaultButton_InitVar_bAdded = false;


vgui2::EditablePanel::EditablePanel(Panel *parent, const char *panelName) : Panel(parent, panelName), m_NavGroup(this)
{
	vgui::MessageMapItem_t src;

	if (!Unknown::chained)
	{
		Unknown::chained = true;
		vgui::PanelMessageMap* pMsgMap = vgui::FindOrAddPanelMessageMap("EditablePanel");
		pMsgMap->pfnClassName = vgui2::EditablePanel::GetPanelClassName;
		pMsgMap->baseMap = vgui::FindOrAddPanelMessageMap("Panel");
	}
	if (!Unknown::OnDefaultButtonSet_InitVar_bAdded)
	{
		Unknown::OnDefaultButtonSet_InitVar_bAdded = true;
		vgui::PanelMessageMap* pMsgMap = vgui::FindOrAddPanelMessageMap("EditablePanel");
		memset(&src, 0, sizeof(vgui::MessageMapItem_t));
		src.name = "DefaultButtonSet";
		//src.func.__pfn = (void(*)(vgui2::Panel *))(elf_hash_bucket + 401);
		src.numParams = 1;
		src.firstParamType = vgui::DATATYPE_PTR;
		src.firstParamName = "button";

		pMsgMap->entries.InsertBefore(pMsgMap->entries.Size(), src);
	}
	if (!Unknown::OnCurrentDefaultButtonSet_InitVar_bAdded)
	{
		Unknown::OnCurrentDefaultButtonSet_InitVar_bAdded = 1;
		vgui::PanelMessageMap* pMsgMap = vgui::FindOrAddPanelMessageMap("EditablePanel");
		memset(&src, 0, sizeof(vgui::MessageMapItem_t));
		src.name = "CurrentDefaultButtonSet";
		//src.func.__pfn = (void(*)(vgui2::Panel *))(elf_hash_bucket + 405);
		src.numParams = 1;
		src.firstParamType = vgui::DATATYPE_PTR;
		src.firstParamName = "button";
		pMsgMap->entries.InsertBefore(pMsgMap->entries.Size(), src);
	}
	if (!Unknown::OnFindDefaultButton_InitVar_bAdded)
	{
		Unknown::OnFindDefaultButton_InitVar_bAdded = true;
		vgui::PanelMessageMap* pMsgMap = vgui::FindOrAddPanelMessageMap("EditablePanel");
		memset(&src, 0, sizeof(vgui::MessageMapItem_t));
		src.name = "FindDefaultButton";
		//src.func.__pfn = (void(*)(vgui2::Panel *))(elf_hash_bucket + 409);
		pMsgMap->entries.InsertBefore(pMsgMap->entries.Size(), src);
	}

	_buildGroup = new BuildGroup(this, this);
	m_pszConfigName = NULL;
	m_iConfigID = 0;
	m_pDialogVariables = NULL;
	GetSize(_baseTall, _baseWide);
}

//-----------------------------------------------------------------------------
// Purpose: Constructor
//-----------------------------------------------------------------------------
vgui2::EditablePanel::EditablePanel(Panel *parent, const char *panelName, HScheme hScheme) : Panel(parent, panelName, hScheme), m_NavGroup(this)
{
	vgui::MessageMapItem_t src;

	if (!Unknown::chained)
	{
		Unknown::chained = true;
		vgui::PanelMessageMap* pMsgMap = vgui::FindOrAddPanelMessageMap("EditablePanel");
		pMsgMap->pfnClassName = vgui2::EditablePanel::GetPanelClassName;
		pMsgMap->baseMap = vgui::FindOrAddPanelMessageMap("Panel");
	}

	if (!Unknown::OnDefaultButtonSet_InitVar_bAdded)
	{
		Unknown::OnDefaultButtonSet_InitVar_bAdded = true;
		vgui::PanelMessageMap* pMsgMap = vgui::FindOrAddPanelMessageMap("EditablePanel");
		memset(&src, 0, sizeof(vgui::MessageMapItem_t));
		src.name = "DefaultButtonSet";
		//src.func.__pfn = (void(*)(vgui2::Panel *))(elf_hash_bucket + 401);
		src.numParams = 1;
		src.firstParamType = vgui::DATATYPE_PTR;
		src.firstParamName = "button";

		pMsgMap->entries.InsertBefore(pMsgMap->entries.Size(), src);
	}

	if (!Unknown::OnCurrentDefaultButtonSet_InitVar_bAdded)
	{
		Unknown::OnCurrentDefaultButtonSet_InitVar_bAdded = 1;
		vgui::PanelMessageMap* pMsgMap = vgui::FindOrAddPanelMessageMap("EditablePanel");
		memset(&src, 0, sizeof(vgui::MessageMapItem_t));
		src.name = "CurrentDefaultButtonSet";
		//src.func.__pfn = (void(*)(vgui2::Panel *))(elf_hash_bucket + 405);
		src.numParams = 1;
		src.firstParamType = vgui::DATATYPE_PTR;
		src.firstParamName = "button";
		pMsgMap->entries.InsertBefore(pMsgMap->entries.Size(), src);
	}

	if (!Unknown::OnFindDefaultButton_InitVar_bAdded)
	{
		Unknown::OnFindDefaultButton_InitVar_bAdded = true;
		vgui::PanelMessageMap* pMsgMap = vgui::FindOrAddPanelMessageMap("EditablePanel");
		memset(&src, 0, sizeof(vgui::MessageMapItem_t));
		src.name = "FindDefaultButton";
		//src.func.__pfn = (void(*)(vgui2::Panel *))(elf_hash_bucket + 409);
		pMsgMap->entries.InsertBefore(pMsgMap->entries.Size(), src);
	}

	_buildGroup = new BuildGroup(this, this);
	m_pszConfigName = NULL;
	m_iConfigID = 0;
	m_pDialogVariables = NULL;
	GetSize(_baseTall, _baseWide);
}

//-----------------------------------------------------------------------------
// Purpose: Destructor
//-----------------------------------------------------------------------------
vgui2::EditablePanel::~EditablePanel()
{
	delete[] m_pszConfigName;
	delete _buildGroup;

	if (m_pDialogVariables)
	{
		m_pDialogVariables->deleteThis();
	}
}

void vgui2::EditablePanel::OnChildAdded(vgui2::VPANEL child)
{
	Panel::OnChildAdded(child);
	Panel* pChildPanel = ipanel()->GetPanel(child, GetModuleName());
	if (pChildPanel)
	{
		pChildPanel->SetBuildGroup(_buildGroup);
		pChildPanel->AddActionSignalTarget(this);
	}
}

void vgui2::EditablePanel::OnSizeChanged(int wide, int tall)
{
	Panel::OnSizeChanged(wide, tall);

	int dx = _baseWide;
	int dy = _baseTall;

	for (int i = 0; i < GetChildCount(); i++)
	{
		// perform auto-layout on the child panel
		Panel *child = GetChild(i);
		if (!child)
			continue;

		int x, y, w, t;
		child->GetBounds(x, y, w, t);

		int ex;
		int ey;
		// The correct version of this code would say:
		// if ( resize != AUTORESIZE_NO )
		// but we're very close to shipping and this causes artifacts in other vgui panels that now
		// depend on this bug.  So, I've added m_bShouldSkipAutoResize, which defaults to false but can
		// be set using "skip_autoresize" in a .res file

		auto pinCorner = child->GetPinCorner();
		if (pinCorner == PIN_TOPRIGHT || pinCorner == PIN_BOTTOMRIGHT)
			x += dx;
		if (pinCorner == PIN_BOTTOMLEFT || pinCorner == PIN_BOTTOMRIGHT)
			y += dy;

		auto autoResize = child->GetAutoResize();
		if (autoResize == AUTORESIZE_RIGHT || autoResize == AUTORESIZE_DOWNANDRIGHT)
		{
			w += dx;
		}
		if (autoResize == AUTORESIZE_DOWN || autoResize == AUTORESIZE_DOWNANDRIGHT)
		{
			t += dy;
		}
		if (wide >= x + w || (w = wide - x, wide - x >= 0))
		{
			int newTall = t;
			if (tall >= y + t || (newTall = tall - y, t = tall - y, tall - y >= 0))
			{
				child->SetBounds(x, y, w, newTall);
				child->InvalidateLayout(false, false);
			}
		}
	}

	Repaint();
	_baseWide = wide;
	_baseTall = tall;
}

bool vgui2::EditablePanel::RequestInfo(KeyValues * data)
{
	NOT_IMPLEMENTED;

	return false;
}

void vgui2::EditablePanel::RequestFocus(int direction)
{
	NOT_IMPLEMENTED;

}

bool vgui2::EditablePanel::RequestFocusPrev(vgui2::VPANEL panel)
{
	NOT_IMPLEMENTED;
	return false;
}

bool vgui2::EditablePanel::RequestFocusNext(vgui2::VPANEL panel)
{
	NOT_IMPLEMENTED;
	return false;
}

vgui2::VPANEL vgui2::EditablePanel::GetCurrentKeyFocus()
{
	NOT_IMPLEMENTED;
	return VPANEL();
}

vgui::PanelMessageMap * vgui2::EditablePanel::GetMessageMap()
{
	NOT_IMPLEMENTED;

	return nullptr;
}

vgui2::Panel * vgui2::EditablePanel::HasHotkey(wchar_t key)
{
	NOT_IMPLEMENTED;

	return nullptr;
}

void vgui2::EditablePanel::ApplySettings(KeyValues * inResourceData)
{
	NOT_IMPLEMENTED;

}

void vgui2::EditablePanel::ApplyUserConfigSettings(KeyValues * userConfig)
{
	NOT_IMPLEMENTED;

}

void vgui2::EditablePanel::GetUserConfigSettings(KeyValues * userConfig)
{
	NOT_IMPLEMENTED;

}

void vgui2::EditablePanel::OnKeyCodeTyped(vgui2::KeyCode code)
{
	NOT_IMPLEMENTED;

}

void vgui2::EditablePanel::PaintBackground()
{
	NOT_IMPLEMENTED;

}

void vgui2::EditablePanel::LoadControlSettings(const char * resourceName, const char * pathID)
{
	NOT_IMPLEMENTED;

}

void vgui2::EditablePanel::LoadUserConfig(const char * configName, int dialogID)
{
	NOT_IMPLEMENTED;

}

void vgui2::EditablePanel::SaveUserConfig()
{
	NOT_IMPLEMENTED;

}

void vgui2::EditablePanel::LoadControlSettingsAndUserConfig(const char * dialogResourceName, int dialogID)
{
	NOT_IMPLEMENTED;

}

void vgui2::EditablePanel::ActivateBuildMode()
{
	NOT_IMPLEMENTED;

}

vgui2::BuildGroup * vgui2::EditablePanel::GetBuildGroup()
{
	return _buildGroup;
}

vgui2::Panel * vgui2::EditablePanel::CreateControlByName(const char * controlName)
{
	NOT_IMPLEMENTED;

	return nullptr;
}

void vgui2::EditablePanel::SetControlString(const char * controlName, const char * string)
{
	NOT_IMPLEMENTED;

}

void vgui2::EditablePanel::SetControlInt(const char * controlName, int state)
{
	NOT_IMPLEMENTED;

}

int vgui2::EditablePanel::GetControlInt(const char * controlName, int defaultState)
{
	NOT_IMPLEMENTED;

	return 0;
}

const char * vgui2::EditablePanel::GetControlString(const char * controlName, const char * defaultString)
{
	NOT_IMPLEMENTED;

	return nullptr;
}

void vgui2::EditablePanel::GetControlString(const char * controlName, char * buf, int bufSize, const char * defaultString)
{
	NOT_IMPLEMENTED;

}

void vgui2::EditablePanel::SetControlEnabled(const char * controlName, bool enabled)
{
	NOT_IMPLEMENTED;

}

void vgui2::EditablePanel::SetControlVisible(const char * controlName, bool visible)
{
	NOT_IMPLEMENTED;

}

void vgui2::EditablePanel::SetDialogVariable(const char * varName, const char * value)
{
	NOT_IMPLEMENTED;

}

void vgui2::EditablePanel::SetDialogVariable(const char * varName, const wchar_t * value)
{
	NOT_IMPLEMENTED;

}

void vgui2::EditablePanel::SetDialogVariable(const char * varName, int value)
{
	NOT_IMPLEMENTED;

}

void vgui2::EditablePanel::SetDialogVariable(const char * varName, float value)
{
	NOT_IMPLEMENTED;

}

void vgui2::EditablePanel::RegisterControlSettingsFile(const char * resourceName, const char * pathID)
{
	NOT_IMPLEMENTED;

}

vgui2::FocusNavGroup * vgui2::EditablePanel::GetFocusNavGroup()
{
	NOT_IMPLEMENTED;

	return nullptr;
}

void vgui2::EditablePanel::OnDefaultButtonSet(vgui2::Panel * defaultButton)
{
	NOT_IMPLEMENTED;

}

void vgui2::EditablePanel::OnCurrentDefaultButtonSet(vgui2::Panel * defaultButton)
{
	NOT_IMPLEMENTED;

}

void vgui2::EditablePanel::OnFindDefaultButton()
{
	NOT_IMPLEMENTED;

}

void vgui2::EditablePanel::OnClose()
{
	NOT_IMPLEMENTED;

}

const char * vgui2::EditablePanel::GetPanelClassName()
{
	return "Panel";
}

