
#include <sys/types.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>

#define BOARD_DIR	"../boards/"	/* Board data dir               */

time_t current_time;

int core_available(void)
{
	FILE *fp;

	if ((fp = fopen("../area/core", "r")) == NULL)
		return 0;
	fclose(fp);
	return 1;
}

int delete_core(void)
{
	remove("../area/core");
}

int postnote(char *note)
{
	FILE *fp;
	char filename[128];
	char *strtime;

	sprintf(filename, "%s%s", BOARD_DIR, "immortal.brd");
	if ((fp = fopen(filename, "a")) == NULL)
		return;

	strtime = ctime(&current_time);
	strtime[strlen(strtime) - 1] = '\0';

	fprintf(fp,
		"Sender  %s~\nDate    %s~\nTo      %s~\nSubject %s~\nVoting %d\nYesvotes %s~\nNovotes %s~\nAbstentions %s~\nText\n%s~\n\n",
		"DEBUGGER", strtime, "Caretakers", "Found something", 0, "", "",
		"", note);

	fclose(fp);
}

int main(int argc, char **argv)
{
	struct timeval now_time;
	FILE *gdb;
	char buf[5000];
	int pos = 0;
	char c;

	if (!core_available()) {
		printf("gdbnote: No core available.\n\r");
		return;
	}

	printf("gdbnote: Analizing core and posting note.\n\r");

	gettimeofday(&now_time, NULL);
	current_time = (time_t) now_time.tv_sec;

	if ((gdb = popen("gdb -batch -x ../src/gdbnote.cfg", "r")) == NULL) {
		return 0;
	}

	sprintf(buf, "Do not remove this note. Coders can use it\n"
		"to see why the mud crashed.\n\n");
	pos = strlen(buf);

	while (!feof(gdb)) {
		buf[pos++] = getc(gdb);
		if (pos > 4999)
			break;
	}

	if (pos > 0) {
		buf[pos - 1] = '\0';
		postnote(buf);
	}

	pclose(gdb);

	delete_core();

	printf("gdbnote: Done.");

	return 0;
}
