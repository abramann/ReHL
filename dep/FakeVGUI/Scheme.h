#pragma once

#include "dll_api.h"


namespace vgui
{
	class Cursor;
	class Font;

	class VGUI_DLL_API Scheme
	{
	public:
		enum SchemeColor
		{
			sc_user,
			sc_black,
			sc_white,
			sc_primary1,
			sc_primary2,
			sc_primary3,
			sc_secondary1,
			sc_secondary2,
			sc_secondary3,
			sc_last
		};

		enum SchemeFont
		{
			sf_user,
			sf_primary1,
			sf_primary2,
			sf_primary3,
			sf_secondary1,
			sf_last
		};

		enum SchemeCursor
		{
			scu_user,
			scu_none,
			scu_arrow,
			scu_ibeam,
			scu_hourglass,
			scu_crosshair,
			scu_up,
			scu_sizenwse,
			scu_sizenesw,
			scu_sizewe,
			scu_sizens,
			scu_sizeall,
			scu_no,
			scu_hand,
			scu_last
		};
		virtual void setColor(vgui::Scheme::SchemeColor sc, int r, int g, int b, int a);
		virtual void getColor(vgui::Scheme::SchemeColor sc, int *const r, int *const g, int *const b, int *const a);
		virtual void setFont(vgui::Scheme::SchemeFont sf_0, vgui::Font *font);
		virtual vgui::Font *getFont(vgui::Scheme::SchemeFont sf_0);
		virtual void setCursor(vgui::Scheme::SchemeCursor scu, vgui::Cursor *cursor);
		virtual vgui::Cursor *getCursor(vgui::Scheme::SchemeCursor scu);

	private:
		int _color[9][4];
		vgui::Font *_font[5];
		vgui::Cursor *_cursor[14];

	};
}