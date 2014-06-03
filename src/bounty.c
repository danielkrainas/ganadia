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

    if ( ch->pcdata && ch->pcdata->clan && ( !str_cmp(ch->pcdata->clan->name, "Hunters") || ch->pcdata->clan->clan_type == CLAN_GUILD ) )
	{
		send_to_char( "Your job is to capture bounties not post them!\n\r", ch );
		return;
	}

  
    if ( ch->pcdata->clan == NULL  && !IS_IMMORTAL(ch))
	{
		send_to_char("You can't post bounties!\n\r", ch );
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
   		send_to_char( "A bounty should be at least 5000 dollars.\n\r", ch );
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
   	    send_to_char( "Nice try! How about 1 or more dollars instead.\n\r", ch );
   	    return;
   	}

   	if (ch->gold < amount)
   	{
   		send_to_char( "You don't have that many dollars!\n\r", ch );
   		return;
   	}
	
	if(bounty && (bounty->amount + amount) > 2000000000)
	{
		send_to_char("No way a bounty could be over 2 billion dollars!\r\n", ch);
		return;
	}

   	ch->gold = ch->gold - amount;
   	send_to_char( "Bounty has been added.\n\r", ch );
   	sprintf( buf, "%s has added %s dollars to the bounty on %s.", ch->name, num_punct(amount) , victim->name );
    echo_to_all ( AT_RED , buf, 0 );
   	disintigration( ch, victim, amount);

}

void do_rembounty(  CHAR_DATA *ch, char *argument )
{
  BOUNTY_DATA *bounty = get_disintigration( argument );
  CHAR_DATA *victim;

  if(!(victim = get_char_world( ch, argument )))
  {
	send_to_char("&RPlayer must be online to remove the bounty\n\r",ch);
	return;
  }

  if(0>(victim->pcdata->quest_curr-5))
  {
	send_to_char("&RThat person needs atleast 5QP to remove the bounty\n\r",ch);
	return;
  }
  if ( bounty != NULL )
  {
    victim->pcdata->quest_curr-=5;	
    send_to_char("&RYou are charged 5QP to remove your bounty\n\r",victim);
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
				send_to_char("You are rewarded 300,000 dollars!\n\r", ch);
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
	ch_printf( ch, "You receive %ld experience and %ld dollars\n\r from the bounty on %s\n\r", exp, bounty->amount, bounty->target );
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
			do_bigshot(victim, "&RCindy&z:&W Well folks, this is a special bulletin!");
			sprintf( buf, "&RCindy&z:&W %s has finally been caught by %s!", ch->name, victim->name);
			do_bigshot( victim, buf);
			do_bigshot(victim, "&RCindy&z:&W Thank Goodness! The Guardians are well... guarding us, hehe!");
		break;
		case 3:
			do_bigshot( victim, "&RCindy&z:&W Looks like we have a guardian angel among us.");
			sprintf( buf, "&RCindy&z:&W That's right, %s of the Guardians caught %s today!", victim->name, ch->name);
			do_bigshot( victim, buf);
			do_bigshot( victim, "&RCindy&z:&W Keep up the hard work boys!");
		break;
		case 2:
			do_bigshot( victim, "&RCindy&z:&W Whoa! The universe has shrunk by one criminal today!");
			sprintf( buf, "&RCindy&z:&W Yeah, %s of the Guardians has caught the violent %s today!", victim->name, ch->name);
			do_bigshot( victim, buf);
			do_bigshot( victim, "&RCindy&z:&W Well Guardians, stay sharp guys.");
		break;
		case 1:
			do_bigshot( ch, "&RCindy&z:&W Hey all you Hunters out in the universe!");
			sprintf( buf, "&RCindy&z:&W %s has been caught today. By Guardian's own %s!",ch->name, victim->name);
			do_bigshot( victim, buf);
			do_bigshot( victim, "&RCindy&z:&W Reminds me, I'm free and looking!");
			do_bigshot( victim, "&RCindy&z:&W Any of you Guardians like red heads im right here");
		break;
		default:
			do_bigshot( victim, "&RCindy&z:&W Special Report folks, listen in!");
			sprintf( buf, "&RCindy&z:&W %s was caught today by %s of the Guardians, good job!", ch->name, victim->name);
			do_bigshot( victim, buf);
		break;
	}
	}
	else
	switch(number_range(0,4))
	{
		case 4:
			do_bigshot( victim, "&RCindy&z:&W Hi again, its me Cindy for another news report!");
			sprintf( buf, "&RCindy&z:&W That notourious bad guy, %s, has been caught by %s!", ch->name, victim->name);
			do_bigshot( victim, buf);
			do_bigshot( victim, "&RCindy&z:&W We will keep you posted for more details!");
		break;
		case 3:
			do_bigshot( victim, "&RCindy&z:&W Big news! There is one less bounty to get!");
			sprintf( buf, "&RCindy&z:&W The person named %s has been captured by %s!", ch->name, victim->name);
			do_bigshot( victim, buf);
		break;
		case 2:
			do_bigshot( victim, "&RCindy&z:&W Hola Amigos! Special news flash here!");
			sprintf( buf, "&RCindy&z:&W %s has been caught by %s!", ch->name, victim->name);
			do_bigshot( victim, buf);
			do_bigshot( victim, "&RCindy&z:&W Good work! You did good!");
		break;
		case 1:
			do_bigshot( victim, "&RCindy&z:&W Hey everyone, we got some big news!");
			sprintf( buf, "&RCindy&z:&W %s has just been captured! %s was able to get him!", ch->name, victim->name);
			do_bigshot( victim, buf);
			do_bigshot( victim, "&RCindy&z:&W Kudos to you bounty hunter!");
		break;
		default:
			do_bigshot( victim, "&RCindy&z:&W We got a big news flash for you all!");
			sprintf(buf, "&RCindy&z:&W The bounty on %s's head has been claimed by %s!", ch->name, victim->name);
			do_bigshot( victim, buf);
			do_bigshot( victim, "&RCindy&z:&W Good job and keep up the good work!");
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
			do_bigshot( victim, "&RCindy&z:&W Whoa, big news on the bounty scene guys!");
			sprintf( buf, "&RCindy&z:&W %s has just turned himself into Guardian's %s!", ch->name, victim->name);
			do_bigshot( victim, buf);
			do_bigshot( victim, "&RCindy&z:&W The Guardians are cleaning house! Good work!");
			break;
		case 3:
			do_bigshot( victim, "&RCindy&z:&W How is everyone doing, Cindy again!");
			sprintf( buf, "&RCindy&z:&W The violent %s surrendered to Guardian's %s today!", ch->name, victim->name);
			do_bigshot( victim, buf);
			do_bigshot( victim, "&RCindy&z:&W Keep up the good work guys!");
		break;
		case 2:
			do_bigshot( victim, "&RCindy&z:&W Hidi ho! I got some special news for you today!");
			sprintf( buf, "&RCindy&z:&W %s just gave up to Guardian today! The surrender was to %s.", ch->name, victim->name);
			do_bigshot( victim, buf);
			do_bigshot( victim, "&RCindy&z:&W Well, Guardian must be proud and they should be. Good work!");
		break;
		case 1:
			do_bigshot( victim, "&RCindy&z:&W Big news today folks!");
			sprintf( buf, "&RCindy&z:&W %s just surrendered to Guardian today! %s was the supervising Guardian!", ch->name, victim->name);
			do_bigshot( victim, buf);
			do_bigshot( victim, "&RCindy&z:&W Good job Guardian!");
		break;
		default:
			do_bigshot( victim, "&RCindy&z:&W Hey there folks, I got some hot news!");
			sprintf( buf, "&RCindy&z:&W %s surrendered to %s of Guardian, alright!", ch->name, victim->name);
			do_bigshot( victim, buf);
			do_bigshot( victim, "&RCindy&z:&W Good job Guardian!");
		break;
	}
	}
	else
	switch(number_range(0,4))
	{
		case 4:
			do_bigshot( victim, "&RCindy&z:&W Okay! Listen up, this is a special news break!");
			sprintf( buf, "&RCindy&z:&W %s has just turned themself into %s of Hunters!",
				ch->name, victim->name);
			do_bigshot( victim, buf);
			sprintf( buf, "Good work %s!",
				(victim->sex == 1)? "dude": "dudet");
			do_bigshot( victim, buf);
		break;
		case 3:
			do_bigshot( victim, "&RCindy&z:&W Hi guys! I got some big news!");
			sprintf( buf, "&RCindy&z:&W The bounty on %s has been claimed!",
				ch->name);
			do_bigshot( victim, buf);
			sprintf( buf, "&RCindy&z:&W %s turned %self into %s eariler today!",
				ch->name, (ch->sex == 1)? "his":"her", victim->name);
			do_bigshot( victim, buf);
			do_bigshot( victim, "&RCindy&z:&W Good job!");
		break;
		case 2:
			do_bigshot( victim, "&RCindy&z:&W Hold it bounty hunters out there, I got an news update!");
			sprintf( buf, "&RCindy&z:&W %s surrendered himself to %s of Hunters today!", ch->name, victim->name);
			do_bigshot( victim, buf);
			do_bigshot( victim, "&RCindy&z:&W Just another fine example of good bounty hunting!");
		break;
		case 1:
			do_bigshot( victim, "&RCindy&z:&W Hey its me, Cindy and do I have some news for you!");
			sprintf( buf, "&RCindy&z:&W %s has surrender today! %s handled the process.",
				ch->name, victim->name);
			do_bigshot( victim, buf);
			do_bigshot( victim, "&RCindy&z:&W That is one less bounty to catch! Good work!");
		break;
		default:
			do_bigshot( victim, "&RCindy&z:&W This is a special news break!");
			sprintf( buf, "&RCindy&z:&W %s of Hunters has caught the criminal %s!",
				victim->name,
				ch->name);
			do_bigshot( victim, buf);
			do_bigshot( victim, "&RCindy&z:&W Good luck guys!");
		break;
	}
    }
    if ( !str_prefix( arg1 , "report" ) )
    {
	bounty = get_disintigration( ch->name );
	// A just in case crash proofer - Gatz
	if( !bounty )
		return; 
	do_bigshot( victim, "&r---------------------------------=Bulletin=-----------------------------------");
	switch(number_range(0,2))
	{
		case 2:
		do_bigshot( victim, "&RCindy&z:&W We interrupt you for this special report!");
		break;
		case 1:
		clan = get_clan( "Hunters" );
		if(!clan)
		do_bigshot( victim, "&RCindy&z:&W Vigilantes be on the alert!");
		else
		{
			sprintf(buf, "&RCindy&z:&W Hey all you %d bounty hunters out there!",
				clan->members);
			do_bigshot( victim, buf);
		}
		do_bigshot( victim, "&RCindy&z:&W This is Cindy from Gandadia News, thank you for joining us.");
		break;
		default:
		do_bigshot( victim, "&RCindy&z:&W Do I have a hot lead on a bounty today.");
		//do_bigshot( victim, "&PJudy: Today we have a hot, hot, hot bounty for y'all!");
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
		sprintf( buf, "&RCindy&z:&W Wow folks, have I found a hot bounty today of %s!",
			num_punct( bounty->amount));
		else
		sprintf( buf, "&RCindy&z:&W Looks like we got a small fry of %s.",
			num_punct( bounty->amount));
		do_bigshot( victim, buf);
		sprintf( buf, "&RCindy&z:&W The bounty is on %s's head!", ch->name);
		do_bigshot( victim, buf);
		sprintf( buf, "&RCindy&z:&W This fugitive has been seen lurking around %s from reports!",
			(ch->in_room->area->planet != NULL)
                        ? ch->in_room->area->planet->name :
			(checkship)? shipbuf: "space" );
		do_bigshot( victim, buf);
		sprintf( buf, "&RCindy&z:&W Good luck catching %s!",
			( ch->sex == 1)? "him" : "her");
		break;
		case 3:
		sprintf( buf, "&RCindy&z:&W This sneaky character has been seen hiding in %s!",
                        (ch->in_room->area->planet != NULL)
                        ? ch->in_room->area->planet->name : 
			(checkship)? shipbuf : "space" );
		do_bigshot( victim, buf);
		sprintf( buf, "&RCindy&z:&W %s has been reported to be armed and dangerous!",
			ch->name);
		do_bigshot( victim, buf);
		do_bigshot( victim, "&RCindy&z:&W So please be careful!");
		break;
		case 2:
		if( bounty->amount > 40000000)
		{
		sprintf( buf, "&RCindy&z:&W We got a real hot one today! %s has a large bounty of %s!",
			ch->name, num_punct(bounty->amount));
		do_bigshot( victim, buf);
		sprintf( buf, "&RCindy&z:&W This person is shifty! Last reports claim %s was on %s!",
			( ch->sex == 1)? "he" : "she",
                        (ch->in_room->area->planet != NULL)
                        ? ch->in_room->area->planet->name : 
                        (checkship)? shipbuf :"space" );
		do_bigshot( victim, buf);
		do_bigshot( victim, "&RCindy&z:&W Good luck on this one guys, it seems difficult!");
		}
		else
		{
		sprintf( buf, "&RCindy&z:&W Today has been sort of a slow day, however %s has made it worth getting up this morning!",
			ch->name);
		do_bigshot( victim, buf);
		sprintf( buf, "&RCindy&z:&W This guy has been avoiding Guardians and Hunters grasp for awhile! %s was last spotted on %s, however.",
			(ch->sex == 1)? "He" : "She",
                        (ch->in_room->area->planet != NULL)
                        ? ch->in_room->area->planet->name : 
                        (checkship)? shipbuf : "space" );
		do_bigshot( victim, buf);
		do_bigshot( victim, "&RCindy&z:&W Good luck all you Hunters and Guardians!");
		}
		break;
		case 1:
		sprintf( buf, "&RCindy&z:&W %s is on the loose!",
			ch->name);
		do_bigshot( victim, buf);
		sprintf( buf, "&RCindy&z:&W Our sources say %s has been seen around %s.",
			(ch->sex == 1)? "he" : "she",
                        (ch->in_room->area->planet != NULL)
                        ? ch->in_room->area->planet->name : 
                        (checkship)? shipbuf : "space" );
		do_bigshot( victim, buf);
		break;
		default:
		sprintf( buf, "&RCindy&z:&W The bounty of the day is %s!", ch->name);
		do_bigshot( victim, buf); 
		sprintf( buf, "&RCindy&z:&W This cunning %s can be seen hanging around %s!",
			( ch->sex == 1)
			? "mick" : "shelia",
			( ch->in_room->area->planet != NULL)
			? ch->in_room->area->planet->name : 
                        (checkship)? shipbuf : "space" );
		do_bigshot( victim, buf); 
		break;
	}
	switch( number_range(0,5))
	{
		case 5:
			do_bigshot( victim, "&RCindy&z:&W Good bye and so long!");
		break;
		case 4:
			do_bigshot( victim, "&RCindy&z:&W That is all for today!");
		break;
		case 3:
			do_bigshot( victim, "&RCindy&z:&W Till next time!");
		break;
		case 2:
			do_bigshot( victim, "&RCindy&z:&W Maybe i can find a man at the bar.... oh yea, sorry bye!");
		break;
		case 1:
			do_bigshot( victim, "&RCindy&z:&W See you guys later!");
		break;
		default:
			do_bigshot( victim, "&RCindy&z:&W Time for me to bail out!");
		break;
	}
	do_bigshot( victim, "&rThe sound of a phone hanging up is all you can hear as the report ends.");	
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
		ch_printf(ch,"&RThe Prison forces you to 'donate' %d dollars to pay for expenses.\n\r", amount);
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
	



	

