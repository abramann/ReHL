#pragma once

#include "vgui2/VGui.h"
#include "vgui2/MouseCode.h"
#include "vgui2/KeyCode.h"
#include "utlvector.h"
#include "utlsymbol.h"

class KeyValues;

namespace vgui2
{
	class Panel;
	class Label;
	class IScheme;

	class BuildGroup
	{
	public:
		BuildGroup(vgui2::Panel *parentPanel, vgui2::Panel *contextPanel);

		virtual void SetEnabled(bool state);
		virtual bool IsEnabled();
		virtual Panel *GetCurrentPanel();
		virtual void LoadControlSettings(const char *controlResourceName, const char *pathID);
		virtual bool SaveControlSettings();
		virtual void ApplySettings(KeyValues *resourceData);
		virtual void GetSettings(KeyValues *resourceData);
		virtual void RemoveSettings();
		virtual void SetContextPanel(vgui2::Panel *contextPanel);
		virtual Panel *GetContextPanel();
		virtual const char *GetResourceName();
		virtual void PanelAdded(vgui2::Panel *panel);
		virtual void MousePressed(vgui2::MouseCode code, vgui2::Panel *panel);
		virtual void MouseReleased(vgui2::MouseCode code, vgui2::Panel *panel);
		virtual CUtlVector<vgui2::PHandle> *GetControlGroup();
		virtual void ToggleRulerDisplay();
		virtual void SetRulerLabelsVisible(bool state);
		virtual bool HasRulersOn();
		virtual void DrawRulers();
		virtual void CursorMoved(int x, int y, vgui2::Panel *panel);
		virtual void MouseDoublePressed(vgui2::MouseCode code, vgui2::Panel *panel);
		virtual void KeyCodeTyped(vgui2::KeyCode code, vgui2::Panel *panel);
		virtual void ApplySchemeSettings(vgui2::IScheme *pScheme);
		virtual HCursor GetCursor(vgui2::Panel *panel);

	private:
		bool _enabled;
		int _snapX;
		int _snapY;
		vgui2::HCursor _cursor_sizenwse;
		vgui2::HCursor _cursor_sizenesw;
		vgui2::HCursor _cursor_sizewe;
		vgui2::HCursor _cursor_sizens;
		vgui2::HCursor _cursor_sizeall;
		bool _dragging;
		vgui2::MouseCode _dragMouseCode;
		int _dragStartPanelPos[2];
		int _dragStartCursorPos[2];
		vgui2::Panel *_currentPanel;
		CUtlVector<PHandle> _panelDar;
		char *m_pResourceName;
		char *m_pResourcePathID;
		PHandle m_hBuildDialog;
		vgui2::Panel *m_pBuildContext;
		vgui2::Panel *m_pParentPanel;
		CUtlVector<vgui2::PHandle> _controlGroup;
		CUtlVector<int> _groupDeltaX;
		CUtlVector<int> _groupDeltaY;
		vgui2::Label *_rulerNumber[4];
		bool _showRulers;
		CUtlVector<CUtlSymbol> m_RegisteredControlSettingsFiles;
	};
}