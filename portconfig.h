#ifndef PORTCONFIG_H
#define PORTCONFIG_H

#include <QString>

struct portConfig{
    int netAddress=1;
    QString baudrate="115200";
    QString parity="none";
    int stopBits=1;
    QString ip;
    bool isEthDefault;
};

#endif // PORTCONFIG_H
