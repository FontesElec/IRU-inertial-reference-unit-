//=============================== БЛОК ПРЕРЫВАНИЙ ==================================

//========================= РАБОТА В ТАЙМЕРЕ ПО ОТСЕЧКЕ ============================

void SysTick_Handler(void)
{
    I2C1->CR1&=~I2C_CR1_ACK;
    I2C1->CR1&=~I2C_CR1_PE;                                   //останов I2C
    
    n_byte_i2c=0;
   
    if(final_of_cycle) //успешно посчитал за время цикла
    {
        lock_cycle=0;//сбрасываем флаг цикла
        final_of_cycle=0;//сбрасываем флаг готовности расчётов
        SysTick->VAL=0;//сбросим текущее значение счётчика в SYST_CVR
        
    }
    else    //если к концу цикла не успел посчитать
    {
        
        SysTick->CTRL &= ~SysTick_CTRL_TICKINT_Msk; //запрет прерывания по таймеру
    }
}

//------------------------- РАБОТА В ТАЙМЕРЕ ПО ОТСЕЧКЕ ----------------------------

//============================ СОБЫТИЯ НА ШИНЕ I2C =================================
void I2C1_EV_IRQHandler(void)
{
  
   if(I2C1->SR1 & I2C_SR1_STOPF)
   {
       (void) I2C1->SR1;                                          //состояние сбрасывается через чтение регистра статуса
       (void) I2C1->SR2;                                          //состояние сбрасывается через чтение регистра статуса
       RESTART_I2C();
       I2C1->CR1|=I2C_CR1_ACK;
   }
   else if (I2C1->SR2& I2C_SR2_TRA)                                    //если мастер запросил данные
        {
            (void) I2C1->SR1;                                          //состояние сбрасывается через чтение регистра статуса
            (void) I2C1->SR2;                                          //состояние сбрасывается через чтение регистра статуса
            I2C1->DR=data_i2c_out[n_byte_i2c];
            n_byte_i2c++;
            
            
           
            
            
        }
        else                                                           //если мастер отправляет данные
        {
            (void) I2C1->SR1;                                          //состояние сбрасывается через чтение регистра статуса
            (void) I2C1->SR2;                                          //состояние сбрасывается через чтение регистра статуса
            (void) I2C1->DR;
        }
}

//--------------------------- СОБЫТИЯ НА ШИНЕ I2C ---------------------------------


//=========================== ОШИБКИ НА ШИНЕ I2C ==================================
void I2C1_ER_IRQHandler(void)
{
  n_byte_i2c=0;
  RESTART_I2C();
}

//--------------------------- ОШИБКИ НА ШИНЕ I2C -----------------------------------


//-------------------------------- БЛОК ПРЕРЫВАНИЙ ---------------------------------
