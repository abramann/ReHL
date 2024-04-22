#pragma once

#include "IClientPanel.h"
#include "VGui.h"
#include "KeyCode.h"
#include "MouseCode.h"
#include "Color.h"
#include "FakeVGUI\Dar.h"
#include "MessageMap.h"
#include "BuildGroup.h"

namespace vgui2
{
	class IBorder;
	class IScheme;
	class Tooltip;
	class Panel;
	class BaseTooltip;

	class Panel : public IClientPanel
	{
		enum
		{
			BUILDMODE_EDITABLE = 0x1,
			BUILDMODE_DELETABLE = 0x2,
			BUILDMODE_SAVE_XPOS_RIGHTALIGNED = 0x4,
			BUILDMODE_SAVE_XPOS_CENTERALIGNED = 0x8,
			BUILDMODE_SAVE_YPOS_BOTTOMALIGNED = 0x10,
			BUILDMODE_SAVE_YPOS_CENTERALIGNED = 0x20,
		};

	public:

		enum PinCorner_e
		{
			PIN_TOPLEFT = 0x0,
			PIN_TOPRIGHT = 0x1,
			PIN_BOTTOMLEFT = 0x2,
			PIN_BOTTOMRIGHT = 0x3,
		};
		enum AutoResize_e
		{
			AUTORESIZE_NO = 0x0,
			AUTORESIZE_RIGHT = 0x1,
			AUTORESIZE_DOWN = 0x2,
			AUTORESIZE_DOWNANDRIGHT = 0x3,
		};

		Panel();
		Panel(Panel* parent);
		Panel(Panel* parent, const char* panelName);
		Panel(Panel* parent, const char* panelName, vgui2::HScheme scheme);

		virtual ~Panel();

		virtual VPANEL GetVPanel();
		virtual void Think();
		virtual void PerformApplySchemeSettings();
		virtual void PaintTraverse(bool forceRepaint, bool allowForce);
		virtual void Repaint();
		virtual VPANEL IsWithinTraverse(int x, int y, bool traversePopups);
		virtual void GetInset(int &top, int &left, int &right, int &bottom);
		virtual void GetClipRect(int &x0, int &y0, int &x1, int &y1);
		virtual void OnChildAdded(VPANEL child);
		virtual void OnSizeChanged(int newWide, int newTall);
		virtual void InternalFocusChanged(bool lost);
		virtual bool RequestInfo(KeyValues *outputData);
		virtual void RequestFocus(int direction);
		virtual bool RequestFocusPrev(VPANEL existingPanel);
		virtual bool RequestFocusNext(VPANEL existingPanel);
		virtual void OnMessage(const KeyValues *params, VPANEL ifromPanel);
		virtual VPANEL GetCurrentKeyFocus();
		virtual int GetTabPosition();
		virtual const char *GetName();
		virtual const char *GetClassName();
		virtual HScheme GetScheme();
		virtual bool IsProportional();
		virtual bool IsAutoDeleteSet();
		virtual void DeletePanel();
		virtual void *QueryInterface(EInterfaceID id);
		virtual Panel *GetPanel();
		virtual const char *GetModuleName();
		virtual vgui::PanelMessageMap * GetMessageMap();

		virtual void SetVisible(bool state);
		virtual bool IsVisible();
		virtual void PostMessage(vgui2::VPANEL target, KeyValues *message, float delaySeconds);
		virtual void OnMove();
		virtual vgui2::Panel *GetParent();
		virtual vgui2::VPANEL GetVParent();
		virtual void SetParent(vgui2::Panel *newParent);
		virtual void SetParent(vgui2::VPANEL newParent);
		virtual bool HasParent(vgui2::VPANEL potentialParent);
		virtual void SetAutoDelete(bool state);
		virtual void AddActionSignalTarget(vgui2::Panel *messageTarget);
		virtual void AddActionSignalTarget(vgui2::VPANEL messageTarget);
		virtual void RemoveActionSignalTarget(vgui2::Panel *oldTarget);
		virtual void PostActionSignal(KeyValues *message);
		virtual bool RequestInfoFromChild(const char *childName, KeyValues *outputData);
		virtual void PostMessageToChild(const char *childName, KeyValues *message);
		virtual void PostMessage(vgui2::Panel *target, KeyValues *message, float delay);
		virtual bool SetInfo(KeyValues *inputData);
		virtual void SetEnabled(bool state);
		virtual bool IsEnabled();
		virtual bool IsPopup();
		virtual void MoveToFront();
		virtual void SetBgColor(Color color);
		virtual void SetFgColor(Color color);
		virtual Color& GetBgColor(Color& color);
		virtual Color& GetFgColor(Color& color);
		virtual void SetCursor(vgui2::HCursor cursor);
		virtual vgui2::HCursor GetCursor();
		virtual bool HasFocus();
		virtual void InvalidateLayout(bool layoutNow, bool reloadScheme);
		virtual void SetTabPosition(int position);
		virtual void SetBorder(vgui2::IBorder *border);
		virtual vgui2::IBorder *GetBorder();
		virtual void SetPaintBorderEnabled(bool state);
		virtual void SetPaintBackgroundEnabled(bool state);
		virtual void SetPaintEnabled(bool state);
		virtual void SetPostChildPaintEnabled(bool state);
		virtual void GetPaintSize(int& wide, int& tall);
		virtual void SetBuildGroup(vgui2::BuildGroup *buildGroup);
		virtual bool IsBuildGroupEnabled();
		virtual bool IsCursorNone();
		virtual bool IsCursorOver();
		virtual void MarkForDeletion();
		virtual bool IsLayoutInvalid();
		virtual vgui2::Panel *HasHotkey(wchar_t key);
		virtual bool IsOpaque();
		virtual void SetScheme(const char *tag);
		virtual void SetScheme(vgui2::HScheme scheme);
		virtual Color GetSchemeColor(const char *keyName, vgui2::IScheme *pScheme);
		virtual Color GetSchemeColor(const char *keyName, Color defaultColor, vgui2::IScheme *pScheme);
		virtual void ApplySchemeSettings(vgui2::IScheme *pScheme);
		virtual void ApplySettings(KeyValues *inResourceData);
		virtual void GetSettings(KeyValues *outResourceData);
		virtual const char *GetDescription();
		virtual void ApplyUserConfigSettings(KeyValues *userConfig);
		virtual void GetUserConfigSettings(KeyValues *userConfig);
		virtual bool HasUserConfigSettings();
		virtual void OnThink();
		virtual void OnCommand(const char *command);
		virtual void OnMouseCaptureLost();
		virtual void OnSetFocus();
		virtual void OnKillFocus();
		virtual void OnDelete();
		virtual void OnTick();
		virtual void OnCursorMoved(int x, int y);
		virtual void OnCursorEntered();
		virtual void OnCursorExited();
		virtual void OnMousePressed(vgui2::MouseCode code);
		virtual void OnMouseDoublePressed(vgui2::MouseCode code);
		virtual void OnMouseReleased(vgui2::MouseCode code);
		virtual void OnMouseWheeled(int delta);
		virtual void OnKeyCodePressed(vgui2::KeyCode code);
		virtual void OnKeyCodeTyped(vgui2::KeyCode code);
		virtual void OnKeyTyped(wchar_t unichar);
		virtual void OnKeyCodeReleased(vgui2::KeyCode code);
		virtual void OnKeyFocusTicked();
		virtual void OnMouseFocusTicked();
		virtual void PaintBackground();
		virtual void Paint();
		virtual void PaintBorder();
		virtual void PaintBuildOverlay();
		virtual void PostChildPaint();
		virtual void PerformLayout();
		virtual vgui::PanelMap_t *GetPanelMap();
		virtual void SetProportional(bool state);
		virtual void SetMouseInputEnabled(bool state);
		virtual void SetKeyBoardInputEnabled(bool state);
		virtual bool IsMouseInputEnabled();
		virtual bool IsKeyBoardInputEnabled();
		virtual void OnRequestFocus(vgui2::VPANEL subFocus, vgui2::VPANEL defaultPanel);
		virtual void InternalCursorMoved(int x, int y);
		virtual void InternalCursorEntered();
		virtual void InternalCursorExited();
		virtual void InternalMousePressed(int code);
		virtual void InternalMouseDoublePressed(int code);
		virtual void InternalMouseReleased(int code);
		virtual void InternalMouseWheeled(int delta);
		virtual void InternalKeyCodePressed(int code);
		virtual void InternalKeyCodeTyped(int code);
		virtual void InternalKeyTyped(int unichar);
		virtual void InternalKeyCodeReleased(int code);
		virtual void InternalKeyFocusTicked();
		virtual void InternalMouseFocusTicked();
		virtual void InternalInvalidateLayout();
		virtual void InternalMove();

		void Init(int x, int y, int wide, int tall);

		void MakeReadyForUse();

		void SetName(const char *panelName);

		void SetBuildModeEditable(bool state);

		void SetPos(int x, int y);

		void GetPos(int &x, int &y);

		void SetSize(int wide, int tall);

		void GetSize(int &wide, int &tall);

		void SetBounds(int x, int y, int wide, int tall);

		void GetBounds(int &x, int &y, int &wide, int &tall);

		void SetZPos(int z);

		int GetZPos();

		Panel *FindChildByName(const char *childName, bool recurseDown);

		int GetChildCount();

		Panel *GetChild(int index);

		Panel *FindSiblingByName(const char *siblingName);

		void CallParentFunction(KeyValues *message);

		void InternalSetCursor();

		bool IsBuildModeActive();

		void SetBuildModeDeletable(bool state);

		bool IsBuildModeDeletable();

		bool IsBuildModeEditable();

		void GetMinimumSize(int& wide, int &tall);

		void SetMinimumSize(int wide, int tall);

		void MakePopup(bool showTaskbarIcon, bool disabled);

		void ScreenToLocal(int& x, int& y);

		void LocalToScreen(int& x, int& y);

		int GetWide();

		void SetWide(int wide);

		void InternalPerformLayout();

		void SetAutoResize(AutoResize_e resizeDir);

		void SetPinCorner(PinCorner_e pinCorner);

		AutoResize_e GetAutoResize();

		PinCorner_e GetPinCorner();

		bool IsWithin(int x, int y);

		void ParentLocalToScreen(int &x, int &y);

		int GetTall();

		void SetTall(int tall);

		void OnOldMessage(KeyValues *params, VPANEL ifromPanel);

		void PreparePanelMap(vgui::PanelMap_t *panelMap);

		BaseTooltip *vgui2::Panel::GetTooltip();

		void SetTooltip(BaseTooltip *pToolTip, const char *pszText);
protected:

		typedef void(*Panel_RegisterMap)(void);
		typedef void(*PanelMessageFunc_Repaint)(void);
		typedef void(*PanelMessageFunc_OnCommand)(const char * command);
		typedef void(*PanelMessageFunc_OnMouseCaptureLost)(void);
		typedef void(*PanelMessageFunc_OnSetFocus)(void);
		typedef void(*PanelMessageFunc_OnKillFocus)(void);
		typedef void(*PanelMessageFunc_OnDelete)(void);
		typedef void(*PanelMessageFunc_OnTick)(void);
		typedef void(*PanelMessageFunc_OnCursorMoved)(int x, int y);
		typedef void(*PanelMessageFunc_OnMouseFocusTicked)(void);
		typedef void(*PanelMessageFunc_OnRequestFocus)(vgui2::VPANEL subFocus, vgui2::VPANEL defaultPanel);
		typedef void(*PanelMessageFunc_InternalCursorMoved)(int x, int y);
		typedef void(*PanelMessageFunc_InternalCursorEntered)(void);
		typedef void(*PanelMessageFunc_InternalCursorExited)(void);
		typedef void(*PanelMessageFunc_InternalMousePressed)(int code);
		typedef void(*PanelMessageFunc_InternalMouseDoublePressed)(int code);
		typedef void(*PanelMessageFunc_InternalMouseReleased)(int code);
		typedef void(*PanelMessageFunc_InternalMouseWheeled)(int delta);
		typedef void(*PanelMessageFunc_InternalKeyCodePressed)(int code);
		typedef void(*PanelMessageFunc_InternalKeyCodeTyped)(int code);
		typedef void(*PanelMessageFunc_InternalKeyTyped)(int code);
		typedef void(*PanelMessageFunc_InternalKeyCodeReleased)(int unichar);
		typedef void(*PanelMessageFunc_InternalKeyFocusTicked)(void);
		typedef void(*PanelMessageFunc_InternalMouseFocusTicked)(void);
		typedef void(*PanelMessageFunc_InternalInvalidateLayout)(void);
		typedef void(*PanelMessageFunc_InternalMove)(void);

		typedef void(*PanelMessageFunc_OnDefaultButtonSet)(Panel* defaultButton);
		typedef void(*PanelMessageFunc_OnCurrentDefaultButtonSet)(Panel* defaultButton);
		typedef void(*PanelMessageFunc_OnFindDefaultButton)();

	private:
		vgui2::Panel::Panel_RegisterMap m_RegisterClass;
		PanelMessageFunc_Repaint m_Repaint_register;
		PanelMessageFunc_OnCommand m_OnCommand_register;
		PanelMessageFunc_OnMouseCaptureLost m_OnMouseCaptureLost_register;
		PanelMessageFunc_OnSetFocus m_OnSetFocus_register;
		PanelMessageFunc_OnKillFocus m_OnKillFocus_register;
		PanelMessageFunc_OnDelete m_OnDelete_register;
		PanelMessageFunc_OnTick m_OnTick_register;
		PanelMessageFunc_OnCursorMoved m_OnCursorMoved_register;
		PanelMessageFunc_OnMouseFocusTicked m_OnMouseFocusTicked_register;
		PanelMessageFunc_OnRequestFocus m_OnRequestFocus_register;
		PanelMessageFunc_InternalCursorMoved m_InternalCursorMoved_register;
		PanelMessageFunc_InternalCursorEntered m_InternalCursorEntered_register;
		PanelMessageFunc_InternalCursorExited m_InternalCursorExited_register;
		PanelMessageFunc_InternalMousePressed m_InternalMousePressed_register;
		PanelMessageFunc_InternalMouseDoublePressed m_InternalMouseDoublePressed_register;
		PanelMessageFunc_InternalMouseReleased m_InternalMouseReleased_register;
		PanelMessageFunc_InternalMouseWheeled m_InternalMouseWheeled_register;
		PanelMessageFunc_InternalKeyCodePressed m_InternalKeyCodePressed_register;
		PanelMessageFunc_InternalKeyCodeTyped m_InternalKeyCodeTyped_register;
		PanelMessageFunc_InternalKeyTyped m_InternalKeyTyped_register;
		PanelMessageFunc_InternalKeyCodeReleased m_InternalKeyCodeReleased_register;
		PanelMessageFunc_InternalKeyFocusTicked m_InternalKeyFocusTicked_register;
		PanelMessageFunc_InternalMouseFocusTicked m_InternalMouseFocusTicked_register;
		PanelMessageFunc_InternalInvalidateLayout m_InternalInvalidateLayout_register;
		PanelMessageFunc_InternalMove m_InternalMove_register;

		VPANEL _vpanel;
		HCursor _cursor;
		bool _markedForDeletion;
		IBorder *_border;
		bool _needsRepaint;
		BuildGroup *_buildGroup;
		Color _fgColor;
		Color _bgColor;
		bool _paintBorderEnabled;
		bool _paintBackgroundEnabled;
		bool _paintEnabled;
		bool _postChildPaintEnabled;
		char *_panelName;
		bool _needsLayout;
		bool _needsSchemeUpdate;
		bool _autoDelete;
		int _tabPosition;
		vgui::Dar<long unsigned int> _actionSignalTargetDar;
		vgui2::Panel::PinCorner_e _pinCorner;
		vgui2::Panel::AutoResize_e _autoResizeDirection;
		unsigned int m_iScheme;
		unsigned __int8 _buildModeFlags;
		bool m_bProportional;
		bool m_bInPerformLayout;
		Tooltip *m_pTooltips;
	};
}

