#include "feelings.h"

int zt_feelings_randnum(void)
{
	srand(time(NULL));
	return (rand() % 1000) + 1;
}

int zt_feeling_of_day(void)
{
	srand(time(NULL));

	return 0;
}

int zt_change_columns(zt_info* ztinfo, zt_data* data, char* buffer)
{
	const char *columns[] = { ":nick", ":channel", NULL };
	return 0;
	/* todo in future */
}

int zt_feelings_talk_with_me(zt_info* ztinfo, zt_data* data)
{
	char *talk[] = {
		"lets fuckin crazy!", "hey man, how are you doing?", "don't call me that",
		"are you talkin with me?", "hey :D", "i was thinking in you..",
		"wait, im on phone.", "coffee time :)z", "no way!"
	};
	int talk_sz = sizeof(talk) / sizeof(talk[0]);

	int choice = rand() % talk_sz;

	char buf[BUF_MED];

	snprintf(buf, sizeof(buf)-1, "PRIVMSG %s :%s\n", ztinfo->channels[0], talk[choice]);
	write(ztinfo->socket, buf, strlen(buf));

	return 0;
}


int zt_feelings_change(zt_info *ztinfo)
{
	int feelingnow = rand() % FEELINGS_SZ;
	fprintf(stdout, ":. feelings changed from %d:%s, to %d:%s\n",
			ztinfo->feeling, zt_feelings_desc[ztinfo->feeling],
			feelingnow, zt_feelings_desc[feelingnow]);
	ztinfo->feeling = feelingnow;
	return ztinfo->feeling;
}

int zt_feelings_talk(zt_info *ztinfo, zt_data *data)
{
	char *happy_talk[] = {
		"woohoo!! new idea coming..", "i did it! yeah _\\,,/", "playing poker... :D",
		"getting email", "are you sure?", "please, do that.",
		"lets fuckin crazy!", "i am stoned.. .^.", "wtf", "rtfm", "lol",
		"i just finished!", "so eazy look for things on internet nowdays..", "booh!"
	};
	int happy_talk_sz = sizeof(happy_talk) / sizeof(happy_talk[0]);

	int choice = rand() % happy_talk_sz;

	char buf[BUF_MED];

	fprintf(stdout, ":. i wanna talk! -> '%s'\n", happy_talk[choice]);
	snprintf(buf, sizeof(buf)-1, "PRIVMSG %s :%s\n", ztinfo->channels[0], happy_talk[choice]);
	write(ztinfo->socket, buf, strlen(buf));

	return 0;
}

int zt_feelings_wannatalk(int probability)
{
	int randnum = zt_feelings_randnum();

	if (randnum > probability)
		return 0;

	return 1;
}

int zt_feelings_check(zt_info *ztinfo, int probability)
{
	int res = 0;
	int randnum = zt_feelings_randnum();

	if (randnum > probability) {
		res = zt_feelings_change(ztinfo);
	}

	return res;
}

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

int zt_feelings_event(zt_info *ztinfo, zt_data *data)
{
	if (zt_feelings_check(ztinfo, 995))
		zt_feelings_change(ztinfo);
	else {

		// LAST THING BEFORE JOIN, PART, TALK WITH HIM
		if (!zt_feelings_wannatalk(985))
			zt_feelings_talk(ztinfo, data);
		else if (strcasestr(data->message, ztinfo->nick) && !strncmp(data->irccmd, "PRIVMSG", 7))
			zt_feelings_talk_with_me(ztinfo, data);

	}
	return 0;
}
