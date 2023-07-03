#ifndef EX_TRANSLATION_CHANDLETYPEUTILS_H
#define EX_TRANSLATION_CHANDLETYPEUTILS_H

#include <sp_vm_types.h>

#include "CHandleTypeManager.h"

namespace nExTranslator {

    class CHandleTypeUtils {
    public:
        static const CHandleType& GetType(SourcePawn::IPluginContext*, const CHandleTypeManager&, const char*);
    };

} // nExTranslator

#endif //EX_TRANSLATION_CHANDLETYPEUTILS_H
