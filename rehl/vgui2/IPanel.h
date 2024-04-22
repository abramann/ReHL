#pragma once

#include "IClientPanel.h"
#include "VGUI.h"

namespace vgui2
{
	class Panel;
	class SurfacePlat;

	class IPanel : IBaseInterface
	{
	public:

		virtual ~IPanel();

		virtual void Init(vgui2::VPANEL vguiPanel, vgui2::IClientPanel *panel) = 0;
		virtual void SetPos(vgui2::VPANEL vguiPanel, int x, int y) = 0;
		virtual void GetPos(vgui2::VPANEL vguiPanel, int& x, int& y) = 0;
		virtual void SetSize(vgui2::VPANEL vguiPanel, int wide, int tall) = 0;
		virtual void GetSize(vgui2::VPANEL vguiPanel, int& wide, int& tall) = 0;
		virtual void SetMinimumSize(vgui2::VPANEL vguiPanel, int wide, int tall) = 0;
		virtual void GetMinimumSize(vgui2::VPANEL vguiPanel, int& wide, int& tall) = 0;
		virtual void SetZPos(vgui2::VPANEL vguiPanel, int z) = 0;
		virtual int GetZPos(vgui2::VPANEL vguiPanel) = 0;
		virtual void GetAbsPos(vgui2::VPANEL vguiPanel, int& x, int& y) = 0;
		virtual void GetClipRect(vgui2::VPANEL vguiPanel, int& x0, int& y0, int& x1, int& y1) = 0;
		virtual void SetInset(vgui2::VPANEL vguiPanel, int left, int top, int right, int bottom) = 0;
		virtual void GetInset(vgui2::VPANEL vguiPanel, int& left, int& top, int& right, int& bottom) = 0;
		virtual void SetVisible(vgui2::VPANEL vguiPanel, bool state) = 0;
		virtual bool IsVisible(vgui2::VPANEL vguiPanel) = 0;
		virtual void SetParent(vgui2::VPANEL vguiPanel, vgui2::VPANEL newParent) = 0;
		virtual int GetChildCount(vgui2::VPANEL vguiPanel) = 0;
		virtual vgui2::VPANEL GetChild(vgui2::VPANEL vguiPanel, int index) = 0;
		virtual vgui2::VPANEL GetParent(vgui2::VPANEL vguiPanel) = 0;
		virtual void MoveToFront(vgui2::VPANEL vguiPanel) = 0;
		virtual void MoveToBack(vgui2::VPANEL vguiPanel) = 0;
		virtual bool HasParent(vgui2::VPANEL vguiPanel, vgui2::VPANEL potentialParent) = 0;
		virtual bool IsPopup(vgui2::VPANEL vguiPanel) = 0;
		virtual void SetPopup(vgui2::VPANEL vguiPanel, bool state) = 0;
		virtual bool Render_GetPopupVisible(vgui2::VPANEL vguiPanel) = 0;
		virtual void Render_SetPopupVisible(vgui2::VPANEL vguiPanel, bool state) = 0;
		virtual vgui2::HScheme GetScheme(vgui2::VPANEL vguiPanel) = 0;
		virtual bool IsProportional(vgui2::VPANEL vguiPanel) = 0;
		virtual bool IsAutoDeleteSet(vgui2::VPANEL vguiPanel) = 0;
		virtual void DeletePanel(vgui2::VPANEL vguiPanel) = 0;
		virtual void SetKeyBoardInputEnabled(vgui2::VPANEL vguiPanel, bool state) = 0;
		virtual void SetMouseInputEnabled(vgui2::VPANEL vguiPanel, bool state) = 0;
		virtual bool IsKeyBoardInputEnabled(vgui2::VPANEL vguiPanel) = 0;
		virtual bool IsMouseInputEnabled(vgui2::VPANEL vguiPanel) = 0;
		virtual void Solve(vgui2::VPANEL vguiPanel) = 0;
		virtual const char * GetName(vgui2::VPANEL vguiPanel) = 0;
		virtual const char * GetClassName(vgui2::VPANEL vguiPanel) = 0;
		virtual void SendMessage(vgui2::VPANEL vguiPanel, KeyValues *params, vgui2::VPANEL ifrompanel) = 0;
		virtual void Think(vgui2::VPANEL vguiPanel) = 0;
		virtual void PerformApplySchemeSettings(vgui2::VPANEL vguiPanel) = 0;
		virtual void PaintTraverse(vgui2::VPANEL vguiPanel, bool forceRepaint, bool allowForce) = 0;
		virtual void Repaint(vgui2::VPANEL vguiPanel) = 0;
		virtual vgui2::VPANEL IsWithinTraverse(vgui2::VPANEL vguiPanel, int x, int y, bool traversePopups) = 0;
		virtual void OnChildAdded(vgui2::VPANEL vguiPanel, vgui2::VPANEL child) = 0;
		virtual void OnSizeChanged(vgui2::VPANEL vguiPanel, int newWide, int newTall) = 0;
		virtual void InternalFocusChanged(vgui2::VPANEL vguiPanel, bool lost) = 0;
		virtual bool RequestInfo(vgui2::VPANEL vguiPanel, KeyValues *outputData) = 0;
		virtual void RequestFocus(vgui2::VPANEL vguiPanel, int direction) = 0;
		virtual bool RequestFocusPrev(vgui2::VPANEL vguiPanel, vgui2::VPANEL existingPanel) = 0;
		virtual bool RequestFocusNext(vgui2::VPANEL vguiPanel, vgui2::VPANEL existingPanel) = 0;
		virtual vgui2::VPANEL GetCurrentKeyFocus(vgui2::VPANEL vguiPanel) = 0;
		virtual int GetTabPosition(vgui2::VPANEL vguiPanel) = 0;
		virtual vgui2::SurfacePlat *Plat(vgui2::VPANEL vguiPanel) = 0;
		virtual void SetPlat(vgui2::VPANEL vguiPanel, vgui2::SurfacePlat *Plat) = 0;
		virtual vgui2::Panel *GetPanel(vgui2::VPANEL vguiPanel, const char *moduleName) = 0;
		virtual bool IsEnabled(vgui2::VPANEL vguiPanel) = 0;
		virtual void SetEnabled(vgui2::VPANEL vguiPanel, bool state) = 0;
		virtual vgui2::IClientPanel *Client(vgui2::VPANEL vguiPanel) = 0;
		virtual const char *GetModuleName(vgui2::VPANEL vguiPanel) = 0;
	};
	IPanel *ipanel();
}

#define VGUI_PANEL_INTERFACE_VERSION "VGUI_Panel007"
