#include "PluginInfo.h"

namespace chip {

PluginInfo::PluginInfo(QObject *parent)
  : QObject(parent)
{

}

void PluginInfo::setSource(PluginLibrary *library)
{
    if (mLibrary != library) {
        mLibrary = library;
        emit libraryChanged();
    }
}

PluginLibrary *PluginInfo::source()
{
    return mLibrary;
}

void PluginInfo::setStatus(Note* status)
{
    if (mStatus != status) {
        if (mStatus)
            mStatus->deleteLater();
        mStatus = status;
        if (mStatus)
            mStatus->setParent(this);
        emit statusChanged();
    }
}

Note* PluginInfo::status()
{
    return mStatus;
}

QString PluginInfo::library()
{
    if (mLibrary)
        return mLibrary->file().fileName();
    else
        return {};
}

}