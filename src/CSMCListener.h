#ifndef SM_JANSSON_CSMCLISTENER_H
#define SM_JANSSON_CSMCLISTENER_H

#include <map>
#include <ITextParsers.h>
#include <ITranslator.h>
#include <string>

namespace nExTranslator {

    class CSMCListener : public SourceMod::ITextListener_SMC {
    public:
        using container = std::map<std::string, SourceMod::IPhraseCollection*>;

    public:
        CSMCListener(SourceMod::ITranslator*);
        virtual ~CSMCListener() = default;

    public:
        SourceMod::SMCResult ReadSMC_NewSection(const SourceMod::SMCStates *states, const char *name) override;

        SourceMod::SMCResult
        ReadSMC_KeyValue(const SourceMod::SMCStates *states, const char *key, const char *value) override;

        void ReadSMC_ParseStart() override;

        SourceMod::SMCResult ReadSMC_LeavingSection(const SourceMod::SMCStates *states) override;

    public:
        container GetContainer() const;

    protected:
        bool isPlugin(const std::string& name);

    public:
        container m_Container;

        std::string m_pSectionName;

        SourceMod::ITranslator* m_pTranslator;

        const std::string SMX_FILE_EXT = ".smx";
    };

} // nExTranslator

#endif //SM_JANSSON_CSMCLISTENER_H
