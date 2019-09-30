#pragma once


#include <QtCore>

namespace chip {

class Note : public QObject
{
    Q_OBJECT
    Q_PROPERTY(NoteType type READ type WRITE setType NOTIFY typeChanged)
    Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY descriptionChanged)

public:
    enum NoteType {
        NORMAL,
        UNDEFINED,
        WARNING,
        CRITICAL,
        SUCCESS,
        ERROR
    };
    Q_ENUM(NoteType)

    Note(QObject* parent = nullptr);

    Note(NoteType type, QString  desc, QObject* parent = nullptr);

    void setType(NoteType type);

    void setDescription(const QString& desc);

    NoteType type();

    QString description();

signals:
    void typeChanged();

    void descriptionChanged();

private:
    NoteType mType = NORMAL;
    QString mDesc;
};

}




