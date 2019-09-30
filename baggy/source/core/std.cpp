#include "baggy/std.h"


namespace baggy {

QTextStream tsout(stdout);

QString float10(double value)
{
    return QString::number(value, 'f', 16);
}

}

QTextStream &operator<<(QTextStream &ts, const std::string &str)
{
    return ts << QString::fromStdString(str);
}