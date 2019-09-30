#pragma once


#include <QtCore>

#include "components/PluginLoader.h"

namespace chip {

class BaggyGlobal : public QObject
{
    Q_OBJECT

public:
    explicit BaggyGlobal(QObject* parent = nullptr);

private:
//    PluginManager mPluginManager;
};

}

