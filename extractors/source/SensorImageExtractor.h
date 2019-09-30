#pragma once


#include <ros/message_traits.h>
#include <sensor_msgs/Image.h>

#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>

#include <QtCore>

#include <baggy/Extractor.h>
#include <baggy/XmlMessageWriter.h>

class SensorImageExtractor : public baggy::Extractor
{
public:
    void begin(const QDir &cwd, const rosbag::ConnectionInfo &info) override;

    void extract(const rosbag::MessageInstance &message) override;

    void end() override;

protected:
    ~SensorImageExtractor() override;

private:
    QDir mCwd;
    baggy::XmlMessageWriter mXW;
    cv::VideoWriter mWriter;
    qint32 mFrames;
};


