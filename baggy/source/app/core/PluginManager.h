#pragma once


#include "BaseLastError.h"
#include "PluginLibrary.h"

namespace chip {

class PluginManager : public BaseLastError
{
Q_GADGET

public:
    using PluginLibraryList = QList<PluginLibrary*>;

    ~PluginManager() override;

    bool loadPlugins();

    bool unloadPlugins();

    PluginLibraryList plugins() const;

    PluginLibraryList pluginFor(const QString& typeMd5) const;

    QStringList supportedTypes() const;

private:
    const QString PLUGINS_PATH = ".";

    PluginLibraryList mPluginLibraryList;
    QMap<QString, PluginLibraryList> mMapTypes;
};

}
