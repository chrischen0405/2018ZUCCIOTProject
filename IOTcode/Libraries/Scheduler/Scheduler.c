/**
  ******************************************************************************
  * @file    xxx.c 
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

/* Includes ------------------------------------------------------------------*/
#include "LIB_Config.h"
#include "Scheduler.h"

/* Private typedef -----------------------------------------------------------*/

// Total memory per task is 8 bytes
typedef struct {
	void (*pTask)(void);
	uint16_t hwDelay;
	uint16_t hwPeriod;
	uint8_t chRunme;
	uint8_t chTaskType;
}sch_task_t;

typedef struct {
	uint16_t hwErrorTickCount;      // Keeps track of time since last error was recorded
	uint8_t chLastErrorCode;        // The code of the last error (reset after ~1 minute)
	uint8_t chTaskErrorCode;        // Used to display the error code
}sch_error_t;

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

// The array of tasks
static sch_task_t s_tSCHTasksTable[SCH_MAX_TASKS];

static sch_error_t s_tError;

/* Private function prototypes -----------------------------------------------*/
void sch_init(void);
void sch_start(void);
void sch_update(void);

// Core scheduler functions
void sch_dispatch_tasks(void);
task_id_t sch_add_tasks(void (* const pTask)(), // Task function pointer
							uint16_t hwDelay,   // Num ticks 'til task first runs
							uint16_t hwPeriod,  // Num ticks between repeat runs
							uint8_t chTaskType  // Co_op / pre_emp
							);
bool sch_delete_task(task_id_t tTaskIndex);
void sch_report_status(void);

static void sch_go_to_sleep(void);


/* Private functions ---------------------------------------------------------*/


/*------------------------------------------------------------------*-
sch_init()

Scheduler initialization function. Prepares scheduler
data structures and sets up timer interrupts at required rate.
Must call this function before using the scheduler.
-*------------------------------------------------------------------*/
void sch_init(void)
{
	task_id_t tIndex;

	for (tIndex = 0; tIndex < SCH_MAX_TASKS; tIndex ++) {
		sch_delete_task(tIndex);
	}

	s_tError.hwErrorTickCount = 0;
	s_tError.chLastErrorCode = 0;
	s_tError.chTaskErrorCode = 0;
}

/*------------------------------------------------------------------*-
sch_start()

Starts the scheduler, by enabling interrupts.
NOTE: Usually called after all regular tasks are added,
to keep the tasks synchronized.
NOTE: ONLY THE SCHEDULER INTERRUPT SHOULD BE ENABLED!!!
-*------------------------------------------------------------------*/
void sch_start(void)
{
	enable_irq();
}

/*------------------------------------------------------------------*-
sch_update()

This is the scheduler ISR. It is called at a rate
determined by the timer settings in sch_init().
-*------------------------------------------------------------------*/
void sch_update(void)
{
	task_id_t tIndex;

	// NOTE: calculations are in *TICKS* (not milliseconds)
	for (tIndex = 0; tIndex < SCH_MAX_TASKS; tIndex ++) {
	
		// Check if there is a task at this location
		if (s_tSCHTasksTable[tIndex].pTask != NULL) {
			if (0 == s_tSCHTasksTable[tIndex].hwDelay) {
			
				// The task is due to run
				if (SCH_CO_OPERATIVE_TASK == s_tSCHTasksTable[tIndex].chTaskType) {
				
					// If it is a co-operative task, increment the RunMe flag
					s_tSCHTasksTable[tIndex].chRunme ++;
				} else {
				
					// If it is a pre-emptive task, run it IMMEDIATELY
					(*s_tSCHTasksTable[tIndex].pTask)();    // Run the task
					s_tSCHTasksTable[tIndex].chRunme --;    // Reset / reduce chRunMe flag
					
					// Periodic tasks will automatically run again
					// - if this is a'one shot'task, remove it from the array
					if (0 == s_tSCHTasksTable[tIndex].hwPeriod) {
						s_tSCHTasksTable[tIndex].pTask = NULL;
					}
				}
				if (s_tSCHTasksTable[tIndex].hwPeriod) {
				
					// Schedule regular tasks to run again
					s_tSCHTasksTable[tIndex].hwDelay = s_tSCHTasksTable[tIndex].hwPeriod;
				} 
			} else {
			
				// Not yet ready to run: just decrement the delay
				s_tSCHTasksTable[tIndex].hwDelay --;
			}
		}
	}
}

/*------------------------------------------------------------------*-
hSCH_Dispatch_Tasks()

This is the 'dispatcher'function. When a task (function)
is due to run, hSCH_Dispatch_Tasks() will run it.
This function must be called (repeatedly) from the main loop.
-*------------------------------------------------------------------*/
void sch_dispatch_tasks(void)
{
	task_id_t tIndex;

	// Dispatches (runs) the next task (if one is ready)
	for (tIndex = 0; tIndex < SCH_MAX_TASKS; tIndex ++) {
	
		// Only dispatching co-operative tasks
		if ((s_tSCHTasksTable[tIndex].chTaskType) && (s_tSCHTasksTable[tIndex].chRunme > 0)) {
			(*s_tSCHTasksTable[tIndex].pTask)(); // Run the task
			s_tSCHTasksTable[tIndex].chRunme --; // Reset / reduce RunMe flag
			
			// - if this is a 'one shot' task, remove it from the array
			if (0 == s_tSCHTasksTable[tIndex].hwPeriod) {
			
				// Faster than call to delete task
				s_tSCHTasksTable[tIndex].pTask = NULL;
			}
		}
	}

	// Report system status
	sch_report_status();
	
	// The scheduler enters idle mode at this point
	sch_go_to_sleep();
}

/*------------------------------------------------------------------*-
sch_add_tasks()

Causes a task (function) to be executed at regular intervals
or after a user-defined delay
pTask - The name of the function which is to be scheduled.
	NOTE: All scheduled functions must be 'void, void' -
	that is, they must take no parameters, and have
	a void return type.
hwDelay - The interval (TICKS) before the task is first executed
hwPeriod - If 'Per' is 0, the function is only called once,
	at the time determined by 'Del'. If Per is non-zero,
	then the function is called repeatedly at an interval
	determined by the value of Per (see below for examples
	that should help clarify this).
chTaskType - Set to 1 if it a co-op task; 0 if pre-emptive
RETN: The position in the task array at which the task has been added.
	If the return value is SCH_MAX_TASKS then the task could not be
	added to the array (there was insufficient space). If the
	return value is < SCH_MAX_TASKS, then the task was added
	successfully.
Note: this return value may be required, if a task is
	to be subsequently deleted - see sch_delete_task().


EXAMPLES:
Task_ID = sch_add_tasks(Do_X,1000,0,0);
Causes the function Do_X() to be executed once after 1000 ticks.
(Pre-emptive task)

Task_ID = sch_add_tasks(Do_X,0,1000,1);
Causes the function Do_X() to be executed regularly, every 1000 ticks.
(co-operative task)

Task_ID = sch_add_tasks(Do_X,300,1000,0);
Causes the function Do_X() to be executed regularly, every 1000 ticks.
Task will be first executed at T = 300 ticks, then 1300, 2300, etc.
(Pre-emptive task)
*------------------------------------------------------------------*/

task_id_t sch_add_tasks(void (* const pTask)(), // Task function pointer
						uint16_t hwDelay,   // Num ticks 'til task first runs
						uint16_t hwPeriod,  // Num ticks between repeat runs
						uint8_t chTaskType  // Co_op / pre_emp
						)
{
	task_id_t tIndex = 0;

	while ((s_tSCHTasksTable[tIndex].pTask != NULL) && tIndex < SCH_MAX_TASKS) {
		tIndex ++;
	}
	
	// Have we reached the end of the list?
	if(SCH_MAX_TASKS == tIndex) {
	
		// Task list is full
		// Set the global error variable
		s_tError.chTaskErrorCode = ERROR_SCH_TOO_MANY_TASKS;
		
		// Also return an error code
		return SCH_MAX_TASKS;
	}

	// If we're here, there is a space in the task array
	s_tSCHTasksTable[tIndex].pTask = pTask;
	s_tSCHTasksTable[tIndex].hwDelay = hwDelay;
	s_tSCHTasksTable[tIndex].hwPeriod = hwPeriod;
	s_tSCHTasksTable[tIndex].chTaskType = chTaskType;
	s_tSCHTasksTable[tIndex].chRunme = 0;
	
	return tIndex;
}

/*------------------------------------------------------------------*-
sch_delete_task()

Removes a task from the scheduler. Note that this does
*not* delete the associated function from memory:
it simply means that it is no longer called by the scheduler.
PARAMS: tTaskIndex - The task index. Provided by sch_delete_task().
RETURNS: false or true
-*------------------------------------------------------------------*/
bool sch_delete_task(task_id_t tTaskIndex)
{
	bool bReturnCode;

	if (NULL == s_tSCHTasksTable[tTaskIndex].pTask) {
	
		// No task at this location...
		// Set the global error variable
		s_tError.chTaskErrorCode = ERROR_SCH_CANNOT_DELETE_TASK;
		
		// ...also return an error code
		bReturnCode = false;
	} else {
		bReturnCode = true;
	}

	s_tSCHTasksTable[tTaskIndex].pTask = NULL;
	s_tSCHTasksTable[tTaskIndex].hwDelay = 0;
	s_tSCHTasksTable[tTaskIndex].hwPeriod = 0;
	s_tSCHTasksTable[tTaskIndex].chRunme = 0;
	
	// return status
	return bReturnCode;
}

/*------------------------------------------------------------------*-
sch_report_status()

Simple function to display error codes.

adapt, if required, to report errors over serial link, etc.

Errors are only displayed for a limited period
(60000 ticks = 1 minute at 1 ms tick interval).
After this the the error code is reset to 0.

This code may be easily adapted to display the last
error 'for ever': this may be appropriate in your
application.
-*------------------------------------------------------------------*/

void sch_report_status(void)
{
	#ifdef SCH_REPORT_ERRORS
	// ONLY APPLIES IF WE ARE REPORTING ERRORS
	// Check for a new error code
	if (s_tError.chTaskErrorCode == s_tError.chLastErrorCode) {
		if (ERROR_SCH_TOO_MANY_TASKS == s_tError.chTaskErrorCode) {
			printf("Too many tasks!\r\n");
		} else if (ERROR_SCH_CANNOT_DELETE_TASK == s_tError.chTaskErrorCode){
			printf("Can't delete task!\r\n");
		}
		if (s_tError.chTaskErrorCode != 0) {
			s_tError.hwErrorTickCount = 60000;
		} else {
			s_tError.hwErrorTickCount = 0;
		}
	} else {
		if (s_tError.hwErrorTickCount != 0) {
			if (-- s_tError.hwErrorTickCount == 0) {
				s_tError.chTaskErrorCode = 0;
			}
		}
	}
	#endif
}

/*------------------------------------------------------------------*-
sch_go_to_sleep()

This scheduler enters 'idle mode' between clock ticks
to save power. The next clock tick will return the processor
to the normal operating state.

Note: a slight performance improvement is possible if this
function is implemented as a macro, or if the code here is simply
pasted into the  'dispatch'  function.

However, by making this a function call, it becomes easier
- during development - to assess the performance of the
scheduler, using the 'performance analyser' in the Keil
hardware simulator. 

*** May wish to disable this if using a watchdog ***
*** ADAPT AS REQUIRED FOR YOUR HARDWARE ***
-*------------------------------------------------------------------*/
void sch_go_to_sleep(void)
{

}


/*-------------------------------END OF FILE-------------------------------*/

