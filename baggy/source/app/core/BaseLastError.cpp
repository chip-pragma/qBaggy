#include "BaseLastError.h"

namespace chip {

QString BaseLastError::lastError() const
{
    return QString();
}

void BaseLastError::setLastError(const QString &error)
{
    mLastError = error;
}

void BaseLastError::clearLastError()
{
    mLastError = nullptr;
}

}