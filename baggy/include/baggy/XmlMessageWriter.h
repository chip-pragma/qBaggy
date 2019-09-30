#pragma once


#include <QtCore>

#include <rosbag/bag.h>
#include <std_msgs/Header.h>
#include <geometry_msgs/Vector3.h>
#include <geometry_msgs/Quaternion.h>


namespace baggy {

class XmlMessageWriter : public QXmlStreamWriter {
public:
    XmlMessageWriter();

    void open(const QString& file, const rosbag::ConnectionInfo &info);

    void close();

    bool isOpen() const;

    void writeStartMessage();

    void writeStartMessage(const std_msgs::Header& header);

    void writeEndMessage();

    void writeFloat(const QString& tag, float data);

    void writeInteger(const QString& tag, qint64 data);

    void writeString(const QString& tag, const QString& data);

    void writeArray(const QString& tag, const QStringList& data);

    void writeGeometryVector3(const QString &tag, const geometry_msgs::Vector3& data);

    void writeGeometryQuaternion(const QString &tag, const geometry_msgs::Quaternion& data);
};

}