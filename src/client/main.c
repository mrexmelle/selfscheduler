
#include <stdio.h>
#include "selfscheduler.h"

int main(int argc, char * argv[])
{

printf("main - enter\n");

int status;

// open selfscheduler
status=selfscheduler_open_client();
printf("main - open status: %d\n", status);
if(status!=0) { return -1; }

unsigned long long majorSeq, minorSeq;

status=selfscheduler_read_segment(&majorSeq, &minorSeq);
printf("main - read status: %d\n", status);

printf("main - majorSeq: %llu\n", majorSeq);
printf("main - minorSeq: %llu\n", minorSeq);

// close selfscheduler
status=selfscheduler_close_client();
printf("main - close status: %d\n", status);

printf("main - exit\n");

return 0;

}
