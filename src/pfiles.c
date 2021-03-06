/****************************************************************************
 *                   ^     +----- |  / ^     ^ |     | +-\                  *
 *                  / \    |      | /  |\   /| |     | |  \                 *
 *                 /   \   +---   |<   | \ / | |     | |  |                 *
 *                /-----\  |      | \  |  v  | |     | |  /                 *
 *               /       \ |      |  \ |     | +-----+ +-/                  *
 ****************************************************************************
 * AFKMud (c)1997-2002 Alsherok. Contributors: Samson, Dwip, Whir,          *
 * Cyberfox, Karangi, Rathian, Cam, Raine, and Tarl.                        *
 *                                                                          *
 * Original SMAUG 1.4a written by Thoric (Derek Snider) with Altrag,        *
 * Blodkai, Haus, Narn, Scryn, Swordbearer, Tricops, Gorog, Rennard,        *
 * Grishnakh, Fireblade, and Nivek.                                         *
 *                                                                          *
 * Original MERC 2.1 code by Hatchet, Furey, and Kahn.                      *
 *                                                                          *
 * Original DikuMUD code by: Hans Staerfeldt, Katja Nyboe, Tom Madsen,      *
 * Michael Seifert, and Sebastian Hammer.                                   *
 ****************************************************************************
 *                          Pfile Pruning Module                            *
 ****************************************************************************/

#include <sys/types.h>
#include <ctype.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#include "mud.h"

/* Globals */
time_t pfile_time;
HOUR_MIN_SEC set_pfile_time_struct;
HOUR_MIN_SEC *set_pfile_time;
struct tm *new_pfile_time;
struct tm new_pfile_struct;
time_t new_pfile_time_t;
sh_int num_pfiles;		/* Count up number of pfiles */

void save_timedata(void)
{
	FILE *fp;
	char filename[MIL];

	sprintf(filename, "%stime.dat", SYSTEM_DIR);

	fclose(fpReserve);
	if ((fp = fopen(filename, "w")) == NULL) {
		bug("save_timedata: fopen");
		perror(filename);
	} else {
		fprintf(fp, "#TIME\n");
		fprintf(fp, "Purgetime %ld\n", new_pfile_time_t);
		fprintf(fp, "End\n\n");
		fprintf(fp, "#END\n");
	}
	fclose(fp);
	fpReserve = fopen(NULL_FILE, "r");
	return;
}

#ifdef KEY
#undef KEY
#endif
#define KEY( literal, field, value )                                    \
                                if ( !str_cmp( word, literal ) )        \
                                {                                       \
                                    field  = value;                     \
                                    fMatch = TRUE;                      \
                                    break;                              \
                                }

/* Reads the actual time file from disk - Samson 1-21-99 */
void fread_timedata(FILE * fp)
{
	char *word = NULL;
	bool fMatch = FALSE;

	for (;;) {
		word = feof(fp) ? "End" : fread_word(fp);
		fMatch = FALSE;

		switch (UPPER(word[0])) {
		case '*':
			fMatch = TRUE;
			fread_to_eol(fp);
			break;

		case 'E':
			if (!str_cmp(word, "End"))
				return;
			break;

		case 'P':
			KEY("Purgetime", new_pfile_time_t, fread_number(fp));
			break;
		}

		if (!fMatch) {
			bug("Fread_timedata: no match: %s", word);
			fread_to_eol(fp);
		}
	}
}

bool load_timedata(void)
{
	char filename[MIL];
	FILE *fp;
	bool found;

	found = FALSE;
	sprintf(filename, "%stime.dat", SYSTEM_DIR);

	if ((fp = fopen(filename, "r")) != NULL) {

		found = TRUE;
		for (;;) {
			char letter = '\0';
			char *word = NULL;

			letter = fread_letter(fp);
			if (letter == '*') {
				fread_to_eol(fp);
				continue;
			}

			if (letter != '#') {
				bug("Load_timedata: # not found.");
				break;
			}

			word = fread_word(fp);
			if (!str_cmp(word, "TIME")) {
				fread_timedata(fp);
				break;
			} else if (!str_cmp(word, "END"))
				break;
			else {
				bug("Load_timedata: bad section - %s.", word);
				break;
			}
		}
		FCLOSE(fp);
	}

	return found;
}

void init_pfile_scan_time(void)
{
	/*
	 * Init pfile scan time.
	 */
	set_pfile_time = &set_pfile_time_struct;

	new_pfile_time = update_time(localtime(&current_time));
	/* Copies *new_pfile_time to new_pfile_struct, and then points
	   new_pfile_time to new_pfile_struct again. -- Alty */
	new_pfile_struct = *new_pfile_time;
	new_pfile_time = &new_pfile_struct;
	new_pfile_time->tm_mday += 1;
	if (new_pfile_time->tm_hour > 12)
		new_pfile_time->tm_mday += 1;
	new_pfile_time->tm_sec = 0;
	new_pfile_time->tm_min = 0;
	new_pfile_time->tm_hour = 3;

	/* Update new_pfile_time (due to day increment) */
	new_pfile_time = update_time(new_pfile_time);
	new_pfile_struct = *new_pfile_time;
	new_pfile_time = &new_pfile_struct;
	/* Bug fix submitted by Gabe Yoder */
	new_pfile_time_t = mktime(new_pfile_time);
	/* check_pfiles(mktime(new_pfile_time)); */

	if (!load_timedata()) {
		strcpy(log_buf,
		       "Pfile scan time reset to default time of 3am.");
		log_string(log_buf);
	}
	return;
}

time_t now_time;
sh_int deleted = 0;
sh_int days = 0;

#if defined(KEY)
#undef KEY
#endif

#define KEY( literal, field, value )					\
				if ( !strcmp( word, literal ) )	\
				{					\
				    field  = value;			\
				    fMatch = TRUE;			\
				    break;				\
				}

void fread_pfile(FILE * fp, time_t tdiff, char *fname, bool count)
{
	CLAN_DATA *clan_data;
	char *word;
	char *name = NULL;
	char *clan = NULL;
	char *deity = NULL;
	sh_int toplevel = 0;
	sh_int file_ver = 0;

//   int pact;
	bool fMatch;

//  ROOM_INDEX_DATA *room;  /* Needed for losing home */

	for (;;) {
		word = feof(fp) ? "End" : fread_word(fp);
		fMatch = FALSE;

		switch (UPPER(word[0])) {
		case '*':
			fMatch = TRUE;
			fread_to_eol(fp);
			break;

		case 'C':
			KEY("Clan", clan, fread_string(fp));
			break;

		case 'D':
			KEY("Deity", deity, fread_string(fp));
			break;

		case 'E':
			if (!strcmp(word, "End"))
				goto timecheck;
			break;

		case 'N':
			KEY("Name", name, fread_string(fp));
			break;
			/*
			   case 'P':
			   Plr home check - Gatz
			   if ( !str_cmp( word, "PlrHome" ) )
			   {
			   name->plr_home = get_room_index( fread_number( fp ) );
			   if ( !name->plr_home )
			   name->plr_home = NULL;
			   break;
			   }

			   break;
			 */
		case 'T':
			KEY("Toplevel", toplevel, fread_number(fp));
			break;

		case 'V':
			KEY("Version", file_ver, fread_number(fp));
			break;
		}

		if (!fMatch)
			fread_to_eol(fp);
	}

      timecheck:

	if (toplevel < 5 && tdiff > sysdata.newbie_purge) {
		if (unlink(fname) == -1)
			perror("Unlink");
		else {
			days = sysdata.newbie_purge;
			sprintf(log_buf,
				"Player %s was deleted. Exceeded time limit of %d days.",
				name, days);
			log_string(log_buf);
#ifdef AUTO_AUTH
			remove_from_auth(name);
#endif
			deleted++;
			return;
		}
	}

	if (toplevel < LEVEL_IMMORTAL && tdiff > sysdata.regular_purge) {
		if (toplevel < LEVEL_IMMORTAL) {
			if (unlink(fname) == -1)
				perror("Unlink");
			else {
				// Players lose homes - Gatz
				/*
				   if (name->plr_home != NULL)
				   {
				   room = name->plr_home;

				   STRFREE( room->name );
				   room->name = STRALLOC( "An Empty Apartment" );

				   REMOVE_BIT( room->room_flags , ROOM_PLR_HOME );
				   SET_BIT( room->room_flags , ROOM_EMPTY_HOME );

				   fold_area( room->area, room->area->filename, FALSE );

				   }
				 */
				days = sysdata.regular_purge;
				sprintf(log_buf,
					"Player %s was deleted. Exceeded time limit of %d days.",
					name, days);
				log_string(log_buf);
#ifdef AUTO_AUTH
				remove_from_auth(name);
#endif
				deleted++;
				return;
			}
		}
	}
}

void read_pfile(char *dirname, char *filename, bool count)
{
	FILE *fp;
	char fname[MSL];
	struct stat fst;
	time_t tdiff;

	now_time = time(0);

	sprintf(fname, "%s/%s", dirname, filename);

	if (!str_cmp(filename, ".F") || !str_cmp(filename, ".home"))
		return;

	if (stat(fname, &fst) != -1) {
		tdiff = (now_time - fst.st_mtime) / 86400;

		if ((fp = fopen(fname, "r")) != NULL) {
			for (;;) {
				char letter;
				char *word;

				letter = fread_letter(fp);

				if ((letter != '#') && (!feof(fp)))
					continue;

				word = feof(fp) ? "End" : fread_word(fp);

				if (!str_cmp(word, "End"))
					break;

				if (!str_cmp(word, "PLAYER")) {
					fread_pfile(fp, tdiff, fname, count);
				} else if (!str_cmp(word, "END"))	/* Done         */
					break;
			}
			FCLOSE(fp);
		}
	}
	return;
}

void pfile_scan(bool count)
{
	DIR *dp;
	struct dirent *dentry;
	int len;
	char directory_name[100];
	char buf[MAX_STRING_LENGTH];

	sh_int alpha_loop;
	sh_int cou = 0;

	deleted = 0;

	now_time = time(0);
	nice(20);

	for (alpha_loop = 0; alpha_loop <= 25; alpha_loop++) {
		sprintf(directory_name, "%s%c", PLAYER_DIR, 'a' + alpha_loop);
		/* log_string( directory_name ); */
		dp = opendir(directory_name);
		dentry = readdir(dp);
		while (dentry) {
			len = strlen(dentry->d_name);
			if (dentry->d_name[0] != '.'
			    && str_cmp(dentry->d_name, ".clone")
			    && str_cmp(dentry->d_name, ".home")) {
				read_pfile(directory_name, dentry->d_name,
					   count);
				cou++;
			}
			dentry = readdir(dp);
		}
		closedir(dp);
	}

	if (!count)
		log_string("Pfile cleanup completed.");
	else
		log_string("Pfile count completed.");

	sprintf(log_buf, "Total pfiles scanned: %d", cou);
	log_string(log_buf);

	if (!count) {
		sprintf(log_buf, "Total pfiles deleted: %d", deleted);
		log_string(log_buf);

		sprintf(log_buf, "Total pfiles remaining: %d", cou - deleted);
		num_pfiles = cou - deleted;
		log_string(log_buf);
	} else
		num_pfiles = cou;

	return;
}

void do_pfiles(CHAR_DATA * ch, char *argument)
{
	char buf[MSL];

	if (IS_NPC(ch)) {
		send_to_char("Mobs cannot use this command!\n\r", ch);
		return;
	}

	if (argument[0] == '\0' || !argument) {
		/* Makes a backup copy of existing pfiles just in case - Samson */
		sprintf(buf, "tar -cf %spfiles.tar %s*", PLAYER_DIR,
			PLAYER_DIR);

		/* GAH, the shell pipe won't process the command that gets pieced
		   together in the preceeding lines! God only knows why. - Samson */
		system(buf);

		sprintf(log_buf, "Manual pfile cleanup started by %s.",
			ch->name);

		log_string(log_buf);
		pfile_scan(FALSE);
#ifdef SAMSONRENT
		rent_update();
#endif
		return;
	}

	if (!str_cmp(argument, "settime")) {
		new_pfile_time_t = current_time + 86400;
		save_timedata();
		send_to_char("New cleanup time set for 24 hrs from now.\n\r",
			     ch);
		return;
	}

	if (!str_cmp(argument, "count")) {
		sprintf(log_buf, "Pfile count started by %s.", ch->name);
		log_string(log_buf);
		pfile_scan(TRUE);
		return;
	}

	send_to_char("Invalid argument.\n\r", ch);
	return;
}

void check_pfiles(time_t reset)
{
	/* This only counts them up on reboot if the cleanup isn't needed - Samson 1-2-00 */
	if (reset == 255 && new_pfile_time_t > current_time) {
		reset = 0;	/* Call me paranoid, but it might be meaningful later on */
		log_string("Counting pfiles.....");
		pfile_scan(TRUE);
		return;
	}

	if (new_pfile_time_t <= current_time) {
		if (sysdata.CLEANPFILES == TRUE) {

			char buf[MSL];

			/* Makes a backup copy of existing pfiles just in case - Samson */
			sprintf(buf, "tar -cf %spfiles.tar %s*", PLAYER_DIR,
				PLAYER_DIR);

			/* Would use the shell pipe for this, but alas, it requires a ch in order
			   to work, this also gets called during boot_db before the rare item
			   checks for the rent code - Samson */
			system(buf);

			new_pfile_time_t = current_time + 86400;
			save_timedata();
			log_string("Automated pfile cleanup beginning....");
			pfile_scan(FALSE);
#ifdef SAMSONRENT
			if (reset == 0)
				rent_update();
#endif
		} else {
			new_pfile_time_t = current_time + 86400;
			save_timedata();
			log_string("Counting pfiles.....");
			pfile_scan(TRUE);
#ifdef SAMSONRENT
			if (reset == 0)
				rent_update();
#endif
		}
	}
	return;
}

void do_exempt(CHAR_DATA * ch, char *argument)
{
	CHAR_DATA *victim;

	if (argument[0] == '\0') {
		send_to_char("Syntax: exempt <char>\n\r", ch);
		return;
	}

	if ((victim = get_char_world(ch, argument)) == NULL) {
		send_to_char("They must be online to exempt them.\n\r", ch);
		return;
	}

	if (IS_NPC(victim)) {
		send_to_char("You can only exempt mobs.\n\r", ch);
		return;
	}

	SET_BIT(victim->pcdata->act2, ACT_EXEMPT);

	send_to_char("You have been exempt from deletion.\n\r", victim);
	send_to_char("They have been exempt from deletion.\n\r", ch);

	return;
}

/*
void add_employee(char *name, char *shortname)
{
  char buf[MAX_STRING_LENGTH];
  char fbuf[MAX_STRING_LENGTH];

 if(name[0] == '\0' || !name)
 {
   bug("add_employee: No name!\n\r");
   return;
 }

 if(shortname[0] == '\0' || !shortname)
 {
   bug("add_employee: No shortname!\n\r");
   return;
 }
 
 sprintf(fbuf, "%s%s.list", BUSI_DIR, shortname);
 sprintf(buf, "%s~", name);
 append_to_file(fbuf, buf);

}

void remove_employee(char *name, char *shortname)
{
 FILE *fpList;
 FILE *fpNew;
 char *buf;
 char list[MAX_STRING_LENGTH];
 char temp[MAX_STRING_LENGTH];

 if(name[0] == '\0')
 {
   bug("remove_employee: No name!\n\r");
   return;
 }

 if(shortname[0] == '\0' || !shortname)
 {
   bug("remove_employee: No shortname!\n\r");
   return;
 }
 
 sprintf(list, "%s%s.list", BUSI_DIR, shortname);
 sprintf(temp,"%s.temp", list);

 if ( ( fpList = fopen(list, "r" ) ) == NULL )
 {
    bug("Unable to open employee list");
    return;
 }

 if ( ( fpNew = fopen(temp, "w" ) ) == NULL)
 {
    bug("remove_employee: Unable to write temp list");
    return;
 }
 
 for ( ; ; )
 {
     if(feof(fpList))
	break;
     buf = feof( fpList ) ? "End" : fread_string( fpList );
     if(!str_cmp(buf, "End") || buf[0] == '\0')
	break;
     if(str_cmp(name, buf) && strlen(buf) > 2)
	fprintf(fpNew, "%s~\n", buf);
 }
 fclose( fpNew );
 fclose( fpList );
 rename(temp, list);
}

*/
