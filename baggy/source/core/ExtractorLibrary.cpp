#include "ExtractorLibrary.h"

#include "baggy/Exception.h"

#include "baggy/std.h"


namespace chip {

ExtractorLibrary::ExtractorLibrary(QObject *parent)
  : QObject(parent), mInfo{}
{

}

ExtractorLibrary::ExtractorLibrary(const QFileInfo &fileInfo, QObject *parent)
  : QObject(parent), mInfo{}
{
    setFile(fileInfo);
}

ExtractorLibrary::~ExtractorLibrary()
{
    unload();
}

void ExtractorLibrary::load()
{
    using namespace baggy;

    try {
        if (not mLibrary.load())
            throw Exception(mLibrary.errorString());

        mInfoFunc = reinterpret_cast<chip::ExtractorInfoFunc>(mLibrary.resolve(BAGGY_EXTRACTOR_INFO_FUNC));
        if (not mInfoFunc)
            throw Exception(mLibrary.errorString());
        mCreateFunc = reinterpret_cast<chip::CreateExtractorFunc>(mLibrary.resolve(BAGGY_CREATE_EXTRACTOR_FUNC));
        if (not mCreateFunc)
            throw Exception(mLibrary.errorString());
        mDestroyFunc = reinterpret_cast<chip::DestroyExtractorFunc >(mLibrary.resolve(BAGGY_DESTROY_EXTRACTOR_FUNC));
        if (not mDestroyFunc)
            throw Exception(mLibrary.errorString());

        mInfo = mInfoFunc();
        if (mInfo.major != BAGGY_VERSION_MAJOR)
            throw Exception(
              tr("Library and application major version mismatch (%1 vs %2)")
                .arg(mInfo.major)
                .arg(BAGGY_VERSION_MAJOR)
            );
        else if (mInfo.minor != BAGGY_VERSION_MINOR)
            baggy::tsout << tr("Library and application minor version mismatch (%1 vs %2)")
              .arg(mInfo.minor)
              .arg(BAGGY_VERSION_MINOR)
                         << endl;

        mInstance = mCreateFunc();
    }
    catch (Exception &e) {
        unload();
        throw e;
    }
}

void ExtractorLibrary::unload()
{
    if (mInstance) {
        mDestroyFunc(mInstance);
        mLibrary.unload();

        mInfoFunc = nullptr;
        mCreateFunc = nullptr;
        mDestroyFunc = nullptr;
        mInfo = {};
        mInstance = nullptr;
    }
}

bool ExtractorLibrary::isLoaded() const
{
    return mInstance != nullptr;
}

baggy::ExtractorInfo ExtractorLibrary::info() const
{
    if (mInstance)
        return mInfo;
    return {};
}

QFileInfo ExtractorLibrary::file() const
{
    return mFileInfo;
}

void ExtractorLibrary::setFile(const QFileInfo &fileInfo)
{
    mFileInfo = fileInfo;
    mLibrary.setFileName(fileInfo.absoluteFilePath());
}

baggy::Extractor *ExtractorLibrary::instance() const
{
    return mInstance;
}

QFileInfoList ExtractorLibrary::libraryFiles(const QString &libDirPath)
{
    using namespace baggy;

    QDir libDir(QCoreApplication::applicationDirPath());
    if (not libDirPath.isNull()) {
        libDir.setPath(libDirPath);
        if (not libDir.exists())
            throw Exception(tr("Specified extractors directory not exists!"));
    } else if (not libDir.cd("."))
        throw Exception(tr("Native extractors directory not exists!"));
    libDir.setNameFilters({"lib*.so"});
    return libDir.entryInfoList(QDir::Files | QDir::NoSymLinks);
}

}