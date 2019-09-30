#include "baggy/ExtractorException.h"

#include <utility>

namespace baggy {
ExtractorException::ExtractorException(QString msg)
  : Exception(std::move(msg))
{

}
}
