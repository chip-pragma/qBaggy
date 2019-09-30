#pragma once


#include <QtCore>

#include "baggy/Extractor.h"
#include "import.h"

namespace chip {

class ExtractorLibrary : public QObject
{
    Q_GADGET

public:
    ExtractorLibrary(QObject* parent = nullptr);

    explicit ExtractorLibrary(const QFileInfo &fileInfo, QObject* parent = nullptr);

    ~ExtractorLibrary() override;

    void load();

    void unload();

    bool isLoaded() const;

    baggy::ExtractorInfo info() const;

    QFileInfo file() const;

    void setFile(const QFileInfo &fileInfo);

    baggy::Extractor *instance() const;

    static QFileInfoList libraryFiles(const QString& libDirPath = {});

private:
    QFileInfo mFileInfo;
    QLibrary mLibrary;

    baggy::ExtractorInfo mInfo;
    baggy::Extractor *mInstance = nullptr;
    chip::ExtractorInfoFunc mInfoFunc = nullptr;
    chip::CreateExtractorFunc mCreateFunc = nullptr;
    chip::DestroyExtractorFunc mDestroyFunc = nullptr;
};

}



