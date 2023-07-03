#include <smsdk_ext.h>

#include "CPluginListenerV1.h"

namespace nExTranslator {

    CPluginListenerV1::CPluginListenerV1(const Container& container) :
        m_phrCollections(new Container {container})
    {   }

    CPluginListenerV1::~CPluginListenerV1() {
        for (const auto &item: *m_phrCollections)
            if(item.second != nullptr)
                item.second->Destroy();

        delete m_phrCollections;
    }

    void CPluginListenerV1::OnPluginLoaded(SourceMod::IPlugin *plugin) {

        SourceMod::IPhraseCollection* plCollection;

        const char* plName;
        if(plugin != nullptr
        && (plName = plugin->GetFilename())
        && m_phrCollections->count(std::string(plName))
        && (plCollection = plugin->GetPhrases()) != nullptr) {

            SourceMod::IPhraseCollection* buffer = m_phrCollections->at(std::string(plName));

            std::string fileName;
            for(unsigned int i = 0; i < buffer->GetFileCount(); i++)
            {
                fileName = std::string(buffer->GetFile(i)->GetFilename());

                plCollection->AddPhraseFile(fileName.substr(0, fileName.length() - strlen(".txt")).c_str());
            }

        }

        IPluginsListener_V1::OnPluginLoaded(plugin);
    }


} // nExTranslator