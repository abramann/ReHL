#pragma once

#include "interface.h"



namespace vgui2
{

#define CAREERUI_INTERFACE_VERSION "CareerUI001"
	
	class ICareerUI : public IBaseInterface
	{
	public:
		virtual bool IsComplete() = 0;
		virtual bool IsCompletedThisRound() = 0;
		virtual bool IsCompletedThisMatch() = 0;
		virtual const char* GetMap() = 0;
		virtual const char* GetTaskName() = 0;
		virtual const wchar_t*GetLocalizedTaskName() = 0;
		virtual const wchar_t*GetLocalizedTaskNameWithCompletion() = 0;
		virtual void Reset() = 0;
		virtual void Set(bool thisRound) = 0;
		virtual void StartRound() = 0;
		virtual const char* GetWeaponName() const = 0;
		virtual int GetRepeat() const = 0;
		virtual bool MustSurvive() const = 0;
		virtual bool InARow() const = 0;
		virtual int GetPartial() const = 0;
		virtual void SetPartial(int num) = 0;
		virtual bool IsMaxRoundTime() const = 0;
	};
}