#ifndef EX_TRANSLATION_DISPATCHES_H
#define EX_TRANSLATION_DISPATCHES_H

#include <IHandleSys.h>

namespace nExTranslator {

    class CPhraseCollectionTypeDispatcher : public SourceMod::IHandleTypeDispatch {
    public:
        void OnHandleDestroy(SourceMod::HandleType_t type, void* object);
    };

    class CPhraseFileTypeDispatcher : public SourceMod::IHandleTypeDispatch {
    public:
        void OnHandleDestroy(SourceMod::HandleType_t type, void* object);
    };
}

#endif //EX_TRANSLATION_DISPATCHES_H
