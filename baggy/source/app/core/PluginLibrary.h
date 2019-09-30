#pragma once


#include <exception>

#include <QtCore>

#include <baggy/core/BasePluginImpl.h>
#include <baggy/import.h>

#include "BaseLastError.h"


namespace chip {
class PluginLibrary : public BaseLastError
{
    Q_GADGET

public:
    PluginLibrary() = default;

    explicit PluginLibrary(const QFileInfo &file);

    ~PluginLibrary() override;

    bool load();

    bool unload();

    bool isLoaded() const;

    QFileInfo file() const;

    void setFile(const QFileInfo &file);

    BasePluginImpl *instance() const;

private:
    QFileInfo mFile;
    QLibrary mLibrary;

    BasePluginImpl *mInstance = nullptr;
    bc::PluginFunc _libCallPlugin = nullptr;
    bc::FreeFunc _libCallFree = nullptr;
};
}
