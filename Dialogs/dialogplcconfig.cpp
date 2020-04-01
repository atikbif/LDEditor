#include "dialogplcconfig.h"
#include "ui_dialogplcconfig.h"
#include <array>
#include <vector>
#include "Loader/ethconfigreader.h"
#include "Loader/ethconfigwriter.h"
#include <QDebug>
#include <QMessageBox>
#include <QTimer>

std::vector<quint16> DialogPLCConfig::convertToPLCMap(const QByteArray &value)
{
    std::vector<quint16> result;
    if(value.size()>=confSize) {
        int offset = 0;
        quint16 inpType = 0x0000;
        for(int i=0;i<14;i++) {
            if(value.at(i)==1) inpType|=1<<i;
        }
        result.push_back(inpType);
        offset+= 14;
        result.push_back(static_cast<quint16>(value.at(offset)));
        offset++;
        quint16 ip_high=0, ip_low=0;
        ip_high = static_cast<quint8>(value.at(offset));
        ip_high = ip_high << 8;
        ip_high |= static_cast<quint8>(value.at(offset+1));
        ip_low = static_cast<quint8>(value.at(offset+2));
        ip_low = ip_low << 8;
        ip_low |= static_cast<quint8>(value.at(offset+3));
        result.push_back(ip_high);
        result.push_back(ip_low);
        offset+=4;
        ip_high = static_cast<quint8>(value.at(offset));
        ip_high = ip_high << 8;
        ip_high |= static_cast<quint8>(value.at(offset+1));
        ip_low = static_cast<quint8>(value.at(offset+2));
        ip_low = ip_low << 8;
        ip_low |= static_cast<quint8>(value.at(offset+3));
        result.push_back(ip_high);
        result.push_back(ip_low);
        offset+=4;
        ip_high = static_cast<quint8>(value.at(offset));
        ip_high = ip_high << 8;
        ip_high |= static_cast<quint8>(value.at(offset+1));
        ip_low = static_cast<quint8>(value.at(offset+2));
        ip_low = ip_low << 8;
        ip_low |= static_cast<quint8>(value.at(offset+3));
        result.push_back(ip_high);
        result.push_back(ip_low);
        offset+=4;
        if(value.size()>=confSize2) {
            quint16 conf=0;
            conf = static_cast<quint8>(value.at(offset));
            conf = conf << 8;
            conf |= static_cast<quint8>(value.at(offset+1));
            result.push_back(conf);
            offset+=2;
            conf = static_cast<quint8>(value.at(offset));
            conf = conf << 8;
            conf |= static_cast<quint8>(value.at(offset+1));
            result.push_back(conf);
            offset+=2;
        }
    }
    return result;
}

QByteArray DialogPLCConfig::convertFromPLCMap(std::vector<quint16> value)
{
    QByteArray result;
    int offset = 0;
    if(value.size()>=8) {
        quint16 inpType = value.at(0);
        for(int i=0;i<14;i++) {
            if(inpType & (1<<i)) result.push_back(1);else result.push_back('\0');
        }
        offset++;
        result.push_back(static_cast<quint8>(value.at(offset)));
        offset++;
        quint16 ip=0;
        ip = value.at(offset);
        result.push_back(ip>>8);
        result.push_back(ip&0xFF);
        offset++;
        ip = value.at(offset);
        result.push_back(ip>>8);
        result.push_back(ip&0xFF);
        offset++;
        ip = value.at(offset);
        result.push_back(ip>>8);
        result.push_back(ip&0xFF);
        offset++;
        ip = value.at(offset);
        result.push_back(ip>>8);
        result.push_back(ip&0xFF);
        offset++;
        ip = value.at(offset);
        result.push_back(ip>>8);
        result.push_back(ip&0xFF);
        offset++;
        ip = value.at(offset);
        result.push_back(ip>>8);
        result.push_back(ip&0xFF);
        offset++;

        if(value.size()>=10) {
            quint16 conf=0;
            conf = value.at(offset);
            result.push_back(conf>>8);
            result.push_back(conf&0xFF);
            offset++;
            conf = value.at(offset);
            result.push_back(conf>>8);
            result.push_back(conf&0xFF);
        }
    }

    return result;
}

std::vector<quint16> DialogPLCConfig::convertCurrentToPLCMap()
{
    std::vector<QComboBox*> inputs = {ui->comboBoxInp1,
                                      ui->comboBoxInp2,
                                      ui->comboBoxInp3,
                                      ui->comboBoxInp4,
                                      ui->comboBoxInp5,
                                      ui->comboBoxInp6,
                                      ui->comboBoxInp7,
                                      ui->comboBoxInp8,
                                      ui->comboBoxInp9,
                                      ui->comboBoxInp10,
                                      ui->comboBoxInp11,
                                      ui->comboBoxInp12,
                                      ui->comboBoxInp13,
                                      ui->comboBoxInp14};
    std::vector<quint16> result;
    quint16 value = 0x0000;
    for(int i=0;i<14;i++) {
        if(inputs.at(i)->currentIndex()!=0) value|=(1<<i);
    }
    result.push_back(value);
    value = ui->spinBoxNetAddr->value();
    result.push_back(value);
    QString ip = ui->lineEditIP->text();
    QStringList numberString = ip.split(".");
    value = static_cast<quint8>(numberString[0].toInt());
    value = static_cast<quint16>(value << 8);
    value |= static_cast<quint8>(numberString[1].toInt());
    result.push_back(value);
    value = static_cast<quint8>(numberString[2].toInt());
    value = static_cast<quint16>(value << 8);
    value |= static_cast<quint8>(numberString[3].toInt());
    result.push_back(value);
    ip = ui->lineEditMask->text();
    numberString = ip.split(".");
    value = static_cast<quint8>(numberString[0].toInt());
    value = static_cast<quint16>(value << 8);
    value |= static_cast<quint8>(numberString[1].toInt());
    result.push_back(value);
    value = static_cast<quint8>(numberString[2].toInt());
    value = static_cast<quint16>(value << 8);
    value |= static_cast<quint8>(numberString[3].toInt());
    result.push_back(value);
    ip = ui->lineEditGate->text();
    numberString = ip.split(".");
    value = static_cast<quint8>(numberString[0].toInt());
    value = static_cast<quint16>(value << 8);
    value |= static_cast<quint8>(numberString[1].toInt());
    result.push_back(value);
    value = static_cast<quint8>(numberString[2].toInt());
    value = static_cast<quint16>(value << 8);
    value |= static_cast<quint8>(numberString[3].toInt());
    result.push_back(value);
    quint8 can1_speed = ui->comboBoxSpeed1->currentIndex();
    quint8 can1_parity = ui->comboBoxParity1->currentIndex();
    quint8 can1_stop_bits = ui->comboBoxStopBits1->currentIndex();
    quint8 can2_speed = ui->comboBoxSpeed2->currentIndex();
    quint8 can2_parity = ui->comboBoxParity2->currentIndex();
    quint8 can2_stop_bits = ui->comboBoxStopBits2->currentIndex();
    quint16 can1_config = ((can1_parity & 0x0F)<<4) | (can1_stop_bits & 0x0F);
    can1_config = can1_config << 8;
    can1_config |= can1_speed;

    quint16 can2_config = ((can2_parity & 0x0F)<<4) | (can2_stop_bits & 0x0F);
    can2_config = can2_config << 8;
    can2_config |= can2_speed;
    result.push_back(can1_config);
    result.push_back(can2_config);
    return result;
}

bool DialogPLCConfig::checkCurrentConfig()
{
    bool result = true;
    QString ip = ui->lineEditIP->text();
    QStringList numberString = ip.split(".");
    if(numberString.count()!=4) return false;
    for(int i=0;i<4;i++) {
        numberString[i].toInt(&result);
        if(!result) return false;
    }
    ip = ui->lineEditGate->text();
    numberString = ip.split(".");
    if(numberString.count()!=4) return false;
    for(int i=0;i<4;i++) {
        numberString[i].toInt(&result);
        if(!result) return false;
    }
    ip = ui->lineEditMask->text();
    numberString = ip.split(".");
    if(numberString.count()!=4) return false;
    for(int i=0;i<4;i++) {
        numberString[i].toInt(&result);
        if(!result) return false;
    }
    return true;
}

DialogPLCConfig::DialogPLCConfig(PLCConfig config, QWidget *parent) :
    QDialog(parent),config(config),
    ui(new Ui::DialogPLCConfig)
{
    ui->setupUi(this);
    ui->comboBoxSpeed1->setCurrentIndex(2);
    ui->comboBoxSpeed2->setCurrentIndex(2);
    ui->comboBoxParity1->setCurrentIndex(1);
    ui->comboBoxParity2->setCurrentIndex(1);
    setConfig(config.getSettings());
}

DialogPLCConfig::~DialogPLCConfig()
{
    delete ui;
}

void DialogPLCConfig::setConfig(const QByteArray &value)
{
    int offset = 0;
    conf = value;
    std::vector<QComboBox*> inputs = {ui->comboBoxInp1,
                                      ui->comboBoxInp2,
                                      ui->comboBoxInp3,
                                      ui->comboBoxInp4,
                                      ui->comboBoxInp5,
                                      ui->comboBoxInp6,
                                      ui->comboBoxInp7,
                                      ui->comboBoxInp8,
                                      ui->comboBoxInp9,
                                      ui->comboBoxInp10,
                                      ui->comboBoxInp11,
                                      ui->comboBoxInp12,
                                      ui->comboBoxInp13,
                                      ui->comboBoxInp14};
    if(conf.length()>=confSize) {
        for(int i=0;i<14;i++) {
            if(conf.at(offset+i)==0) inputs.at(i)->setCurrentIndex(0);
            else inputs.at(i)->setCurrentIndex(1);
        }
        offset+=14;
        ui->spinBoxNetAddr->setValue(static_cast<quint8>(conf.at(offset)));
        offset++;
        std::array<quint8,4> ip;
        for(int i=0;i<4;i++) {
            ip[i] = static_cast<quint8>(conf.at(offset+i));
        }
        ui->lineEditIP->setText(QString::number(ip[0])+"."+QString::number(ip[1])+"."+QString::number(ip[2])+"."+QString::number(ip[3]));
        offset+=4;
        for(int i=0;i<4;i++) {
            ip[i] = static_cast<quint8>(conf.at(offset+i));
        }
        ui->lineEditMask->setText(QString::number(ip[0])+"."+QString::number(ip[1])+"."+QString::number(ip[2])+"."+QString::number(ip[3]));
        offset+=4;
        for(int i=0;i<4;i++) {
            ip[i] = static_cast<quint8>(conf.at(offset+i));
        }
        ui->lineEditGate->setText(QString::number(ip[0])+"."+QString::number(ip[1])+"."+QString::number(ip[2])+"."+QString::number(ip[3]));
        offset+=4;
    }
    if(conf.length()>=confSize2) {
        uint8_t highByte = static_cast<quint8>(conf.at(offset));
        uint8_t lowByte = static_cast<quint8>(conf.at(offset+1));
        uint8_t parity = highByte >> 4;
        if(parity>2) parity = 1;
        uint8_t stopbits = highByte & 0x0F;
        if(stopbits>1) stopbits = 0;
        if(lowByte>6) lowByte = 2;
        ui->comboBoxSpeed1->setCurrentIndex(lowByte);
        ui->comboBoxParity1->setCurrentIndex(parity);
        ui->comboBoxStopBits1->setCurrentIndex(stopbits);
        offset+=2;
        highByte = static_cast<quint8>(conf.at(offset));
        lowByte = static_cast<quint8>(conf.at(offset+1));
        parity = highByte >> 4;
        if(parity>2) parity = 1;
        stopbits = highByte & 0x0F;
        if(stopbits>1) stopbits = 0;
        if(lowByte>6) lowByte = 2;
        ui->comboBoxSpeed2->setCurrentIndex(lowByte);
        ui->comboBoxParity2->setCurrentIndex(parity);
        ui->comboBoxStopBits2->setCurrentIndex(stopbits);
        offset+=2;
    }
}

QByteArray DialogPLCConfig::getConfig() const
{
    return conf;
}

void DialogPLCConfig::setCurrentIP(const QString &value)
{
    ui->lineEditCurIP->setText(value);
}

void DialogPLCConfig::setPeriodms(int value)
{
    Q_UNUSED(value)
}

int DialogPLCConfig::getPeriodms() const
{
    return 0;
}

void DialogPLCConfig::on_pushButtonRead_clicked()
{
    ui->lineEditInfo->setText("Чтение настроек");
    repaint();
    QThread::msleep(200);
    EthConfigReader* reader = new EthConfigReader(ui->lineEditCurIP->text());
    connect(reader,&EthConfigReader::info,[this](const QString &value){
        ui->lineEditInfo->setText(value);
        repaint();
    });
    connect(reader,&EthConfigReader::readResult,[this](std::vector<quint16> value){
        QByteArray conf = convertFromPLCMap(value);
        setConfig(conf);
        ui->lineEditInfo->setText("Настройки успешно прочитаны");
        repaint();
    });
    QTimer::singleShot(5000,this,[reader](){delete reader;});
}


void DialogPLCConfig::on_pushButtonWrite_clicked()
{
    if(checkCurrentConfig()==false) {
        ui->lineEditInfo->setText("Проверьте корректность IP настроек");
    }else {
        ui->lineEditInfo->setText("Запись настроек");
        repaint();
        QThread::msleep(200);
        std::vector<quint16> conf = convertCurrentToPLCMap();
        if(conf.size()!=0) {
            // запись настроек
            EthConfigWriter* writer = new EthConfigWriter(ui->lineEditCurIP->text(),conf);
            connect(writer,&EthConfigWriter::info,[this](const QString &value){
                ui->lineEditInfo->setText(value);
                repaint();
            });
            connect(writer,&EthConfigWriter::success,[this](){;
                ui->lineEditInfo->setText("Настройки успешно записаны");
                repaint();
            });
            QTimer::singleShot(5000,this,[writer](){delete writer;});
        }else {
            ui->lineEditInfo->setText("Не удалось распознать настройки");
        }
    }
}

void DialogPLCConfig::on_pushButtonSave_clicked()
{
    if(checkCurrentConfig()==false) {
        ui->lineEditInfo->setText("Проверьте корректность IP настроек");
    }else {
        std::vector<quint16> vect_conf = convertCurrentToPLCMap();
        QByteArray res = convertFromPLCMap(vect_conf);
        emit saveConf(res);
        ui->lineEditInfo->setText("Текущие настройки прикреплены к проекту");
    }

}
