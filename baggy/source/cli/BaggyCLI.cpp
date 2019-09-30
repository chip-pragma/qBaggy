#include "BaggyCLI.h"

#include <rosbag/view.h>

#include "baggy/std.h"
#include "CoreException.h"
#include "ExtractorLibrary.h"
#include "SafeExtractor.h"
#include "Process.h"


namespace chip {

BaggyCLI::BaggyCLI(QObject *parent)
  : QObject(parent)
{

}

BaggyCLI::~BaggyCLI()
{
    doFinish();
}

void BaggyCLI::exec()
{
    try {
        doParseArgs();
        doFindExtractors();
        doOpenBag();
        doExtractMessages();
    }
    catch (baggy::CoreException &e) {
        baggy::tsout << "\nERROR: " << e.message() << endl;
        QCoreApplication::exit(1);
        return;
    }

    QCoreApplication::exit();
}

void BaggyCLI::doParseArgs()
{
    using namespace baggy;

    QCommandLineParser clip;
    clip.setApplicationDescription(tr("ROS bag-file data extractor.") + "\n" + QString(BAGGY_AUTHOR));
    clip.addHelpOption();
    clip.addVersionOption();

    clip.addPositionalArgument("BAG_FILE", tr("Input bag-file"));

    QCommandLineOption cleanDirOption("clean-out-dir", tr("Force clean output directory"));
    clip.addOption(cleanDirOption);

    QCommandLineOption outDirOption(
      QStringList() << "d" << "out-directory",
      tr("Output directory"), "OUT_DIR"
    );
    clip.addOption(outDirOption);

    auto app = QCoreApplication::instance();
    clip.process(*app);

    auto posArgs = clip.positionalArguments();
    if (posArgs.count() != 1)
        throw CoreException(tr("Positional arguments were expected"));
    mBagFile.setFile(posArgs.at(0));

    if (clip.isSet(outDirOption)) {
        mOutDir.setPath(clip.value(outDirOption));
    } else {
        mOutDir.setPath(QString("%1/%2.qbaggy").arg(mBagFile.path(), mBagFile.fileName()));
    }

    mCleanDir = clip.isSet(cleanDirOption);

    if (not mBagFile.exists())
        throw CoreException(tr("File <%1> not exists").arg(mBagFile.filePath()));
    if (not mBagFile.isFile())
        throw CoreException(tr("Specified path <%1> is not file").arg(mBagFile.filePath()));
    if (not mBagFile.isReadable())
        throw CoreException(tr("No permission to read file").arg(mBagFile.filePath()));

    if (not mOutDir.exists()) {
        baggy::tsout << tr("A directory <%1> that does not exist will be created.").arg(mOutDir.path()) << endl;

        auto prefix = QDir::current();
        if (mOutDir.isAbsolute())
            prefix = QDir::root();
        if (not prefix.mkpath(mOutDir.path()))
            throw CoreException(tr("Unable to create directory <%1>").arg(mOutDir.path()));
    }
    if (not mOutDir.isEmpty()) {
        if (mCleanDir)
            cleanDirectory(mOutDir);
        else
            throw CoreException(tr("Directory <%1> is not empty").arg(mOutDir.path()));
    }

    baggy::tsout << "Bag-file: " << mBagFile.absoluteFilePath() << endl;
    baggy::tsout << "Out directory: " << mOutDir.absolutePath() << endl;
}

void BaggyCLI::doFindExtractors()
{
    auto fileList = ExtractorLibrary::libraryFiles();

    for (auto &info : fileList) {
        auto lib = new ExtractorLibrary(info);
        try {
            lib->load();
            auto md5 = lib->info().md5sum;
            mExtLibs[md5].push_back(lib);
        }
        catch (baggy::CoreException &e) {
            delete lib;
            baggy::tsout << tr("Unable load <%1>: %2").arg(info.fileName(), e.message());
        }
    }
}

void BaggyCLI::doOpenBag()
{
    using namespace baggy;

    try {
        mBag.open(mBagFile.filePath().toStdString());
        rosbag::View view(mBag);
        baggy::tsout << tr("%1 messages of %2 types:").arg(view.size()).arg(view.getConnections().size()) << endl;

        qint32 incompatibilities = 0;
        for (auto &con : view.getConnections()) {
            baggy::tsout << "[" << con->id << "] " << con->topic << " (" << con->datatype << "): ";
            if (not mExtLibs.contains(QString::fromStdString(con->md5sum))) {
                incompatibilities++;
                baggy::tsout << tr("UNSUPPORTED");
            } else {
                auto &list = mExtLibs[QString::fromStdString(con->md5sum)];
                baggy::tsout << list[0]->file().fileName() << " ";
                if (list.count() > 1) {
                    baggy::tsout << tr("(and %1 more)").arg(list.count() - 1);
                }
            }
            baggy::tsout << endl;
        }
        if (incompatibilities > 0) {
            throw CoreException(tr("There are %1 topics without extractors").arg(incompatibilities));
        }
    }
    catch (rosbag::BagException &e) {
        throw CoreException(tr("Unable open bag-file <%1>: %2").arg(mBagFile.filePath(), e.what()));
    }
}

void BaggyCLI::doExtractMessages()
{
    rosbag::View view(mBag);
    QMap<std::string, SafeExtractor *> extractors;
    Process proc;
    auto conCount = view.getConnections().size();
    auto regex = QRegExp("[^-a-z0-9]");

    proc.setEnd(conCount);
    proc.start(tr("Begin"));
    for (auto &con : view.getConnections()) {
        auto topic = QString::fromStdString(con->topic);
        topic.replace("/", "-");
        topic.replace(regex, "_");
        topic.insert(0, QString::number(con->id));

        auto topicOut = mOutDir;
        if (not topicOut.mkdir(topic) or not topicOut.cd(topic))
            throw baggy::CoreException(tr("Unable to create topic (%1) output directory").arg(topic));

        auto safeExt = new SafeExtractor(mExtLibs[QString::fromStdString(con->md5sum)][0], this);
        safeExt->begin(topicOut, *con);
        extractors.insert(con->md5sum, safeExt);
        proc.update();
    }
    proc.finish();

    proc.setEnd(view.size());
    proc.start(tr("Extraction"));
    for (auto &msg : view) {
        extractors[msg.getMD5Sum()]->extract(msg);
        proc.update();
    }
    proc.finish();

    proc.setEnd(conCount);
    proc.start(tr("End"));
    for (auto &con : view.getConnections()) {
        auto safeExt = extractors[con->md5sum];
        safeExt->end();
        delete safeExt;
        proc.update();
    }
    proc.finish();
}

void BaggyCLI::doFinish()
{
    baggy::tsout << tr("Exit...") << endl;

    mBag.close();

    Process progress;
    progress.setEnd(mExtLibs.size());
    progress.start();
    for (auto& libs : mExtLibs) {
        progress.setEnd(progress.end() + libs.size());
        progress.update();
        for (auto &lib : libs) {
            lib->unload();
            progress.update();
        }
    }
    progress.finish();
}

void BaggyCLI::cleanDirectory(QDir &dir)
{
    auto entryList = dir.entryInfoList(QDir::NoDotAndDotDot | QDir::Files | QDir::Dirs);
    for (auto &info : entryList) {
        if (not dir.remove(info.fileName())) {
            QDir subDir(info.absoluteFilePath());
            if (not subDir.removeRecursively())
                throw baggy::CoreException(tr("Unable to remove <%1>").arg(info.fileName()));
        }

    }
}

}