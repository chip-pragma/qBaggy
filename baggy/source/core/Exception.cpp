#include "baggy/Exception.h"

#include <utility>

namespace baggy {
Exception::Exception(QString msg)
  : mMessage(std::move(msg))
{

}

QString Exception::message() const
{
    return mMessage;
}

const char *Exception::what() const noexcept
{
    return message().toLocal8Bit();
}
}