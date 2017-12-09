
#ifndef __SELFSCHEDULER_H__
#define __SELFSCHEDULER_H__

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

extern int selfscheduler_open_server();
extern int selfscheduler_open_client();
extern int selfscheduler_close_server();
extern int selfscheduler_close_client();
extern int selfscheduler_write_segment(const unsigned long long aMajorSeq, const unsigned long long aMinorSeq);
extern int selfscheduler_increase_segment_by_one();
extern int selfscheduler_read_segment(unsigned long long * aMajorSeq, unsigned long long * aMinorSeq);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __SELFSCHEDULER_H__

