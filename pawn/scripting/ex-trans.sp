#include <ex_translations>

public void OnPluginStart()
{
    PhraseCollection collection;

    if((collection = GetPluginPhraseCollection(GetMyHandle())) == null)
    {
        LogMessage("My phrase collection is null :/");
        return;
    }
        
    LogMessage("My phrase collection size: %d", collection.Size);

    PhraseFile file;
    char buffer[PLATFORM_MAX_PATH];
    for(int i = 0; i < collection.Size; i++)
    {
        file = collection.GetFile(i);

        if(file == null)
        {
            LogMessage("Fuck");
            continue;
        }

        file.GetName(buffer, sizeof(buffer));
        LogMessage("This is: %s", buffer);
    }

    // Fatal error expected
    collection.AddFile("core.phrases.txt");

    if(collection.AddFile("core.phrases"))
    {
        LogMessage("Ho ho got it!");
    }

    int idx;
    if((idx = collection.FindFile("core.phrases.txt")) == -1)
    {
        LogMessage(":///////////// something wrong. contact author");
        return;
    }

    PhraseFile corePhrases = collection.GetFile(idx);

    // 1
    LogMessage("No Access contains in core.phrases.txt? %d", corePhrases.IsPhraseExists("No Access"));

    // 1
    LogMessage("No Access contains in collection? %d", collection.IsPhraseInCollection("No Access"));

    // 1
    LogMessage("Is core.phrases splitted? : %d", corePhrases.Splitted);

    char langCode[4];
    char langName[MAX_NAME_LENGTH];
    for(int i = 0; i < GetLanguageCount(); i++)
    {
        if(!corePhrases.Splitted)
            continue;

        GetLanguageInfo(i, langCode, sizeof(langCode), langName, sizeof(langName));

        if(!corePhrases.GetPath(i, buffer, sizeof(buffer)))
            continue;

        LogMessage("LANG: %d ; %s ; %s \n PATH: %s", i, langCode, langName, buffer);
    }

    delete collection;
    delete corePhrases;

    if((collection = GetPluginPhraseCollection(GetMyHandle())) == null)
    {
        LogMessage("My phrase collection is null :/");
        return;
    }

    LogMessage("My phrase collection size: %d", collection.Size);

    for(int i = 0; i < collection.Size; i++)
    {
        file = collection.GetFile(i);

        if(file == null)
        {
            LogMessage("Fuck");
            continue;
        }

        file.GetName(buffer, sizeof(buffer));
        LogMessage("This is: %s", buffer);
    }

    RebuildLanguageDatabase();

    LogMessage("My phrase collection size: %d", collection.Size);

    delete collection;
}