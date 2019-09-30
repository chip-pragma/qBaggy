#pragma once


#include <QtCore>

namespace chip {

class BaseLastError
{
public:
    virtual ~BaseLastError() = default;

    virtual QString lastError() const final;

protected:
    virtual void setLastError(const QString& error);

    virtual void clearLastError();

private:
    QString mLastError;
};
}




