#include "PluginLibrary.h"

#include "Exception.h"

namespace chip {
PluginLibrary::PluginLibrary(const QFileInfo &file)
{
    setFile(file);
}

PluginLibrary::~PluginLibrary()
{
    unload();
}

bool PluginLibrary::load()
{
    try {
        if (not mLibrary.load())
            throw Exception(mLibrary.errorString());

        _libCallPlugin = reinterpret_cast<bc::PluginFunc>(mLibrary.resolve(BAGGY_CORE_PLUGIN_FUNC));
        if (not _libCallPlugin)
            throw Exception(mLibrary.errorString());
        _libCallFree = reinterpret_cast<bc::FreeFunc>(mLibrary.resolve(BAGGY_CORE_FREE_FUNC));
        if (not _libCallFree)
            throw Exception(mLibrary.errorString());

        mInstance = _libCallPlugin();
        auto pv = mInstance->coreVersion();
        auto cv = Version::from(BAGGY_CORE_VERSION);

        if (pv.majorPart < cv.majorPart) {
            setLastError(
              QObject::tr("Старая версия плагина (core:%1 > plugin:%2)").arg(cv.toString(), pv.toString())
              );
        } else if (pv.majorPart > cv.majorPart) {
            throw Exception(
              QObject::tr("Старая версия приложения (core:%1 < plugin:%2)").arg(cv.toString(), pv.toString())
              );
        }
    }
    catch (Exception &e) {
        setLastError(e.message());
        unload();
        return false;
    }

    return true;
}

bool PluginLibrary::unload()
{
    bool result = true;
    if (mInstance) {
        result = _libCallFree();
        mLibrary.unload();
        clearLastError();

        mInstance = nullptr;
        _libCallPlugin = nullptr;
        _libCallFree = nullptr;
    }
    return result;
}

bool PluginLibrary::isLoaded() const
{
    return mInstance != nullptr;
}

QFileInfo PluginLibrary::file() const
{
    return mFile;
}

void PluginLibrary::setFile(const QFileInfo &file)
{
    mFile = file;
    mLibrary.setFileName(file.absoluteFilePath());
}

BasePluginImpl *PluginLibrary::instance() const
{
    return mInstance;
}
}