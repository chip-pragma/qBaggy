#include "SafeExtractor.h"

#include "CoreException.h"


namespace chip {
SafeExtractor::SafeExtractor(ExtractorLibrary *source, QObject *parent)
  : QObject(parent)
{
    mSource = source;
    Q_ASSERT(mSource);
}

ExtractorLibrary *SafeExtractor::source()
{
    return mSource;
}

void SafeExtractor::begin(const QDir &cwd, const rosbag::ConnectionInfo &info)
{
    Q_ASSERT(mSource);
    try {
        mSource->instance()->begin(cwd, info);
    }
    catch (baggy::ExtractorException &e) {
        throwCoreException(e);
    }
}

void SafeExtractor::extract(const rosbag::MessageInstance &message)
{
    Q_ASSERT(mSource);
    try {
        mSource->instance()->extract(message);
    }
    catch (baggy::ExtractorException &e) {
        throwCoreException(e);
    }
}

void SafeExtractor::end()
{
    Q_ASSERT(mSource);
    try {
        mSource->instance()->end();
    }
    catch (baggy::ExtractorException &e) {
        throwCoreException(e);
    }
}

void SafeExtractor::throwCoreException(baggy::ExtractorException &exception)
{
    throw baggy::CoreException(
      QString("%1: %2").arg(mSource->file().fileName(), exception.message())
    );
}
}