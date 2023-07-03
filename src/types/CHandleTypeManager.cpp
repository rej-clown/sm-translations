#include "CHandleTypeManager.h"

#include <smsdk_ext.h>
#include <cstring>

namespace nExTranslator {
    CHandleTypeManager::CHandleTypeManager() :
        m_vecTypes()
    {}

    CHandleTypeManager::~CHandleTypeManager() {

    }

    const std::vector<CHandleType>& CHandleTypeManager::types() const {
        return m_vecTypes;
    }

    const CHandleType& CHandleTypeManager::getByName(const char *name) const {
        const auto& iter =
                std::find_if(m_vecTypes.begin(),m_vecTypes.end(),
                             [&](const auto &item) {
           return strcmp(name, item.name()) == 0;
        });

        return ((iter == m_vecTypes.end()) ? NULL_HANDLE_TYPE : *iter);
    }

    const CHandleType& CHandleTypeManager::getById(const SourceMod::HandleType_t& id) const {
        if(id == NO_HANDLE_TYPE)
            return NULL_HANDLE_TYPE;

        const auto& iter =
                std::find_if(m_vecTypes.begin(),m_vecTypes.end(),
                             [&](const auto &item) {
                                 return item.id() == id;
                             });

        return ((iter == m_vecTypes.end()) ? NULL_HANDLE_TYPE : *iter);
    }

    const CHandleType& CHandleTypeManager::getByIndex(const size_t &index) const {
        if(index >= count())
            return NULL_HANDLE_TYPE;

        return types()[index];
    }

    size_t CHandleTypeManager::count() const {
        return types().size();
    }

    SourceMod::HandleType_t CHandleTypeManager::registerType(const char *name,
                                                             SourceMod::IHandleTypeDispatch *dispatch,
                                                             const SourceMod::HandleType_t &parent = 0,
                                                             SourceMod::TypeAccess *access = nullptr,
                                                             SourceMod::HandleAccess *handleAccess = nullptr,
                                                             SourceMod::IdentityToken_t *identityToken = nullptr) {
        SourceMod::HandleType_t type = 0;
        if(g_pHandleSys->FindHandleType(name, &type))
            return NO_HANDLE_TYPE;

        SourceMod::HandleError error = SourceMod::HandleError_None;
        if((type = g_pHandleSys->CreateType(name,
                                            dispatch,
                                            parent,
                                            access,
                                            handleAccess,
                                            identityToken,
                                            &error)) == BAD_HANDLE)
            return NO_HANDLE_TYPE;

        if(error != SourceMod::HandleError_None)
            g_pHandleSys->RemoveType(type, identityToken);

        type = NO_HANDLE_TYPE;
        if(g_pHandleSys->FindHandleType(name, &type))
            m_vecTypes.emplace_back(type, name, dispatch, parent, access, handleAccess, identityToken);

        return type;
    }

    void CHandleTypeManager::removeType(const SourceMod::HandleType_t &id) {
        std::remove_if(m_vecTypes.begin(),
                       m_vecTypes.end(),
                       [&id](const auto &item) {
            return item.id() == id;
        });
    }

    void CHandleTypeManager::clear() {
        for (const auto &item: m_vecTypes)
            g_pHandleSys->RemoveType(item.id(), const_cast<IdentityToken_t *>(item.ident()));

        m_vecTypes.clear();
    }

    CHandleTypeManager NULL_HANDLE_TYPE_MANAGER = {};
    CHandleTypeManager g_TypeMgr = {};

} // nJansson