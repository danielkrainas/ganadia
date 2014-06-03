/*
 ****************************************************************************
 *   Re-written by Kristen													*
 *									   										*
 *   Changed all ship weapons to Cowboy Bebop theme			   				*
 *									   										*
 *   Originally creator Antrox										   		*
 ****************************************************************************
*/

/*
Weapon List

00 - Dual Machineguns
01 - 105mm Rifle
02 - 30mm Cannon
03 - Grappling Claw
04 - Machine Gun
05 - Cluster Missile Launcher
06 - Multiple Munitions Launcher
07 - Plasma Cannon
08 - Gatling Gun
09 - Super Plasma Cannon
10 - None

*/

#include <math.h>
#include <sys/types.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "mud.h"

bool    autofly(SHIP_DATA *ship);

void do_dual_machine_guns(CHAR_DATA *ch, char *argument )
{
    int chance;
    SHIP_DATA *ship;
    SHIP_DATA *target;
    char buf[MAX_STRING_LENGTH];

    if (  (ship = ship_from_turret(ch->in_room->vnum))  == NULL )
    {
    	send_to_char("&RYou must be in the cockpit of a ship to do that!\n\r",ch);
        return;
    }

    if (ship->shipstate == SHIP_HYPERSPACE)
    {
        send_to_char("&RYou can only do that in realspace!\n\r",ch);
        return;
    }

    if (ship->starsystem == NULL)
    {
        send_to_char("&RYou can't do that until after you've finished launching!\n\r",ch);
        return;
    }

    if ( ship->ammo < 4 )
    {
        send_to_char("&ROh No! Not Enough Ammo!\n\r",ch);
        return;
    }

    if ( autofly(ship) )
	{
    	send_to_char("&RYou'll have to turn off the ships autopilot first.\n\r",ch);
    	return;
    }

    chance = IS_NPC(ch) ? ch->top_level
             : (int) ( ch->perm_dex*2 + ch->pcdata->learned[gsn_spacecombat]/3
             + ch->pcdata->learned[gsn_spacecombat2]/3 + ch->pcdata->learned[gsn_spacecombat3]/3 );

  	if ( ch->in_room->vnum == ship->gunseat && !str_prefix( argument , "lasers") )
   	{
    	if (ship->statet0 == LASER_DAMAGED)
        {
        	send_to_char("&RThe ships weapons are damaged.\n\r",ch);
    	   	return;
    	}
    	if (ship->target0 == NULL )
    	{
    	  	send_to_char("&RYou need to choose a target first.\n\r",ch);
    	   	return;
    	}
    	target = ship->target0;
    	if (ship->target0->starsystem != ship->starsystem)
    	{
    	   	send_to_char("&RYour target seems to have left.\n\r",ch);
    	    ship->target0 = NULL;
    	  	return;
    	}
        if ( abs(target->vx - ship->vx) > 500 ||
             abs(target->vy - ship->vy) > 500 ||
             abs(target->vz - ship->vz) > 500 )
        {
        	send_to_char("&RThat ship is out of range.\n\r",ch);
    	  	return;
        }
        ship->statet0++;
        chance += target->class*25;
        chance -= target->manuever/10;
        chance -= target->currspeed/20;
        chance -= ( abs(target->vx - ship->vx)/70 );
        chance -= ( abs(target->vy - ship->vy)/70 );
        chance -= ( abs(target->vz - ship->vz)/70 );
        chance = URANGE( 10 , chance , 90 );
        act( AT_PLAIN, "$n presses the fire button.", ch, NULL, argument , TO_ROOM );
        if ( number_percent( ) > chance )
        {
          	sprintf( buf , "Dual machine guns fire from %s and miss you." , ship->name);
            echo_to_cockpit( AT_ORANGE , target , buf );
            sprintf( buf , "You shoot dual machine guns at %s but miss." , target->name);
            echo_to_cockpit( AT_ORANGE , ship , buf );
            learn_from_failure( ch, gsn_spacecombat );
    	    learn_from_failure( ch, gsn_spacecombat2 );
    	    learn_from_failure( ch, gsn_spacecombat3 );
    	    sprintf( buf, "Dual machine guns fire from %s and barely miss %s." , ship->name , target->name );
            echo_to_system( AT_ORANGE , ship , buf , target );
	    WAIT_STATE( ch , 8 );  
  	    return;
        }
        sprintf( buf, "Dual machine guns fire from %s and hit %s." , ship->name, target->name );
        echo_to_system( AT_ORANGE , ship , buf , target );
        sprintf( buf , "You are hit by dual machine guns fired from %s!" , ship->name);
        echo_to_cockpit( AT_BLOOD , target , buf );
        sprintf( buf , "Your dual machine guns fire and hit %s!." , target->name);
        echo_to_cockpit( AT_YELLOW , ship , buf );
        learn_from_success( ch, gsn_spacecombat );
        learn_from_success( ch, gsn_spacecombat2 );
        learn_from_success( ch, gsn_spacecombat3 );
        echo_to_ship( AT_RED , target , "A small explosion vibrates through the ship." );
        damage_ship_ch( target , 20 , 30 , ch );
    	WAIT_STATE( ch , 8 );
	// Adrenaline Rush! Gatz

	if(ch->perm_frc < 5)
		ch->perm_frc+=1;

	
        if ( autofly(target) && target->target0 != ship )
        {
           	target->target0 = ship;
            sprintf( buf , "You are being targetted by %s." , target->name);
            echo_to_cockpit( AT_BLOOD , ship , buf );
        }
   		ship->ammo -= 4;
        return;
	}
}

void do_105mm_rifle(CHAR_DATA *ch, char *argument )
{
    int chance;
    SHIP_DATA *ship;
    SHIP_DATA *target;
    char buf[MAX_STRING_LENGTH];

    if (  (ship = ship_from_turret(ch->in_room->vnum))  == NULL )
    {
    	send_to_char("&RYou must be in the cockpit of a ship to do that!\n\r",ch);
        return;
    }

	if (ship->shipstate == SHIP_HYPERSPACE)
	{
		send_to_char("&RYou can only do that in realspace!\n\r",ch);
		return;
	}

	if (ship->starsystem == NULL)
	{
		send_to_char("&RYou can't do that until after you've finished launching!\n\r",ch);
		return;
	}

    if ( ship->ammo < 8 )
	{
		send_to_char("&ROh No! Not Enough Ammo!\n\r",ch);
		return;
	}

	if ( autofly(ship) )
	{
		send_to_char("&RYou'll have to turn off the ships autopilot first.\n\r",ch);
		return;
	}

    chance = IS_NPC(ch) ? ch->top_level
           	 : (int) ( ch->perm_dex*2 + ch->pcdata->learned[gsn_spacecombat]/3
             + ch->pcdata->learned[gsn_spacecombat2]/3 + ch->pcdata->learned[gsn_spacecombat3]/3 );

	if ( ch->in_room->vnum == ship->gunseat && !str_prefix( argument , "lasers") )
	{
		if (ship->statet0 == LASER_DAMAGED)
		{
			send_to_char("&RThe ships weapons are damaged.\n\r",ch);
			return;
		}
		if (ship->target0 == NULL )
		{
			send_to_char("&RYou need to choose a target first.\n\r",ch);
			return;
		}
		target = ship->target0;
		if (ship->target0->starsystem != ship->starsystem)
		{
			send_to_char("&RYour target seems to have left.\n\r",ch);
			ship->target0 = NULL;
			return;
		}
		if ( abs(target->vx - ship->vx) > 1000 ||
			 abs(target->vy - ship->vy) > 1000 ||
		     abs(target->vz - ship->vz) > 1000 )
		{
			send_to_char("&RThat ship is out of range.\n\r",ch);
			return;
		}
		ship->statet0++;
		chance += target->class*25;
		chance -= target->manuever/10;
		chance -= target->currspeed/20;
		chance -= ( abs(target->vx - ship->vx)/70 );
		chance -= ( abs(target->vy - ship->vy)/70 );
		chance -= ( abs(target->vz - ship->vz)/70 );
		chance = URANGE( 10 , chance , 90 );
		act( AT_PLAIN, "$n presses the fire button.", ch, NULL, argument , TO_ROOM );
		if ( number_percent( ) > chance )
		{
			sprintf( buf , "a 105mm Rifle fires from %s and misses you." , ship->name);
			echo_to_cockpit( AT_ORANGE , target , buf );
    	    sprintf( buf , "You shoot the 105mm Rifle at %s but miss." , target->name);
            echo_to_cockpit( AT_ORANGE , ship , buf );
            learn_from_failure( ch, gsn_spacecombat );
    	    learn_from_failure( ch, gsn_spacecombat2 );
    	    learn_from_failure( ch, gsn_spacecombat3 );
    	    sprintf( buf, "a 105mm Rifle fires from %s and barely misses %s." , ship->name , target->name );
            echo_to_system( AT_ORANGE , ship , buf , target );
    	    WAIT_STATE( ch , 16 );
	    return;
        }
        sprintf( buf, "a 105mm Rifle fires from %s and hits %s." , ship->name, target->name );
        echo_to_system( AT_ORANGE , ship , buf , target );
        sprintf( buf , "You are hit by a 105mm Rifle shot from %s!" , ship->name);
        echo_to_cockpit( AT_BLOOD , target , buf );
        sprintf( buf , "Your 105mm Rifle shot hits %s!." , target->name);
        echo_to_cockpit( AT_YELLOW , ship , buf );
        learn_from_success( ch, gsn_spacecombat );
        learn_from_success( ch, gsn_spacecombat2 );
        learn_from_success( ch, gsn_spacecombat3 );
        echo_to_ship( AT_RED , target , "A small explosion vibrates through the ship." );
        damage_ship_ch( target , 40 , 50 , ch );
    	WAIT_STATE( ch , 16 );

        if ( autofly(target) && target->target0 != ship )
        {
        	target->target0 = ship;
        	sprintf( buf , "You are being targetted by %s." , target->name);
        	echo_to_cockpit( AT_BLOOD , ship , buf );
        }
  		ship->ammo -= 8;
        return;
    }

}

void do_30mm_cannon(CHAR_DATA *ch, char *argument )
{
    int chance;
    SHIP_DATA *ship;
    SHIP_DATA *target;
    char buf[MAX_STRING_LENGTH];

	if (  (ship = ship_from_turret(ch->in_room->vnum))  == NULL )
	{
		send_to_char("&RYou must be in the cockpit of a ship to do that!\n\r",ch);
		return;
	}

	if (ship->shipstate == SHIP_HYPERSPACE)
	{
		send_to_char("&RYou can only do that in realspace!\n\r",ch);
		return;
	}

	if (ship->starsystem == NULL)
	{
		send_to_char("&RYou can't do that until after you've finished launching!\n\r",ch);
		return;
	}

	if ( ship->ammo < 6 )
	{
		send_to_char("&ROh No! Not Enough Ammo!\n\r",ch);
		return;
	}

	if ( autofly(ship) )
	{
		send_to_char("&RYou'll have to turn off the ships autopilot first.\n\r",ch);
		return;
	}

    chance = IS_NPC(ch) ? ch->top_level
             : (int) ( ch->perm_dex*2 + ch->pcdata->learned[gsn_spacecombat]/3
             + ch->pcdata->learned[gsn_spacecombat2]/3 + ch->pcdata->learned[gsn_spacecombat3]/3 );

	if ( ch->in_room->vnum == ship->gunseat && !str_prefix( argument , "lasers") )
	{
    	if (ship->statet0 == LASER_DAMAGED)
  	    {
    		send_to_char("&RThe ships weapons are damaged.\n\r",ch);
          	return;
    	}
    	if (ship->target0 == NULL )
    	{
    	  	send_to_char("&RYou need to choose a target first.\n\r",ch);
    	   	return;
    	}
    	target = ship->target0;
    	if (ship->target0->starsystem != ship->starsystem)
    	{
    	  	send_to_char("&RYour target seems to have left.\n\r",ch);
    	    ship->target0 = NULL;
    	 	return;
    	}
        if ( abs(target->vx - ship->vx) > 400 ||
             abs(target->vy - ship->vy) > 400 ||
             abs(target->vz - ship->vz) > 400 )
        {
    		send_to_char("&RThat ship is out of range.\n\r",ch);
    	   	return;
        }
        ship->statet0++;
        chance += target->class*25;
        chance -= target->manuever/10;
        chance -= target->currspeed/20;
        chance -= ( abs(target->vx - ship->vx)/70 );
        chance -= ( abs(target->vy - ship->vy)/70 );
        chance -= ( abs(target->vz - ship->vz)/70 );
        chance = URANGE( 10 , chance , 90 );
        act( AT_PLAIN, "$n presses the fire button.", ch, NULL, argument , TO_ROOM );
        if ( number_percent( ) > chance )
        {
        	sprintf( buf , "a 30mm cannon fires from %s but misses you." , ship->name);
        	echo_to_cockpit( AT_ORANGE , target , buf );
        	sprintf( buf , "You shoot the 30mm cannon at %s but miss." , target->name);
        	echo_to_cockpit( AT_ORANGE , ship , buf );
        	learn_from_failure( ch, gsn_spacecombat );
    	 	learn_from_failure( ch, gsn_spacecombat2 );
    		learn_from_failure( ch, gsn_spacecombat3 );
    	  	sprintf( buf, "30mm cannon fire from %s barely misses %s." , ship->name , target->name );
          	echo_to_system( AT_ORANGE , ship , buf , target );
    		WAIT_STATE( ch , 12 );  
		return;
     	}
      	sprintf( buf, "a 30mm cannon fires from %s and hits %s." , ship->name, target->name );
     	echo_to_system( AT_ORANGE , ship , buf , target );
    	sprintf( buf , "You are hit by 30mm cannon fire from %s!" , ship->name);
      	echo_to_cockpit( AT_BLOOD , target , buf );
      	sprintf( buf , "Your 30mm cannon fire hits %s!." , target->name);
      	echo_to_cockpit( AT_YELLOW , ship , buf );
    	learn_from_success( ch, gsn_spacecombat );
    	learn_from_success( ch, gsn_spacecombat2 );
		learn_from_success( ch, gsn_spacecombat3 );
		echo_to_ship( AT_RED , target , "A small explosion vibrates through the ship." );
		damage_ship_ch( target , 30  , 40 , ch );
    	WAIT_STATE( ch , 12 );

		if ( autofly(target) && target->target0 != ship )
		{
			target->target0 = ship;
			sprintf( buf , "You are being targetted by %s." , target->name);
			echo_to_cockpit( AT_BLOOD , ship , buf );
        }
  		ship->ammo -= 6;
        return;
    }
}

void do_grappling_claw(CHAR_DATA *ch, char *argument )
{
    int chance;
    SHIP_DATA *ship;
    SHIP_DATA *target;
    char buf[MAX_STRING_LENGTH];

	if (  (ship = ship_from_turret(ch->in_room->vnum))  == NULL )
	{
		send_to_char("&RYou must be in the cockpit of a ship to do that!\n\r",ch);
		return;
	}

	if (ship->shipstate == SHIP_HYPERSPACE)
	{
		send_to_char("&RYou can only do that in realspace!\n\r",ch);
		return;
	}

	if (ship->starsystem == NULL)
	{
		send_to_char("&RYou can't do that until after you've finished launching!\n\r",ch);
		return;
	}

	if ( autofly(ship) )
	{
		send_to_char("&RYou'll have to turn off the ships autopilot first.\n\r",ch);
		return;
	}

	chance = IS_NPC(ch) ? ch->top_level
             : (int) ( ch->perm_dex*2 + ch->pcdata->learned[gsn_spacecombat]/3
             + ch->pcdata->learned[gsn_spacecombat2]/3 + ch->pcdata->learned[gsn_spacecombat3]/3 );

	if ( ch->in_room->vnum == ship->gunseat && !str_prefix( argument , "lasers") )
	{
  	    if (ship->target0 == NULL )
   	    {
   	    	send_to_char("&RYou need to choose a target first.\n\r",ch);
   	     	return;
   	    }
   	    target = ship->target0;
   	    if (ship->target0->starsystem != ship->starsystem)
   	    {
   	     	send_to_char("&RYour target seems to have left.\n\r",ch);
   	        ship->target0 = NULL;
   	    	return;
   	    }
        if ( abs(target->vx - ship->vx) > 100 ||
             abs(target->vy - ship->vy) > 100 ||
             abs(target->vz - ship->vz) > 100 )
        {
        	send_to_char("&RYou must be next to them to use the grappling claw.\n\r",ch);
  	     	return;
        }
        ship->statet0++;
        chance += target->class*25;
        chance -= target->manuever/10;
        chance -= target->currspeed/20;
        chance -= ( abs(target->vx - ship->vx)/70 );
        chance -= ( abs(target->vy - ship->vy)/70 );
        chance -= ( abs(target->vz - ship->vz)/70 );
        chance = URANGE( 10 , chance , 90 );
        act( AT_PLAIN, "$n presses the fire button.", ch, NULL, argument , TO_ROOM );
        if ( number_percent( ) > chance )
        {
                sprintf( buf , "%s swings their grappling claw at you but misses." , ship->name);
                echo_to_cockpit( AT_ORANGE , target , buf );
                sprintf( buf , "Your grappling claw misses %s." , target->name);
                echo_to_cockpit( AT_ORANGE , ship , buf );
                learn_from_failure( ch, gsn_spacecombat );
    	        learn_from_failure( ch, gsn_spacecombat2 );
    	        learn_from_failure( ch, gsn_spacecombat3 );
    	        sprintf( buf, "%s swings their grappling claw at %s but misses." , ship->name , target->name );
                echo_to_system( AT_ORANGE , ship , buf , target );
    		WAIT_STATE( ch , 30 );
	        return;
        }
        sprintf( buf, "%s hits %s with their grappling claw." , ship->name, target->name );
        echo_to_system( AT_ORANGE , ship , buf , target );
        sprintf( buf , "You are hit by %s's grappling claw!" , ship->name);
        echo_to_cockpit( AT_BLOOD , target , buf );
        sprintf( buf , "Your grappling claw slams into %s!." , target->name);
        echo_to_cockpit( AT_YELLOW , ship , buf );
        learn_from_success( ch, gsn_spacecombat );
        learn_from_success( ch, gsn_spacecombat2 );
        learn_from_success( ch, gsn_spacecombat3 );
        echo_to_ship( AT_RED , target , "A small explosion vibrates through the ship." );
        if ( ship->class == 0 )
        	damage_ship_ch( target , 75  , 75 , ch );
        else
        if ( ship->class == 1 )
           	damage_ship_ch( target , 150  , 150 , ch );
        else
          	damage_ship_ch( target , 300  , 300 , ch );
    	WAIT_STATE( ch , 30 );

        if ( autofly(target) && target->target0 != ship )
        {
            target->target0 = ship;
            sprintf( buf , "You are being targetted by %s." , target->name);
        	echo_to_cockpit( AT_BLOOD , ship , buf );
		}
		return;
	}
}

void do_machine_gun(CHAR_DATA *ch, char *argument )
{
    int chance;
    SHIP_DATA *ship;
    SHIP_DATA *target;
    char buf[MAX_STRING_LENGTH];

        if (  (ship = ship_from_turret(ch->in_room->vnum))  == NULL )
        {
            send_to_char("&RYou must be in the cockpit of a ship to do that!\n\r",ch);
            return;
        }

        if (ship->shipstate == SHIP_HYPERSPACE)
        {
             send_to_char("&RYou can only do that in realspace!\n\r",ch);
             return;
        }
    	if (ship->starsystem == NULL)
    	{
    	     send_to_char("&RYou can't do that until after you've finished launching!\n\r",ch);
    	     return;
    	}
    	if ( ship->ammo < 2 )
        {
             send_to_char("&ROh No! Not Enough Ammo!\n\r",ch);
             return;
        }

      	if ( autofly(ship) )
    	{
    	     send_to_char("&RYou'll have to turn off the ships autopilot first.\n\r",ch);
    	     return;
    	}


        chance = IS_NPC(ch) ? ch->top_level
                 : (int) ( ch->perm_dex*2 + ch->pcdata->learned[gsn_spacecombat]/3
                 + ch->pcdata->learned[gsn_spacecombat2]/3 + ch->pcdata->learned[gsn_spacecombat3]/3 );

    	if ( ch->in_room->vnum == ship->gunseat && !str_prefix( argument , "lasers") )
    	{

    	     if (ship->statet0 == LASER_DAMAGED)
    	     {
    	        send_to_char("&RThe ships weapons are damaged.\n\r",ch);
    	      	return;
    	     }
    	     if (ship->target0 == NULL )
    	     {
    	     	send_to_char("&RYou need to choose a target first.\n\r",ch);
    	     	return;
    	     }
    	     target = ship->target0;
    	     if (ship->target0->starsystem != ship->starsystem)
    	     {
    	     	send_to_char("&RYour target seems to have left.\n\r",ch);
    	        ship->target0 = NULL;
    	     	return;
    	     }
             if ( abs(target->vx - ship->vx) > 500 ||
                  abs(target->vy - ship->vy) > 500 ||
                  abs(target->vz - ship->vz) > 500 )
             {
                send_to_char("&RThat ship is out of range.\n\r",ch);
    	     	return;
             }
             ship->statet0++;
             chance += target->class*25;
             chance -= target->manuever/10;
             chance -= target->currspeed/20;
             chance -= ( abs(target->vx - ship->vx)/70 );
             chance -= ( abs(target->vy - ship->vy)/70 );
             chance -= ( abs(target->vz - ship->vz)/70 );
             chance = URANGE( 10 , chance , 90 );
             act( AT_PLAIN, "$n presses the fire button.", ch,
                  NULL, argument , TO_ROOM );
             if ( number_percent( ) > chance )
             {
                sprintf( buf , "Machine Gun fire from %s misses you." , ship->name);
                echo_to_cockpit( AT_ORANGE , target , buf );
                sprintf( buf , " miss %s." , target->name);
                echo_to_cockpit( AT_ORANGE , ship , buf );
                learn_from_failure( ch, gsn_spacecombat );
    	        learn_from_failure( ch, gsn_spacecombat2 );
    	        learn_from_failure( ch, gsn_spacecombat3 );
    	        sprintf( buf, "Machine gun fire from %s barely misses %s." , ship->name , target->name );
                echo_to_system( AT_ORANGE , ship , buf , target );
    	        WAIT_STATE( ch , 4 );
		return;
             }
             sprintf( buf, "Machine gun fire from %s hits %s." , ship->name, target->name );
             echo_to_system( AT_ORANGE , ship , buf , target );
             sprintf( buf , "You are hit by machine gun fire from %s!" , ship->name);
             echo_to_cockpit( AT_BLOOD , target , buf );
             sprintf( buf , "Your machine gun fire hits %s!." , target->name);
             echo_to_cockpit( AT_YELLOW , ship , buf );
             learn_from_success( ch, gsn_spacecombat );
             learn_from_success( ch, gsn_spacecombat2 );
             learn_from_success( ch, gsn_spacecombat3 );
             echo_to_ship( AT_RED , target , "A small explosion vibrates through the ship." );
             damage_ship_ch( target , 10  , 20 , ch );
    		 WAIT_STATE( ch , 4 );

             if ( autofly(target) && target->target0 != ship )
             {
                target->target0 = ship;
                sprintf( buf , "You are being targetted by %s." , target->name);
                echo_to_cockpit( AT_BLOOD , ship , buf );
             }
  			 ship->ammo -= 2;
             return;
    	}

}

void do_clustermissilelauncher(CHAR_DATA *ch, char *argument )
{
    int chance;
    SHIP_DATA *ship;
    SHIP_DATA *target;
    char buf[MAX_STRING_LENGTH];

        if (  (ship = ship_from_turret(ch->in_room->vnum))  == NULL )
        {
            send_to_char("&RYou must be in the cockpit of a ship to do that!\n\r",ch);
            return;
        }
        if (ship->shipstate == SHIP_HYPERSPACE)
        {
             send_to_char("&RYou can only do that in realspace!\n\r",ch);
             return;
        }
    	if (ship->starsystem == NULL)
    	{
    	     send_to_char("&RYou can't do that until after you've finished launching!\n\r",ch);
    	     return;
    	}
    	if ( ship->ammo < 20 )
        {
             send_to_char("&ROh No! Not Enough Ammo!\n\r",ch);
             return;
        }
  		if ( autofly(ship) )
        {
             send_to_char("&RYou'll have to turn off the ships autopilot first.\n\r",ch);
             return;
        }


        chance = IS_NPC(ch) ? ch->top_level
                 : (int) ( ch->perm_dex*2 + ch->pcdata->learned[gsn_spacecombat]/3
                           + ch->pcdata->learned[gsn_spacecombat2]/3 + ch->pcdata->learned[gsn_spacecombat3]/3 );

    	if ( ch->in_room->vnum == ship->gunseat && !str_prefix( argument , "lasers") )
    	{

    	     if (ship->statet0 == LASER_DAMAGED)
    	     {
    	        send_to_char("&RThe ships weapons are damaged.\n\r",ch);
    	      	return;
    	     }
    	     if (ship->target0 == NULL )
    	     {
    	     	send_to_char("&RYou need to choose a target first.\n\r",ch);
    	     	return;
    	     }
    	     target = ship->target0;
    	     if (ship->target0->starsystem != ship->starsystem)
    	     {
    	     	send_to_char("&RYour target seems to have left.\n\r",ch);
    	        ship->target0 = NULL;
    	     	return;
    	     }
             if ( abs(target->vx - ship->vx) > 600 ||
                  abs(target->vy - ship->vy) > 600 ||
                  abs(target->vz - ship->vz) > 600 )
             {
                send_to_char("&RThat ship is out of range.\n\r",ch);
    	     	return;
             }
             ship->statet0++;
             chance += target->class*25;
             chance -= target->manuever/10;
             chance -= target->currspeed/20;
             chance -= ( abs(target->vx - ship->vx)/70 );
             chance -= ( abs(target->vy - ship->vy)/70 );
             chance -= ( abs(target->vz - ship->vz)/70 );
             chance = URANGE( 10 , chance , 90 );
             act( AT_PLAIN, "$n presses the fire button.", ch,
                  NULL, argument , TO_ROOM );
             if ( number_percent( ) > chance )
             {
                sprintf( buf , " %s launches missiles at you but misses." , ship->name);
                echo_to_cockpit( AT_ORANGE , target , buf );
                sprintf( buf , "You launch missiles at %s but miss." , target->name);
                echo_to_cockpit( AT_ORANGE , ship , buf );
                learn_from_failure( ch, gsn_spacecombat );
    	        learn_from_failure( ch, gsn_spacecombat2 );
    	        learn_from_failure( ch, gsn_spacecombat3 );
    	        sprintf( buf, "Missiles launch from %s barely missing %s." , ship->name , target->name );
                echo_to_system( AT_ORANGE , ship , buf , target );
    	        WAIT_STATE( ch , 24 );
	        return;
             }
             sprintf( buf, "Missiles launch from %s and hit %s." , ship->name, target->name );
             echo_to_system( AT_ORANGE , ship , buf , target );
             sprintf( buf , "You are hit by missiles launched from %s!" , ship->name);
             echo_to_cockpit( AT_BLOOD , target , buf );
             sprintf( buf , "You launch missiles and they hit %s!." , target->name);
             echo_to_cockpit( AT_YELLOW , ship , buf );
             learn_from_success( ch, gsn_spacecombat );
             learn_from_success( ch, gsn_spacecombat2 );
             learn_from_success( ch, gsn_spacecombat3 );
             echo_to_ship( AT_RED , target , "A small explosion vibrates through the ship." );
             damage_ship_ch( target , 60 , 75 , ch );
     		 WAIT_STATE( ch , 24 );

             if ( autofly(target) && target->target0 != ship )
             {
                target->target0 = ship;
                sprintf( buf , "You are being targetted by %s." , target->name);
                echo_to_cockpit( AT_BLOOD , ship , buf );
             }
   		     ship->ammo -= 20;
             return;
    	}

}

void do_multiplemunitionslauncher(CHAR_DATA *ch, char *argument )
{
    int chance;
    SHIP_DATA *ship;
    SHIP_DATA *target;
    char buf[MAX_STRING_LENGTH];

        if (  (ship = ship_from_turret(ch->in_room->vnum))  == NULL )
        {
            send_to_char("&RYou must be in the cockpit of a ship to do that!\n\r",ch);
            return;
        }
        if (ship->shipstate == SHIP_HYPERSPACE)
        {
             send_to_char("&RYou can only do that in realspace!\n\r",ch);
             return;
        }
    	if (ship->starsystem == NULL)
    	{
    	     send_to_char("&RYou can't do that until after you've finished launching!\n\r",ch);
    	     return;
    	}
    	if ( ship->ammo < 40 )
        {
             send_to_char("&ROh No! Not Enough Ammo!\n\r",ch);
             return;
        }

        if ( autofly(ship) )
    	{
    		send_to_char("&RYou'll have to turn off the ships autopilot first.\n\r",ch);
    		return;
    	}


        chance = IS_NPC(ch) ? ch->top_level
                 : (int) ( ch->perm_dex*2 + ch->pcdata->learned[gsn_spacecombat]/3
                           + ch->pcdata->learned[gsn_spacecombat2]/3 + ch->pcdata->learned[gsn_spacecombat3]/3 );

    	if ( ch->in_room->vnum == ship->gunseat && !str_prefix( argument , "lasers") )
    	{

    	     if (ship->statet0 == LASER_DAMAGED)
    	     {
    	        send_to_char("&RThe ships weapons are damaged.\n\r",ch);
    	      	return;
    	     }
    	     if (ship->target0 == NULL )
    	     {
    	     	send_to_char("&RYou need to choose a target first.\n\r",ch);
    	     	return;
    	     }
    	     target = ship->target0;
    	     if (ship->target0->starsystem != ship->starsystem)
    	     {
    	     	send_to_char("&RYour target seems to have left.\n\r",ch);
    	        ship->target0 = NULL;
    	     	return;
    	     }
             if ( abs(target->vx - ship->vx) > 400 ||
                  abs(target->vy - ship->vy) > 400 ||
                  abs(target->vz - ship->vz) > 400 )
             {
                send_to_char("&RThat ship is out of range.\n\r",ch);
    	     	return;
             }
             ship->statet0++;
             chance += target->class*25;
             chance -= target->manuever/10;
             chance -= target->currspeed/20;
             chance -= ( abs(target->vx - ship->vx)/70 );
             chance -= ( abs(target->vy - ship->vy)/70 );
             chance -= ( abs(target->vz - ship->vz)/70 );
             chance = URANGE( 10 , chance , 90 );
             act( AT_PLAIN, "$n presses the fire button.", ch,
                  NULL, argument , TO_ROOM );
             if ( number_percent( ) > chance )
             {
                sprintf( buf , "Grenades and missiles from %s miss you." , ship->name);
                echo_to_cockpit( AT_ORANGE , target , buf );
                sprintf( buf , "You shoot the multiple munition launcher at %s but miss." , target->name);
                echo_to_cockpit( AT_ORANGE , ship , buf );
                learn_from_failure( ch, gsn_spacecombat );
    	        learn_from_failure( ch, gsn_spacecombat2 );
    	        learn_from_failure( ch, gsn_spacecombat3 );
    	        sprintf( buf, "Grenades and missiles fire from %s barely missing %s." , ship->name , target->name );
                echo_to_system( AT_ORANGE , ship , buf , target );
    	        WAIT_STATE( ch , 30 );
	        return;
             }
             sprintf( buf, "Grenades and missiles fire from %s hitting %s." , ship->name, target->name );
             echo_to_system( AT_ORANGE , ship , buf , target );
             sprintf( buf , "You are hit by grenades and missiles fired from %s!" , ship->name);
             echo_to_cockpit( AT_BLOOD , target , buf );
             sprintf( buf , "Your multiple munition fire hits %s!." , target->name);
             echo_to_cockpit( AT_YELLOW , ship , buf );
             learn_from_success( ch, gsn_spacecombat );
             learn_from_success( ch, gsn_spacecombat2 );
             learn_from_success( ch, gsn_spacecombat3 );
             echo_to_ship( AT_RED , target , "A small explosion vibrates through the ship." );
             damage_ship_ch( target , 75 , 100 , ch );
    		 WAIT_STATE( ch , 30 );

             if ( autofly(target) && target->target0 != ship )
             {
                target->target0 = ship;
                sprintf( buf , "You are being targetted by %s." , target->name);
                echo_to_cockpit( AT_BLOOD , ship , buf );
             }
  			 ship->ammo -= 40;
             return;
    	}

}

void do_plasma_cannon(CHAR_DATA *ch, char *argument )
{
    int chance;
    SHIP_DATA *ship;
    SHIP_DATA *target;
    char buf[MAX_STRING_LENGTH];

        if (  (ship = ship_from_turret(ch->in_room->vnum))  == NULL )
        {
            send_to_char("&RYou must be in the cockpit of a ship to do that!\n\r",ch);
            return;
        }
        if (ship->shipstate == SHIP_HYPERSPACE)
        {
             send_to_char("&RYou can only do that in realspace!\n\r",ch);
             return;
        }
    	if (ship->starsystem == NULL)
    	{
    	     send_to_char("&RYou can't do that until after you've finished launching!\n\r",ch);
    	     return;
    	}
    	if ( ship->ammo < 80 )
        {
             send_to_char("&ROh No! Not Enough Ammo!\n\r",ch);
             return;
        }

        if ( autofly(ship) )
    	{
    	     send_to_char("&RYou'll have to turn off the ships autopilot first.\n\r",ch);
    	     return;
    	}


        chance = IS_NPC(ch) ? ch->top_level
                 : (int) ( ch->perm_dex*2 + ch->pcdata->learned[gsn_spacecombat]/3
                           + ch->pcdata->learned[gsn_spacecombat2]/3 + ch->pcdata->learned[gsn_spacecombat3]/3 );

    	if ( ch->in_room->vnum == ship->gunseat && !str_prefix( argument , "lasers") )
    	{

    	     if (ship->statet0 == LASER_DAMAGED)
    	     {
    	        send_to_char("&RThe ships weapons are damaged.\n\r",ch);
    	      	return;
    	     }
    	     if (ship->target0 == NULL )
    	     {
    	     	send_to_char("&RYou need to choose a target first.\n\r",ch);
    	     	return;
    	     }
    	     target = ship->target0;
    	     if (ship->target0->starsystem != ship->starsystem)
    	     {
    	     	send_to_char("&RYour target seems to have left.\n\r",ch);
    	        ship->target0 = NULL;
    	     	return;
    	     }
             if ( abs(target->vx - ship->vx) > 750 ||
                  abs(target->vy - ship->vy) > 750 ||
                  abs(target->vz - ship->vz) > 750 )
             {
                send_to_char("&RThat ship is out of range.\n\r",ch);
    	     	return;
             }
             ship->statet0++;
             chance += target->class*25;
             chance -= target->manuever/10;
             chance -= target->currspeed/20;
             chance -= ( abs(target->vx - ship->vx)/70 );
             chance -= ( abs(target->vy - ship->vy)/70 );
             chance -= ( abs(target->vz - ship->vz)/70 );
             chance = URANGE( 10 , chance , 90 );
             act( AT_PLAIN, "$n presses the fire button.", ch,
                  NULL, argument , TO_ROOM );
             if ( number_percent( ) > chance )
             {
                sprintf( buf , "The plasma cannon fires from %s missing you." , ship->name);
                echo_to_cockpit( AT_ORANGE , target , buf );
                sprintf( buf , "You charge up the plasma cannon and fire at %s but miss." , target->name);
                echo_to_cockpit( AT_ORANGE , ship , buf );
                learn_from_failure( ch, gsn_spacecombat );
    	        learn_from_failure( ch, gsn_spacecombat2 );
    	        learn_from_failure( ch, gsn_spacecombat3 );
    	        sprintf( buf, "The plasma cannon fired from %s barely misses %s." , ship->name , target->name );
                echo_to_system( AT_ORANGE , ship , buf , target );
    	        WAIT_STATE( ch , 42 );
		return;
             }
             sprintf( buf, "The plasma cannon fired from %s hits %s." , ship->name, target->name );
             echo_to_system( AT_ORANGE , ship , buf , target );
             sprintf( buf , "You are hit by plasma cannon fire from %s!" , ship->name);
             echo_to_cockpit( AT_BLOOD , target , buf );
             sprintf( buf , "You charge up the plasma cannon and fire, hitting %s!." , target->name);
             echo_to_cockpit( AT_YELLOW , ship , buf );
             learn_from_success( ch, gsn_spacecombat );
             learn_from_success( ch, gsn_spacecombat2 );
             learn_from_success( ch, gsn_spacecombat3 );
             echo_to_ship( AT_RED , target , "A small explosion vibrates through the ship." );
             damage_ship_ch( target , 100 , 150 , ch );
    		 WAIT_STATE( ch , 42 );
             if ( autofly(target) && target->target0 != ship )
             {
                target->target0 = ship;
                sprintf( buf , "You are being targetted by %s." , target->name);
                echo_to_cockpit( AT_BLOOD , ship , buf );
             }
  			 ship->ammo -= 80;
             return;
    	}

}

void do_gatling_gun(CHAR_DATA *ch, char *argument )
{
    int chance;
    SHIP_DATA *ship;
    SHIP_DATA *target;
    char buf[MAX_STRING_LENGTH];

        if (  (ship = ship_from_turret(ch->in_room->vnum))  == NULL )
        {
            send_to_char("&RYou must be in the cockpit of a ship to do that!\n\r",ch);
            return;
        }
        if (ship->shipstate == SHIP_HYPERSPACE)
        {
             send_to_char("&RYou can only do that in realspace!\n\r",ch);
             return;
        }
    	if (ship->starsystem == NULL)
    	{
    	     send_to_char("&RYou can't do that until after you've finished launching!\n\r",ch);
    	     return;
    	}
    	if ( ship->ammo < 10 )
        {
             send_to_char("&ROh No! Not Enough Ammo!\n\r",ch);
             return;
        }

        if ( autofly(ship) )
    	{
    	 	send_to_char("&RYou'll have to turn off the ships autopilot first.\n\r",ch);
    	    return;
		}


        chance = IS_NPC(ch) ? ch->top_level
                 : (int) ( ch->perm_dex*2 + ch->pcdata->learned[gsn_spacecombat]/3
                           + ch->pcdata->learned[gsn_spacecombat2]/3 + ch->pcdata->learned[gsn_spacecombat3]/3 );

    	if ( ch->in_room->vnum == ship->gunseat && !str_prefix( argument , "lasers") )
    	{

    	     if (ship->statet0 == LASER_DAMAGED)
    	     {
    	        send_to_char("&RThe ships weapons are damaged.\n\r",ch);
    	      	return;
    	     }
    	     if (ship->target0 == NULL )
    	     {
    	     	send_to_char("&RYou need to choose a target first.\n\r",ch);
    	     	return;
    	     }
    	     target = ship->target0;
    	     if (ship->target0->starsystem != ship->starsystem)
    	     {
    	     	send_to_char("&RYour target seems to have left.\n\r",ch);
    	        ship->target0 = NULL;
    	     	return;
    	     }
             if ( abs(target->vx - ship->vx) >500 ||
                  abs(target->vy - ship->vy) >500 ||
                  abs(target->vz - ship->vz) >500 )
             {
                send_to_char("&RThat ship is out of range.\n\r",ch);
    	     	return;
             }
             ship->statet0++;
             chance += target->class*25;
             chance -= target->manuever/10;
             chance -= target->currspeed/20;
             chance -= ( abs(target->vx - ship->vx)/70 );
             chance -= ( abs(target->vy - ship->vy)/70 );
             chance -= ( abs(target->vz - ship->vz)/70 );
             chance = URANGE( 10 , chance , 90 );
             act( AT_PLAIN, "$n presses the fire button.", ch,
                  NULL, argument , TO_ROOM );
             if ( number_percent( ) > chance )
             {
                sprintf( buf , "Gatling gun fire from %s misses you." , ship->name);
                echo_to_cockpit( AT_ORANGE , target , buf );
                sprintf( buf , "You shoot the gatling gun at %s but miss." , target->name);
                echo_to_cockpit( AT_ORANGE , ship , buf );
                learn_from_failure( ch, gsn_spacecombat );
    	        learn_from_failure( ch, gsn_spacecombat2 );
    	        learn_from_failure( ch, gsn_spacecombat3 );
    	        sprintf( buf, "Gatling gun fire from %s barely misses %s." , ship->name , target->name );
                echo_to_system( AT_ORANGE , ship , buf , target );
    	        WAIT_STATE( ch , 16 );
		return;
             }
             sprintf( buf, "Gatling gun fire from %s hits %s." , ship->name, target->name );
             echo_to_system( AT_ORANGE , ship , buf , target );
             sprintf( buf , "You are hit by %s's gatling gun fire!" , ship->name);
             echo_to_cockpit( AT_BLOOD , target , buf );
             sprintf( buf , "You shoot the gatling gun at %s and hit!" , target->name);
             echo_to_cockpit( AT_YELLOW , ship , buf );
             learn_from_success( ch, gsn_spacecombat );
             learn_from_success( ch, gsn_spacecombat2 );
             learn_from_success( ch, gsn_spacecombat3 );
             echo_to_ship( AT_RED , target , "A small explosion vibrates through the ship." );
             damage_ship_ch( target , 50 , 60 , ch );
    		 WAIT_STATE( ch , 16 );

             if ( autofly(target) && target->target0 != ship )
             {
                target->target0 = ship;
                sprintf( buf , "You are being targetted by %s." , target->name);
                echo_to_cockpit( AT_BLOOD , ship , buf );
             }
  			 ship->ammo -= 10;
             return;
    	}

}

void do_super_plasma_cannon(CHAR_DATA *ch, char *argument )
{
    int chance;
    SHIP_DATA *ship;
    SHIP_DATA *target;
    char buf[MAX_STRING_LENGTH];

        if (  (ship = ship_from_turret(ch->in_room->vnum))  == NULL )
        {
            send_to_char("&RYou must be in the cockpit of a ship to do that!\n\r",ch);
            return;
        }

        if (ship->shipstate == SHIP_HYPERSPACE)
        {
             send_to_char("&RYou can only do that in realspace!\n\r",ch);
             return;
        }

    	if (ship->starsystem == NULL)
    	{
    	     send_to_char("&RYou can't do that until after you've finished launching!\n\r",ch);
    	     return;
    	}

    	if ( ship->ammo < 200 )
        {
             send_to_char("&ROh No! Not Enough Ammo!\n\r",ch);
             return;
        }

                if ( autofly(ship) )
    	        {
    	            send_to_char("&RYou'll have to turn off the ships autopilot first.\n\r",ch);
    	            return;
    	        }


        chance = IS_NPC(ch) ? ch->top_level
                 : (int) ( ch->perm_dex*2 + ch->pcdata->learned[gsn_spacecombat]/3
                           + ch->pcdata->learned[gsn_spacecombat2]/3 + ch->pcdata->learned[gsn_spacecombat3]/3 );

    	if ( ch->in_room->vnum == ship->gunseat && !str_prefix( argument , "lasers") )
    	{

    	     if (ship->statet0 == LASER_DAMAGED)
    	     {
    	        send_to_char("&RThe ships weapons are damaged.\n\r",ch);
    	      	return;
    	     }
    	     if (ship->target0 == NULL )
    	     {
    	     	send_to_char("&RYou need to choose a target first.\n\r",ch);
    	     	return;
    	     }
    	     target = ship->target0;
    	     if (ship->target0->starsystem != ship->starsystem)
    	     {
    	     	send_to_char("&RYour target seems to have left.\n\r",ch);
    	        ship->target0 = NULL;
    	     	return;
    	     }
             if ( abs(target->vx - ship->vx) > 750 ||
                  abs(target->vy - ship->vy) > 750 ||
                  abs(target->vz - ship->vz) > 750 )
             {
                send_to_char("&RThat ship is out of range.\n\r",ch);
    	     	return;
             }
             ship->statet0++;
             chance += target->class*25;
             chance -= target->manuever/10;
             chance -= target->currspeed/20;
             chance -= ( abs(target->vx - ship->vx)/70 );
             chance -= ( abs(target->vy - ship->vy)/70 );
             chance -= ( abs(target->vz - ship->vz)/70 );
             chance = URANGE( 10 , chance , 90 );
             act( AT_PLAIN, "$n presses the fire button.", ch,
                  NULL, argument , TO_ROOM );
             if ( number_percent( ) > chance )
             {
                sprintf( buf , "%s charges up their plasma cannon. They fire it at you but miss." , ship->name);
                echo_to_cockpit( AT_ORANGE , target , buf );
                sprintf( buf , "You charge up the plasma cannon. You fire it at %s but miss." , target->name);
                echo_to_cockpit( AT_ORANGE , ship , buf );
                learn_from_failure( ch, gsn_spacecombat );
    	        learn_from_failure( ch, gsn_spacecombat2 );
    	        learn_from_failure( ch, gsn_spacecombat3 );
    	        sprintf( buf, "The plasma cannon shot from %s barely misses %s." , ship->name , target->name );
                echo_to_system( AT_ORANGE , ship , buf , target );
    	        WAIT_STATE( ch , 64 );
		return;
             }
             sprintf( buf, "%s charges up their plasma cannon. They fire it at %s and hit!" , ship->name, target->name );
             echo_to_system( AT_ORANGE , ship , buf , target );
             sprintf( buf , "%s charges up their plasma cannon. They fire it at you and hit!" , ship->name);
             echo_to_cockpit( AT_BLOOD , target , buf );
             sprintf( buf , "You charge up the plasma cannon. You fire it at %s and hit!" , target->name);
             echo_to_cockpit( AT_YELLOW , ship , buf );
             learn_from_success( ch, gsn_spacecombat );
             learn_from_success( ch, gsn_spacecombat2 );
             learn_from_success( ch, gsn_spacecombat3 );
             echo_to_ship( AT_RED , target , "A small explosion vibrates through the ship." );
             damage_ship_ch( target , 150 , 250 , ch );
    		 WAIT_STATE( ch , 64 );

             if ( autofly(target) && target->target0 != ship )
             {
                target->target0 = ship;
                sprintf( buf , "You are being targetted by %s." , target->name);
                echo_to_cockpit( AT_BLOOD , ship , buf );
             }
   		     ship->ammo -= 200;
             return;
    	}

}

/* Weapon System */
void do_firstweapon(CHAR_DATA *ch, char *argument )
{
    SHIP_DATA *ship;

if (  (ship = ship_from_turret(ch->in_room->vnum))  == NULL )
{
   send_to_char("&RYou must be in the cockpit of a ship to do that!\n\r",ch);
   return;
}

if ( ship->firstweapon == 0 )
{
   do_dual_machine_guns( ch, "" );
}
if ( ship->firstweapon == 1 )
{
   do_105mm_rifle( ch, "" );
}
if ( ship->firstweapon == 2 )
{
   do_30mm_cannon( ch, "" );
}
if ( ship->firstweapon == 3 )
{
   do_grappling_claw( ch, "" );
}
if ( ship->firstweapon == 4 )
{
   do_machine_gun( ch, "" );
}
if ( ship->firstweapon == 5 )
{
   do_clustermissilelauncher( ch, "" );
}
if ( ship->firstweapon == 6 )
{
   do_multiplemunitionslauncher( ch, "" );
}
if ( ship->firstweapon == 7 )
{
   do_plasma_cannon( ch, "" );
}
if ( ship->firstweapon == 8 )
{
   do_gatling_gun( ch, "" );
}
if ( ship->firstweapon == 9 )
{
   do_super_plasma_cannon( ch, "" );
}
if ( ship->firstweapon == 10 )
{
send_to_char( "No weapon equipped!\n\r", ch );
}
return;
}

void do_secondweapon(CHAR_DATA *ch, char *argument )
{
    SHIP_DATA *ship;

if (  (ship = ship_from_turret(ch->in_room->vnum))  == NULL )
{
   send_to_char("&RYou must be in the cockpit of a ship to do that!\n\r",ch);
   return;
}

if ( ship->secondweapon == 0 )
{
   do_dual_machine_guns( ch, "" );
}
if ( ship->secondweapon == 1 )
{
   do_105mm_rifle( ch, "" );
}
if ( ship->secondweapon == 2 )
{
   do_30mm_cannon( ch, "" );
}
if ( ship->secondweapon == 3 )
{
   do_grappling_claw( ch, "" );
}
if ( ship->secondweapon == 4 )
{
   do_machine_gun( ch, "" );
}
if ( ship->secondweapon == 5 )
{
   do_clustermissilelauncher( ch, "" );
}
if ( ship->secondweapon == 6 )
{
   do_multiplemunitionslauncher( ch, "" );
}
if ( ship->secondweapon == 7 )
{
   do_plasma_cannon( ch, "" );
}
if ( ship->secondweapon == 8 )
{
   do_gatling_gun( ch, "" );
}
if ( ship->secondweapon == 9 )
{
   do_super_plasma_cannon( ch, "" );
}
if ( ship->secondweapon == 10 )
{
send_to_char( "No weapon equipped!\n\r", ch );
}
return;
}

void do_thirdweapon(CHAR_DATA *ch, char *argument )
{
    SHIP_DATA *ship;

if (  (ship = ship_from_turret(ch->in_room->vnum))  == NULL )
{
   send_to_char("&RYou must be in the cockpit of a ship to do that!\n\r",ch);
   return;
}

if ( ship->thirdweapon == 0 )
{
   do_dual_machine_guns( ch, "" );
}
if ( ship->thirdweapon == 1 )
{
   do_105mm_rifle( ch, "" );
}
if ( ship->thirdweapon == 2 )
{
   do_30mm_cannon( ch, "" );
}
if ( ship->thirdweapon == 3 )
{
   do_grappling_claw( ch, "" );
}
if ( ship->thirdweapon == 4 )
{
   do_machine_gun( ch, "" );
}
if ( ship->thirdweapon == 5 )
{
   do_clustermissilelauncher( ch, "" );
}
if ( ship->thirdweapon == 6 )
{
   do_multiplemunitionslauncher( ch, "" );
}
if ( ship->thirdweapon == 7 )
{
   do_plasma_cannon( ch, "" );
}
if ( ship->thirdweapon == 8 )
{
   do_gatling_gun( ch, "" );
}
if ( ship->thirdweapon == 9 )
{
   do_super_plasma_cannon( ch, "" );
}
if ( ship->thirdweapon == 10 )
{
send_to_char( "No weapon equipped!\n\r", ch );
}
return;
}

