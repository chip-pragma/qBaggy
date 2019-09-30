#pragma once


#include "baggy/Exception.h"

namespace baggy {
class ExtractorException : public Exception
{
public:
    ExtractorException(QString msg = {});
};
}



