/***************************************************************************
*                           STAR WARS REALITY 1.0                          *
*--------------------------------------------------------------------------*
* Star Wars Reality Code Additions and changes from the Smaug Code         *
* copyright (c) 1997 by Sean Cooper                                        *
* -------------------------------------------------------------------------*
* Starwars and Starwars Names copyright(c) Lucas Film Ltd.                 *
*--------------------------------------------------------------------------*
* SMAUG 1.0 (C) 1994, 1995, 1996 by Derek Snider                           *
* SMAUG code team: Thoric, Altrag, Blodkai, Narn, Haus,                    *
* Scryn, Rennard, Swordbearer, Gorog, Grishnakh and Tricops                *
* ------------------------------------------------------------------------ *
* Merc 2.1 Diku Mud improvments copyright (C) 1992, 1993 by Michael        *
* Chastain, Michael Quan, and Mitchell Tse.                                *
* Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,          *
* Michael Seifert, Hans Henrik St{rfeldt, Tom Madsen, and Katja Nyboe.     *
* ------------------------------------------------------------------------ *
*		            Bounty Hunter Module    			   *
*                    (  and area capturing as well  )                      *
****************************************************************************/

#include <sys/types.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
/* #include <stdlib.h> */
#include <time.h>
#include <limits.h>
#include "mud.h"

// Used for Bigshot - Gatz
SHIP_DATA * first_ship;
SHIP_DATA * last_ship;

BOUNTY_DATA * first_bounty;
BOUNTY_DATA * last_bounty;
BOUNTY_DATA * first_disintigration;
BOUNTY_DATA * last_disintigration;


void   disintigration args ( ( CHAR_DATA *ch , CHAR_DATA *victim , long amount ) );
void nodisintigration args ( ( CHAR_DATA *ch , CHAR_DATA *victim , long amount ) );
int xp_compute( CHAR_DATA *ch , CHAR_DATA *victim);
void do_bigshot( CHAR_DATA *ch, char *argument );
void bigshot( CHAR_DATA *ch, CHAR_DATA *victim, char *argument);
void show_char_to_char( CHAR_DATA *list, CHAR_DATA *ch );
void show_list_to_char( OBJ_DATA *list, CHAR_DATA *ch, bool fShort,
        bool fShowN );
char *strip_color(char *str);


void save_disintigrations()
{
    BOUNTY_DATA *tbounty;
    FILE *fpout;
    char filename[256];

    sprintf( filename, "%s%s", SYSTEM_DIR, DISINTIGRATION_LIST );
    fpout = fopen( filename, "w" );
    if ( !fpout )
    {
         bug( "FATAL: cannot open disintigration.lst for writing!\n\r", 0 );
         return;
    }
    for ( tbounty = first_disintigration; tbounty; tbounty = tbounty->next )
    {
        fprintf( fpout, "%s\n", tbounty->target );
        fprintf( fpout, "%ld\n", tbounty->amount );
    }
    fprintf( fpout, "$\n" );
    fclose( fpout );

}


bool is_disintigration( CHAR_DATA *victim )
{
    BOUNTY_DATA *bounty;

    for ( bounty = first_disintigration; bounty; bounty = bounty->next )
    if ( !str_cmp( victim->name , bounty->target ) )
             return TRUE;
    return FALSE;
}

BOUNTY_DATA *get_disintigration( char *target )
{
    BOUNTY_DATA *bounty;

    for ( bounty = first_disintigration; bounty; bounty = bounty->next )
       if ( !str_cmp( target, bounty->target ) )
         return bounty;
    return NULL;
}

void load_bounties( )
{
    FILE *fpList;
    char *target;
    char bountylist[256];
    BOUNTY_DATA *bounty;
    long int  amount;

    first_disintigration = NULL;
    last_disintigration	= NULL;

    log_string( "Loading disintigrations..." );

    sprintf( bountylist, "%s%s", SYSTEM_DIR, DISINTIGRATION_LIST );
    fclose( fpReserve );
    if ( ( fpList = fopen( bountylist, "r" ) ) == NULL )
    {
	perror( bountylist );
	exit( 1 );
    }

    for ( ; ; )
    {
        target = feof( fpList ) ? "$" : fread_word( fpList );
        if ( target[0] == '$' )
        break;
	CREATE( bounty, BOUNTY_DATA, 1 );
        LINK( bounty, first_disintigration, last_disintigration, next, prev );
	bounty->target = STRALLOC(target);
	amount = fread_number( fpList );
	bounty->amount = amount;
    }
    fclose( fpList );
    log_string(" Done bounties " );
    fpReserve = fopen( NULL_FILE, "r" );

    return;
}

void do_bounties( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    DESCRIPTOR_DATA *d;
    bool found;
    BOUNTY_DATA *bounty;
    int count = 0;
    long amount;

    one_argument( argument, arg );
    
    set_char_color( AT_WHITE, ch );
    send_to_char( "\n\rBounty                     Amount\n\r", ch );
    if ( arg[0] == '\0' )
   {
    for ( bounty = first_disintigration; bounty; bounty = bounty->next )
    {
	amount = bounty->amount;
        set_char_color( AT_RED, ch );
	ch_printf( ch, "%-26s %-14s\n\r", bounty->target, num_punct(bounty->amount));
        count++;
    }

    if ( !count )
    {
        set_char_color( AT_GREY, ch );
        send_to_char( "There are no bounties set at this time.\n\r", ch );
	return;
    }
   }
    else
   {
    found = FALSE;
        for ( d = first_descriptor; d; d = d->next )
            if ( (d->connected == CON_PLAYING || d->connected == CON_EDITING )
            && ( victim = d->character ) != NULL
            &&   !IS_NPC(victim)
	    &&  !IS_SET(victim->act,PLR_WIZINVIS)
            &&  (bounty = get_disintigration( victim->name )) > 0)  
            {
                found = TRUE;
		set_char_color( AT_RED, ch );
		amount = bounty->amount;
	       	ch_printf( ch, "%-26s %-14s\n\r", victim->name, num_punct(amount) );
            }
        if ( !found )
	{
            send_to_char( "None\n\r", ch );
	    return;
	}
    }
}

void disintigration ( CHAR_DATA *ch , CHAR_DATA *victim , long amount )
{
    BOUNTY_DATA *bounty;
    bool found;

    found = FALSE;

    for ( bounty = first_disintigration; bounty; bounty = bounty->next )
    {
    	if ( !str_cmp( bounty->target , victim->name ))
    	{
    		found = TRUE;
    		break;
    	}
    }

    if (! found)
    {
        CREATE( bounty, BOUNTY_DATA, 1 );
        LINK( bounty, first_disintigration, last_disintigration, next, prev );

        bounty->target      = STRALLOC( victim->name );
        bounty->amount      = 0;
    }

    bounty->amount      = bounty->amount + amount;
    save_disintigrations();
}

void do_addbounty( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_STRING_LENGTH];
    long int amount;
    CHAR_DATA *victim;
    char buf[MAX_STRING_LENGTH];
    BOUNTY_DATA *bounty;

    argument = one_argument(argument, arg);

    if ( !argument || argument[0] == '\0' )
    {
    	send_to_char( "Usage: Addbounty <target> <amount>\n\r", ch );
    	return;
    }
 
   if( ch == victim && !IS_IMMORTAL(ch))
   {
	send_to_char("You can't place bounties on yourself!\n\r", ch);
	return;
   }

    if ( ch->pcdata && ch->pcdata->clan && ( !str_cmp(ch->pcdata->clan->name, "RBH") || ch->pcdata->clan->clan_type == CLAN_GUILD ) )
	{
		send_to_char( "Your job is to capture bounties not post them!\n\r", ch );
		return;
	}

  
    if ( ch->pcdata->clan == NULL  && !IS_IMMORTAL(ch))
	{
		send_to_char("You aren't in ISSP! You can't post bounties!\n\r", ch );
		return;
	}
    if ( ch->pcdata && ch->pcdata->clan && ( !str_cmp(ch->pcdata->clan->name, "GLM") ) )
	{
		send_to_char("You are a Merchant! Focus on making items, not bounties!\n\r", ch);		
		return;
	}

    if ( ch->pcdata && ch->pcdata->clan && ( !str_cmp(ch->pcdata->clan->name, "RDS" ) || !str_cmp(ch->pcdata->clan->name, "WTS" ) )&& !IS_IMMORTAL(ch) )
	{
		send_to_char( "Crime syndicates can't post bounties!\n\r", ch);
		return;
	}

   	if (argument[0] == '\0' )
   	    amount = 0;
   	else
   		amount = atoi (argument);

   	if ( amount < 5000 )
   	{
   		send_to_char( "A bounty should be at least 5000 wulongs.\n\r", ch );
   		return;
   	}

	if( amount > 2000000000)
	{
		send_to_char( "You can't put that large of a bounty on someone!\r\n", ch);
		return;
	}

   	if ( !(victim = get_char_world( ch, arg )) )
   	{
   	    send_to_char( "They don't appear to be here .. wait until they log in.\n\r", ch );
   	    return;
   	}

	bounty = get_disintigration( arg );

   	if ( IS_NPC(victim) )
   	{
   		send_to_char( "You can only set bounties on other players .. not mobs!\n\r", ch );
		return;
   	}

   	if (amount <= 0)
   	{
   	    send_to_char( "Nice try! How about 1 or more wulongs instead.\n\r", ch );
   	    return;
   	}

   	if (ch->gold < amount)
   	{
   		send_to_char( "You don't have that many wulongs!\n\r", ch );
   		return;
   	}
	
	if(bounty && (bounty->amount + amount) > 2000000000)
	{
		send_to_char("No way a bounty could be over 2 billion wulongs!\r\n", ch);
		return;
	}

   	ch->gold = ch->gold - amount;
   	send_to_char( "Bounty has been added.\n\r", ch );
   	sprintf( buf, "%s has added %s wulongs to the bounty on %s.", ch->name, num_punct(amount) , victim->name );
    echo_to_all ( AT_RED , buf, 0 );
   	disintigration( ch, victim, amount);

}

void do_rembounty(  CHAR_DATA *ch, char *argument )
{
  BOUNTY_DATA *bounty = get_disintigration( argument );

  if ( bounty != NULL )
  {
    remove_disintigration(bounty);
	send_to_char( "Bounty has been removed.\n\r", ch );
  }
  return;

}

void remove_disintigration( BOUNTY_DATA *bounty )
{
	UNLINK( bounty, first_disintigration, last_disintigration, next, prev );
	STRFREE( bounty->target );
	DISPOSE( bounty );

	save_disintigrations();
}

void claim_disintigration( CHAR_DATA *ch , CHAR_DATA *victim )
{
	BOUNTY_DATA *bounty;
	long int     exp;
	char buf[MAX_STRING_LENGTH];
	int chance;

    if ( IS_NPC(victim) )
    	return;

	bounty = get_disintigration( victim->name );

    if ( ch == victim )
    {
    	if ( bounty != NULL )
        remove_disintigration(bounty);
        return;
	}

    if (bounty && (!ch->pcdata || !ch->pcdata->clan || ( str_cmp(ch->pcdata->clan->name, "RBH") && ch->pcdata->clan->clan_type != CLAN_GUILD ) ) )
	{
		if( ch->pcdata->clan && !str_cmp(ch->pcdata->clan->name, "ISSP"))
		{
			
			bigshot( victim, ch, "capture");
			/*
	                sprintf( buf, "%s has been captured by ISSP!", victim->name);
	                echo_to_all ( AT_RED , buf, 0 ); */
		        remove_disintigration(bounty);
			if(ch->pcdata->bank < 2000000000)
			{
				ch->pcdata->bank += (bounty->amount/4);
				ch_printf( ch, "You recieve a small bonus of %d your hard work!\n\r", (bounty->amount/4) );
			}
			else
				send_to_char("Your bank account is too full to recieve your ISSP bonus!\n\r", ch);
		}
	   //	remove_disintigration(bounty);
	   	bounty = NULL;
	}

	if (bounty == NULL)
	{
		if ( IS_SET(victim->act , PLR_KILLER ) && !IS_NPC(ch) )
	    {
	       exp = URANGE(1, xp_compute(ch, victim) , ( exp_level(ch->skill_level[HUNTING_ABILITY]+1) - exp_level(ch->skill_level[HUNTING_ABILITY]) ));
	       gain_exp( ch , exp , HUNTING_ABILITY );
	       set_char_color( AT_BLOOD, ch );
	       ch_printf( ch, "You receive %ld hunting experience for executing a wanted killer.\n\r", exp );
	    }
	    else if ( !IS_NPC(ch) )
	    {
	        SET_BIT(ch->act, PLR_KILLER );
	        ch_printf( ch, "You are now wanted for the murder of %s.\n\r", victim->name );
	    }
	
	    if(IS_SET(victim->pcdata->flags, PCFLAG_OUTLAW) && !IS_NPC(ch) && !IS_NPC(victim))
            {
		chance = number_range(1,3);
		REMOVE_BIT(victim->pcdata->flags, PCFLAG_OUTLAW);
                sprintf( buf, "The wretched outlaw %s has been killed!", victim->name);
                echo_to_all ( AT_RED , buf, 0 );
		switch(chance)
		{
			case 1:
				ch->gold += 300000;
				send_to_char("You are rewarded 300,000 wulongs!\n\r", ch);
				break;
			case 2:
				ch->strtrain += 300;
				send_to_char("You are rewarded a large amount of strength training!\n\r", ch);
				break;
			case 3:
				ch->pcdata->quest_curr += 4;
				send_to_char("You are rewarded 4 quest points!\n\r", ch);
				break;
			default:
				ch->inttrain += 300;
				send_to_char("You are rewarded a large amount of intelligence training!\n\r", ch);
        	  }
	    }
	    return;

	}

	ch->gold += bounty->amount;

    exp = URANGE(1, bounty->amount + xp_compute(ch, victim) , ( exp_level(ch->skill_level[HUNTING_ABILITY]+1) - exp_level(ch->skill_level[HUNTING_ABILITY]) ));
	if ( ch->pcdata && ch->pcdata->clan && ( !str_cmp(ch->pcdata->clan->name, "GLM") ) )
	{
		ch_printf( ch, "&w&W&RGLM is suppose to make weapons, not use them!\n\r");
		exp = exp/2;
	}
	gain_exp( ch , exp , HUNTING_ABILITY );

	set_char_color( AT_BLOOD, ch );
	ch_printf( ch, "You receive %ld experience and %ld wulongs\n\r from the bounty on %s\n\r", exp, bounty->amount, bounty->target );
	bigshot( victim, ch, "capture");
	/*
	sprintf( buf, "The bounty on %s has been claimed!", victim->name);
	echo_to_all ( AT_RED , buf, 0 );
	sprintf( buf, "%s is rushed away to the police.", victim->name );
	echo_to_all ( AT_RED , buf, 0 );
	*/
	if ( !IS_SET(victim->act , PLR_KILLER ) )
		SET_BIT(ch->act, PLR_KILLER );
	remove_disintigration(bounty);
}

/* Pretty nifty function to be more Bebop like
   in this case ch is the person who had the bounty
   and victim is the person who nabbed them and
   the argument is how they nabbed them -Gatz
*/
void  bigshot( CHAR_DATA *ch , CHAR_DATA *victim , char *argument)
{
    BOUNTY_DATA *bounty;
    CLAN_DATA *clan;
    char buf[MAX_STRING_LENGTH];
    char buf1[MAX_STRING_LENGTH];
    char arg1[MAX_STRING_LENGTH];

    argument = one_argument(argument, arg1);

    // Just incase!
    if( arg1[0] == '\0')
    	return;
   // This way you turn it off for reasons like RP, with cset that is - Gatz
   if( sysdata.big_shot )
	return;
    

    if ( !str_prefix( arg1 , "capture" ) )
    {	
	// Unique set of cases to make it more livelier - Gatz
	if( victim->pcdata && victim->pcdata->clan && ( !str_cmp(victim->pcdata->clan->name, "ISSP" )))
	{
	switch(number_range(0,4))
	{
		case 4:
			do_bigshot(victim, "&CPunch: Well Amigos, this is a special bulletin!");
			sprintf( buf, "&PJudy: That's right, %s has finally been caught by %s!", ch->name, victim->name);
			do_bigshot( victim, buf);
			do_bigshot(victim, "&CPunch: Thank Goodness! ISSP is keeping the universe safe!");
		break;
		case 3:
			do_bigshot( victim, "&PJudy: A big thanks to the boys in blue of ISSP!");
			sprintf( buf, "&CPunch: That's right, %s of ISSP caught %s today!", victim->name, ch->name);
			do_bigshot( victim, buf);
			do_bigshot( victim, "&PJudy: Keep up the hard work boys!");
		break;
		case 2:
			do_bigshot( victim, "&CPunch: Whoa Amigos! The universe has shrunk by one criminal today!");
			sprintf( buf, "&PJudy: Yeah, %s of ISSP has caught the violent %s today!", victim->name, ch->name);
			do_bigshot( victim, buf);
			do_bigshot( victim, "&CPunch: ISSP, lets see this be a continuing trend!");
		break;
		case 1:
			do_bigshot( ch, "&PJudy: Hey all you bounty hunters out in the universe!");
			sprintf( buf, "&CPunch: Yee haw! %s has been caught today. By ISSP's own %s!",ch->name, victim->name);
			do_bigshot( victim, buf);
			do_bigshot( victim, "&PJudy: Now, now Punch. Your blood pressure!");
			do_bigshot( victim, "&CPunch: You are not my Mother!");
			do_bigshot( victim, "Judy glares at Punch evilly.");
			do_bigshot( victim, "&CPunch: Sorry Ma'am.");
		break;
		default:
			do_bigshot( victim, "&CPunch: Special Report folks, listen in!");
			sprintf( buf, "&PJudy: %s was caught today by %s of ISSP, good job!", ch->name, victim->name);
			do_bigshot( victim, buf);
		break;
	}
	}
	else
	switch(number_range(0,4))
	{
		case 4:
			do_bigshot( victim, "&CPunch: Hola Amigos! Aye dios mio, we got hot news!");
			sprintf( buf, "&PJudy: That notourious bad guy, %s, has been caught by %s!", ch->name, victim->name);
			do_bigshot( victim, buf);
			do_bigshot( victim, "&CPunch: We will keep you posted for more details!");
		break;
		case 3:
			do_bigshot( victim, "&PJudy: Big news bounty hunters! There is one less bounty to get!");
			sprintf( buf, "&CPunch: The person named %s has been captured by %s!", ch->name, victim->name);
			do_bigshot( victim, buf);
		break;
		case 2:
			do_bigshot( victim, "&CPunch: Hola Amigos! Special news flash here!");
			sprintf( buf, "&PJudy: %s has been caught by %s!", ch->name, victim->name);
			do_bigshot( victim, buf);
			do_bigshot( victim, "&CPunch: Good work! You did good!");
		break;
		case 1:
			do_bigshot( victim, "&PJudy: Hey everyone, we got some big news!");
			sprintf( buf, "&CPunch: %s has just been captured! %s was able to get him!", ch->name, victim->name);
			do_bigshot( victim, buf);
			do_bigshot( victim, "&PJudy: Kudos to you bounty hunter!");
		break;
		default:
			do_bigshot( victim, "&CPunch: We got a big news flash for y'all!");
			sprintf(buf, "&PJudy: The bounty on %s's head has been claimed by %s!", ch->name, victim->name);
			do_bigshot( victim, buf);
			do_bigshot( victim, "&CPunch: Good job and keep up the good work!");
		break;
	}
    }
    if ( !str_prefix( arg1 , "surrender" ) )
    {
        if( victim->pcdata && victim->pcdata->clan && ( !str_cmp(victim->pcdata->clan->name, "ISSP" )))
        {
        switch(number_range(0,4))
        {
		case 4:
			do_bigshot( victim, "&CPunch: Whoa, big news on the bounty scene amigos!");
			sprintf( buf, "&PJudy: %s has just turned himself into ISSP's %s!", ch->name, victim->name);
			do_bigshot( victim, buf);
			do_bigshot( victim, "&CPunch: The ISSP is cleaning house! Good work!");
			break;
		case 3:
			do_bigshot( victim, "&PJudy: Shucks howdy everyone!");
			sprintf( buf, "&CPunch: The violent %s surrendered to ISSP's %s today!", ch->name, victim->name);
			do_bigshot( victim, buf);
			do_bigshot( victim, "&PJudy: Keep up the good work guys!");
		break;
		case 2:
			do_bigshot( victim, "&CPunch: Howdy Amigos! We got some special news for you today!");
			sprintf( buf, "&PJudy: %s just gave up to ISSP today! The surrender was to %s.", ch->name, victim->name);
			do_bigshot( victim, buf);
			do_bigshot( victim, "&CPunch: Well, ISSP must be proud and they should be. Good work!");
		break;
		case 1:
			do_bigshot( victim, "&PJudy: Big news today folks! Tell 'em Punch!");
			sprintf( buf, "&CPunch: %s just surrendered to ISSP today! %s was the supervising ISSP!", ch->name, victim->name);
			do_bigshot( victim, buf);
			do_bigshot( victim, "&PJudy: Good job ISSP!");
		break;
		default:
			do_bigshot( victim, "&CPunch: Hey there folks, we got some hot news!");
			sprintf( buf, "&PJudy: %s surrendered to %s of ISSP, alright!", ch->name, victim->name);
			do_bigshot( victim, buf);
		break;
	}
	}
	else
	switch(number_range(0,4))
	{
		case 4:
			do_bigshot( victim, "&CPunch: Hola amigos! Listen up, this is a special news break!");
			sprintf( buf, "&PJudy: %s has just turned themself into %s of RBH!",
				ch->name, victim->name);
			do_bigshot( victim, buf);
			sprintf( buf, "Good work %s!",
				(victim->sex == 1)? "amigo": "amiga");
			do_bigshot( victim, buf);
		break;
		case 3:
			do_bigshot( victim, "&PJudy: Howdy partners! We got some big news!");
			sprintf( buf, "&CPunch: The bounty on %s has been claimed!",
				ch->name);
			do_bigshot( victim, buf);
			sprintf( buf, "&PJudy: %s turned %self into %s eariler today!",
				ch->name, (ch->sex == 1)? "his":"her", victim->name);
			do_bigshot( victim, buf);
			do_bigshot( victim, "&CPunch: Good job!");
		break;
		case 2:
			do_bigshot( victim, "&CPunch: Hold it bounty hunters out there, we got an news update!");
			sprintf( buf, "&PJudy: %s surrendered himself to %s of RBH today!", ch->name, victim->name);
			do_bigshot( victim, buf);
			do_bigshot( victim, "&CPunch: Just another fine example of good bounty hunting!");
		break;
		case 1:
			do_bigshot( victim, "&PJudy: Hey cowboys we got some news for you!");
			sprintf( buf, "&CPunch: %s has surrender today! %s handled the process.",
				ch->name, victim->name);
			do_bigshot( victim, buf);
			do_bigshot( victim, "&PJudy: That is one less bounty to catch! Good work!");
		break;
		default:
			do_bigshot( victim, "&CPunch: This is a special news break!");
			sprintf( buf, "&PJudy: %s of RBH has caught the criminal %s!",
				victim->name,
				ch->name);
			do_bigshot( victim, buf);
			do_bigshot( victim, "&CPunch: Good luck guys!");
		break;
	}
    }
    if ( !str_prefix( arg1 , "report" ) )
    {
	bounty = get_disintigration( ch->name );
	// A just in case crash proofer - Gatz
	if( !bounty )
		return; 
	do_bigshot( victim, "Big Shot's theme song American Money begins to play.");
	switch(number_range(0,2))
	{
		case 2:
		do_bigshot( victim, "&CPunch: Amigo! Welcome to another exciting episode of Big Shot!");
		do_bigshot( victim, "&PJudy: That's right cowboys, get ready for another hot bounty of the day!");
		break;
		case 1:
		clan = get_clan( "RBH" );
		if(!clan)
		do_bigshot( victim, "&PJudy: Hey all you 300,000 bounty hunters out there!");
		else
		{
			sprintf(buf, "&PJudy: Hey all you %d bounty hunters out there!",
				clan->members);
			do_bigshot( victim, buf);
		}
		do_bigshot( victim, "&CPunch: Howdy Partners! We got another sizzling episode of Big Shot today!");
		break;
		default:
		do_bigshot( victim, "&CPunch: Hey bounty hunters, welcome to another episode of Big Shot.");
		do_bigshot( victim, "&PJudy: Today we have a hot, hot, hot bounty for y'all!");
	}
	SHIP_DATA *ship;
	bool checkship;
	char shipbuf[MAX_STRING_LENGTH];
//	char *shipbuf;	

  	checkship = FALSE;
  	for ( ship = first_ship; ship; ship = ship->next )
  	{
        	if ( ship->class > SHIP_PLATFORM )
        	        continue;
	
        	if (ship->type == MOB_SHIP)
        	        continue;
		if(ship->firstroom == 0 || ship->lastroom == 0)	
			continue;
		if(!ch->in_room)
			break;

		sh_int chv, fv, lv;

		chv = ch->in_room->vnum;
		fv = ship->firstroom;
		lv = ship->lastroom;
		if(chv >= fv && chv <= lv)
		{
			checkship = TRUE;
			break;
		}
        }
        if(checkship)
	{
		sprintf(shipbuf, ship->name);
		sprintf(shipbuf,strip_color(shipbuf));
	}
	
	switch(number_range(0,4))
	{
		case 4:
		if( bounty->amount > 5000000)
		sprintf( buf, "&CPunch: Wow folks, we got a hot bounty today of %s!",
			num_punct( bounty->amount));
		else
		sprintf( buf, "&CPunch: Looks like we got a small fry bounty of %s.",
			num_punct( bounty->amount));
		do_bigshot( victim, buf);
		sprintf( buf, "&PJudy: The bounty is on %s's head!", ch->name);
		do_bigshot( victim, buf);
		sprintf( buf, "&CPunch: This %s has been seen lurking around %s from reports!",
			( ch->sex == 1)? "bandido": "bandida",
			(ch->in_room->area->planet != NULL)
                        ? ch->in_room->area->planet->name :
			(checkship)? shipbuf: "space" );
		do_bigshot( victim, buf);
		sprintf( buf, "&PJudy: Good luck catching %s!",
			( ch->sex == 1)? "him" : "her");
		break;
		case 3:
		sprintf( buf, "&CPunch: This sneaky character has been seen hiding in %s!",
                        (ch->in_room->area->planet != NULL)
                        ? ch->in_room->area->planet->name : 
			(checkship)? shipbuf : "space" );
		do_bigshot( victim, buf);
		sprintf( buf, "&PJudy: %s has been reported to be armed and dangerous!",
			ch->name);
		do_bigshot( victim, buf);
		do_bigshot( victim, "&CPunch: So be careful amigos!");
		break;
		case 2:
		if( bounty->amount > 40000000)
		{
		sprintf( buf, "&PJudy: We got a real hot one today! %s has a large bounty of %s!",
			ch->name, num_punct(bounty->amount));
		do_bigshot( victim, buf);
		sprintf( buf, "&CPunch: This person is shifty! Last reports claim %s was on %s!",
			( ch->sex == 1)? "he" : "she",
                        (ch->in_room->area->planet != NULL)
                        ? ch->in_room->area->planet->name : 
                        (checkship)? shipbuf :"space" );
		do_bigshot( victim, buf);
		do_bigshot( victim, "&PJudy: Good luck on this one guys, it seems difficult!");
		}
		else
		{
		sprintf( buf, "&PJudy: Today has been sort of a slow day, however %s has picked things up!",
			ch->name);
		do_bigshot( victim, buf);
		sprintf( buf, "&CPunch: This guy has been avoiding ISSP and RBH grasp for awhile! %s was last spotted on %s, however.",
			(ch->sex == 1)? "He" : "She",
                        (ch->in_room->area->planet != NULL)
                        ? ch->in_room->area->planet->name : 
                        (checkship)? shipbuf : "space" );
		do_bigshot( victim, buf);
		do_bigshot( victim, "&PJudy: Good luck ISSP and RBH!");
		}
		break;
		case 1:
		sprintf( buf, "&CPunch: %s is on the loose!",
			ch->name);
		do_bigshot( victim, buf);
		sprintf( buf, "&PJudy: Our sources say %s has been seen around %s.",
			(ch->sex == 1)? "he" : "she",
                        (ch->in_room->area->planet != NULL)
                        ? ch->in_room->area->planet->name : 
                        (checkship)? shipbuf : "space" );
		do_bigshot( victim, buf);
		break;
		default:
		sprintf( buf, "&PJudy: The bounty of the day is %s!", ch->name);
		do_bigshot( victim, buf); 
		sprintf( buf, "&CPunch: This sly %s can be seen hanging around %s!",
			( ch->sex == 1)
			? "hombre" : "chica",
			( ch->in_room->area->planet != NULL)
			? ch->in_room->area->planet->name : 
                        (checkship)? shipbuf : "space" );
		do_bigshot( victim, buf); 
		break;
	}
	switch( number_range(0,5))
	{
		case 5:
			do_bigshot( victim, "&CPunch: Good bye and so long Amigos!");
			do_bigshot( victim, "&PJudy: Happy hunting y'all!");
		break;
		case 4:
			do_bigshot( victim, "&CPunch: That is all for today!");
			do_bigshot( victim, "&PJudy: Adios cowboys!");
		break;
		case 3:
			do_bigshot( victim, "&PJudy: See ya Space Cowboy!");
			do_bigshot( victim, "&CPunch: Till Next time Amigos!");
		break;
		case 2:
			do_bigshot( victim, "&CPunch: Adios Amigos!");
			do_bigshot( victim, "&PJudy: Shucks howdy, that was fun. Bye!");
		break;
		case 1:
			do_bigshot( victim, "&PJudy: See all you guys later!");
			do_bigshot( victim, "&CPunch: Goodbye and Adios from all of us at Big Shot!");
		break;
		default:
			do_bigshot( victim, "&CPunch: So long Amigos!");
			do_bigshot( victim, "&PJudy: See y'all next time!");
		break;
	}
	do_bigshot( victim, "A large gun comes on the screen and goes \"BANG!\"");	
    }
    return;    
}


void do_surrender( CHAR_DATA *ch, char *argument )
{
    BOUNTY_DATA *bounty;
    char arg[MAX_STRING_LENGTH];
    long int amount;
    CHAR_DATA *victim;
    char buf[MAX_STRING_LENGTH];
    bool canget = FALSE;

    if ( IS_NPC(ch) )
        return;

    one_argument(argument, arg);
    if ( arg[0] == '\0' )
    {
        send_to_char( "Usage: Surrender 'Name'\n\r", ch );
        return;
    }
    if ( (victim = get_char_room(ch, arg)) == NULL || IS_NPC(victim) )
    {
        send_to_char( "They aren't here.\n\r", ch );
        return;
    }

    if( ch == victim )
    {
	send_to_char("You can't do that!\n\r", ch);
	return;
    }
    bounty = get_disintigration( ch->name );
    if(!bounty)
    {
	send_to_char("&RYou need a bounty to surrender!\n\r", ch);
	return;
    }
    if ( victim->pcdata && victim->pcdata->clan && ( !str_cmp(victim->pcdata->clan->name, "ISSP") ) )
        	canget = TRUE;    
    if ( victim->pcdata && victim->pcdata->clan && ( !str_cmp(victim->pcdata->clan->name, "RBH") ) )
        	canget = TRUE;
    if( canget)
    {
	act(AT_GREEN, "$n puts $s hands in the air to surrender!", ch, victim, NULL, TO_ROOM);
	act(AT_GREEN, "You put your hands in the air to surrender!", ch, victim, NULL, TO_CHAR);
	do_say(ch, "I give up, I surrender! Please let me live!");
       	OBJ_DATA *obj;
       	OBJ_DATA *obj_next;
	int amount, cnt = 0;

       	if ( ( obj = get_eq_char( ch, WEAR_DUAL_WIELD ) ) != NULL )
       	   unequip_char( ch, obj );
       	if ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) != NULL )
       	   unequip_char( ch, obj );
       	if ( ( obj = get_eq_char( ch, WEAR_HOLD ) ) != NULL )
       	   unequip_char( ch, obj );
       	if ( ( obj = get_eq_char( ch, WEAR_MISSILE_WIELD ) ) != NULL )
       	   unequip_char( ch, obj );
       	if ( ( obj = get_eq_char( ch, WEAR_LIGHT ) ) != NULL )
          unequip_char( ch, obj );

        for ( obj = ch->first_carrying; obj; obj = obj_next )
	{
	    obj_next = obj->next_content;

	    if ( obj->wear_loc == WEAR_NONE )
	    {
		if ( obj->pIndexData->progtypes & DROP_PROG && obj->count > 1 )
		{
		   ++cnt;
		   separate_obj( obj );
		   obj_from_char( obj );
		   if ( !obj_next )
		     obj_next = ch->first_carrying;
		}
		else
		{
		   cnt += obj->count;
		   obj_from_char( obj );
		}
		act( AT_ACTION, "$n drops $p.", ch, obj, NULL, TO_ROOM );
		act( AT_ACTION, "You drop $p.", ch, obj, NULL, TO_CHAR );
		obj = obj_to_room( obj, ch->in_room );
	    }
	}
	amount = ch->pcdata->bank;
	amount = amount/10;

	if( (ch->pcdata->bank - amount) >= 0 && amount > 0)
	{
		ch_printf(ch,"&RThe Prison forces you to 'donate' %d wulongs to pay for expenses.\n\r", amount);
		ch->pcdata->bank -= amount;
	}
	char_from_room(ch);
  	char_to_room(ch, get_room_index(ROOM_PRISON));
	do_look(ch, "auto");
	if( victim->pcdata->clan && !str_cmp(victim->pcdata->clan->name, "ISSP"))
		amount = (bounty->amount/4);
	else
		amount = bounty->amount;

        if(victim->pcdata->bank < 2000000000)
        {
		victim->pcdata->bank += amount;
		ch_printf( victim, "You recieve a small bonus of %s your hard work!\n\r", num_punct(amount) );
        }

        ch->pcdata->bountyrelease = (bounty->amount/625000);
        if( ch->pcdata->bountyrelease > 40 )
               ch->pcdata->bountyrelease = 40;
        if( ch->pcdata->bountyrelease < 3)
               ch->pcdata->bountyrelease = 3;
	if( ch->pcdata->bountyrelease == 0)
		ch->pcdata->bountyrelease = 3;	
/*
        if( ch->pcdata->clan && !str_cmp(ch->pcdata->clan->name, "ISSP") ||
   	     ( !str_cmp(ch->pcdata->clan->name, "RBH")))
		ch->pcdata->bountyrelease += 10;
*/
	// Lets make sure we save the characters	
        remove_disintigration(bounty);

	save_char_obj(victim);
        save_char_obj(ch);
	// Yee-Haw, it's Big Shot time!
	bigshot( ch, victim, "surrender");
     }
     else
	send_to_char("They are not in RBH or ISSP!\n\r",ch);
     return;
}
	



	

