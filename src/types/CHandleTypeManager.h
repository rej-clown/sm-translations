#ifndef SM_JANSSON_CHANDLETYPEMANAGER_H
#define SM_JANSSON_CHANDLETYPEMANAGER_H

#include <vector>

#include "CHandleType.h"

namespace nExTranslator {
    class CHandleTypeManager {
    public:
        CHandleTypeManager();
        virtual ~CHandleTypeManager();

    public:
        virtual SourceMod::HandleType_t registerType(const char* name,
                                                     SourceMod::IHandleTypeDispatch *dispatch,
                                                     const SourceMod::HandleType_t& parent,
                                                     SourceMod::TypeAccess* access,
                                                     SourceMod::HandleAccess* handleAccess,
                                                     SourceMod::IdentityToken_t *identityToken);

        virtual void removeType(const SourceMod::HandleType_t&);

        virtual const CHandleType& getByName(const char* name) const;
        virtual const CHandleType& getById(const SourceMod::HandleType_t& ident) const;
        virtual const CHandleType& getByIndex(const size_t& index) const;

    protected:
        virtual const std::vector<CHandleType>& types() const;

    public:
        virtual size_t count() const;
        virtual void clear();

    private:
        std::vector<CHandleType> m_vecTypes;
    };

    using CTypeMgr = CHandleTypeManager;

    extern CHandleTypeManager g_TypeMgr;
    extern CHandleTypeManager NULL_HANDLE_TYPE_MANAGER;

} // nJansson

#endif //SM_JANSSON_CHANDLETYPEMANAGER_H
