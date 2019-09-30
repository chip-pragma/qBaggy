#pragma once


#include <string>

#include <QtCore>
#include <rosbag/message_instance.h>


namespace baggy {
struct ExtractorInfo
{
    qint32 major;
    qint32 minor;
    const char *md5sum;
    const char *dataType;
};

template<class TMsg>
ExtractorInfo makeExtractorInfo(qint32 major, qint32 minor)
{
    return {
      .major = major,
      .minor = minor,
      .md5sum = ros::message_traits::md5sum<TMsg>(),
      .dataType = ros::message_traits::datatype<TMsg>()
    };
}

class Extractor;
}


extern "C" {
baggy::ExtractorInfo baggy_extractor_info();

baggy::Extractor *baggy_create_extractor();

bool baggy_destroy_extractor(baggy::Extractor *extractor);
}


namespace baggy {

class Extractor
{
    friend Extractor *::baggy_create_extractor();

    friend bool::baggy_destroy_extractor(Extractor *extractor);

public:
    virtual void begin(const QDir &cwd, const rosbag::ConnectionInfo &info) = 0;

    virtual void extract(const rosbag::MessageInstance &message) = 0;

    virtual void end() = 0;

protected:
    Extractor() = default;

    virtual ~Extractor() = default;
};

}