#include "main.h"

#ifndef _ZTFEELINGS_H_
#define _ZTFEELINGS_H_

typedef enum {
	ZT_NORMAL     = 1, 
	ZT_HAPPY      = 2,
	ZT_AGGRESSIVE = 4,
	ZT_SAD        = 8,
	ZT_FRIENDLY   = 16
} zt_feelings_type;

static char *zt_feelings_desc[] = { 
	"normal", "happy", "aggressive", "sad", "friendly"
};

typedef struct
{
	zt_feelings_type dayfeel;

	zt_feelings_type feeling_now;
	
	int wanna_talk;

	int wanna_listen_music;
} zt_feelings;


int zt_feeling_of_day(void);
int zt_feelings_check(void);
int zt_check_for_changes(void);
int zt_if_join(zt_feelings *);
int zt_if_query(void);
int zt_if_part(void);
int zt_send_msg(void);

#endif
