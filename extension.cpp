#include <natives.h>
#include <dispatches.h>
#include <CSMCListener.h>
#include <types/CHandleTypeManager.h>

#include "extension.h"

using namespace nExTranslator;

CExTranslator g_exTranslator;
SMEXT_LINK(&g_exTranslator);

#define REGISTER_TYPE(manager, type, dispatch, ident) manager.registerType(type, dispatch, 0, nullptr, nullptr, ident)

bool CExTranslator::SDK_OnLoad(char *error, size_t maxlength, bool late)
{
    g_TypeMgr = {};

    sharesys->AddNatives(myself, niPhraseCollection);
    sharesys->AddNatives(myself, niPhraseFile);
    sharesys->AddNatives(myself, niGlobal);
    sharesys->RegisterLibrary(myself, "ex_translations");

    if(!REGISTER_TYPE(g_TypeMgr, "PhraseCollection", new CPhraseCollectionTypeDispatcher(), myself->GetIdentity())
    || !REGISTER_TYPE(g_TypeMgr, "PhraseFile", new CPhraseFileTypeDispatcher(), myself->GetIdentity()))
    {
        sprintf(error, "Something went wrong on type registration");
        return false;
    }

    m_pPluginListener = nullptr;

    smutils->BuildPath(Path_SM, m_extSettings, PLATFORM_MAX_PATH, SMEXT_CONF_PATH);

    if(!libsys->PathExists(m_extSettings))
    {
        sprintf(error, "Path must be exists: %s", m_extSettings);
        return false;
    }

    OnCoreMapStart(nullptr, 0, 0);

    return true;
}

void CExTranslator::SDK_OnUnload()
{
    g_TypeMgr.clear();
}

void CExTranslator::OnCoreMapStart(edict_t *pEdictList, int edictCount, int clientMax) {

    CSMCListener smcListener { translator };

    if(textparsers->ParseFile_SMC(m_extSettings, &smcListener, nullptr) == SMCError_Okay)
    {
        if(m_pPluginListener != nullptr)
        {
            plsys->RemovePluginsListener_V1(m_pPluginListener);

            delete m_pPluginListener;

            m_pPluginListener = nullptr;
        }

        m_pPluginListener = new nExTranslator::CPluginListenerV1(smcListener.GetContainer());

        plsys->AddPluginsListener_V1(m_pPluginListener);
    }

    IExtensionInterface::OnCoreMapStart(pEdictList, edictCount, clientMax);
}

static cell_t CMTRebuildLanguage(IPluginContext *pContext, const cell_t *params)
{
    translator->RebuildLanguageDatabase();
    return 1;
}

const sp_nativeinfo_t cmt_translator_natives[] =
{
    {"RebuildLanguageDatabase", CMTRebuildLanguage},
    {NULL, NULL}
};