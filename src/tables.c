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
* 			Table load/save Module				   *
****************************************************************************/

#include <time.h>
#include <stdio.h>
#include <string.h>
#include "mud.h"

#if defined(KEY)
#undef KEY
#endif

#define KEY( literal, field, value )					\
				if ( !str_cmp( word, literal ) )	\
				{					\
				    field  = value;			\
				    fMatch = TRUE;			\
				    break;				\
				}

/* global variables */
int top_sn;
int top_herb;

SKILLTYPE *skill_table[MAX_SKILL];
SKILLTYPE *herb_table[MAX_HERB];

char *const skill_tname[] =
    { "unknown", "Spell", "Skill", "Weapon", "Tongue", "Herb" };

SPELL_FUN *spell_function(char *name)
{
	if (!str_cmp(name, "spell_smaug"))
		return spell_smaug;
	if (!str_cmp(name, "spell_acid_blast"))
		return spell_acid_blast;
	if (!str_cmp(name, "spell_animate_dead"))
		return spell_animate_dead;
	if (!str_cmp(name, "spell_astral_walk"))
		return spell_astral_walk;
	if (!str_cmp(name, "spell_blindness"))
		return spell_blindness;
	if (!str_cmp(name, "spell_burning_hands"))
		return spell_burning_hands;
	if (!str_cmp(name, "spell_call_lightning"))
		return spell_call_lightning;
	if (!str_cmp(name, "spell_cause_critical"))
		return spell_cause_critical;
	if (!str_cmp(name, "spell_cause_light"))
		return spell_cause_light;
	if (!str_cmp(name, "spell_cause_serious"))
		return spell_cause_serious;
	if (!str_cmp(name, "spell_change_sex"))
		return spell_change_sex;
	if (!str_cmp(name, "spell_charm_person"))
		return spell_charm_person;
	if (!str_cmp(name, "spell_chill_touch"))
		return spell_chill_touch;
	if (!str_cmp(name, "spell_colour_spray"))
		return spell_colour_spray;
	if (!str_cmp(name, "spell_control_weather"))
		return spell_control_weather;
	if (!str_cmp(name, "spell_create_food"))
		return spell_create_food;
	if (!str_cmp(name, "spell_create_water"))
		return spell_create_water;
	if (!str_cmp(name, "spell_cure_addiction"))
		return spell_cure_addiction;
	if (!str_cmp(name, "spell_cure_blindness"))
		return spell_cure_blindness;
	if (!str_cmp(name, "spell_cure_poison"))
		return spell_cure_poison;
	if (!str_cmp(name, "spell_curse"))
		return spell_curse;
	if (!str_cmp(name, "spell_detect_poison"))
		return spell_detect_poison;
	if (!str_cmp(name, "spell_dispel_evil"))
		return spell_dispel_evil;
	if (!str_cmp(name, "spell_dispel_magic"))
		return spell_dispel_magic;
	if (!str_cmp(name, "spell_dream"))
		return spell_dream;
	if (!str_cmp(name, "spell_earthquake"))
		return spell_earthquake;
	if (!str_cmp(name, "spell_enchant_weapon"))
		return spell_enchant_weapon;
	if (!str_cmp(name, "spell_energy_drain"))
		return spell_energy_drain;
	if (!str_cmp(name, "spell_faerie_fire"))
		return spell_faerie_fire;
	if (!str_cmp(name, "spell_faerie_fog"))
		return spell_faerie_fog;
	if (!str_cmp(name, "spell_farsight"))
		return spell_farsight;
	if (!str_cmp(name, "spell_fireball"))
		return spell_fireball;
	if (!str_cmp(name, "spell_flamestrike"))
		return spell_flamestrike;
	if (!str_cmp(name, "spell_gate"))
		return spell_gate;
	if (!str_cmp(name, "spell_knock"))
		return spell_knock;
	if (!str_cmp(name, "spell_harm"))
		return spell_harm;
	if (!str_cmp(name, "spell_identify"))
		return spell_identify;
	if (!str_cmp(name, "spell_invis"))
		return spell_invis;
	if (!str_cmp(name, "spell_know_alignment"))
		return spell_know_alignment;
	if (!str_cmp(name, "spell_lightning_bolt"))
		return spell_lightning_bolt;
	if (!str_cmp(name, "spell_locate_object"))
		return spell_locate_object;
	if (!str_cmp(name, "spell_magic_missile"))
		return spell_magic_missile;
	if (!str_cmp(name, "spell_mist_walk"))
		return spell_mist_walk;
	if (!str_cmp(name, "spell_pass_door"))
		return spell_pass_door;
	if (!str_cmp(name, "spell_plant_pass"))
		return spell_plant_pass;
	if (!str_cmp(name, "spell_poison"))
		return spell_poison;
	if (!str_cmp(name, "spell_polymorph"))
		return spell_polymorph;
	if (!str_cmp(name, "spell_possess"))
		return spell_possess;
	if (!str_cmp(name, "spell_recharge"))
		return spell_recharge;
	if (!str_cmp(name, "spell_remove_curse"))
		return spell_remove_curse;
	if (!str_cmp(name, "spell_remove_invis"))
		return spell_remove_invis;
	if (!str_cmp(name, "spell_remove_trap"))
		return spell_remove_trap;
	if (!str_cmp(name, "spell_shocking_grasp"))
		return spell_shocking_grasp;
	if (!str_cmp(name, "spell_sleep"))
		return spell_sleep;
	if (!str_cmp(name, "spell_solar_flight"))
		return spell_solar_flight;
	if (!str_cmp(name, "spell_suggest"))
		return spell_suggest;
	if (!str_cmp(name, "spell_summon"))
		return spell_summon;
	if (!str_cmp(name, "spell_teleport"))
		return spell_teleport;
	if (!str_cmp(name, "spell_ventriloquate"))
		return spell_ventriloquate;
	if (!str_cmp(name, "spell_weaken"))
		return spell_weaken;
	if (!str_cmp(name, "spell_word_of_recall"))
		return spell_word_of_recall;
	if (!str_cmp(name, "spell_acid_breath"))
		return spell_acid_breath;
	if (!str_cmp(name, "spell_fire_breath"))
		return spell_fire_breath;
	if (!str_cmp(name, "spell_frost_breath"))
		return spell_frost_breath;
	if (!str_cmp(name, "spell_gas_breath"))
		return spell_gas_breath;
	if (!str_cmp(name, "spell_lightning_breath"))
		return spell_lightning_breath;
	if (!str_cmp(name, "spell_spiral_blast"))
		return spell_spiral_blast;
	if (!str_cmp(name, "spell_scorching_surge"))
		return spell_scorching_surge;
	if (!str_cmp(name, "spell_helical_flow"))
		return spell_helical_flow;
	if (!str_cmp(name, "spell_transport"))
		return spell_transport;
	if (!str_cmp(name, "spell_portal"))
		return spell_portal;

	if (!str_cmp(name, "spell_ethereal_fist"))
		return spell_ethereal_fist;
	if (!str_cmp(name, "spell_spectral_furor"))
		return spell_spectral_furor;
	if (!str_cmp(name, "spell_hand_of_chaos"))
		return spell_hand_of_chaos;
	if (!str_cmp(name, "spell_disruption"))
		return spell_disruption;
	if (!str_cmp(name, "spell_sonic_resonance"))
		return spell_sonic_resonance;
	if (!str_cmp(name, "spell_mind_wrack"))
		return spell_mind_wrack;
	if (!str_cmp(name, "spell_mind_wrench"))
		return spell_mind_wrench;
	if (!str_cmp(name, "spell_revive"))
		return spell_revive;
	if (!str_cmp(name, "spell_sulfurous_spray"))
		return spell_sulfurous_spray;
	if (!str_cmp(name, "spell_caustic_fount"))
		return spell_caustic_fount;
	if (!str_cmp(name, "spell_acetum_primus"))
		return spell_acetum_primus;
	if (!str_cmp(name, "spell_galvanic_whip"))
		return spell_galvanic_whip;
	if (!str_cmp(name, "spell_magnetic_thrust"))
		return spell_magnetic_thrust;
	if (!str_cmp(name, "spell_quantum_spike"))
		return spell_quantum_spike;
	if (!str_cmp(name, "spell_black_hand"))
		return spell_black_hand;
	if (!str_cmp(name, "spell_black_fist"))
		return spell_black_fist;
	if (!str_cmp(name, "spell_black_lightning"))
		return spell_black_lightning;
	if (!str_cmp(name, "spell_midas_touch"))
		return spell_midas_touch;

	if (!str_cmp(name, "reserved"))
		return NULL;
	if (!str_cmp(name, "spell_null"))
		return spell_null;
	return spell_notfound;
}

DO_FUN *skill_function(char *name)
{
	switch (name[3]) {
	case 'a':
		if (!str_cmp(name, "do_aassign"))
			return do_aassign;
		if (!str_cmp(name, "do_addbounty"))
			return do_addbounty;
		if (!str_cmp(name, "do_addweapon"))
			return do_addweapon;
		if (!str_cmp(name, "do_addsenator"))
			return do_addsenator;
		if (!str_cmp(name, "do_addpilot"))
			return do_addpilot;
		if (!str_cmp(name, "do_add_patrol"))
			return do_add_patrol;
		if (!str_cmp(name, "do_accelerate"))
			return do_accelerate;
		if (!str_cmp(name, "do_advance"))
			return do_advance;
		if (!str_cmp(name, "do_affected"))
			return do_affected;
		if (!str_cmp(name, "do_afk"))
			return do_afk;
		if (!str_cmp(name, "do_aid"))
			return do_aid;
		if (!str_cmp(name, "do_allow"))
			return do_allow;
		if (!str_cmp(name, "do_allships"))
			return do_allships;
		if (!str_cmp(name, "do_allspeeders"))
			return do_allspeeders;
		if (!str_cmp(name, "do_ammo"))
			return do_ammo;
		if (!str_cmp(name, "do_ansi"))
			return do_ansi;
		if (!str_cmp(name, "do_answer"))
			return do_answer;
		if (!str_cmp(name, "do_apply"))
			return do_apply;
		if (!str_cmp(name, "do_appraise"))
			return do_appraise;
		if (!str_cmp(name, "do_appoint"))
			return do_appoint;
		if (!str_cmp(name, "do_aquest"))
			return do_aquest;
		if (!str_cmp(name, "do_areas"))
			return do_areas;
		if (!str_cmp(name, "do_arm"))
			return do_arm;
		if (!str_cmp(name, "do_aset"))
			return do_aset;
		if (!str_cmp(name, "do_ask"))
			return do_ask;
		if (!str_cmp(name, "do_astat"))
			return do_astat;
		if (!str_cmp(name, "do_at"))
			return do_at;
		if (!str_cmp(name, "do_attack"))
			return do_attack;
		if (!str_cmp(name, "do_auction"))
			return do_auction;
		if (!str_cmp(name, "do_authorize"))
			return do_authorize;
		if (!str_cmp(name, "do_autotrack"))
			return do_autotrack;
		if (!str_cmp(name, "do_autopilot"))
			return do_autopilot;
		if (!str_cmp(name, "do_avtalk"))
			return do_avtalk;
		if (!str_cmp(name, "do_applylicense"))
			return do_applylicense;
		if (!str_cmp(name, "do_attend"))
			return do_attend;
		if (!str_cmp(name, "do_afkmessage"))
			return do_afkmessage;
		if (!str_cmp(name, "do_addradio"))
			return do_addradio;
		if (!str_cmp(name, "do_appeal"))
			return do_appeal;
		break;
	case 'b':
		if (!str_cmp(name, "do_backstab"))
			return do_backstab;
		if (!str_cmp(name, "do_balzhur"))
			return do_balzhur;
		if (!str_cmp(name, "do_bamfin"))
			return do_bamfin;
		if (!str_cmp(name, "do_bamfout"))
			return do_bamfout;
		if (!str_cmp(name, "do_ban"))
			return do_ban;
		if (!str_cmp(name, "do_bank"))
			return do_bank;
		if (!str_cmp(name, "do_bash"))
			return do_bash;
		if (!str_cmp(name, "do_bashdoor"))
			return do_bashdoor;
		if (!str_cmp(name, "do_beep"))
			return do_beep;
		if (!str_cmp(name, "do_beg"))
			return do_beg;
		if (!str_cmp(name, "do_berserk"))
			return do_berserk;
		if (!str_cmp(name, "do_bestow"))
			return do_bestow;
		if (!str_cmp(name, "do_bestowarea"))
			return do_bestowarea;
		if (!str_cmp(name, "do_bigshot"))
			return do_bigshot;
		if (!str_cmp(name, "do_bio"))
			return do_bio;
		if (!str_cmp(name, "do_board"))
			return do_board;
		if (!str_cmp(name, "do_boards"))
			return do_boards;
		if (!str_cmp(name, "do_bodybag"))
			return do_bodybag;
		if (!str_cmp(name, "do_bomb"))
			return do_bomb;
		if (!str_cmp(name, "do_bounties"))
			return do_bounties;
		if (!str_cmp(name, "do_brandish"))
			return do_brandish;
		if (!str_cmp(name, "do_bribe"))
			return do_bribe;
		if (!str_cmp(name, "do_bset"))
			return do_bset;
		if (!str_cmp(name, "do_bstat"))
			return do_bstat;
		if (!str_cmp(name, "do_bug"))
			return do_bug;
		if (!str_cmp(name, "do_bury"))
			return do_bury;
		if (!str_cmp(name, "do_buy"))
			return do_buy;
		if (!str_cmp(name, "do_buyhome"))
			return do_buyhome;
		if (!str_cmp(name, "do_buyship"))
			return do_buyship;
		if (!str_cmp(name, "do_background"))
			return do_background;
		if (!str_cmp(name, "do_bankfreeze"))
			return do_bankfreeze;
		if (!str_cmp(name, "do_bandage"))
			return do_bandage;
		if (!str_cmp(name, "do_bootup"))
			return do_bootup;
		break;
	case 'c':
		if (!str_cmp(name, "do_copyover"))
			return do_copyover;
		if (!str_cmp(name, "do_calculate"))
			return do_calculate;
		if (!str_cmp(name, "do_capture"))
			return do_capture;
		if (!str_cmp(name, "do_cast"))
			return do_cast;
		if (!str_cmp(name, "do_cedit"))
			return do_cedit;
		if (!str_cmp(name, "do_chaff"))
			return do_chaff;
		if (!str_cmp(name, "do_channels"))
			return do_channels;
		if (!str_cmp(name, "do_chat"))
			return do_chat;
		if (!str_cmp(name, "do_check_vnums"))
			return do_check_vnums;
		if (!str_cmp(name, "do_circle"))
			return do_circle;
		if (!str_cmp(name, "do_clans"))
			return do_clans;
		if (!str_cmp(name, "do_clan_donate"))
			return do_clan_donate;
		if (!str_cmp(name, "do_clan_withdraw"))
			return do_clan_withdraw;
		if (!str_cmp(name, "do_clantalk"))
			return do_clantalk;
		if (!str_cmp(name, "do_clanbuyship"))
			return do_clanbuyship;
		if (!str_cmp(name, "do_clansellship"))
			return do_clansellship;
		if (!str_cmp(name, "do_climb"))
			return do_climb;
		if (!str_cmp(name, "do_close"))
			return do_close;
		if (!str_cmp(name, "do_clone"))
			return do_clone;
		if (!str_cmp(name, "do_closebay"))
			return do_closebay;
		if (!str_cmp(name, "do_closehatch"))
			return do_closehatch;
		if (!str_cmp(name, "do_cmdtable"))
			return do_cmdtable;
		if (!str_cmp(name, "do_commands"))
			return do_commands;
		if (!str_cmp(name, "do_comment"))
			return do_comment;
		if (!str_cmp(name, "do_compare"))
			return do_compare;
		if (!str_cmp(name, "do_config"))
			return do_config;
		if (!str_cmp(name, "do_consider"))
			return do_consider;
		if (!str_cmp(name, "do_copyship"))
			return do_copyship;
		if (!str_cmp(name, "do_credits"))
			return do_credits;
		if (!str_cmp(name, "do_cset"))
			return do_cset;
		if (!str_cmp(name, "do_commandeer"))
			return do_commandeer;
		if (!str_cmp(name, "do_calm"))
			return do_calm;
		break;
	case 'd':
		if (!str_cmp(name, "do_demote"))
			return do_demote;
		if (!str_cmp(name, "do_deny"))
			return do_deny;
		if (!str_cmp(name, "do_description"))
			return do_description;
		if (!str_cmp(name, "do_destro"))
			return do_destro;
		if (!str_cmp(name, "do_destroy"))
			return do_destroy;
		if (!str_cmp(name, "do_destroyrestore"))
			return do_destroyrestore;
		if (!str_cmp(name, "do_destroyslay"))
			return do_destroyslay;
		if (!str_cmp(name, "do_detrap"))
			return do_detrap;
		if (!str_cmp(name, "do_dig"))
			return do_dig;
		if (!str_cmp(name, "do_disarm"))
			return do_disarm;
		if (!str_cmp(name, "do_disconnect"))
			return do_disconnect;
		if (!str_cmp(name, "do_disguise"))
			return do_disguise;
		if (!str_cmp(name, "do_dismount"))
			return do_dismount;
		if (!str_cmp(name, "do_dmesg"))
			return do_dmesg;
		if (!str_cmp(name, "do_down"))
			return do_down;
		if (!str_cmp(name, "do_drag"))
			return do_drag;
		if (!str_cmp(name, "do_drink"))
			return do_drink;
		if (!str_cmp(name, "do_drive"))
			return do_drive;
		if (!str_cmp(name, "do_drop"))
			return do_drop;
		if (!str_cmp(name, "do_deceive"))
			return do_deceive;
		break;
	case 'e':
		if (!str_cmp(name, "do_east"))
			return do_east;
		if (!str_cmp(name, "do_eat"))
			return do_eat;
		if (!str_cmp(name, "do_echo"))
			return do_echo;
		if (!str_cmp(name, "do_ein"))
			return do_ein;
		if (!str_cmp(name, "do_elite_guard"))
			return do_elite_guard;
		if (!str_cmp(name, "do_emote"))
			return do_emote;
		if (!str_cmp(name, "do_empty"))
			return do_empty;
		if (!str_cmp(name, "do_empower"))
			return do_empower;
		if (!str_cmp(name, "do_enlist"))
			return do_enlist;
		if (!str_cmp(name, "do_enter"))
			return do_enter;
		if (!str_cmp(name, "do_equipment"))
			return do_equipment;
		if (!str_cmp(name, "do_examine"))
			return do_examine;
		if (!str_cmp(name, "do_exits"))
			return do_exits;
		if (!str_cmp(name, "do_endurance"))
			return do_endurance;
		break;
	case 'f':
		if (!str_cmp(name, "do_fakequit"))
			return do_fakequit;
		if (!str_cmp(name, "do_fill"))
			return do_fill;
		if (!str_cmp(name, "do_finger"))
			return do_finger;
		if (!str_cmp(name, "do_fire"))
			return do_fire;
		if (!str_cmp(name, "do_fire2"))
			return do_fire2;
		if (!str_cmp(name, "do_first_aid"))
			return do_first_aid;
		if (!str_cmp(name, "do_firstweapon"))
			return do_firstweapon;
		if (!str_cmp(name, "do_fixchar"))
			return do_fixchar;
		if (!str_cmp(name, "do_flee"))
			return do_flee;
		if (!str_cmp(name, "do_fly"))
			return do_fly;
		if (!str_cmp(name, "do_focus"))
			return do_focus;
		if (!str_cmp(name, "do_foldarea"))
			return do_foldarea;
		if (!str_cmp(name, "do_follow"))
			return do_follow;
		if (!str_cmp(name, "do_for"))
			return do_for;
		if (!str_cmp(name, "do_force"))
			return do_force;
		if (!str_cmp(name, "do_forceclose"))
			return do_forceclose;
		if (!str_cmp(name, "do_form_password"))
			return do_form_password;
		if (!str_cmp(name, "do_fquit"))
			return do_fquit;
		if (!str_cmp(name, "do_freeze"))
			return do_freeze;
		if (!str_cmp(name, "do_frenzy"))
			return do_frenzy;
		break;
	case 'g':
		if (!str_cmp(name, "do_gather_intelligence"))
			return do_gather_intelligence;
		if (!str_cmp(name, "do_get"))
			return do_get;
		if (!str_cmp(name, "do_gemcutting"))
			return do_gemcutting;
		if (!str_cmp(name, "do_giveplanet"))
			return do_giveplanet;
		if (!str_cmp(name, "do_give"))
			return do_give;
		if (!str_cmp(name, "do_glance"))
			return do_glance;
		if (!str_cmp(name, "do_global_boards"))
			return do_global_boards;
		if (!str_cmp(name, "do_global_note"))
			return do_global_note;
		if (!str_cmp(name, "do_gold"))
			return do_gold;
		if (!str_cmp(name, "do_goto"))
			return do_goto;
		if (!str_cmp(name, "do_gouge"))
			return do_gouge;
		if (!str_cmp(name, "do_group"))
			return do_group;
		if (!str_cmp(name, "do_gtell"))
			return do_gtell;
		if (!str_cmp(name, "do_guilds"))
			return do_guilds;
		if (!str_cmp(name, "do_guildtalk"))
			return do_guildtalk;
		if (!str_cmp(name, "do_group_heal"))
			return do_group_heal;
		if (!str_cmp(name, "do_gainage"))
			return do_gainage;
		break;
	case 'h':
		if (!str_cmp(name, "do_hack_systems"))
			return do_hack_systems;
		if (!str_cmp(name, "do_hackbaydoor"))
			return do_hackbaydoor;
		if (!str_cmp(name, "do_hackwho"))
			return do_hackwho;
		if (!str_cmp(name, "do_hackclanaccount"))
			return do_hackclanaccount;
		if (!str_cmp(name, "do_hackbanktransfer"))
			return do_hackbanktransfer;
		if (!str_cmp(name, "do_hackshipdoor"))
			return do_hackshipdoor;
		if (!str_cmp(name, "do_hacklaunch"))
			return do_hacklaunch;
		if (!str_cmp(name, "do_hackbroadcast"))
			return do_hackbroadcast;
		if (!str_cmp(name, "do_hackbounty"))
			return do_hackbounty;
		if (!str_cmp(name, "do_hail"))
			return do_hail;
		if (!str_cmp(name, "do_hedit"))
			return do_hedit;
		if (!str_cmp(name, "do_hell"))
			return do_hell;
		if (!str_cmp(name, "do_help"))
			return do_help;
		if (!str_cmp(name, "do_hide"))
			return do_hide;
		if (!str_cmp(name, "do_hijack"))
			return do_hijack;
		if (!str_cmp(name, "do_hitall"))
			return do_hitall;
		if (!str_cmp(name, "do_hlist"))
			return do_hlist;
		if (!str_cmp(name, "do_holylight"))
			return do_holylight;
		if (!str_cmp(name, "do_homepage"))
			return do_homepage;
		if (!str_cmp(name, "do_hset"))
			return do_hset;
		if (!str_cmp(name, "do_hyperspace"))
			return do_hyperspace;
		if (!str_cmp(name, "do_hackbank"))
			return do_hackbank;
		if (!str_cmp(name, "do_heal"))
			return do_heal;
		break;
	case 'i':
		if (!str_cmp(name, "do_i103"))
			return do_i103;
		if (!str_cmp(name, "do_i104"))
			return do_i104;
		if (!str_cmp(name, "do_i105"))
			return do_i105;
		if (!str_cmp(name, "do_ide"))
			return do_ide;
		if (!str_cmp(name, "do_idea"))
			return do_idea;
		if (!str_cmp(name, "do_immortalize"))
			return do_immortalize;
		if (!str_cmp(name, "do_immtalk"))
			return do_immtalk;
		if (!str_cmp(name, "do_info"))
			return do_info;
		if (!str_cmp(name, "do_invade"))
			return do_invade;
		if (!str_cmp(name, "do_induct"))
			return do_induct;
		if (!str_cmp(name, "do_installarea"))
			return do_installarea;
		if (!str_cmp(name, "do_instaroom"))
			return do_instaroom;
		if (!str_cmp(name, "do_instazone"))
			return do_instazone;
		if (!str_cmp(name, "do_inventory"))
			return do_inventory;
		if (!str_cmp(name, "do_invis"))
			return do_invis;
		if (!str_cmp(name, "do_ipcompare"))
			return do_ipcompare;
		if (!str_cmp(name, "do_ignore"))
			return do_ignore;
		break;
	case 'j':
		if (!str_cmp(name, "do_jumpvector"))
			return do_jumpvector;
		if (!str_cmp(name, "do_jail"))
			return do_jail;
		if (!str_cmp(name, "do_junk"))
			return do_junk;
		break;
	case 'k':
		if (!str_cmp(name, "do_kick"))
			return do_kick;
		if (!str_cmp(name, "do_kill"))
			return do_kill;
		break;
	case 'l':
		if (!str_cmp(name, "do_lagout"))
			return do_lagout;
		if (!str_cmp(name, "do_languages"))
			return do_languages;
		if (!str_cmp(name, "do_last"))
			return do_last;
		if (!str_cmp(name, "do_land"))
			return do_land;
		if (!str_cmp(name, "do_launch"))
			return do_launch;
		if (!str_cmp(name, "do_leave"))
			return do_leave;
		if (!str_cmp(name, "do_leaveship"))
			return do_leaveship;
		if (!str_cmp(name, "do_level"))
			return do_level;
		if (!str_cmp(name, "do_light"))
			return do_light;
		if (!str_cmp(name, "do_list"))
			return do_list;
		if (!str_cmp(name, "do_lift"))
			return do_lift;
		if (!str_cmp(name, "do_litterbug"))
			return do_litterbug;
		if (!str_cmp(name, "do_loadarea"))
			return do_loadarea;
		if (!str_cmp(name, "do_loadup"))
			return do_loadup;
		if (!str_cmp(name, "do_lock"))
			return do_lock;
		if (!str_cmp(name, "do_log"))
			return do_log;
		if (!str_cmp(name, "do_look"))
			return do_look;
		if (!str_cmp(name, "do_low_purge"))
			return do_low_purge;
		if (!str_cmp(name, "do_lobby"))
			return do_lobby;
		break;
	case 'm':
		if (!str_cmp(name, "do_mailroom"))
			return do_mailroom;
		if (!str_cmp(name, "do_make"))
			return do_make;
		if (!str_cmp(name, "do_makearmor"))
			return do_makearmor;
		if (!str_cmp(name, "do_makeknife"))
			return do_makeknife;
		if (!str_cmp(name, "do_makegun"))
			return do_makegun;
		if (!str_cmp(name, "do_makerifle"))
			return do_makerifle;
		if (!str_cmp(name, "do_makesword"))
			return do_makesword;
		if (!str_cmp(name, "do_makerestore"))
			return do_makerestore;
		if (!str_cmp(name, "do_makeslay"))
			return do_makeslay;
		if (!str_cmp(name, "do_makespice"))
			return do_makespice;
		if (!str_cmp(name, "do_makebloodyeye"))
			return do_makebloodyeye;
		if (!str_cmp(name, "do_makeyelloweye"))
			return do_makeyelloweye;
		if (!str_cmp(name, "do_makepurpleeye"))
			return do_makepurpleeye;
		if (!str_cmp(name, "do_makeblueeye"))
			return do_makeblueeye;
		if (!str_cmp(name, "do_makegreeneye"))
			return do_makegreeneye;
		if (!str_cmp(name, "do_makeapplicator"))
			return do_makeapplicator;
		if (!str_cmp(name, "do_makerawdrug"))
			return do_makerawdrug;
		if (!str_cmp(name, "do_makegrenade"))
			return do_makegrenade;
		if (!str_cmp(name, "do_makelight"))
			return do_makelight;
		if (!str_cmp(name, "do_makelandmine"))
			return do_makelandmine;
		if (!str_cmp(name, "do_makecomlink"))
			return do_makecomlink;
		if (!str_cmp(name, "do_makeshield"))
			return do_makeshield;
		if (!str_cmp(name, "do_makecontainer"))
			return do_makecontainer;
		if (!str_cmp(name, "do_makejewelry"))
			return do_makejewelry;
		if (!str_cmp(name, "do_makeboard"))
			return do_makeboard;
		if (!str_cmp(name, "do_makeclan"))
			return do_makeclan;
		if (!str_cmp(name, "do_makemissile"))
			return do_makemissile;
		if (!str_cmp(name, "do_makeship"))
			return do_makeship;
		if (!str_cmp(name, "do_makerepair"))
			return do_makerepair;
		if (!str_cmp(name, "do_makeshop"))
			return do_makeshop;
		if (!str_cmp(name, "do_makestarsystem"))
			return do_makestarsystem;
		if (!str_cmp(name, "do_makewizlist"))
			return do_makewizlist;
		if (!str_cmp(name, "do_makeplanet"))
			return do_makeplanet;
		if (!str_cmp(name, "do_massign"))
			return do_massign;
		if (!str_cmp(name, "do_mass_propaganda"))
			return do_mass_propaganda;
		if (!str_cmp(name, "do_mcreate"))
			return do_mcreate;
		if (!str_cmp(name, "do_mdelete"))
			return do_mdelete;
		if (!str_cmp(name, "do_memory"))
			return do_memory;
		if (!str_cmp(name, "do_mfind"))
			return do_mfind;
		if (!str_cmp(name, "do_minvoke"))
			return do_minvoke;
		if (!str_cmp(name, "do_mine"))
			return do_mine;
		if (!str_cmp(name, "do_mlist"))
			return do_mlist;
		if (!str_cmp(name, "do_mortality"))
			return do_mortality;
		if (!str_cmp(name, "do_mount"))
			return do_mount;
		if (!str_cmp(name, "do_mp_close_passage"))
			return do_mp_close_passage;
		if (!str_cmp(name, "do_mp_damage"))
			return do_mp_damage;
		if (!str_cmp(name, "do_mp_deposit"))
			return do_mp_deposit;
		if (!str_cmp(name, "do_mp_open_passage"))
			return do_mp_open_passage;
		if (!str_cmp(name, "do_mp_practice"))
			return do_mp_practice;
		if (!str_cmp(name, "do_mp_restore"))
			return do_mp_restore;
		if (!str_cmp(name, "do_mp_slay"))
			return do_mp_slay;
		if (!str_cmp(name, "do_mp_withdraw"))
			return do_mp_withdraw;
		if (!str_cmp(name, "do_mpadvance"))
			return do_mpadvance;
		if (!str_cmp(name, "do_mpapply"))
			return do_mpapply;
		if (!str_cmp(name, "do_mpapplyb"))
			return do_mpapplyb;
		if (!str_cmp(name, "do_mpasound"))
			return do_mpasound;
		if (!str_cmp(name, "do_mpat"))
			return do_mpat;
		if (!str_cmp(name, "do_mpdream"))
			return do_mpdream;
		if (!str_cmp(name, "do_mpecho"))
			return do_mpecho;
		if (!str_cmp(name, "do_mpechoaround"))
			return do_mpechoaround;
		if (!str_cmp(name, "do_mpechoat"))
			return do_mpechoat;
		if (!str_cmp(name, "do_mpechozone"))
			return do_mpechozone;
		if (!str_cmp(name, "do_mpedit"))
			return do_mpedit;
		if (!str_cmp(name, "do_mpgain"))
			return do_mpgain;
		if (!str_cmp(name, "do_mpdelay"))
			return do_mpdelay;
		if (!str_cmp(name, "do_mpforce"))
			return do_mpforce;
		if (!str_cmp(name, "do_mpgoto"))
			return do_mpgoto;
		if (!str_cmp(name, "do_mpinvis"))
			return do_mpinvis;
		if (!str_cmp(name, "do_mpjunk"))
			return do_mpjunk;
		if (!str_cmp(name, "do_mpkill"))
			return do_mpkill;
		if (!str_cmp(name, "do_mpmload"))
			return do_mpmload;
		if (!str_cmp(name, "do_mpnothing"))
			return do_mpnothing;
		if (!str_cmp(name, "do_mpoload"))
			return do_mpoload;
		if (!str_cmp(name, "do_mppkset"))
			return do_mppkset;
		if (!str_cmp(name, "do_mppurge"))
			return do_mppurge;
		if (!str_cmp(name, "do_mpsleep"))
			return do_mpsleep;
		if (!str_cmp(name, "do_mpstat"))
			return do_mpstat;
		if (!str_cmp(name, "do_mptransfer"))
			return do_mptransfer;
		if (!str_cmp(name, "do_mrange"))
			return do_mrange;
		if (!str_cmp(name, "do_mset"))
			return do_mset;
		if (!str_cmp(name, "do_mstat"))
			return do_mstat;
		if (!str_cmp(name, "do_music"))
			return do_music;
		if (!str_cmp(name, "do_mwhere"))
			return do_mwhere;
		if (!str_cmp(name, "do_makelaptop"))
			return do_makelaptop;
		if (!str_cmp(name, "do_mpwarkill"))
			return do_mpwarkill;
		if (!str_cmp(name, "do_mpreloadship"))
			return do_mpreloadship;
		if (!str_cmp(name, "do_mprepairship"))
			return do_mprepairship;
		break;
	case 'n':
		if (!str_cmp(name, "do_name"))
			return do_name;
		if (!str_cmp(name, "do_newbiechat"))
			return do_newbiechat;
		if (!str_cmp(name, "do_newbieset"))
			return do_newbieset;
		if (!str_cmp(name, "do_newclan"))
			return do_newclan;
		if (!str_cmp(name, "do_newzones"))
			return do_newzones;
		if (!str_cmp(name, "do_newpassword"))
			return do_newpassword;
		if (!str_cmp(name, "do_noemote"))
			return do_noemote;
		if (!str_cmp(name, "do_noresolve"))
			return do_noresolve;
		if (!str_cmp(name, "do_north"))
			return do_north;
		if (!str_cmp(name, "do_northeast"))
			return do_northeast;
		if (!str_cmp(name, "do_northwest"))
			return do_northwest;
		if (!str_cmp(name, "do_notell"))
			return do_notell;
		if (!str_cmp(name, "do_notitle"))
			return do_notitle;
		if (!str_cmp(name, "do_noteroom"))
			return do_noteroom;
		if (!str_cmp(name, "do_newclans"))
			return do_newclans;
		if (!str_cmp(name, "do_new_silence"))
			return do_new_silence;
		if (!str_cmp(name, "do_new_unsilence"))
			return do_new_unsilence;
		if (!str_cmp(name, "do_newwhois"))
			return do_newwhois;
		if (!str_cmp(name, "do_newstudy"))
			return do_newstudy;
		if (!str_cmp(name, "do_newendurance"))
			return do_newendurance;
		if (!str_cmp(name, "do_newstretch"))
			return do_newstretch;
		if (!str_cmp(name, "do_newresearch"))
			return do_newresearch;
		if (!str_cmp(name, "do_newsocialize"))
			return do_newsocialize;
		break;
	case 'o':
		if (!str_cmp(name, "do_oassign"))
			return do_oassign;
		if (!str_cmp(name, "do_ocreate"))
			return do_ocreate;
		if (!str_cmp(name, "do_odelete"))
			return do_odelete;
		if (!str_cmp(name, "do_ofind"))
			return do_ofind;
/*	if ( !str_cmp( name, "do_ogrub" ))		return do_ogrub;
*/ if (!str_cmp(name, "do_oinvoke"))
			return do_oinvoke;
		if (!str_cmp(name, "do_oinvade"))
			return do_oinvade;
		if (!str_cmp(name, "do_oldscore"))
			return do_oldscore;
		if (!str_cmp(name, "do_olist"))
			return do_olist;
		if (!str_cmp(name, "do_ooc"))
			return do_ooc;
		if (!str_cmp(name, "do_opedit"))
			return do_opedit;
		if (!str_cmp(name, "do_open"))
			return do_open;
		if (!str_cmp(name, "do_openbay"))
			return do_openbay;
		if (!str_cmp(name, "do_openhatch"))
			return do_openhatch;
/*	if ( !str_cmp( name, "do_opentourney" ))	return do_opentourney; */
		if (!str_cmp(name, "do_opstat"))
			return do_opstat;
		if (!str_cmp(name, "do_orange"))
			return do_orange;
		if (!str_cmp(name, "do_order"))
			return do_order;
		if (!str_cmp(name, "do_orders"))
			return do_orders;
		if (!str_cmp(name, "do_ordertalk"))
			return do_ordertalk;
		if (!str_cmp(name, "do_oset"))
			return do_oset;
		if (!str_cmp(name, "do_ostat"))
			return do_ostat;
		if (!str_cmp(name, "do_outcast"))
			return do_outcast;
		if (!str_cmp(name, "do_override"))
			return do_override;
		if (!str_cmp(name, "do_overthrow"))
			return do_overthrow;
		if (!str_cmp(name, "do_owhere"))
			return do_owhere;
		if (!str_cmp(name, "do_osay"))
			return do_osay;
		if (!str_cmp(name, "do_otell"))
			return do_otell;
		if (!str_cmp(name, "do_oreply"))
			return do_oreply;
		break;
	case 'p':
		if (!str_cmp(name, "do_pager"))
			return do_pager;
		if (!str_cmp(name, "do_paint"))
			return do_paint;
		if (!str_cmp(name, "do_pardon"))
			return do_pardon;
		if (!str_cmp(name, "do_password"))
			return do_password;
		if (!str_cmp(name, "do_peace"))
			return do_peace;
		if (!str_cmp(name, "do_pick"))
			return do_pick;
		if (!str_cmp(name, "do_pickshiplock"))
			return do_pickshiplock;
		if (!str_cmp(name, "do_planets"))
			return do_planets;
		if (!str_cmp(name, "do_poison_weapon"))
			return do_poison_weapon;
		if (!str_cmp(name, "do_postguard"))
			return do_postguard;
		if (!str_cmp(name, "do_practice"))
			return do_practice;
		if (!str_cmp(name, "do_prompt"))
			return do_prompt;
		if (!str_cmp(name, "do_propaganda"))
			return do_propaganda;
		if (!str_cmp(name, "do_pull"))
			return do_pull;
		if (!str_cmp(name, "do_punch"))
			return do_punch;
		if (!str_cmp(name, "do_purge"))
			return do_purge;
		if (!str_cmp(name, "do_push"))
			return do_push;
		if (!str_cmp(name, "do_put"))
			return do_put;
		if (!str_cmp(name, "do_pcrename"))
			return do_pcrename;
		if (!str_cmp(name, "do_plead"))
			return do_plead;
		break;
	case 'q':
		if (!str_cmp(name, "do_qpset"))
			return do_qpset;
		if (!str_cmp(name, "do_qpstat"))
			return do_qpstat;
		if (!str_cmp(name, "do_quaff"))
			return do_quaff;
		if (!str_cmp(name, "do_qui"))
			return do_qui;
		/*if ( !str_cmp( name, "do_quickdraw" ))                return do_quickdraw; */
		if (!str_cmp(name, "do_quit"))
			return do_quit;
		break;
	case 'r':
		if (!str_cmp(name, "do_radar"))
			return do_radar;
		if (!str_cmp(name, "do_rank"))
			return do_rank;
		if (!str_cmp(name, "do_rassign"))
			return do_rassign;
		if (!str_cmp(name, "do_rat"))
			return do_rat;
		if (!str_cmp(name, "do_rdelete"))
			return do_rdelete;
		if (!str_cmp(name, "do_reboo"))
			return do_reboo;
		if (!str_cmp(name, "do_reboot"))
			return do_reboot;
		if (!str_cmp(name, "do_recho"))
			return do_recho;
		if (!str_cmp(name, "do_recall"))
			return do_recall;
		if (!str_cmp(name, "do_recite"))
			return do_recite;
		if (!str_cmp(name, "do_recite"))
			return do_recite;
		if (!str_cmp(name, "do_redit"))
			return do_redit;
		if (!str_cmp(name, "do_refuel"))
			return do_refuel;
		if (!str_cmp(name, "do_reinforcements"))
			return do_reinforcements;
		if (!str_cmp(name, "do_regoto"))
			return do_regoto;
		if (!str_cmp(name, "do_reload"))
			return do_reload;
		if (!str_cmp(name, "do_rembounty"))
			return do_rembounty;
		if (!str_cmp(name, "do_remweapon"))
			return do_remweapon;
		if (!str_cmp(name, "do_remove"))
			return do_remove;
		if (!str_cmp(name, "do_rempilot"))
			return do_rempilot;
		if (!str_cmp(name, "do_remsenator"))
			return do_remsenator;
		if (!str_cmp(name, "do_renameship"))
			return do_renameship;
		if (!str_cmp(name, "do_rent"))
			return do_rent;
		if (!str_cmp(name, "do_repair"))
			return do_repair;
		if (!str_cmp(name, "do_repairset"))
			return do_repairset;
		if (!str_cmp(name, "do_repairship"))
			return do_repairship;
		if (!str_cmp(name, "do_repairshops"))
			return do_repairshops;
		if (!str_cmp(name, "do_repairstat"))
			return do_repairstat;
		if (!str_cmp(name, "do_reply"))
			return do_reply;
		if (!str_cmp(name, "do_report"))
			return do_report;
		if (!str_cmp(name, "do_rescue"))
			return do_rescue;
		if (!str_cmp(name, "do_reset"))
			return do_reset;
		if (!str_cmp(name, "do_resetship"))
			return do_resetship;
		if (!str_cmp(name, "do_resign"))
			return do_resign;
		if (!str_cmp(name, "do_rest"))
			return do_rest;
		if (!str_cmp(name, "do_restore"))
			return do_restore;
		if (!str_cmp(name, "do_restoretime"))
			return do_restoretime;
		if (!str_cmp(name, "do_restrict"))
			return do_restrict;
		if (!str_cmp(name, "do_retire"))
			return do_retire;
		if (!str_cmp(name, "do_retran"))
			return do_retran;
		if (!str_cmp(name, "do_return"))
			return do_return;
		if (!str_cmp(name, "do_revert"))
			return do_revert;
		if (!str_cmp(name, "do_rip"))
			return do_rip;
		if (!str_cmp(name, "do_rlist"))
			return do_rlist;
		if (!str_cmp(name, "do_rpedit"))
			return do_rpedit;
		if (!str_cmp(name, "do_rpstat"))
			return do_rpstat;
		if (!str_cmp(name, "do_rreset"))
			return do_rreset;
		if (!str_cmp(name, "do_rset"))
			return do_rset;
		if (!str_cmp(name, "do_rstat"))
			return do_rstat;
		if (!str_cmp(name, "do_rumors"))
			return do_rumors;
		if (!str_cmp(name, "do_rp"))
			return do_rp;
		if (!str_cmp(name, "do_rpauthorize"))
			return do_rpauthorize;
		if (!str_cmp(name, "do_research"))
			return do_research;
		if (!str_cmp(name, "do_resuscitate"))
			return do_resuscitate;
		break;
	case 's':
		if (!str_cmp(name, "do_sacrifice"))
			return do_sacrifice;
		if (!str_cmp(name, "do_save"))
			return do_save;
		if (!str_cmp(name, "do_savearea"))
			return do_savearea;
		if (!str_cmp(name, "do_say"))
			return do_say;
		if (!str_cmp(name, "do_scan"))
			return do_scan;
		if (!str_cmp(name, "do_score"))
			return do_score;
		if (!str_cmp(name, "do_search"))
			return do_search;
		if (!str_cmp(name, "do_secondweapon"))
			return do_secondweapon;
		if (!str_cmp(name, "do_sedit"))
			return do_sedit;
		if (!str_cmp(name, "do_sell"))
			return do_sell;
		if (!str_cmp(name, "do_sellhome"))
			return do_sellhome;
		if (!str_cmp(name, "do_sellship"))
			return do_sellship;
		if (!str_cmp(name, "do_senate"))
			return do_senate;
		if (!str_cmp(name, "do_set_boot_time"))
			return do_set_boot_time;
		if (!str_cmp(name, "do_setclan"))
			return do_setclan;
		if (!str_cmp(name, "do_semote"))
			return do_semote;
		if (!str_cmp(name, "do_setlevels"))
			return do_setlevels;
		if (!str_cmp(name, "do_setship"))
			return do_setship;
		if (!str_cmp(name, "do_setstarsystem"))
			return do_setstarsystem;
		if (!str_cmp(name, "do_setplanet"))
			return do_setplanet;
		if (!str_cmp(name, "do_setrestore"))
			return do_setrestore;
		if (!str_cmp(name, "do_setslay"))
			return do_setslay;
		if (!str_cmp(name, "do_ships"))
			return do_ships;
		if (!str_cmp(name, "do_shops"))
			return do_shops;
		if (!str_cmp(name, "do_shopset"))
			return do_shopset;
		if (!str_cmp(name, "do_shopstat"))
			return do_shopstat;
		if (!str_cmp(name, "do_shout"))
			return do_shout;
		if (!str_cmp(name, "do_shove"))
			return do_shove;
		if (!str_cmp(name, "do_showclan"))
			return do_showclan;
		if (!str_cmp(name, "do_showship"))
			return do_showship;
		if (!str_cmp(name, "do_showrestore"))
			return do_showrestore;
		if (!str_cmp(name, "do_showslay"))
			return do_showslay;
		if (!str_cmp(name, "do_showstarsystem"))
			return do_showstarsystem;
		if (!str_cmp(name, "do_shutdow"))
			return do_shutdow;
		if (!str_cmp(name, "do_shutdown"))
			return do_shutdown;
		if (!str_cmp(name, "do_shutoff"))
			return do_shutoff;
		if (!str_cmp(name, "do_silence"))
			return do_silence;
		if (!str_cmp(name, "do_sit"))
			return do_sit;
		if (!str_cmp(name, "do_sla"))
			return do_sla;
		if (!str_cmp(name, "do_slay"))
			return do_slay;
		if (!str_cmp(name, "do_sleep"))
			return do_sleep;
		if (!str_cmp(name, "do_slist"))
			return do_slist;
		if (!str_cmp(name, "do_slookup"))
			return do_slookup;
		if (!str_cmp(name, "do_smalltalk"))
			return do_smalltalk;
		if (!str_cmp(name, "do_smoke"))
			return do_smoke;
		if (!str_cmp(name, "do_snipe"))
			return do_snipe;
		if (!str_cmp(name, "do_snoop"))
			return do_snoop;
		if (!str_cmp(name, "do_sober"))
			return do_sober;
		if (!str_cmp(name, "do_socials"))
			return do_socials;
		if (!str_cmp(name, "do_south"))
			return do_south;
		if (!str_cmp(name, "do_sound"))
			return do_sound;
		if (!str_cmp(name, "do_southeast"))
			return do_southeast;
		if (!str_cmp(name, "do_showplanet"))
			return do_showplanet;
		if (!str_cmp(name, "do_southwest"))
			return do_southwest;
		if (!str_cmp(name, "do_speak"))
			return do_speak;
		if (!str_cmp(name, "do_speeders"))
			return do_speeders;
		if (!str_cmp(name, "do_split"))
			return do_split;
		if (!str_cmp(name, "do_sset"))
			return do_sset;
		if (!str_cmp(name, "do_stand"))
			return do_stand;
		if (!str_cmp(name, "do_starsystems"))
			return do_starsystems;
		if (!str_cmp(name, "do_status"))
			return do_status;
		if (!str_cmp(name, "do_steal"))
			return do_steal;
		if (!str_cmp(name, "do_stealth"))
			return do_stealth;
		if (!str_cmp(name, "do_study"))
			return do_study;
		if (!str_cmp(name, "do_stretch"))
			return do_stretch;
		if (!str_cmp(name, "do_socialize"))
			return do_socialize;
		if (!str_cmp(name, "do_switch"))
			return do_switch;
		if (!str_cmp(name, "do_swlist"))
			return do_swlist;	//Locke
		if (!str_cmp(name, "do_swcreate"))
			return do_swcreate;	//Locke
		if (!str_cmp(name, "do_swset"))
			return do_swset;	//Locke
		if (!str_cmp(name, "do_swstat"))
			return do_swstat;	//Locke
		if (!str_cmp(name, "do_shiphail"))
			return do_shiphail;
		if (!str_cmp(name, "do_shiptalk"))
			return do_shiptalk;
		if (!str_cmp(name, "do_spacetalk"))
			return do_spacetalk;
		if (!str_cmp(name, "do_special_forces"))
			return do_special_forces;
		if (!str_cmp(name, "do_suicide"))
			return do_suicide;
		if (!str_cmp(name, "do_surrender"))
			return do_surrender;
		if (!str_cmp(name, "do_systemtalk"))
			return do_systemtalk;
		if (!str_cmp(name, "do_shadowbox"))
			return do_shadowbox;
		if (!str_cmp(name, "do_self_medicate"))
			return do_self_medicate;
		if (!str_cmp(name, "do_surgery"))
			return do_surgery;
		if (!str_cmp(name, "do_setclantalk"))
			return do_setclantalk;
		if (!str_cmp(name, "do_support"))
			return do_support;
		break;
	case 't':
		if (!str_cmp(name, "do_takedrug"))
			return do_takedrug;
		if (!str_cmp(name, "do_tamp"))
			return do_tamp;
		if (!str_cmp(name, "do_target"))
			return do_target;
		if (!str_cmp(name, "do_teach"))
			return do_teach;
		if (!str_cmp(name, "do_tell"))
			return do_tell;
		if (!str_cmp(name, "do_thirdweapon"))
			return do_thirdweapon;
		if (!str_cmp(name, "do_throw"))
			return do_throw;
		if (!str_cmp(name, "do_time"))
			return do_time;
		if (!str_cmp(name, "do_timecmd"))
			return do_timecmd;
		if (!str_cmp(name, "do_title"))
			return do_title;
		if (!str_cmp(name, "do_top20"))
			return do_top20;
		if (!str_cmp(name, "do_toggle"))
			return do_toggle;
		if (!str_cmp(name, "do_torture"))
			return do_torture;
		if (!str_cmp(name, "do_track"))
			return do_track;
		if (!str_cmp(name, "do_tractorbeam"))
			return do_tractorbeam;
		if (!str_cmp(name, "do_train"))
			return do_train;
		if (!str_cmp(name, "do_trajectory"))
			return do_trajectory;
		if (!str_cmp(name, "do_transfer"))
			return do_transfer;
		if (!str_cmp(name, "do_transship"))
			return do_transship;
		if (!str_cmp(name, "do_trust"))
			return do_trust;
		if (!str_cmp(name, "do_typo"))
			return do_typo;
		if (!str_cmp(name, "do_treat"))
			return do_treat;
		break;
	case 'u':
		if (!str_cmp(name, "do_unfoldarea"))
			return do_unfoldarea;
		if (!str_cmp(name, "do_unhell"))
			return do_unhell;
		if (!str_cmp(name, "do_unlock"))
			return do_unlock;
		if (!str_cmp(name, "do_unsilence"))
			return do_unsilence;
		if (!str_cmp(name, "do_up"))
			return do_up;
		if (!str_cmp(name, "do_use"))
			return do_use;
		if (!str_cmp(name, "do_users"))
			return do_users;
		break;
	case 'v':
		if (!str_cmp(name, "do_value"))
			return do_value;
		if (!str_cmp(name, "do_vassign"))
			return do_vassign;
		if (!str_cmp(name, "do_visible"))
			return do_visible;
		if (!str_cmp(name, "do_visitors"))
			return do_visitors;
		if (!str_cmp(name, "do_vnums"))
			return do_vnums;
		if (!str_cmp(name, "do_vsearch"))
			return do_vsearch;
		break;
	case 'w':
		if (!str_cmp(name, "do_wake"))
			return do_wake;
		if (!str_cmp(name, "do_wartalk"))
			return do_wartalk;
		if (!str_cmp(name, "do_wear"))
			return do_wear;
		if (!str_cmp(name, "do_weather"))
			return do_weather;
		if (!str_cmp(name, "do_west"))
			return do_west;
		if (!str_cmp(name, "do_where"))
			return do_where;
		if (!str_cmp(name, "do_whisper"))
			return do_whisper;
		if (!str_cmp(name, "do_who"))
			return do_who;
		if (!str_cmp(name, "do_whois"))
			return do_whois;
		if (!str_cmp(name, "do_webwho"))
			return do_webwho;
		if (!str_cmp(name, "do_wimpy"))
			return do_wimpy;
		if (!str_cmp(name, "do_wizhelp"))
			return do_wizhelp;
		if (!str_cmp(name, "do_wizlist"))
			return do_wizlist;
		if (!str_cmp(name, "do_wizlock"))
			return do_wizlock;
		break;
	case 'y':
		if (!str_cmp(name, "do_yell"))
			return do_yell;
		break;
	case 'z':
		if (!str_cmp(name, "do_zap"))
			return do_zap;
		if (!str_cmp(name, "do_zones"))
			return do_zones;
		if (!str_cmp(name, "do_zecho"))
			return do_zecho;
	}
	return skill_notfound;
}

char *spell_name(SPELL_FUN * spell)
{
	if (spell == spell_smaug)
		return "spell_smaug";
	if (spell == spell_acid_blast)
		return "spell_acid_blast";
	if (spell == spell_animate_dead)
		return "spell_animate_dead";
	if (spell == spell_astral_walk)
		return "spell_astral_walk";
	if (spell == spell_blindness)
		return "spell_blindness";
	if (spell == spell_burning_hands)
		return "spell_burning_hands";
	if (spell == spell_call_lightning)
		return "spell_call_lightning";
	if (spell == spell_cause_critical)
		return "spell_cause_critical";
	if (spell == spell_cause_light)
		return "spell_cause_light";
	if (spell == spell_cause_serious)
		return "spell_cause_serious";
	if (spell == spell_change_sex)
		return "spell_change_sex";
	if (spell == spell_charm_person)
		return "spell_charm_person";
	if (spell == spell_chill_touch)
		return "spell_chill_touch";
	if (spell == spell_colour_spray)
		return "spell_colour_spray";
	if (spell == spell_control_weather)
		return "spell_control_weather";
	if (spell == spell_create_food)
		return "spell_create_food";
	if (spell == spell_create_water)
		return "spell_create_water";
	if (spell == spell_cure_blindness)
		return "spell_cure_blindness";
	if (spell == spell_cure_poison)
		return "spell_cure_poison";
	if (spell == spell_curse)
		return "spell_curse";
	if (spell == spell_detect_poison)
		return "spell_detect_poison";
	if (spell == spell_dispel_evil)
		return "spell_dispel_evil";
	if (spell == spell_dispel_magic)
		return "spell_dispel_magic";
	if (spell == spell_dream)
		return "spell_dream";
	if (spell == spell_earthquake)
		return "spell_earthquake";
	if (spell == spell_enchant_weapon)
		return "spell_enchant_weapon";
	if (spell == spell_energy_drain)
		return "spell_energy_drain";
	if (spell == spell_faerie_fire)
		return "spell_faerie_fire";
	if (spell == spell_faerie_fog)
		return "spell_faerie_fog";
	if (spell == spell_farsight)
		return "spell_farsight";
	if (spell == spell_fireball)
		return "spell_fireball";
	if (spell == spell_flamestrike)
		return "spell_flamestrike";
	if (spell == spell_gate)
		return "spell_gate";
	if (spell == spell_knock)
		return "spell_knock";
	if (spell == spell_harm)
		return "spell_harm";
	if (spell == spell_identify)
		return "spell_identify";
	if (spell == spell_invis)
		return "spell_invis";
	if (spell == spell_know_alignment)
		return "spell_know_alignment";
	if (spell == spell_lightning_bolt)
		return "spell_lightning_bolt";
	if (spell == spell_locate_object)
		return "spell_locate_object";
	if (spell == spell_magic_missile)
		return "spell_magic_missile";
	if (spell == spell_mist_walk)
		return "spell_mist_walk";
	if (spell == spell_pass_door)
		return "spell_pass_door";
	if (spell == spell_plant_pass)
		return "spell_plant_pass";
	if (spell == spell_poison)
		return "spell_poison";
	if (spell == spell_polymorph)
		return "spell_polymorph";
	if (spell == spell_possess)
		return "spell_possess";
	if (spell == spell_recharge)
		return "spell_recharge";
	if (spell == spell_remove_curse)
		return "spell_remove_curse";
	if (spell == spell_remove_invis)
		return "spell_remove_invis";
	if (spell == spell_remove_trap)
		return "spell_remove_trap";
	if (spell == spell_shocking_grasp)
		return "spell_shocking_grasp";
	if (spell == spell_sleep)
		return "spell_sleep";
	if (spell == spell_solar_flight)
		return "spell_solar_flight";
	if (spell == spell_summon)
		return "spell_summon";
	if (spell == spell_teleport)
		return "spell_teleport";
	if (spell == spell_ventriloquate)
		return "spell_ventriloquate";
	if (spell == spell_weaken)
		return "spell_weaken";
	if (spell == spell_word_of_recall)
		return "spell_word_of_recall";
	if (spell == spell_acid_breath)
		return "spell_acid_breath";
	if (spell == spell_fire_breath)
		return "spell_fire_breath";
	if (spell == spell_frost_breath)
		return "spell_frost_breath";
	if (spell == spell_gas_breath)
		return "spell_gas_breath";
	if (spell == spell_lightning_breath)
		return "spell_lightning_breath";
	if (spell == spell_spiral_blast)
		return "spell_spiral_blast";
	if (spell == spell_scorching_surge)
		return "spell_scorching_surge";
	if (spell == spell_helical_flow)
		return "spell_helical_flow";
	if (spell == spell_transport)
		return "spell_transport";
	if (spell == spell_portal)
		return "spell_portal";
	if (spell == spell_suggest)
		return "spell_suggest";
	if (spell == spell_cure_addiction)
		return "spell_cure_addiction";

	if (spell == spell_ethereal_fist)
		return "spell_ethereal_fist";
	if (spell == spell_spectral_furor)
		return "spell_spectral_furor";
	if (spell == spell_hand_of_chaos)
		return "spell_hand_of_chaos";
	if (spell == spell_disruption)
		return "spell_disruption";
	if (spell == spell_sonic_resonance)
		return "spell_sonic_resonance";
	if (spell == spell_mind_wrack)
		return "spell_mind_wrack";
	if (spell == spell_mind_wrench)
		return "spell_mind_wrench";
	if (spell == spell_revive)
		return "spell_revive";
	if (spell == spell_sulfurous_spray)
		return "spell_sulfurous_spray";
	if (spell == spell_caustic_fount)
		return "spell_caustic_fount";
	if (spell == spell_acetum_primus)
		return "spell_acetum_primus";
	if (spell == spell_galvanic_whip)
		return "spell_galvanic_whip";
	if (spell == spell_magnetic_thrust)
		return "spell_magnetic_thrust";
	if (spell == spell_quantum_spike)
		return "spell_quantum_spike";
	if (spell == spell_black_hand)
		return "spell_black_hand";
	if (spell == spell_black_fist)
		return "spell_black_fist";
	if (spell == spell_black_lightning)
		return "spell_black_lightning";
	if (spell == spell_midas_touch)
		return "spell_midas_touch";

	if (spell == spell_null)
		return "spell_null";
	return "reserved";
}

char *skill_name(DO_FUN * skill)
{
	if (skill == do_copyover)
		return "do_copyover";
	if (skill == do_i103)
		return "do_i103";
	if (skill == do_i104)
		return "do_i104";
	if (skill == do_i105)
		return "do_i105";
	if (skill == do_teach)
		return "do_teach";
	if (skill == do_gather_intelligence)
		return "do_gather_intellingence";
	if (skill == do_add_patrol)
		return "do_add_patrol";
	if (skill == do_appeal)
		return "do_appeal";
	if (skill == do_addradio)
		return "do_addradio";
	if (skill == do_calm)
		return "do_calm";
	if (skill == do_attend)
		return "do_attend";
	if (skill == do_gainage)
		return "do_gainage";
	if (skill == do_special_forces)
		return "do_special_forces";
	if (skill == do_support)
		return "do_support";
	if (skill == do_setclantalk)
		return "do_setclantalk";
	if (skill == do_treat)
		return "do_treat";
	if (skill == do_surgery)
		return "do_surgery";
	if (skill == do_group_heal)
		return "do_group_heal";
	if (skill == do_self_medicate)
		return "do_self_medicate";
	if (skill == do_shadowbox)
		return "do_shadowbox";
	if (skill == do_applylicense)
		return "do_applylicense";
	if (skill == do_jail)
		return "do_jail";
	if (skill == do_junk)
		return "do_junk";
	if (skill == do_elite_guard)
		return "do_elite_guard";
	if (skill == do_endurance)
		return "do_endurance";
	if (skill == do_smalltalk)
		return "do_smalltalk";
	if (skill == do_propaganda)
		return "do_propaganda";
	if (skill == do_plead)
		return "do_plead";
	if (skill == do_pcrename)
		return "do_pcrename";
	if (skill == do_bandage)
		return "do_bandage";
	if (skill == do_bribe)
		return "do_bribe";
	if (skill == do_bootup)
		return "do_bootup";
	if (skill == do_mass_propaganda)
		return "do_mass_propaganda";
	if (skill == do_background)
		return "do_background";
	if (skill == do_senate)
		return "do_senate";
	if (skill == do_autopilot)
		return "do_autopilot";
	if (skill == do_allspeeders)
		return "do_allspeeders";
	if (skill == do_speeders)
		return "do_speeders";
	if (skill == do_suicide)
		return "do_suicide";
	if (skill == do_surrender)
		return "do_surrender";
	if (skill == do_train)
		return "do_train";
	if (skill == do_clone)
		return "do_clone";
	if (skill == do_commandeer)
		return "do_commandeer";
	if (skill == do_copyship)
		return "do_copyship";
	if (skill == do_hijack)
		return "do_hijack";
	if (skill == do_heal)
		return "do_heal";
	if (skill == do_pickshiplock)
		return "do_pickshiplock";
	if (skill == do_makejewelry)
		return "do_makejewelry";
	if (skill == do_makelaptop)
		return "do_makelaptop";
	if (skill == do_spacetalk)
		return "do_spacetalk";
	if (skill == do_shiphail)
		return "do_shiphail";
	if (skill == do_shiptalk)
		return "do_shiptalk";
	if (skill == do_systemtalk)
		return "do_systemtalk";
	if (skill == do_planets)
		return "do_planets";
	if (skill == do_makeplanet)
		return "do_makeplanet";
	if (skill == do_setplanet)
		return "do_setplanet";
	if (skill == do_semote)
		return "do_semote";
	if (skill == do_hack_systems)
		return "do_hack_systems";
	if (skill == do_hackclanaccount)
		return "do_hackclanaccount";
	if (skill == do_hackbank)
		return "do_hackbank";
	if (skill == do_hackbaydoor)
		return "do_hackbaydoor";
	if (skill == do_hackwho)
		return "do_hackwho";
	if (skill == do_hackbanktransfer)
		return "do_hackbanktransfer";
	if (skill == do_hackshipdoor)
		return "do_hackshipdoor";
	if (skill == do_hacklaunch)
		return "do_hacklaunch";
	if (skill == do_hackbroadcast)
		return "do_hackbroadcast";
	if (skill == do_hackbounty)
		return "do_hackbounty";
	if (skill == do_hail)
		return "do_hail";
	if (skill == do_appoint)
		return "do_appoint";
	if (skill == do_deceive)
		return "do_deceive";
	if (skill == do_demote)
		return "do_demote";
	if (skill == do_empower)
		return "do_empower";
	if (skill == do_capture)
		return "do_capture";
	if (skill == do_newsocialize)
		return "do_newsocialize";
	if (skill == do_newresearch)
		return "do_newresearch";
	if (skill == do_newstretch)
		return "do_newstretch";
	if (skill == do_newstudy)
		return "do_newstudy";
	if (skill == do_newendurance)
		return "do_newendurance";
	if (skill == do_newclan)
		return "do_newclan";
	if (skill == do_newpassword)
		return "do_newpassword";
	if (skill == do_newwhois)
		return "do_newwhois";
	if (skill == do_newclans)
		return "do_newclans";
	if (skill == do_new_silence)
		return "do_new_silence";
	if (skill == do_new_unsilence)
		return "do_new_unsilence";
	if (skill == do_arm)
		return "do_arm";
	if (skill == do_chaff)
		return "do_chaff";
	if (skill == do_allships)
		return "do_allships";
	if (skill == do_fly)
		return "do_fly";
	if (skill == do_bomb)
		return "do_bomb";
	if (skill == do_bank)
		return "do_bank";
	if (skill == do_drive)
		return "do_drive";
	if (skill == do_bankfreeze)
		return "do_bankfreeze";
	if (skill == do_ammo)
		return "do_ammo";
	if (skill == do_takedrug)
		return "do_takedrug";
	if (skill == do_use)
		return "do_use";
	if (skill == do_enlist)
		return "do_enlist";
	if (skill == do_resuscitate)
		return "do_resuscitate";
	if (skill == do_resign)
		return "do_resign";
	if (skill == do_tractorbeam)
		return "do_tractorbeam";
	if (skill == do_makearmor)
		return "do_makearmor";
	if (skill == do_makemissile)
		return "do_makemissile";
	if (skill == do_gemcutting)
		return "do_gemcutting";
	if (skill == do_reinforcements)
		return "do_reinforcements";
	if (skill == do_postguard)
		return "do_postguard";
	if (skill == do_torture)
		return "do_torture";
	if (skill == do_toggle)
		return "do_toggle";
	if (skill == do_throw)
		return "do_throw";
	if (skill == do_snipe)
		return "do_snipe";
	if (skill == do_disguise)
		return "do_disguise";
	if (skill == do_mine)
		return "do_mine";
	if (skill == do_first_aid)
		return "do_first_aid";
	if (skill == do_jumpvector)
		return "do_jumpvector";
	if (skill == do_autotrack)
		return "do_autotrack";
	if (skill == do_openbay)
		return "do_openbay";
	if (skill == do_otell)
		return "do_otell";
	if (skill == do_oreply)
		return "do_oreply";
	if (skill == do_closebay)
		return "do_closebay";
	if (skill == do_reload)
		return "do_reload";
	if (skill == do_radar)
		return "do_radar";
	if (skill == do_recall)
		return "do_recall";
	if (skill == do_buyship)
		return "do_buyship";
	if (skill == do_buyhome)
		return "do_buyhome";
	if (skill == do_clanbuyship)
		return "do_clanbuyship";
	if (skill == do_clansellship)
		return "do_clansellship";
	if (skill == do_sellhome)
		return "do_sellhome";
	if (skill == do_sellship)
		return "do_sellship";
	if (skill == do_info)
		return "do_info";
	if (skill == do_invade)
		return "do_invade";
	if (skill == do_ignore)
		return "do_ignore";
	if (skill == do_openhatch)
		return "do_openhatch";
	if (skill == do_closehatch)
		return "do_closehatch";
	if (skill == do_hyperspace)
		return "do_hyperspace";
	if (skill == do_target)
		return "do_target";
	if (skill == do_fire)
		return "do_fire";
	if (skill == do_fire2)
		return "do_fire2";
	if (skill == do_calculate)
		return "do_calculate";
	if (skill == do_repairship)
		return "do_repairship";
	if (skill == do_refuel)
		return "do_refuel";
	if (skill == do_addpilot)
		return "do_addpilot";
	if (skill == do_addsenator)
		return "do_addsenator";
	if (skill == do_rempilot)
		return "do_rempilot";
	if (skill == do_aassign)
		return "do_aassign";
	if (skill == do_accelerate)
		return "do_accelerate";
	if (skill == do_addbounty)
		return "do_addbounty";
	if (skill == do_addweapon)
		return "do_addweapon";
	if (skill == do_advance)
		return "do_advance";
	if (skill == do_affected)
		return "do_affected";
	if (skill == do_afk)
		return "do_afk";
	if (skill == do_afkmessage)
		return "do_afkmessage";
	if (skill == do_aid)
		return "do_aid";
	if (skill == do_allow)
		return "do_allow";
	if (skill == do_ansi)
		return "do_ansi";
	if (skill == do_sound)
		return "do_sound";
	if (skill == do_answer)
		return "do_answer";
	if (skill == do_apply)
		return "do_apply";
	if (skill == do_appraise)
		return "do_appraise";
	if (skill == do_aquest)
		return "do_aquest";
	if (skill == do_areas)
		return "do_areas";
	if (skill == do_aset)
		return "do_aset";
	if (skill == do_ask)
		return "do_ask";
	if (skill == do_astat)
		return "do_astat";
	if (skill == do_at)
		return "do_at";
	if (skill == do_attack)
		return "do_attack";
	if (skill == do_auction)
		return "do_auction";
	if (skill == do_authorize)
		return "do_authorize";
	if (skill == do_avtalk)
		return "do_avtalk";
	if (skill == do_backstab)
		return "do_backstab";
	if (skill == do_balzhur)
		return "do_balzhur";
	if (skill == do_bamfin)
		return "do_bamfin";
	if (skill == do_bamfout)
		return "do_bamfout";
	if (skill == do_ban)
		return "do_ban";
	if (skill == do_bash)
		return "do_bash";
	if (skill == do_bashdoor)
		return "do_bashdoor";
	if (skill == do_beep)
		return "do_beep";
	if (skill == do_beg)
		return "do_beg";
	if (skill == do_berserk)
		return "do_berserk";
	if (skill == do_firstweapon)
		return "do_firstweapon";
	if (skill == do_secondweapon)
		return "do_secondweapon";
	if (skill == do_thirdweapon)
		return "do_thirdweapon";
	if (skill == do_bestow)
		return "do_bestow";
	if (skill == do_bestowarea)
		return "do_bestowarea";
	if (skill == do_bigshot)
		return "do_bigshot";
	if (skill == do_bio)
		return "do_bio";
	if (skill == do_board)
		return "do_board";
	if (skill == do_boards)
		return "do_boards";
	if (skill == do_bodybag)
		return "do_bodybag";
	if (skill == do_bounties)
		return "do_bounties";
	if (skill == do_brandish)
		return "do_brandish";
	if (skill == do_bset)
		return "do_bset";
	if (skill == do_bstat)
		return "do_bstat";
	if (skill == do_bug)
		return "do_bug";
	if (skill == do_bury)
		return "do_bury";
	if (skill == do_buy)
		return "do_buy";
	if (skill == do_cast)
		return "do_cast";
	if (skill == do_cedit)
		return "do_cedit";
	if (skill == do_channels)
		return "do_channels";
	if (skill == do_chat)
		return "do_chat";
	if (skill == do_ooc)
		return "do_ooc";
	if (skill == do_check_vnums)
		return "do_check_vnums";
	if (skill == do_circle)
		return "do_circle";
	if (skill == do_clans)
		return "do_clans";
	if (skill == do_clan_donate)
		return "do_clan_donate";
	if (skill == do_clan_withdraw)
		return "do_clan_withdraw";
	if (skill == do_clantalk)
		return "do_clantalk";
	if (skill == do_climb)
		return "do_climb";
	if (skill == do_close)
		return "do_close";
	if (skill == do_cmdtable)
		return "do_cmdtable";
	if (skill == do_commands)
		return "do_commands";
	if (skill == do_comment)
		return "do_comment";
	if (skill == do_compare)
		return "do_compare";
	if (skill == do_config)
		return "do_config";
	if (skill == do_consider)
		return "do_consider";
	if (skill == do_credits)
		return "do_credits";
	if (skill == do_cset)
		return "do_cset";
	if (skill == do_deny)
		return "do_deny";
	if (skill == do_description)
		return "do_description";
	if (skill == do_destro)
		return "do_destro";
	if (skill == do_destroy)
		return "do_destroy";
	if (skill == do_destroyrestore)
		return "do_destroyrestore";
	if (skill == do_destroyslay)
		return "do_destroyslay";
	if (skill == do_detrap)
		return "do_detrap";
	if (skill == do_dig)
		return "do_dig";
	if (skill == do_disarm)
		return "do_disarm";
	if (skill == do_disconnect)
		return "do_disconnect";
	if (skill == do_dismount)
		return "do_dismount";
	if (skill == do_dmesg)
		return "do_dmesg";
	if (skill == do_down)
		return "do_down";
	if (skill == do_drag)
		return "do_drag";
	if (skill == do_drink)
		return "do_drink";
	if (skill == do_drop)
		return "do_drop";
/*  if ( skill == do_diagnose )		return "do_diagnose";*/
	if (skill == do_east)
		return "do_east";
	if (skill == do_eat)
		return "do_eat";
	if (skill == do_echo)
		return "do_echo";
	if (skill == do_ein)
		return "do_ein";
	if (skill == do_emote)
		return "do_emote";
	if (skill == do_empty)
		return "do_empty";
	if (skill == do_enter)
		return "do_enter";
	if (skill == do_equipment)
		return "do_equipment";
	if (skill == do_examine)
		return "do_examine";
	if (skill == do_exits)
		return "do_exits";
	if (skill == do_fakequit)
		return "do_fakequit";
	if (skill == do_finger)
		return "do_finger";
	if (skill == do_fill)
		return "do_fill";
	if (skill == do_fixchar)
		return "do_fixchar";
	if (skill == do_flee)
		return "do_flee";
	if (skill == do_focus)
		return "do_focus";
	if (skill == do_foldarea)
		return "do_foldarea";
	if (skill == do_follow)
		return "do_follow";
	if (skill == do_for)
		return "do_for";
	if (skill == do_force)
		return "do_force";
	if (skill == do_forceclose)
		return "do_forceclose";
	if (skill == do_form_password)
		return "do_form_password";
	if (skill == do_fquit)
		return "do_fquit";
	if (skill == do_freeze)
		return "do_freeze";
	if (skill == do_frenzy)
		return "do_frenzy";
	if (skill == do_get)
		return "do_get";
	if (skill == do_giveplanet)
		return "do_giveplanet";
	if (skill == do_give)
		return "do_give";
	if (skill == do_glance)
		return "do_glance";
	if (skill == do_global_boards)
		return "do_global_boards";
	if (skill == do_global_note)
		return "do_global_note";
	if (skill == do_gold)
		return "do_gold";
	if (skill == do_goto)
		return "do_goto";
	if (skill == do_gouge)
		return "do_gouge";
	if (skill == do_group)
		return "do_group";
	if (skill == do_gtell)
		return "do_gtell";
	if (skill == do_guilds)
		return "do_guilds";
	if (skill == do_guildtalk)
		return "do_guildtalk";
	if (skill == do_hedit)
		return "do_hedit";
	if (skill == do_hell)
		return "do_hell";
	if (skill == do_help)
		return "do_help";
	if (skill == do_hide)
		return "do_hide";
	if (skill == do_hitall)
		return "do_hitall";
	if (skill == do_hlist)
		return "do_hlist";
	if (skill == do_holylight)
		return "do_holylight";
	if (skill == do_homepage)
		return "do_homepage";
	if (skill == do_hset)
		return "do_hset";
	if (skill == do_ide)
		return "do_ide";
	if (skill == do_idea)
		return "do_idea";
	if (skill == do_immortalize)
		return "do_immortalize";
	if (skill == do_immtalk)
		return "do_immtalk";
	if (skill == do_induct)
		return "do_induct";
	if (skill == do_installarea)
		return "do_installarea";
	if (skill == do_instaroom)
		return "do_instaroom";
	if (skill == do_instazone)
		return "do_instazone";
	if (skill == do_inventory)
		return "do_inventory";
	if (skill == do_invis)
		return "do_invis";
	if (skill == do_ipcompare)
		return "do_ipcompare";
	if (skill == do_kick)
		return "do_kick";
	if (skill == do_kill)
		return "do_kill";
	if (skill == do_lagout)
		return "do_lagout";
	if (skill == do_lobby)
		return "do_lobby";
	if (skill == do_languages)
		return "do_languages";
	if (skill == do_last)
		return "do_last";
	if (skill == do_land)
		return "do_land";
	if (skill == do_launch)
		return "do_launch";
	if (skill == do_leave)
		return "do_leave";
	if (skill == do_leaveship)
		return "do_leaveship";
	if (skill == do_level)
		return "do_level";
	if (skill == do_light)
		return "do_light";
	if (skill == do_list)
		return "do_list";
	if (skill == do_lift)
		return "do_lift";
	if (skill == do_litterbug)
		return "do_litterbug";
	if (skill == do_loadarea)
		return "do_loadarea";
	if (skill == do_loadup)
		return "do_loadup";
	if (skill == do_lock)
		return "do_lock";
	if (skill == do_log)
		return "do_log";
	if (skill == do_look)
		return "do_look";
	if (skill == do_low_purge)
		return "do_low_purge";
	if (skill == do_mailroom)
		return "do_mailroom";
	if (skill == do_make)
		return "do_make";
	if (skill == do_makelight)
		return "do_makelight";
	if (skill == do_makecomlink)
		return "do_makecomlink";
	if (skill == do_makegrenade)
		return "do_makegrenade";
	if (skill == do_makelandmine)
		return "do_makelandmine";
	if (skill == do_makecontainer)
		return "do_makecontainer";
	if (skill == do_makeshield)
		return "do_makeshield";
	if (skill == do_makerestore)
		return "do_makerestore";
	if (skill == do_makeslay)
		return "do_makeslay";
	if (skill == do_makeknife)
		return "do_makeknife";
	if (skill == do_makegun)
		return "do_makegun";
	if (skill == do_makerifle)
		return "do_makerifle";
	if (skill == do_makesword)
		return "do_makesword";
	if (skill == do_makespice)
		return "do_makespice";
	if (skill == do_makebloodyeye)
		return "do_makebloodyeye";
	if (skill == do_makeyelloweye)
		return "do_makeyelloweye";
	if (skill == do_makepurpleeye)
		return "do_makepurpleeye";
	if (skill == do_makeblueeye)
		return "do_makeblueeye";
	if (skill == do_makegreeneye)
		return "do_makegreeneye";
	if (skill == do_makeapplicator)
		return "do_makeapplicator";
	if (skill == do_makerawdrug)
		return "do_makerawdrug";
	if (skill == do_none)
		return "do_none";
	if (skill == do_makeboard)
		return "do_makeboard";
	if (skill == do_makeclan)
		return "do_makeclan";
	if (skill == do_makerepair)
		return "do_makerepair";
	if (skill == do_makeshop)
		return "do_makeshop";
	if (skill == do_makeship)
		return "do_makeship";
	if (skill == do_makestarsystem)
		return "do_makestarsystem";
	if (skill == do_makewizlist)
		return "do_makewizlist";
	if (skill == do_massign)
		return "do_massign";
	if (skill == do_mcreate)
		return "do_mcreate";
	if (skill == do_mdelete)
		return "do_mdelete";
	if (skill == do_memory)
		return "do_memory";
	if (skill == do_mfind)
		return "do_mfind";
	if (skill == do_minvoke)
		return "do_minvoke";
	if (skill == do_mlist)
		return "do_mlist";
	if (skill == do_mortality)
		return "do_mortality";
	if (skill == do_mount)
		return "do_mount";
	if (skill == do_mpwarkill)
		return "do_mpwarkill";
	if (skill == do_mp_close_passage)
		return "do_mp_close_passage";
	if (skill == do_mp_damage)
		return "do_mp_damage";
	if (skill == do_mp_deposit)
		return "do_mp_deposit";
	if (skill == do_mp_open_passage)
		return "do_mp_open_passage";
	if (skill == do_mp_practice)
		return "do_mp_practice";
	if (skill == do_mp_restore)
		return "do_mp_restore";
	if (skill == do_mp_slay)
		return "do_mp_slay";
	if (skill == do_mp_withdraw)
		return "do_mp_withdraw";
	if (skill == do_mpadvance)
		return "do_mpadvance";
	if (skill == do_mpechozone)
		return "do_mpechozone";
	if (skill == do_mpapply)
		return "do_mpapply";
	if (skill == do_mpapplyb)
		return "do_mpapplyb";
	if (skill == do_mpasound)
		return "do_mpasound";
	if (skill == do_mpat)
		return "do_mpat";
	if (skill == do_mpdream)
		return "do_mpdream";
	if (skill == do_mpecho)
		return "do_mpecho";
	if (skill == do_mpechoaround)
		return "do_mpechoaround";
	if (skill == do_mpechoat)
		return "do_mpechoat";
	if (skill == do_mpdelay)
		return "do_mpdelay";
	if (skill == do_mpedit)
		return "do_mpedit";
	if (skill == do_mpgain)
		return "do_mpgain";
	if (skill == do_mpforce)
		return "do_mpforce";
	if (skill == do_mpgoto)
		return "do_mpgoto";
	if (skill == do_mpinvis)
		return "do_mpinvis";
	if (skill == do_mpjunk)
		return "do_mpjunk";
	if (skill == do_mpkill)
		return "do_mpkill";
	if (skill == do_mpmload)
		return "do_mpmload";
	if (skill == do_mpnothing)
		return "do_mpnothing";
	if (skill == do_mpoload)
		return "do_mpoload";
	if (skill == do_mppkset)
		return "do_mppkset";
	if (skill == do_mppurge)
		return "do_mppurge";
	if (skill == do_mpsleep)
		return "do_mpsleep";
	if (skill == do_mpstat)
		return "do_mpstat";
	if (skill == do_mptransfer)
		return "do_mptransfer";
	if (skill == do_mrange)
		return "do_mrange";
	if (skill == do_mset)
		return "do_mset";
	if (skill == do_mstat)
		return "do_mstat";
	if (skill == do_music)
		return "do_music";
	if (skill == do_mwhere)
		return "do_mwhere";
	if (skill == do_name)
		return "do_name";
	if (skill == do_newbiechat)
		return "do_newbiechat";
	if (skill == do_newbieset)
		return "do_newbieset";
	if (skill == do_newzones)
		return "do_newzones";
	if (skill == do_noemote)
		return "do_noemote";
	if (skill == do_noresolve)
		return "do_noresolve";
	if (skill == do_north)
		return "do_north";
	if (skill == do_northeast)
		return "do_northeast";
	if (skill == do_northwest)
		return "do_northwest";
	if (skill == do_notell)
		return "do_notell";
	if (skill == do_notitle)
		return "do_notitle";
	if (skill == do_noteroom)
		return "do_noteroom";
	if (skill == do_oassign)
		return "do_oassign";
	if (skill == do_ocreate)
		return "do_ocreate";
	if (skill == do_odelete)
		return "do_odelete";
	if (skill == do_ofind)
		return "do_ofind";
/*    if ( skill == do_ogrub )		return "do_ogrub";
*/ if (skill == do_oinvoke)
		return "do_oinvoke";
	if (skill == do_oinvade)
		return "do_oinvade";
	if (skill == do_oldscore)
		return "do_oldscore";
	if (skill == do_olist)
		return "do_olist";
	if (skill == do_opedit)
		return "do_opedit";
	if (skill == do_open)
		return "do_open";
/*  if ( skill == do_opentourney )	return "do_opentourney"; */
	if (skill == do_opstat)
		return "do_opstat";
	if (skill == do_orange)
		return "do_orange";
	if (skill == do_order)
		return "do_order";
	if (skill == do_orders)
		return "do_orders";
	if (skill == do_ordertalk)
		return "do_ordertalk";
	if (skill == do_oset)
		return "do_oset";
	if (skill == do_ostat)
		return "do_ostat";
	if (skill == do_outcast)
		return "do_outcast";
	if (skill == do_overthrow)
		return "do_overthrow";
	if (skill == do_override)
		return "do_override";
	if (skill == do_owhere)
		return "do_owhere";
	if (skill == do_osay)
		return "do_osay";
	if (skill == do_pager)
		return "do_pager";
	if (skill == do_paint)
		return "do_paint";
	if (skill == do_pardon)
		return "do_pardon";
	if (skill == do_password)
		return "do_password";
	if (skill == do_peace)
		return "do_peace";
	if (skill == do_pick)
		return "do_pick";
	if (skill == do_poison_weapon)
		return "do_poison_weapon";
	if (skill == do_practice)
		return "do_practice";
	if (skill == do_prompt)
		return "do_prompt";
	if (skill == do_pull)
		return "do_pull";
	if (skill == do_punch)
		return "do_punch";
	if (skill == do_purge)
		return "do_purge";
	if (skill == do_push)
		return "do_push";
	if (skill == do_put)
		return "do_put";
	if (skill == do_qpset)
		return "do_qpset";
	if (skill == do_qpstat)
		return "do_qpstat";
	if (skill == do_quaff)
		return "do_quaff";
	if (skill == do_qui)
		return "do_qui";
	/*if ( skill == do_quickdraw )              return "do_quickdraw"; */
	if (skill == do_quit)
		return "do_quit";
	if (skill == do_rank)
		return "do_rank";
	if (skill == do_rassign)
		return "do_rassign";
	if (skill == do_rat)
		return "do_rat";
	if (skill == do_rdelete)
		return "do_rdelete";
	if (skill == do_reboo)
		return "do_reboo";
	if (skill == do_reboot)
		return "do_reboot";
	if (skill == do_recho)
		return "do_recho";
	if (skill == do_recite)
		return "do_recite";
	if (skill == do_redit)
		return "do_redit";
	if (skill == do_regoto)
		return "do_regoto";
	if (skill == do_rembounty)
		return "do_rembounty";
	if (skill == do_remweapon)
		return "do_remweapon";
	if (skill == do_remove)
		return "do_remove";
	if (skill == do_renameship)
		return "do_renameship";
	if (skill == do_rent)
		return "do_rent";
	if (skill == do_repair)
		return "do_repair";
	if (skill == do_repairset)
		return "do_repairset";
	if (skill == do_repairshops)
		return "do_repairshops";
	if (skill == do_repairstat)
		return "do_repairstat";
	if (skill == do_reply)
		return "do_reply";
	if (skill == do_report)
		return "do_report";
	if (skill == do_rescue)
		return "do_rescue";
	if (skill == do_reset)
		return "do_reset";
	if (skill == do_resetship)
		return "do_resetship";
	if (skill == do_rest)
		return "do_rest";
	if (skill == do_restore)
		return "do_restore";
	if (skill == do_restoretime)
		return "do_restoretime";
	if (skill == do_restrict)
		return "do_restrict";
	if (skill == do_retire)
		return "do_retire";
	if (skill == do_retran)
		return "do_retran";
	if (skill == do_return)
		return "do_return";
	if (skill == do_revert)
		return "do_revert";
	if (skill == do_rip)
		return "do_rip";
	if (skill == do_rlist)
		return "do_rlist";
	if (skill == do_rpedit)
		return "do_rpedit";
	if (skill == do_rpstat)
		return "do_rpstat";
	if (skill == do_rreset)
		return "do_rreset";
	if (skill == do_rset)
		return "do_rset";
	if (skill == do_rstat)
		return "do_rstat";
	if (skill == do_rumors)
		return "do_rumors";
	if (skill == do_rp)
		return "do_rp";
	if (skill == do_rpauthorize)
		return "do_rpauthorize";
	if (skill == do_research)
		return "do_research";
	if (skill == do_sacrifice)
		return "do_sacrifice";
	if (skill == do_save)
		return "do_save";
	if (skill == do_savearea)
		return "do_savearea";
	if (skill == do_say)
		return "do_say";
	if (skill == do_scan)
		return "do_scan";
	if (skill == do_score)
		return "do_score";
	if (skill == do_search)
		return "do_search";
	if (skill == do_sedit)
		return "do_sedit";
	if (skill == do_sell)
		return "do_sell";
	if (skill == do_set_boot_time)
		return "do_set_boot_time";
	if (skill == do_setclan)
		return "do_setclan";
	if (skill == do_setlevels)
		return "do_setlevels";
	if (skill == do_setship)
		return "do_setship";
	if (skill == do_setrestore)
		return "do_setrestore";
	if (skill == do_setslay)
		return "do_setslay";
	if (skill == do_setstarsystem)
		return "do_setstarsystem";
	if (skill == do_ships)
		return "do_ships";
	if (skill == do_shops)
		return "do_shops";
	if (skill == do_shopset)
		return "do_shopset";
	if (skill == do_shopstat)
		return "do_shopstat";
	if (skill == do_shout)
		return "do_shout";
	if (skill == do_shove)
		return "do_shove";
	if (skill == do_showclan)
		return "do_showclan";
	if (skill == do_showplanet)
		return "do_showplanet";
	if (skill == do_showship)
		return "do_showship";
	if (skill == do_showrestore)
		return "do_showrestore";
	if (skill == do_showslay)
		return "do_showslay";
	if (skill == do_showstarsystem)
		return "do_showstarsystem";
	if (skill == do_shutdow)
		return "do_shutdow";
	if (skill == do_shutdown)
		return "do_shutdown";
	if (skill == do_shutoff)
		return "do_shutoff";
	if (skill == do_silence)
		return "do_silence";
	if (skill == do_sit)
		return "do_sit";
	if (skill == do_sla)
		return "do_sla";
	if (skill == do_slay)
		return "do_slay";
	if (skill == do_sleep)
		return "do_sleep";
	if (skill == do_slist)
		return "do_slist";
	if (skill == do_slookup)
		return "do_slookup";
	if (skill == do_smoke)
		return "do_smoke";
	if (skill == do_snoop)
		return "do_snoop";
	if (skill == do_sober)
		return "do_sober";
	if (skill == do_socials)
		return "do_socials";
	if (skill == do_south)
		return "do_south";
	if (skill == do_southeast)
		return "do_southeast";
	if (skill == do_southwest)
		return "do_southwest";
	if (skill == do_speak)
		return "do_speak";
	if (skill == do_split)
		return "do_split";
	if (skill == do_sset)
		return "do_sset";
	if (skill == do_stand)
		return "do_stand";
	if (skill == do_stretch)
		return "do_stretch";
	if (skill == do_status)
		return "do_status";
	if (skill == do_starsystems)
		return "do_starsystems";
	if (skill == do_steal)
		return "do_steal";
	if (skill == do_stealth)
		return "do_stealth";
	if (skill == do_switch)
		return "do_switch";
	if (skill == do_study)
		return "do_study";
	if (skill == do_socialize)
		return "do_socialize";
	if (skill == do_swlist)
		return "do_swlist";
	if (skill == do_swcreate)
		return "do_swcreate";
	if (skill == do_swset)
		return "do_swset";
	if (skill == do_swstat)
		return "do_swstat";
	if (skill == do_tamp)
		return "do_tamp";
	if (skill == do_tell)
		return "do_tell";
	if (skill == do_time)
		return "do_time";
	if (skill == do_timecmd)
		return "do_timecmd";
	if (skill == do_title)
		return "do_title";
	if (skill == do_top20)
		return "do_top20";
	if (skill == do_track)
		return "do_track";
	if (skill == do_trajectory)
		return "do_trajectory";
	if (skill == do_transfer)
		return "do_transfer";
	if (skill == do_transship)
		return "do_transship";
	if (skill == do_trust)
		return "do_trust";
	if (skill == do_typo)
		return "do_typo";
	if (skill == do_unfoldarea)
		return "do_unfoldarea";
	if (skill == do_unhell)
		return "do_unhell";
	if (skill == do_unlock)
		return "do_unlock";
	if (skill == do_unsilence)
		return "do_unsilence";
	if (skill == do_up)
		return "do_up";
	if (skill == do_users)
		return "do_users";
	if (skill == do_value)
		return "do_value";
	if (skill == do_vassign)
		return "do_vassign";
	if (skill == do_visible)
		return "do_visible";
	if (skill == do_visitors)
		return "do_visitors";	/* added by Funf */
	if (skill == do_vnums)
		return "do_vnums";
	if (skill == do_vsearch)
		return "do_vsearch";
	if (skill == do_wake)
		return "do_wake";
	if (skill == do_wartalk)
		return "do_wartalk";
	if (skill == do_wear)
		return "do_wear";
	if (skill == do_weather)
		return "do_weather";
	if (skill == do_west)
		return "do_west";
	if (skill == do_where)
		return "do_where";
	if (skill == do_whisper)
		return "do_whisper";
	if (skill == do_who)
		return "do_who";
	if (skill == do_whois)
		return "do_whois";
	if (skill == do_webwho)
		return "do_webwho";
	if (skill == do_wimpy)
		return "do_wimpy";
	if (skill == do_wizhelp)
		return "do_wizhelp";
	if (skill == do_wizlist)
		return "do_wizlist";
	if (skill == do_wizlock)
		return "do_wizlock";
	if (skill == do_yell)
		return "do_yell";
	if (skill == do_zap)
		return "do_zap";
	if (skill == do_zones)
		return "do_zones";
	if (skill == do_zecho)
		return "do_zecho";
	return "reserved";
}

/*
 * Function used by qsort to sort skills
 */
int skill_comp(SKILLTYPE ** sk1, SKILLTYPE ** sk2)
{
	SKILLTYPE *skill1 = (*sk1);
	SKILLTYPE *skill2 = (*sk2);

	if (!skill1 && skill2)
		return 1;
	if (skill1 && !skill2)
		return -1;
	if (!skill1 && !skill2)
		return 0;
	if (skill1->type < skill2->type)
		return -1;
	if (skill1->type > skill2->type)
		return 1;
	return strcmp(skill1->name, skill2->name);
}

/*
 * Sort the skill table with qsort
 */
void sort_skill_table()
{
	log_string("Sorting skill table...");
	qsort(&skill_table[1], top_sn - 1, sizeof(SKILLTYPE *),
	      (int (*)(const void *, const void *))skill_comp);
}

/*
 * Write skill data to a file
 */
void fwrite_skill(FILE * fpout, SKILLTYPE * skill)
{
	SMAUG_AFF *aff;

	fprintf(fpout, "Name         %s~\n", skill->name);
	fprintf(fpout, "Type         %s\n", skill_tname[skill->type]);
	fprintf(fpout, "Flags        %d\n", skill->flags);
	if (skill->target)
		fprintf(fpout, "Target       %d\n", skill->target);
	if (skill->minimum_position)
		fprintf(fpout, "Minpos       %d\n", skill->minimum_position);
	if (skill->saves)
		fprintf(fpout, "Saves        %d\n", skill->saves);
	if (skill->slot)
		fprintf(fpout, "Slot         %d\n", skill->slot);
	if (skill->min_mana)
		fprintf(fpout, "Mana         %d\n", skill->min_mana);
	if (skill->beats)
		fprintf(fpout, "Rounds       %d\n", skill->beats);
	if (skill->guild != -1)
		fprintf(fpout, "Guild        %d\n", skill->guild);
	if (skill->skill_fun)
		fprintf(fpout, "Code         %s\n",
			skill_name(skill->skill_fun));
	else if (skill->spell_fun)
		fprintf(fpout, "Code         %s\n",
			spell_name(skill->spell_fun));
	fprintf(fpout, "Dammsg       %s~\n", skill->noun_damage);
	if (skill->msg_off && skill->msg_off[0] != '\0')
		fprintf(fpout, "Wearoff      %s~\n", skill->msg_off);

	if (skill->hit_char && skill->hit_char[0] != '\0')
		fprintf(fpout, "Hitchar      %s~\n", skill->hit_char);
	if (skill->hit_vict && skill->hit_vict[0] != '\0')
		fprintf(fpout, "Hitvict      %s~\n", skill->hit_vict);
	if (skill->hit_room && skill->hit_room[0] != '\0')
		fprintf(fpout, "Hitroom      %s~\n", skill->hit_room);

	if (skill->miss_char && skill->miss_char[0] != '\0')
		fprintf(fpout, "Misschar     %s~\n", skill->miss_char);
	if (skill->miss_vict && skill->miss_vict[0] != '\0')
		fprintf(fpout, "Missvict     %s~\n", skill->miss_vict);
	if (skill->miss_room && skill->miss_room[0] != '\0')
		fprintf(fpout, "Missroom     %s~\n", skill->miss_room);

	if (skill->die_char && skill->die_char[0] != '\0')
		fprintf(fpout, "Diechar      %s~\n", skill->die_char);
	if (skill->die_vict && skill->die_vict[0] != '\0')
		fprintf(fpout, "Dievict      %s~\n", skill->die_vict);
	if (skill->die_room && skill->die_room[0] != '\0')
		fprintf(fpout, "Dieroom      %s~\n", skill->die_room);

	if (skill->imm_char && skill->imm_char[0] != '\0')
		fprintf(fpout, "Immchar      %s~\n", skill->imm_char);
	if (skill->imm_vict && skill->imm_vict[0] != '\0')
		fprintf(fpout, "Immvict      %s~\n", skill->imm_vict);
	if (skill->imm_room && skill->imm_room[0] != '\0')
		fprintf(fpout, "Immroom      %s~\n", skill->imm_room);

	if (skill->dice && skill->dice[0] != '\0')
		fprintf(fpout, "Dice         %s~\n", skill->dice);
	if (skill->value)
		fprintf(fpout, "Value        %d\n", skill->value);
	if (skill->difficulty)
		fprintf(fpout, "Difficulty   %d\n", skill->difficulty);
	if (skill->participants)
		fprintf(fpout, "Participants %d\n", skill->participants);
	if (skill->components && skill->components[0] != '\0')
		fprintf(fpout, "Components   %s~\n", skill->components);
	if (skill->teachers && skill->teachers[0] != '\0')
		fprintf(fpout, "Teachers     %s~\n", skill->teachers);
	for (aff = skill->affects; aff; aff = aff->next)
		fprintf(fpout, "Affect       '%s' %d '%s' %d\n",
			aff->duration, aff->location, aff->modifier,
			aff->bitvector);
	if (skill->alignment)
		fprintf(fpout, "Alignment   %d\n", skill->alignment);

	if (skill->type != SKILL_HERB) {
		fprintf(fpout, "Minlevel     %d\n", skill->min_level);
	}
	fprintf(fpout, "End\n\n");
}

/*
 * Save the skill table to disk
 */
void save_skill_table()
{
	int x;
	FILE *fpout;

	if ((fpout = fopen(SKILL_FILE, "w")) == NULL) {
		bug("Cannot open skills.dat for writting", 0);
		perror(SKILL_FILE);
		return;
	}

	for (x = 0; x < top_sn; x++) {
		if (!skill_table[x]->name || skill_table[x]->name[0] == '\0')
			break;
		fprintf(fpout, "#SKILL\n");
		fwrite_skill(fpout, skill_table[x]);
	}
	fprintf(fpout, "#END\n");
	fclose(fpout);
}

/*
 * Save the herb table to disk
 */
void save_herb_table()
{
	int x;
	FILE *fpout;

	if ((fpout = fopen(HERB_FILE, "w")) == NULL) {
		bug("Cannot open herbs.dat for writting", 0);
		perror(HERB_FILE);
		return;
	}

	for (x = 0; x < top_herb; x++) {
		if (!herb_table[x]->name || herb_table[x]->name[0] == '\0')
			break;
		fprintf(fpout, "#HERB\n");
		fwrite_skill(fpout, herb_table[x]);
	}
	fprintf(fpout, "#END\n");
	fclose(fpout);
}

/*
 * Save the socials to disk
 */
void save_socials()
{
	FILE *fpout;
	SOCIALTYPE *social;
	int x;

	if ((fpout = fopen(SOCIAL_FILE, "w")) == NULL) {
		bug("Cannot open socials.dat for writting", 0);
		perror(SOCIAL_FILE);
		return;
	}

	for (x = 0; x < 27; x++) {
		for (social = social_index[x]; social; social = social->next) {
			if (!social->name || social->name[0] == '\0') {
				bug("Save_socials: blank social in hash bucket %d", x);
				continue;
			}
			fprintf(fpout, "#SOCIAL\n");
			fprintf(fpout, "Name        %s~\n", social->name);
			if (social->char_no_arg)
				fprintf(fpout, "CharNoArg   %s~\n",
					social->char_no_arg);
			else
				bug("Save_socials: NULL char_no_arg in hash bucket %d", x);
			if (social->others_no_arg)
				fprintf(fpout, "OthersNoArg %s~\n",
					social->others_no_arg);
			if (social->char_found)
				fprintf(fpout, "CharFound   %s~\n",
					social->char_found);
			if (social->others_found)
				fprintf(fpout, "OthersFound %s~\n",
					social->others_found);
			if (social->vict_found)
				fprintf(fpout, "VictFound   %s~\n",
					social->vict_found);
			if (social->char_auto)
				fprintf(fpout, "CharAuto    %s~\n",
					social->char_auto);
			if (social->others_auto)
				fprintf(fpout, "OthersAuto  %s~\n",
					social->others_auto);
			fprintf(fpout, "End\n\n");
		}
	}
	fprintf(fpout, "#END\n");
	fclose(fpout);
}

int get_skill(char *skilltype)
{
	if (!str_cmp(skilltype, "Spell"))
		return SKILL_SPELL;
	if (!str_cmp(skilltype, "Skill"))
		return SKILL_SKILL;
	if (!str_cmp(skilltype, "Weapon"))
		return SKILL_WEAPON;
	if (!str_cmp(skilltype, "Tongue"))
		return SKILL_TONGUE;
	if (!str_cmp(skilltype, "Herb"))
		return SKILL_HERB;
	return SKILL_UNKNOWN;
}

/*
 * Save the commands to disk
 */
void save_commands()
{
	CMDTYPE * command;
	DATA_ENTRY * entry;
	DATA_CONN * conn;
	int x;

	if ((conn = dopen()) == NULL) 
	{
		bug("Cannot connect to database", 0);
		return;
	}

	for (x = 0; x < 126; x++) {
		for (command = command_hash[x]; command; command = command->next) 
		{
			if (!command->name || command->name[0] == '\0') 
			{
				bug("Save_commands: blank command in hash bucket %d", x);
				continue;
			}

			CREATE(entry, DATA_ENTRY, 1);
			dentry_init(entry);
			dwrite_string("_id", command->id, entry);
			dwrite_string("Name", command->name, entry);
			dwrite_string("Code", skill_name(command->do_fun), entry);
			dwrite_number("Position", command->position, entry);
			dwrite_number("Level", command->level, entry);
			dwrite_number("Log", command->log, entry);
			dwrite_number("Ooc", command->ooc, entry);
			dentry_finalize(entry);
			dentry_save(entry, DSET_COMMANDS, conn);
			dentry_destroy(entry);
			DISPOSE(entry);
		}
	}

	dclose(conn);
}

SKILLTYPE *fread_skill(FILE * fp)
{
	char buf[MAX_STRING_LENGTH];
	char *word;
	bool fMatch;
	SKILLTYPE *skill;

	CREATE(skill, SKILLTYPE, 1);

	skill->guild = -1;

	for (;;) {
		word = feof(fp) ? "End" : fread_word(fp);
		fMatch = FALSE;

		switch (UPPER(word[0])) {
		case '*':
			fMatch = TRUE;
			fread_to_eol(fp);
			break;

		case 'A':
			KEY("Alignment", skill->alignment, fread_number(fp));
			if (!str_cmp(word, "Affect")) {
				SMAUG_AFF *aff;

				CREATE(aff, SMAUG_AFF, 1);
				aff->duration = str_dup(fread_word(fp));
				aff->location = fread_number(fp);
				aff->modifier = str_dup(fread_word(fp));
				aff->bitvector = fread_number(fp);
				aff->next = skill->affects;
				skill->affects = aff;
				fMatch = TRUE;
				break;
			}
			break;

		case 'C':
			if (!str_cmp(word, "Code")) {
				SPELL_FUN *spellfun;
				DO_FUN *dofun;
				char *w = fread_word(fp);

				fMatch = TRUE;
				if ((spellfun =
				     spell_function(w)) != spell_notfound)
					skill->spell_fun = spellfun;
				else if ((dofun =
					  skill_function(w)) != skill_notfound)
					skill->skill_fun = dofun;
				else {
					sprintf(buf,
						"fread_skill: unknown skill/spell %s",
						w);
					bug(buf, 0);
					skill->spell_fun = spell_null;
				}
				break;
			}
			KEY("Code", skill->spell_fun,
			    spell_function(fread_word(fp)));
			KEY("Components", skill->components,
			    fread_string_nohash(fp));
			break;

		case 'D':
			KEY("Dammsg", skill->noun_damage,
			    fread_string_nohash(fp));
			KEY("Dice", skill->dice, fread_string_nohash(fp));
			KEY("Diechar", skill->die_char,
			    fread_string_nohash(fp));
			KEY("Dieroom", skill->die_room,
			    fread_string_nohash(fp));
			KEY("Dievict", skill->die_vict,
			    fread_string_nohash(fp));
			KEY("Difficulty", skill->difficulty, fread_number(fp));
			break;

		case 'E':
			if (!str_cmp(word, "End"))
				return skill;
			break;

		case 'F':
			KEY("Flags", skill->flags, fread_number(fp));
			break;

		case 'G':
			KEY("Guild", skill->guild, fread_number(fp));
			break;

		case 'H':
			KEY("Hitchar", skill->hit_char,
			    fread_string_nohash(fp));
			KEY("Hitroom", skill->hit_room,
			    fread_string_nohash(fp));
			KEY("Hitvict", skill->hit_vict,
			    fread_string_nohash(fp));
			break;

		case 'I':
			KEY("Immchar", skill->imm_char,
			    fread_string_nohash(fp));
			KEY("Immroom", skill->imm_room,
			    fread_string_nohash(fp));
			KEY("Immvict", skill->imm_vict,
			    fread_string_nohash(fp));
			break;

		case 'M':
			KEY("Mana", skill->min_mana, fread_number(fp));
			KEY("Minlevel", skill->min_level, fread_number(fp));
			KEY("Minpos", skill->minimum_position,
			    fread_number(fp));
			KEY("Misschar", skill->miss_char,
			    fread_string_nohash(fp));
			KEY("Missroom", skill->miss_room,
			    fread_string_nohash(fp));
			KEY("Missvict", skill->miss_vict,
			    fread_string_nohash(fp));
			break;

		case 'N':
			KEY("Name", skill->name, fread_string_nohash(fp));
			break;

		case 'P':
			KEY("Participants", skill->participants,
			    fread_number(fp));
			break;

		case 'R':
			KEY("Rounds", skill->beats, fread_number(fp));
			break;

		case 'S':
			KEY("Slot", skill->slot, fread_number(fp));
			KEY("Saves", skill->saves, fread_number(fp));
			break;

		case 'T':
			KEY("Target", skill->target, fread_number(fp));
			KEY("Teachers", skill->teachers,
			    fread_string_nohash(fp));
			KEY("Type", skill->type, get_skill(fread_word(fp)));
			break;

		case 'V':
			KEY("Value", skill->value, fread_number(fp));
			break;

		case 'W':
			KEY("Wearoff", skill->msg_off, fread_string_nohash(fp));
			break;
		}

		if (!fMatch) {
			sprintf(buf, "Fread_skill: no match: %s", word);
			bug(buf, 0);
		}
	}
}

void load_skill_table()
{
	FILE *fp;

	if ((fp = fopen(SKILL_FILE, "r")) != NULL) {
		top_sn = 0;
		for (;;) {
			char letter;
			char *word;

			letter = fread_letter(fp);
			if (letter == '*') {
				fread_to_eol(fp);
				continue;
			}

			if (letter != '#') {
				bug("Load_skill_table: # not found.", 0);
				break;
			}

			word = fread_word(fp);
			if (!str_cmp(word, "SKILL")) {
				if (top_sn >= MAX_SKILL) {
					bug("load_skill_table: more skills than MAX_SKILL %d", MAX_SKILL);
					fclose(fp);
					return;
				}
				skill_table[top_sn++] = fread_skill(fp);
				continue;
			} else if (!str_cmp(word, "END"))
				break;
			else {
				bug("Load_skill_table: bad section.", 0);
				continue;
			}
		}
		fclose(fp);
	} else {
		bug("Cannot open skills.dat", 0);
		exit(0);
	}
}

void load_herb_table()
{
	FILE *fp;

	if ((fp = fopen(HERB_FILE, "r")) != NULL) {
		top_herb = 0;
		for (;;) {
			char letter;
			char *word;

			letter = fread_letter(fp);
			if (letter == '*') {
				fread_to_eol(fp);
				continue;
			}

			if (letter != '#') {
				bug("Load_herb_table: # not found.", 0);
				break;
			}

			word = fread_word(fp);
			if (!str_cmp(word, "HERB")) {
				if (top_herb >= MAX_HERB) {
					bug("load_herb_table: more herbs than MAX_HERB %d", MAX_HERB);
					fclose(fp);
					return;
				}
				herb_table[top_herb++] = fread_skill(fp);
				if (herb_table[top_herb - 1]->slot == 0)
					herb_table[top_herb - 1]->slot =
					    top_herb - 1;
				continue;
			} else if (!str_cmp(word, "END"))
				break;
			else {
				bug("Load_herb_table: bad section.", 0);
				continue;
			}
		}
		fclose(fp);
	} else {
		bug("Cannot open herbs.dat", 0);
		exit(0);
	}
}

void fread_social(FILE * fp)
{
	char buf[MAX_STRING_LENGTH];
	char *word;
	bool fMatch;
	SOCIALTYPE *social;

	CREATE(social, SOCIALTYPE, 1);

	for (;;) {
		word = feof(fp) ? "End" : fread_word(fp);
		fMatch = FALSE;

		switch (UPPER(word[0])) {
		case '*':
			fMatch = TRUE;
			fread_to_eol(fp);
			break;

		case 'C':
			KEY("CharNoArg", social->char_no_arg,
			    fread_string_nohash(fp));
			KEY("CharFound", social->char_found,
			    fread_string_nohash(fp));
			KEY("CharAuto", social->char_auto,
			    fread_string_nohash(fp));
			break;

		case 'E':
			if (!str_cmp(word, "End")) {
				if (!social->name) {
					bug("Fread_social: Name not found", 0);
					free_social(social);
					return;
				}
				if (!social->char_no_arg) {
					bug("Fread_social: CharNoArg not found",
					    0);
					free_social(social);
					return;
				}
				add_social(social);
				return;
			}
			break;

		case 'N':
			KEY("Name", social->name, fread_string_nohash(fp));
			break;

		case 'O':
			KEY("OthersNoArg", social->others_no_arg,
			    fread_string_nohash(fp));
			KEY("OthersFound", social->others_found,
			    fread_string_nohash(fp));
			KEY("OthersAuto", social->others_auto,
			    fread_string_nohash(fp));
			break;

		case 'V':
			KEY("VictFound", social->vict_found,
			    fread_string_nohash(fp));
			break;
		}

		if (!fMatch) {
			sprintf(buf, "Fread_social: no match: %s", word);
			bug(buf, 0);
		}
	}
}

void load_socials()
{
	FILE *fp;

	if ((fp = fopen(SOCIAL_FILE, "r")) != NULL) {
		top_sn = 0;
		for (;;) {
			char letter;
			char *word;

			letter = fread_letter(fp);
			if (letter == '*') {
				fread_to_eol(fp);
				continue;
			}

			if (letter != '#') {
				bug("Load_socials: # not found.", 0);
				break;
			}

			word = fread_word(fp);
			if (!str_cmp(word, "SOCIAL")) {
				fread_social(fp);
				continue;
			} else if (!str_cmp(word, "END"))
				break;
			else {
				bug("Load_socials: bad section.", 0);
				continue;
			}
		}
		fclose(fp);
	} else {
		bug("Cannot open socials.dat", 0);
		exit(0);
	}
}

void dread_command(DATA_ENTRY * entry)
{
	char buf[MAX_STRING_LENGTH];
	char *word;
	bool fMatch;
	CMDTYPE *command;
	
	CREATE(command, CMDTYPE, 1);
	command->id = dread_string("_id", entry);
	command->name = dread_string("Name", entry);
	command->do_fun = skill_function(dread_word("Code", entry));
	command->position = dread_number("Position", entry);
	command->level = dread_number("Level", entry);
	command->log = dread_number("Log", entry);
	command->ooc = dread_number("Ooc", entry);	
	if (!command->name) {
		bug("Dread_command: Name not found", 0);
		free_command(command);
		return;
	}

	if (!command->do_fun) {
		bug("Dread_command: Function not found", 0);
		free_command(command);
		return;
	}

	if (!command->ooc)
		command->ooc = 0;

	add_command(command);
}

/*void fread_command(FILE * fp)
{
	char buf[MAX_STRING_LENGTH];
	char *word;
	bool fMatch;
	CMDTYPE *command;
	
	CREATE(command, CMDTYPE, 1);

	for (;;) {
		word = feof(fp) ? "End" : fread_word(fp);
		fMatch = FALSE;

		switch (UPPER(word[0])) {
		case '*':
			fMatch = TRUE;
			fread_to_eol(fp);
			break;

		case 'C':
			KEY("Code", command->do_fun,
			    skill_function(fread_word(fp)));
			break;

		case 'E':
			if (!str_cmp(word, "End")) {
				if (!command->name) {
					bug("Fread_command: Name not found", 0);
					free_command(command);
					return;
				}
				if (!command->do_fun) {
					bug("Fread_command: Function not found",
					    0);
					free_command(command);
					return;
				}
				if (!command->ooc)
					command->ooc = 0;
				add_command(command);
				return;
			}
			break;

		case 'L':
			KEY("Level", command->level, fread_number(fp));
			KEY("Log", command->log, fread_number(fp));
			break;

		case 'N':
			KEY("Name", command->name, fread_string_nohash(fp));
			break;
		case 'O':
			KEY("Ooc", command->ooc, fread_number(fp));
			break;
		case 'P':
			KEY("Position", command->position, fread_number(fp));
			break;
		}

		if (!fMatch) {
			sprintf(buf, "Fread_command: no match: %s", word);
			bug(buf, 0);
		}
	}
}*/

void load_commands()
{
//	FILE *fp;
	DATA_CONN *dc;
	DATA_CURSOR *cursor;
	DATA_ENTRY *entry;
	
	if ((dc = dopen()) == NULL) {
		bug("load_commands: cannot connect to database", 0);
		exit(0);
	}

	cursor = dcursor_open(DSET_COMMANDS, dc);
	while(dcursor_next(cursor)) {
		entry = dcursor_entry(cursor);
		dread_command(entry);
	}

	if(!dcursor_close(cursor)) {
		bug("load_commands: cannot close cursor", 0);
	}

	if(!dclose(dc)) {
		bug("load_commands: cannot close connection", 0);
	}
	
/*	if ((fp = fopen(COMMAND_FILE, "r")) != NULL) {
		top_sn = 0;
		for (;;) {
			char letter;
			char *word;

			letter = fread_letter(fp);
			if (letter == '*') {
				fread_to_eol(fp);
				continue;
			}

			if (letter != '#') {
				bug("Load_commands: # not found.", 0);
				break;
			}

			word = fread_word(fp);
			if (!str_cmp(word, "COMMAND")) {
				fread_command(fp);
				continue;
			} else if (!str_cmp(word, "END"))
				break;
			else {
				bug("Load_commands: bad section.", 0);
				continue;
			}
		}
		fclose(fp);
	} else {
		bug("Cannot open commands.dat", 0);
		exit(0);
	}*/
}
