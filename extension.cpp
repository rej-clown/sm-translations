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

CMTranslation g_cmTranslation;		/**< Global singleton for extension's main interface */

SMEXT_LINK(&g_cmTranslation);

HandleType_t htTCollection=0;
CMTCollectionHandler g_trCollection;

HandleType_t htTFile=0;
CMTFileHandler g_trFile;

bool CMTranslation::SDK_OnLoad(char *error, size_t maxlength, bool late)
{
    sharesys->AddNatives(myself, cmt_collection_natives);
    sharesys->AddNatives(myself, cmt_file_natives);
	sharesys->RegisterLibrary(myself, "ex_translations");

	// TODO: Secure & Access
	HandleAccess haPhraseCollection;
	haPhraseCollection.access[HandleAccess_Clone] = 0;
	haPhraseCollection.access[HandleAccess_Delete] = 0;
	haPhraseCollection.access[HandleAccess_Read] = 0;

	htTCollection = handlesys->CreateType("PhraseCollection", &g_trCollection, 0, NULL, &haPhraseCollection, myself->GetIdentity(), NULL);

    HandleAccess haPhraseFile;
	haPhraseFile.access[HandleAccess_Clone] = 0;
	haPhraseFile.access[HandleAccess_Delete] = 0;
	haPhraseFile.access[HandleAccess_Read] = 0;

	htTFile = handlesys->CreateType("PhraseFile", &g_trFile, htTCollection, NULL, &haPhraseFile, myself->GetIdentity(), NULL);
    
    return true;
}

void CMTranslation::SDK_OnUnload()
{
	handlesys->RemoveType(htTCollection, myself->GetIdentity());
    handlesys->RemoveType(htTFile, myself->GetIdentity());
}






