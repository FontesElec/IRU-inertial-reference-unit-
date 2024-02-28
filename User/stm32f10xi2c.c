
void I2C_Init(void)
{
    RCC->APB2ENR|=RCC_APB2ENR_IOPBEN;                           // запитка порта B
    RCC->APB1ENR|=RCC_APB1ENR_I2C1EN;                       // тактирование на I2C1
         
/*(ИСПРАВЛЕНО принудительным рестартом мастера) ВНИМАНИЕ, ПЕРЕД КОНФИГОМ ПОРТА В ОТКРЫТЫЙ СТОК ОБЯЗАТЕЛЬНО ПОДАТЬ ЕДИНИЦУ НА SCL И SCK, ТАКИМ ОБРАЗОМ ЗАРЯЖАЕМ ЗАТВОРЫ ПОРТА,
  ИНАЧЕ ПРИ ПЕРЕКЛЮЧЕНИИ В ОТКРЫТЫЙ СТОК ПРОИЗОЙДЁТ ПРОСАДКА НАПРЯЖЕНИЯ ЛИНИЙ ШИНЫ И ЗАВИСАНИЕ*/
    
    
    //GPIOB->BSRR|=GPIO_BSRR_BS6|GPIO_BSRR_BS7;               // заряжаем порт (исправилось при принудительных рестартах мастера)
    GPIOB->CRL &= ~(GPIO_CRL_CNF6|GPIO_CRL_CNF7);			// Вычищаем CNF биты 
    GPIOB->CRL |= GPIO_CRL_MODE6|GPIO_CRL_MODE7;			// частота работы шины APB 50 мГц
    GPIOB->CRL |= GPIO_CRL_CNF6|GPIO_CRL_CNF7;			    // PB6 и PB7 в режим альтернативной функции

    I2C1->CR1&=~I2C_CR1_PE;                                     //останов I2C
    while (I2C1->CR1&I2C_CR1_PE){};                             //ждём подтверждения останова
    
    I2C1->CR1&=~(I2C_CR1_SMBUS|I2C_CR1_NOSTRETCH);              //шина в режим I2C, включена задержка линии тактирования
    I2C1->CR2|=0b110010|                                        //под частоту шины 50мГц      
               I2C_CR2_ITEVTEN|                                 //прерывания по событиям
               I2C_CR2_ITERREN;                                 //прерывния по ошибкам на шине
        
    I2C1->CCR|=I2C_CCR_FS|I2C_CCR_DUTY;                         //fast mode, скважность 16/9
    I2C1->CCR|=I2C_CCR_CCR&5;                                   //400 кГц
    I2C1->TRISE=6;                                              //для 100нс времени нарастания фронта
    
    I2C1->OAR1&=~I2C_OAR1_ADDMODE;                              //режим адресации 7 бит
    I2C1->OAR1|=(SLAVE_I2C_ADR<<1);                             //записываем адрес слэйва
        
    NVIC_EnableIRQ(I2C1_EV_IRQn);                           // разрешить прерывание по событиям на шине I2C
    NVIC_EnableIRQ(I2C1_ER_IRQn);                           // разрешить прерывание по ошибкам на шине I2C
}

void RESTART_I2C(void)
{
   I2C1->CR1&=~I2C_CR1_ACK;
   I2C1->CR1&=~I2C_CR1_PE;                                   //останов I2C
         
   n_byte_i2c=0;
       
   I2C1->CR1|=I2C_CR1_PE;                                    //запуск I2C
   while (!(I2C1->CR1&I2C_CR1_PE)){};                        //ждём подтверждения запуска    
}


void I2C_ALLOW_ANSWER(void)
{
                   
    I2C1->CR1|=I2C_CR1_PE;                                      //запуск I2C
    while (!(I2C1->CR1&I2C_CR1_PE)){};                          //ждём подтверждения запуска
                      
    I2C1->CR1|=I2C_CR1_ACK;                                     //отвечаем на свой адрес и дальнейшие операции (писать эту строку только после PE)
}    

void I2C_RESTRICT_ANSWER(void)
{    
            
    I2C1->CR1&=~I2C_CR1_ACK;                                    //перестаём откликаться на запросы мастера
    I2C1->CR1&=~I2C_CR1_PE;                                     //останов I2C 
}
