#include "precompiled.h"
#include "IPanel.h"
#include "vgui2/Panel.h"
#include "vgui2/IVGui.h"
#include "vgui2\VGui_Controls.h"
#include "ISchemeManager.h"
#include "BuildGroup.h"
#include "ToolTip.h"
#include "ISurface.h"
#include "vgui2/IInput.h"
#include "IVGui.h"
#include "KeyValues.h"
#include "IScheme.h"
#include "IBorder.h"
#include "utldict.h"
#include "MemoryPool.h"

const char *g_PinCornerStrings[] =
{
	"PIN_TOPLEFT",
	"PIN_TOPRIGHT",
	"PIN_BOTTOMLEFT",
	"PIN_BOTTOMRIGHT",

	"PIN_CENTER_TOP",
	"PIN_CENTER_RIGHT",
	"PIN_CENTER_BOTTOM",
	"PIN_CENTER_LEFT",
};



static char *CopyString(const char *in)
{
	if (!in)
		return NULL;

	int len = strlen(in);
	char *n = new char[len + 1];
	Q_strncpy(n, in, len + 1);
	return n;
}

//DECLARE_BUILD_FACTORY(Panel);

//-----------------------------------------------------------------------------
// Purpose: Constructor
//-----------------------------------------------------------------------------
vgui2::Panel::Panel()
{
	Init(0, 0, 64, 24);
}

//-----------------------------------------------------------------------------
// Purpose: Constructor
//-----------------------------------------------------------------------------
vgui2::Panel::Panel(Panel *parent)
{
	Init(0, 0, 64, 24);
	SetParent(parent);
}

//-----------------------------------------------------------------------------
// Purpose: Constructor
//-----------------------------------------------------------------------------
vgui2::Panel::Panel(Panel *parent, const char *panelName)
{
	Init(0, 0, 64, 24);
	SetName(panelName);
	SetParent(parent);
	SetBuildModeEditable(true);
}

//-----------------------------------------------------------------------------
// Purpose: Constructor
//-----------------------------------------------------------------------------
vgui2::Panel::Panel(Panel *parent, const char *panelName, HScheme scheme)
{
	Init(0, 0, 64, 24);
	SetName(panelName);
	SetParent(parent);
	SetBuildModeEditable(true);
	SetScheme(scheme);
}

//-----------------------------------------------------------------------------
// Purpose: Setup
//-----------------------------------------------------------------------------
void vgui2::Panel::Init(int x, int y, int wide, int tall)
{
	_vpanel = ivgui()->AllocPanel();
	ipanel()->Init(_vpanel, this);

	m_pTooltips = 0;
	SetPos(x, y);
	SetSize(wide, tall); 

	_needsRepaint = 0;
	_markedForDeletion = 0;
	_autoDelete = 1;
	_autoResizeDirection = AUTORESIZE_NO;
	_pinCorner = PIN_TOPLEFT;
	_cursor = 2;
	_needsLayout = 1;
	_needsSchemeUpdate = 1;
	_border = 0;
	_buildGroup = 0;
	_paintBorderEnabled = 1;
	_paintBackgroundEnabled = 1;
	_paintEnabled = 1;
	_postChildPaintEnabled = 0;
	_panelName = 0;
	_tabPosition = 0;
	m_bProportional = 0;
	m_iScheme = 0;
	_buildModeFlags = 0;
	m_bInPerformLayout = 0;
}

//-----------------------------------------------------------------------------
// Purpose: Destructor
//-----------------------------------------------------------------------------
vgui2::Panel::~Panel()
{
	_autoDelete = 0;
	_markedForDeletion = 1;

	// remove panel from any list
	SetParent((VPANEL)NULL);

	// Stop our children from pointing at us, and delete them if possible
	while (ipanel()->GetChildCount(GetVPanel()))
	{
		VPANEL child = ipanel()->GetChild(GetVPanel(), 0);
		if (ipanel()->IsAutoDeleteSet(child))
		{
			ipanel()->DeletePanel(child);
		}
		else
		{
			ipanel()->SetParent(child, NULL);
		}
	}

	// delete VPanel
	ivgui()->FreePanel(_vpanel);
	// free our name
	delete[] _panelName;

	_actionSignalTargetDar.Purge();

	NOT_IMPLEMENTED;
	//if (_vpanel)
	//	delete[](_vpanel);

	_vpanel = NULL;
	CHECK_REQUIRED;
	delete this;
}

//-----------------------------------------------------------------------------
// Purpose: fully construct this panel so its ready for use right now (i.e fonts loaded, colors set, default label text set, ...)
//-----------------------------------------------------------------------------
void vgui2::Panel::MakeReadyForUse()
{
	CHECK_REQUIRED;
	// Check from this calling
	vgui2::surface()->SolveTraverse(GetVPanel(), true);
}


//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void vgui2::Panel::SetName(const char *panelName)
{
	if (_panelName)
	{
		delete[] _panelName;
		_panelName = NULL;
	}

	if (panelName)
	{
		int len = Q_strlen(panelName) + 1;
		_panelName = new char[len];
		Q_strncpy(_panelName, panelName, len);
	}
}

//-----------------------------------------------------------------------------
// Purpose: returns the given name of the panel
//-----------------------------------------------------------------------------
const char *vgui2::Panel::GetName()
{
	if (_panelName)
		return _panelName;

	return "";
}

//-----------------------------------------------------------------------------
// Purpose: returns the name of the module that this instance of panel was compiled into
//-----------------------------------------------------------------------------
const char *vgui2::Panel::GetModuleName()
{
	return vgui2::GetControlsModuleName();
}

vgui::PanelMessageMap * vgui2::Panel::GetMessageMap()
{
	NOT_IMPLEMENTED;
	return nullptr;
}

//-----------------------------------------------------------------------------
// Purpose: returns the classname of the panel (as specified in the panelmaps)
//-----------------------------------------------------------------------------
const char *vgui2::Panel::GetClassName()
{
	// loop up the panel map name
	vgui::PanelMessageMap *panelMap = GetMessageMap();
	if (panelMap)
	{
		return panelMap->pfnClassName();
	}

	return "Panel";
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void vgui2::Panel::SetPos(int x, int y)
{
	ipanel()->SetPos(GetVPanel(), x, y);
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void vgui2::Panel::GetPos(int &x, int &y)
{
	ipanel()->GetPos(GetVPanel(), x, y);
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void vgui2::Panel::SetSize(int wide, int tall)
{
	ipanel()->SetSize(GetVPanel(), wide, tall);
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void vgui2::Panel::GetSize(int &wide, int &tall)
{
	ipanel()->GetSize(GetVPanel(),  wide, tall);
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void vgui2::Panel::SetBounds(int x, int y, int wide, int tall)
{
	SetPos(x, y);
	SetSize(wide, tall);
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void vgui2::Panel::GetBounds(int &x, int &y, int &wide, int &tall)
{
	GetPos(x, y);
	GetSize(wide, tall);
}

//-----------------------------------------------------------------------------
// Purpose: returns safe handle to parent
//-----------------------------------------------------------------------------
vgui2::VPANEL vgui2::Panel::GetVParent()
{
	if (vgui2::ipanel())
	{
		return vgui2::ipanel()->GetParent(GetVPanel());
	}

	return 0;
}

//-----------------------------------------------------------------------------
// Purpose: returns a pointer to a controls version of a Panel pointer
//-----------------------------------------------------------------------------
vgui2::Panel *vgui2::Panel::GetParent()
{
	if (ipanel())
	{
		VPANEL parent = ipanel()->GetParent(GetVPanel());
		if (parent)
		{
			Panel *pParent = ipanel()->GetPanel(parent, vgui2::GetControlsModuleName());
			return pParent;
		}
	}

	return NULL;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void vgui2::Panel::SetVisible(bool state)
{
	vgui2::ipanel()->SetVisible(GetVPanel(), state);
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
bool vgui2::Panel::IsVisible()
{
	if (vgui2::ipanel())
	{
		return vgui2::ipanel()->IsVisible(GetVPanel());
	}

	return false;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void vgui2::Panel::SetEnabled(bool state)
{
	if (state != ipanel()->IsEnabled(GetVPanel()))
	{
		ipanel()->SetEnabled(GetVPanel(), state);
		InvalidateLayout(false, false);
		Repaint();
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
bool vgui2::Panel::IsEnabled()
{
	return ipanel()->IsEnabled(GetVPanel());
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
bool vgui2::Panel::IsPopup()
{
	return ipanel()->IsPopup(GetVPanel());
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void vgui2::Panel::Repaint()
{
	if (vgui2::surface())
	{
		vgui2::surface()->Invalidate(GetVPanel());
	}
}

vgui2::VPANEL vgui2::Panel::GetVPanel()
{
	return _vpanel;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void vgui2::Panel::Think()
{
	if (IsVisible())
	{
		// update any tooltips
		if (m_pTooltips)
		{
			m_pTooltips->PerformLayout();
		}
		if (_needsLayout)
		{
			InternalPerformLayout();
		}
	}

	OnThink();
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void vgui2::Panel::PaintTraverse(bool repaint, bool allowForce)
{
	if (!IsVisible())
	{
		return;
	}

	if (_needsRepaint && allowForce)
	{
		repaint = true;
		_needsRepaint = false;
	}
	else if ( allowForce )
    {
      _needsRepaint = false;
    }

	int clipRect[4];
	vgui2::ipanel()->GetClipRect(_vpanel, clipRect[0], clipRect[1], clipRect[2], clipRect[3]);
	if ((clipRect[2] <= clipRect[0]) || (clipRect[3] <= clipRect[1]))
	{
		repaint = false;
	}
	else if (repaint || _paintBorderEnabled)
	{
		// draw the background with no inset
		if (_paintBackgroundEnabled)
		{
			surface()->PushMakeCurrent(_vpanel, false);
			PaintBackground();
			surface()->PopMakeCurrent(_vpanel);
		}

		// draw the front of the panel with the inset
		if (_paintEnabled)
		{
			surface()->PushMakeCurrent(_vpanel, true);
			Paint();
			surface()->PopMakeCurrent(_vpanel);
		}
	}

	CHECK_REQUIRED;
	// traverse and paint all our children
	int childCount = vgui2::ipanel()->GetChildCount(GetVPanel());
	for (int i = 0; i < childCount; i++)
	{
		VPANEL child = ipanel()->GetChild(GetVPanel(), i);
		bool bVisible = ipanel()->IsVisible(child);

		if (surface()->ShouldPaintChildPanel(child))
		{
			if (bVisible)
			{
				ipanel()->PaintTraverse(child, repaint, allowForce);
			}
		}
		else
		{
			// Invalidate the child panel so that it gets redrawn
			surface()->Invalidate(child);

			// keep traversing the tree, just don't allow anyone to paint after here
			if (bVisible)
			{
				ipanel()->PaintTraverse(child, false, false);
			}
		}
	}

	// draw the border last
	if (repaint)
	{
		if (_paintBorderEnabled && (_border != nullptr))
		{
			// Paint the border over the background with no inset
			surface()->PushMakeCurrent(_vpanel, false);
			PaintBorder();
			surface()->PopMakeCurrent(_vpanel);
		}

	NOT_IMPLEMENTED;
	/*	
	// IsBuildGroupEnabled recurses up all the parents and ends up being very expensive as it wanders all over memory
		if (IsBuildGroupEnabled() && HasFocus())
		{
			// outline all selected panels 
			CUtlVector<PHandle> *controlGroup = _buildGroup->GetControlGroup();
			for (int i = 0; i < controlGroup->Size(); ++i)
			{
				// outline all selected panels 
				CUtlVector<PHandle> *controlGroup = _buildGroup->GetControlGroup();
				for (int i = 0; i < controlGroup->Size(); ++i)
				{
					surface()->PushMakeCurrent(((*controlGroup)[i].Get())->GetVPanel(), false);
					(*controlGroup[i].Get())->PaintBuildOverlay();
					surface()->PopMakeCurrent(((*controlGroup)[i].Get())->GetVPanel());
				}

				_buildGroup->DrawRulers();
			}
		}
		*/
		// All of our children have painted, etc, now allow painting in top of them
		if (_postChildPaintEnabled)
		{
			surface()->PushMakeCurrent(_vpanel, false);
			PostChildPaint();
			surface()->PopMakeCurrent(_vpanel);
		}
	}

	surface()->SwapBuffers(_vpanel);

	//if (bPushedViewport)
	//{
	//	surface()->PopFullscreenViewport();
	//}
}


//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void vgui2::Panel::PaintBorder()
{
	NOT_IMPLEMENTED;
	//_border->Paint(GetVPanel());
}


//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void vgui2::Panel::PaintBackground()
{
	int wide, tall;
	GetSize(wide, tall);
	NOT_IMPLEMENTED;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void vgui2::Panel::Paint()
{
	// empty on purpose
	// PaintBackground is painted and default behavior is for Paint to do nothing
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void vgui2::Panel::PostChildPaint()
{
	// Empty on purpose
	// This is called if _postChildPaintEnabled is true and allows painting to
	//  continue on the surface after all of the panel's children have painted 
	//  themselves.  Allows drawing an overlay on top of the children, etc.
}

//-----------------------------------------------------------------------------
// Purpose: Draws a black rectangle around the panel.
//-----------------------------------------------------------------------------
void vgui2::Panel::PaintBuildOverlay()
{
	int wide, tall;
	GetSize(wide, tall);
	surface()->DrawSetColor(0, 0, 0, 255);

	surface()->DrawFilledRect(0, 0, wide, 2);           //top
	surface()->DrawFilledRect(0, tall - 2, wide, tall);   //bottom
	surface()->DrawFilledRect(0, 2, 2, tall - 2);         //left
	surface()->DrawFilledRect(wide - 2, 2, wide, tall - 2); //right
}

//-----------------------------------------------------------------------------
// Purpose: Returns true if the panel's draw code will fully cover it's area
//-----------------------------------------------------------------------------
bool vgui2::Panel::IsOpaque()
{
	// FIXME: Add code to account for the 'SkipChild' functionality in Frame
	if (IsVisible() && _paintBackgroundEnabled && (_bgColor[3] == 255))
		return true;

	return false;
}

//-----------------------------------------------------------------------------
// Purpose: sets the parent
//-----------------------------------------------------------------------------
void vgui2::Panel::SetParent(Panel *newParent)
{
	// Assert that the parent is from the same module as the child
	// FIXME: !!! work out how to handle this properly!
	//	Assert(!newParent || !strcmp(newParent->GetModuleName(), GetControlsModuleName()));

	if (newParent)
	{
		SetParent(newParent->GetVPanel());
	}
	else
	{
		SetParent((VPANEL)NULL);
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void vgui2::Panel::SetParent(VPANEL newParent)
{	
	if (newParent)
	{
		ipanel()->SetParent(GetVPanel(), newParent);
		if (!GetVParent())
			return;
	}
	else
	{
		ipanel()->SetParent(_vpanel, NULL);
		if (!GetVParent())
			return;
	}

	if (!IsPopup())
	{
		SetProportional(ipanel()->IsProportional(GetVParent()));

		// most of the time KBInput == parents kbinput
		if (ipanel()->IsKeyBoardInputEnabled(GetVParent()) != IsKeyBoardInputEnabled())
		{
			SetKeyBoardInputEnabled(ipanel()->IsKeyBoardInputEnabled(GetVParent()));
		}

		if (ipanel()->IsMouseInputEnabled(GetVParent()) != IsMouseInputEnabled())
		{
			SetMouseInputEnabled(ipanel()->IsMouseInputEnabled(GetVParent()));
		}
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void vgui2::Panel::OnChildAdded(VPANEL child)
{
	// Nothing
}

//-----------------------------------------------------------------------------
// Purpose: default message handler
//-----------------------------------------------------------------------------
void vgui2::Panel::OnSizeChanged(int newWide, int newTall)
{
	InvalidateLayout(false, false); // our size changed so force us to layout again
}

//-----------------------------------------------------------------------------
// Purpose: sets Z ordering - lower numbers are always behind higher z's
//-----------------------------------------------------------------------------
void vgui2::Panel::SetZPos(int z)
{
	vgui2::ipanel()->SetZPos(GetVPanel(), z);
}

//-----------------------------------------------------------------------------
// Purpose: sets Z ordering - lower numbers are always behind higher z's
//-----------------------------------------------------------------------------
int vgui2::Panel::GetZPos()
{
	return (vgui2::ipanel()->GetZPos(GetVPanel()));
}

//-----------------------------------------------------------------------------
// Purpose: Moves the panel to the front of the z-order
//-----------------------------------------------------------------------------
void vgui2::Panel::MoveToFront(void)
{
	CHECK_REQUIRED;

	// FIXME: only use ipanel() as per src branch?
	if (IsPopup())
	{
		vgui2::surface()->BringToFront(GetVPanel());
	}
	else
	{
		vgui2::ipanel()->MoveToFront(GetVPanel());
	}
}

//-----------------------------------------------------------------------------
// Purpose: Iterates up the hierarchy looking for a particular parent
//-----------------------------------------------------------------------------
bool vgui2::Panel::HasParent(VPANEL potentialParent)
{
	if (!potentialParent)
		return false;

	return ipanel()->HasParent(GetVPanel(), potentialParent);
}

//-----------------------------------------------------------------------------
// Purpose: Finds a child panel by string name
// Output : Panel * - NULL if no panel of that name is found
//-----------------------------------------------------------------------------
vgui2::Panel *vgui2::Panel::FindChildByName(const char *childName, bool recurseDown)
{
	for (int i = 0; i < GetChildCount(); i++)
	{
		Panel *pChild = GetChild(i);
		if (!pChild)
			continue;

		if (!Q_stricmp(pChild->GetName(), childName))
		{
			return pChild;
		}

		if (recurseDown)
		{
			Panel *panel = pChild->FindChildByName(childName, recurseDown);
			if (panel)
			{
				return panel;
			}
		}
	}

	return NULL;
}

//-----------------------------------------------------------------------------
// Purpose: Finds a sibling panel by name
//-----------------------------------------------------------------------------
vgui2::Panel *vgui2::Panel::FindSiblingByName(const char *siblingName)
{
	if (!GetVParent())
		return NULL;

	int siblingCount = ipanel()->GetChildCount(GetVParent());
	for (int i = 0; i < siblingCount; i++)
	{
		VPANEL sibling = vgui2::ipanel()->GetChild(GetVParent(), i);
		Panel *panel = vgui2::ipanel()->GetPanel(sibling, vgui2::GetControlsModuleName());
		if (!Q_stricmp(panel->GetName(), siblingName))
		{
			return panel;
		}
	}

	return NULL;
}

//-----------------------------------------------------------------------------
// Purpose: Dispatches immediately a message to the parent
//-----------------------------------------------------------------------------
void vgui2::Panel::CallParentFunction(KeyValues *message)
{
	if (GetVParent())
	{
		ipanel()->SendMessage(GetVParent(), message, GetVPanel());
	}
	if (message)
	{
		message->deleteThis();
	}
}

//-----------------------------------------------------------------------------
// Purpose: if set to true, panel automatically frees itself when parent is deleted
//-----------------------------------------------------------------------------
void vgui2::Panel::SetAutoDelete(bool state)
{
	_autoDelete = true;
}


//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
bool vgui2::Panel::IsAutoDeleteSet()
{
	return _autoDelete;
}


//-----------------------------------------------------------------------------
// Purpose: Just calls 'delete this'
//-----------------------------------------------------------------------------
void vgui2::Panel::DeletePanel()
{
	NOT_IMPLEMENTED;
	/*
	// Avoid re-entrancy
	_flags.SetFlag(MARKED_FOR_DELETION);
	_flags.ClearFlag(AUTODELETE_ENABLED);
	delete this;
	*/
}

//-----------------------------------------------------------------------------
// Purpose: data accessor
//-----------------------------------------------------------------------------
vgui2::HScheme vgui2::Panel::GetScheme()
{
	if (m_iScheme)
	{
		return m_iScheme; // return our internal scheme
	}

	if (GetVParent()) // recurse down the heirarchy 
	{
		return ipanel()->GetScheme(GetVParent());
	}

	return scheme()->GetDefaultScheme();
}

bool vgui2::Panel::IsProportional()
{
	return m_bProportional;
}

//-----------------------------------------------------------------------------
// Purpose: set the scheme to render this panel with by name
//-----------------------------------------------------------------------------
void vgui2::Panel::SetScheme(const char *tag)
{
	if (strlen(tag) > 0 && scheme()->GetScheme(tag)) // check the scheme exists
	{
		SetScheme(scheme()->GetScheme(tag));
	}
}

//-----------------------------------------------------------------------------
// Purpose: set the scheme to render this panel with 
//-----------------------------------------------------------------------------
void vgui2::Panel::SetScheme(HScheme scheme)
{
	if (scheme != m_iScheme)
	{
		m_iScheme = scheme;

		// This will cause the new scheme to be applied at a later point
		//		InvalidateLayout( false, true );
	}
}


//-----------------------------------------------------------------------------
// Purpose: returns the char of this panels hotkey
//-----------------------------------------------------------------------------
vgui2::Panel *vgui2::Panel::HasHotkey(wchar_t key)
{
	return nullptr;
}

void vgui2::Panel::InternalCursorMoved(int x, int y)
{
	NOT_IMPLEMENTED;
	/*
	if (IsCursorNone())
		return;

	if (!IsMouseInputEnabled())
	{
		return;
	}

	if (IsBuildGroupEnabled())
	{
		if (_buildGroup->CursorMoved(x, y, this))
		{
			return;
		}
	}

	if (m_pTooltips)
	{
		if (_tooltipText)
		{
			m_pTooltips->SetText(_tooltipText);
		}
		m_pTooltips->ShowTooltip(this);
	}

	ScreenToLocal(x, y);

	OnCursorMoved(x, y);
	*/
}

void vgui2::Panel::InternalCursorEntered()
{
	NOT_IMPLEMENTED;
	
	/*
	if (IsCursorNone() || !IsMouseInputEnabled())
		return;

	if (IsBuildGroupEnabled())
		return;

	if (m_pTooltips)
	{
		m_pTooltips->ResetDelay();

		if (_tooltipText)
		{
			m_pTooltips->SetText(_tooltipText);
		}
		m_pTooltips->ShowTooltip(this);
	}

	OnCursorEntered();
	*/
}

void vgui2::Panel::InternalCursorExited()
{ 
	CHECK_REQUIRED;
	if (IsCursorNone() || !IsMouseInputEnabled())
		return;

	if (IsBuildGroupEnabled())
		return;

	if (m_pTooltips)
	{
		m_pTooltips->HideTooltip();
	}

	OnCursorExited();
}

void vgui2::Panel::InternalMousePressed(int code)
{
	NOT_IMPLEMENTED;
	if (IsCursorNone())
		return;

	if (IsBuildGroupEnabled())
	{
		//if (_buildGroup->MousePressed((MouseCode)code, this))
		//{
		//	return;
		//}
	}
}

void vgui2::Panel::InternalMouseDoublePressed(int code)
{
	NOT_IMPLEMENTED;
}

void vgui2::Panel::InternalMouseReleased(int code)
{
	NOT_IMPLEMENTED;
}

void vgui2::Panel::InternalMouseWheeled(int delta)
{
	NOT_IMPLEMENTED;
}

void vgui2::Panel::InternalKeyCodePressed(int code)
{
	NOT_IMPLEMENTED;
	if (IsKeyBoardInputEnabled())
	{
		OnKeyCodePressed((KeyCode)code);
	}
	else
	{
		CallParentFunction(new KeyValues("KeyCodePressed", "code", code));
	}
}

void vgui2::Panel::InternalKeyCodeTyped(int code)
{
	NOT_IMPLEMENTED;
	/*
	if (IsKeyBoardInputEnabled())
	{
		bool shift = (input()->IsKeyDown(KEY_LSHIFT) || input()->IsKeyDown(KEY_RSHIFT));
		bool ctrl = (input()->IsKeyDown(KEY_LCONTROL) || input()->IsKeyDown(KEY_RCONTROL));
		bool alt = (input()->IsKeyDown(KEY_LALT) || input()->IsKeyDown(KEY_RALT));

		int modifiers = 0;
		if (shift)
		{
			modifiers |= MODIFIER_SHIFT;
		}
		if (ctrl)
		{
			modifiers |= MODIFIER_CONTROL;
		}
		if (alt)
		{
			modifiers |= MODIFIER_ALT;
		}

		// Things in build mode don't have accelerators
		if (IsBuildGroupEnabled())
		{
			_buildGroup->KeyCodeTyped((KeyCode)code, this);
			return;
		}

		if (!s_bSuppressRebindChecks && IsKeyRebound((KeyCode)code, modifiers))
		{
			return;
		}

		bool oldVal = s_bSuppressRebindChecks;
		s_bSuppressRebindChecks = true;
		OnKeyCodeTyped((KeyCode)code);
		s_bSuppressRebindChecks = oldVal;
	}
	else
	{
		if (GetVPanel() == surface()->GetEmbeddedPanel())
		{
			input()->OnKeyCodeUnhandled(code);
		}
		CallParentFunction(new KeyValues("KeyCodeTyped", "code", code));
	}
	*/
}

void vgui2::Panel::InternalKeyTyped(int unichar)
{
	NOT_IMPLEMENTED;
	
	/*
	if (IsKeyBoardInputEnabled())
	{
		if (IsBuildGroupEnabled())
		{
			if (_buildGroup->KeyTyped((wchar_t)unichar, this))
			{
				return;
			}
		}

		OnKeyTyped((wchar_t)unichar);
	}
	else
	{
		CallParentFunction(new KeyValues("KeyTyped", "unichar", unichar));
	}
	*/
}

void vgui2::Panel::InternalKeyCodeReleased(int code)
{
	NOT_IMPLEMENTED;
/*
	if (IsKeyBoardInputEnabled())
	{
		if (IsBuildGroupEnabled())
		{
			if (_buildGroup->KeyCodeReleased((KeyCode)code, this))
			{
				return;
			}
		}

		OnKeyCodeReleased((KeyCode)code);
	}
	else
	{
		CallParentFunction(new KeyValues("KeyCodeReleased", "code", code));
	}
	*/
}

void vgui2::Panel::InternalKeyFocusTicked()
{
	NOT_IMPLEMENTED;
	
	/*
	if (IsBuildGroupEnabled())
		return;

	OnKeyFocusTicked();
	*/
}

void vgui2::Panel::InternalMouseFocusTicked()
{
	NOT_IMPLEMENTED;
	
	/*
	if (IsBuildGroupEnabled())
	{
		// must repaint so the numbers will be accurate
		if (_buildGroup->HasRulersOn())
		{
			PaintTraverse(true);
		}
		return;
	}

	// update cursor
	InternalSetCursor();
	OnMouseFocusTicked();
	*/
}


void vgui2::Panel::InternalSetCursor()
{
	NOT_IMPLEMENTED;
	
	/*
	bool visible = IsVisible();

	if (visible)
	{
		// chain up and make sure all our parents are also visible
		VPANEL p = GetVParent();
		while (p)
		{
			visible &= ipanel()->IsVisible(p);
			p = ipanel()->GetParent(p);
		}

		// only change the cursor if this panel is visible, and if its part of the main VGUI tree
		if (visible && HasParent(surface()->GetEmbeddedPanel()))
		{
			HCursor cursor = GetCursor();

			if (IsBuildGroupEnabled())
			{
				cursor = _buildGroup->GetCursor(this);
			}

			if (input()->GetCursorOveride())
			{
				cursor = input()->GetCursorOveride();
			}

			surface()->SetCursor(cursor);
		}
	}
	*/
}

//-----------------------------------------------------------------------------
// Purpose: Called every frame the panel is visible, designed to be overridden
//-----------------------------------------------------------------------------
void vgui2::Panel::OnThink()
{
	// Nothing
}

// input messages handlers (designed for override)
void vgui2::Panel::OnCursorMoved(int x, int y)
{
	NOT_IMPLEMENTED;

/*
if (ParentNeedsCursorMoveEvents())
	{
		// figure out x and y in parent space
		int thisX, thisY;
		ipanel()->GetPos(GetVPanel(), thisX, thisY);
		CallParentFunction(new KeyValues("OnCursorMoved", "x", x + thisX, "y", y + thisY));
	}
	*/
}

void vgui2::Panel::OnCursorEntered()
{
	// Nothing
}

void vgui2::Panel::OnCursorExited()
{	
	// Nothing
}

void vgui2::Panel::OnMousePressed(MouseCode code)
{
	// Nothing
}

void vgui2::Panel::OnMouseDoublePressed(MouseCode code)
{	
	// Nothing
}

void vgui2::Panel::OnMouseReleased(MouseCode code)
{	
	// Nothing
}

void vgui2::Panel::OnMouseWheeled(int delta)
{
	CHECK_REQUIRED;

	CallParentFunction(new KeyValues("MouseWheeled", "delta", delta));
}

// base implementation forwards Key messages to the Panel's parent - override to 'swallow' the input
void vgui2::Panel::OnKeyCodePressed(KeyCode code)
{
	CHECK_REQUIRED;

	CallParentFunction(new KeyValues("KeyCodePressed", "code", code));
}

void vgui2::Panel::OnKeyCodeTyped(KeyCode keycode)
{
	NOT_IMPLEMENTED;
	/*
	if (GetVPanel() == surface()->GetEmbeddedPanel())
		{
			vgui2::input()->OnKeyCodeUnhandled(keycode);
		}
		CallParentFunction(new KeyValues("KeyCodeTyped", "code", keycode));
		*/
}

void vgui2::Panel::OnKeyTyped(wchar_t unichar)
{
	CHECK_REQUIRED;

	CallParentFunction(new KeyValues("KeyTyped", "unichar", unichar));
}

void vgui2::Panel::OnKeyCodeReleased(KeyCode code)
{
	CHECK_REQUIRED;

	CallParentFunction(new KeyValues("KeyCodeReleased", "code", code));
}

void vgui2::Panel::OnKeyFocusTicked()
{
	CHECK_REQUIRED;

	CallParentFunction(new KeyValues("KeyFocusTicked"));
}

void vgui2::Panel::OnMouseFocusTicked()
{
	CHECK_REQUIRED;

	CallParentFunction(new KeyValues("OnMouseFocusTicked"));
}

bool vgui2::Panel::IsWithin(int x, int y)
{
	CHECK_REQUIRED;

	// check against our clip rect
	int clipRect[4];
	ipanel()->GetClipRect(GetVPanel(), clipRect[0], clipRect[1], clipRect[2], clipRect[3]);

	if (x < clipRect[0])
	{
		return false;
	}

	if (y < clipRect[1])
	{
		return false;
	}

	if (x >= clipRect[2])
	{
		return false;
	}

	if (y >= clipRect[3])
	{
		return false;
	}

	return true;
}

//-----------------------------------------------------------------------------
// Purpose: determines which is the topmost panel under the coordinates (x, y)
//-----------------------------------------------------------------------------
vgui2::VPANEL vgui2::Panel::IsWithinTraverse(int x, int y, bool traversePopups)
{
	NOT_IMPLEMENTED;

	/*
	// if this one is not visible, its children won't be either
	// also if it doesn't want mouse input its children can't get it either
	if (!IsVisible() || !IsMouseInputEnabled())
		return NULL;

	if (traversePopups)
	{
		// check popups first
		int i;
		CUtlVector< VPANEL > &children = ipanel()->GetChildren(GetVPanel());
		int childCount = children.Count();
		for (i = childCount - 1; i >= 0; i--)
		{
			VPANEL panel = children[i];
			if (ipanel()->IsPopup(panel))
			{
				panel = ipanel()->IsWithinTraverse(panel, x, y, true);
				if (panel != null)
				{
					return panel;
				}
			}
		}

		// check children recursive, if you find one, just return first one
		// this checks in backwards order so the last child drawn for this panel is chosen which
		// coincides to how it would be visibly displayed
		for (i = childCount - 1; i >= 0; i--)
		{
			VPANEL panel = children[i];
			// we've already checked popups so ignore
			if (!ipanel()->IsPopup(panel))
			{
				panel = ipanel()->IsWithinTraverse(panel, x, y, true);
				if (panel != 0)
				{
					return panel;
				}
			}
		}

		// check ourself
		if (!IsMouseInputDisabledForThisPanel() && IsWithin(x, y))
		{
			return GetVPanel();
		}
	}
	else
	{
		// since we're not checking popups, it must be within us, so we can check ourself first
		if (IsWithin(x, y))
		{
			// check children recursive, if you find one, just return first one
			// this checks in backwards order so the last child drawn for this panel is chosen which
			// coincides to how it would be visibly displayed
			CUtlVector< VPANEL > &children = ipanel()->GetChildren(GetVPanel());
			int childCount = children.Count();
			for (int i = childCount - 1; i >= 0; i--)
			{
				VPANEL panel = children[i];
				// ignore popups
				if (!ipanel()->IsPopup(panel))
				{
					panel = ipanel()->IsWithinTraverse(panel, x, y, false);
					if (panel != 0)
					{
						return panel;
					}
				}
			}

			// not a child, must be us
			if (!IsMouseInputDisabledForThisPanel())
				return GetVPanel();
		}
	}
	*/
	return NULL;
}

void vgui2::Panel::LocalToScreen(int& x, int& y)
{
	int px, py;
	ipanel()->GetAbsPos(GetVPanel(), px, py);

	x = x + px;
	y = y + py;
}

void vgui2::Panel::ScreenToLocal(int& x, int& y)
{
	int px, py;
	ipanel()->GetAbsPos(GetVPanel(), px, py);

	x = x - px;
	y = y - py;
}

void vgui2::Panel::ParentLocalToScreen(int &x, int &y)
{
	int px, py;
	ipanel()->GetAbsPos(GetVParent(), px, py);

	x = x + px;
	y = y + py;
}

void vgui2::Panel::MakePopup(bool showTaskbarIcon, bool disabled)
{
	surface()->CreatePopup(GetVPanel(), false, showTaskbarIcon, disabled, true, true);
}

void vgui2::Panel::SetCursor(HCursor cursor)
{
	_cursor = cursor;
}

vgui2::HCursor vgui2::Panel::GetCursor()
{
	return _cursor;
}

void vgui2::Panel::SetMinimumSize(int wide, int tall)
{
	ipanel()->SetMinimumSize(GetVPanel(), wide, tall);
}

void vgui2::Panel::GetMinimumSize(int& wide, int &tall)
{
	ipanel()->GetMinimumSize(GetVPanel(), wide, tall);
}

bool vgui2::Panel::IsBuildModeEditable()
{
	return true;
}

void vgui2::Panel::SetBuildModeEditable(bool state)
{
	if (state)
	{
		_buildModeFlags |= BUILDMODE_EDITABLE;
	}
	else
	{
		_buildModeFlags &= ~BUILDMODE_EDITABLE;
	}
}

//-----------------------------------------------------------------------------
// Purpose: data accessor
//-----------------------------------------------------------------------------
bool vgui2::Panel::IsBuildModeDeletable()
{
	return (_buildModeFlags & BUILDMODE_DELETABLE);
}

//-----------------------------------------------------------------------------
// Purpose: data accessor
//-----------------------------------------------------------------------------
void vgui2::Panel::SetBuildModeDeletable(bool state)
{
	if (state)
	{
		_buildModeFlags |= BUILDMODE_DELETABLE;
	}
	else
	{
		_buildModeFlags &= ~BUILDMODE_DELETABLE;
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
bool vgui2::Panel::IsBuildModeActive()
{
	return _buildGroup ? _buildGroup->IsEnabled() : false;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void vgui2::Panel::GetClipRect(int& x0, int& y0, int& x1, int& y1)
{
	ipanel()->GetClipRect(GetVPanel(), x0, y0, x1, y1);
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
int vgui2::Panel::GetChildCount()
{
	if (ipanel())
	{
		return ipanel()->GetChildCount(GetVPanel());
	}

	return 0;
}

//-----------------------------------------------------------------------------
// Purpose: returns a child by the specified index
//-----------------------------------------------------------------------------
vgui2::Panel *vgui2::Panel::GetChild(int index)
{
	// get the child and cast it to a panel
	// this assumes that the child is from the same module as the this (precondition)
	return ipanel()->GetPanel(ipanel()->GetChild(GetVPanel(), index), GetControlsModuleName());
}

//-----------------------------------------------------------------------------
// Purpose: moves the key focus back
//-----------------------------------------------------------------------------
bool vgui2::Panel::RequestFocusPrev(VPANEL panel)
{
	CHECK_REQUIRED;
	// chain to parent
	if (GetVParent())
	{
		return ipanel()->RequestFocusPrev(GetVParent(), GetVPanel());
	}
	return false;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
bool vgui2::Panel::RequestFocusNext(VPANEL panel)
{
	CHECK_REQUIRED;
	// chain to parent
	if (GetVParent())
	{
		return ipanel()->RequestFocusNext(GetVParent(), GetVPanel());
	}
	return false;
}

//-----------------------------------------------------------------------------
// Purpose: Sets the panel to have the current sub focus
// Input  : direction - the direction in which focus travelled to arrive at this panel; forward = 1, back = -1
//-----------------------------------------------------------------------------
void vgui2::Panel::RequestFocus(int direction)
{
	CHECK_REQUIRED;

	//	ivgui()->DPrintf2("RequestFocus(%s, %s)\n", GetName(), GetClassName());
	OnRequestFocus(GetVPanel(), NULL);
}

//-----------------------------------------------------------------------------
// Purpose: Called after a panel requests focus to fix up the whole chain
//-----------------------------------------------------------------------------
void vgui2::Panel::OnRequestFocus(VPANEL subFocus, VPANEL defaultPanel)
{
	CHECK_REQUIRED;

	CallParentFunction(new KeyValues("OnRequestFocus", "subFocus", subFocus, "defaultPanel", defaultPanel));
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
vgui2::VPANEL vgui2::Panel::GetCurrentKeyFocus()
{
	return 0;
}

//-----------------------------------------------------------------------------
// Purpose: returns true if the panel has focus
//-----------------------------------------------------------------------------
bool vgui2::Panel::HasFocus()
{
	CHECK_REQUIRED;
	
	if (input()->GetFocus() == GetVPanel())
	{
		return true;
	}
	return false;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void vgui2::Panel::SetTabPosition(int position)
{
	_tabPosition = position;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
int vgui2::Panel::GetTabPosition()
{
	return _tabPosition;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void vgui2::Panel::InternalFocusChanged(bool lost)
{
	// Nothing
}

//-----------------------------------------------------------------------------
// Purpose: Called when a panel loses it's mouse capture
//-----------------------------------------------------------------------------
void vgui2::Panel::OnMouseCaptureLost()
{
	CHECK_REQUIRED;
	
	if (m_pTooltips)
	{
		m_pTooltips->ResetDelay();
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void vgui2::Panel::AddActionSignalTarget(Panel *messageTarget)
{
	CHECK_REQUIRED;

	HPanel target = ivgui()->PanelToHandle(messageTarget->GetVPanel());
	if (!_actionSignalTargetDar.HasElement(target))
	{
		_actionSignalTargetDar.AddElement(target);
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void vgui2::Panel::AddActionSignalTarget(VPANEL messageTarget)
{
	CHECK_REQUIRED;

	HPanel target = ivgui()->PanelToHandle(messageTarget);
	if (!_actionSignalTargetDar.HasElement(target))
	{
		_actionSignalTargetDar.AddElement(target);
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void vgui2::Panel::RemoveActionSignalTarget(Panel *oldTarget)
{
	CHECK_REQUIRED;

	_actionSignalTargetDar.RemoveElement(ivgui()->PanelToHandle(oldTarget->GetVPanel()));
}

//-----------------------------------------------------------------------------
// Purpose: Sends a message to all the panels that have requested action signals
//-----------------------------------------------------------------------------
void vgui2::Panel::PostActionSignal(KeyValues *message)
{
	NOT_IMPLEMENTED;
	/*
	if (m_bIsSilent != true)
	{
		// add who it was from the message
		message->SetPtr("panel", this);
		int i;
		for (i = _actionSignalTargetDar.GetCount() - 1; i > 0; i--)
		{
			VPANEL panel = ivgui()->HandleToPanel(_actionSignalTargetDar[i]);
			if (panel)
			{
				ivgui()->PostMessage(panel, message->MakeCopy(), GetVPanel());
			}
		}

		// do this so we can save on one MakeCopy() call
		if (i == 0)
		{
			VPANEL panel = ivgui()->HandleToPanel(_actionSignalTargetDar[i]);
			if (panel)
			{
				ivgui()->PostMessage(panel, message, GetVPanel());
				return;
			}
		}
	}
	message->deleteThis();
	*/
}

void vgui2::Panel::SetBorder(IBorder *border)
{
	CHECK_REQUIRED;

	_border = border;

	if (border)
	{
		int x, y, x2, y2;
		border->GetInset(x, y, x2, y2);
		ipanel()->SetInset(GetVPanel(), x, y, x2, y2);
	}
	else
	{
		ipanel()->SetInset(GetVPanel(), 0, 0, 0, 0);
	}
}

vgui2::IBorder *vgui2::Panel::GetBorder()
{
	return _border;
}


void vgui2::Panel::SetPaintBorderEnabled(bool state)
{
	_paintBorderEnabled = state;
}

void vgui2::Panel::SetPaintBackgroundEnabled(bool state)
{
	_paintBackgroundEnabled = state;
}

void vgui2::Panel::SetPaintEnabled(bool state)
{
	_paintEnabled = state;
}

void vgui2::Panel::SetPostChildPaintEnabled(bool state)
{
	_postChildPaintEnabled = state;
}

void vgui2::Panel::GetInset(int& left, int& top, int& right, int& bottom)
{
	CHECK_REQUIRED;

	ipanel()->GetInset(GetVPanel(), left, top, right, bottom);
}

void vgui2::Panel::GetPaintSize(int& wide, int& tall)
{
	CHECK_REQUIRED;
	
	GetSize(wide, tall);
	if (_border != nullptr)
	{
		int left, top, right, bottom;
		_border->GetInset(left, top, right, bottom);

		wide -= (left + right);
		tall -= (top + bottom);
	}
}

int vgui2::Panel::GetWide()
{
	CHECK_REQUIRED;
	
	int wide, tall;
	ipanel()->GetSize(GetVPanel(), wide, tall);
	return wide;
}

void vgui2::Panel::SetWide(int wide)
{
	CHECK_REQUIRED;

	ipanel()->SetSize(GetVPanel(), wide, GetTall());
}

int vgui2::Panel::GetTall()
{
	int wide, tall;
	ipanel()->GetSize(GetVPanel(), wide, tall);
	return tall;
}

void vgui2::Panel::SetTall(int tall)
{
	ipanel()->SetSize(GetVPanel(), GetWide(), tall);
}

void vgui2::Panel::SetBuildGroup(BuildGroup* buildGroup)
{
	CHECK_REQUIRED;

	_buildGroup = buildGroup;

	_buildGroup->PanelAdded(this);
}

bool vgui2::Panel::IsBuildGroupEnabled()
{
	if (_buildGroup)
		return _buildGroup->IsEnabled();

	return false;
}

void vgui2::Panel::SetBgColor(Color color)
{
	_bgColor = color;
}

void vgui2::Panel::SetFgColor(Color color)
{
	_fgColor = color;
}

Color& vgui2::Panel::GetBgColor(Color& color)
{
	color = _bgColor;
	return color;
}

Color& vgui2::Panel::GetFgColor(Color& color)
{
	color = _fgColor;
	return color;
}

void vgui2::Panel::InternalPerformLayout()
{
	m_bInPerformLayout = true;
	_needsLayout = false;

	PerformLayout();

	m_bInPerformLayout = false;
}

void vgui2::Panel::PerformLayout()
{
	// Nothing
}

vgui::PanelMap_t * vgui2::Panel::GetPanelMap()
{
	NOT_IMPLEMENTED;
	return nullptr;
}

void vgui2::Panel::InvalidateLayout(bool layoutNow, bool reloadScheme)
{	
	_needsLayout = true;
	if (reloadScheme)
	{
		_needsSchemeUpdate = true;

		for (int i = 0; i < GetChildCount(); i++)
		{
			auto childPanel =  vgui2::ipanel()->GetPanel((VPANEL)GetChild(i), GetControlsModuleName());
			if (childPanel)
			{
				childPanel->InvalidateLayout(layoutNow, reloadScheme);
			}
		}

		PerformApplySchemeSettings();
	}
	if (layoutNow)
	{
		InternalPerformLayout();
		Repaint();
	}
}

bool vgui2::Panel::IsCursorNone()
{
	CHECK_REQUIRED;

	HCursor cursor = GetCursor();

	if (!cursor)
	{
		return true;
	}

	return false;
}

//-----------------------------------------------------------------------------
// Purpose: returns true if the cursor is currently over the panel
// Output : Returns true on success, false on failure.
//-----------------------------------------------------------------------------
bool vgui2::Panel::IsCursorOver(void)
{
	CHECK_REQUIRED;

	int x, y;
	input()->GetCursorPos(x, y);
	return IsWithin(x, y);
}

//-----------------------------------------------------------------------------
// Purpose: Called when a panel receives a command message from another panel
//-----------------------------------------------------------------------------
void vgui2::Panel::OnCommand(const char *command)
{
	CHECK_REQUIRED;

	if (!Q_stricmp("performlayout", command))
	{
		InvalidateLayout(false, false);
	}
	else if (!Q_stricmp("reloadscheme", command))
	{
		InvalidateLayout(false, true);
	}
	else
	{
		// if noone else caught this, pass along to the listeners
		// (this is useful for generic dialogs - otherwise, commands just get ignored)
		KeyValues *msg = new KeyValues(command);
		PostActionSignal(msg);
	}
}

//-----------------------------------------------------------------------------
// Purpose: panel gained focus message
//-----------------------------------------------------------------------------
void vgui2::Panel::OnSetFocus()
{
	Repaint();
}

//-----------------------------------------------------------------------------
// Purpose: panel lost focus message
//-----------------------------------------------------------------------------
void vgui2::Panel::OnKillFocus()
{
	Repaint();
}

//-----------------------------------------------------------------------------
// Purpose: Sets the object up to be deleted next frame
//-----------------------------------------------------------------------------
void vgui2::Panel::MarkForDeletion()
{
	NOT_IMPLEMENTED;
	if (_markedForDeletion)
		return;

	_markedForDeletion = 1;
	_autoDelete = 0;
	if (ivgui()->IsRunning())
	{
		ivgui()->MarkPanelForDeletion(GetVPanel());
	}

	else
	{
		//(*((void(__cdecl **)(vgui2::Panel *const))this->_vptr_IClientPanel + 29))(this);
	}
}

//-----------------------------------------------------------------------------
// Purpose: return true if this object require a perform layout
//-----------------------------------------------------------------------------
bool vgui2::Panel::IsLayoutInvalid()
{
	return _needsLayout;
}


//-----------------------------------------------------------------------------
// Sets the pin corner + resize mode for resizing panels
//-----------------------------------------------------------------------------
void vgui2::Panel::SetAutoResize(AutoResize_e resizeDir)
{
	_autoResizeDirection = resizeDir;
}


//-----------------------------------------------------------------------------
// Sets the pin corner for non-resizing panels
//-----------------------------------------------------------------------------
void vgui2::Panel::SetPinCorner(PinCorner_e pinCorner)
{
	_pinCorner = pinCorner;
}


//-----------------------------------------------------------------------------
// Purpose: data accessor
//-----------------------------------------------------------------------------
vgui2::Panel::PinCorner_e vgui2::Panel::GetPinCorner()
{
	return (PinCorner_e)_pinCorner;
}

//-----------------------------------------------------------------------------
// Purpose: data accessor
//-----------------------------------------------------------------------------
vgui2::Panel::AutoResize_e vgui2::Panel::GetAutoResize()
{
	return (AutoResize_e)_autoResizeDirection;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void vgui2::Panel::ApplySchemeSettings(IScheme *pScheme)
{
	// get colors
	SetFgColor(GetSchemeColor("Panel.FgColor", pScheme));
	SetBgColor(GetSchemeColor("Panel.BgColor", pScheme));

	// mark us as no longer needing scheme settings applied
	_needsSchemeUpdate = 0;

	if (IsBuildGroupEnabled())
	{
		_buildGroup->ApplySchemeSettings(pScheme);
		return;
	}
}

//-----------------------------------------------------------------------------
// Purpose: Checks to see if the panel needs it's scheme info setup
//-----------------------------------------------------------------------------
void vgui2::Panel::PerformApplySchemeSettings()
{
	if (_needsSchemeUpdate)
	{
		IScheme *pScheme = scheme()->GetIScheme(GetScheme());
		if (pScheme) // this should NEVER be null, but if it is bad things would happen in ApplySchemeSettings...
		{
			NOT_IMPLEMENTED;
			ApplySchemeSettings(pScheme);
		}
	}
}

//-----------------------------------------------------------------------------
// Purpose: Loads panel details related to autoresize from the resource info
//-----------------------------------------------------------------------------
#if defined( _DEBUG )
static Panel *lastWarningParent = 0;
#endif

//-----------------------------------------------------------------------------
// Purpose: Loads panel details from the resource info
//-----------------------------------------------------------------------------
void vgui2::Panel::ApplySettings(KeyValues *inResourceData)
{
	/*
	// First restore to default values
	if (_flags.IsFlagSet(NEEDS_DEFAULT_SETTINGS_APPLIED))
	{
		InternalInitDefaultValues(GetAnimMap());
	}

	// Let PanelAnimationVars auto-retrieve settings (we restore defaults above
	//  since a script might be missing certain values)
	InternalApplySettings(GetAnimMap(), inResourceData);

	// clear any alignment flags
	_buildModeFlags &= ~(BUILDMODE_SAVE_XPOS_RIGHTALIGNED
		| BUILDMODE_SAVE_XPOS_CENTERALIGNED
		| BUILDMODE_SAVE_YPOS_BOTTOMALIGNED
		| BUILDMODE_SAVE_YPOS_CENTERALIGNED
		| BUILDMODE_SAVE_WIDE_FULL
		| BUILDMODE_SAVE_TALL_FULL
		| BUILDMODE_SAVE_PROPORTIONAL_TO_PARENT
		| BUILDMODE_SAVE_WIDE_PROPORTIONAL_TALL
		| BUILDMODE_SAVE_TALL_PROPORTIONAL_WIDE
		| BUILDMODE_SAVE_XPOS_PROPORTIONAL_PARENT
		| BUILDMODE_SAVE_YPOS_PROPORTIONAL_PARENT
		| BUILDMODE_SAVE_XPOS_PROPORTIONAL_SELF
		| BUILDMODE_SAVE_YPOS_PROPORTIONAL_SELF);

	// get the position
	int alignScreenWide, alignScreenTall;	// screen dimensions used for pinning in splitscreen
	surface()->GetScreenSize(alignScreenWide, alignScreenTall);

	int screenWide = alignScreenWide;
	int screenTall = alignScreenTall;

	// temporarily remove the override to get the fullscreen dimensions
	if (surface()->IsScreenSizeOverrideActive())
	{
		surface()->ForceScreenSizeOverride(false, 0, 0);
		surface()->GetScreenSize(screenWide, screenTall);

		// restore the override
		surface()->ForceScreenSizeOverride(true, alignScreenWide, alignScreenTall);
	}

	int parentX = 0;
	int parentY = 0;

	// flag to cause windows to get screenWide and screenTall from their parents,
	// this allows children windows to use fill and right/bottom alignment even
	// if their parent does not use the full screen.
	if (inResourceData->GetInt("proportionalToParent", 0) == 1)
	{
		_buildModeFlags |= BUILDMODE_SAVE_PROPORTIONAL_TO_PARENT;
		if (GetParent() != NULL)
		{
			GetParent()->GetBounds(parentX, parentY, alignScreenWide, alignScreenTall);
		}
	}

	// size
	int wide = ComputeWide(inResourceData, alignScreenWide, alignScreenTall, false);
	int tall = ComputeTall(inResourceData, alignScreenWide, alignScreenTall, false);

	int x, y;
	GetPos(x, y);
	const char *xstr = inResourceData->GetString("xpos", NULL);
	const char *ystr = inResourceData->GetString("ypos", NULL);
	_buildModeFlags |= ComputePos(xstr, x, wide, alignScreenWide, true);
	_buildModeFlags |= ComputePos(ystr, y, tall, alignScreenTall, false);


	bool bUsesTitleSafeArea = false;
	int titleSafeWide = 0;
	int titleSafeTall = 0;

	Rect_t excludeEdgeFromTitleSafe;	// if a side is set to != 0, don't title safe relative to that edge
	excludeEdgeFromTitleSafe.x = 0;
	excludeEdgeFromTitleSafe.y = 0;
	excludeEdgeFromTitleSafe.width = 0;
	excludeEdgeFromTitleSafe.height = 0;

	if (IsX360() || panel_test_title_safe.GetBool())
	{
		// "usetitlesafe" "1" - required inner 90%
		// "usetitlesafe" "2" - suggested inner 85%

		int iUseTitleSafeValue = 0;
		if (inResourceData->FindKey("usetitlesafe"))
		{
			iUseTitleSafeValue = inResourceData->GetInt("usetitlesafe");
			bUsesTitleSafeArea = (iUseTitleSafeValue > 0);
		}

		if (bUsesTitleSafeArea)
		{
			titleSafeWide = screenWide * (iUseTitleSafeValue == 1 ? 0.05f : 0.075f);
			titleSafeTall = screenTall * (iUseTitleSafeValue == 1 ? 0.05f : 0.075f);

			// Don't title safe internal boundaries for split screen viewports
			int splitX = 0;
			int splitY = 0;
			vgui::surface()->OffsetAbsPos(splitX, splitY);

			bool bHorizontalSplit = (alignScreenTall != screenTall);
			bool bVerticalSplit = (alignScreenWide != screenWide);

			if (bHorizontalSplit)
			{
				// top or bottom?
				if (splitY != parentY)
				{
					excludeEdgeFromTitleSafe.y = 1;
				}
				else
				{
					excludeEdgeFromTitleSafe.height = 1;
				}
			}

			if (bVerticalSplit)
			{
				// left or right
				if (splitX != parentX)
				{
					excludeEdgeFromTitleSafe.x = 1;
				}
				else
				{
					excludeEdgeFromTitleSafe.width = 1;
				}
			}

			if (_buildModeFlags & BUILDMODE_SAVE_XPOS_RIGHTALIGNED)
			{
				if (!excludeEdgeFromTitleSafe.width)
				{
					x -= titleSafeWide;	// right edge
				}
			}
			else if (_buildModeFlags & BUILDMODE_SAVE_XPOS_CENTERALIGNED)
			{
			}
			else if (!excludeEdgeFromTitleSafe.x)
			{
				x += titleSafeWide;	// left edge
			}

			if (_buildModeFlags & BUILDMODE_SAVE_YPOS_BOTTOMALIGNED)
			{
				if (!excludeEdgeFromTitleSafe.height)
				{
					y -= titleSafeTall;	// bottom edge
				}
			}
			else if (_buildModeFlags & BUILDMODE_SAVE_YPOS_CENTERALIGNED)
			{
			}
			else if (!excludeEdgeFromTitleSafe.y)
			{
				y += titleSafeTall;	// top edge
			}
		}
	}
	SetNavUp(inResourceData->GetString("navUp"));
	SetNavDown(inResourceData->GetString("navDown"));
	SetNavLeft(inResourceData->GetString("navLeft"));
	SetNavRight(inResourceData->GetString("navRight"));
	SetNavToRelay(inResourceData->GetString("navToRelay"));
	SetNavActivate(inResourceData->GetString("navActivate"));
	SetNavBack(inResourceData->GetString("navBack"));

	SetPos(x, y);

	if (inResourceData->FindKey("zpos"))
	{
		SetZPos(inResourceData->GetInt("zpos"));
	}

	if (bUsesTitleSafeArea)
	{
		if (_buildModeFlags & BUILDMODE_SAVE_WIDE_FULL)
		{
			if (!excludeEdgeFromTitleSafe.x)
				wide -= titleSafeWide;

			if (!excludeEdgeFromTitleSafe.width)
				wide -= titleSafeWide;
		}

		if (_buildModeFlags & BUILDMODE_SAVE_TALL_FULL)
		{
			if (!excludeEdgeFromTitleSafe.y)
				tall -= titleSafeTall;

			if (!excludeEdgeFromTitleSafe.height)
				tall -= titleSafeTall;
		}
	}

	SetSize(wide, tall);

	// NOTE: This has to happen after pos + size is set
	ApplyAutoResizeSettings(inResourceData);

	// only get colors if we're ignoring the scheme
	if (inResourceData->GetInt("IgnoreScheme", 0))
	{
		PerformApplySchemeSettings();
	}

	// state
	int state = inResourceData->GetInt("visible", 1);
	if (state == 0)
	{
		SetVisible(false);
	}
	else if (state == 1)
	{
		SetVisible(true);
	}

	SetEnabled(inResourceData->GetInt("enabled", true));

	bool bMouseEnabled = inResourceData->GetInt("mouseinputenabled", true);
	if (!bMouseEnabled)
	{
		SetMouseInputEnabled(false);
	}

	// tab order
	SetTabPosition(inResourceData->GetInt("tabPosition", 0));

	const char *tooltip = inResourceData->GetString("tooltiptext", NULL);
	if (tooltip && *tooltip)
	{
		GetTooltip()->SetText(tooltip);
	}

	// paint background?
	int nPaintBackground = inResourceData->GetInt("paintbackground", -1);
	if (nPaintBackground >= 0)
	{
		SetPaintBackgroundEnabled(nPaintBackground != 0);
	}

	// paint border?
	int nPaintBorder = inResourceData->GetInt("paintborder", -1);
	if (nPaintBorder >= 0)
	{
		SetPaintBorderEnabled(nPaintBorder != 0);
	}

	// border?
	const char *pBorder = inResourceData->GetString("border", "");
	if (*pBorder)
	{
		IScheme *pScheme = scheme()->GetIScheme(GetScheme());
		SetBorder(pScheme->GetBorder(pBorder));
	}

	// check to see if we have a new name assigned
	const char *newName = inResourceData->GetString("fieldName", NULL);
	if (newName)
	{
		// Only slam the name if the new one differs...
		SetName(newName);
	}

	// Automatically add an action signal target if one is specified.  This allows for
	// nested child buttons to add their distant parents as action signal targets.
	int nActionSignalLevel = inResourceData->GetInt("actionsignallevel", -1);
	if (nActionSignalLevel != -1)
	{
		Panel *pActionSignalTarget = this;
		while (nActionSignalLevel--)
		{
			pActionSignalTarget = pActionSignalTarget->GetParent();
		}
		AddActionSignalTarget(pActionSignalTarget);
	}

	// check to see if we need to render to the frame buffer even if 
	// stereo mode is trying to render all of the ui to a render target
	m_bForceStereoRenderToFrameBuffer = inResourceData->GetBool("ForceStereoRenderToFrameBuffer", false);

	//=============================================================================
	// HPE_BEGIN:
	// [pfreese] Support for reading rounded corner flags
	//=============================================================================
	int roundedCorners = inResourceData->GetInt("RoundedCorners", -1);
	if (roundedCorners >= 0)
	{
		m_roundedCorners = roundedCorners;
	}
	//=============================================================================
	// HPE_END
	//=============================================================================

	const char *pszSiblingName = inResourceData->GetString("pin_to_sibling", NULL);
	PinCorner_e pinOurCornerToSibling = GetPinCornerFromString(inResourceData->GetString("pin_corner_to_sibling", NULL));
	PinCorner_e pinSiblingCorner = GetPinCornerFromString(inResourceData->GetString("pin_to_sibling_corner", NULL));
	PinToSibling(pszSiblingName, pinOurCornerToSibling, pinSiblingCorner);


	// Allow overriding of colors. Used mostly by HUD elements, where scheme color usage is often undesired.
	IScheme *pScheme = vgui::scheme()->GetIScheme(GetScheme());
	for (int i = 0; i < m_OverridableColorEntries.Count(); i++)
	{
		// Need to ensure the key exists, so we don't overwrite existing colors when it's not set.
		if (inResourceData->FindKey(m_OverridableColorEntries[i].m_pszScriptName, false))
		{
			// Get the color as a string - test whether it is an actual color or a reference to a scheme color
			const char *pColorStr = inResourceData->GetString(m_OverridableColorEntries[i].m_pszScriptName);
			Color &clrDest = m_OverridableColorEntries[i].m_colFromScript;
			if (pColorStr[0] == '.' || isdigit(pColorStr[0]))
			{
				float r = 0.0f, g = 0.0f, b = 0.0f, a = 0.0f;
				sscanf(pColorStr, "%f %f %f %f", &r, &g, &b, &a);
				clrDest[0] = (unsigned char)r;
				clrDest[1] = (unsigned char)g;
				clrDest[2] = (unsigned char)b;
				clrDest[3] = (unsigned char)a;
			}
			else
			{
				// First character wasn't a digit or a decimal - do a scheme color lookup
				clrDest = pScheme->GetColor(pColorStr, Color(255, 255, 255, 255));
			}

			(*m_OverridableColorEntries[i].m_pColor) = m_OverridableColorEntries[i].m_colFromScript;
			m_OverridableColorEntries[i].m_bOverridden = true;
		}
	}

	const char *pKeyboardInputEnabled = inResourceData->GetString("keyboardinputenabled", NULL);
	if (pKeyboardInputEnabled && pKeyboardInputEnabled[0])
	{
		SetKeyBoardInputEnabled(atoi(pKeyboardInputEnabled));
	}

	OnChildSettingsApplied(inResourceData, this);
	*/
}

//-----------------------------------------------------------------------------
// Purpose: Saves out a resource description of this panel
//-----------------------------------------------------------------------------
void vgui2::Panel::GetSettings(KeyValues *outResourceData)
{
	NOT_IMPLEMENTED;
	/*
	// control class name (so it can be recreated later if needed)
	outResourceData->SetString("ControlName", GetClassName());

	// name
	outResourceData->SetString("fieldName", _panelName);

	// positioning
	int screenWide, screenTall;
	surface()->GetScreenSize(screenWide, screenTall);
	int x, y;
	GetPos(x, y);
	if (IsProportional())
	{
		x = scheme()->GetProportionalNormalizedValueEx(GetScheme(), x);
		y = scheme()->GetProportionalNormalizedValueEx(GetScheme(), y);
	}
	// correct for alignment
	if (_buildModeFlags & BUILDMODE_SAVE_XPOS_RIGHTALIGNED)
	{
		x = screenWide - x;
		char xstr[32];
		Q_snprintf(xstr, sizeof(xstr), "r%d", x);
		outResourceData->SetString("xpos", xstr);
	}
	else if (_buildModeFlags & BUILDMODE_SAVE_XPOS_CENTERALIGNED)
	{
		x = (screenWide / 2) + x;
		char xstr[32];
		Q_snprintf(xstr, sizeof(xstr), "c%d", x);
		outResourceData->SetString("xpos", xstr);
	}
	else
	{
		outResourceData->SetInt("xpos", x);
	}
	if (_buildModeFlags & BUILDMODE_SAVE_YPOS_BOTTOMALIGNED)
	{
		y = screenTall - y;
		char ystr[32];
		Q_snprintf(ystr, sizeof(ystr), "r%d", y);
		outResourceData->SetString("ypos", ystr);
	}
	else if (_buildModeFlags & BUILDMODE_SAVE_YPOS_CENTERALIGNED)
	{
		y = (screenTall / 2) + y;
		char ystr[32];
		Q_snprintf(ystr, sizeof(ystr), "c%d", y);
		outResourceData->SetString("ypos", ystr);
	}
	else
	{
		outResourceData->SetInt("ypos", y);
	}
	if (m_pTooltips)
	{
		if (strlen(m_pTooltips->GetText()) > 0)
		{
			outResourceData->SetString("tooltiptext", m_pTooltips->GetText());
		}
	}
	int wide, tall;
	GetSize(wide, tall);
	if (IsProportional())
	{
		wide = scheme()->GetProportionalNormalizedValueEx(GetScheme(), wide);
		tall = scheme()->GetProportionalNormalizedValueEx(GetScheme(), tall);
	}

	int z = ipanel()->GetZPos(GetVPanel());
	if (z)
	{
		outResourceData->SetInt("zpos", z);
	}

	// Correct for alignment
	if (_buildModeFlags & BUILDMODE_SAVE_WIDE_FULL)
	{
		wide = screenWide - wide;
		char wstr[32];
		Q_snprintf(wstr, sizeof(wstr), "f%d", wide);
		outResourceData->SetString("wide", wstr);
	}
	else
	{
		outResourceData->SetInt("wide", wide);
	}
	outResourceData->SetInt("tall", tall);

	outResourceData->SetInt("AutoResize", GetAutoResize());
	outResourceData->SetInt("PinCorner", GetPinCorner());

	//=============================================================================
	// HPE_BEGIN:
	// [pfreese] Support for writing out rounded corner flags
	//=============================================================================
	outResourceData->SetInt("RoundedCorners", m_roundedCorners);
	//=============================================================================
	// HPE_END
	//=============================================================================

	outResourceData->SetString("pin_to_sibling", _pinToSibling);
	outResourceData->SetInt("pin_corner_to_sibling", _pinCornerToSibling);
	outResourceData->SetInt("pin_to_sibling_corner", _pinToSiblingCorner);


	// state
	outResourceData->SetInt("visible", IsVisible());
	outResourceData->SetInt("enabled", IsEnabled());

	outResourceData->SetInt("tabPosition", GetTabPosition());

	for (int i = 0; i < m_OverridableColorEntries.Count(); i++)
	{
		if (m_OverridableColorEntries[i].m_bOverridden)
		{
			outResourceData->SetColor(m_OverridableColorEntries[i].m_pszScriptName, m_OverridableColorEntries[i].m_colFromScript);
		}
	}
	*/
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
Color vgui2::Panel::GetSchemeColor(const char *keyName, IScheme *pScheme)
{
	return pScheme->GetColor(keyName, Color(255, 255, 255, 255));
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
Color vgui2::Panel::GetSchemeColor(const char *keyName, Color defaultColor, IScheme *pScheme)
{
	return pScheme->GetColor(keyName, defaultColor);
}

//-----------------------------------------------------------------------------
// Purpose: Returns a string description of the panel fields for use in the UI
//-----------------------------------------------------------------------------
const char *vgui2::Panel::GetDescription(void)
{
	static const char *panelDescription = "string fieldName, int xpos, int ypos, int wide, int tall, bool visible, bool enabled, int tabPosition, corner pinCorner, autoresize autoResize, string tooltiptext";
	return panelDescription;
}

//-----------------------------------------------------------------------------
// Purpose: user configuration settings
//			this is used for any control details the user wants saved between sessions
//			eg. dialog positions, last directory opened, list column width
//-----------------------------------------------------------------------------
void vgui2::Panel::ApplyUserConfigSettings(KeyValues *userConfig)
{
}

//-----------------------------------------------------------------------------
// Purpose: returns user config settings for this control
//-----------------------------------------------------------------------------
void vgui2::Panel::GetUserConfigSettings(KeyValues *userConfig)
{
}

//-----------------------------------------------------------------------------
// Purpose: optimization, return true if this control has any user config settings
//-----------------------------------------------------------------------------
bool vgui2::Panel::HasUserConfigSettings()
{
	return false;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void vgui2::Panel::InternalInvalidateLayout()
{
	InvalidateLayout(false, false);
}

//-----------------------------------------------------------------------------
// Purpose: called whenever the panel moves
//-----------------------------------------------------------------------------
void vgui2::Panel::OnMove()
{
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void vgui2::Panel::InternalMove()
{
	OnMove();
	for (int i = 0; i<GetChildCount(); i++)
	{
		// recursively apply to all children
		GetChild(i)->OnMove();
	}
}

//-----------------------------------------------------------------------------
// Purpose: empty function
//-----------------------------------------------------------------------------
void vgui2::Panel::OnTick()
{
}

//-----------------------------------------------------------------------------
// Purpose: versioning
//-----------------------------------------------------------------------------
void *vgui2::Panel::QueryInterface(EInterfaceID id)
{
	if (id == ICLIENTPANEL_STANDARD_INTERFACE)
	{
		return this;
	}

	return NULL;
}

vgui2::Panel * vgui2::Panel::GetPanel()
{
	NOT_IMPLEMENTED;
	return nullptr;
}

/*
//-----------------------------------------------------------------------------
// Purpose: Map all the base messages to functions
//			ordering from most -> least used improves speed
//-----------------------------------------------------------------------------
vgui::MessageMapItem_t vgui2::Panel::m_MessageMap[] =
{
	MAP_MESSAGE_INT(Panel, "RequestFocus", RequestFocus, "direction")
};

// IMPLEMENT_PANELMAP( Panel, NULL )
vgui::PanelMap_t vgui2::Panel::m_PanelMap = { vgui2::Panel::m_MessageMap, ARRAYSIZE(vgui2::Panel::m_MessageMap), "Panel", NULL };
vgui::PanelMap_t *vgui2::Panel::GetPanelMap(void) { return &m_PanelMap; }
*/
//-----------------------------------------------------------------------------
// Purpose: !! Soon to replace existing prepare panel map
//-----------------------------------------------------------------------------
void PreparePanelMessageMap(vgui::PanelMessageMap *panelMap)
{
	NOT_IMPLEMENTED;
	/*
	// iterate through the class hierarchy message maps
	while (panelMap != NULL && !panelMap->processed)
	{
		// hash message map strings into symbols
		for (int i = 0; i < panelMap->entries.Count(); i++)
		{
			MessageMapItem_t *item = &panelMap->entries[i];

			if (item->name)
			{
				item->nameSymbol = KeyValuesSystem()->GetSymbolForString(item->name);
			}
			else
			{
				item->nameSymbol = INVALID_KEY_SYMBOL;
			}
			if (item->firstParamName)
			{
				item->firstParamSymbol = KeyValuesSystem()->GetSymbolForString(item->firstParamName);
			}
			else
			{
				item->firstParamSymbol = INVALID_KEY_SYMBOL;
			}
			if (item->secondParamName)
			{
				item->secondParamSymbol = KeyValuesSystem()->GetSymbolForString(item->secondParamName);
			}
			else
			{
				item->secondParamSymbol = INVALID_KEY_SYMBOL;
			}
		}

		panelMap->processed = true;
		panelMap = panelMap->baseMap;
	}
	*/	
}



//-----------------------------------------------------------------------------
// Purpose: Handles a message
//			Dispatches the message to a set of message maps
//-----------------------------------------------------------------------------
void vgui2::Panel::OnMessage(const KeyValues *params, VPANEL ifromPanel)
{
	NOT_IMPLEMENTED;
	/*
	vgui::PanelMessageMap *panelMap = GetMessageMap();
	bool bFound = false;
	int iMessageName = params->GetNameSymbol();

	if (!panelMap->processed)
	{
		PreparePanelMessageMap(panelMap);
	}

	// iterate through the class hierarchy message maps
	for (; panelMap != NULL && !bFound; panelMap = panelMap->baseMap)
	{
#if defined( _DEBUG )
		//		char const *className = panelMap->pfnClassName();
		//		NOTE_UNUSED( className );
#endif

		// iterate all the entries in the panel map
		for (int i = 0; i < panelMap->entries.Count(); i++)
		{
			vgui::MessageMapItem_t *pMap = &panelMap->entries[i];

			if (iMessageName == pMap->nameSymbol)
			{
				bFound = true;

				switch (pMap->numParams)
				{
				case 0:
				{
					//(this->*(pMap->func))();
					break;
				}

				case 1:
				{
					KeyValues *param1 = params->FindKey(pMap->firstParamSymbol);
					if (!param1)
					{
						param1 = const_cast<KeyValues *>(params);
					}

					switch (pMap->firstParamType)
					{
					case DATATYPE_INT:
						typedef void (vgui2::Panel::*MessageFunc_Int_t)(int);
						(this->*((MessageFunc_Int_t)pMap->func))(param1->GetInt());
						break;

					case DATATYPE_UINT64:
						typedef void (vgui2::Panel::*MessageFunc_Uin64_t)(uint64);
						(this->*((MessageFunc_Uin64_t)pMap->func))(param1->GetUint64());
						break;

					case DATATYPE_PTR:
						typedef void (vgui2::Panel::*MessageFunc_Ptr_t)(void *);
						(this->*((MessageFunc_Ptr_t)pMap->func))(param1->GetPtr());
						break;

					case DATATYPE_FLOAT:
						typedef void (vgui2::Panel::*MessageFunc_Float_t)(float);
						(this->*((MessageFunc_Float_t)pMap->func))(param1->GetFloat());
						break;

					case DATATYPE_CONSTCHARPTR:
						typedef void (vgui2::Panel::*MessageFunc_CharPtr_t)(const char *);
						(this->*((MessageFunc_CharPtr_t)pMap->func))(param1->GetString());
						break;

					case DATATYPE_CONSTWCHARPTR:
						typedef void (vgui2::Panel::*MessageFunc_WCharPtr_t)(const wchar_t *);
						(this->*((MessageFunc_WCharPtr_t)pMap->func))(param1->GetWString());
						break;

					case DATATYPE_KEYVALUES:
						typedef void (vgui2::Panel::*MessageFunc_KeyValues_t)(KeyValues *);
						if (pMap->firstParamName)
						{
							(this->*((MessageFunc_KeyValues_t)pMap->func))((KeyValues *)param1->GetPtr());
						}
						else
						{
							// no param set, so pass in the whole thing
							(this->*((MessageFunc_KeyValues_t)pMap->func))(const_cast<KeyValues *>(params));
						}
						break;

					default:
						Assert(!("No handler for vgui message function"));
						break;
					}
					break;
				}

				case 2:
				{
					KeyValues *param1 = params->FindKey(pMap->firstParamSymbol);
					if (!param1)
					{
						param1 = const_cast<KeyValues *>(params);
					}
					KeyValues *param2 = params->FindKey(pMap->secondParamSymbol);
					if (!param2)
					{
						param2 = const_cast<KeyValues *>(params);
					}

					if ((DATATYPE_INT == pMap->firstParamType) && (DATATYPE_INT == pMap->secondParamType))
					{
						typedef void (vgui2::Panel::*MessageFunc_IntInt_t)(int, int);
						(this->*((MessageFunc_IntInt_t)pMap->func))(param1->GetInt(), param2->GetInt());
					}
					else if ((DATATYPE_PTR == pMap->firstParamType) && (DATATYPE_INT == pMap->secondParamType))
					{
						typedef void (vgui2::Panel::*MessageFunc_PtrInt_t)(void *, int);
						(this->*((MessageFunc_PtrInt_t)pMap->func))(param1->GetPtr(), param2->GetInt());
					}
					else if ((DATATYPE_CONSTCHARPTR == pMap->firstParamType) && (DATATYPE_INT == pMap->secondParamType))
					{
						typedef void (vgui2::Panel::*MessageFunc_ConstCharPtrInt_t)(const char *, int);
						(this->*((MessageFunc_ConstCharPtrInt_t)pMap->func))(param1->GetString(), param2->GetInt());
					}
					else if ((DATATYPE_CONSTCHARPTR == pMap->firstParamType) && (DATATYPE_CONSTCHARPTR == pMap->secondParamType))
					{
						typedef void (vgui2::Panel::*MessageFunc_ConstCharPtrConstCharPtr_t)(const char *, const char *);
						(this->*((MessageFunc_ConstCharPtrConstCharPtr_t)pMap->func))(param1->GetString(), param2->GetString());
					}
					else if ((DATATYPE_INT == pMap->firstParamType) && (DATATYPE_CONSTCHARPTR == pMap->secondParamType))
					{
						typedef void (vgui2::Panel::*MessageFunc_IntConstCharPtr_t)(int, const char *);
						(this->*((MessageFunc_IntConstCharPtr_t)pMap->func))(param1->GetInt(), param2->GetString());
					}
					else if ((DATATYPE_PTR == pMap->firstParamType) && (DATATYPE_CONSTCHARPTR == pMap->secondParamType))
					{
						typedef void (vgui2::Panel::*MessageFunc_PtrConstCharPtr_t)(void *, const char *);
						(this->*((MessageFunc_PtrConstCharPtr_t)pMap->func))(param1->GetPtr(), param2->GetString());
					}
					else if ((DATATYPE_PTR == pMap->firstParamType) && (DATATYPE_CONSTWCHARPTR == pMap->secondParamType))
					{
						typedef void (vgui2::Panel::*MessageFunc_PtrConstCharPtr_t)(void *, const wchar_t *);
						(this->*((MessageFunc_PtrConstCharPtr_t)pMap->func))(param1->GetPtr(), param2->GetWString());
					}
					else if ((DATATYPE_HANDLE == pMap->firstParamType) && (DATATYPE_CONSTCHARPTR == pMap->secondParamType))
					{
						typedef void (vgui2::Panel::*MessageFunc_HandleConstCharPtr_t)(VPANEL, const char *);
						VPANEL vp = ivgui()->HandleToPanel(param1->GetInt());
						(this->*((MessageFunc_HandleConstCharPtr_t)pMap->func))(vp, param2->GetString());
					}
					else if ((DATATYPE_HANDLE == pMap->firstParamType) && (DATATYPE_CONSTWCHARPTR == pMap->secondParamType))
					{
						typedef void (vgui2::Panel::*MessageFunc_HandleConstCharPtr_t)(VPANEL, const wchar_t *);
						VPANEL vp = ivgui()->HandleToPanel(param1->GetInt());
						(this->*((MessageFunc_HandleConstCharPtr_t)pMap->func))(vp, param2->GetWString());
					}
					else
					{
						// the message isn't handled
						ivgui()->DPrintf("Message '%s', sent to '%s', has invalid parameter types\n", params->GetName(), GetName());
					}
					break;
				}

				default:
					Assert(!("Invalid number of parameters"));
					break;
				}

				// break the loop
				bFound = true;
				break;
			}
		}
	}

	if (!bFound)
	{
		OnOldMessage(const_cast<KeyValues *>(params), ifromPanel);
	}
	*/
}

void vgui2::Panel::OnOldMessage(KeyValues *params, VPANEL ifromPanel)
{
	NOT_IMPLEMENTED;
	/*
	bool bFound = false;
	// message map dispatch
	int iMessageName = params->GetNameSymbol();

	PanelMap_t *panelMap = GetPanelMap();
	if (!panelMap->processed)
	{
		PreparePanelMap(panelMap);
	}

	// iterate through the class hierarchy message maps
	for (; panelMap != NULL && !bFound; panelMap = panelMap->baseMap)
	{
		vgui::MessageMapItem_t *pMessageMap = panelMap->dataDesc;

		for (int i = 0; i < panelMap->dataNumFields; i++)
		{
			if (iMessageName == pMessageMap[i].nameSymbol)
			{
				// call the mapped function
				switch (pMessageMap[i].numParams)
				{
				case 2:
					if ((DATATYPE_INT == pMessageMap[i].firstParamType) && (DATATYPE_INT == pMessageMap[i].secondParamType))
					{
						typedef void (vgui2::Panel::*MessageFunc_IntInt_t)(int, int);
						(this->*((MessageFunc_IntInt_t)pMessageMap[i].func))(params->GetInt(pMessageMap[i].firstParamName), params->GetInt(pMessageMap[i].secondParamName));
					}
					else if ((DATATYPE_PTR == pMessageMap[i].firstParamType) && (DATATYPE_INT == pMessageMap[i].secondParamType))
					{
						typedef void (vgui2::Panel::*MessageFunc_PtrInt_t)(void *, int);
						(this->*((MessageFunc_PtrInt_t)pMessageMap[i].func))(params->GetPtr(pMessageMap[i].firstParamName), params->GetInt(pMessageMap[i].secondParamName));
					}
					else if ((DATATYPE_CONSTCHARPTR == pMessageMap[i].firstParamType) && (DATATYPE_INT == pMessageMap[i].secondParamType))
					{
						typedef void (vgui2::Panel::*MessageFunc_ConstCharPtrInt_t)(const char *, int);
						(this->*((MessageFunc_ConstCharPtrInt_t)pMessageMap[i].func))(params->GetString(pMessageMap[i].firstParamName), params->GetInt(pMessageMap[i].secondParamName));
					}
					else if ((DATATYPE_CONSTCHARPTR == pMessageMap[i].firstParamType) && (DATATYPE_CONSTCHARPTR == pMessageMap[i].secondParamType))
					{
						typedef void (vgui2::Panel::*MessageFunc_ConstCharPtrConstCharPtr_t)(const char *, const char *);
						(this->*((MessageFunc_ConstCharPtrConstCharPtr_t)pMessageMap[i].func))(params->GetString(pMessageMap[i].firstParamName), params->GetString(pMessageMap[i].secondParamName));
					}
					else if ((DATATYPE_INT == pMessageMap[i].firstParamType) && (DATATYPE_CONSTCHARPTR == pMessageMap[i].secondParamType))
					{
						typedef void (vgui2::Panel::*MessageFunc_IntConstCharPtr_t)(int, const char *);
						(this->*((MessageFunc_IntConstCharPtr_t)pMessageMap[i].func))(params->GetInt(pMessageMap[i].firstParamName), params->GetString(pMessageMap[i].secondParamName));
					}
					else if ((DATATYPE_PTR == pMessageMap[i].firstParamType) && (DATATYPE_CONSTCHARPTR == pMessageMap[i].secondParamType))
					{
						typedef void (vgui2::Panel::*MessageFunc_PtrConstCharPtr_t)(void *, const char *);
						(this->*((MessageFunc_PtrConstCharPtr_t)pMessageMap[i].func))(params->GetPtr(pMessageMap[i].firstParamName), params->GetString(pMessageMap[i].secondParamName));
					}
					else if ((DATATYPE_PTR == pMessageMap[i].firstParamType) && (DATATYPE_CONSTWCHARPTR == pMessageMap[i].secondParamType))
					{
						typedef void (vgui2::Panel::*MessageFunc_PtrConstCharPtr_t)(void *, const wchar_t *);
						(this->*((MessageFunc_PtrConstCharPtr_t)pMessageMap[i].func))(params->GetPtr(pMessageMap[i].firstParamName), params->GetWString(pMessageMap[i].secondParamName));
					}
					else if ((DATATYPE_HANDLE == pMessageMap[i].firstParamType) && (DATATYPE_CONSTCHARPTR == pMessageMap[i].secondParamType))
					{
						typedef void (vgui2::Panel::*MessageFunc_HandleConstCharPtr_t)(VPANEL, const char *);
						VPANEL vp = ivgui()->HandleToPanel(params->GetInt(pMessageMap[i].firstParamName));
						(this->*((MessageFunc_HandleConstCharPtr_t)pMessageMap[i].func))(vp, params->GetString(pMessageMap[i].secondParamName));
					}
					else if ((DATATYPE_HANDLE == pMessageMap[i].firstParamType) && (DATATYPE_CONSTWCHARPTR == pMessageMap[i].secondParamType))
					{
						typedef void (vgui2::Panel::*MessageFunc_HandleConstCharPtr_t)(VPANEL, const wchar_t *);
						VPANEL vp = ivgui()->HandleToPanel(params->GetInt(pMessageMap[i].firstParamName));
						(this->*((MessageFunc_HandleConstCharPtr_t)pMessageMap[i].func))(vp, params->GetWString(pMessageMap[i].secondParamName));
					}
					else
					{
						// the message isn't handled
						ivgui()->DPrintf("Message '%s', sent to '%s', has invalid parameter types\n", params->GetName(), GetName());
					}
					break;

				case 1:
					switch (pMessageMap[i].firstParamType)
					{
					case DATATYPE_BOOL:
						typedef void (vgui2::Panel::*MessageFunc_Bool_t)(bool);
						(this->*((MessageFunc_Bool_t)pMessageMap[i].func))((bool)params->GetInt(pMessageMap[i].firstParamName));
						break;

					case DATATYPE_CONSTCHARPTR:
						typedef void (vgui2::Panel::*MessageFunc_ConstCharPtr_t)(const char *);
						(this->*((MessageFunc_ConstCharPtr_t)pMessageMap[i].func))((const char *)params->GetString(pMessageMap[i].firstParamName));
						break;

					case DATATYPE_CONSTWCHARPTR:
						typedef void (vgui2::Panel::*MessageFunc_ConstCharPtr_t)(const char *);
						(this->*((MessageFunc_ConstCharPtr_t)pMessageMap[i].func))((const char *)params->GetWString(pMessageMap[i].firstParamName));
						break;

					case DATATYPE_INT:
						typedef void (vgui2::Panel::*MessageFunc_Int_t)(int);
						(this->*((MessageFunc_Int_t)pMessageMap[i].func))(params->GetInt(pMessageMap[i].firstParamName));
						break;

					case DATATYPE_FLOAT:
						typedef void (vgui2::Panel::*MessageFunc_Float_t)(float);
						(this->*((MessageFunc_Float_t)pMessageMap[i].func))(params->GetFloat(pMessageMap[i].firstParamName));
						break;

					case DATATYPE_PTR:
						typedef void (vgui2::Panel::*MessageFunc_Ptr_t)(void *);
						(this->*((MessageFunc_Ptr_t)pMessageMap[i].func))((void *)params->GetPtr(pMessageMap[i].firstParamName));
						break;

					case DATATYPE_HANDLE:
					{
						typedef void (vgui2::Panel::*MessageFunc_Ptr_t)(void *);
						VPANEL vp = ivgui()->HandleToPanel(params->GetInt(pMessageMap[i].firstParamName));
						Panel *panel = ipanel()->GetPanel(vp, GetModuleName());
						(this->*((MessageFunc_Ptr_t)pMessageMap[i].func))((void *)panel);
					}
					break;

					case DATATYPE_KEYVALUES:
						typedef void (vgui2::Panel::*MessageFunc_KeyValues_t)(KeyValues *);
						if (pMessageMap[i].firstParamName)
						{
							(this->*((MessageFunc_KeyValues_t)pMessageMap[i].func))((KeyValues *)params->GetPtr(pMessageMap[i].firstParamName));
						}
						else
						{
							(this->*((MessageFunc_KeyValues_t)pMessageMap[i].func))(params);
						}
						break;

					default:
						// the message isn't handled
						ivgui()->DPrintf("Message '%s', sent to '%s', has an invalid parameter type\n", params->GetName(), GetName());
						break;
					}

					break;

				default:
					(this->*(pMessageMap[i].func))();
					break;
				};

				// break the loop
				bFound = true;
				break;
			}
		}
	}

	// message not handled
	// debug code
	if (!bFound)
	{
		static int s_bDebugMessages = -1;
		if (s_bDebugMessages == -1)
		{
			s_bDebugMessages = CommandLine()->FindParm("-vguimessages") ? 1 : 0;
		}
		if (s_bDebugMessages == 1)
		{
			ivgui()->DPrintf("Message '%s' not handled by panel '%s'\n", params->GetName(), GetName());
		}
	}
	*/
}

//-----------------------------------------------------------------------------
// Purpose: Safe call to get info from child panel by name
//-----------------------------------------------------------------------------
bool vgui2::Panel::RequestInfoFromChild(const char *childName, KeyValues *outputData)
{
	Panel *panel = FindChildByName(childName, false);
	if (panel)
	{
		return panel->RequestInfo(outputData);
	}
	return false;
}

//-----------------------------------------------------------------------------
// Purpose: Posts a message
//-----------------------------------------------------------------------------
void vgui2::Panel::PostMessage(Panel *target, KeyValues *message, float delay)
{
	ivgui()->PostMessage(target->GetVPanel(), message, GetVPanel(), delay);
}

void vgui2::Panel::PostMessage(VPANEL target, KeyValues *message, float delaySeconds)
{
	ivgui()->PostMessage(target, message, GetVPanel(), delaySeconds);
}

//-----------------------------------------------------------------------------
// Purpose: Safe call to post a message to a child by name
//-----------------------------------------------------------------------------
void vgui2::Panel::PostMessageToChild(const char *childName, KeyValues *message)
{
	CHECK_REQUIRED;

	Panel *panel = FindChildByName(childName, false);
	if (panel)
	{
		ivgui()->PostMessage(panel->GetVPanel(), message, GetVPanel(), 0);
	}
	else
	{
		message->deleteThis();
	}
}

//-----------------------------------------------------------------------------
// Purpose: Requests some information from the panel
//			Look through the message map for the handler
//-----------------------------------------------------------------------------
bool vgui2::Panel::RequestInfo(KeyValues *outputData)
{
	CHECK_REQUIRED;

	if (GetVParent())
	{
		return ipanel()->RequestInfo(GetVParent(), outputData);
	}

	return false;
}

//-----------------------------------------------------------------------------
// Purpose: sets a specified value in the control - inverse of RequestInfo
//-----------------------------------------------------------------------------
bool vgui2::Panel::SetInfo(KeyValues *inputData)
{
	CHECK_REQUIRED;

	// doesn't chain to parent
	return false;
}

//-----------------------------------------------------------------------------
// Purpose: Prepares the hierarchy panel maps for use (with message maps etc)
//-----------------------------------------------------------------------------
void vgui2::Panel::PreparePanelMap(vgui::PanelMap_t *panelMap)
{
	NOT_IMPLEMENTED;
	/*
	
	// iterate through the class hierarchy message maps
	while (panelMap != NULL && !panelMap->processed)
	{
		// fixup cross-dll boundary panel maps
		if (panelMap->baseMap == (PanelMap_t*)0x00000001)
		{
			panelMap->baseMap = &vgui2::Panel::m_PanelMap;
		}

		// hash message map strings into symbols
		for (int i = 0; i < panelMap->dataNumFields; i++)
		{
			MessageMapItem_t *item = &panelMap->dataDesc[i];

			if (item->name)
			{
				item->nameSymbol = KeyValuesSystem()->GetSymbolForString(item->name);
			}
			else
			{
				item->nameSymbol = INVALID_KEY_SYMBOL;
			}
			if (item->firstParamName)
			{
				item->firstParamSymbol = KeyValuesSystem()->GetSymbolForString(item->firstParamName);
			}
			else
			{
				item->firstParamSymbol = INVALID_KEY_SYMBOL;
			}
			if (item->secondParamName)
			{
				item->secondParamSymbol = KeyValuesSystem()->GetSymbolForString(item->secondParamName);
			}
			else
			{
				item->secondParamSymbol = INVALID_KEY_SYMBOL;
			}
		}

		panelMap->processed = true;
		panelMap = panelMap->baseMap;
	}
	*/
}

//-----------------------------------------------------------------------------
// Purpose: Called to delete the panel
//-----------------------------------------------------------------------------
void vgui2::Panel::OnDelete()
{
	CHECK_REQUIRED;

	delete this;
}

//-----------------------------------------------------------------------------
// Purpose: Panel handle implementation
//			Returns a pointer to a valid panel, NULL if the panel has been deleted
//-----------------------------------------------------------------------------
vgui2::Panel *vgui2::PHandle::Get()
{
	if (m_iPanelID != INVALID_PANEL)
	{
		VPANEL panel = ivgui()->HandleToPanel(m_iPanelID);
		if (panel)
		{
			Panel *vguiPanel = ipanel()->GetPanel(panel, GetControlsModuleName());
			return vguiPanel;
		}
	}
	return nullptr;
}

//-----------------------------------------------------------------------------
// Purpose: sets the smart pointer
//-----------------------------------------------------------------------------
vgui2::Panel *vgui2::PHandle::Set(vgui2::Panel *pent)
{
	if (pent)
	{
		m_iPanelID = vgui2::ivgui()->PanelToHandle(pent->GetVPanel());
	}
	else
	{
		m_iPanelID = INVALID_PANEL;
	}
	return pent;
}

//-----------------------------------------------------------------------------
// Purpose: Returns a handle to a valid panel, NULL if the panel has been deleted
//-----------------------------------------------------------------------------
vgui2::VPANEL vgui2::VPanelHandle::Get()
{
	if (m_iPanelID != INVALID_PANEL)
	{
		if (ivgui())
		{
			return ivgui()->HandleToPanel(m_iPanelID);
		}
	}
	return NULL;
}

//-----------------------------------------------------------------------------
// Purpose: sets the smart pointer
//-----------------------------------------------------------------------------
vgui2::VPANEL vgui2::VPanelHandle::Set(vgui2::VPANEL pent)
{
	if (pent)
	{
		m_iPanelID = ivgui()->PanelToHandle(pent);
	}
	else
	{
		m_iPanelID = INVALID_PANEL;
	}
	return pent;
}

//-----------------------------------------------------------------------------
// Purpose: returns a pointer to the tooltip object associated with the panel
//-----------------------------------------------------------------------------
vgui2::BaseTooltip *vgui2::Panel::GetTooltip()
{
	NOT_IMPLEMENTED;
	return nullptr;
	/*
	if (!m_pTooltips)
	{
		m_pTooltips = new TextTooltip(this, NULL);
		m_bToolTipOverridden = false;

		if (IsConsoleStylePanel())
		{
			m_pTooltips->SetEnabled(false);
		}
	}

	return m_pTooltips;*/
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void vgui2::Panel::SetTooltip(BaseTooltip *pToolTip, const char *pszText)
{
	NOT_IMPLEMENTED;
	/*
	if (!m_bToolTipOverridden)
	{
		// Remove the one we made, we're being overridden.
		delete m_pTooltips;
	}

	m_pTooltips = pToolTip;
	m_bToolTipOverridden = true;

	if (_tooltipText)
	{
		delete[] _tooltipText;
		_tooltipText = NULL;
	}

	if (pszText)
	{
		int len = Q_strlen(pszText) + 1;
		_tooltipText = new char[len];
		Q_strncpy(_tooltipText, pszText, len);
	}
	*/
}

//-----------------------------------------------------------------------------
// Purpose: sets the proportional flag on this panel and all it's children
//-----------------------------------------------------------------------------
void vgui2::Panel::SetProportional(bool state)
{
	// only do something if the state changes
	if (state != m_bProportional)
	{
		m_bProportional = state;

		for (int i = 0; i < GetChildCount(); i++)
		{
			// recursively apply to all children
			GetChild(i)->SetProportional(IsProportional());
		}
	}
	InvalidateLayout(false, false);
}


void vgui2::Panel::SetKeyBoardInputEnabled(bool state)
{
	CHECK_REQUIRED;

	ipanel()->SetKeyBoardInputEnabled(GetVPanel(), state);
	for (int i = 0; i < GetChildCount(); i++)
	{
		Panel *child = GetChild(i);
		if (!child)
		{
			continue;
		}
		child->SetKeyBoardInputEnabled(state);
	}

	// If turning off keyboard input enable, then make sure
	// this panel is not the current key focus of a parent panel
	if (!state)
	{
		Panel *pParent = GetParent();
		if (pParent)
		{
			if (pParent->GetCurrentKeyFocus() == GetVPanel())
			{
				pParent->RequestFocusNext(GetVPanel());
			}
		}
	}
}

void vgui2::Panel::SetMouseInputEnabled(bool state)
{
	NOT_IMPLEMENTED;
	ipanel()->SetMouseInputEnabled(GetVPanel(), state);
	/*	for(int i=0;i<GetChildCount();i++)
	{
	GetChild(i)->SetMouseInput(state);
	}
	vgui::surface()->CalculateMouseVisible();
	*/
}

bool vgui2::Panel::IsKeyBoardInputEnabled()
{
	return ipanel()->IsKeyBoardInputEnabled(GetVPanel());
}

bool vgui2::Panel::IsMouseInputEnabled()
{
	return ipanel()->IsMouseInputEnabled(GetVPanel());
}


//-----------------------------------------------------------------------------
// Purpose: Utility class for handling message map allocation
//-----------------------------------------------------------------------------
class CPanelMessageMapDictionary
{
public:
	CPanelMessageMapDictionary() : m_PanelMessageMapPool(sizeof(vgui::PanelMessageMap), 32)
	{
		m_MessageMaps.RemoveAll();
	}

	vgui::PanelMessageMap	*FindOrAddPanelMessageMap(char const *className);
	vgui::PanelMessageMap	*FindPanelMessageMap(char const *className);
private:

	struct PanelMessageMapDictionaryEntry
	{
		vgui::PanelMessageMap *map;
	};

	char const *StripNamespace(char const *className);

	CUtlDict< PanelMessageMapDictionaryEntry, int > m_MessageMaps;
	CMemoryPool m_PanelMessageMapPool;
};

char const *CPanelMessageMapDictionary::StripNamespace(char const *className)
{
	if (!Q_strnicmp(className, "vgui::", 6))
	{
		return className + 6;
	}
	return className;
}

//-----------------------------------------------------------------------------
// Purpose: Find but don't add mapping
//-----------------------------------------------------------------------------
vgui::PanelMessageMap *CPanelMessageMapDictionary::FindPanelMessageMap(char const *className)
{
	int lookup = m_MessageMaps.Find(StripNamespace(className));
	if (lookup != m_MessageMaps.InvalidIndex())
	{
		return m_MessageMaps[lookup].map;
	}
	return NULL;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
vgui::PanelMessageMap *CPanelMessageMapDictionary::FindOrAddPanelMessageMap(char const *className)
{
	vgui::PanelMessageMap *map = FindPanelMessageMap(className);
	if (map)
		return map;

	PanelMessageMapDictionaryEntry entry;
	// use the alloc in place method of new
	entry.map = new vgui::PanelMessageMap;// (m_PanelMessageMapPool.Alloc(sizeof(vgui::PanelMessageMap))) vgui::PanelMessageMap;
	Construct(entry.map);
	m_MessageMaps.Insert(StripNamespace(className), entry);
	return entry.map;
}

CPanelMessageMapDictionary& GetPanelMessageMapDictionary()
{
	static CPanelMessageMapDictionary dictionary;
	return dictionary;
}



namespace vgui
{
	CPanelMessageMapDictionary& GetPanelMessageMapDictionary()
	{
		static CPanelMessageMapDictionary dictionary;
		return dictionary;
	}
	//-----------------------------------------------------------------------------
	// Purpose: 
	//-----------------------------------------------------------------------------
	PanelMessageMap *FindOrAddPanelMessageMap(char const *className)
	{
		return GetPanelMessageMapDictionary().FindOrAddPanelMessageMap(className);
	}

	//-----------------------------------------------------------------------------
	// Purpose: Find but don't add mapping
	//-----------------------------------------------------------------------------
	PanelMessageMap *FindPanelMessageMap(char const *className)
	{
		return GetPanelMessageMapDictionary().FindPanelMessageMap(className);
	}
}
