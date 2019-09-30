#pragma once


#include "baggy/Exception.h"

namespace baggy {

class CoreException : public Exception {
public:
    CoreException(QString msg = QString());
};

}