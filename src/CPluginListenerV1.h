#ifndef SM_JANSSON_CPLUGINLISTENERV1_H
#define SM_JANSSON_CPLUGINLISTENERV1_H

#include <map>
#include <IPluginSys.h>
#include <ITranslator.h>
#include <string>

namespace nExTranslator {

    class CPluginListenerV1 : public SourceMod::IPluginsListener_V1 {
    public:
        using Container = std::map<std::string, SourceMod::IPhraseCollection*>;

    public:
        CPluginListenerV1(const Container&);
        virtual ~CPluginListenerV1();

    public:
        void OnPluginLoaded(SourceMod::IPlugin *plugin) override;

    public:
        Container* m_phrCollections;
    };

} // nExTranslator

#endif //SM_JANSSON_CPLUGINLISTENERV1_H
