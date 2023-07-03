#ifndef SM_JANSSON_T_H
#define SM_JANSSON_T_H

#include <sp_vm_types.h>
#include <IHandleSys.h>

namespace nExTranslator {

    extern const char *PHRASE_NATIVES_TYPE;

    extern SourceMod::IPhraseFile* ReadPhraseFileHandle(
            SourcePawn::IPluginContext* ctx,
            const HandleSecurity* sec,
            const char *type,
            const cell_t& address);

    extern cell_t GetPhraseFileName(SourcePawn::IPluginContext* ctx, const cell_t* params);
    extern cell_t TranslationPhraseExists(SourcePawn::IPluginContext* ctx, const cell_t* params);
    extern cell_t GetPhraseFilePath(SourcePawn::IPluginContext* ctx, const cell_t* params);
    extern cell_t IsPhraseFileSplitted(SourcePawn::IPluginContext* ctx, const cell_t* params);

} // nExtTranslations

#endif //SM_JANSSON_T_H
