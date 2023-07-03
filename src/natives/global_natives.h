#ifndef EX_TRANSLATION_GLOBAL_NATIVES_H
#define EX_TRANSLATION_GLOBAL_NATIVES_H

#include <sp_vm_types.h>
#include <IHandleSys.h>

namespace nExTranslator {

    extern Handle_t CreateHandleByType(SourcePawn::IPluginContext* ctx,
                                       const char* type,
                                       void* data,
                                       const SourceMod::HandleSecurity *sec = nullptr,
                                       const SourceMod::HandleAccess *access = nullptr);


    extern cell_t RebuildLanguageDatabase(SourcePawn::IPluginContext *ctx, const cell_t *params);
    extern cell_t GetPluginPhraseCollection(SourcePawn::IPluginContext *ctx, const cell_t *params);

} // nExTranslator

#endif //EX_TRANSLATION_GLOBAL_NATIVES_H
