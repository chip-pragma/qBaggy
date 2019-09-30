#pragma once


#include <QtCore>

#include "core/PluginLibrary.h"
#include "model/Note.h"

namespace chip {

class PluginInfo : public QObject
{
Q_OBJECT
Q_PROPERTY(Note* status READ status WRITE setStatus NOTIFY statusChanged)
Q_PROPERTY(QString library READ library NOTIFY libraryChanged)

public:
    PluginInfo(QObject* parent = nullptr);

    void setSource(PluginLibrary* library);

    PluginLibrary* source();

    void setStatus(Note* status);

    Note* status();

    QString library();

signals:
    void statusChanged();

    void libraryChanged();

private:
    PluginLibrary* mLibrary = nullptr;
    Note* mStatus = nullptr;
};



using PluginInfoList = QList<QObject*>;

}




