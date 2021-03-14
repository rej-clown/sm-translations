#if defined _ex_translations_included_
	#endinput
#endif
#define _ex_translations_included_

methodmap PhraseCollection < Handle
{
	public native PhraseCollection(Handle hPlugin); //?

	public native int GetSize();
	public native PhraseFile GetFile(int index);
	public native void AddFile(const char[] filename);
}

methodmap PhraseFile < Handle
{
	public native void GetFilename(char[] buffer, int size);
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
