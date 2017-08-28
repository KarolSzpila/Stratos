/*
 * tuner_r820t.h
 *
 *  Created on: 09.08.2017
 *      Author: Karol
 */

#ifndef TUNERS_TUNER_R820T_H_
#define TUNERS_TUNER_R820T_H_

#include <stdint.h>
#include <tuner_r82xx.h>

int r820t_init(void* dev);
int r820t_exit(void* dev);
int r820t_set_freq(void* dev, uint32_t freq);
int r820t_set_bw(void* dev, int bw);
int r820t_set_gain(void* dev, int gain);
int r820t_set_gain_mode(void* dev, int manual);


#endif /* TUNERS_TUNER_R820T_H_ */
