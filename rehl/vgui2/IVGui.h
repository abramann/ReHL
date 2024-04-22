#pragma once

#include "interface.h"
#include "VGui.h"

class KeyValues;

namespace vgui2
{

#define VGUI_IVGUI_INTERFACE_VERSION "VGUI_ivgui006"

	class IVGui : public IBaseInterface
	{
	public:
		virtual ~IVGui();

		virtual bool Init(CreateInterfaceFn *factoryList, int numFactories) = 0;
		virtual void Shutdown() = 0;
		virtual void Start() = 0;
		virtual void Stop() = 0;
		virtual bool IsRunning() = 0;
		virtual void RunFrame() = 0;
		virtual void hutdownMessage(unsigned int shutdownID) = 0;
		virtual vgui2::VPANEL AllocPanel() = 0;
		virtual void FreePanel(vgui2::VPANEL ipanel) = 0;
		virtual void DPrintf(const char *format, ...) = 0;
		virtual void DPrintf2(const char *format, ...) = 0;
		virtual void pewAllActivePanelNames() = 0;
		virtual vgui2::HPanel PanelToHandle(vgui2::VPANEL panel) = 0;
		virtual vgui2::VPANEL HandleToPanel(vgui2::HPanel index) = 0;
		virtual void MarkPanelForDeletion(vgui2::VPANEL panel) = 0;
		virtual void AddTickSignal(vgui2::VPANEL panel, int intervalMilliseconds) = 0;
		virtual void RemoveTickSignal(vgui2::VPANEL panel) = 0;
		virtual void PostMessage(vgui2::VPANEL target, KeyValues *params, vgui2::VPANEL from, float delay) = 0;
		virtual vgui2::HContext CreateContext() = 0;
		virtual void DestroyContext(vgui2::HContext context) = 0;
		virtual void AssociatePanelWithContext(vgui2::HContext context, vgui2::VPANEL pRoot) = 0;
		virtual void ActivateContext(vgui2::HContext context) = 0;
		virtual void SetSleep(bool state) = 0;
		virtual bool GetShouldVGuiControlSleep() = 0;
	};
	
	IVGui* ivgui();
}