#include "pico/stdlib.h"
#include "mbtimer.h"
#include "common.h"

eMBRcvState eRcvState;
repeating_timer_t g_mb_timer;

volatile uint8_t mb_state_rtu_finish;
volatile uint16_t mb_timeout;
volatile uint16_t mb_downcounter;

void vMBPortTimersCallback(struct repeating_timer *t)
{
  if (t == &g_mb_timer)
    xMBRTUTimerT35Expired();
}
 
void xMBPortTimersInit( uint32_t usTim1Timerout50us)
{
	mb_timeout = usTim1Timerout50us * 50;
}

void vMBPortTimersEnable( void )
{
	cancel_repeating_timer(&g_mb_timer);
	add_repeating_timer_us(mb_timeout, vMBPortTimersCallback, NULL, &g_mb_timer);
}


void vMBPortTimersDisable( void )
{
	cancel_repeating_timer(&g_mb_timer);
}

void xMBRTUTimerT35Expired( void )
{
	switch ( eRcvState ) {
		/* Timer t35 expired. Startup phase is finished. */
		case STATE_RX_INIT:
			break;

		/* A frame was received and t35 expired. Notify the listener that
		* a new frame was received. */
		case STATE_RX_RCV:
			mb_state_rtu_finish = TRUE;
			break;
		
		/* An error occured while receiving the frame. */
		case STATE_RX_ERROR:
			break;

		/* Function called in an illegal state. */
		default:
			break;
	}
	vMBPortTimersDisable(  );
	eRcvState = STATE_RX_IDLE;

	//printf("tim3\r\n");
}
