#include "feelings.h"

int zt_feeling_of_day(void)
{
	srand(time(NULL));



	return 0;
}

int zt_feelings_check(void)
{
	return 0;
}

int zt_check_for_changes();

int zt_if_join(zt_feelings *zt)
{
	int ret = 0;

	if (!(zt->dayfeel & ZT_HAPPY))
	       ret = 1;

	return ret;
}
int zt_if_query();
int zt_if_part();
int zt_send_msg();
