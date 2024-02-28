void SysTick_Settings(void) //настройка таймера
{
    
    SysTick->LOAD = SystemCoreClock/(1000/SCALE); //загружаем в SYST_RVR значение под период в 4 мс
        
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk |        //от внутреннего генератора
                    SysTick_CTRL_ENABLE_Msk    |        //разрешить считать
                    SysTick_CTRL_TICKINT_Msk;           //разрешить прерывание от таймера
}

