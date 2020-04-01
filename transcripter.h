#ifndef TRANSCRIPTER_H
#define TRANSCRIPTER_H

#include <QString>

class Transcripter
{
public:
    Transcripter() = default;
    static QString getTranscriptedWord(const QString &input);
};

#endif // TRANSCRIPTER_H
