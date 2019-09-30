#include "SensorImuExtractor.h"

#include <baggy/Extractor.h>
#include <baggy/std.h>

void SensorImuExtractor::begin(const QDir &cwd, const rosbag::ConnectionInfo &info)
{
    mXW.open(cwd.path() + "/data.xml", info);
}

void SensorImuExtractor::extract(const rosbag::MessageInstance &message)
{
    auto msg = message.instantiate<sensor_msgs::Imu>();

    mXW.writeStartMessage(msg->header);

    mXW.writeGeometryVector3("angular_velocity", msg->angular_velocity);
    QStringList avcList;
    for (auto &avc : msg->angular_velocity_covariance)
        avcList << baggy::float10(avc);
    mXW.writeArray("angular_velocity_covariance", avcList);

    mXW.writeGeometryVector3("linear_acceleration", msg->linear_acceleration);
    QStringList lacList;
    for (auto &lac : msg->linear_acceleration_covariance)
        lacList << baggy::float10(lac);
    mXW.writeArray("linear_acceleration_covariance", lacList);

    mXW.writeGeometryQuaternion("orientation", msg->orientation);
    QStringList ocList;
    for (auto &oc : msg->orientation_covariance)
        ocList << baggy::float10(oc);
    mXW.writeArray("orientation_covariance", ocList);

    mXW.writeEndMessage();
}

void SensorImuExtractor::end()
{
    mXW.close();
}

baggy::ExtractorInfo baggy_extractor_info() {
    return baggy::makeExtractorInfo<sensor_msgs::Imu>(EXTRACTOR_VERSION_MAJOR, EXTRACTOR_VERSION_MINOR);
}

baggy::Extractor *baggy_create_extractor() {
    return static_cast<baggy::Extractor*>(new SensorImuExtractor);
}

bool baggy_destroy_extractor(baggy::Extractor *extractor) {
    delete extractor;
    return true;
}
