#include "PluginLoader.h"

#include <QRandomGenerator>
#include <utility>

#include "Exception.h"


namespace chip {

qint32 PluginLoader::total()
{
    QMutexLocker locker(&mTotalMutex);
    return mTotal;
}

qint32 PluginLoader::loaded()
{
    QMutexLocker locker(&mLoadedMutex);
    return mLoaded;
}

qint32 PluginLoader::failed()
{
    QMutexLocker locker(&mFailedMutex);
    return mFailed;
}

QString PluginLoader::processingFile()
{
    QMutexLocker locker(&mProcessingFileMutex);
    return mProcessingFile;
}

bool PluginLoader::isWorking()
{
    QMutexLocker locker(&mWorkingMutex);
    return mWorking;
}

void PluginLoader::load(const QUrl &pluginsDir)
{
    QDir resultDir(QCoreApplication::applicationDirPath());

    try {
        if (pluginsDir.isLocalFile()) {
            if (not resultDir.cd(pluginsDir.toLocalFile())) {
                auto msg = tr("Unavailable custom plugin directory <%1>").arg(pluginsDir.toLocalFile());
                emit notification(new Note(Note::WARNING, msg));
            } else
                throw Exception();
        } else
            throw Exception();
    }
    catch (Exception &e) {
        if (not resultDir.cd(PLUGINS_PATH)) {
            auto msg = tr("Unavailable native plugin directory <%1>").arg(PLUGINS_PATH);
            emit notification(new Note(Note::ERROR, msg));
            return;
        }
    }

    QThreadPool::globalInstance()->start(
      new __chip__::PluginLoaderWorker(this, resultDir)
    );
}

void PluginLoader::setTotal(qint32 total)
{
    if (mTotalMutex.tryLock()) {
        if (mTotal != total) {
            mTotal = total;
            emit totalChanged();
        }
        mTotalMutex.unlock();
    }
}

void PluginLoader::setLoaded(qint32 loaded)
{
    if (mLoadedMutex.tryLock()) {
        if (mLoaded != loaded) {
            mLoaded = loaded;
            emit loadedChanged();
        }
        mLoadedMutex.unlock();
    }


}

void PluginLoader::setFailed(qint32 failed)
{
    if (mFailedMutex.tryLock()) {
        if (mFailed != failed) {
            mFailed = failed;
            emit failedChanged();
        }
        mFailedMutex.unlock();
    }
}

void PluginLoader::setProcessingFile(const QString &file)
{
    if (mProcessingFileMutex.tryLock()) {
        if (mProcessingFile != file) {
            mProcessingFile = file;
            emit processingFileChanged();
        }
        mProcessingFileMutex.unlock();
    }
}

void PluginLoader::setWorking(bool working)
{
    if (mWorkingMutex.tryLock()) {
        if (mWorking != working) {
            mWorking = working;
            emit workingChanged();
        }
        mWorkingMutex.unlock();
    }
}

void PluginLoader::emitCompleted(PluginInfoList pluginList)
{
    emit completed(std::move(pluginList));
}

}

namespace __chip__ {

PluginLoaderWorker::PluginLoaderWorker(PluginLoader *owner, const QDir &pluginDir)
  : mOwner(owner), mPluginDir(pluginDir)
{

}

void PluginLoaderWorker::run()
{
    auto own = mOwner;
    own->setWorking(true);
    own->setTotal(0);
    own->setLoaded(0);
    own->setFailed(0);
    own->setProcessingFile(nullptr);

    PluginInfoList pluginList;
    qint32 loaded = 0, failed = 0;

#if defined(BAGGY_PLUGIN_LOADER_TEST)
    const auto findCount = 5;
    own->setTotal(findCount);

    for (qint32 i = 1; i <= findCount; i++) {
        char letter = 'A' + (i - 1) % 26;
        int count = (i - 1) / 26 + 1;
        own->setProcessingFile({count, letter});

        QThread::msleep(QRandomGenerator::global()->bounded(10, 100));

        auto p = QRandomGenerator::global()->bounded(5);
        p == 0 ? failed++ : loaded++;

        own->setFailed(failed);
        own->setLoaded(loaded);
    }
#else
    mPluginDir.setNameFilters({"*.plugin.so"});
    auto files = mPluginDir.entryInfoList(QDir::Filter::Files);

    own->setTotal(files.count());


    for (auto &libFile : files) {
        own->setProcessingFile(libFile.fileName());

        auto plugin = new PluginLibrary(libFile.absoluteFilePath());
        auto info = new PluginInfo();

        info->setSource(plugin);
        auto note = new Note();
        if (plugin->load()) {
            if (not plugin->lastError().isNull())
                note->setType(Note::WARNING);
            own->setLoaded(++loaded);
            /*auto types = plugin->instance()->supportedTypes();
            for (auto ti = types.constBegin(); ti != types.constEnd(); ti++) {
                auto fi = mMapTypes.find(ti.key());
                if (fi != mMapTypes.end()) {
                    fi.value().append(plugin);
                } else {
                    mMapTypes.insert(ti.key(), {plugin});
                }
            }*/
        } else {
            note->setType(Note::ERROR);
            own->setFailed(++failed);
        }

        note->setDescription(plugin->lastError());
        info->setStatus(note);
        pluginList.append(info);
    }
#endif

    own->setWorking(false);
    own->emitCompleted(pluginList);
}

}