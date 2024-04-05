//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: names mouse button inputs
// NOTE: Button codes also contain key codes, but we won't worry about that
//
// $NoKeywords: $
//===========================================================================//

#ifndef MOUSECODE_H
#define MOUSECODE_H

#ifdef _WIN32
#pragma once
#endif

//#include "inputsystem/ButtonCode.h"

namespace vgui
{
	enum MouseCode
	{
		MOUSE_LEFT_0,
		MOUSE_RIGHT_0,
		MOUSE_MIDDLE_0,
		MOUSE_4,
		MOUSE_5,
		MOUSE_LAST_0,
	};
}

#endif // MOUSECODE_H
