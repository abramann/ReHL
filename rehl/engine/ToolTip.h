#pragma once

#include "utlvector.h"

namespace vgui2
{
	class Panel;
	class IScheme;

	class  Tooltip
	{
	public:
		Tooltip(vgui2::Panel *parent, const char *text);
		
		virtual ~Tooltip();

		void ApplySchemeSettings(vgui2::IScheme *pScheme);
		const char *GetText();
		int GetTooltipDelay();
		void HideTooltip();
		void InvalidateLayout();
		void PerformLayout();
		void ResetDelay();
		void SetText(const char *text);
		void SetTooltipDelay(int tooltipDelay);
		void SetTooltipFormatToMultiLine();
		void SetTooltipFormatToSingleLine();
		void ShowTooltip(vgui2::Panel *currentPanel);
		void SizeTextWindow();

	private:
		CUtlVector<char> m_Text;
		int _delay;
		int _tooltipDelay;
		bool _makeVisible;
		bool _displayOnOneLine;
		bool m_bInvalidLayout;
	};
}