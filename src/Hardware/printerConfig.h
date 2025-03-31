//
// Created by keqiu on 25-3-31.
//

#ifndef PRINTERCONFIG_H
#define PRINTERCONFIG_H

#include <Arduino.h>

/*6个STB*/
#define STB6_Pin 32
#define STB5_Pin 33
#define STB4_Pin 25
#define STB3_Pin 26
#define STB2_Pin 27
#define STB1_Pin 14
#define ALL_STB_NUM 0xFF //所有通道选中

#define kAddTime 0.001 //0.0012~0.0015


#define LAT_Pin 12 //锁存引脚
#define VH_EN_Pin 17 //电源引脚

#define PRINT_TIME 2700 //打印时间，即对应STB通道高电平保持时间，选择在1.5ms至3ms即可，过长会损坏打印头。
#define PRINT_WAIT_TIME 200 //冷却时间200us,问AI得到的值，手册上也没写
#define LAT_TIME 1 //锁存时间1us,手册要求LAT建立时间最少100ns,然后再保持50ns







#endif //PRINTERCONFIG_H
