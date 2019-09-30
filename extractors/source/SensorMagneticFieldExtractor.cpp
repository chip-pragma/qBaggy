#include "SensorMagneticFieldExtractor.h"

#include <baggy/Extractor.h>
#include <baggy/std.h>

void SensorMagneticFieldExtractor::begin(const QDir &cwd, const rosbag::ConnectionInfo &info)
{
    mXW.open(cwd.path() + "/data.xml", info);
}

void SensorMagneticFieldExtractor::extract(const rosbag::MessageInstance &message)
{
    auto msg = message.instantiate<sensor_msgs::MagneticField>();

    mXW.writeStartMessage(msg->header);

    mXW.writeGeometryVector3("magnetic_field", msg->magnetic_field);
    QStringList mfcList;
    for (auto &mfc : msg->magnetic_field_covariance)
        mfcList << baggy::float10(mfc);
    mXW.writeArray("magnetic_field_covariance", mfcList);

    mXW.writeEndMessage();
}

void SensorMagneticFieldExtractor::end()
{
    mXW.close();
}

baggy::ExtractorInfo baggy_extractor_info() {
    return baggy::makeExtractorInfo<sensor_msgs::MagneticField>(EXTRACTOR_VERSION_MAJOR, EXTRACTOR_VERSION_MINOR);
}

baggy::Extractor *baggy_create_extractor() {
    return static_cast<baggy::Extractor*>(new SensorMagneticFieldExtractor);
}

bool baggy_destroy_extractor(baggy::Extractor *extractor) {
    delete extractor;
    return true;
}
