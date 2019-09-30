#include "SensorImageExtractor.h"
#include <baggy/Extractor.h>
#include <baggy/std.h>
#include <baggy/ExtractorException.h>


#include <cv_bridge/cv_bridge.h>


void SensorImageExtractor::begin(const QDir &cwd, const rosbag::ConnectionInfo &info)
{
    mCwd = cwd;
    mFrames = 0;
    mXW.open(cwd.path() + "/data.xml", info);
    mXW.writeAttribute("fps", QString::number(20.0, 'f', 2));
}

void SensorImageExtractor::extract(const rosbag::MessageInstance &message)
{
    auto msg = message.instantiate<sensor_msgs::Image>();

    if (not mWriter.isOpened()) {
        auto fileName = (mCwd.path() + "/frames.avi").toLocal8Bit().data();
        mWriter.open(fileName,CV_FOURCC('M','J','P','G'), 20.0, cv::Size(msg->width, msg->height));

        if (not mWriter.isOpened())
            throw baggy::ExtractorException(QString("Unable to create video <%1>").arg(fileName));
    }

    mXW.writeStartMessage(msg->header);
    mXW.writeInteger("frame", mFrames++);
    mXW.writeEndMessage();

    auto cvImage = cv_bridge::toCvCopy(msg, msg->encoding);
    auto mat = cvImage->image;
    cv::cvtColor(mat, mat, CV_RGB2BGR);
    mWriter << mat;
    mat.release();
}

void SensorImageExtractor::end()
{
    mXW.close();
    mWriter.release();
}

SensorImageExtractor::~SensorImageExtractor()
{
    end();
}

baggy::ExtractorInfo baggy_extractor_info()
{
    return baggy::makeExtractorInfo<sensor_msgs::Image>(EXTRACTOR_VERSION_MAJOR, EXTRACTOR_VERSION_MINOR);
}

baggy::Extractor *baggy_create_extractor()
{
    return static_cast<baggy::Extractor *>(new SensorImageExtractor);
}

bool baggy_destroy_extractor(baggy::Extractor *extractor)
{
    delete extractor;
    return true;
}