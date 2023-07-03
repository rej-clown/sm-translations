#if defined _ex_translations_included_
    #endinput
#endif
#define _ex_translations_included_

native void RebuildLanguageDatabase();

//              (must be freed via delete)
native PhraseCollection GetPluginPhraseCollection(Handle hPlugin);

methodmap PhraseCollection < Handle
{
    // @desc        Getting a PhraseFile descriptor at a given index
    //              (must be freed via delete)
    //
    // @return      PhraseFile handle (all done) / null
    //
    // @error       On create PhraseFile handle (log err)
    //
    // @excepts     On read invalid handle
    public native PhraseFile GetFile(int index);

    // @desc        Getting the PhraseFile position in the collection by name
    //              Name with an extension is expected
    //              Example: core.phrases.txt
    //
    // @return      index / -1 - not found
    //
    // @excepts     On read invalid handle
    public native int FindFile(const char[] fileName);

    // @desc        Adding PhraseFile to a collection
    //              Name without an extension is expected
    //              Example: core.phrases
    //
    // @return      true on success
    //
    // @error       On .txt / .cfg or another exts found 
    //
    // @excepts     On read invalid handle
    public native bool AddFile(const char[] fileName); 

    // @desc        Checks the presence of translation phrase in collection
    //
    // @return      true / false
    //
    // @excepts     On read invalid handle
    public native bool IsPhraseInCollection(const char[] translationPhrase);

    // @desc        Number of phrase files in the collection
    //
    // @return      count of PhraseFile
    //
    // @excepts     On read invalid handle
    property int Size {
        public native get();
    }
}

methodmap PhraseFile < Handle
{
    // @desc        Getting phrase file name
    //
    // @return      true on receive data / false
    //
    // @excepts     On read invalid handle
    public native bool GetName(char[] buffer, int maxLength);

    // @desc        Getting full path to the phrase file by lang
    //              (if it splitted then lang > -1 or -1 otherwise)
    //
    // @return      true on receive data / false
    //
    // @excepts     On read invalid handle
    //              On lang >= max lang count
    public native bool GetPath(int lang, char[] buffer, int maxLength);

    // @desc        Checks the presence of translation phrase in phrase file
    //
    // @return      true / false
    //
    // @excepts     On read invalid handle
    public native bool IsPhraseExists(const char[] translationPhrase);

    // @desc        Checks is it splitted phrase file
    //
    // @return      true / false
    //
    // @excepts     On read invalid handle
    property bool Splitted {
        public native get();
    }

    // Exports a Phrases tree to a phrase file. 
    //
    // @param   kv          KeyValue tree
    // @param   path        Path to the phrase file
    //
    // @return              true - success/false - otherwise
    //
    // @error               Invalid (parent/child)handle
    public bool ExportToFile(KeyValues kv, const char[] path)
    {
        kv.Rewind();
        if(!kv.GotoFirstSubKey())
        {
            return false;
        }

        File hFile;
        if((hFile = OpenFile(path, "w")) == null)
        {
            return false;
        }

        char szBuffer[PLATFORM_MAX_PATH];
        char szValue[PLATFORM_MAX_PATH];

        hFile.WriteLine("\"Phrases\" \n{");

        do
        {
            kv.GetSectionName(szBuffer, sizeof(szBuffer));
            hFile.WriteLine("\t\"%s\" \n\t{", szBuffer);

            if(kv.GotoFirstSubKey(false))
            {
                do
                {
                    kv.GetSectionName(szBuffer, sizeof(szBuffer));
                    kv.GetString("", szValue, sizeof(szValue));

                    hFile.WriteLine("\t\t\"%s\"\t\t\t\"%s\"", szBuffer, szValue);

                } while(kv.GotoNextKey(false));

                kv.GoBack();
            }

            hFile.WriteLine("\t}");

        } while(kv.GotoNextKey());

        hFile.WriteLine("}");

        delete hFile;
        return true;
    }

    // Imports a phrase file in KeyValues format
    //
    // @param   path        Path to the phrase file
    //
    // @return              KeyValue or null
    //
    // @error               Invalid (parent/child)handle
    public KeyValues ImportFromFile(const char[] path)
    {
        KeyValues kv;
        kv = new KeyValues("Phrases");

        if(!kv.ImportFromFile(path))
        {
            delete kv;
        }

        return kv;
    }
}


public Extension __ext_ex_translations =
{
    name = "Extended Translations",
    file = "ex_translations.ext",
#if defined AUTOLOAD_EXTENSIONS
    autoload = 1,
#else
    autoload = 0,
#endif
#if defined REQUIRE_EXTENSIONS
    required = 1,
#else
    required = 0,
#endif
};

#if !defined REQUIRE_EXTENSIONS
public void __ext_ex_translations_SetNTVOptional()
{
    MarkNativeAsOptional("RebuildLanguageDatabase");
    MarkNativeAsOptional("GetPluginPhraseCollection");
}
#endif