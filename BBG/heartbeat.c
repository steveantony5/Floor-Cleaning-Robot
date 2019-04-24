#include "heartbeat.h"

void *heartbeat_thread_callback()
{
	printf("Inside heartbeat thread\n");
	uart2  = malloc(sizeof(uart_properties));
	uart2->uart_no = 2;
	uart2->baudrate = B115200;

	uint8_t isOpen2 = uart_config(uart2);
	int8_t success;

	printf("IS OPEN 2: %d\n",isOpen2);
	char hb ='h';

	char hb1[2]; 
	while(1)
	{
		usleep(1000000);

//		pthread_mutex_lock(&lock_res);
		success = uart_send(uart2,&hb,sizeof(char));
//		pthread_mutex_unlock(&lock_res);

		if(success > 0)
		{
			printf("HEARTBEAT SENT\n");
		}

	//	usleep(1000000);

	//	uart_receive_task(uart2, hb1, sizeof(hb1));
	}
	uart_close(uart2);
		


}