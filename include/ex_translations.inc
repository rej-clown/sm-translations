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
