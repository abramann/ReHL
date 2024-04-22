#pragma once

#include "interface.h"
#include "archtypes.h"
#include "HTMLCommands.h"

class IHTMLSerializer;
class IHTMLResponses;
class IHTMLResponses_HL1;
class CUtlString;


typedef struct HTMLCommandBuffer_t;

class IHTMLChromeController
{
public:
	virtual ~IHTMLChromeController();

	virtual bool Init(const char *pchHTMLCacheDir, const char *pchCookiePath) = 0;
	virtual void  Shutdown() = 0;
	virtual bool RunFrame() = 0;
	virtual void  SetWebCookie(const char *pchHostname, const char *pchKey, const char *pchValue, const char *pchPath, int nExpires) = 0;
	virtual void  GetWebCookiesForURL(CUtlString *pstrValue, const char *pchURL, const char *pchName) = 0;
	virtual void  SetClientBuildID(uint64 ulBuildID) = 0;
	virtual bool BHasPendingMessages() = 0;
	virtual void  CreateBrowser(IHTMLResponses *pBrowser, bool bPopupWindow, const char *pchUserAgentIdentifier) = 0;
	virtual void  RemoveBrowser(IHTMLResponses *pBrowser) = 0;
	virtual void  WakeThread() = 0;
	virtual HTMLCommandBuffer_t * GetFreeCommandBuffer(EHTMLCommands eCmd, int iBrowser) = 0;
	virtual void  PushCommand(HTMLCommandBuffer_t *pCmd) = 0;
	virtual void  SetCefThreadTargetFrameRate(uint32 nFPS) = 0;
	virtual IHTMLSerializer *reateSerializer(IHTMLResponses_HL1 *pResponseTarget) = 0;
};

#define CHROMEHTML_INTERFACE_VERSION "ChromeHTML_Controller_001"