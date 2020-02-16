#include "adcmanager.h"


ADCManager::ADCManager()
{

}

std::vector<QString> ADCManager::getConverterFunction(PLCConfig config, int cnt)
{
    std::vector<QString> result;
    result.push_back("void calculate_adc(){\n");
    result.push_back("\tunsigned char i=0;\n");
    result.push_back("\tfloat v=0;\n");
    result.push_back("\tfor(i=0;i<"+QString::number(cnt)+";i++) {\n");
    result.push_back("\t\tv=ain_raw[i];\n");
    result.push_back("\t\tswitch(i) {\n");

    for(int i=0;i<cnt;i++) {
        result.push_back("\t\t\tcase " + QString::number(i) +":\n");

        int sensType = config.getSensorType(i);
        switch(sensType) {
            case 0:
                result.push_back("\t\t\t\tain[i]=0;\n");
                result.push_back("\t\t\t\tain_under[i]=0;\n");
                result.push_back("\t\t\t\tain_over[i]=0;\n");
                result.push_back("\t\t\t\tain_alarm[i]=0;\n");
                break;
            case 1:
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
