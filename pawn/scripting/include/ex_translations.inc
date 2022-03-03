#if defined _ex_translations_included_
    #endinput
#endif
#define _ex_translations_included_

methodmap PhraseCollection < Handle
{
    // Creates a collection of plugin phrase files 
    //
    // The PhraseCollection must be freed via delete or CloseHandle().
    public native PhraseCollection(Handle hPlugin);


    // Retrieves the collection size.
    //
    // @return           	Size.
    public native int GetSize();


    // Retrieves a PhraseFile object from the collection.
    //
    // @param   index       File index
    //
    // @return              Phrase file (must be freed)
    //
    // @error               Invalid handle or index		
    public native PhraseFile GetFile(int index);


    // Adding a new file to the plugin collection 
    // 
    // @param   filename    Translation filename (with out .txt extension)
    //
    // @noreturn
    //
    // @error               Invalid handle or file isn't exists?
    public native void AddFile(const char[] filename);


    // Retrieves an index of phrase file.
    //
    // @param   filename    Translation filename
    //
    // @return              Index or -1 if not found
    //
    // @error               Invalid handle
    public native int FindFile(const char[] filename);
}

methodmap PhraseFile < Handle
{
    // Retrieves a phrase filename.
    //
    // @param   buffer      Buffer to store
    //
    // @noreturn
    //
    // @error               Invalid (parent/child)handle
    public native void GetFilename(char[] buffer, int size);


    // Checks for the presence of a phrase in the current phrase file 
    //
    // @param   phrase      Phrase to check
    //
    // @return              true - exsits / false - otherwise
    //
    // @error               Invalid (parent/child)handle
    public native bool TranslationPhraseExists(const char[] phrase);


    // Is the file split by language 
    //
    // @return              true - splitted / false - otherwise
    //
    // @error               Invalid (parent/child)handle
    public native bool IsSplitted();


    // Retrieves the full path to the phrase file. 
    //
    // @param   lang        -1 if the file is not split, otherwise - language id 
    // @param   buffer      Buffer to store
    // @param   size        Buffer size
    //
    // @return              true - path exists / false - otherwise
    //
    // @error               Invalid (parent/child)handle / lang id
    public native bool GetFilepath(int lang = -1, char[] buffer, int size);


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


/**
 *  Reparses all loaded translations files.
 */
native void RebuildLanguageDatabase();

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
}
#endif