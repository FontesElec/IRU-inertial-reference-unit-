//==================================== ПАРАМЕТРЫ ДАТЧИКА ДЛЯ ЗАГРУЗКИ ПО SPI ===============================

#define ONLY_SPI_MODE          0b00010000               //режим "только SPI" регистра USER CONTROL
#define INNER_I2C_MSTR_EN      0b00100000               //запуск мастер I2C внутренней шины
#define INNER_I2C_F_400        13                       //частота внутренней шины I2C 400кГц
#define INNER_I2C_F_500        9                        //частота внутренней шины I2C 500кГц
#define STOP_BETW_READ         0b00001000               //останавливаем шину между чтениями
#define MAG_I2C_SLV_ADDR       0x0C                     //адрес магнитометра
#define MAG_RESET              0b00000001               //сброс магнитометра
#define I2C_READ_SLV           0b10000000               //направление на чтение из слэйва I2C
#define CONT_MEAS_1            0b00000010               //режим неприрывного измерения 1
#define CONT_MEAS_2            0b00000110               //режим неприрывного измерения 2
#define I2C_16BIT_OUT          0b00010000               //датчик отдаёт 16 бит на выходе
#define I2C_SLV0_EN            0b10000000               //получаем данные от slave0
#define I2C_SLV0_LENG_1        1                        //сколько данных будем читать из slave0 за один раз
#define I2C_SLV0_LENG_2        2                        //сколько данных будем читать из slave0 за один раз
//#define GYRO_CONF_PARAM        0b00010000               //без самоконтроля, 1000град/сек, максимальная частота обновления (для гироскопа)
#define GYRO_CONF_PARAM        0b00010001               //без самоконтроля, 1000град/сек, максимальная частота обновления (для гироскопа)
#define ACCEL_CONF_PARAM       0b00001000               //без самоконтроля, 4g (акселерометр, 1й регистр)
//#define ACCEL_CONF_PARAM_2     0b00001001               //без фильтра, максимум частоты (акселерометр, 2й регистр)
#define ACCEL_CONF_PARAM_2     0b00001000               //без фильтра, максимум частоты (акселерометр, 2й регистр)
#define HARD_RESET             0b10000000               //сброс всех настроек              
#define PWR_MGR_ALL_DISABLE    0b00111111               //отключение всех каналов измерения
#define PWR_MGR_ALL_ENABLE     0b00000000               //включение всех каналов измерения

//------------------------------------ ПАРАМЕТРЫ ДАТЧИКА ДЛЯ ЗАГРУЗКИ ПО SPI -------------------------------