/*
 * events.c
 *
 *  Created on: 13 sept. 2020
 *      Author: Th�o
 */

#include "mask_def.h"
#include "flags.h"
#include "events.h"
#include "../scheduler/scheduler.h"

//Ensemble des flags
volatile static Mask_t flags ;
volatile static bool_e new_flag_it = FALSE ;
static TIM_HandleTypeDef * htim_event ;
static system_t * sys ;

//Si false, init du module event
static bool_e initialized = FALSE ;




#define DEFINE_EVENT(event_function_param, nb_mask_param){  	\
		.function = event_function_param ,						\
		.nb_mask = nb_mask_param , 								\
		.state = EVENT_ENABLE									\
}


//	----------------------	Events main	----------------------

static void gyro_init_ok(mask_def_ids_t mask_id){
	//On dit que le gyro est ok et qu'il effectu des mesures
	EVENT_Set_flag(FLAG_GYRO_OK);
	EVENT_Set_flag(FLAG_GYRO_READING);
	//On lance la tâche d'update du gyro
	SCHEDULER_enable_task(TASK_GYRO_UPDATE, TRUE);
}


//Attention !!!! nb_mask <= EVENT_NB_MASK_PER_EVENT_MAX else failure :)
static Event_t events_main[EVENT_MAIN_COUNT] ={
		//Events array
		[EVENT_MAIN_GYRO_INIT_OK] = DEFINE_EVENT(gyro_init_ok, 1)
};


//	----------------------	Events it	----------------------

static void gyro_dma_done_func(mask_def_ids_t mask_id){
	EVENT_Clean_flag(FLAG_GYRO_DMA_DONE);
	GYRO_dma_done(&sys->sensors.gyro);
}

static void gyro_data_ready_func(mask_def_ids_t mask_id){

}



//Définitions des events
//Attention !!!! nb_mask <= EVENT_NB_MASK_PER_EVENT_MAX sinon d�rapage :)
static Event_t events_it[EVENT_IT_COUNT] ={
		//Events array
		[EVENT_IT_GYRO_DMA_DONE] = DEFINE_EVENT(gyro_dma_done_func, 1),
		[EVENT_IT_GYRO_DATA_READY] = DEFINE_EVENT(gyro_data_ready_func, 0)
};

//Déclenchement des events en main
void EVENT_process_events_main(){
	if(initialized){
	//Pout chaque event
		for(uint32_t e = 0; e < EVENT_MAIN_COUNT; e ++){

			uint32_t m = 0 ;
			bool_e function_did_run_once = FALSE ;
			//On test chaque paires de masque
			while(m < events_main[e].nb_mask && !function_did_run_once && events_main[e].state == EVENT_ENABLE){
				if(Mask_test_and(events_main[e].mask_and[m], flags)){		//Mask and test
					if(Mask_test_or(events_main[e].mask_or[m], flags)){		//Mask or test
						if(!Mask_test_or(events_main[e].mask_not[m], flags)){	//Mask not test
							events_main[e].function(m);
							function_did_run_once = TRUE ;
						}
					}
				}
				m++ ;
			}

		}
	}
}

//Déclenchement des events en it
void EVENT_process_events_it(){
	if(initialized){
	//Pout chaque event
		for(uint32_t e = 0; e < EVENT_IT_COUNT; e ++){
			uint32_t m = 0 ;
			bool_e function_did_run_once = FALSE ;
			//On test chaque paires de masque
			while(m < events_it[e].nb_mask && !function_did_run_once){
				if(Mask_test_and(events_it[e].mask_and[m], flags)){		//Mask and test
					if(Mask_test_or(events_it[e].mask_or[m], flags)){		//Mask or test
						if(!Mask_test_or(events_it[e].mask_not[m], flags)){	//Mask not test
							events_it[e].function(m);
							function_did_run_once = TRUE ;
						}
					}
				}
				m++ ;

			}
		}
	}
}

//Set et clean depuis le main
bool_e EVENT_Set_flag(Flags_t flag){
	bool_e to_ret ;
	__disable_irq();
	to_ret = MASK_set_flag(&flags, flag);		//It désactivitées pour éviter la réentrance
	__enable_irq();
	return to_ret ;
}
bool_e EVENT_Clean_flag(Flags_t flag){
	bool_e to_ret ;
	__disable_irq();
	to_ret = MASK_clean_flag(&flags, flag);		//It désactivitées pour éviter la réentrance
	__enable_irq();
	return to_ret ;
}

//Set et clean de flag depuis it
bool_e EVENT_set_flag_it(Flags_t flag){
	new_flag_it = TRUE ;
	return MASK_set_flag(&flags, flag);
}
bool_e EVENT_clean_flag_it(Flags_t flag){
	new_flag_it = TRUE ;
	return MASK_clean_flag(&flags, flag);
}

void EVENT_timmer_callback(TIM_HandleTypeDef * htim){
	if(new_flag_it){
		if(htim == htim_event){
			new_flag_it = FALSE ;
			EVENT_process_events_it();
		}
	}
}

void EVENT_init(system_t * sys_, TIM_HandleTypeDef * htim_event_){
	initialized = TRUE ;
	sys =sys_;

	//Configuration des mask associés aux events
	mask_def_events_it_init(events_it);
	mask_def_events_main_init(events_main);

	//On lance le timmer dédié à l'it event
	if(htim_event_ != NULL){
		htim_event = htim_event_ ;
		HAL_TIM_Base_Start_IT(htim_event);
	}






}



