#pragma once

#include <QtCore>

#include <rosbag/bag.h>
#include <rosbag/view.h>

#include <baggy/core/BasePluginImpl.h>
#include <baggy/import.h>

#include "PluginLibrary.h"
#include "BaseLastError.h"

namespace chip {

class BagData : public BaseLastError
{
public:
    using ConnectionInfoVector = QVector<const rosbag::ConnectionInfo *>;

    BagData() = default;

    ~BagData();

    bool openFile(const QString& fileName);

    void closeFile();

    QString fileName() const;

    bool isOpened() const;

    ConnectionInfoVector connections();

private:
    rosbag::Bag mBag;
    rosbag::View mView;
    QString mBagFileName;
};

}




