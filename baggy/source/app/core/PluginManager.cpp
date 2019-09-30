#include "PluginManager.h"

namespace chip {

PluginManager::~PluginManager()
{
    unloadPlugins();
}

bool PluginManager::loadPlugins()
{


    return true;
}

bool PluginManager::unloadPlugins()
{
    for (auto& plugin: mPluginLibraryList) {
        delete plugin;
    }
    mPluginLibraryList.clear();
    mMapTypes.clear();
}

PluginManager::PluginLibraryList PluginManager::plugins() const
{
    return mPluginLibraryList;
}

PluginManager::PluginLibraryList PluginManager::pluginFor(const QString &typeMd5) const
{
    auto it = mMapTypes.find(typeMd5);
    if (it == mMapTypes.end())
        return {};
    return it.value();
}

QStringList PluginManager::supportedTypes() const
{
    return {mMapTypes.keys()};
}

};