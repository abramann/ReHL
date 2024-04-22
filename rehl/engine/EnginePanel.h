#pragma once

#include "vgui2\EditablePanel.h"


class CEnginePanel : public vgui2::EditablePanel
{
public:
	using BaseClass = vgui2::EditablePanel;

	using BaseClass::BaseClass;

	vgui2::VPANEL IsWithinTraverse(int x, int y, bool traversePopups) override
	{
		vgui2::VPANEL panel = NULL_HANDLE;

		if (m_bCanFocus)
		{
			panel = BaseClass::IsWithinTraverse(x, y, traversePopups);

			if (panel == GetVPanel())
				panel = NULL_HANDLE;
		}

		return panel;
	}

public:
	bool m_bCanFocus = true;
};

class CStaticPanel : public vgui2::Panel
{
public:
	using BaseClass = vgui2::Panel;

	using BaseClass::BaseClass;

	vgui2::VPANEL GetCurrentKeyFocus() override;
	vgui2::VPANEL IsWithinTraverse(int x, int y, bool traversePopups) override;
};