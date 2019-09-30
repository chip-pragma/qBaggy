#include <QCoreApplication>

#include "BaggyCLI.h"


int main(int argc, char **argv)
{
    using namespace chip;

    QCoreApplication app(argc, argv);
    QCoreApplication::setApplicationName("qBaggy CLI");
    QCoreApplication::setApplicationVersion(
      QString("%1.%2.%3")
        .arg(BAGGY_VERSION_MAJOR)
        .arg(BAGGY_VERSION_MINOR)
        .arg(BAGGY_VERSION_PATCH)
    );
    QCoreApplication::setOrganizationDomain("free.chip");
    QCoreApplication::setOrganizationName("chip_pragma");

    BaggyCLI baggy(&app);

    QTimer::singleShot(0, &baggy, &BaggyCLI::exec);
    return QCoreApplication::exec();
}
