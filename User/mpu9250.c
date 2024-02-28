#include "mpu9250Registers.h"
#include "mpu9250UserSettings.h"


void MPU9250_SPI_INIT (void)
{
    /*В ЗАПРОСЕ, В КАЧЕСТВЕ ФЛАГА ГОТОВНОСТИ БУДЕМ ИСПОЛЬЗОВАТЬ ФЛАГ ЗАПОЛНЕНИЯ БУФЕРА ПРИЁМА, 
    ЭТО ГАРАНТИРУЕТ, ЧТО ВСЕ ДАННЫЕ ТОЧНО ПЕРЕДАЛИСЬ И ОШИБОК НЕ БУДЕТ, 
    В СЛУЧАЕ С ОТСЛЕЖИВАНИЕМ TXE ВОЗНИКАЮТ ОШИБКИ РАБОТЫ*/
    
    RCC->APB2ENR|=RCC_APB2ENR_IOPAEN| RCC_APB2ENR_AFIOEN;//тактирование порта А и запуск тактирования на альтернативную функциюч
    GPIOA->CRL&= ~(GPIO_CRL_CNF2|GPIO_CRL_CNF4|GPIO_CRL_CNF5|GPIO_CRL_CNF6|GPIO_CRL_CNF7);//сбрасываем в 0 регистры конфига, иначе не работает	  
    GPIOA->CRL|=GPIO_CRL_MODE2_0|GPIO_CRL_MODE4_0|GPIO_CRL_MODE5_0|GPIO_CRL_MODE6_0|GPIO_CRL_MODE7_0;//10МГц на A4-A7
    GPIOA->CRL|=GPIO_CRL_CNF5_1|GPIO_CRL_CNF6_1|GPIO_CRL_CNF7_1;//порты А5-А7 на альтернативные функции, A4 на выход
    
    GPIOA->BSRR|=GPIO_BSRR_BS2;//установка 1 на SS(PA4)
    GPIOA->BSRR|=GPIO_BSRR_BS4;//установка 1 на SS(PA4)
        
    RCC->APB2ENR|=RCC_APB2ENR_SPI1EN;//тактирование на SPI1
    
    
    SPI1->CR1|=SPI_CR1_BR_2|SPI_CR1_BR_1|SPI_CR1_MSTR|SPI_CR1_SSI|SPI_CR1_CPOL|SPI_CR1_CPHA;//включить SPI1, режим мастер, f/4 , software slave management
    SPI1->CR2|=SPI_CR2_SSOE;
    SPI1->CR1|=SPI_CR1_SPE;//включить spi
    
    //SPI1->SR=0x0002;//значение статус-регистра по умолчанию
    
    for (i=0; i<2000000; i++);//ждём включения SPI слэйва (иначе отправит нули на первые запросы)
    


//============================== НАСТРОЙКА ПАРАМЕТРОВ ДАТЧИКА =============================

    SPI_SET_PARAM_8BIT(POWER_MGR_1,HARD_RESET);
    for(i=10000;i>0;i--);//ждём
    SPI_SET_PARAM_8BIT(POWER_MGR_1,1);
    for(i=10000;i>0;i--);//ждём
    SPI_SET_PARAM_8BIT(POWER_MGR_2,PWR_MGR_ALL_DISABLE);
    for(i=10000;i>0;i--);//ждём
    SPI_SET_PARAM_8BIT(POWER_MGR_2,PWR_MGR_ALL_ENABLE);
    for(i=10000;i>0;i--);//ждём
    SPI_SET_PARAM_8BIT (CONFIG,0x02);
    
    
 //НАСТРОЙКА ВНУТРЕННЕй I2C ШИНЫ ДЛЯ РАБОТЫ МАГНЕТОМЕТРА
 
    SPI_SET_PARAM_8BIT (BYPASS_EN_CFG,0x22);
    SPI_SET_PARAM_8BIT (USER_CONTROL,(INNER_I2C_MSTR_EN));                      //включаем мастер I2C
    SPI_SET_PARAM_8BIT (INNER_I2C_MST_CTRL,(STOP_BETW_READ|INNER_I2C_F_400));   //задали частоту внутренней шины I2C, СТОП между пакетами
        
    SPI_SET_PARAM_8BIT (I2C_SLV0_ADDR,MAG_I2C_SLV_ADDR);                        //повесили магнитометр на slave0
    
    
    SPI_SET_PARAM_8BIT (I2C_SLV0_REG,MAG_CTRL_2);                               //загружаем адрес регистра AK8963
    SPI_SET_PARAM_8BIT (I2C_SLV_D_OUT,MAG_RESET);                               //сброс магнитометра   
    
    SPI_SET_PARAM_8BIT (I2C_SLV0_CTRL,(I2C_SLV0_EN|I2C_SLV0_LENG_1));           //доступ к slave0, настройка длины пакета данных
    for(i=10000;i>0;i--);//ждём
        
    SPI_SET_PARAM_8BIT (I2C_SLV0_REG,MAG_CTRL_1);                               //загружаем адрес регистра AK8963
    SPI_SET_PARAM_8BIT (I2C_SLV_D_OUT,(I2C_16BIT_OUT|CONT_MEAS_2));             //16 бит и постоянно мерить (режим 2)
    SPI_SET_PARAM_8BIT (I2C_SLV0_CTRL,(I2C_SLV0_EN|I2C_SLV0_LENG_1));           //доступ к slave0, настройка длины пакета данных
    for(i=1000;i>0;i--);//ждём
    
    
    //SPI_SET_PARAM_8BIT (I2C_SLV0_ADDR,(MAG_I2C_SLV_ADDR|I2C_READ_SLV));         //магинтометр на slave0 в режиме чтения
    
    
 //НАСТРОЙКА РАБОТЫ ГИРОСКОПА и АКСЕЛЕРОМЕТРА
 
    SPI_SET_PARAM_8BIT (GYRO_CONFIG,GYRO_CONF_PARAM);
    SPI_SET_PARAM_8BIT (ACCEL_CONFIG,ACCEL_CONF_PARAM);
    SPI_SET_PARAM_8BIT (ACCEL_CONFIG_2,ACCEL_CONF_PARAM_2);
   

//------------------------------ НАСТРОЙКА ПАРАМЕТРОВ ДАТЧИКА -----------------------------

//===================================== КАЛИБРОВКА ДУС ===================================
 

/*КАЛИБРОВКУ АКСЕЛЛЕРОМЕТРА ЛУЧШЕ ПРОИЗВОДИТЬ ТОЛЬКО ПОСЛЕ ОБНУЛЕНИЯ СОДЕРЖИМОГО
РЕГИСТРОВ СМЕЩЕНИЯ, ВЕРОЯТНО, В РЕГИСТРЫ УЖЕ ЗАПИСАНЫ ЗНАЧЕНИЯ ПО УМОЛЧАНИЮ*/

/*КАЛИБРОВКА ПРОИСХОДИТ ПРИ ЗАПУСКЕ ПЛАТЫ В НОРМАЛЬНОМ ПОЛОЖЕНИИ ВС, ПОЭТОМУ ПО ОСЯМ
ДАТЧИКА X,Y КАЛИБРОВКА ОТНОСИТЕЛЬНО 0G, ПО ОСИ Z КАЛИБРОВКА ОТНОСИТЕЛЬНО 1G*/

/*В НОВОЙ ВЕРСИИ КАЛИБРОВОЧНЫЕ КОНСТАНТЫ АКСЕЛЕРОМЕТРА ЗАПИСЫВАЕМ СРАЗУ, ОНИ НЕ ПЛАВАЮТ В ПРОЦЕССЕ*/
   
    for (i=0; i<30000; i++);//ждём включения SPI слэйва (иначе отправит нули на первые запросы)
    
    int32_t gyro_offset_x=0;//32 разряда потому что при суммировании всех погрешностей возникала ошибка
    int32_t gyro_offset_y=0;
    int32_t gyro_offset_z=0;
    
    
    
   
    SPI_SET_PARAM_16BIT(GYRO_OFFSET_WX,0);//сбрасываем предзаписаные значения
    SPI_SET_PARAM_16BIT(GYRO_OFFSET_WY,0);
    SPI_SET_PARAM_16BIT(GYRO_OFFSET_WZ,0); 
   
    for(i=0;i<10000;i++)
    {
        gyro_offset_x=gyro_offset_x+SPI_GET_PARAM(PARAM_Wx);
        gyro_offset_y=gyro_offset_y+SPI_GET_PARAM(PARAM_Wy);
        gyro_offset_z=gyro_offset_z+SPI_GET_PARAM(PARAM_Wz);
    }
   
        
    gyro_offset_x=-(((long)gyro_offset_x)/10000.0);//усредняем значение за 1000 итераций
    gyro_offset_y=-(((long)gyro_offset_y)/10000.0);
    gyro_offset_z=-(((long)gyro_offset_z)/10000.0);
   
    SPI_SET_PARAM_16BIT(ACCEL_OFFSET_NX,0);//сбрасываем предзаписаные значения
    SPI_SET_PARAM_16BIT(ACCEL_OFFSET_NY,0);
    SPI_SET_PARAM_16BIT(ACCEL_OFFSET_NZ,0);
    
    SPI_SET_PARAM_16BIT(GYRO_OFFSET_WX,(int16_t)gyro_offset_x);//заливаем параметры в регистры датчика
    SPI_SET_PARAM_16BIT(GYRO_OFFSET_WY,(int16_t)gyro_offset_y);
    SPI_SET_PARAM_16BIT(GYRO_OFFSET_WZ,(int16_t)gyro_offset_z);
    
    SPI_SET_PARAM_16BIT(ACCEL_OFFSET_NX,(-9190));//заливаем калибровочные значения акселерометра
    SPI_SET_PARAM_16BIT(ACCEL_OFFSET_NY,(9810));
    SPI_SET_PARAM_16BIT(ACCEL_OFFSET_NZ,(52)); 

    
   
    
    
 
  
//------------------------------------- КАЛИБРОВКА ДУС -----------------------------------
  
} 



int16_t SPI_GET_PARAM(uint8_t adr_reg)
{
    
    CS_ON;
    
    SPI_DR_8bit=0b10000000|adr_reg;  
    while(!(SPI1->SR & SPI_SR_RXNE));//ждём опустошение буфера передачи
    spi_receive=(uint8_t)(SPI1->DR);//закидываем ответ
    spi_receive=0;
    
    SPI_DR_8bit=0;        
    while(!(SPI1->SR & SPI_SR_RXNE));
    spi_receive=((uint8_t)(SPI1->DR))<<8;//старший байт параметра
       
    SPI_DR_8bit=0;        
    while(!(SPI1->SR & SPI_SR_RXNE));
    spi_receive=spi_receive|(uint8_t)(SPI1->DR);//и младший байт параметра
    CS_OFF;
    return spi_receive;
    
}


void SPI_SET_PARAM_8BIT (uint8_t adr,uint8_t  param)
{
    CS_ON;
    SPI_DR_8bit=adr;
    while(!(SPI1->SR & SPI_SR_RXNE));//ждём опустошение буфера передачи
    spi_receive=(uint8_t)(SPI1->DR);//читаем данные
               
    SPI_DR_8bit=param;//режим "только SPI"
    while(!(SPI1->SR & SPI_SR_RXNE));//ждём опустошение буфера передачи
    spi_receive=(uint8_t)(SPI1->DR);//закидываем ответ
    CS_OFF;
    spi_receive=0;
}

void  SPI_SET_PARAM_16BIT (uint8_t adr,int16_t param)
{
   /*ЕСЛИ НЕ СБРАСЫВАТЬ CS, АДРЕСАЦИЯ ИДЁТ АВТОМАТИЧЕСКИ ПО ИНКРЕМЕНТУ, СЛЕДУЮЩИЙ ПАРАМЕТР 
    МОЖНО ЗАДАТЬ НЕ ЗАДАВАЯ ВНОВЬ АДРЕСА*/ 
    
    CS_ON;
    SPI_DR_8bit=adr;
    while(!(SPI1->SR & SPI_SR_RXNE));//ждём опустошение буфера передачи
    spi_receive=(uint8_t)(SPI1->DR);//читаем данные
               
    SPI_DR_8bit=(param>>8);//MSB
    while(!(SPI1->SR & SPI_SR_RXNE));//ждём опустошение буфера передачи
    spi_receive=(uint8_t)(SPI1->DR);//закидываем ответ
        
    SPI_DR_8bit=param;//LSB
    while(!(SPI1->SR & SPI_SR_RXNE));//ждём опустошение буфера передачи
    spi_receive=(uint8_t)(SPI1->DR);//закидываем ответ
    CS_OFF;
    spi_receive=0;
}

void GET_PARAM(void)
{
    
    int16_t Bx0,By0,Bz0; // Вспомогательные переменные для загрузки значений индукции
    
    //ПОЛУЧАЕМ ИСХОДНЫЕ ЗНАЧЕНИЯ УСКОРЕНИЙ, УГЛОВ, ИНДУКЦИИ
        
        Nx=Nx*(1-kalm_k)+kalm_k*((double)SPI_GET_PARAM(PARAM_Nx))*0.00012207;//эквивалентно значению 1/8192
        Ny=Ny*(1-kalm_k)+kalm_k*((double)SPI_GET_PARAM(PARAM_Ny))*0.00012207;
        Nz=Nz*(1-kalm_k)+kalm_k*((double)SPI_GET_PARAM(PARAM_Nz))*0.00012207;
          
        ax=ax*(1-kalm_k)+kalm_k*((double)(SPI_GET_PARAM(PARAM_Wx)))*0.00003052*SCALE;//(2000/(2^16))/1000*SCALE, сразу делим на 1000, потому что период в мс
        ay=ay*(1-kalm_k)+kalm_k*((double)(SPI_GET_PARAM(PARAM_Wy)))*0.00003052*SCALE;
        az=az*(1-kalm_k)+kalm_k*((double)(SPI_GET_PARAM(PARAM_Wz)))*0.00003052*SCALE;
                
        SPI_SET_PARAM_8BIT (I2C_SLV0_ADDR,(MAG_I2C_SLV_ADDR|I2C_READ_SLV));         //магинтометр на slave0 в режиме чтения
        
        SPI_SET_PARAM_8BIT (I2C_SLV0_REG,3);                                    //загружаем адрес регистра AK8963
        SPI_SET_PARAM_8BIT (I2C_SLV0_CTRL,(I2C_SLV0_EN|7));       //доступ к slave0, настройка длины пакета данных (почему 7?)
    
              
        Bx0=SPI_GET_PARAM (EXT_SENS_DATA_0);
        Bx0=(((int16_t)(((Bx0&0xff00)>>8)|((Bx0)<<8)))-Bx_bias); 
        Bx=Bx*(1-kalm_k_m)+kalm_k_m*((double)Bx0);
        Bz0=SPI_GET_PARAM (EXT_SENS_DATA_2);
        Bz0=(((int16_t)(((Bz0&0xff00)>>8)|((Bz0)<<8)))-Bz_bias);
        Bz=(Bz*(1-kalm_k_m)+kalm_k_m*((double)Bz0));
        By0=SPI_GET_PARAM (EXT_SENS_DATA_4);
        By0=(-((int16_t)(((By0&0xff00)>>8)|((By0)<<8)))-By_bias);
        By=By*(1-kalm_k_m)+kalm_k_m*((double)By0);
      
}



void AK8963_CALIB(void)
{
    
   //зажигаем лампу на плате
   RCC->APB2ENR|=RCC_APB2ENR_IOPCEN;//тактирование порта C
   GPIOC->CRH&= ~(GPIO_CRH_CNF13);//сбрасываем в 0 регистры конфига, иначе не работает	  
   GPIOC->CRH|=GPIO_CRH_MODE13_0;//10МГц на A4-A7    
    
   GPIOC->BSRR&=~(GPIO_BSRR_BS13);//установка 0 на SS(PA4)
    
    long Bx_min=0,Bx_max=0, By_min=0, By_max=0, Bz_min=0, Bz_max=0;
    const uint8_t sample_size=40;
    long X_maximums[sample_size],X_minimums[sample_size],Y_maximums[sample_size],Y_minimums[sample_size],Z_maximums[sample_size],Z_minimums[sample_size];
    
      
      
    //обнулим значения матриц экстремумов
    
    RESET_MATRIX(X_maximums,sample_size);
    RESET_MATRIX(Y_maximums,sample_size);
    RESET_MATRIX(Z_maximums,sample_size);
    RESET_MATRIX(X_minimums,sample_size);
    RESET_MATRIX(Y_minimums,sample_size);
    RESET_MATRIX(Z_minimums,sample_size);
    
    
     //поморгаем чтобы понять что вышли в режим точной калибровки    
    
    for(i=3;i>0;i--)
    {
        for(int p=300;p>0;p--)
        { for(int r=3000;r>0;r--);}   
        GPIOC->BSRR|=GPIO_BSRR_BR13;//установка 0 
            
        for(int p=300;p>0;p--)
        { for(int r=3000;r>0;r--){;};}   
        GPIOC->BSRR|=GPIO_BSRR_BS13;//установка 1 
    }
    
    GPIOC->BSRR|=GPIO_BSRR_BR13;//установка 0 
    //поморгали, начинаем калибровку     
    
    //калибровка компаса 
    
    for(int i=40000;i>0;i--)
    {
        //собираем данные, вращаем во всех плоскостях
       GET_PARAM();
       
       IS_IT_MAXIMUM(Bx,X_maximums,sample_size);
       IS_IT_MINIMUM(Bx,X_minimums,sample_size);
       IS_IT_MAXIMUM(Bz,Z_maximums,sample_size);
       IS_IT_MINIMUM(Bz,Z_minimums,sample_size); 
       IS_IT_MAXIMUM(By,Y_maximums,sample_size);
       IS_IT_MINIMUM(By,Y_minimums,sample_size);
        
    }
    
    //чтобы избавиться от влияния помех в виде зашкала значений возьмём для расчёта параметры из середины матрицы
    
    Bx_min=X_minimums[sample_size/2];
    By_min=Y_minimums[sample_size/2];
    Bz_min=Z_minimums[sample_size/2];
    Bx_max=X_maximums[sample_size/2];
    By_max=Y_maximums[sample_size/2];
    Bz_max=Z_maximums[sample_size/2];
    
    //получим смещения
    
    Bx_bias=(Bx_min+Bx_max)/2;
    Bz_bias=(Bz_min+Bz_max)/2;
    By_bias=(By_min+By_max)/2;
    
    
    
    
    //поморгаем чтобы понять что вышли из режима точной калибровки    
    for(i=7;i>0;i--)
    {
        for(int p=300;p>0;p--)
        { for(int r=3000;r>0;r--);}   
        GPIOC->BSRR|=GPIO_BSRR_BR13;//установка 0 
            
        for(int p=300;p>0;p--)
        { for(int r=3000;r>0;r--){;};}   
        GPIOC->BSRR|=GPIO_BSRR_BS13;//установка 1 
    }
    
    for(int p=10000;p>0;p--)
    { for(int r=3000;r>0;r--){;};}   
    GPIOC->BSRR|=GPIO_BSRR_BS13;//установка 1 
   
    //выставляем значения в обход фильтра
    double kalm_k_m_temp=kalm_k_m;
    kalm_k_m=1; //отрубаем фильтр
    
    GET_PARAM();
    
    kalm_k_m=kalm_k_m_temp; //возвращаем фильтр
        
       
    
 
}



void IS_IT_MAXIMUM(long parameter,long array[], uint8_t sample_size)
{
     if(parameter>array[sample_size-1])
     {
         long temp=parameter;
         for(int iter=0; iter<sample_size; iter++)
         {
             if(temp>array[iter])
             {
                for(int num_of_param=1;num_of_param<sample_size-iter;num_of_param++)//сравниваем все элементы матрицы с текущим и если больше,то
                {
                    array[sample_size-num_of_param]=array[sample_size-num_of_param-1]; //перебрасываем все значения максимумов ближе к хвосту
                }
                array[iter]=temp;   //    ставим новое значение максимума на место предыдущего                 
                iter=sample_size;        // если хоть раз нашли число, большее чем элементы матрицы маскимумов, то выходим из цикла                     }
             }
         }
     }
}

void IS_IT_MINIMUM(long parameter,long array[],uint8_t sample_size)
{
    if(parameter<array[sample_size-1])
     {
         long temp=parameter;
         for(int iter=0; iter<sample_size; iter++)
         {
             if(temp<array[iter])
             {
                for(int num_of_param=1;num_of_param<sample_size-iter;num_of_param++)//сравниваем все элементы матрицы с текущим и если больше,то
                {
                    array[sample_size-num_of_param]=array[sample_size-num_of_param-1]; //перебрасываем все значения максимумов ближе к хвосту
                }
                array[iter]=temp;   //    ставим новое значение максимума на место предыдущего                 
                iter=sample_size;        // если хоть раз нашли число, большее чем элементы матрицы маскимумов, то выходим из цикла                     }
             }
         }
     }
}


void RESET_MATRIX(long array[],uint8_t sample_size)
{
    for(int i=0;i<sample_size;i++)
    {
        array[i]=0;
    }
}
