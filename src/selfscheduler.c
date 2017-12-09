
#include <limits.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <sys/shm.h>

#include "selfscheduler.h"

// vars for selfscheduler
static int gShmId;
static const key_t KSelfSchedulerShmKey= 2404;
static const size_t KSelfSchedulerShmMajorSeqSize=8;
static const size_t KSelfSchedulerShmMinorSeqSize=8;
static void * gShmBuffer=NULL;
static pthread_mutex_t gSelfSchedulerMutex=PTHREAD_MUTEX_INITIALIZER;

int selfscheduler_open_server()
{
	// if key is already used
	int testShmId=shmget(KSelfSchedulerShmKey, KSelfSchedulerShmMajorSeqSize+KSelfSchedulerShmMinorSeqSize,
		IPC_EXCL);
	if(testShmId>=0)
	{
		return -3;
	}

	gShmId=shmget(KSelfSchedulerShmKey, KSelfSchedulerShmMajorSeqSize+KSelfSchedulerShmMinorSeqSize,
		IPC_CREAT|0666);
	if(gShmId<0) { perror("shmget"); return -1; } // shmget error

	gShmBuffer=shmat(gShmId, NULL, 0);
	if(gShmBuffer==(void*)(-1)) { perror("shmat"); return -2; } // shmat error

	return 0;
}

int selfscheduler_open_client()
{
	gShmId=shmget(KSelfSchedulerShmKey, KSelfSchedulerShmMajorSeqSize+KSelfSchedulerShmMinorSeqSize,
		IPC_EXCL);
	if(gShmId<0) { perror("shmget"); return -1; } // shmget error

	gShmBuffer=shmat(gShmId, NULL, 0);
	if(gShmBuffer==(void*)(-1)) { perror("shmat"); return -2; } // shmat error

	return 0;
}

int selfscheduler_close_server()
{
	int status;

	status=shmdt(gShmBuffer);
	if(status!=0) { perror("shmdt"); return -1; } // shmdt error

	gShmBuffer=NULL;

	status=shmctl(gShmId, IPC_RMID, NULL);
	if(status<0) { perror("shmctl"); return -2; } // to release shm after shmdt

	return 0;
}

int selfscheduler_close_client()
{
	int status;

	status=shmdt(gShmBuffer);
	if(status!=0) { perror("shmdt"); return -1; } // shmdt error

	gShmBuffer=NULL;

	return 0;
}

int selfscheduler_write_segment(const unsigned long long aMajorSeq, const unsigned long long aMinorSeq)
{
	if(gShmBuffer==NULL) { return -1; } // shm not initialised

	char * shmMajorBuffer=(char*)(gShmBuffer);
	char * shmMinorBuffer=shmMajorBuffer+KSelfSchedulerShmMajorSeqSize;

	// copy value from param
	pthread_mutex_lock(&gSelfSchedulerMutex);
	memcpy(shmMajorBuffer, &aMajorSeq, KSelfSchedulerShmMajorSeqSize);
	memcpy(shmMinorBuffer, &aMinorSeq, KSelfSchedulerShmMinorSeqSize);
	pthread_mutex_unlock(&gSelfSchedulerMutex);

	return 0;
}

int selfscheduler_increase_segment_by_one()
{
	if(gShmBuffer==NULL) { return -1; } // shm not initialised

	char * shmMajorBuffer=(char*)(gShmBuffer);
	char * shmMinorBuffer=shmMajorBuffer+KSelfSchedulerShmMajorSeqSize;

	// increase value
	pthread_mutex_lock(&gSelfSchedulerMutex);
	if(*(unsigned long long*)(shmMinorBuffer)<ULONG_MAX)
	{
		(*(unsigned long long*)(shmMinorBuffer))++;
	}
	else
	{
		(*(unsigned long long*)(shmMajorBuffer))++;
		*(unsigned long long*)(shmMinorBuffer)=0;
	}
	pthread_mutex_unlock(&gSelfSchedulerMutex);

	return 0;
}

int selfscheduler_read_segment(unsigned long long * aMajorSeq, unsigned long long * aMinorSeq)
{
	if(gShmBuffer==NULL) { return -1; } // shm not initialised

	char * shmMajorBuffer=(char*)(gShmBuffer);
	char * shmMinorBuffer=shmMajorBuffer+KSelfSchedulerShmMajorSeqSize;

	// copy value to param
	if(aMajorSeq) { memcpy(aMajorSeq, (void*)(shmMajorBuffer), KSelfSchedulerShmMajorSeqSize); }
	if(aMinorSeq) { memcpy(aMinorSeq, (void*)(shmMinorBuffer), KSelfSchedulerShmMinorSeqSize); }

	return 0;
}

