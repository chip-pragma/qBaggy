#include "BagData.h"

#include <QtCore>


namespace chip {

BagData::~BagData()
{
    closeFile();
}

bool BagData::openFile(const QString &fileName)
{
    mBag.close();

    try {
        mBag.open(fileName.toStdString());
        mBagFileName = fileName;
    }
    catch (rosbag::BagException &e) {
        setLastError(QString::fromStdString(e.what()));
        return false;
    }
    return true;
}

void BagData::closeFile()
{
    mBag.close();
    mBagFileName = nullptr;
}

QString BagData::fileName() const
{
    return mBagFileName;
}

bool BagData::isOpened() const
{
    return not mBagFileName.isNull();
}

BagData::ConnectionInfoVector BagData::connections()
{
    if (not isOpened()) {
        setLastError(QObject::tr("bag-файл не был открыт"));
        return {};
    }
    rosbag::View view(mBag);
    return ConnectionInfoVector::fromStdVector(view.getConnections());
}

}