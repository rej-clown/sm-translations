#ifndef _INCLUDE_SOURCEMOD_EXTENSION_PROPER_H_
#define _INCLUDE_SOURCEMOD_EXTENSION_PROPER_H_

#include <CPluginListenerV1.h>

#include "smsdk_ext.h"

class CExTranslator : public SDKExtension
{
public:
	virtual bool SDK_OnLoad(char *error, size_t maxlen, bool late);
	virtual void SDK_OnUnload();

public:
    void OnCoreMapStart(edict_t *pEdictList, int edictCount, int clientMax) override;

public:
    char m_extSettings[PLATFORM_MAX_PATH];
    nExTranslator::CPluginListenerV1* m_pPluginListener;
};

extern CExTranslator g_exTranslator;

#endif // _INCLUDE_SOURCEMOD_EXTENSION_PROPER_H_
