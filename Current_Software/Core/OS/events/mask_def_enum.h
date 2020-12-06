/*
 * mask_def_enum.h
 *
 *  Created on: 4 oct. 2020
 *      Author: Th�o
 */

#ifndef EVENTS_MASK_DEF_ENUM_H_
#define EVENTS_MASK_DEF_ENUM_H_

typedef enum{
	//MASK_DEF

	//Gyro
	MASK_GYRO_INIT_OK = 0,
	MASK_GYRO_INIT_COUNT,

	MASK_GYRO_DATA_READY = 0,
	MASK_GYRO_DATA_READY_COUNT,

	//Acc
	MASK_ACC_INIT_OK = 0,
	MASK_ACC_INIT_COUNT,

	MASK_ACC_DATA_READY = 0,
	MASK_ACC_DATA_READY_COUNT,

	MASK_DEF_LAST
}mask_def_ids_t;

#endif /* EVENTS_MASK_DEF_ENUM_H_ */
