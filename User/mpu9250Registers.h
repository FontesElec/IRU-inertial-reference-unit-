//=========================================== КАРТА РЕГИСТРОВ ДАТЧИКА =====================================

#define GYRO_OFFSET_WZ          19                              //адрес регистра смещения гироскопа на оси X (в ск датчика) (MSB)
#define GYRO_OFFSET_WX          21                              //адрес регистра смещения гироскопа на оси Y (в ск датчика) (MSB)
#define GYRO_OFFSET_WY          23                              //адрес регистра смещения гироскопа на оси Z (в ск датчика) (MSB)
#define CONFIG                  26                              //DLPF фильтры и FSYNC
#define GYRO_CONFIG             27                              //конфигурация гироскопа
#define ACCEL_CONFIG            28                              //конфигурация акселерометра
#define ACCEL_CONFIG_2          29                              //конфигурация акселерометра
#define INNER_I2C_MST_CTRL      36                              //инициализация внутренней шины мастера датчика MPU9250
#define I2C_SLV0_ADDR           37                              //адрес слэйва внутренней шины MPU
#define I2C_SLV0_REG            38                              //сюда пишем стартовый адрес регистра
#define I2C_SLV0_CTRL           39                              //инициализация слэйва
#define BYPASS_EN_CFG           55                              //обход шины I2C
#define PARAM_Nz                59                              //младший адрес регистра параметра Nx (в ск датчика)
#define PARAM_Nx                61                              //младший адрес регистра параметра Ny (в ск датчика)
#define PARAM_Ny                63                              //младший адрес регистра параметра Nz (в ск датчика)
#define PARAM_Wz                67                              //младший адрес регистра параметра Wx (в ск датчика)
#define PARAM_Wx                69                              //младший адрес регистра параметра Wy (в ск датчика)
#define PARAM_Wy                71                              //младший адрес регистра параметра Wz (в ск датчика)
#define EXT_SENS_DATA_0         73                              //данные с датчика
#define EXT_SENS_DATA_2         75                              //данные с датчика
#define EXT_SENS_DATA_4         77                              //данные с датчика
#define I2C_SLV_D_OUT           99                              //данные на отправку в слэйв 0
#define USER_CONTROL            106                             //пользовательские настройки
#define POWER_MGR_1             107                             //управление питанием 1й регистр
#define POWER_MGR_2             108                             //управление питанием 2й регистр
#define ACCEL_OFFSET_NZ         119                             //регистр смещения акселерометра по оси X (в ск датчика)
#define ACCEL_OFFSET_NX         122                             //регистр смещения акселерометра по оси Y (в ск датчика)
#define ACCEL_OFFSET_NY         125                             //регистр смещения акселерометра по оси Z (в ск датчика)


#define MAG_STAT_2              9                               //регистр статуса
#define MAG_CTRL_1              10                              //внутренний регистр магнитометра
#define MAG_CTRL_2              11                              //внутренний регистр магнитометра


//------------------------------------------ КАРТА РЕГИСТРОВ ДАТЧИКА ---------------------------------------