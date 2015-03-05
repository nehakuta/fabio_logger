/** 
 * \defgroup MY_TIME Timers and time functions
 *
 * \{
 */

#ifndef TIME_H_INCLUDED
#define TIME_H_INCLUDED

#include <stdint.h>

struct tm {
	int tm_sec;
	int tm_min;
	int tm_hour;
	int tm_mday;
	int tm_mon;
	int tm_year;
	int tm_wday;
	int tm_yday;
	int tm_isdst;
};

struct timezone {
    int tz_minuteswest;     /**< Minutes west of Greenwich */
    int tz_dsttime;         /**< Type of DST correction */
};

struct timeval {
	int tv_usec;    /**< Microseconds */
};

#ifndef timercmp
	#define timercmp(a, b, CMP)			\
		((a)->tv_usec CMP (b)->tv_usec)
#endif

static inline void timeradd(struct timeval *a, struct timeval *b, struct timeval *result)
{	
	result->tv_usec = a->tv_usec + b->tv_usec;
}

static inline void timersub(struct timeval *t1, struct timeval *t2, struct timeval *t_diff)
{	
	t_diff->tv_usec = t1->tv_usec - t2->tv_usec;
}

static inline int timer_is_set(struct timeval *timer)
{	
	if (timer->tv_usec != 0) {
		return 1;
	}
	
	return 0;
}

static inline void gettimeofday(struct timeval *tv, struct timezone *tz)
{	
	// TODO: 
	tv->tv_usec = 0;
}

#define TIMEVAL_ZERO	{0}

static inline void delay_us(unsigned us)
{
	(void)us;
	// TODO
}

static inline void timer_set_timeout_us(struct timeval *timer, unsigned us)
{
	(void)timer;
	(void)us;	
	// TODO
}

static inline int periodic_timer_expired(struct timeval *t_timeout, unsigned us)
{
	return 0; // TODO
}

static inline int timer_expired(struct timeval *t_timeout)
{
	return periodic_timer_expired(t_timeout, 0);
}

#endif
/** \} */
