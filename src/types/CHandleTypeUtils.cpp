#include "CHandleTypeUtils.h"

namespace nExTranslator {
    const CHandleType &CHandleTypeUtils::GetType(
            SourcePawn::IPluginContext *ctx,
            const CHandleTypeManager &manager,
            const char *name) {

        if(ctx == nullptr || name == nullptr)
            return NULL_HANDLE_TYPE;

        const auto& type = manager.getByName(name);

        if(type.id() == NO_HANDLE_TYPE)
            ctx->ReportError("Type '%s' is unknown type", name);

        return type;
    }
} // nExTranslator