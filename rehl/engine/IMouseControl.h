#pragma once

class IMouseControl
{
public:
	virtual bool VGUI2MouseControl() = 0;
	virtual void SetVGUI2MouseControl(bool state) = 0;

	IMouseControl* getMouseControl() { return this; }
};