#include <natives.h>
#include <smsdk_ext.h>
#include <IPluginSys.h>
#include <types/CHandleTypeUtils.h>

#include "phcollection_natives.h"
#include "global_natives.h"

namespace nExTranslator {

    Handle_t CreateHandleByType(SourcePawn::IPluginContext *ctx,
                                const char *type,
                                void *data,
                                const SourceMod::HandleSecurity *sec,
                                const SourceMod::HandleAccess *access) {
        if(ctx == nullptr || type == nullptr)
            return BAD_HANDLE;

        const auto& handleType = CHandleTypeUtils::GetType(ctx, g_TypeMgr, type);

        if(handleType.id() == NO_HANDLE_TYPE)
            return BAD_HANDLE;

        Handle_t handle = BAD_HANDLE;
        HandleError error = SourceMod::HandleError_None;
        if((handle = handleType.createHandle(data, sec, access, &error)) == BAD_HANDLE)
            smutils->LogError(myself, "Create handle failed (code: %d, type: %s)", error, handleType.name());

        return handle;
    }

    cell_t RebuildLanguageDatabase(SourcePawn::IPluginContext *ctx, const cell_t *params) {
        translator->RebuildLanguageDatabase();
        return 1;
    }

    cell_t GetPluginPhraseCollection(SourcePawn::IPluginContext *ctx, const cell_t *params) {

        IPlugin* pPlugin;
        HandleError error {};
        if((pPlugin = plsys->PluginFromHandle(params[1], &error)) == nullptr)
        {
            smutils->LogError(myself, "Plugin handle %x is invalid (error %d)", params[1], error);
            return BAD_HANDLE;
        }

        HandleSecurity phSec {ctx->GetIdentity(), myself->GetIdentity()};
        return (cell_t) CreateHandleByType(ctx, PHCOLLECTION_NATIVES_TYPE, pPlugin->GetPhrases(), &phSec);
    }

    const sp_nativeinfo_t niGlobal[] = {
            { "GetPluginPhraseCollection", GetPluginPhraseCollection },
            { "RebuildLanguageDatabase", RebuildLanguageDatabase },
            { nullptr, nullptr }
    };

} // nExTranslator