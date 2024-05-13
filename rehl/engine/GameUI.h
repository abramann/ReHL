//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//

#ifndef IGAMEUI_H
#define IGAMEUI_H
#ifdef _WIN32
#pragma once
#endif

#include "interface.h"

// reasons why the user can't connect to a game server
enum ESteamLoginFailure
{
	STEAMLOGINFAILURE_NONE,
	STEAMLOGINFAILURE_BADTICKET,
	STEAMLOGINFAILURE_NOSTEAMLOGIN,
	STEAMLOGINFAILURE_VACBANNED,
	STEAMLOGINFAILURE_LOGGED_IN_ELSEWHERE
};

enum ESystemNotify
{
	SYSTEMNOTIFY_STORAGEDEVICES_CHANGED,
	SYSTEMNOTIFY_USER_SIGNEDIN,
	SYSTEMNOTIFY_USER_SIGNEDOUT,
	SYSTEMNOTIFY_XUIOPENING,
	SYSTEMNOTIFY_XUICLOSED,
	SYSTEMNOTIFY_INVITE_SHUTDOWN,	// Cross-game invite is causing us to shutdown
};

//-----------------------------------------------------------------------------
// Purpose: contains all the functions that the GameUI dll exports
//-----------------------------------------------------------------------------
class IGameUI : public IBaseInterface
{
public:
	virtual ~IGameUI();

	virtual void Initialize( CreateInterfaceFn *factories, int count) = 0;
	virtual void Start(cl_enginefuncs_s *engineFuncs, int interfaceVersion, IBaseSystem *system) = 0;
	virtual void Shutdown() = 0;
	virtual void ActiveGameUI() = 0;
	virtual void DemoGameUI() = 0;
	virtual int HasExclusiveInput() = 0;
	virtual void RunFrame() = 0;
	virtual void ConnectToServer(const char *game, int IP_0, int port) = 0;
	virtual void DisconnectFromServer() = 0;
	virtual void HideGameUI() = 0;
	virtual bool IsGameUIActive() = 0;
	virtual void LoadingStarted(const char *resourceType, const char *resourceName) = 0;
	virtual void LoadingFinished(const char *resourceType, const char *resourceName) = 0;
	virtual void StartProgressBar(const char *progressType, int progressSteps) = 0;
	virtual int ContinueProgressBar(int progressPoint, float progressFraction) = 0;
	virtual void StopProgressBar(bool bError, const char *failureReason, const char *extendedReason) = 0;
	virtual int SetProgressBarStatusText(const char *statusText) = 0;
	virtual void SetSecondaryProgressBar(float progress) = 0;
	virtual void SetSecondaryProgressBarText(const char *statusText) = 0;
	virtual void ValidateCDKey(bool force, bool inConnect) = 0;
	virtual void OnDisconnectFromServer(int eSteamLoginFailure, const char *username) = 0;
};

#define GAMEUI_INTERFACE_VERSION "GameUI007"

extern IGameUI* staticGameUIFuncs;

#endif // IGAMEUI_H
