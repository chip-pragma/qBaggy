#include "SensorLaserScanExtractor.h"

#include <baggy/Extractor.h>
#include <baggy/std.h>
#include <baggy/ExtractorException.h>


void SensorLaserScanExtractor::begin(const QDir &cwd, const rosbag::ConnectionInfo &info)
{
    mXW.open(cwd.path() + "/data.xml", info);
}

void SensorLaserScanExtractor::extract(const rosbag::MessageInstance &message)
{
    auto msg = message.instantiate<sensor_msgs::LaserScan>();

    mXW.writeStartMessage(msg->header);

    mXW.writeFloat("scan_time", msg->scan_time);
    mXW.writeFloat("time_increment", msg->time_increment);

    mXW.writeFloat("angle_increment", msg->angle_increment);
    mXW.writeFloat("angle_max", msg->angle_min);
    mXW.writeFloat("angle_min", msg->angle_max);

    mXW.writeFloat("range_min", msg->range_min);
    mXW.writeFloat("range_max", msg->range_max);

    QStringList ranges;
    for (auto &r : msg->ranges)
        ranges << baggy::float10(r);
    mXW.writeArray("ranges", ranges);

    QStringList intensities;
    for (auto &i : msg->intensities)
        intensities << baggy::float10(i);
    mXW.writeArray("intensities", intensities);

    mXW.writeEndMessage();
}

void SensorLaserScanExtractor::end()
{
    mXW.close();
}

baggy::ExtractorInfo baggy_extractor_info() {
    return baggy::makeExtractorInfo<sensor_msgs::LaserScan>(EXTRACTOR_VERSION_MAJOR, EXTRACTOR_VERSION_MINOR);
}

baggy::Extractor *baggy_create_extractor() {
    return static_cast<baggy::Extractor*>(new SensorLaserScanExtractor);
}

bool baggy_destroy_extractor(baggy::Extractor *extractor) {
    delete extractor;
    return true;
}
