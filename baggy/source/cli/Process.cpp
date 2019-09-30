#include "Process.h"

#include "baggy/std.h"


namespace chip {
Process::Process(QObject *parent)
  : QObject(parent)
{

}

void Process::setEnd(qint32 end)
{
    mEnd = end;
}

qint32 Process::end()
{
    return mEnd;
}

qint32 Process::value()
{
    return mValue;
}

qint64 Process::interval()
{
    return mInterval;
}

void Process::start(const QString& prefix, qint64 interval)
{
    mValue = 0;
    mInterval = interval;
    mTimer.start();
    mLastPrint = "";
    if (not prefix.isNull())
        baggy::tsout << prefix << " - ";
}

void Process::update(qint32 increment, bool force)
{
    mValue += increment;
    auto diff = mTimer.elapsed() - mLastElapsed;
    if (diff > mInterval or force) {
        auto percents = QString::number((static_cast<double>(mValue) / mEnd) * 100.0, 'f', 2);
        print(QString("%1%").arg(percents));
        mLastElapsed = mTimer.elapsed();
    }

    QCoreApplication::processEvents();
}

void Process::finish()
{
    if (mValue >= mEnd)
        print(tr("full complete!"));
    else
        print(tr("complete %1/%2").arg(mValue).arg(mEnd));
    baggy::tsout << endl;
    mTimer.invalidate();
}

void Process::print(const QString &line)
{
    QString backspace(mLastPrint.size(), '\b');
    mLastPrint = line;
    auto fill = backspace.size() - mLastPrint.size();
    baggy::tsout << backspace << mLastPrint << QString(fill > 0 ? fill : 0, ' ');
    baggy::tsout.flush();
}
}