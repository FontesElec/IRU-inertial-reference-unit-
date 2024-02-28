#include "stm32f10x.h"                  // Device header
//========================================== СЛУЖЕБНЫЕ МАКРОСЫ ===========================================

#define SLAVE_I2C_ADR           23                              //адрес платы датчика по I2C
#define N_PARAM_TO_I2C          13                              //сколько значений отправим на запрос
#define SCALE                   10                              //период отсечки в милисекундах
#define M_PI                    3.14159265358979323846          //потому что константа)))
#define SPI_DR_8bit             (*(uint8_t*)&(SPI1->DR))        //отправка данных в регистр SPI в режиме 8 бит
#define CS_ON                   (GPIOA->BSRR|=GPIO_BSRR_BR4)    //выбор слэйва, запуск обмена по SPI
#define CS_OFF                  (GPIOA->BSRR|=GPIO_BSRR_BS4)    //окончание работы по SPI, сброс слэйва
#define SAFE_TIME               1400                            //период (в мкс), перед концом такта, для безопасного отключения шины I2C


//------------------------------------------ СЛУЖЕБНЫЕ МАКРОСЫ -------------------------------------------

//=================================== ПЕРЕМЕННЫЕ ===================================

uint32_t i;//счётчики
int16_t spi_receive=0;//сюда льём данные из регистра приёма SPI
int16_t Wx,Wy,Wz; //параметры датчиков
double ax,ay,az; //углы
double pitch,roll,heading;//углы тангажа, крена и курса
double pitch_n, roll_n, heading_m;//углы тангажа и крена, выведенные через параметры ускорения, магнитный курс
int8_t lock_cycle=1;//флаг перехода на прерывание по времени
int8_t final_of_cycle=0;//флаг завершения расчётов в цикле
double g0, g1, g2, g3; //старые настройки кватерниона
double gi0,gi1,gi2,gi3; //новые значения для кватерниона поворота
double gc0, gc1, gc2, gc3; //кватернион поворота через параметры ДУС-а
double x0, x1, x2, x3;/*коэффициенты кватерниона поворота X 
(используем для получения локального поворота с ДУС и углового положения через акселерометр)*/
double N; //модуль ускорения
double Nx, Ny, Nz; //Значения ускорений с датчиков
double Nxg, Nyg, Nzg;//Ускорения, спроецированые на оси инерциальной СК 
double Bx, By, Bz;//Индукция по осям
double Bxg,Bzg; //проекции значений индукции датчика на инерциальные оси
double kalm_k=0.1;//Коэффициент Калмана
double kalm_k_m=0.08; //коэффициент Калмана для магнитометра
double magnet_correction=0.08;//Жёсткость корректировки по магнитному курсу
double filter_k=0.98;//Фильтрация, 1 - полностью данные от ДУС, 0 - полностью от акселей
uint8_t n_byte_i2c=0;//номер байта на отправку
uint8_t data_i2c_out[18];//данные под передачу по i2c


//----------------------------------- ПЕРЕМЕННЫЕ -----------------------------------