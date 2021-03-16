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

void CMTCollectionHandler::OnHandleDestroy(HandleType_t type, void *object)
{
	CMTranslationCollection *cmtCollection = (CMTranslationCollection *) object;
	delete cmtCollection;
}

CMTranslationCollection::CMTranslationCollection(IPlugin *plugin)
{
    pl = plugin;
    collection = plugin->GetPhrases();
}

CMTranslationCollection::~CMTranslationCollection()
{
    pl = NULL;
    collection = NULL;
}

unsigned int CMTranslationCollection::GetSize()
{
    return (unsigned int) collection->GetFileCount();
}

int CMTranslationCollection::FindFile(const char *filename)
{
    IPhraseFile *pFile;
    for(unsigned int i = 0; i < GetSize(); i++) {
        pFile = GetFile(i);
        // g_pSM->LogMessage(myself, "Input: %s, Value: %s, Index: %d", filename, (pFile->GetFilename()), i);

        if(strcmp((pFile->GetFilename()), filename) == 0) 
        {
            return i;
        }
    }

    return -1;
}

IPhraseFile *CMTranslationCollection::GetFile(unsigned int file)
{
    if(file > GetSize()) 
    {
        return NULL;
    }

    return collection->GetFile(file);
}

IPhraseFile *CMTranslationCollection::AddFile(const char *filename)
{
    return collection->AddPhraseFile(filename);
}

static cell_t CMTGetCollection(IPluginContext *pContext, const cell_t *params) 
{
    Handle_t hndl = static_cast<Handle_t>(params[1]);

    IPlugin *pl;
    HandleError err;
    if(!(pl = plsys->PluginFromHandle(hndl, &err)))
    {
        return pContext->ThrowNativeError("Plugin handle %x is invalid (error %d)", hndl, err);
    }

    CMTranslationCollection *collection = NULL;
    collection = new CMTranslationCollection(pl);
    
    hndl = handlesys->CreateHandle(htTCollection, collection, pContext->GetIdentity(), myself->GetIdentity(), &err);
    // g_pSM->LogMessage(myself, "Create collection %x (code: %d)", hndl, err);

    return hndl;
}

static cell_t CMTGetCollectionSize(IPluginContext *pContext, const cell_t *params)
{
    Handle_t hndl = static_cast<Handle_t>(params[1]);

    HandleError err;
    CMTranslationCollection *collection;
    if((err = handlesys->ReadHandle(hndl, htTCollection, NULL, (void **)&collection)) != HandleError_None)
    {
        return pContext->ThrowNativeError("Invalid handle index %x (error: %d)", hndl, err);
    }

    return collection->GetSize();
}

static cell_t CMTCollectionAddFile(IPluginContext *pContext, const cell_t *params)
{
    Handle_t hndl = static_cast<Handle_t>(params[1]);

    HandleError err;
    CMTranslationCollection *collection;
    if((err = handlesys->ReadHandle(hndl, htTCollection, NULL, (void **)&collection)) != HandleError_None)
    {
        return pContext->ThrowNativeError("Invalid handle index %x (error: %d)", hndl, err);
    }

    char *filename;
    pContext->LocalToString(params[2], &filename);

    collection->AddFile((const char *)filename);
    return 1;
}

static cell_t CMTCollectionGetFile(IPluginContext *pContext, const cell_t *params)
{
    Handle_t hndl = static_cast<Handle_t>(params[1]);

    HandleError err;
    CMTranslationCollection *collection;
    if((err = handlesys->ReadHandle(hndl, htTCollection, NULL, (void **)&collection)) != HandleError_None)
    {
        return pContext->ThrowNativeError("Invalid handle index %x (error: %d)", hndl, err);
    }

    unsigned int uiFileIndex = (unsigned int) params[2];
    
    CMTranslationFile *tFile = NULL;
    tFile = new CMTranslationFile((collection->GetFile(uiFileIndex)));

    if(tFile->IsNull())
    {
        delete tFile;
        return pContext->ThrowNativeError("Invalid translation file index %d (error: %d)", uiFileIndex, err);
    }

    hndl = handlesys->CreateHandle(htTFile, tFile, pContext->GetIdentity(), myself->GetIdentity(), &err);
    // g_pSM->LogMessage(myself, "Create file %x (code: %d)", hndl, err);

    return hndl;
}

static cell_t CMTCollectionFindFile(IPluginContext *pContext, const cell_t *params)
{
    Handle_t hndl = static_cast<Handle_t>(params[1]);

    HandleError err;
    CMTranslationCollection *collection;
    if((err = handlesys->ReadHandle(hndl, htTCollection, NULL, (void **)&collection)) != HandleError_None)
    {
        return pContext->ThrowNativeError("Invalid handle index %x (error: %d)", hndl, err);
    }

    char *filename;
    pContext->LocalToString(params[2], &filename);

    return collection->FindFile((const char *) filename);
}

const sp_nativeinfo_t cmt_collection_natives[] =
{
    {"PhraseCollection.PhraseCollection", CMTGetCollection},
    {"PhraseCollection.GetSize", CMTGetCollectionSize},
    {"PhraseCollection.GetFile", CMTCollectionGetFile},
    {"PhraseCollection.AddFile", CMTCollectionAddFile},
    {"PhraseCollection.FindFile", CMTCollectionFindFile},
    {NULL, NULL}
};