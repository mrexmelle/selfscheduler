
#include <limits.h>
#include <signal.h>
#include <stdio.h>
#include "selfscheduler.h"

static int gExitLoop=0;

void main_signalhandler(int aSignal)
{
	switch(aSignal)
	{
		case SIGINT:
		{
			gExitLoop=1;
			printf("main_signalhandler - SIGINT raised\n");
			break;
		}
		default:
		{
			break;
		}
	}
}

int main(int argc, char * argv[])
{

printf("main - enter\n");

int status;

signal(SIGINT, main_signalhandler);

gExitLoop=0;

// open selfscheduler
status=selfscheduler_open_server();
printf("main - open status: %d\n", status);
if(status!=0) { return -1; }

// init value
status=selfscheduler_write_segment(0,0);
printf("main - write status: %d\n", status);
if(status!=0) { return -2; }

static unsigned int counter=0;

while(1)
{
	counter++;
	if(counter==10000000) // 10M
	{
		selfscheduler_increase_segment_by_one();
		counter=0;

//		unsigned long long majorSeq, minorSeq;

//		status=selfscheduler_read_segment(&majorSeq, &minorSeq);
//		printf("main - read status: %d\n", status);

//		printf("main - majorSeq: %llu\n", majorSeq);
//		printf("main - minorSeq: %llu\n", minorSeq);
	}

	// read exit status
	if(gExitLoop==1) { break; }
}

// close selfscheduler
status=selfscheduler_close_server();
printf("main - close status: %d\n", status);

printf("main - exit\n");

return 0;

}
