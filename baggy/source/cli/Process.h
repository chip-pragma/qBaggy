#pragma once


#include <QtCore>


namespace chip {

class Process
  : public QObject
{
Q_OBJECT

public:
    Process(QObject* parent = nullptr);

    void setEnd(qint32 end);

    qint32 end();

    qint32 value();

    qint64 interval();

    void start(const QString& prefix = {}, qint64 interval = 200);

    void update(qint32 increment = 1, bool force = false);

    void finish();

private:
    qint32 mEnd = 0.0;
    qint32 mValue = 0.0;
    qint64 mInterval = 200;
    QElapsedTimer mTimer;
    qint64 mLastElapsed = 0;
    QString mLastPrint;

    void print(const QString &line);
};

}




