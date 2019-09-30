#include "CoreException.h"

#include <utility>


namespace baggy {


CoreException::CoreException(QString msg)
  : Exception(std::move(msg))
{

}
}