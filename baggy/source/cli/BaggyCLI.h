#pragma once


#include <QtCore>
#include <ExtractorLibrary.h>

#include "baggy/Extractor.h"


namespace chip {

class BaggyCLI
  : public QObject
{
Q_OBJECT

public:

    BaggyCLI(QObject *parent);

    ~BaggyCLI() override;

public slots:

    void exec();

private:
    QFileInfo mBagFile;
    QDir mOutDir;
    bool mCleanDir = false;
    QMap<QString, QList<ExtractorLibrary *>> mExtLibs;
    rosbag::Bag mBag;

    void doParseArgs();

    void doFindExtractors();

    void doOpenBag();

    void doExtractMessages();

    void doFinish();

    static void cleanDirectory(QDir &dir);
};

}




