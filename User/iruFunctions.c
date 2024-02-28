void GET_QUATERNION (double ay_loc, double ax_loc, double az_loc)
{
  double cos_heading, sin_heading, cos_roll, sin_roll, cos_pitch, sin_pitch;  
  
  cos_heading=cos(M_PI*ay_loc/360);
  sin_heading=sin(M_PI*ay_loc/360);
  cos_roll=cos(M_PI*ax_loc/360);
  sin_roll=sin(M_PI*ax_loc/360);
  cos_pitch=cos(M_PI*az_loc/360);
  sin_pitch=sin(M_PI*az_loc/360);

  x0=cos_heading*cos_pitch*cos_roll-sin_heading*sin_pitch*sin_roll;             //составляющие кватерниона поворота
  x1=cos_heading*cos_pitch*sin_roll+sin_heading*sin_pitch*cos_roll;
  x2=sin_heading*cos_pitch*cos_roll+cos_heading*sin_pitch*sin_roll;
  x3=cos_heading*sin_pitch*cos_roll-sin_heading*cos_pitch*sin_roll;
    
}    

void DATA_SPLITTING(void)
{
   
    
    
   int16_t data_i2c_in[N_PARAM_TO_I2C]={(int16_t)(Nx*10000),\
                                        (int16_t)(Ny*10000),\
                                        (int16_t)(Nz*10000),\
                                        (int16_t)(N*10000),\
                                        (int16_t)(heading*10),\
                                        (int16_t)(roll*10),\
                                        (int16_t)(pitch*10),\
                                        (int16_t)(roll_n*10),\
                                        (int16_t)(pitch_n*10),\
                                        (int16_t)(Bx),\
                                        (int16_t)(By),\
                                        (int16_t)(Bz),\
                                        (int16_t)(heading_m*10)};                  //все данные, что отправим по i2c
    
    
    //int16_t data_i2c_in[13]={1200,1201,1202,1203,1204,1205,1206,1207,1208,1209,1210,1211,1212};
    //int16_t data_i2c_in[9]={1,2,3,4,5,6,7,8,9};
    for(i=0;i<N_PARAM_TO_I2C;i++)
    {
        data_i2c_out[2*i]=(data_i2c_in[i]>>8);
        data_i2c_out[2*i+1]=(data_i2c_in[i]);
    }
}
