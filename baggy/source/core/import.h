#pragma once


#include "baggy/Extractor.h"


#define BAGGY_EXTRACTOR_INFO_FUNC "baggy_extractor_info"
#define BAGGY_CREATE_EXTRACTOR_FUNC "baggy_create_extractor"
#define BAGGY_DESTROY_EXTRACTOR_FUNC "baggy_destroy_extractor"

namespace chip {

using ExtractorInfoFunc = baggy::ExtractorInfo (*)();
using CreateExtractorFunc = baggy::Extractor *(*)();
using DestroyExtractorFunc = bool (*)(baggy::Extractor *);

}