#pragma once


#include <QtCore>

#include "model/PluginInfo.h"

namespace __chip__ {
class PluginLoaderWorker;
}

namespace chip {
class PluginLoader
  : public QObject
{
    Q_OBJECT
    Q_PROPERTY(qint32 total READ total NOTIFY totalChanged)
    Q_PROPERTY(qint32 loaded READ loaded NOTIFY loadedChanged)
    Q_PROPERTY(qint32 failed READ failed NOTIFY failedChanged)
    Q_PROPERTY(QString processingFile READ processingFile NOTIFY processingFileChanged)
    Q_PROPERTY(bool isWorking READ isWorking NOTIFY workingChanged)

    friend __chip__::PluginLoaderWorker;

public:
    qint32 total();

    qint32 loaded();

    qint32 failed();

    QString processingFile();

    bool isWorking();

    Q_INVOKABLE void load(const QUrl& pluginsDir = QUrl());

signals:

    void totalChanged();

    void loadedChanged();

    void failedChanged();

    void processingFileChanged();

    void workingChanged();

    void completed(QList<QObject*> pluginList);

    void notification(Note* note);

private:
    const QString PLUGINS_PATH = ".";

    qint32 mTotal = 0;
    qint32 mLoaded = 0;
    qint32 mFailed = 0;
    QString mProcessingFile;
    bool mWorking = false;

    QMutex mTotalMutex;
    QMutex mLoadedMutex;
    QMutex mFailedMutex;
    QMutex mProcessingFileMutex;
    QMutex mWorkingMutex;

    void setTotal(qint32 total);

    void setLoaded(qint32 loaded);

    void setProcessingFile(const QString &file);

    void setFailed(qint32 failed);

    void setWorking(bool working);

    void emitCompleted(QList<QObject*> pluginList);
};
}

namespace __chip__ {

using namespace chip;

class PluginLoaderWorker
  : public QRunnable
{
public:
    explicit PluginLoaderWorker(PluginLoader *owner, const QDir& pluginDir);

    void run() override;

private:
    PluginLoader *mOwner;
    QDir mPluginDir;
};

}