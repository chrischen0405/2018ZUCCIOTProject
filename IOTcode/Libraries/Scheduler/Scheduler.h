/**
  ******************************************************************************
  * @file    xxx.h
  * @author  Waveshare Team
  * @version 
  * @date    xx-xx-2014
  * @brief   xxxxx.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, WAVESHARE SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  ******************************************************************************
  */


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SCH_H
#define __SCH_H 

/* Includes ------------------------------------------------------------------*/
#include "MacroAndConst.h"


#ifndef SCH_MAX_TASKS
	#define SCH_MAX_TASKS     10
#endif


/* Exported types ------------------------------------------------------------*/
typedef uint8_t task_id_t;

/* Exported constants --------------------------------------------------------*/

// The maximum number of tasks required at any one time
// during the execution of the program
// MUST BE ADJUSTED FOR EACH NEW PROJECT

/* Exported macro ------------------------------------------------------------*/

#define SCH_PRE_EMPTIVE_TASK             0
#define SCH_CO_OPERATIVE_TASK            1

#define ERROR_SCH_TOO_MANY_TASKS         1
#define ERROR_SCH_CANNOT_DELETE_TASK     2

/* Exported functions ------------------------------------------------------- */
extern void sch_init(void);
extern void sch_start(void);
extern void sch_update(void);
extern void sch_dispatch_tasks(void);
extern task_id_t sch_add_tasks(void (*pTask)(), // Task function pointer
						uint16_t hwDelay,   // Num ticks 'til task first runs
						uint16_t hwPeriod,  // Num ticks between repeat runs
						uint8_t chTaskType  // Co_op / pre_emp
						);
extern bool sch_delete_task(task_id_t tTaskIndex);
extern void sch_report_status(void);

#endif
/*-------------------------------END OF FILE-------------------------------*/

