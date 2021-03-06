#include <iostream>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

using namespace std;
#define MAX 64

int main(int argc, char *argv[]){
        int period;
        if(argc != 2){
           printf("ERROR: Wrong number of Arguments\n");
           exit(1);
        }
        period = atoi(argv[1]);
        int GPIOPin = 60;
	cout << "GPIO LED Flash Pin:" << GPIOPin << " start" << endl;

	FILE *LEDHandle = NULL;
	char setValue[4], GPIOString[4],GPIOValue[MAX], GPIODirection[MAX];
	sprintf(GPIOString, "%d", GPIOPin);
	sprintf(GPIOValue, "/sys/class/gpio/gpio%d/value", GPIOPin);
	sprintf(GPIODirection, "/sys/class/gpio/gpio%d/direction", GPIOPin);

        //export the pin
	if((LEDHandle = fopen("/sys/class/gpio/export", "ab")) == NULL){
		printf("Cannot export the GPIO pin.\n");
		return 1;
	}
	strcpy(setValue, GPIOString);
	fwrite(&setValue, sizeof(char), 2, LEDHandle);
	fclose(LEDHandle);

	//set the direction of the pin
	if((LEDHandle = fopen(GPIODirection, "rb+")) == NULL){
		printf("Cannot open direction handle.\n");
		return 1;
	}
	strcpy(setValue, "out");
	fwrite(&setValue, sizeof(char), 3, LEDHandle);
	fclose(LEDHandle);

	//flash the LED
        double divisor = ((double)period/1000000.0);
        int times =(int)(30.0/divisor); 
	for(int i=0; i<(times*2); i++){
		if((LEDHandle = fopen(GPIOValue, "rb+")) == NULL){
			printf("Cannot open value handle.\n");
			return 1;
		}
		if((i%2)==1){
			strcpy(setValue,"0");
		}
		else{
			strcpy(setValue,"1");
		}
		fwrite(&setValue, sizeof(char), 1, LEDHandle);
		fclose(LEDHandle);
	//	sleep(10);
		usleep(period/2);
	}
	
	//unexport the pin
	if((LEDHandle = fopen("/sys/class/gpio/unexport", "ab")) == NULL){
		printf("Cannot unexport the GPIO pin.\n");
		return 1;
	}
	strcpy(setValue, GPIOString);
	fwrite(&setValue, sizeof(char), 2, LEDHandle);
	fclose(LEDHandle);

	cout << "GPIO LED Flash Pin:" << GPIOPin << " end" << endl;

	return 0;
}
