#pragma once


#include <QtCore>

#include "baggy/Extractor.h"
#include "baggy/ExtractorException.h"
#include "ExtractorLibrary.h"


namespace chip {

class SafeExtractor
  : public QObject
{
Q_OBJECT

public:
    SafeExtractor(ExtractorLibrary *source, QObject *parent = nullptr);

    ExtractorLibrary *source();

    void begin(const QDir &cwd, const rosbag::ConnectionInfo &info);

    void extract(const rosbag::MessageInstance &message);

    void end();

protected:
    virtual void throwCoreException(baggy::ExtractorException &exception);

private:
    ExtractorLibrary *mSource = nullptr;
};

}




