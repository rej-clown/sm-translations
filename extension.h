#ifndef _INCLUDE_SOURCEMOD_EXTENSION_PROPER_H_
#define _INCLUDE_SOURCEMOD_EXTENSION_PROPER_H_

#include <string>

#include "./src/CPluginListenerV1.h"
#include "./src/CSMCListener.h"

#include "smsdk_ext.h"

class CExTranslator : public SDKExtension
{
public:
	virtual bool SDK_OnLoad(char *error, size_t maxlen, bool late);
	virtual void SDK_OnUnload();

public:
    void OnCoreMapStart(edict_t *pEdictList, int edictCount, int clientMax) override;

public:
    char m_extSettings[PLATFORM_MAX_PATH];
    nExTranslator::CPluginListenerV1* m_pPluginListener;
};

class CMTranslationCollection
{	
public :
	CMTranslationCollection(IPlugin *plugin);
	~CMTranslationCollection();
public :
	// void Destroy();
	unsigned int GetSize();
	int FindFile(const char *filename);
	IPhraseFile *GetFile(unsigned int file);
	IPhraseFile *AddFile(const char *filename);

private :
	IPlugin *pl;
	IPhraseCollection *collection;
};

class CMTranslationFile
{
public :
	CMTranslationFile(IPhraseFile *pFile);
	~CMTranslationFile();

public :
	const char *GetFilename();
	const char *GetFilepath(int lang, PathType type);
	bool TranslationPhraseExists(const char *phrase);
	bool IsNull();
	bool IsSplitted();
	bool IsPathExists(int lang);

private: 
	IPhraseFile *file;
};

class CMTCollectionHandler : public IHandleTypeDispatch
{
public:
	void OnHandleDestroy(HandleType_t type, void *object);
};

class CMTFileHandler : public IHandleTypeDispatch
{
public:
	void OnHandleDestroy(HandleType_t type, void *object);
};

extern HandleType_t htTCollection;
extern CMTCollectionHandler g_trCollection;

extern HandleType_t htTFile;
extern CMTFileHandler g_trFile;

extern CExTranslator g_exTranslator;

extern const sp_nativeinfo_t cmt_collection_natives[];
extern const sp_nativeinfo_t cmt_file_natives[];
extern const sp_nativeinfo_t cmt_translator_natives[];

#endif // _INCLUDE_SOURCEMOD_EXTENSION_PROPER_H_
