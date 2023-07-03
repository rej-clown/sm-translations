#include <natives.h>
#include <smsdk_ext.h>
#include <ITranslator.h>
#include <types/CHandleTypeUtils.h>

#include "phrase_natives.h"

namespace nExTranslator {
    const char* PHRASE_NATIVES_TYPE = "PhraseFile";

    SourceMod::IPhraseFile* ReadPhraseFileHandle(SourcePawn::IPluginContext *ctx,
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

        return (SourceMod::IPhraseFile*)obj;
    }

    void BuildPhrasePath(const char* name, int lang, SourceMod::PathType type, char* buffer, const size_t& maxLength) {
        if(lang != -1)
        {
            const char* code;
            translator->GetLanguageInfo((unsigned int) lang, &code, nullptr);

            smutils->BuildPath(
                    type,
                    buffer,
                    maxLength,
                    "translations/%s/%s",
                    code,
                    name);
        }
        else smutils->BuildPath(
                    type,
                    buffer,
                    maxLength,
                    "translations/%s",
                    name);
    }

    // bool PhraseFile.GetName(char[], int);
    cell_t GetPhraseFileName(SourcePawn::IPluginContext *ctx, const cell_t *params) {
        HandleSecurity sec {ctx->GetIdentity(), myself->GetIdentity()};

        SourceMod::IPhraseFile* ptr;
        if((ptr = ReadPhraseFileHandle(ctx, &sec, PHRASE_NATIVES_TYPE, params[1])) == nullptr)
            return false;

        size_t wbt = 0;
        ctx->StringToLocalUTF8(params[2], params[3], ptr->GetFilename(), &wbt);

        return wbt != 0;
    }

    // bool PhraseFile.PhraseExists(const char[]);
    cell_t TranslationPhraseExists(SourcePawn::IPluginContext *ctx, const cell_t *params) {
        HandleSecurity sec {ctx->GetIdentity(), myself->GetIdentity()};

        SourceMod::IPhraseFile* ptr;
        if((ptr = ReadPhraseFileHandle(ctx, &sec, PHRASE_NATIVES_TYPE, params[1])) == nullptr)
            return false;

        char* buffer;
        ctx->LocalToString(params[2], &buffer);

        return ptr->TranslationPhraseExists(buffer);
    }

    // PhraseFile.Path(int lang, char[] buffer, int)
    cell_t GetPhraseFilePath(SourcePawn::IPluginContext *ctx, const cell_t *params) {
        HandleSecurity sec {ctx->GetIdentity(), myself->GetIdentity()};

        SourceMod::IPhraseFile* ptr;
        if((ptr = ReadPhraseFileHandle(ctx, &sec, PHRASE_NATIVES_TYPE, params[1])) == nullptr)
            return false;

        int lang = params[2];
        if(lang != -1 && (unsigned int)lang >= translator->GetLanguageCount())
            return ctx->ThrowNativeError("Invalid lang id %d (max: %d)", lang, (translator->GetLanguageCount() - 1));

        char *buffer;
        ctx->LocalToString(params[3], &buffer);

        BuildPhrasePath(ptr->GetFilename(), lang, Path_SM_Rel, buffer, params[4]);

        return true;
    }

    cell_t IsPhraseFileSplitted(SourcePawn::IPluginContext *ctx, const cell_t *params) {
        HandleSecurity sec {ctx->GetIdentity(), myself->GetIdentity()};

        SourceMod::IPhraseFile* ptr;
        if((ptr = ReadPhraseFileHandle(ctx, &sec, PHRASE_NATIVES_TYPE, params[1])) == nullptr)
            return false;

        const char *code;
        char* buffer = (char*) malloc((PLATFORM_MAX_PATH + 1) * sizeof(char));

        for(unsigned int i = 0, size = translator->GetLanguageCount();
            i < size;
            i++)
        {
            if(!translator->GetLanguageInfo(i, &code, nullptr))
                continue;

            BuildPhrasePath(ptr->GetFilename(), i, Path_SM, buffer, PLATFORM_MAX_PATH);

            if(libsys->PathExists(buffer))
            {
                free(buffer);
                return true;
            }
        }

        free(buffer);
        return false;
    }

    const sp_nativeinfo_t niPhraseFile[] = {
            { "PhraseFile.GetName", GetPhraseFileName },
            { "PhraseFile.GetPath", GetPhraseFilePath },
            { "PhraseFile.IsPhraseExists", TranslationPhraseExists },
            { "PhraseFile.Splitted.get", IsPhraseFileSplitted },
            { nullptr, nullptr }
    };
} // nExtTranslations