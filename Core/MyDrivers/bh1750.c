/*
 * bh1750.c
 *
 *  Driver for the bh1750 sensor
 */

#include "bh1750.h"
#include "i2c.h"

#define BH1750_ADDR (0x23 << 1)  // Direction of I2C for sensor BH1750 (0x23 displaced one bit to the left)


uint8_t L_res_mode = 0x13;  // Low resolution continuous mode


void BH1750_Init(void){
	HAL_I2C_Master_Transmit(&hi2c1, BH1750_ADDR, &L_res_mode, 1, HAL_MAX_DELAY);
	  HAL_Delay(30);
}



uint16_t BH1750_ReadData(void){

	uint8_t sensor_values[2];

	HAL_I2C_Master_Receive(&hi2c1, BH1750_ADDR, sensor_values, 2, 100);

	return (sensor_values[0] << 8) | sensor_values[1];
}



float BH1750_ReadLux(void){

	uint16_t raw_data = BH1750_ReadData();

	return raw_data/1.2f;

}


