#include "Note.h"

#include <utility>

namespace chip {

Note::Note(QObject *parent)
  : QObject(parent)
{
}

Note::Note(Note::NoteType type, QString desc, QObject *parent)
  : QObject(parent), mType(type), mDesc(std::move(desc))
{
}

void Note::setType(Note::NoteType type)
{
    if (mType != type) {
        mType = type;
        emit typeChanged();
    }
}

void Note::setDescription(const QString &desc)
{
    if (mDesc != desc) {
        mDesc = desc;
        emit descriptionChanged();
    }
}

Note::NoteType Note::type()
{
    return mType;
}

QString Note::description()
{
    return mDesc;
}

}