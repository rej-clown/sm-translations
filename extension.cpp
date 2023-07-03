#include "extension.h"

CExTranslator g_exTranslator;

SMEXT_LINK(&g_exTranslator);

HandleType_t htTCollection=0;
CMTCollectionHandler g_trCollection;

HandleType_t htTFile=0;
CMTFileHandler g_trFile;

bool CExTranslator::SDK_OnLoad(char *error, size_t maxlength, bool late)
{
    sharesys->AddNatives(myself, cmt_collection_natives);
    sharesys->AddNatives(myself, cmt_file_natives);
    sharesys->AddNatives(myself, cmt_translator_natives);
    sharesys->RegisterLibrary(myself, "ex_translations");

    // TODO: Secure & Access
    HandleAccess haPhraseCollection;
    haPhraseCollection.access[HandleAccess_Clone] = 0;
    haPhraseCollection.access[HandleAccess_Delete] = 0;
    haPhraseCollection.access[HandleAccess_Read] = 0;

    htTCollection = handlesys->CreateType("PhraseCollection", &g_trCollection, 0, NULL, &haPhraseCollection, myself->GetIdentity(), NULL);

    HandleAccess haPhraseFile;
    haPhraseFile.access[HandleAccess_Clone] = 0;
    haPhraseFile.access[HandleAccess_Delete] = 0;
    haPhraseFile.access[HandleAccess_Read] = 0;

    htTFile = handlesys->CreateType("PhraseFile", &g_trFile, htTCollection, NULL, &haPhraseFile, myself->GetIdentity(), NULL);

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
    handlesys->RemoveType(htTCollection, myself->GetIdentity());
    handlesys->RemoveType(htTFile, myself->GetIdentity());
}

void CExTranslator::OnCoreMapStart(edict_t *pEdictList, int edictCount, int clientMax) {

    nExTranslator::CSMCListener smcListener { translator };

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