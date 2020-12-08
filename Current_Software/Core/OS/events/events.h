/*
 * events.h
 *
 *  Created on: 13 sept. 2020
 *      Author: Th�o
 */


#ifndef EVENTS_EVENTS_H_
#define EVENTS_EVENTS_H_

#include "stm32f4xx_hal.h"
#include "mask_def_enum.h"
#include "mask.h"
#include "../system_d.h"


#define EVENT_NB_MASK_PER_EVENT_MAX 7

typedef enum events_id_main_e{
	//Event list
	EVENT_MAIN_GYRO_INIT_OK,
	EVENT_MAIN_ACC_INIT_OK,
	EVENT_MAIN_COUNT
}events_id_main_e;		//Events déclenchés en tâche de fond

typedef enum events_id_it_e{
	//Event list
	EVENT_IT_GYRO_DATA_READY,
	EVENT_IT_ACC_DATA_READY,
	EVENT_IT_ORIENTATION_UPDATE,
	EVENT_IT_COUNT
}events_id_it_e;		//Event déclenchés en it

typedef enum{
	EVENT_TYPE_HIGH_LVL,
	EVENT_TYPE_ERROR
}Event_type_t;

typedef enum Event_state_e{
	EVENT_ENABLED,
	EVENT_DISABLED
}Event_state_e;

typedef struct{
	Mask_t mask_and[EVENT_NB_MASK_PER_EVENT_MAX] ;	//Masques de conditions et
	Mask_t mask_or[EVENT_NB_MASK_PER_EVENT_MAX] ;	//Masques de déclenchement
	Mask_t mask_not[EVENT_NB_MASK_PER_EVENT_MAX] ; 	//Masques de conditions non
	uint32_t nb_mask ;								//Nombre de paires de masque
	Event_state_e state ;
	void (*function)(mask_def_ids_t mask_id);
}Event_t;


//Init event
void EVENT_init(system_t * sys_, TIM_HandleTypeDef * htim_event_);

//Process events handler
void EVENT_process_events_main(void);
void EVENT_process_events_it(void);

//Events timmer it to call
void EVENT_timmer_callback(TIM_HandleTypeDef * htim);


//Set and clean flags from main only
bool_e EVENT_Set_flag(Flags_t flag);
bool_e EVENT_Clean_flag(Flags_t flag);

//Set and clean flags from it (between disable_irq : : enable_irq)
bool_e EVENT_set_flag_it(Flags_t flag);
bool_e EVENT_clean_flag_it(Flags_t flag);



#endif /* EVENTS_EVENTS_H_ */
