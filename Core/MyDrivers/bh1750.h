/*
 * bh1750.h
 *
 *  Created on: Jul 16, 2026
 *      Author: PABLO
 */

#ifndef MYDRIVERS_BH1750_H_
#define MYDRIVERS_BH1750_H_

#include <stdint.h>



void BH1750_Init(void);  // Initializes sensor

uint16_t BH1750_ReadData(void);  // Obtains output from sensor

float BH1750_ReadLux(void);  // Obtains Lux from sensor

uint32_t BHT1750_AdaptiveSampling(float value, float prev);  // Implements the logic for deciding the ammount of time the microcontroller sleeps




#endif /* MYDRIVERS_BH1750_H_ */
