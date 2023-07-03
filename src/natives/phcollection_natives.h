#ifndef EX_TRANSLATION_PHCOLLECTION_NATIVES_H
#define EX_TRANSLATION_PHCOLLECTION_NATIVES_H

#include <sp_vm_types.h>
#include <IHandleSys.h>

namespace nExTranslator {

    extern const char *PHCOLLECTION_NATIVES_TYPE;

    extern SourceMod::IPhraseCollection* ReadPhraseCollectionHandle(
            SourcePawn::IPluginContext* ctx,
            const HandleSecurity* sec,
            const char *type,
            const cell_t& address);

    extern cell_t GetCollectionSize(SourcePawn::IPluginContext* ctx, const cell_t* params);
    extern cell_t AddFileToCollection(SourcePawn::IPluginContext* ctx, const cell_t* params);
    extern cell_t GetFileFromCollection(SourcePawn::IPluginContext* ctx, const cell_t* params);
    extern cell_t FindFileInCollection(SourcePawn::IPluginContext* ctx, const cell_t* params);
    extern cell_t IsTranslationPhraseInCollection(SourcePawn::IPluginContext* ctx, const cell_t* params);

} // nExTranslator

#endif //EX_TRANSLATION_PHCOLLECTION_NATIVES_H
