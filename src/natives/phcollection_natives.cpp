#include <natives.h>
#include <smsdk_ext.h>
#include <ITranslator.h>
#include <types/CHandleTypeUtils.h>

#include "global_natives.h"
#include "phrase_natives.h"
#include "phcollection_natives.h"

namespace nExTranslator {
    const char* PHCOLLECTION_NATIVES_TYPE = "PhraseCollection";

    SourceMod::IPhraseCollection *ReadPhraseCollectionHandle(SourcePawn::IPluginContext *ctx,
                               const HandleSecurity *sec,
                               const char *type,
                               const cell_t &address) {
        if(ctx == nullptr || type == nullptr)
            return BAD_HANDLE;

        const auto& handleType = CHandleTypeUtils::GetType(ctx, g_TypeMgr, type);

        if(handleType.id() == NO_HANDLE_TYPE)
            return nullptr;

        void* obj;
        HandleError error {};
        if((error = handlesys->ReadHandle(address, handleType.id(), sec, (void**)&obj))
           != SourceMod::HandleError_None)
            ctx->ReportError("Read '%s' handle error (code: %d)", type, error);

        if(error != SourceMod::HandleError_None)
            obj = nullptr;

        return (SourceMod::IPhraseCollection*)obj;
    }

    cell_t GetCollectionSize(SourcePawn::IPluginContext *ctx, const cell_t *params) {
        HandleSecurity sec {ctx->GetIdentity(), myself->GetIdentity()};

        SourceMod::IPhraseCollection* ptr;
        if((ptr = ReadPhraseCollectionHandle(ctx, &sec, PHCOLLECTION_NATIVES_TYPE, params[1])) == nullptr)
            return -1;

        return (cell_t) ptr->GetFileCount();
    }

    cell_t AddFileToCollection(SourcePawn::IPluginContext *ctx, const cell_t *params) {
        HandleSecurity sec {ctx->GetIdentity(), myself->GetIdentity()};

        SourceMod::IPhraseCollection* ptr;
        if((ptr = ReadPhraseCollectionHandle(ctx, &sec, PHCOLLECTION_NATIVES_TYPE, params[1])) == nullptr)
            return false;

        char *buffer;
        ctx->LocalToString(params[2], &buffer);

        if(buffer == nullptr)
            return false;

        return ptr->AddPhraseFile(buffer) != nullptr;
    }

    cell_t GetFileFromCollection(SourcePawn::IPluginContext *ctx, const cell_t *params) {
        HandleSecurity sec {ctx->GetIdentity(), myself->GetIdentity()};

        SourceMod::IPhraseCollection* ptr;
        if((ptr = ReadPhraseCollectionHandle(ctx, &sec, PHCOLLECTION_NATIVES_TYPE, params[1])) == nullptr)
            return BAD_HANDLE;

        auto idx = (unsigned int) params[2];

        if(idx >= ptr->GetFileCount())
            return BAD_HANDLE;

        HandleSecurity phSec {ctx->GetIdentity(), myself->GetIdentity()};
        return (cell_t) CreateHandleByType(ctx, PHRASE_NATIVES_TYPE, ptr->GetFile(idx), &phSec);
    }

    cell_t FindFileInCollection(SourcePawn::IPluginContext *ctx, const cell_t *params) {
        HandleSecurity sec {ctx->GetIdentity(), myself->GetIdentity()};

        SourceMod::IPhraseCollection* ptr;
        if((ptr = ReadPhraseCollectionHandle(ctx, &sec, PHCOLLECTION_NATIVES_TYPE, params[1])) == nullptr)
            return -1;

        char *buffer;
        ctx->LocalToString(params[2], &buffer);

        SourceMod::IPhraseFile* pFile;
        for(unsigned int i = 0; i < ptr->GetFileCount(); i++)
        {
            pFile = ptr->GetFile(i);

            if(!strcmp(buffer, pFile->GetFilename()))
                return (cell_t)i;
        }

        return -1;
    }

    cell_t IsTranslationPhraseInCollection(SourcePawn::IPluginContext *ctx, const cell_t *params) {
        HandleSecurity sec {ctx->GetIdentity(), myself->GetIdentity()};

        SourceMod::IPhraseCollection* ptr;
        if((ptr = ReadPhraseCollectionHandle(ctx, &sec, PHCOLLECTION_NATIVES_TYPE, params[1])) == nullptr)
            return false;

        char* buffer;
        ctx->LocalToString(params[2], &buffer);

        return ptr->TranslationPhraseExists(buffer);
    }

    const sp_nativeinfo_t niPhraseCollection[] = {
            { "PhraseCollection.Size.get", GetCollectionSize },
            { "PhraseCollection.GetFile", GetFileFromCollection },
            { "PhraseCollection.AddFile", AddFileToCollection },
            { "PhraseCollection.FindFile", FindFileInCollection },
            { "PhraseCollection.IsPhraseInCollection", IsTranslationPhraseInCollection },
            { nullptr, nullptr }
    };

} // nExTranslator