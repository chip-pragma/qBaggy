#pragma once


#include <string>
#include <QtCore/QTextStream>


namespace baggy {

extern QTextStream tsout;

QString float10(double value);

}

QTextStream& operator<<(QTextStream& ts, const std::string& str);