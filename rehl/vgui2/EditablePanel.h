#pragma once

#include "Panel.h"
#include "FocusNavGroup.h"

namespace vgui2
{
	class EditablePanel : public Panel
	{
	public:
		EditablePanel(vgui2::Panel *parent, const char *panelName);
		EditablePanel(Panel *parent, const char *panelName, vgui2::HScheme hScheme);
		~EditablePanel();

		struct Unknown
		{
			static bool chained;
			static bool OnDefaultButtonSet_InitVar_bAdded;
			static bool OnCurrentDefaultButtonSet_InitVar_bAdded;
			static bool OnFindDefaultButton_InitVar_bAdded;
		};

		virtual void OnChildAdded(vgui2::VPANEL child);
		virtual void OnSizeChanged(int wide, int tall);
		virtual bool RequestInfo(KeyValues *data);
		virtual void RequestFocus(int direction);
		virtual bool RequestFocusPrev(vgui2::VPANEL panel);
		virtual bool RequestFocusNext(vgui2::VPANEL panel);
		virtual VPANEL GetCurrentKeyFocus();
		virtual vgui::PanelMessageMap *GetMessageMap();

		virtual vgui2::Panel *HasHotkey(wchar_t key);
		virtual void ApplySettings(KeyValues *inResourceData);
		virtual void ApplyUserConfigSettings(KeyValues *userConfig);
		virtual void GetUserConfigSettings(KeyValues *userConfig);
		virtual void OnKeyCodeTyped(vgui2::KeyCode code);
		virtual void PaintBackground();
		virtual void LoadControlSettings(const char *resourceName, const char *pathID);
		virtual void LoadUserConfig(const char *configName, int dialogID);
		virtual void SaveUserConfig();
		virtual void LoadControlSettingsAndUserConfig(const char *dialogResourceName, int dialogID);
		virtual void ActivateBuildMode();
		virtual BuildGroup *GetBuildGroup();
		virtual Panel *CreateControlByName(const char *controlName);
		virtual void SetControlString(const char *controlName, const char *string);
		virtual void SetControlInt(const char *controlName, int state);
		virtual int GetControlInt(const char *controlName, int defaultState);
		virtual const char * GetControlString(const char *controlName, const char *defaultString);
		virtual void GetControlString(const char *controlName, char *buf, int bufSize, const char *defaultString);
		virtual void SetControlEnabled(const char *controlName, bool enabled);
		virtual void SetControlVisible(const char *controlName, bool visible);
		virtual void SetDialogVariable(const char *varName, const char *value);
		virtual void SetDialogVariable(const char *varName, const wchar_t *value);
		virtual void SetDialogVariable(const char *varName, int value);
		virtual void SetDialogVariable(const char *varName, float value);
		virtual void RegisterControlSettingsFile(const char *resourceName, const char *pathID);
		virtual FocusNavGroup *GetFocusNavGroup();
		virtual void OnDefaultButtonSet(vgui2::Panel *defaultButton);
		virtual void OnCurrentDefaultButtonSet(vgui2::Panel *defaultButton);
		virtual void OnFindDefaultButton();
		virtual void OnClose();
		
		static const char *GetPanelClassName();

		static void ChainToMap(void);

		struct PanelMessageFunc_OnDefaultButtonSet
		{
			static void InitVar(void);
		};

		struct PanelMessageFunc_OnCurrentDefaultButtonSet
		{
			static void InitVar(void);
		};

		struct PanelMessageFunc_OnFindDefaultButton
		{
			static void InitVar(void);
		};

	private:
		typedef void(*EditablePanel_RegisterMap)();

		EditablePanel_RegisterMap m_RegisterClass;
		PanelMessageFunc_OnDefaultButtonSet m_OnDefaultButtonSet_register;
		PanelMessageFunc_OnCurrentDefaultButtonSet m_OnCurrentDefaultButtonSet_register;
		PanelMessageFunc_OnFindDefaultButton m_OnFindDefaultButton_register;

		vgui2::BuildGroup *_buildGroup;
		vgui2::FocusNavGroup m_NavGroup;
		KeyValues *m_pDialogVariables;
		int _baseWide;
		int _baseTall;
		char *m_pszConfigName;
		int m_iConfigID;
	};
}
