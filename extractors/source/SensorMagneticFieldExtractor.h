#pragma once


#include <ros/message_traits.h>
#include <sensor_msgs/MagneticField.h>

#include <baggy/Extractor.h>
#include <baggy/XmlMessageWriter.h>

class SensorMagneticFieldExtractor : public baggy::Extractor
{
public:
    void begin(const QDir &cwd, const rosbag::ConnectionInfo &info) override;

    void extract(const rosbag::MessageInstance &message) override;

    void end() override;

private:
    baggy::XmlMessageWriter mXW;
};


