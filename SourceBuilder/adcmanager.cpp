#include "adcmanager.h"
#include "plcutils.h"


ADCManager::ADCManager()
{

}

std::vector<QString> ADCManager::getConverterFunction(PLCConfig config, int cnt)
{
    std::vector<QString> result;

    quint16 used_ai = 0x0000;
    if(cnt<=16) {
        for(int i=0;i<cnt;i++) {
            int sensType = config.getSensorType(i);
            if(sensType) used_ai |= 1<<i;
        }
    }
    result.push_back(QString("uint16_t used_ai = 0x") + QString::number(used_ai,16) + ";\n\n");

    result.push_back(QString("const char* adc_names[") + QString::number(cnt) + "] = {");
    for(int i=0;i<cnt;i++) {
        result.push_back(QString("\"") + PLCUtils::getADCName(i) + "\",");
    }
    result.push_back("};\n\n");

    result.push_back(QString("uint8_t tdu[") + QString::number(cnt) + "] = {");
    for(int i=0;i<cnt;i++) {
        result.push_back(QString::number(config.getTDUCode(i)) + ",");
    }
    result.push_back("};\n\n");

    result.push_back("void calculate_adc(){\n");
    result.push_back("\tunsigned char i=0;\n");
    result.push_back("\tfloat v=0;\n");
    result.push_back("\tfor(i=0;i<"+QString::number(cnt)+";i++) {\n");
    result.push_back("\t\tv=ain_raw[i];\n");
    result.push_back("\t\tswitch(i) {\n");



    for(int i=0;i<cnt;i++) {
        result.push_back("\t\t\tcase " + QString::number(i) +":\n");
        int sensType = config.getSensorTypeCode(i);
        switch(sensType) {
            case -1:
                result.push_back("\t\t\t\tain[i]=0;\n");
                result.push_back("\t\t\t\tain_under[i]=0;\n");
                result.push_back("\t\t\t\tain_over[i]=0;\n");
                result.push_back("\t\t\t\tain_alarm[i]=0;\n");
                break;
            case 0:
                result.push_back("\t\t\t\tif(v<400) v=400;\n");
                result.push_back("\t\t\t\tif(v>2000) v=2000;\n");
                result.push_back("\t\t\t\tv = 255*v/1600-255/4.0+0.5;\n");
                result.push_back("\t\t\t\tif(v>255) v=255;\n");
                result.push_back("\t\t\t\tif(ain_raw[i]<=200) {ain[i]=0;ain_alarm[i]=1;ain_under[i]=1;ain_over[i]=0;}\n");
                result.push_back("\t\t\t\telse if(ain_raw[i]<=340) {ain[i]=0;ain_alarm[i]=0;ain_under[i]=1;ain_over[i]=0;}\n");
                result.push_back("\t\t\t\telse if(ain_raw[i]>=2200) {ain[i]=255;ain_alarm[i]=1;ain_under[i]=0;ain_over[i]=1;}\n");
                result.push_back("\t\t\t\telse if(ain_raw[i]>=2050) {ain[i]=255;ain_alarm[i]=0;ain_under[i]=0;ain_over[i]=1;}\n");
                result.push_back("\t\t\t\telse {ain[i]=(unsigned char)v;ain_alarm[i]=0;ain_under[i]=0;ain_over[i]=0;}\n");
                //result.push_back("\t\t\t\t{ain[i]=100;ain_alarm[i]=0;ain_under[i]=0;ain_over[i]=0;}\n");
                break;
            case 1:
                result.push_back("\t\t\t\tif(v>20000) v=20000;\n");
                result.push_back("\t\t\t\tv = 255*v/20000+0.5;\n");
                result.push_back("\t\t\t\tif(v>255) v=255;\n");
                result.push_back("\t\t\t\tain[i]=(unsigned char)v;\n");
                result.push_back("\t\t\t\tain_under[i]=0;\n");
                result.push_back("\t\t\t\tain_over[i]=0;\n");
                result.push_back("\t\t\t\tain_alarm[i]=0;\n");
                break;
            case 2:
                result.push_back("\t\t\t\tif(v>20000) v=20000;\n");
                result.push_back("\t\t\t\tif(v<4000) v=4000;\n");
                result.push_back("\t\t\t\tv = 255*v/16000-255/4.0+0.5;\n");
                result.push_back("\t\t\t\tif(v>255) v=255;\n");
                result.push_back("\t\t\t\tif(ain_raw[i]<=2000) {ain[i]=0;ain_alarm[i]=1;ain_under[i]=1;ain_over[i]=0;}\n");
                result.push_back("\t\t\t\telse if(ain_raw[i]<=3400) {ain[i]=0;ain_alarm[i]=0;ain_under[i]=1;ain_over[i]=0;}\n");
                result.push_back("\t\t\t\telse if(ain_raw[i]>=22000) {ain[i]=255;ain_alarm[i]=1;ain_under[i]=0;ain_over[i]=1;}\n");
                result.push_back("\t\t\t\telse if(ain_raw[i]>=20500) {ain[i]=255;ain_alarm[i]=0;ain_under[i]=0;ain_over[i]=1;}\n");
                result.push_back("\t\t\t\telse {ain[i]=(unsigned char)v;ain_alarm[i]=0;ain_under[i]=0;ain_over[i]=0;}\n");
                break;
            case 3:
                result.push_back("\t\t\t\tif(v>20000) v=20000;\n");
                result.push_back("\t\t\t\tif(v<4000) v=4000;\n");
                result.push_back("\t\t\t\tv = 255*v/8000-255/4.0+0.5;\n");
                result.push_back("\t\t\t\tif(v>255) v=255;\n");
                result.push_back("\t\t\t\tif(ain_raw[i]<=1000) {ain[i]=0;ain_alarm[i]=1;ain_under[i]=1;ain_over[i]=0;}\n");
                result.push_back("\t\t\t\telse if(ain_raw[i]<=1700) {ain[i]=0;ain_alarm[i]=0;ain_under[i]=1;ain_over[i]=0;}\n");
                result.push_back("\t\t\t\telse if(ain_raw[i]>=11000) {ain[i]=255;ain_alarm[i]=1;ain_under[i]=0;ain_over[i]=1;}\n");
                result.push_back("\t\t\t\telse if(ain_raw[i]>=10250) {ain[i]=255;ain_alarm[i]=0;ain_under[i]=0;ain_over[i]=1;}\n");
                result.push_back("\t\t\t\telse {ain[i]=(unsigned char)v;ain_alarm[i]=0;ain_under[i]=0;ain_over[i]=0;}\n");
                break;
            case 4:
                result.push_back("\t\t\t\tif(v>2500) v=2500;\n");
                result.push_back("\t\t\t\tv = 255*v/2500+0.5;\n");
                result.push_back("\t\t\t\tif(v>255) v=255;\n");
                result.push_back("\t\t\t\tain[i]=(unsigned char)v;\n");
                result.push_back("\t\t\t\tain_under[i]=0;\n");
                result.push_back("\t\t\t\tain_over[i]=0;\n");
                result.push_back("\t\t\t\tain_alarm[i]=0;\n");
                break;
            default:
                result.push_back("\t\t\t\tain[i]=0;\n");
                result.push_back("\t\t\t\tain_under[i]=0;\n");
                result.push_back("\t\t\t\tain_over[i]=0;\n");
                result.push_back("\t\t\t\tain_alarm[i]=0;\n");
                break;
        }

        result.push_back("\t\t\t\tbreak;\n");
    }

    result.push_back("\t\t};\n");
    result.push_back("\t}\n");
    result.push_back("}\n");
    result.push_back("\n");
    return result;
}
