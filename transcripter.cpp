#include "transcripter.h"
#include <QString>

QString Transcripter::getTranscriptedWord(const QString &input)
{
    QString res;
    for(int i=0;i<input.size();i++) {
        QChar ch = input.at(i);
        switch(ch.unicode()) {
            case 0x0401: res.append("JO");break;
            case 0x0410: res.append("A");break;
            case 0x0411: res.append("B");break;
            case 0x0412: res.append("V");break;
            case 0x0413: res.append("G");break;
            case 0x0414: res.append("D");break;
            case 0x0415: res.append("E");break;
            case 0x0416: res.append("ZH");break;
            case 0x0417: res.append("Z");break;
            case 0x0418: res.append("I");break;
            case 0x0419: res.append("Y");break;
            case 0x041A: res.append("K");break;
            case 0x041B: res.append("L");break;
            case 0x041C: res.append("M");break;
            case 0x041D: res.append("N");break;
            case 0x041E: res.append("O");break;
            case 0x041F: res.append("P");break;
            case 0x0420: res.append("R");break;
            case 0x0421: res.append("S");break;
            case 0x0422: res.append("T");break;
            case 0x0423: res.append("U");break;
            case 0x0424: res.append("F");break;
            case 0x0425: res.append("H");break;
            case 0x0426: res.append("C");break;
            case 0x0427: res.append("CH");break;
            case 0x0428: res.append("SH");break;
            case 0x0429: res.append("CH");break;
            case 0x042A: break;
            case 0x042B: res.append("I");break;
            case 0x042C: break;
            case 0x042D: res.append("e");break;
            case 0x042E: res.append("juU");break;
            case 0x042F: res.append("ja");break;
            case 0x0430: res.append("a");break;
            case 0x0431: res.append("b");break;
            case 0x0432: res.append("v");break;
            case 0x0433: res.append("g");break;
            case 0x0434: res.append("d");break;
            case 0x0435: res.append("e");break;
            case 0x0436: res.append("zh");break;
            case 0x0437: res.append("z");break;
            case 0x0438: res.append("i");break;
            case 0x0439: res.append("y");break;
            case 0x043A: res.append("k");break;
            case 0x043B: res.append("l");break;
            case 0x043C: res.append("m");break;
            case 0x043D: res.append("n");break;
            case 0x043E: res.append("o");break;
            case 0x043F: res.append("p");break;
            case 0x0440: res.append("r");break;
            case 0x0441: res.append("s");break;
            case 0x0442: res.append("t");break;
            case 0x0443: res.append("u");break;
            case 0x0444: res.append("f");break;
            case 0x0445: res.append("h");break;
            case 0x0446: res.append("c");break;
            case 0x0447: res.append("ch");break;
            case 0x0448: res.append("sh");break;
            case 0x0449: res.append("ch");break;
            case 0x044A: break;
            case 0x044B: res.append("i");break;
            case 0x044C: break;
            case 0x044D: res.append("e");break;
            case 0x044E: res.append("ju");break;
            case 0x044F: res.append("ja");break;
            case 0x0451: res.append("jo");break;
            default: res.append(ch);
        }
    }
    return res;
}
