/*
 * IMC2 version 0.10 - an inter-mud communications protocol
 * Copyright (C) 1996 & 1997 Oliver Jowett <oliver@randomly.org>
 *
 * IMC2 Gold versions 1.00 though 2.00 are developed by MudWorld.
 * Copyright (C) 1999 - 2002 Haslage Net Electronics (Anthony R. Haslage)
 *
 * IMC2 MUD-Net version 3.10 is developed by Alsherok and Crimson Oracles
 * Copyright (C) 2002 Roger Libiez ( Samson )
 * Additional code Copyright (C) 2002 Orion Elder
 * Registered with the United States Copyright Office
 * TX 5-555-584
 *
 * IMC2 Continuum version 4.00 is developed by Rogel
 * Copyright (C) 2003 by Rogel, WhoStyles Copyright (C) 2003 by Kris Craig
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program (see the file COPYING); if not, write to the
 * Free Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 */

#ifdef IMCSMAUG
   #define SOCIALSTRUCT
   #define SOCIAL_DATA SOCIALTYPE
   #define IMCMAX_LEVEL MAX_LEVEL
   #define IMCSTRALLOC STRALLOC
   #define IMCSTRFREE STRFREE
   #define IMCMAXPLAYERS sysdata.maxplayers
   #define CH_IMCDATA(ch)		((ch)->pcdata->imcchardata)
   #define CH_IMCLEVEL(ch)          ((ch)->level)
   #define CH_IMCNAME(ch)           ((ch)->name)
   #define CH_IMCTITLE(ch)          ((ch)->pcdata->title)
   #define CH_IMCRANK(ch)           ((ch)->pcdata->rank)
   #define CH_IMCSEX(ch)            ((ch)->sex)
   #define FIRST_IMCBLACKLIST(ch) 	((ch)->pcdata->imcchardata->imcfirst_blacklist)
   #define LAST_IMCBLACKLIST(ch)		((ch)->pcdata->imcchardata->imclast_blacklist)
   #define IMC_RREPLY(ch) 		((ch)->pcdata->imcchardata->rreply)
   #define IMC_CSUBSCRIBED(ch) 		((ch)->pcdata->imcchardata->imc_csubscribed)
   #define IMC_PFLAGS(ch)      ((ch)->pcdata->imcchardata->imc_pflags)
   #define IS_IMCVISIBLE(ch)		( !IS_NPC(ch) && !IS_SET( IMC_PFLAGS(ch), PSET_IMCPVISIBLE) )
#endif

#if defined(IMCMERC) || defined(IMCROM)
   #define first_descriptor descriptor_list
   #define IMCMAX_LEVEL MAX_LEVEL
   #define IMCSTRALLOC str_dup
   #define IMCSTRFREE free_string
   #define IMCMAXPLAYERS 0
   #define CH_IMCDATA(ch)		((ch)->pcdata->imcchardata)
   #define CH_IMCLEVEL(ch)          ((ch)->level)
   #define CH_IMCNAME(ch)           ((ch)->name)
   #define CH_IMCTITLE(ch)          ((ch)->pcdata->title)
   #define CH_IMCRANK(ch)           (title_table[(ch)->class][(ch)->level][(ch)->sex == SEX_FEMALE ? 1 : 0])
   #define CH_IMCSEX(ch)            ((ch)->sex)
   #define FIRST_IMCBLACKLIST(ch) 	((ch)->pcdata->imcchardata->imcfirst_blacklist)
   #define LAST_IMCBLACKLIST(ch)		((ch)->pcdata->imcchardata->imclast_blacklist)
   #define IMC_RREPLY(ch) 		((ch)->pcdata->imcchardata->rreply)
   #define IMC_CSUBSCRIBED(ch) 		((ch)->pcdata->imcchardata->imc_csubscribed)
   #define IMC_PFLAGS(ch)      ((ch)->pcdata->imcchardata->imc_pflags)
   #define IS_IMCVISIBLE(ch)		( !IS_NPC(ch) && !IS_SET( IMC_PFLAGS(ch), PSET_IMCPVISIBLE) )
#endif

#ifdef IMCACK
   extern int max_players;
   #define first_descriptor first_desc
   #define IMCMAX_LEVEL MAX_LEVEL
   #define IMCMAXPLAYERS max_players
   #define IMCSTRALLOC str_dup
   #define IMCSTRFREE free_string
   #define CH_IMCDATA(ch)		((ch)->pcdata->imcchardata)
   #define CH_IMCLEVEL(ch)		((ch)->level)
   #define CH_IMCNAME(ch)           ((ch)->name)
   #define CH_IMCTITLE(ch)          ((ch)->pcdata->title)
   #define CH_IMCRANK(ch)           (class_table[(ch)->class].who_name)
   #define CH_IMCSEX(ch)            ((ch)->sex)
   #define FIRST_IMCBLACKLIST(ch) 	((ch)->pcdata->imcchardata->imcfirst_blacklist)
   #define LAST_IMCBLACKLIST(ch)		((ch)->pcdata->imcchardata->imclast_blacklist)
   #define IMC_RREPLY(ch) 		((ch)->pcdata->imcchardata->rreply)
   #define IMC_CSUBSCRIBED(ch) 		((ch)->pcdata->imcchardata->imc_csubscribed)
   #define IMC_PFLAGS(ch)      ((ch)->pcdata->imcchardata->imc_pflags)
   #define IS_IMCVISIBLE(ch)		( !IS_NPC(ch) && !IS_SET( IMC_PFLAGS(ch), PSET_IMCPVISIBLE) )
#endif

#ifdef IMCCIRCLE
//   #if _CIRCLEMUD < CIRCLEMUD_VERSION(3, 0, 21)
//   #  error "Requires CircleMUD 3.0 bpl21+ (varargs output functions)"
//   #endif

   /* This should be in an act.social.h, if it existed. Introducing
      it in an IMC patch would be too intrusive. */
   struct social_messg {
     int act_nr;
     int hide;
     int min_victim_position;
     char *char_no_arg;
     char *others_no_arg;
     char *char_found;
     char *others_found;
     char *vict_found;
     char *not_found;
     char *char_auto;
     char *others_auto;
   };
   #define social_table soc_mess_list
   extern struct social_messg *soc_mess_list;
   #define SOCIALSTRUCT
   struct social_messg *find_social(const char *name);
   typedef struct social_messg SOCIAL_DATA;

   typedef struct char_data CHAR_DATA;
   typedef struct descriptor_data DESCRIPTOR_DATA;

   extern int max_players;

   const char *title_female(int chclass, int level);
   const char *title_male(int chclass, int level);
   void smash_tilde(char *str);

   #define first_descriptor		descriptor_list
   #define current_time			time(NULL)
   #define IMCSTRALLOC			strdup
   #define IMCSTRFREE			free
   #define IMCMAXPLAYERS		max_players	/* comm.c */
   #define PULSE_PER_SECOND		PASSES_PER_SEC
   #define log_string			basic_mud_log
   #define bug				basic_mud_log
   #define URANGE(a, b, c)		((b) < (a) ? (a) : ((b) > (c) ? (c) : (b)))
   #define write_to_buffer(d, txt, n)	write_to_output((txt), (d))
   #define IMCMAX_LEVEL			LVL_IMPL
   #define CH_IMCLEVEL(ch)          GET_LEVEL(ch)
   #define CH_IMCNAME(ch)           GET_NAME(ch)
   #define CH_IMCTITLE(ch)          GET_TITLE(ch)
   #define CH_IMCRANK(ch)           (GET_SEX(ch) == SEX_FEMALE ? title_female(GET_CLASS(ch), GET_LEVEL(ch))	\
								: title_male(GET_CLASS(ch), GET_LEVEL(ch)))
   #define CH_IMCSEX(ch)            GET_SEX(ch)
   #define FIRST_IMCBLACKLIST(ch)      ((ch)->player_specials->imcfirst_blacklist)
   #define LAST_IMCBLACKLIST(ch)       ((ch)->player_specials->imclast_blacklist)
   #define IMC_RREPLY(ch)		((ch)->player_specials->rreply)
   #define IMC_CSUBSCRIBED(ch) 		((ch)->player_specials->imc_csubscribed)
   #define IMC_PFLAGS(ch)      ((ch)->player_specials->imc_pflags)
   #define IS_IMCVISIBLE(ch)		( !IS_NPC(ch) && !IS_SET( IMC_PFLAGS(ch), PSET_IMCPVISIBLE) )
#endif
    
