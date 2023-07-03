#include "CSMCListener.h"

#include <cstring>

#include "../extension.h"

namespace nExTranslator {

    CSMCListener::CSMCListener(SourceMod::ITranslator *pTranslator) :
        m_Container(),
        m_pTranslator(pTranslator)
    {}

    SourceMod::SMCResult CSMCListener::ReadSMC_NewSection(const SourceMod::SMCStates *states, const char *name) {

        if(m_pTranslator == nullptr)
            return SourceMod::SMCResult_Halt;

        m_pSectionName = isPlugin(name) ? std::string(name) : "";

        if(m_pSectionName.length() && !m_Container.count(m_pSectionName))
        {
            m_Container[m_pSectionName] = m_pTranslator->CreatePhraseCollection();
        }

        return ITextListener_SMC::ReadSMC_NewSection(states, name);
    }

    SourceMod::SMCResult
    CSMCListener::ReadSMC_KeyValue(const SourceMod::SMCStates *states, const char *key, const char *value) {

        if(value != nullptr && m_pSectionName.length())
            m_Container[m_pSectionName]->AddPhraseFile(value);

        return ITextListener_SMC::ReadSMC_KeyValue(states, key, value);
    }

    void CSMCListener::ReadSMC_ParseStart() {

        m_Container.clear();
        m_pSectionName = "";

        ITextListener_SMC::ReadSMC_ParseStart();
    }

    SourceMod::SMCResult CSMCListener::ReadSMC_LeavingSection(const SourceMod::SMCStates *states) {

        m_pSectionName = "";

        return ITextListener_SMC::ReadSMC_LeavingSection(states);
    }

    CSMCListener::container CSMCListener::GetContainer() const {
        return m_Container;
    }

    bool CSMCListener::isPlugin(const std::string &name) {
        return name.substr(name.length() - SMX_FILE_EXT.length(), name.length()) == SMX_FILE_EXT;
    }

} // nExTranslator