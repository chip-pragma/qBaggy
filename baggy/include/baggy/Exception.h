#pragma once


#include <QtCore>


namespace baggy {
class Exception
  : public std::exception
{
public:
    Exception(QString msg = QString());

    virtual QString message() const;

    const char *what() const noexcept final;

protected:
    QString mMessage;
};
}




