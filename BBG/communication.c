#include "communication.h"


char *get_lux()
{
	memset(lux,' ',sizeof(lux));
	sprintf(lux,"%f",comm_rec.lux);
	return lux;
}

char *get_distance()
{
	memset(distance,' ',sizeof(distance));
	sprintf(distance,"%f",comm_rec.distance);
	return distance;
} 

char *get_waterLevel()
{
	memset(waterLevel,' ', sizeof(waterLevel));
	sprintf(waterLevel,"%f", comm_rec.distance);
	return waterLevel;
}

void *communication_thread_callback()
{

	printf("Inside communication thread\n");

	struct sensor_struct comm;

	struct sensor_struct sensor;

	struct logger_struct logger;

	uart_properties *uart4 = malloc(sizeof(uart_properties));
	uart4->uart_no = 4;
	uart4->baudrate = B115200;

	uint8_t isOpen4 = uart_config(uart4);
	int i = 0;
	printf("Open success? %d\n", isOpen4);

	char obj_detect = '1';
	char valve_close = '2';
	char valve_open = '3';
	char lux_auto = '4';

	char task[15];
	if (isOpen4 == 0) {
		unsigned char receive[30];
		while(1)
		{
			//	printf("Entered while\n");
			char buf[30];
				// sprintf(buf, "foo %d", ++i);

			/*	strcpy(sensor.task_name,"LUX");
				sensor.timeStamp = 100;
				sensor.sensor_data = 9999;

				// Send data to uart1
				if (uart_send(uart1, &sensor, sizeof(sensor_struct)) < 0) {
					printf("Could not send data to uart port");
					return -1;
				}

	*/
	//			usleep(1000);



				// usleep(10000);
			if(uart_receive(uart4,&sensor, sizeof(sensor_struct)) > 0)
			{
				if((strcmp(sensor.task_name,"DIST") == 0) && sensor.sensor_data < 10)
				{
					pthread_mutex_lock(&lock_res);
					uart_send(uart2, &obj_detect, sizeof(char));
					pthread_mutex_unlock(&lock_res);
				}

				if((strcmp(sensor.task_name,"LUX") == 0) && sensor.sensor_data < 10)
				{
					pthread_mutex_lock(&lock_res);
					uart_send(uart2, &lux_auto, sizeof(char));
					pthread_mutex_unlock(&lock_res);
				}

				if((strcmp(sensor.task_name,"WAT") == 0) && sensor.sensor_data < 300)
				{
					pthread_mutex_lock(&lock_res);
					uart_send(uart2, &valve_close, sizeof(char));
					pthread_mutex_unlock(&lock_res);
				}

				if((strcmp(sensor.task_name,"WAT") == 0) && sensor.sensor_data >= 300)
				{
					pthread_mutex_lock(&lock_res);
					uart_send(uart2, &valve_open, sizeof(char));
					pthread_mutex_unlock(&lock_res);
				}
			}

		}
		uart_close(uart4);
	}
	printf("EOF\n");
	// return 0;
}
