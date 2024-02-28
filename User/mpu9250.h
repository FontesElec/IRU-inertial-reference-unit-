#include "stm32f10x.h"                  // Device header

void     MPU9250_SPI_INIT(void);//инициализация и настройка SPI и настройка MPU9250
void     SPI_SET_PARAM_16BIT (uint8_t,int16_t);//задать настройки параметров датчика по SPI (адрес регистра, параметр)
void     SPI_SET_PARAM_8BIT (uint8_t,uint8_t);//задать настройки параметров датчика по SPI (адрес регистра, параметр)
int16_t  SPI_GET_PARAM(uint8_t);//из неё будем запрашивать у датчика нужные параметры
void     GET_PARAM(void);//получение параметров датчика по spi
void     AK8963_CALIB(void); //калибровка магнитометра
void     RESET_MATRIX(long array[],uint8_t sample_size);
void     IS_IT_MAXIMUM(long parameter,long array[],uint8_t sample_size);
void     IS_IT_MINIMUM(long parameter,long array[],uint8_t sample_size);



double    Bx_bias=0,By_bias=0,Bz_bias=0; //смещение осей магнетометра

