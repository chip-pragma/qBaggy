#include "baggy/XmlMessageWriter.h"
#include "baggy/ExtractorException.h"
#include "baggy/std.h"


namespace baggy {

XmlMessageWriter::XmlMessageWriter()
  : QXmlStreamWriter()
{

}

void XmlMessageWriter::open(const QString &file, const rosbag::ConnectionInfo &info)
{
    auto xmlFile = new QFile(file);
    if (not xmlFile->open(QFile::NewOnly | QFile::WriteOnly))
        throw baggy::ExtractorException(xmlFile->errorString());

    setDevice(xmlFile);
    setAutoFormatting(true);
    setAutoFormattingIndent(2);

    writeStartDocument();
    writeStartElement("connection");
    writeAttribute("datatype", QString::fromStdString(info.datatype));
}

void XmlMessageWriter::close()
{
    if (device() and device()->isOpen()) {
        writeEndElement();
        writeEndDocument();
        device()->close();
        device()->deleteLater();
    }
}

bool XmlMessageWriter::isOpen() const
{
    return device()->isOpen();
}

void XmlMessageWriter::writeStartMessage()
{
    writeStartElement("message");
}

void XmlMessageWriter::writeStartMessage(const std_msgs::Header &header)
{
    writeStartMessage();
    writeAttribute("sec", QString::number(header.stamp.sec));
    writeAttribute("nsec", QString::number(header.stamp.nsec));
}

void XmlMessageWriter::writeEndMessage()
{
    writeEndElement();
}

void XmlMessageWriter::writeFloat(const QString &tag, float data)
{
    writeStartElement(tag);
    writeCharacters(float10(data));
    writeEndElement();
}

void XmlMessageWriter::writeInteger(const QString &tag, qint64 data)
{
    writeStartElement(tag);
    writeCharacters(QString::number(data));
    writeEndElement();
}

void XmlMessageWriter::writeString(const QString &tag, const QString &data)
{
    writeStartElement(tag);
    writeCharacters(data);
    writeEndElement();
}

void XmlMessageWriter::writeArray(const QString &tag, const QStringList &data)
{
    writeStartElement(tag);
    writeCharacters(data.join(' '));
    writeEndElement();
}

void XmlMessageWriter::writeGeometryVector3(const QString &tag, const geometry_msgs::Vector3 &data)
{
    writeArray(
      tag,
      QStringList() << float10(data.x) << float10(data.y) << float10(data.z)
    );
}

void XmlMessageWriter::writeGeometryQuaternion(const QString &tag, const geometry_msgs::Quaternion &data)
{
    writeArray(
      tag,
      QStringList() << float10(data.x) << float10(data.y) << float10(data.z) << float10(data.w)
    );
}
}