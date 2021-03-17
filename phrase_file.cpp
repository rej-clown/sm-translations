/**
 * vim: set ts=4 :
 * =============================================================================
 * SourceMod Sample Extension
 * Copyright (C) 2004-2008 AlliedModders LLC.  All rights reserved.
 * =============================================================================
 *
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License, version 3.0, as published by the
 * Free Software Foundation.
 * 
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * As a special exception, AlliedModders LLC gives you permission to link the
 * code of this program (as well as its derivative works) to "Half-Life 2," the
 * "Source Engine," the "SourcePawn JIT," and any Game MODs that run on software
 * by the Valve Corporation.  You must obey the GNU General Public License in
 * all respects for all other code used.  Additionally, AlliedModders LLC grants
 * this exception to all derivative works.  AlliedModders LLC defines further
 * exceptions, found in LICENSE.txt (as of this writing, version JULY-31-2007),
 * or <http://www.sourcemod.net/license.php>.
 *
 * Version: $Id$
 */


#include "extension.h"

void CMTFileHandler::OnHandleDestroy(HandleType_t type, void *object)
{
	CMTranslationFile *cmtFile = (CMTranslationFile *) object;
	delete cmtFile;
}

CMTranslationFile::CMTranslationFile(IPhraseFile *pFile)
{
    file = pFile;
}

CMTranslationFile::~CMTranslationFile()
{
    file = NULL;
}

const char *CMTranslationFile::GetFilename()
{
    return file->GetFilename();
}

const char *CMTranslationFile::GetFilepath(int lang, PathType type)
{
    char *path = new char[PLATFORM_MAX_PATH];

    if(lang != -1)
    {
        const char *code;
        translator->GetLanguageInfo((unsigned int)lang, &code, NULL);

        g_pSM->BuildPath(type, 
            path, 
            PLATFORM_MAX_PATH,
            "translations/%s/%s",
            code,
            file->GetFilename());
    } 
    else 
    {
        g_pSM->BuildPath(type, 
            path, 
            PLATFORM_MAX_PATH,
            "translations/%s",
            file->GetFilename());
    }

    // g_pSM->LogMessage(myself, "Path: %s", path);

    return (const char *) path;
}

bool CMTranslationFile::TranslationPhraseExists(const char *phrase)
{
    return file->TranslationPhraseExists(phrase);
}

bool CMTranslationFile::IsNull()
{
    return (file == NULL);
}

bool CMTranslationFile::IsSplitted()
{
    const char *code;
    for(unsigned int i = 0; i < translator->GetLanguageCount(); i++)
    {
        if(!translator->GetLanguageInfo(i, &code, NULL))
        {
            continue;
        }

        if(IsPathExists(i))
        {
            return true;
        }
    }

    return false;
}

bool CMTranslationFile::IsPathExists(int lang)
{
    const char *path;
    path = GetFilepath(lang, Path_SM);

    return libsys->PathExists(path);
}

static cell_t CMTFTranslationExists(IPluginContext *pContext, const cell_t *params)
{
    Handle_t hndl = static_cast<Handle_t>(params[1]);

    HandleError err;
    CMTranslationFile *tFile;
    if((err = handlesys->ReadHandle(hndl, htTFile, NULL, (void **)&tFile)) != HandleError_None)
    {
        return pContext->ThrowNativeError("Invalid handle index %x (error: %d)", hndl, err);
    }

    char *str;
    pContext->LocalToString(params[2], &str);

    return tFile->TranslationPhraseExists((const char *)str);
}

static cell_t CMTFilename(IPluginContext *pContext, const cell_t *params)
{
    Handle_t hndl = static_cast<Handle_t>(params[1]);

    HandleError err;
    CMTranslationFile *tFile;
    if((err = handlesys->ReadHandle(hndl, htTFile, NULL, (void **)&tFile)) != HandleError_None)
    {
        return pContext->ThrowNativeError("Invalid handle index %x (error: %d)", hndl, err);
    }

    pContext->StringToLocal(params[2], params[3], (tFile->GetFilename()));
    return 1;
}

static cell_t CMTFSplitted(IPluginContext *pContext, const cell_t *params)
{
    Handle_t hndl = static_cast<Handle_t>(params[1]);

    HandleError err;
    CMTranslationFile *tFile;
    if((err = handlesys->ReadHandle(hndl, htTFile, NULL, (void **)&tFile)) != HandleError_None)
    {
        return pContext->ThrowNativeError("Invalid handle index %x (error: %d)", hndl, err);
    }

    return tFile->IsSplitted();
}

static cell_t CMTFGetFilepath(IPluginContext *pContext, const cell_t *params)
{
    Handle_t hndl = static_cast<Handle_t>(params[1]);

    HandleError err;
    CMTranslationFile *tFile;
    if((err = handlesys->ReadHandle(hndl, htTFile, NULL, (void **)&tFile)) != HandleError_None)
    {
        return pContext->ThrowNativeError("Invalid handle index %x (error: %d)", hndl, err);
    }

    int lang = (int)params[2];
    if(lang != -1 && (unsigned int)lang >= translator->GetLanguageCount())
    {
        return pContext->ThrowNativeError("Invalid lang id %d (max: %d)", lang, (translator->GetLanguageCount() - 1));
    }

    pContext->StringToLocal(params[3], params[4], (tFile->GetFilepath(lang, Path_SM_Rel)));
    return tFile->IsPathExists(lang);
}

const sp_nativeinfo_t cmt_file_natives[] =
{
    {"PhraseFile.IsSplitted", CMTFSplitted},
    {"PhraseFile.GetFilename", CMTFilename},
    {"PhraseFile.GetFilepath", CMTFGetFilepath},
    {"PhraseFile.TranslationPhraseExists", CMTFTranslationExists},
    {NULL, NULL}
};