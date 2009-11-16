/***************************************************************************
 *  Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,	   *
 *  Michael Seifert, Hans Henrik St{rfeldt, Tom Madsen, and Katja Nyboe.   *
 *									   *
 *  Merc Diku Mud improvments copyright (C) 1992, 1993 by Michael	   *
 *  Chastain, Michael Quan, and Mitchell Tse.				   *
 *									   *
 *  In order to use any part of this Merc Diku Mud, you must comply with   *
 *  both the original Diku license in 'license.doc' as well the Merc	   *
 *  license in 'license.txt'.  In particular, you may not remove either of *
 *  these copyright notices.						   *
 *									   *
 *  Much time and thought has gone into this software and you are	   *
 *  benefitting.  We hope that you share your changes too.  What goes	   *
 *  around, comes around.						   *
 ***************************************************************************/

#if defined(macintosh)
#include <types.h>
#else
#include <sys/types.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "merc.h"

#define MAX_SLAY_TYPES 4
#define MONK_AUTODROP 12
#define BERSERK_HITS 4

/*
 * Local functions.
 */
void  autodrop        args((CHAR_DATA *ch));
void  dropinvis       args((CHAR_DATA *ch));
bool  check_dodge     args((CHAR_DATA *ch, CHAR_DATA *victim, int dt));
bool  check_parry     args((CHAR_DATA *ch, CHAR_DATA *victim, int dt));
void  dam_message     args((CHAR_DATA *ch, CHAR_DATA *victim, int dam, int dt));
void  death_cry       args((CHAR_DATA *ch));
void  group_gain      args((CHAR_DATA *ch, CHAR_DATA *victim));
int   xp_compute      args((CHAR_DATA *gch, CHAR_DATA *victim));
void  set_fighting    args((CHAR_DATA *ch, CHAR_DATA *victim));
bool  can_counter     args((CHAR_DATA *ch));
bool  can_bypass      args((CHAR_DATA *ch, CHAR_DATA *victim));
int   dambonus        args((CHAR_DATA *ch, CHAR_DATA *victim, int dam, int stance));
void  decap_message   args((CHAR_DATA *ch, CHAR_DATA *victim));
void  angel_eye       args((CHAR_DATA *ch, CHAR_DATA *victim, int dam));
void  ragnarokdecap   args((CHAR_DATA *ch, CHAR_DATA *victim));
void  total_dam       args((CHAR_DATA *ch, CHAR_DATA *victim));
int   blood_ratio     args((CHAR_DATA *ch));
bool kingdom_fair_fight args ( (CHAR_DATA *ch, CHAR_DATA *victim) );
//bool worldpk = FALSE;
//bool worldtest = FALSE;
/*Extern functions*/
double mathproxy_log(double dArg);
/*
 * Control the fights going on.
 * Called periodically by update_handler.
 */
void violence_update( void )
{
    CHAR_DATA *ch;
    CHAR_DATA *ch_next;
    CHAR_DATA *emb;
    CHAR_DATA *victim = NULL;
    CHAR_DATA *rch;
    CHAR_DATA *rch_next;
    CHAR_DATA *mount;
    int chance = 0;

    chance = number_percent();

    for ( ch = char_list; ch != NULL; ch = ch->next )
    {
        if(IS_CLASS(ch, CLASS_DEMON))

            victim = ch->blinkykill;
        if (victim != NULL)
        {

            if (IS_SET(ch->flag3, AFF3_BLINK_1ST_RD))
            {
                REMOVE_BIT(ch->flag3, AFF3_BLINK_1ST_RD);
                SET_BIT(ch->flag3, AFF3_BLINK_2ND_RD);
            }
            else if (IS_SET(ch->flag3, AFF3_BLINK_2ND_RD))
            {
                REMOVE_BIT(ch->flag3, AFF3_BLINK_2ND_RD);
                REMOVE_BIT(ch->flag3, AFF3_BLINK_1ST_RD);
                /* Has the victim left? */
                if (victim->in_room != ch->in_room)
                {
                    SET_BIT(ch->affected_by2,EXTRA_BLINKY);
                    victim = NULL;
                    act("$n pops back into existence.", ch, NULL, NULL, TO_ROOM);
                    stc("You pop back into existence.\n\r", ch);
                    stc("Your victim is no longer in the room.\n\r", ch);
                    break;
                }
                SET_BIT(ch->affected_by2,EXTRA_BLINKY);
                act("$n pops back into existence next to $N.", ch,NULL,victim, TO_NOTVICT);
                act("You pop back into existence next to $N.", ch, NULL,victim, TO_CHAR);
                act("$n pops back into existence next to you.", ch, NULL,victim, TO_VICT);
                if (!is_safe(ch,victim))
                {
                    if (ch->hit > 0)
                    {
                        set_fighting(ch, victim);
                        do_say(ch, "Muhahahahahaha");
                        multi_hit(ch, victim, gsn_blinky);
                        //        multi_hit(ch, victim, gsn_blinky);
                        //        multi_hit(ch, victim, gsn_blinky);
                        multi_hit(ch,victim, gsn_blinky);
                        if ( number_range(1,10) < 2 )
                            multi_hit(ch, victim, gsn_blinky);
                        if (IS_SET(ch->newbits, THIRD_HAND) && get_eq_char(ch, WEAR_THIRD) != NULL && number_range(1,10) < 2)
                            multi_hit(ch, victim, gsn_blinky);
                        if (IS_SET(ch->newbits, FOURTH_HAND) && get_eq_char(ch, WEAR_FOURTH) != NULL && number_range(1,10) < 2)
                            multi_hit(ch, victim, gsn_blinky);
                        update_pos(victim);
                    }
                }
            }
        }                                                   /*end of blink*/
        ch_next = ch->next;
        if (ch->fight_timer > 0)
        {
            if(ch->fight_timer > 50)
            {
                ch->fight_timer = 50;
            }
            else
            {
                ch->fight_timer--;
            }
        }

        if (ch->rage > 0 && IS_CLASS(ch, CLASS_DEMON)) ch->rage--;
        if ( ch->embracing != NULL )
        {
            emb = ch->embracing;
            if ( emb == NULL ) ch->embracing = NULL;
            if ( ch->in_room != emb->in_room )
            {
                if ( emb->embraced != NULL )
                {
                    emb->embraced = NULL;
                    ch->embracing = NULL;
                }
                else ch->embracing = NULL;
            }
        }
        if ( ch->embraced != NULL )
        {
            emb = ch->embraced;
            if ( emb == NULL ) ch->embraced = NULL;
            if ( ch->in_room != emb->in_room )
            {
                if ( emb->embracing != NULL )
                {
                    emb->embracing = NULL;
                    ch->embraced   = NULL;
                }
                else ch->embraced = NULL;
            }
        }
        // Dokuyake affects - Beorn
        if ( IS_SET(ch->ninjacrap, NINJA_VENOM) )
        {
            ch->hit -=  number_range( 1000,5000 );
            stc("You scream in agony as the venom sears through your veins.\n\r",ch);
            act("$n screams in agony as the venom sears their veins.",ch,NULL,NULL,TO_ROOM);

            if ( number_range(1,10) == 1 )
            {
                stc("You sigh in relief as the venom fades away.\n\r",ch);
                act("$n sighs in relief as the venom fades away.",ch,NULL,NULL,TO_ROOM);
                REMOVE_BIT(ch->ninjacrap, NINJA_VENOM);
            }
            else
            {
                ch->hit -=  number_range( 1000,5000 );
                stc("You scream in agony as the venom sears through your veins.\n\r",ch);
                act("$n screams in agony as the venom sears through their veins.",ch,NULL,NULL,TO_ROOM);
            }
        }

        if ( IS_SET(ch->monkstuff, MONK_DEATH) )
        {
            if ( ch->hit > (ch->max_hit /2 ) )
            {
                ch->hit -=  number_range( 50,200 );
                stc("Your writhe in agony as magical energies tear you asunder.\n\r",ch);
                act("$n writhes in agony as magical forces tear apart $s body.",ch,NULL,NULL,TO_ROOM);
            }
            else
            {
                if ( number_range(1,2) == 1 )
                {
                    stc("You feel the magical forces leave your body.\n\r",ch);
                    act("The magical forces leave $n's body.",ch,NULL,NULL,TO_ROOM);
                    REMOVE_BIT(ch->monkstuff, MONK_DEATH);
                }
                else
                {
                    ch->hit -=  number_range( 50,200 );
                    stc("Your writhe in agony as magical energies tear you asunder.\n\r",ch);
                    act("$n writhes in agony as magical forces tear apart $s body.",ch,NULL,NULL,TO_ROOM);
                }
            }
        }
        if (IS_SET(ch->monkstuff, MONK_HEAL))
        {
            if (ch->hit < (ch->max_hit /2 ) && ch->hit > 0)
            {
                if (ch->hit < ch->max_hit) ch->hit +=  number_range( 200,400 );
                if ( ch->move < ch->max_move ) ch->move += number_range( 175,400 );
                stc("Your body emits glowing sparks.\n\r",ch);
                act("$n's body emits glowing sparks and fizzes.",ch,NULL,NULL,TO_ROOM);
            }
            else
            {
                if ( number_range( 1,2 ) == 1 )
                {
                    stc("The sparks fizzle and die.\n\r",ch);
                    act("The sparks around $n's body fizzle and die.",ch,NULL,NULL,TO_ROOM);
                    REMOVE_BIT(ch->monkstuff, MONK_HEAL);
                }
                else
                {
                    if ( ch->hit < ch->max_hit )
                        ch->hit +=  number_range( 200,400 );
                    if ( ch->move < ch->max_move )
                        ch->move += number_range( 175,400 );
                    stc("Your body emits glowing sparks.\n\r",ch);
                    act("$n's body emits glowing sparks and fizzes.",ch,NULL,NULL,TO_ROOM);
                }
            }
        }

        /*
         * Bloodrage
         */
        /*    if ( !IS_NPC(ch) && ch->pcdata->bloodpower[1] > 0 )
            if ( --ch->pcdata->bloodpower[1] == 0 )
                stcf(ch,"#rYour #RBloodrage#r has subsided.#n\n\r");*/

        if ((victim = ch->fighting ) == NULL || ch->in_room == NULL) continue;

        /*
         * Fix that nasty dupe bug... Devik
         */
        if ( ch->in_room != victim->in_room ) { ch->fighting = NULL; update_pos(ch); continue; }

        if (!IS_NPC(ch) && !IS_NPC(victim) && !is_safe(ch, victim) &&!is_safe(victim,ch))
        {
            if (ch->fight_timer < 10) ch->fight_timer = 10;
            else if (ch->fight_timer < 25) ch->fight_timer += 3;
        }
        if (IS_AWAKE(ch) && IS_AWAKE(victim) && ch->in_room == victim->in_room) multi_hit( ch, victim, TYPE_UNDEFINED );
        else stop_fighting(ch, FALSE);
        if ((victim = ch->fighting ) == NULL) continue;
        //MILITARY FIGHT REGEN to be done
        if ( IS_CLASS(ch,CLASS_RUNEMASTER) && is_rune_robe(ch,"0000000x0") )
        {
            stcf(ch,"#LYour robes flash with #7white light#L. *(#72500#L)*#n\n\r");
            act("#L%n's robes flash with #7white light#L.#n",ch,NULL,NULL,TO_ROOM);
            ch->hit += 2500;
            if ( ch->hit > ch->max_hit ) ch->hit = ch->max_hit;
        }

        /*	
         * Fun for the whole family!
         */
        for ( rch = ch->in_room->people; rch != NULL; rch = rch_next )
        {
            rch_next = rch->next_in_room;
            if ( IS_AWAKE(rch) && rch->fighting == NULL )
            {
                /*
                 * Mount's auto-assist their riders and vice versa.
                 */
                if ( (mount = rch->mount) != NULL )
                {
                    if (mount == ch) multi_hit( rch, victim, TYPE_UNDEFINED );
                    continue;
                }
                /*
                 * PC's auto-assist others in their group.
                 */
                if ( !IS_NPC(ch) || IS_AFFECTED(ch, AFF_CHARM) )
                {
                    if ((!IS_NPC(rch) || IS_AFFECTED(rch, AFF_CHARM) ) && is_same_group(ch, rch))
                        multi_hit( rch, victim, TYPE_UNDEFINED );
                    continue;
                }
                /*
                 * NPC's assist NPC's of same type or 12.5% chance regardless.
                 */
                if ( IS_NPC(rch) && !IS_AFFECTED(rch, AFF_CHARM) )
                {
                    if (rch->pIndexData == ch->pIndexData || number_bits(3) == 0)
                    {
                        CHAR_DATA *vch;
                        CHAR_DATA *target;
                        int number;

                        target = NULL;
                        number = 0;
                        for ( vch = ch->in_room->people; vch; vch = vch->next )
                        {
                            if (can_see(rch,vch) && is_same_group(vch, victim) && number_range(0,number)==0)
                            {
                                target = vch;
                                number++;
                            }
                        }
                        if ( target != NULL )
                            multi_hit( rch, target, TYPE_UNDEFINED );
                    }
                }
            }
        }

    }
    return;
}

void total_dam( CHAR_DATA *ch, CHAR_DATA *victim )
{
    char buf[MAX_STRING_LENGTH];

    if (!IS_NPC(ch) && IS_SET(ch->act, PLR_BRIEF5))
    {
        if (ch->pcdata->briefhit)
        {
            sprintf(buf,"You hit %s a total of %d time%c #r[#l**#R%d#l**#r]#n\n\r",
                PERS(victim, ch),
                ch->pcdata->briefhit,
                (ch->pcdata->briefhit == 1 ? ' ' : 's'),
                ch->pcdata->briefdam );
            send_to_char(buf,ch);
            ch->pcdata->briefhit = 0; ch->pcdata->briefdam = 0;
        }
    }

    if (!IS_NPC(victim) && IS_SET(victim->act, PLR_BRIEF5))
    {
        if (victim->pcdata->briefthit)
        {
            sprintf(buf,"%s hits you a total of %d time%c #r[#l**#R%d#l**#r]#n\n\r",
                PERS(ch, victim),
                victim->pcdata->briefthit,
                (victim->pcdata->briefthit == 1 ? ' ' : 's'),
                victim->pcdata->brieftdam );
            send_to_char(buf,victim);
            victim->pcdata->briefthit = 0; victim->pcdata->brieftdam = 0;
        }
    }
}

bool MultiHit = 0;

/*
 * Do one group of attacks.
 */
void multi_hit( CHAR_DATA *ch, CHAR_DATA *victim, int dt )
{
    OBJ_DATA *wield;
    OBJ_DATA *wield1;
    OBJ_DATA *wield2;
    OBJ_DATA *wield3;
    OBJ_DATA *wield4;
    char buf[MAX_STRING_LENGTH];
    int sn, level, chance, unarmed, maxcount,countup,x,wieldie,i;
    AFFECT_DATA af;
    int wieldorig = 0;
    int wieldtype = 0;

    if ( ch->position < POS_SLEEPING ) return;
    if ( IS_CREATOR(ch) || IS_CLASS(ch, CLASS_MONK) )
    {
        if (!IS_NPC(ch) && wieldorig == 0 )
        {
            countup = number_range(0,2);
            if (countup == 0) unarmed = number_range(0,3);
            else if (countup == 1) unarmed = number_range(4,7);
            else unarmed = number_range(0,7);
            if (!IS_NPC(ch) && ch->cmbt[unarmed] != 0 && wieldorig == 0)
                fightaction(ch,victim,ch->cmbt[unarmed], dt, wieldtype);
        }
    }
    if ( dt == gsn_headbutt )
    {
        one_hit( ch, victim, dt, 1 );
        return;
    }
    if ( dt == gsn_hooves )
    {
        one_hit( ch, victim, dt, 1 );
        one_hit( ch, victim, dt, 1 );
        if (number_range(1,3) != 2)
        {
            one_hit( ch, victim, dt, 1 );
            one_hit( ch, victim, dt, 1 );
        }
        return;
    }
    if(IS_CLASS(ch,CLASS_WRAITH))
    {
        one_hit(ch,victim,dt,1);
        one_hit(ch,victim,dt,1);
        one_hit(ch,victim,dt,1);
    }
    if (dt == gsn_shiroken)
    {
        for(i = number_range(3,5);i>0;i--)
            one_hit(ch, victim, dt, 1);
        if(ch->pcdata->powers[NPOWER_NINGENNO] >=4)
            spell_poison(gsn_poison,(ch->level*number_range(50,60)),ch,victim);
        return;
    }
    if ( dt == gsn_laserbeam )
    {
        one_hit(ch,victim,dt,1);
        one_hit(ch,victim,dt,1);
        one_hit(ch,victim,dt,1);
        one_hit(ch,victim,dt,1);
        one_hit(ch,victim,dt,1);
        return;
    }

    if ( dt == gsn_elvenmagic )
    {
        one_hit(ch,victim,dt,1);
        one_hit(ch,victim,dt,1);
        one_hit(ch,victim,dt,1);
        one_hit(ch,victim,dt,1);
        return;
    }

    if ( dt == gsn_ancestorswill )
    {
        one_hit(ch,victim,dt,1);
        one_hit(ch,victim,dt,1);
        one_hit(ch,victim,dt,1);
        return;
    }

    if ( dt == gsn_firearrow )
    {
        one_hit(ch,victim,dt,1);
        one_hit(ch,victim,dt,1);
        return;
    }

    if ( dt == gsn_icearrow )
    {
        one_hit(ch,victim,dt,1);
        one_hit(ch,victim,dt,1);
        return;
    }

    if ( dt == gsn_lightningarrow )
    {
        one_hit(ch,victim,dt,1);
        one_hit(ch,victim,dt,1);
        return;
    }

    if ( dt == gsn_laser )
    {
        one_hit(ch,victim,dt,1);
        one_hit(ch,victim,dt,1);
        return;
    }
    if ( dt == gsn_stinger )
    {
        one_hit(ch,victim,dt,1);
        one_hit(ch,victim,dt,1);
        return;
    }
    if (dt == gsn_claws)
    {
        one_hit( ch, victim, dt, 1 );
        one_hit( ch, victim, dt, 1 );
        return;
    }
    if (dt == gsn_tentacle)
    {
        int x;

        act("You shoot forth a shower of tentacles to destroy $N.", ch, NULL, victim, TO_CHAR);
        act("$n shoots forth a shower of tentacles aiming straight at you.", ch, NULL, victim, TO_VICT);
        if (!IS_SET(ch->act, PLR_BRIEF6))
            act("$n shoots forth a shower of tentacles striking $N.", ch, NULL, victim, TO_NOTVICT);

        x = number_range(2, 5);
        while (--x >= 0) one_hit(ch, victim, dt, 1);
        return;
    }
    if ( dt == gsn_fangs )
    {
        one_hit( ch, victim, dt, 1 );
        return;
    }
    if ( dt== gsn_quills)
    {
        if ( (IS_CLASS(ch,CLASS_WEREWOLF)) && (ch->power[DISC_WERE_PAIN] > 9))
        {
            one_hit(ch,victim,dt,1);
            one_hit(ch,victim,dt,1);
            one_hit(ch,victim,dt,1);
            one_hit(ch,victim,dt,1);
            one_hit(ch,victim,dt,1);
        }
        one_hit(ch,victim,dt,1);
        return;
    }
    if (dt == gsn_darktendrils && IS_CLASS(ch, CLASS_DROW))
    {
        one_hit(ch,victim,dt,1);
        one_hit(ch,victim,dt,1);
        one_hit(ch,victim,dt,1);
        one_hit(ch,victim,dt,1);
        return;
    }
    if (dt == gsn_deathaura && IS_CLASS(ch, CLASS_WRAITH))
    {
        one_hit(ch,victim,dt,1);
        one_hit(ch,victim,dt,1);
        one_hit(ch,victim,dt,1);
        one_hit(ch,victim,dt,1);
        one_hit(ch,victim,dt,1);
        one_hit(ch,victim,dt,1);
        one_hit(ch,victim,dt,1);
        one_hit(ch,victim,dt,1);
        return;
    }
    if ( dt==gsn_venomtong)
    {
        one_hit(ch,victim,dt,1);
        return;
    }
    if ( dt==gsn_spiketail)
    {
        one_hit(ch,victim,dt,1);
        return;
    }
    if ( dt==gsn_badbreath)
    {
        one_hit(ch,victim,dt,1);
        return;
    }
    if ( dt==gsn_magma)
    {
        one_hit(ch,victim,dt,1);
        return;
    }
    if ( dt==gsn_icebreath)
    {
        one_hit(ch,victim,dt,1);
        return;
    }
    if ( dt==gsn_firebreath)
    {
        one_hit(ch,victim,dt,1);
        return;
    }
    if ( dt==gsn_acidbreath)
    {
        one_hit(ch,victim,dt,1);
        return;
    }
    if ( dt==gsn_hatredbreath)
    {
        one_hit(ch,victim,dt,1);
        return;
    }
    if ( dt==gsn_fireblast)
    {
        one_hit(ch,victim,dt,1);
        return;
    }
    if ( dt==gsn_iceblast)
    {
        one_hit(ch,victim,dt,1);
        return;
    }
    if ( dt==gsn_acidblast)
    {
        one_hit(ch,victim,dt,1);
        return;
    }
    if ( dt==gsn_hatredblast)
    {
        one_hit(ch,victim,dt,1);
        return;
    }

    if ( dt==gsn_frost)
    {
        one_hit(ch,victim,dt,1);
        return;
    }
    if ( dt==gsn_shards)
    {
        one_hit(ch,victim,dt,1);
        return;
    }
    if ( dt==gsn_cheapshot)
    {
        send_to_char("You stun them with a shoulder charge!\n\r",ch);
        send_to_char("You are stunned by a shoulder charge!\n\r",victim);
        one_hit(ch,victim,dt,1);
        one_hit(ch,victim,dt,1);
        return;
    }
    if ( dt == gsn_buffet )
    {
        one_hit( ch, victim, dt, 1 );
        return;
    }
    if ( dt == gsn_sweep )
    {
        one_hit( ch, victim, dt, 1 );
        return;
    }
    if ( dt == gsn_rfangs )
    {
        one_hit( ch, victim, dt, 1);
        return;
    }
    if (dt==gsn_heavenlyaura)
    {
        if (IS_CLASS(ch, CLASS_MONK)) x = number_range(1,4);
        else
        {
                                                            // angels don't like em samurais.
            if (IS_CLASS(victim, CLASS_SAMURAI)) x = number_range(3,5);
            else x = number_range(2,5);
        }
        for (i=0; i < x; i++)
            one_hit(ch,victim,dt,1);
        return;
    }
    if (dt==gsn_mageshield)
    {
        if (IS_ITEMAFF(ch, ITEMA_MAGESHIELD))
        {
            one_hit(ch,victim,dt,1);
            one_hit(ch,victim,dt,1);
            one_hit(ch,victim,dt,1);
            one_hit(ch,victim,dt,1);
            one_hit(ch,victim,dt,1);
        }
        return;
    }
    if ( dt == gsn_bladespin)
    {
        if (number_range(1,2)==1) one_hit(ch,victim,dt,1);
        if (number_range(1,2)==1) one_hit(ch,victim,gsn_lightningkick,1);
        if (number_range(1,2)==1) one_hit(ch,victim,gsn_knee,1);
        if (number_range(1,2)==1) one_hit(ch,victim,gsn_lightningslash,1);
        if ((ch->wpn[0] >= 1000)  && (ch->wpn[3] >= 1000)
            && (ch->wpn[6] >= 1000) && (ch->wpn[9] >= 1000)
            && (ch->wpn[1] >= 1000) && (ch->wpn[4] >= 1000)
            && (ch->wpn[7] >= 1000)
            && (ch->wpn[10] >= 1000) && (ch->wpn[2] >= 1000)
            && (ch->wpn[5] >= 1000) && (ch->wpn[8] >= 1000)
            && (ch->wpn[11] >= 1000) && (ch->wpn[12] >= 1000))
        {
            if (number_range(1,2)==1) one_hit(ch,victim,dt,1);
            if (number_range(1,2)==1) one_hit(ch,victim,gsn_lightningkick,1);
            //    if (number_range(1,2)==1) one_hit(ch,victim,gsn_knee,1);
            //    if (number_range(1,2)==1) one_hit(ch,victim,gsn_lightningslash,1);
        }
        return;
    }

    MultiHit++;

    wield1 = get_eq_char( ch, WEAR_WIELD );
    wield2 = get_eq_char( ch, WEAR_HOLD );
    wield3 = get_eq_char( ch, WEAR_THIRD );
    wield4 = get_eq_char( ch, WEAR_FOURTH);
    if ( wield1 != NULL && wield1->item_type == ITEM_WEAPON ) wieldorig  = 1;
    if ( wield2 != NULL && wield2->item_type == ITEM_WEAPON ) wieldorig += 2;
    if ( wield3 != NULL && wield3->item_type == ITEM_WEAPON ) wieldorig += 4;
    if ( wield4 != NULL && wield4->item_type == ITEM_WEAPON ) wieldorig += 8;
    wieldtype = wieldorig;
    wieldie = number_range(1,4);
    if ( wieldorig == 15 )
    {
        if (wieldie == 2) wieldtype = 8;
        else wieldtype = 4;
    }
    if ( wieldorig == 14 )
    {
        if (wieldie == 2) wieldtype = 8;
        else if (wieldie == 1) wieldtype = 4;
        else if (wieldie == 3) wieldtype = 2;
        else wieldtype = 1;
    }
    if ( wieldorig == 13 )
    {
        if ( wieldie == 1) wieldtype = 8;
        else if (wieldie == 2) wieldtype = 4;
        else wieldtype = 1;
    }
    if ( wieldorig == 12 )
    {
        if ( wieldie == 1 ) wieldtype = 8;
        else wieldtype = 4;
    }
    if ( wieldorig == 11 )
    {
        if ( wieldie == 1 ) wieldtype = 8;
        if ( wieldie == 2 ) wieldtype = 2;
        else wieldtype = 1;
    }
    if ( wieldorig == 10 )
    {
        if ( wieldie == 1 ) wieldtype = 8;
        else wieldtype = 2;
    }
    if ( wieldorig == 9 )
    {
        if ( wieldie == 1 ) wieldtype = 8;
        else wieldtype = 1;
    }
    if ( wieldorig == 8 ) wieldtype = 8;
    if ( wieldorig == 7 )
    {
        if ( wieldie == 1 ) wieldtype = 4;
        else if ( wieldie == 2 ) wieldtype = 2;
        else wieldtype = 1;
    }
    if ( wieldorig == 6 )
    {
        if ( wieldie == 1 || wieldie == 2) wieldtype = 2;
        else wieldtype = 4;
    }
    if ( wieldorig == 5 )
    {
        if ( wieldie == 1) wieldtype = 4;
        else wieldtype = 1;
    }
    if ( wieldorig == 4 ) wieldtype = 4;
    if ( wieldorig == 3 )
    {
        if ( wieldie == 2 || wieldie == 4) wieldtype = 2;
        else wieldtype = 1;
    }
    if ( wieldorig == 2 ) wieldtype = 2;
    if ( wieldorig == 1 ) wieldtype = 1;

    if ( wieldtype == 8 ) wield = wield4;
    else if ( wieldtype == 4 ) wield = wield3;
    else if ( wieldtype == 2 ) wield = wield2;
    else wield = wield1;

    if (!IS_NPC(ch) && ch->stance[0] > 0 && number_percent() == 5)
    {
        int stance = ch->stance[0];
        if (ch->stance[stance] >= 200)
        {
            special_move(ch,victim);
            MultiHit--;
            total_dam(ch,victim);
            return;
        }
    }

    unarmed = 0;
    if (IS_SET( ch->flag2, VAMP_OBJMASK) )
    {
        act("You return to your normal form.",ch,NULL,NULL,TO_CHAR);
        sprintf(buf,"%s reforms as %s.\n\r",ch->morph, ch->name );
        act(buf,ch,NULL,NULL,TO_ROOM);
        free_string(ch->morph);
        free_string(ch->objdesc);
        ch->long_descr = str_dup("");
        REMOVE_BIT(ch->flag2, VAMP_OBJMASK);
        REMOVE_BIT(ch->affected_by, AFF_POLYMORPH);
    }
    one_hit( ch, victim, dt, wieldtype );

    if ( victim == NULL || victim->position != POS_FIGHTING )
    {
        MultiHit--;
        total_dam(ch,victim);
        return;
    }

    /* Only want one spell per round from spell weapons...otherwise it's
     * too powerful, and would unbalance player killing (as this is a PK mud).
     */

    if ( dt == TYPE_UNDEFINED )
    {
        dt = TYPE_HIT;
        if ( wield != NULL && wield->item_type == ITEM_WEAPON )
        {
            dt += wield->value[3];
            if (wield->value[0] >= 1)
            {
                /* Look, just don't ask...   KaVir */
                if (wield->value[0] >= 1000) sn = wield->value[0] - ((wield->value[0] / 1000) * 1000);
                else sn = wield->value[0];
                if (((skill_table[sn].skill_level >= 7) && (ch->level < 7)) || ((skill_table[sn].target != TAR_CHAR_OFFENSIVE) && (skill_table[sn].target != TAR_CHAR_DEFENSIVE)))
                {
                    //          send_to_char("Your weapon has an illegal spell affect, please report this.\n\r", ch);
                }
                else if (sn != skill_lookup("gas breath") && sn != skill_lookup("sleep") && sn != 0 && sn != skill_lookup("gate"))
                {
                    if (victim->position == POS_FIGHTING) (*skill_table[sn].spell_fun) (sn,wield->level,ch,victim);
                }
            }
        }
    }
    if ( victim == NULL || victim->position != POS_FIGHTING )
    {
        MultiHit--;
        total_dam(ch,victim);
        return;
    }
    if ( ch->fighting != victim || dt == gsn_backstab || dt == gsn_shiroken
        || dt == gsn_garotte || dt == gsn_headbutt || dt == gsn_circle || dt == gsn_laserbeam
        || dt == gsn_ancestorswill || dt == gsn_firearrow || dt == gsn_icearrow || dt == gsn_lightningarrow
        || dt == gsn_elvenmagic )
    {
        MultiHit--;
        total_dam(ch,victim);
        return;
    }
    maxcount = number_attacks(ch, victim);
    if (!IS_NPC(ch))
    {
        chance = 0;
        if (wield != NULL && wield->item_type == ITEM_WEAPON)
        {
            int tempnum = wield->value[3];
            chance = ((ch->wpn[tempnum] + (ch->wpn[tempnum] * (ch->wpn_lvl[tempnum] * 0.5)) ) * 0.5);
        }
        else chance = (ch->wpn[0]) * 0.5;
        if (number_percent() <= chance) maxcount += 1;
    }
    if (wieldorig == 3) maxcount += 1;
    for (countup = 0; countup <= maxcount; countup++)
    {
        wieldtype = wieldorig;
        if ( wieldorig == 15 )
        {
            if (wieldie == 2)  wieldtype = 8;
            else wieldtype = 4;
        }
        if ( wieldorig == 14 )
        {
            if (wieldie == 2) wieldtype = 8;
            else if (wieldie == 1) wieldtype = 4;
            else if (wieldie == 3) wieldtype = 2;
            else wieldtype = 1;
        }
        if ( wieldorig == 13 )
        {
            if ( wieldie == 1) wieldtype = 8;
            else if (wieldie == 2) wieldtype = 4;
            else wieldtype = 1;
        }
        if ( wieldorig == 12 )
        {
            if ( wieldie == 1 ) wieldtype = 8;
            else wieldtype = 4;
        }
        if ( wieldorig == 11 )
        {
            if ( wieldie == 1 ) wieldtype = 8;
            if ( wieldie == 2 ) wieldtype = 2;
            else wieldtype = 1;
        }
        if ( wieldorig == 10 )
        {
            if ( wieldie == 1 ) wieldtype = 8;
            else wieldtype = 2;
        }
        if ( wieldorig == 9 )
        {
            if ( wieldie == 1 ) wieldtype = 8;
            else wieldtype = 1;
        }
        if ( wieldorig == 7 )
        {
            if ( wieldie == 1 ) wieldtype = 4;
            else if ( wieldie == 2 ) wieldtype = 2;
            else wieldtype = 1;
        }
        if ( wieldorig == 6 )
        {
            if ( wieldie == 1 || wieldie == 2) wieldtype = 2;
            else wieldtype = 4;
        }
        if ( wieldorig == 5 )
        {
            if ( wieldie == 1) wieldtype = 4;
            else wieldtype = 1;
        }
        if ( wieldorig == 3 )
        {
            if ( wieldie == 2 || wieldie == 4) wieldtype = 2;
            else wieldtype = 1;
        }
        one_hit( ch, victim, -1, wieldtype );
        if ( victim == NULL || victim->position != POS_FIGHTING )
        {
            MultiHit--;
            total_dam(ch,victim);
            return;
        }
        if ( ch->fighting != victim )
        {
            MultiHit--;
            total_dam(ch,victim);
            return;
        }
    }
    if (!IS_NPC(ch) && IS_VAMPAFF(ch,VAM_FANGS))
    {
        if (!IS_NPC(victim))
        {
            one_hit(ch,victim, (TYPE_HIT + 10),0);
            one_hit( ch, victim, ( TYPE_HIT + 10 ), 0 );
        }
        else
        {
            one_hit(ch,victim, (TYPE_HIT + 10),1);
            one_hit( ch, victim, ( TYPE_HIT + 10 ), 1 );
        }
    }
    if (!IS_NPC(ch) && IS_CLASS(ch,CLASS_KAHN))
    {
        if (IS_SET(ch->pcdata->stats[UNI_AFF], VAM_CLAWS) && number_range(1,100) < 51)
            one_hit( ch, victim, gsn_claws, 1 );
        one_hit( ch, victim, gsn_supreme, 2 );
        one_hit( ch, victim, gsn_claws, 2 );
        if ((IS_VAMPAFF(ch, VAM_FANGS)) && number_range(1,100) < 51)
            one_hit( ch, victim, gsn_fangs, 1 );
        one_hit( ch, victim, gsn_supreme, 2 );
        one_hit( ch, victim, gsn_fangs, 2 );
        if (ch->pcdata->powers[KAHN_TIGER] > 3)
            one_hit( ch, victim, gsn_spiket, 1);
        one_hit( ch, victim, gsn_supreme, 1);
        one_hit( ch, victim, gsn_spiket, 2);
    }

    if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_DRAGON))
    {
        if ((ch->pcdata->classfour[DRAGON_POWER] > 4) &&  (number_range(1,100) < 51))
            one_hit( ch, victim, gsn_spiket, 1);
        one_hit( ch, victim, gsn_spiket, 1);
        if ((IS_SET(ch->pcdata->classfour[DRAGON_POWER], DRAG_CLAWS)) && (number_range(1,100) < 51))
            one_hit( ch, victim, gsn_claws, 0 );
        one_hit( ch, victim, gsn_claws, 1 );
        if ((IS_VAMPAFF(ch, VAM_FANGS)) && number_range(1,100) < 51)
            one_hit( ch, victim, gsn_fangs, 1 );
        if ((IS_SET(ch->pcdata->classfour[DRAGON_POWER], DRAG_TAIL)) &&  (number_range(1,100) < 51))
            one_hit( ch, victim, gsn_spiket, 0);
        one_hit( ch, victim, gsn_spiket, 1);

    }
    if ( IS_CLASS(ch, CLASS_DEMON) && IS_SET(ch->warp, WARP_SPIKETAIL) && number_range(1,3) == 1)
    {
        if (IS_NPC(victim))
        {
            one_hit( ch, victim, gsn_spiket, 0);
            one_hit( ch, victim, gsn_spiket, 1);
        }
        else one_hit( ch, victim, gsn_spiket, 0);
    }
    /* Golems speciels */
    if (IS_NPC(ch))
    {
        if (ch->pIndexData->vnum == MOB_VNUM_FIRE)
        {
            if ((sn = skill_lookup( "curse" ) ) > 0)
                (*skill_table[sn].spell_fun) (sn,50,ch,victim);
            if ((sn = skill_lookup( "imp faerie fire" ) ) > 0)
                (*skill_table[sn].spell_fun) (sn,50,ch,victim);
        }
        if (ch->pIndexData->vnum == MOB_VNUM_STONE)
        {
            af.type      = skill_lookup("reserved");
            af.duration  = 20;
            af.location  = APPLY_DAMROLL;
            af.modifier  = -50;
            af.bitvector = 0;
            affect_to_char(victim, &af );

            af.type      = skill_lookup("reserved");
            af.duration  = 20;
            af.location  = APPLY_HITROLL;
            af.modifier  = -50;
            af.bitvector = 0;
            affect_to_char(victim, &af );
            send_to_char("You feel weak in the presence of the stone golem.\n\r",victim);
        }
        if (ch->pIndexData->vnum == MOB_VNUM_CLAY)
        {
            if ((sn = skill_lookup( "clay" ) ) > 0)
                (*skill_table[sn].spell_fun) (sn,50,ch,victim);
        }
        if (ch->pIndexData->vnum == MOB_VNUM_IRON)
        {
            if ((sn = skill_lookup( "group heal" ) ) > 0)
                (*skill_table[sn].spell_fun) (sn,50,ch,victim);
        }
    }
    if ( IS_CLASS(ch, CLASS_DEMON) && IS_SET(ch->warp, WARP_SHARDS) && number_range(1,3) == 1)
        one_hit( ch, victim, gsn_shards, 0);
    if ( IS_CLASS(ch, CLASS_DEMON) && IS_SET(ch->warp, WARP_MAGMA) && number_range(1,3) == 1)
    {
        if (IS_NPC(victim))
        {
            one_hit( ch, victim, gsn_magma, 0);
            one_hit( ch, victim, gsn_magma, 1);
        }
        else one_hit( ch, victim, gsn_magma, 0);
    }

    /*  if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_MECHA))
      {
         one_hit(ch,victim, gsn_machine,1);
         one_hit(ch,victim, gsn_machine,1);
         one_hit(ch,victim, gsn_machine,1);
         one_hit(ch,victim, gsn_rocket,1);
         one_hit(ch,victim, gsn_rocket,1);
         one_hit(ch,victim, gsn_proton,1);
      }
    */

    if ( IS_CLASS(ch, CLASS_MONK))
    {
        if (IS_NPC(victim))
        {
            one_hit( ch, victim, gsn_supreme, 0);
            one_hit( ch, victim, gsn_supreme, 1);
            one_hit( ch, victim, gsn_supreme, 2);
            one_hit( ch, victim, gsn_supreme, 3);
        }
        else
        {
            one_hit( ch, victim, gsn_supreme, 0);
            one_hit( ch, victim, gsn_supreme, 1);
            one_hit( ch, victim, gsn_supreme, 2);
            one_hit( ch, victim, gsn_supreme, 3);
            one_hit( ch, victim, gsn_supreme, 3);

        }
    }

    if ( IS_CLASS(ch, CLASS_WEREWOLF))
    {

        if (IS_NPC(victim))
        {
            one_hit( ch, victim, gsn_supreme, 2);
            one_hit( ch, victim, gsn_supreme, 2);
            one_hit( ch, victim, gsn_supreme, 2);
            one_hit( ch, victim, gsn_supreme, 2);
        }
        else
        {
            one_hit( ch, victim, gsn_supreme, 2);
            one_hit( ch, victim, gsn_supreme, 2);
            //      one_hit( ch, victim, gsn_supreme, 2);

        }
    }

    if ( IS_CLASS(ch, CLASS_DEMON) && IS_SET(ch->warp, WARP_VENOMTONG) && number_range(1,3) == 1)
        one_hit( ch, victim, gsn_venomt, 0);
    if ( victim == NULL || victim->position != POS_FIGHTING )
    {
        MultiHit--;
        total_dam(ch,victim);
        return;
    }

    if (!IS_NPC(ch) && !IS_NPC(victim))                     // pkpower
    {
        if (ch->pcdata->upgrade_level > 3)
        {
            i = number_range(1,10);
            if (i < 5) one_hit(ch,victim, gsn_supreme,0);
            else if (i < 7) one_hit(ch,victim, gsn_supreme,1);
            else ;
        }
    }
    if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_SHAPESHIFTER))
    {
        if (ch->pcdata->powers[SHAPE_FORM] == HYDRA_FORM)
        {
            one_hit(ch,victim, gsn_fangs, 1);
            if (ch->pcdata->powers[HYDRA_LEVEL]>0) one_hit(ch,victim, gsn_fangs, 1);
            if (ch->pcdata->powers[HYDRA_LEVEL]>1) one_hit(ch,victim, gsn_fangs, 1);
            if (ch->pcdata->powers[HYDRA_LEVEL]>3) one_hit(ch,victim, gsn_fangs, 1);
            if (ch->pcdata->powers[HYDRA_LEVEL]>4) one_hit(ch,victim, gsn_fangs, 1);
        }
        if (ch->pcdata->powers[SHAPE_FORM] == TIGER_FORM)
        {
            if (ch->pcdata->powers[TIGER_LEVEL]>0) multi_hit(ch, victim, gsn_claws);
            if (ch->pcdata->powers[TIGER_LEVEL]>1) multi_hit(ch, victim, gsn_fangs);
            if (ch->pcdata->powers[TIGER_LEVEL]>0) multi_hit(ch, victim, gsn_claws);
            if (ch->pcdata->powers[TIGER_LEVEL]>1) multi_hit(ch, victim, gsn_fangs);
        }
        if (ch->pcdata->powers[SHAPE_FORM] == BULL_FORM)
        {
            if (ch->pcdata->powers[BULL_LEVEL]>0) multi_hit(ch, victim, gsn_headbutt);
            if (ch->pcdata->powers[BULL_LEVEL]>1) multi_hit(ch, victim, gsn_hooves);
        }
        if (ch->pcdata->powers[SHAPE_FORM] == FAERIE_FORM)
        {
            if (IS_NPC(victim))
            {
                one_hit(ch, victim, gsn_fireball,1);
                one_hit(ch, victim, gsn_fireball,1);
            }
            else
            {
                one_hit(ch, victim, gsn_fireball,0);
                one_hit(ch, victim, gsn_fireball,0);
            }
            if (ch->pcdata->powers[FAERIE_LEVEL] > 1) one_hit(ch, victim, gsn_buffet,1);
        }
    }
    if (!IS_NPC(ch) && ch->level > 2)
    {
        if (IS_VAMPAFF(ch,VAM_HORNS) && number_range(1,3) == 1)
            multi_hit( ch, victim, gsn_headbutt );
    }
    if (IS_CLASS(ch, CLASS_TANARRI) && !IS_NPC(ch))
    {
        if (IS_SET(ch->pcdata->powers[TANARRI_POWER], TANARRI_FANGS))
        {
            one_hit(ch, victim, gsn_fangs, 1);
            one_hit(ch, victim, gsn_fangs, 2);
            one_hit(ch, victim, gsn_fangs, 3);
            one_hit(ch, victim, gsn_fangs, 4);
            if (IS_SET(ch->pcdata->powers[TANARRI_POWER], TANARRI_HEAD)) one_hit(ch, victim, gsn_fangs, 5);
        }
        if (IS_SET(ch->pcdata->powers[TANARRI_POWER], TANARRI_FIERY))
        {
            one_hit(ch, victim, gsn_fiery, 1);
            one_hit(ch, victim, gsn_fiery, 2);
            if (IS_CLASS(victim, CLASS_ANGEL)) one_hit(ch, victim, gsn_fiery, 2);
        }
    }
    if (IS_CLASS(ch, CLASS_UNDEAD_KNIGHT) && ch->pcdata->powers[WEAPONSKILL] > 15)
    {
        one_hit(ch,victim,gsn_lightningslash,1);
        one_hit(ch,victim,gsn_lightningslash,1);
        one_hit(ch,victim,gsn_lightningslash,1);
        one_hit(ch,victim,gsn_lightningslash,1);
    }
    if (IS_CLASS(ch, CLASS_LICH))
    {
        if (ch->pcdata->powers[CON_LORE] > 4)
            one_hit(ch,victim, gsn_fireball,0);
        one_hit(ch,victim, gsn_fireball,1);
        one_hit(ch,victim, gsn_supreme,1);
        if (ch->pcdata->powers[NECROMANTIC] > 4)
            one_hit(ch,victim, gsn_chillhand,0);
        one_hit(ch,victim, gsn_chillhand,1);
        one_hit(ch,victim, gsn_supreme,1);
        if (ch->pcdata->powers[DEATH_LORE] > 4)
            one_hit(ch,victim, gsn_deathaura,0);
        one_hit(ch,victim, gsn_deathaura,1);
        one_hit(ch,victim, gsn_supreme,1);
    }
    if (IS_CLASS(ch, CLASS_DEMON)  && IS_DEMAFF(ch,DEM_HORNS) && number_range(1,3) == 1)
        one_hit( ch, victim, gsn_headbutt, 0 );
    if (IS_CLASS(ch, CLASS_DEMON) && number_range(1,3) == 1 && ch->power[DISC_DAEM_HELL] > 3)
        one_hit(ch, victim, gsn_hellfire, 0);
    if (IS_CLASS(ch, CLASS_DEMON)  && IS_DEMAFF(ch, DEM_TAIL) && number_range(1,2) == 2)
        one_hit( ch, victim, gsn_sweep, 0 );
    if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_NINJA) && ch->pcdata->powers[NPOWER_NINGENNO] >=5 && number_range(1,2) != 1)
        multi_hit(ch, victim, gsn_shiroken);
    if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_DROID) && ch->pcdata->powers[CYBORG_FACE] > 5 && number_range(1,2) != 1)
        multi_hit(ch, victim, gsn_laserbeam);
    if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_ELF) && ch->pcdata->classtwo[ELF_ARC_FIREARROW] > 0 && number_range(1,2) != 1)
        multi_hit(ch, victim, gsn_firearrow);
    if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_ELF) && ch->pcdata->classtwo[ELF_ARC_ICEARROW] > 0 && number_range(1,2) != 1)
        multi_hit(ch, victim, gsn_icearrow);
    if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_ELF) && ch->pcdata->classtwo[ELF_ARC_LIGHTARROW] > 0 && number_range(1,2) != 1)
        multi_hit(ch, victim, gsn_lightningarrow);
    if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_ELF) && IS_SET(ch->special,SPC_ELF_MAGE) && number_range(1,2) != 1)
        multi_hit(ch, victim, gsn_elvenmagic);
    if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_WIZARD) && number_range(1,2) != 1)
        multi_hit(ch, victim, gsn_elvenmagic);

    if (!IS_NPC(ch))
    {
        if (IS_VAMPAFF(ch,VAM_TAIL) && number_range(1,2) == 1)
            multi_hit( ch, victim, gsn_sweep );
    }

    if (!IS_NPC(ch) && IS_VAMPAFF(ch,VAM_HEAD) && number_range(1,2) == 1)
    {
        multi_hit( ch, victim, gsn_fangs );
    }
    if (!IS_NPC(ch) && IS_SET(ch->newbits,NEW_QUILLS) && IS_CLASS(ch, CLASS_WEREWOLF))
        multi_hit(ch,victim,gsn_quills);
    if (!IS_NPC(ch) && IS_SET(ch->newbits,NEW_BLADESPIN) && IS_CLASS(ch, CLASS_SAMURAI))
        multi_hit(ch,victim,gsn_bladespin);
    if (!IS_NPC(ch) && IS_SET(ch->newbits, NEW_DARKTENDRILS) && IS_CLASS(ch, CLASS_DROW))
        multi_hit(ch,victim,gsn_darktendrils);
    if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_WIZARD) && number_range(1,2) != 1)
        multi_hit(ch,victim,gsn_lightning);
    if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_WRAITH) && (IS_SET(ch->pcdata->powers[AURAS], DEATH_AURA)))
        multi_hit(ch,victim,gsn_deathaura);
    if (!IS_NPC(ch) && IS_SET(ch->newbits,NEW_MULTIARMS) && IS_CLASS(ch, CLASS_MAGE))
        multi_hit(ch,victim,gsn_mageshield);
    if (!IS_NPC(ch) && ch->monkab[BODY] > 3 && IS_CLASS(ch, CLASS_MONK))
        multi_hit(ch,victim,gsn_heavenlyaura);
    if (!IS_NPC(ch) && IS_CLASS(ch,CLASS_ANGEL))
    {
        if (IS_SET(ch->pcdata->powers[ANGEL_POWERS], ANGEL_AURA))
        {
            // multi_hit(ch,victim,gsn_heavenlyaura);
            multi_hit(ch,victim,gsn_heavenlyaura);
        }

        if (IS_SET(ch->pcdata->powers[ANGEL_POWERS], ANGEL_HALO))
        {
            switch(number_range(1,2))
            {
                default : break;
                case 1 :
                    if ((sn = skill_lookup( "web" ) ) > 0)
                        (*skill_table[sn].spell_fun) (sn,50,ch,victim);
                    break;
                case 2 :
                    if ((sn = skill_lookup( "imp fireball" ) ) > 0)
                        (*skill_table[sn].spell_fun) (sn,50,ch,victim);
                    break;
            }
        }
    }
    if (!IS_NPC(ch) && IS_SET(ch->newbits, NEW_CUBEFORM) && IS_CLASS(ch, CLASS_DROID))
    {
        one_hit(ch,victim,gsn_stuntubes,1);
        multi_hit(ch,victim,gsn_stinger);
    }
    if (!IS_NPC(ch)&& IS_CLASS(ch,CLASS_WEREWOLF))
    {
        if (IS_SET(ch->newbits,NEW_SLAM) && number_range(1,5-ch->power[DISC_WERE_BEAR]/3)==1)
            multi_hit(ch,victim,gsn_cheapshot);
        if (IS_SET(ch->newbits, NEW_REND) && number_range(1,5) == 3)
            multi_hit( ch, victim, gsn_telekinetic);
    }
    if (!IS_NPC(ch) && IS_VAMPAFF(ch,VAM_WINGS) && number_range(1,2) == 1 && ch->level > 2)
        multi_hit( ch, victim, gsn_buffet );
    if (!IS_NPC(ch) && IS_VAMPAFF(ch,VAM_WINGS) && number_range(1,2) == 1 && ch->level > 2)
        multi_hit( ch, victim, gsn_buffet );
    if (IS_CLASS(ch, CLASS_DEMON) && IS_SET(ch->warp, WARP_WINGS) && number_range(1,3) == 1)
        one_hit(ch, victim, gsn_buffet, 0 );
    if (IS_CLASS(ch, CLASS_DRAGON) && IS_SET(ch->pcdata->classfour[DRAGON_POWER], DRAG_WINGS) && number_range(1,3) == 1)
        one_hit(ch, victim, gsn_buffet, 0 );
    if (!IS_NPC(ch) && IS_CLASS(ch,CLASS_WEREWOLF) && ch->power[DISC_WERE_RAPT] > 0)
        multi_hit(ch,victim,gsn_rfangs);
    if ( victim == NULL || victim->position != POS_FIGHTING )
    {
        MultiHit--;
        total_dam(ch,victim);
        return;
    }
    if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_VAMPIRE) && IS_VAMPAFF(ch, VAM_SERPENTIS))
        spell_poison( gsn_poison, (ch->level*number_range(5,10)),ch,victim );
    else if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_WEREWOLF) && ch->power[DISC_WERE_SPID] > 0)
        spell_poison( gsn_poison, (ch->level*number_range(5,10)),ch,victim );
    else if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_DROW) && IS_SET(ch->pcdata->powers[1], DPOWER_DROWPOISON))
        spell_poison(gsn_poison,(ch->level*number_range(10,20)),ch,victim);
    else if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_NINJA) && ch->pcdata->powers[NPOWER_NINGENNO] >=5)
        spell_poison(gsn_poison,(ch->level*number_range(5,10)),ch,victim);
    else if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_RUNEMASTER))
        rune_attack(ch,victim);
    if (victim->itemaffect < 1)
    {
        MultiHit--;
        total_dam(ch,victim);
        return;
    }
    if (IS_NPC(victim) || victim->spl[1] < 4) level = victim->level;
    else level = (victim->spl[1] * 0.25);

    if (IS_SET(victim->pcdata->classtwo[ELF_REDSTONE], ELFSTONE_FLAMERING) && !IS_AFFECTED(ch, AFF_FLAMING))
    {
        if (number_percent() > 95)
        {
            SET_BIT(ch->affected_by, AFF_FLAMING);
            if (!IS_CLASS(victim, CLASS_ELF))
            {
                act("Your flamering catches $n on fire!", ch, NULL, victim, TO_VICT);
                if (!IS_SET(ch->act, PLR_BRIEF6))
                    act("$N's flamering catches $n on fire!", ch, NULL, victim, TO_NOTVICT);
                act("$N's flamering catches you on fire!.", ch, NULL, victim, TO_CHAR);
            }
            else
            {
                act("Your flamering catches $n on fire!", ch, NULL, victim, TO_VICT);
                if (!IS_SET(ch->act, PLR_BRIEF6))
                    act("$N's flamering catches $n on fire!", ch, NULL, victim, TO_NOTVICT);
                act("$N's flamering catches you on fire!.", ch, NULL, victim, TO_CHAR);
            }
        }
        else
        {
            MultiHit--;
            total_dam(ch,victim);
            return;
        }
    }

    if (IS_SET(victim->newbits, NEW_MONKFLAME) && !IS_AFFECTED(ch, AFF_FLAMING))
    {
        if (number_percent() > 95)
        {
            SET_BIT(ch->affected_by, AFF_FLAMING);
            if (!IS_CLASS(ch, CLASS_WEREWOLF))
            {
                act("Your flaming hands catch $n on fire!", ch, NULL, victim, TO_VICT);
                if (!IS_SET(ch->act, PLR_BRIEF6))
                    act("$N's flaming hands catch $n on fire!", ch, NULL, victim, TO_NOTVICT);
                act("$N's flaming hands catch you on fire!.", ch, NULL, victim, TO_CHAR);
            }
            else
            {
                act("Your flaming claws catch $n on fire!", ch, NULL, victim, TO_VICT);
                if (!IS_SET(ch->act, PLR_BRIEF6))
                    act("$N's flaming claws catch $n on fire!", ch, NULL, victim, TO_NOTVICT);
                act("$N's flaming claws catch you on fire!.", ch, NULL, victim, TO_CHAR);
            }
        }
        else
        {
            MultiHit--;
            total_dam(ch,victim);
            return;
        }
    }
    if (!IS_NPC(victim))
    {
        if (IS_SET(victim->pcdata->powers[AURAS], DEATH_AURA) && IS_CLASS(victim, CLASS_UNDEAD_KNIGHT))
        {
            while (number_range(1,2) != 1)
                one_hit(victim,ch,gsn_deathaura,0);
        }
        if (IS_SET(victim->pcdata->powers[AURAS], DEATH_AURA) && IS_CLASS(victim, CLASS_WRAITH))
        {
            while (number_range(1,2) != 1)
                one_hit(victim,ch,gsn_deathaura,0);
        }
        if (IS_SET(victim->pcdata->powers[AURAS], FEAR_AURA) && IS_CLASS(victim, CLASS_UNDEAD_KNIGHT))
        {
            af.type  = skill_lookup("reserved");
            af.duration  = 20;
            af.location  = APPLY_DAMROLL;
            af.modifier  = -20;
            af.bitvector = 0;
            affect_to_char(ch, &af );

            af.type  = skill_lookup("reserved");
            af.duration  = 20;
            af.location  = APPLY_HITROLL;
            af.modifier  = -20;
            af.bitvector = 0;
            affect_to_char(ch, &af );
            send_to_char("You piss your pants out of fear.\n\r",ch);
        }
    }

    if (IS_ITEMAFF(victim, ITEMA_SHOCKSHIELD) && ch->position == POS_FIGHTING)
        if ( (sn = skill_lookup( "lightning bolt" ) ) > 0)
            (*skill_table[sn].spell_fun) (sn,level,victim,ch);
    if (IS_ITEMAFF(victim, ITEMA_FIRESHIELD) && ch->position == POS_FIGHTING)
        if ( (sn = skill_lookup( "fireball" ) ) > 0)
            (*skill_table[sn].spell_fun) (sn,level,victim,ch);
    if (IS_ITEMAFF(victim, ITEMA_ICESHIELD) && ch->position == POS_FIGHTING)
        if ( (sn = skill_lookup( "chill touch" ) ) > 0)
            (*skill_table[sn].spell_fun) (sn,level,victim,ch);
    if (IS_ITEMAFF(victim, ITEMA_ACIDSHIELD) && ch->position == POS_FIGHTING)
        if ( (sn = skill_lookup( "acid blast" ) ) > 0)
            (*skill_table[sn].spell_fun) (sn,level,victim,ch);

    /*int elehits;
    int eletype;
    int eletype2;
    int eletype3;
    int eletype4;
    int eletype5;
    int eledam;
    int test;
    if (IS_ITEMAFF(victim, ITEMA_SHOCKSHIELD) && ch->position == POS_FIGHTING){
          eletype2+=2;
          eletype5-=2;
          eletype4+=1;
    }
    if (IS_ITEMAFF(victim, ITEMA_FIRESHIELD) && ch->position == POS_FIGHTING){
          eletype3+=2;
          eletype4-=2;
          eletype5+=1;
    }
    if (IS_ITEMAFF(victim, ITEMA_ICESHIELD) && ch->position == POS_FIGHTING){
          eletype4+=2;
          eletype3-=2;
          eletype2+=1;
    }
    if (IS_ITEMAFF(victim, ITEMA_ACIDSHIELD) && ch->position == POS_FIGHTING){
          eletype5+=2;
          eletype2-=2;
          eletype3+=1;
    }
    if(eletype2<0)eletype2=0;
    else elehits+=1;
    if(eletype3<0)eletype3=0;
    else elehits+=1;
    if(eletype4<0)eletype4=0;
    else elehits+=1;
    if(eletype5<0)eletype5=0;
    else elehits+=1;
    eletype=eletypr2+eletypr3+eletypr4+eletypr5;
    if eletype==4 eletype -=2;
    if eletype==5 eletype -=1;
    for(test=0;test<numberhits;test++){
    eledam=eletype*number_range(150,250);
        sprintf(buf, "An elemental shield strikes you  [#P%d#n]",eledam);
        act(buf, victim, NULL, ch, TO_CHAR);
        sprintf(buf, "Your elemental shield strikes them [#P%d#n]",new_dam);
        act(buf, victim, NULL, ch, TO_VICT);
        hurt_person(victim,ch, eledam);
       }*/

    if (IS_ITEMAFF(victim, ITEMA_CHAOSSHIELD) && ch->position == POS_FIGHTING)
        if ( (sn = skill_lookup( "chaos blast" ) ) > 0)
            (*skill_table[sn].spell_fun) (sn,level,victim,ch);

            {
                MultiHit--;
                total_dam(ch,victim);
                return;
            }
}

int number_attacks( CHAR_DATA *ch, CHAR_DATA *victim )
{
    int count = 1;

    if (IS_NPC(ch))
    {
        count = ch->level/1000;
        if ( count < 1 ) count = 1;
        if( ch->pIndexData->hitsizedice > 20) count += 20;
        else count += ch->pIndexData->hitsizedice;
        if ( count > 50 ) count = 50;
        return count;
    }
    if (IS_NPC(victim))
    {
        if ( IS_STANCE(ch, STANCE_VIPER) && number_percent() < ch->stance[STANCE_VIPER] * 0.5) count += 1;
        else if ( IS_STANCE(ch, STANCE_MANTIS) && number_percent() < ch->stance[STANCE_MANTIS] * 0.5) count += 1;
        else if ( IS_STANCE(ch, STANCE_TIGER) && number_percent() < ch->stance[STANCE_TIGER] * 0.5) count += 1;
        else if ( IS_STANCE(ch, STANCE_WOLF) && number_percent() < ch->stance[STANCE_WOLF] * 0.5) count += 10;
        else if ( ch->stance[0] > 12 && IS_SET(ch->stance[(ch->stance[0]+6)], STANCEPOWER_SPEED)
            && number_percent() < ch->stance[(ch->stance[0])] * 0.5 ) count += 2;
        else if (ch->stance[0] == STANCE_WOLF) count += 2;
        if (!IS_NPC(ch))
        {

            if (ch->pcdata->upgrade_level > 0) count += ch->pcdata->upgrade_level*2;
            /* if (ch->pcdata->upgrade_level > 4) count += ch->pcdata->upgrade_level - 4;  */
            if ( IS_CLASS(ch, CLASS_MILITARY))
                count+=6+(((ch->pcdata->Bootcamp[2])*2.75)+(ch->pcdata->Bootcamp[1]*1.75));
            if (IS_CLASS(ch, CLASS_WRAITH))
            {
                count += (ch->pdeath*2);
                if (ch->power[WRAITH_POWER] > 0)
                {
                    count += 20.5;
                }
            }
            if ( IS_CLASS(ch, CLASS_VAMPIRE) && ch->power[DISC_VAMP_CELE] > 0)
            {
                count += 2;
                if (ch->power[DISC_VAMP_CELE] > 5)
                {
                    count += 5;
                    if ( ch->power[DISC_VAMP_CELE] > 7)
                    {
                        count += 7;
                        if ( ch->power[DISC_VAMP_CELE] > 9)
                            count += 10;
                    }
                }
            }
            if (IS_CLASS(ch, CLASS_TANARRI)) count += 7 + (ch->pcdata->rank/2);
            if (IS_CLASS(ch, CLASS_MONK)) count += (6 * ch->monkab[COMBAT]);
            if (IS_CLASS(ch, CLASS_MONK)) count += 4;
            /*    if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_MECHA)) count += 30;*/
            if (IS_CLASS(ch, CLASS_DROID)) count += ch->pcdata->powers[CYBORG_LIMBS];
            if (IS_CLASS(ch, CLASS_ANGEL)) count += ch->pcdata->powers[ANGEL_JUSTICE];
            if (IS_CLASS(ch, CLASS_ANGEL)) count += ch->pcdata->powers[ANGEL_HARMONY];
            if (IS_CLASS(ch, CLASS_WRAITH)) count += 5 + ((ch->pkill)-(ch->pdeath))/3;

            if (IS_CLASS(ch, CLASS_WEREWOLF))
            {
                if (ch->power[DISC_WERE_LYNX] > 2 )count += 6;
                if (ch->power[DISC_WERE_BOAR] > 6)
                {
                    if( ch->move > 120000 ) count += 6;
                    else
                        count += ch->move/20000;
                    if (ch->stance[0] == STANCE_WOLF) count += 5;
                }
            }
            if (IS_CLASS(ch, CLASS_UNDEAD_KNIGHT))
                count += (ch->pcdata->powers[WEAPONSKILL]+3);
            if (IS_CLASS(ch, CLASS_LICH)) count += 4;
            if (IS_CLASS(ch, CLASS_SHAPESHIFTER))
            {
                count +=4;
                if (IS_SET(ch->pcdata->powers[SHAPE_FORM], BULL_FORM)) count += 5;
                if (IS_SET(ch->pcdata->powers[SHAPE_FORM], TIGER_FORM)) count += 6;
                if (IS_SET(ch->pcdata->powers[SHAPE_FORM], FAERIE_FORM)) count += 4;
                if (IS_SET(ch->pcdata->powers[SHAPE_FORM], HYDRA_FORM)) count += 9;
            }

            if ( IS_CLASS(ch, CLASS_DEMON))
            {
                if (IS_DEMPOWER( ch, DEM_SPEED)) count += 15;
                if (IS_SET(ch->warp, WARP_QUICKNESS)) count += 10;
            }
            if ( IS_CLASS(ch,CLASS_DRAGON))
            {
                count += ch->pcdata->classfour[DRAGON_SPIRIT];
                count += 7;
            }
            if (IS_CLASS(ch, CLASS_DROW))
            {
                count += 2;
                if (IS_SET(ch->special, SPC_DROW_WAR)) count += 2;
                if (IS_SET(ch->pcdata->powers[1],DPOWER_SPEED)) count += 6;
                if (IS_SET(ch->pcdata->powers[1], DPOWER_ARMS)) count += 2;
            }
        }
        if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_MAGE) && IS_ITEMAFF(ch, ITEMA_BEAST)) count += 2;
        if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_NINJA) && ch->pcdata->powers[NPOWER_CHIKYU] >= 3)
            count += 6;
        if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_NINJA) && ch->pcdata->rank == BELT_ONE)
            count += 10;
        if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_NINJA) && ch->pcdata->rank == BELT_TWO)
            count += 10;
        if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_NINJA) && ch->pcdata->rank == BELT_THREE)
            count += 11;
        if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_NINJA) && ch->pcdata->rank == BELT_FOUR)
            count += 11;
        if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_NINJA) && ch->pcdata->rank == BELT_FIVE)
            count += 12;
        if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_NINJA) && ch->pcdata->rank == BELT_SIX)
            count += 12;
        if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_NINJA) && ch->pcdata->rank == BELT_SEVEN)
            count += 13;
        if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_NINJA) && ch->pcdata->rank == BELT_EIGHT)
            count += 13;
        if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_NINJA) && ch->pcdata->rank == BELT_NINE)
            count += 14;
        if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_NINJA) && ch->pcdata->rank == BELT_TEN)
            count += 15;
        if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_WIZARD))
            count += 4;
        if (IS_CLASS(ch, CLASS_SAMURAI)) count += 8;
        if (IS_CLASS(ch, CLASS_WRAITH))
            count += 10;
        if ( IS_CLASS(ch, CLASS_PHOENIX))
        {
            count += 13;
            if (ch->pcdata->phoenix[0]>5) count += 12;
            if (ch->pcdata->phoenix[1]>3) count += 10;

        }

        if ( !IS_NPC(ch) && IS_CLASS(ch, CLASS_ELF))
        {
            count += 4;

            if (IS_SET(ch->special,SPC_ELF_WARRIOR))
            {
                count += (ch->pcdata->classtwo[ELF_WAR_FURY]) * 6;
            }
            if (IS_SET(ch->special,SPC_ELF_ARCHER))
            {
                count += ch->pcdata->classtwo[ELF_ARC_FIREARROW]+2;
                count += ch->pcdata->classtwo[ELF_ARC_ICEARROW]+2;
                count += ch->pcdata->classtwo[ELF_ARC_LIGHTARROW]+2;
            }
            if (IS_SET(ch->special,SPC_ELF_MAGE))
            {
                count += (ch->pcdata->classtwo[ELF_REDSTONE]);
                count += (ch->pcdata->classtwo[ELF_GREENSTONE]);
                count += (ch->pcdata->classtwo[ELF_BLUESTONE]);
            }
        }

        if (!IS_NPC(ch) && IS_CLASS(ch,CLASS_KAHN))
        {
            count += 8;
            if (IS_SET(ch->polyaff, POLY_ZULOFORM)) count += 10;
        }

        if ( IS_CLASS(ch, CLASS_RUNEMASTER) )
        {
            count += 2 + UMIN( 6-ch->generation, 4 );
            if ( is_rune_weapon(ch,"550000000") )
                count = UMAX( count-5, 1 );
        }
        if ( IS_ITEMAFF(ch, ITEMA_SPEED) ) count += 2;
    }
    else
    {
        if (!IS_NPC(ch)) if (ch->pcdata->upgrade_level > 0) count += ch->pcdata->upgrade_level*2;
        if ( IS_STANCE(ch, STANCE_VIPER) && number_percent() <  ch->stance[STANCE_VIPER] * 0.5) count += 1;
        else if ( IS_STANCE(ch, STANCE_MANTIS) && number_percent() < ch->stance[STANCE_MANTIS] * 0.5) count += 1;
        else if ( IS_STANCE(ch, STANCE_TIGER) && number_percent() < ch->stance[STANCE_TIGER] * 0.5) count += 1;
        else if ( ch->stance[0] > 12 && IS_SET(ch->stance[(ch->stance[0]+6)], STANCEPOWER_SPEED)
            && number_percent() < ch->stance[(ch->stance[0])] * 0.5 ) count += 2;
        if (!IS_NPC(ch))
        {
            if (IS_CLASS(ch, CLASS_DROID))
            {
                count += ch->pcdata->powers[CYBORG_LIMBS] + 2;
                count += ch->pcdata->powers[CYBORG_BODY];
            }
            if ( IS_CLASS(ch, CLASS_MILITARY))
                count+=6+(((ch->pcdata->Bootcamp[2])*2.75)+(ch->pcdata->Bootcamp[1]*1.75));

            if ( IS_CLASS(ch, CLASS_VAMPIRE))
            {
                count += 4;
                if (ch->power[DISC_VAMP_CELE] > 0) count += 3;
                if (ch ->power[DISC_VAMP_CELE] > 5) count += 5;
                if (ch->power[DISC_VAMP_CELE] > 7) count += 8;
                if ( ch->power[DISC_VAMP_CELE] > 9) count += 10;
            }
            if ( IS_CLASS(ch, CLASS_PHOENIX))
            {
                count += 11;
                if (ch->pcdata->phoenix[0]>5) count += 10;
                if (ch->pcdata->phoenix[1]>3) count += 8;

            }

            if ( IS_CLASS(ch, CLASS_MONK) && ch->monkab[COMBAT] > 0 )
            {
                count += ch->monkab[COMBAT] + 2;
                if ( get_eq_char(ch,WEAR_WIELD) == NULL ) count += 3;
                if (IS_CLASS(ch, CLASS_MONK) && (ch->chi[CURRENT] == 1)) count += 2;
                else if (IS_CLASS(ch, CLASS_MONK) && (ch->chi[CURRENT] == 2)) count += 3;
                else if (IS_CLASS(ch, CLASS_MONK) && (ch->chi[CURRENT] == 3)) count += 4;
                else if (IS_CLASS(ch, CLASS_MONK) && (ch->chi[CURRENT] == 4)) count += 5;
                else if (IS_CLASS(ch, CLASS_MONK) && (ch->chi[CURRENT] == 5)) count += 6;
                else if (IS_CLASS(ch, CLASS_MONK) && (ch->chi[CURRENT] == 6)) count += 7;
            }
            if ( IS_CLASS(ch, CLASS_MAGE) && IS_ITEMAFF(ch, ITEMA_BEAST)) count += 3;
            if (IS_CLASS(ch, CLASS_ANGEL)) count += 8;
            if (IS_CLASS(ch, CLASS_ANGEL)) count += ch->pcdata->powers[ANGEL_JUSTICE];
            if (IS_CLASS(ch, CLASS_WEREWOLF))
            {
                if ( ch->power[DISC_WERE_LYNX] > 2) count += 4;
                if ( ch->power[DISC_WERE_BOAR] > 6) count += 1;
                {
                    if (ch->move > 120000)
                        count +=6;
                    else count += (ch->move/20000);
                }
            }
            if (IS_CLASS(ch, CLASS_LICH))
            {
                count += 0 + ch->pcdata->powers[NECROMANTIC];
            }

            if (IS_CLASS(ch, CLASS_TANARRI))
            {
                count += 7 + ch->pcdata->rank;
                if (IS_SET(ch->pcdata->powers[TANARRI_POWER], TANARRI_SPEED)) count += 6;
            }
            if (IS_CLASS(ch, CLASS_UNDEAD_KNIGHT))
                count += ch->pcdata->powers[WEAPONSKILL]-3;
            if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_NINJA) && ch->pcdata->rank == BELT_ONE)
                count += 10;
            if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_NINJA) && ch->pcdata->rank == BELT_TWO)
                count += 10;
            if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_NINJA) && ch->pcdata->rank == BELT_THREE)
                count += 11;
            if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_NINJA) && ch->pcdata->rank == BELT_FOUR)
                count += 11;
            if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_NINJA) && ch->pcdata->rank == BELT_FIVE)
                count += 12;
            if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_NINJA) && ch->pcdata->rank == BELT_SIX)
                count += 12;
            if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_NINJA) && ch->pcdata->rank == BELT_SEVEN)
                count += 13;
            if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_NINJA) && ch->pcdata->rank == BELT_EIGHT)
                count += 13;
            if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_NINJA) && ch->pcdata->rank == BELT_NINE)
                count += 14;
            if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_NINJA) && ch->pcdata->rank == BELT_TEN)
                count += 18;
            if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_WIZARD))
                count += 8;
            if ( IS_ITEMAFF(ch, ITEMA_SPEED) ) count += 1;
            if (IS_CLASS(ch, CLASS_SAMURAI))
            {
                if (get_eq_char( ch, WEAR_HOLD ) == NULL)
                    count += 2;
                else
                    count +=8;
            }
            if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_MAGE) && IS_ITEMAFF(ch, ITEMA_BEAST)) count += 4;
            if (IS_CLASS(ch, CLASS_DEMON)&& ch->power[DISC_DAEM_ATTA] > 0) count += ch->power[DISC_DAEM_ATTA];

            if ( !IS_NPC(ch) && IS_CLASS(ch, CLASS_ELF))
            {
                count += 4;

                if (IS_SET(ch->special,SPC_ELF_WARRIOR))
                {
                    count += (ch->pcdata->classtwo[ELF_WAR_FURY]) * 2;
                }
                if (IS_SET(ch->special,SPC_ELF_ARCHER))
                {
                    count += ch->pcdata->classtwo[ELF_ARC_FIREARROW];
                    count += ch->pcdata->classtwo[ELF_ARC_ICEARROW];
                    count += ch->pcdata->classtwo[ELF_ARC_LIGHTARROW];
                }
                if (IS_SET(ch->special,SPC_ELF_MAGE))
                {
                    count += (ch->pcdata->classtwo[ELF_REDSTONE]) - 1;
                    count += (ch->pcdata->classtwo[ELF_GREENSTONE]) - 1;
                    count += (ch->pcdata->classtwo[ELF_BLUESTONE]) - 1;
                }
            }

            if ( IS_CLASS(ch,CLASS_DRAGON))
            {
                count += ch->pcdata->classfour[DRAGON_SPIRIT];

            }

            if (!IS_NPC(ch) && IS_CLASS(ch,CLASS_KAHN))
            {
                count += 7;
                if (IS_SET(ch->polyaff, POLY_ZULOFORM)) count += 8;
            }

            if (IS_CLASS(ch, CLASS_DROW))
            {
                count += 10;
                if (IS_SET(ch->special, SPC_DROW_WAR)) count += 4;
                if (IS_SET(ch->pcdata->powers[1],DPOWER_SPEED)) count += 8;
                if (IS_SET(ch->pcdata->powers[1], DPOWER_ARMS)) count += 4;
            }
            if ( IS_CLASS(ch, CLASS_RUNEMASTER) )
            {
                count += 2 + UMIN( 6-ch->generation, 4 );
                if ( is_rune_weapon(ch,"550000000") )
                    count = UMAX( count-5, 1 );
            }
        }
        if (ch->power[DISC_VAMP_CELE] >= 3)
            count -= victim->power[DISC_VAMP_CELE] / 5;
        if (victim->power[DISC_WERE_MANT] >= 3)
            count -= victim->power[DISC_WERE_MANT] / 3;
        else if (IS_ITEMAFF(victim, ITEMA_AFFMANTIS))
            count -= 1;
        if (victim->power[DISC_DAEM_ATTA] > 0)
            count -= victim->power[DISC_DAEM_ATTA] /2;
        if (number_range(1,4) == 2)
            count -= 1;
    }
    if ( !IS_NPC(ch) )
        //	count += blood_ratio(ch)/100;
        if ( count < 1)
            count = 1;
    if ( count > 50 )
        count = 50;
    return count;
}

/*
 * Hit one guy once.
 */
void one_hit( CHAR_DATA *ch, CHAR_DATA *victim, int dt, int handtype)
{
    OBJ_DATA *wield;
    int victim_ac;
    int thac0;
    int thac0_00;
    int thac0_32;
    int dam, wdam;
    int diceroll;
    int level;
    int attack_modify;
    int right_hand = 0;
    int left_hand = 0;
    char buf[MAX_STRING_LENGTH];

    /*
     * Can't beat a dead char!
     * Guard against weird room-leavings.
     */
    if (victim->position == POS_DEAD || ch->in_room != victim->in_room)
        return;
    if (!IS_NPC(victim) && worldpk) return;
    if ( !IS_NPC(ch) )
    {
        if (IS_AFFECTED( ch, AFF_PEACE) && ch->fighting != NULL)
            REMOVE_BIT(ch->affected_by, AFF_PEACE);
    }
    if ( !IS_NPC(victim) )
    {
        if (IS_AFFECTED(victim, AFF_PEACE) && victim->fighting != NULL)
            REMOVE_BIT(victim->affected_by, AFF_PEACE);
    }
    /*
     * Figure out the type of damage message.
     */
    if (handtype == 8)
    {
        wield = get_eq_char( ch, WEAR_FOURTH );
        right_hand = 8;
    }
    else if (handtype == 4)
    {
        wield = get_eq_char( ch, WEAR_THIRD );
        right_hand = 4;
    }
    else if (handtype == 2)
    {
        wield = get_eq_char( ch, WEAR_HOLD );
        right_hand = 2;
    }
    else if (handtype == 0)
    {
        wield = NULL;
        right_hand = 0;
    }
    else
    {
        if (IS_SET(ch->act, PLR_RIGHTHAND))
            wield = get_eq_char( ch, WEAR_WIELD );
        else if (IS_SET(ch->act, PLR_LEFTHAND))
            wield = get_eq_char( ch, WEAR_HOLD );
        else
        {
            if (number_range(1,10) > 4)
                wield = get_eq_char( ch, WEAR_HOLD );
            else
                wield = get_eq_char( ch, WEAR_WIELD );
        }
        right_hand = 1;                                     // improve the right hand anyway...
    }
    if ( dt == TYPE_UNDEFINED )
    {
        dt = TYPE_HIT;
        if ( wield != NULL && wield->item_type == ITEM_WEAPON )
            dt += wield->value[3];
    }
    if (ch->wpn[dt-1000] > 5)
    {
        if ( ch->name == "Jay")
        {
            sprintf(buf, "%s attacking %s wpn is %d.", ch->name, victim->name, ch->wpn[dt-1000]);
            log_string(buf);
        }
        level = (ch->wpn[dt-1000] / 5);
    }
    //jaytest
    else level = 1;
    if (level > (40 + (ch->wpn_lvl[dt-1000]*10) )) level = (40 + (ch->wpn_lvl[dt-1000]*10) );

    /*
     * Calculate to-hit-armor-class-0 versus armor.
     */
    if ( IS_NPC(ch) )
    {
        thac0_00 = 20;
        thac0_32 =  0;
    }
    else
    {
        thac0_00 = SKILL_THAC0_00;
        thac0_32 = SKILL_THAC0_32;
    }
    thac0     = interpolate( level, thac0_00, thac0_32 ) - char_hitroll(ch);
    victim_ac = UMAX( -100, char_ac(victim) / 10 );
    if (!can_see( ch, victim ))
        victim_ac -= 4;
    if ( !IS_NPC(ch) && IS_CLASS(ch, CLASS_DRAGON))
    {
        victim_ac += 1100;
    }
    if (ch->stance[0] > 12 && IS_SET(ch->stance[(ch->stance[0]+6)], STANCEPOWER_DAMAGE_3))
        victim_ac += 300;
    if (ch->stance[0] > 12 && IS_SET(ch->stance[(ch->stance[0]+6)], STANCEPOWER_DAMAGE_2))
        victim_ac += 200;
    if (ch->stance[0] > 12 && IS_SET(ch->stance[(ch->stance[0]+6)], STANCEPOWER_DAMAGE_1))
        victim_ac += 100;
    if ( IS_NPC(ch) && IS_SET(ch->act, ACT_KILLER) )
        victim_ac += ch->level;

    /*
     * The moment of excitement!
     */
    while ((diceroll = number_bits( 5 ) ) >= 20)
        ;

    if ( diceroll == 0 || ( diceroll != 19 && diceroll < thac0 - victim_ac ) )
    {                                                       /* Miss. */
        damage( ch, victim, 0, dt );
        tail_chain( );
        if (!is_safe(ch,victim))
        {
            improve_wpn(ch,dt,right_hand);
            improve_wpn(ch,dt,right_hand);
            improve_stance(ch);
            improve_stance(ch);
            improve_wpn(ch,dt,left_hand);
        }
        return;
    }

    /*
     * Hit.
     * Calc damage.
     */
    if ( IS_NPC(ch) )
    {
        dam = number_range( ch->level / 2, ch->level * 2 );
        if ( wield != NULL )
            dam += dam * 0.5;
        if ( IS_SET(ch->act, ACT_KILLER ) )
            dam *= 10;
    }
    else
    {
        if (dt == gsn_mageshield)
        {
            dam = (ch->spl[RED_MAGIC] + ch->spl[YELLOW_MAGIC] + ch->spl[GREEN_MAGIC]
                + ch->spl[PURPLE_MAGIC] + ch->spl[BLUE_MAGIC])/5;
            dam = number_range(1*dam, 2*dam);
        }
        else if (wield != NULL && wield->item_type == ITEM_WEAPON)
            dam = dice( wield->value[1], wield->value[2] );
        else if ( IS_VAMPAFF(ch, VAM_CLAWS) && IS_GAR1(ch, WOLF_RAZORCLAWS))
            dam = dice( 1000, 1500 );
        else if ( IS_VAMPAFF(ch,VAM_CLAWS))
            dam = dice( 10, 20 );
        else if ( IS_SET(ch->newbits, NEW_MONKADAM))
            dam = dice( 20, 25 );
        else
            dam = dice( 4, 10 );
        if (IS_CLASS(ch, CLASS_NINJA) && ch->pcdata->rank == BELT_SEVEN)
            dam += dice( 1, 7 );
        if (IS_CLASS(ch, CLASS_NINJA) && ch->pcdata->rank == BELT_EIGHT)
            dam += dice( 8, 14 );
        if (IS_CLASS(ch, CLASS_NINJA) && ch->pcdata->rank == BELT_NINE)
            dam += dice( 15, 21 );
        if (IS_CLASS(ch, CLASS_NINJA) && ch->pcdata->rank == BELT_TEN)
            dam += dice( 23, 30 );
        else
            dam = dice( 4, 8 );
    }
    /*
     * Bonuses.
     */
    /*  if ( !IS_NPC(victim) && (char_damroll(ch)+(char_ac(victim)/5) ) > 0 )
        dam += char_damroll(ch)+(char_ac(victim)/5);
      if (!IS_AWAKE(victim)) dam *= 2;
      if (!IS_NPC(ch) && dt >= TYPE_HIT) dam = dam + (dam * (UMIN(350,((ch->wpn[dt-1000])*(ch->wpn_lvl[dt-1000]+1)) / 60)));*/

    dam += char_damroll(ch);
    if (!IS_AWAKE(victim)) dam *= 2;
    if (!IS_NPC(ch) && dt >= TYPE_HIT) dam = dam + (dam * (UMIN(350,(ch->wpn[dt-1000]+1)) / 60));
    dam = up_dam(ch, victim, dam);

    /* Other Resistance */
    if (IS_ITEMAFF(victim, ITEMA_RESISTANCE))
    {
        dam *= 0.9;
        //    dam /= 4;
    }

    //  if (IS_NPC(ch) && dam > 2000) dam = 2000 + (dam-500)/2;    /* mob damage >500 halved */

    if ( dam <= 0 ) dam = 5;

    /* up that damage */
    if (!IS_NPC(ch))
    {
        if (ch->pcdata->upgrade_level == 1)
            dam *= 1.10;
        else if (ch->pcdata->upgrade_level == 2)
            dam *= 1.20;
        else if (ch->pcdata->upgrade_level == 3)
            dam *= 1.30;
        else if (ch->pcdata->upgrade_level == 4)
            dam *= 1.40;
        else if (ch->pcdata->upgrade_level == 5)
            dam *= 1.50;
        else if (ch->pcdata->upgrade_level == 6)
            dam *= 1.60;
        else if (ch->pcdata->upgrade_level == 7)
            dam *= 1.70;
        else if (ch->pcdata->upgrade_level == 8)
            dam *= 1.80;
        else if (ch->pcdata->upgrade_level == 9)
            dam *= 1.90;
        else if (ch->pcdata->upgrade_level == 10)
            dam *= 2.00;

        /*  dam *= (float)1+(float)((float).10*(float)ch->pcdata->upgrade_level);   */
        /* 10% extra damage per level */
    }

    dam += (blood_ratio(ch)*dam)/100;

    /*
     * I doubt this has much effect - Jobo
     */
    if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_VAMPIRE) && IS_NPC(victim))
        dam *= (1 + ch->power[DISC_VAMP_POTE]/10);

    if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_TANARRI))
    {
        if (IS_SET(ch->pcdata->powers[TANARRI_POWER], TANARRI_MIGHT)) dam *= 2.5;
        if (ch->pcdata->powers[TANARRI_FURY_ON] == 1) dam *= 2.5;
    }

    if ( !IS_NPC(ch) && IS_CLASS(ch, CLASS_RUNEMASTER) )
        dam *= 2;

    /*
     * This one for angels have no effect, take a look and rebalance. Jobo
     */
    if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_ANGEL)) dam *= (1 + ch->pcdata->powers[ANGEL_JUSTICE] / 10);
    if ( !IS_NPC(ch) && IS_CLASS(ch, CLASS_DEMON) &&  IS_DEMPOWER(ch, DEM_MIGHT) )
        dam *= 1.1;
    if (!IS_NPC(ch) && !IS_NPC(victim) && IS_CLASS(ch, CLASS_UNDEAD_KNIGHT) && IS_CLASS(victim, CLASS_SHAPESHIFTER)) dam *= 1.2;
    if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_MAGE))
    {
        if (dt==gsn_mageshield && ch->pcdata->powers[PINVOKE]>6) dam *=1.4;
        if (dt==gsn_mageshield && ch->pcdata->powers[PINVOKE]>9) dam *=1.4;
    }
    if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_KAHN) && IS_SET(ch->polyaff, POLY_ZULOFORM))
        dam *= 2.75;
    if ( !IS_NPC(ch) && IS_CLASS(ch, CLASS_MILITARY))
    {
        if (ch->pcdata->Bootcamp[3] == 1) dam *= 2.7;
        else if (ch->pcdata->Bootcamp[3] == 2) dam *= 2.8;
        else if (ch->pcdata->Bootcamp[3] == 3) dam *= 2.9;
        else if (ch->pcdata->Bootcamp[3] == 4) dam *= 3.0;
        else if (ch->pcdata->Bootcamp[3] == 5) dam *= 3.1;
        else if (ch->pcdata->Bootcamp[3] == 6) dam *= 3.25;
        else if (ch->pcdata->Bootcamp[3] == 7) dam *= 3.4;
        else if (ch->pcdata->Bootcamp[3] == 8) dam *= 3.65;
        else if (ch->pcdata->Bootcamp[3] == 9) dam *= 3.8;
        else if (ch->pcdata->Bootcamp[3] == 10) dam *= 3.95;
        else if (ch->pcdata->Bootcamp[3] == 0) dam *= 1;
    }

    if ( !IS_NPC(ch) && IS_CLASS(ch, CLASS_NINJA))
    {
        if (ch->pcdata->rank == BELT_ONE) dam *= 1.9;
        else if (ch->pcdata->rank == BELT_TWO) dam *= 2.0;
        else if (ch->pcdata->rank == BELT_THREE) dam *= 2.1;
        else if (ch->pcdata->rank == BELT_FOUR) dam *= 2.2;
        else if (ch->pcdata->rank == BELT_FIVE) dam *= 2.3;
        else if (ch->pcdata->rank == BELT_SIX) dam *= 2.4;
        else if (ch->pcdata->rank == BELT_SEVEN) dam *= 2.5;
        else if (ch->pcdata->rank == BELT_EIGHT) dam *= 2.6;
        else if (ch->pcdata->rank == BELT_NINE) dam *= 2.7;
        else if (ch->pcdata->rank == BELT_TEN) dam *= 2.9;
    }

    if ( !IS_NPC(ch) && IS_CLASS(ch, CLASS_ELF))
    {
        if (IS_SET(ch->special,SPC_ELF_WARRIOR))
        {
            if (ch->pcdata->classtwo[ELF_WAR_RAGE] > 0) dam *= 1.9;
            else if (ch->pcdata->classtwo[ELF_WAR_RAGE] > 1) dam *= 2.0;
            else if (ch->pcdata->classtwo[ELF_WAR_RAGE] > 2) dam *= 2.1;
            else if (ch->pcdata->classtwo[ELF_WAR_RAGE] > 3) dam *= 2.2;
            else if (ch->pcdata->classtwo[ELF_WAR_RAGE] > 4) dam *= 2.3;
        }
        if (IS_SET(ch->special,SPC_ELF_ARCHER)) dam *= 2.8;
        if (IS_SET(ch->special,SPC_ELF_MAGE)) dam *= 1.5;
    }

    if(!IS_NPC(ch) && IS_CLASS(ch, CLASS_PHOENIX)) dam*=100.2;

    if ( !IS_NPC(ch) && IS_CLASS(ch, CLASS_DEMON) &&  IS_SET(ch->warp, WARP_STRONGARMS))
        dam   *= 1.2;

    if(IS_NPC(victim))
    {

        if ( ch->stance[0] > 12 && IS_SET(ch->stance[(ch->stance[0]+6)], STANCEPOWER_DAMAGE_3)
            && ch->stance[(ch->stance[0])] >100)
            dam += dam * 1.5 * (ch->stance[(ch->stance[0])] / 200);
        else if ( ch->stance[0] > 12 && IS_SET(ch->stance[(ch->stance[0]+6)], STANCEPOWER_DAMAGE_2)
            && ch->stance[(ch->stance[0])] >100)
            dam += dam * 1.0 * (ch->stance[(ch->stance[0])] / 200);
        else if ( ch->stance[0] > 12 && IS_SET(ch->stance[(ch->stance[0]+6)], STANCEPOWER_DAMAGE_1)
            && ch->stance[(ch->stance[0])] >100)
            dam += dam * 0.5 * (ch->stance[(ch->stance[0])] / 200);
    }

    if(!IS_NPC(victim))
    {

        if ( ch->stance[0] > 12 && IS_SET(ch->stance[(ch->stance[0]+6)], STANCEPOWER_DAMAGE_3)
            && ch->stance[(ch->stance[0])] >100)
            dam += dam * 0.5 * (ch->stance[(ch->stance[0])] / 200);
        else if ( ch->stance[0] > 12 && IS_SET(ch->stance[(ch->stance[0]+6)], STANCEPOWER_DAMAGE_2)
            && ch->stance[(ch->stance[0])] >100)
            dam += dam * 0.3 * (ch->stance[(ch->stance[0])] / 200);
        else if ( ch->stance[0] > 12 && IS_SET(ch->stance[(ch->stance[0]+6)], STANCEPOWER_DAMAGE_1)
            && ch->stance[(ch->stance[0])] >100)
            dam += dam * 0.1 * (ch->stance[(ch->stance[0])] / 200);
    }

    if ( victim->stance[0] > 12 && IS_SET(victim->stance[(victim->stance[0]+6)], STANCEPOWER_RESIST_3)
        && victim->stance[(victim->stance[0])] >100)
        dam *= 0.7;
    else if ( victim->stance[0] > 12 && IS_SET(victim->stance[(victim->stance[0]+6)], STANCEPOWER_RESIST_2)
        && victim->stance[(victim->stance[0])] >100)
        dam *= 0.8;
    else if ( victim->stance[0] > 12 && IS_SET(victim->stance[(victim->stance[0]+6)], STANCEPOWER_RESIST_1)
        && victim->stance[(victim->stance[0])] >100)
        dam *= 0.9;

    /* The test ends here */

    if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_SHAPESHIFTER))
    {
        dam *= 1.4;
        if (ch->pcdata->powers[SHAPE_FORM] == TIGER_FORM && ch->pcdata->powers[TIGER_LEVEL] > 1) dam *= 1.2;
        else if (ch->pcdata->powers[SHAPE_FORM] == FAERIE_FORM && ch->pcdata->powers[FAERIE_LEVEL] > 1) dam *= 1.2;
        else if (ch->pcdata->powers[SHAPE_FORM] == HYDRA_FORM && ch->pcdata->powers[HYDRA_LEVEL] > 1) dam *= 1.6;
        else if (ch->pcdata->powers[SHAPE_FORM] == BULL_FORM && ch->pcdata->powers[BULL_LEVEL] > 1) dam *= 1.7;
        else dam = dam;
    }
    if(!IS_NPC(ch) && IS_CLASS(ch, CLASS_MILITARY))
    {
        int jmod;
        jmod=(ch->pcdata->Bootcamp[0]);
        jmod/=9;
        jmod+=1;
        dam*=jmod;
    }
    if (IS_CLASS(ch,CLASS_DROID) && !IS_NPC(ch))
    {
        if (ch->pcdata->powers[CYBORG_LIMBS] > 0) dam *= 1.8;
        if (ch->pcdata->powers[CYBORG_LIMBS] > 2) dam *= 2.0;
    }
    if (IS_CLASS(ch, CLASS_UNDEAD_KNIGHT))
    {
        if (ch->pcdata->powers[WEAPONSKILL] > 4) dam *=1.1;
        if (ch->pcdata->powers[WEAPONSKILL] > 8) dam *=1.2;
    }
    if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_WEREWOLF) && ch->power[DISC_WERE_BEAR] > 5)
        dam *= 1.2;
    if (IS_CLASS(ch, CLASS_WRAITH))
        dam *= 2.2;
    if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_SHAPESHIFTER))
    {
        if (victim->pcdata->powers[SHAPE_FORM] == FAERIE_FORM && victim->pcdata->powers[FAERIE_LEVEL]>0)
        {
            int growl = number_range(1,50);
            if (growl == 10 || growl == 20 || growl == 30 || growl == 40 || growl == 50)
            {
                stc("#3You dodge your opponents attack#n.\n\r",victim);
                stc("#3Your opponent dodges your attack#n.\n\r",ch);
                dam = 0;
            }
        }
    }
    /*if (!IS_NPC(ch) && IS_CLASS(victim, CLASS_MILITARY))
    {//if (victim->pcdata->Bootcamp[4]>0)

        int sgrowl = number_range(1,20);
        sgrowl += victim->pcdata->Bootcamp[4];
        if (sgrowl == 21 || sgrowl == 23 || sgrowl ==25  || sgrowl == 27 || sgrowl == 29)
        {
          stc("#3Your high endurance allows you to shake off your opponents attack#n.\n\r",victim);
          stc("#3Your opponent shakes off your attack#n.\n\r",ch);
          dam = 0;
        }

    }*/
    if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_WRAITH))
    {
        int block = number_range(1,50);
        if (block == 10 || block == 20 || block == 30 || block == 40 || block == 50)
        {
            stc("#0You phase out of sight and your opponent misses!#n\n\r",victim);
            stc("#0Your opponent phases out of sight and you hit air!#n\n\r",ch);
            dam = 0;
        }
    }
    if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_DROW) && IS_SET(victim->newbits, NEW_DARKTENDRILS))
    {
        char buf[512];
        int powheal;
        int ddodge = number_range(1,50);
        if (ddodge == 10 || ddodge == 20 || ddodge == 30 || ddodge == 40 || ddodge == 50)
        {
            powheal = (victim->hit / 500);
            victim->hit += powheal;
            if(victim->hit > victim->max_hit) victim->hit = victim->max_hit;
            sprintf(buf,"#0Your dark aura absorbs the damage.[#l**#R%d#l**#r]#n\n\r", powheal);
            send_to_char(buf, victim);
            stc("#0Your opponent's dark aura absorbs your damage#n.\n\r",ch);
            dam = 0;
        }
    }
    /*  if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_MECHA) && IS_MECHAFF(victim, MECHA_FORCEFIELD))
      {
        if (number_range(1,11) == 2)
        {
          stc("#0Your forcefield flares brightly stopping the blow in its tracks.#n\n\r",victim);
          stc("#0Your opponents forcefield flares brightly stopping your blow in its tracks.#n\n\r",ch);
          dam = 0;
        }
      }
    */
    if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_MAGE) && IS_ITEMAFF(victim, ITEMA_ILLUSIONS))
    {
        int ddodge = number_range(1,50);
        if (ddodge == 1 || ddodge == 10 || ddodge == 20 || ddodge == 30 || ddodge == 40)
        {
            stc("#CYour illusion takes the blow, saving you from harm#n.\n\r",victim);
            stc("#CYour weapon passes through your opponent without doing any harm#n.\n\r",ch);
            dam = 0;
        }
    }
    if(!IS_NPC(victim) && IS_CLASS(victim, CLASS_DROID))
    {
        dam = UMAX((dam * .8), (((dam * 100) - (victim->pcdata->powers[CYBORG_BODY] * 4)) / 100));
    }
    if (!IS_NPC(ch) && wield != NULL)
    {
        if (number_range(1,6) == 2)
        {
            if (ch->fighting == victim && IS_WEAP(wield,WEAPON_VAMPIRIC))
            {
                wdam = number_range(1,wield->level / 5 + 1);
                act("$p draws life from $n.",victim,wield,NULL,TO_ROOM);
                act("You feel $p drawing your life away.",
                    victim,wield,NULL,TO_CHAR);
                hurt_person(ch,victim, wdam);
                if (victim->hit <= 0) return;
            }
            if (ch->fighting == victim && (IS_WEAP(wield,WEAPON_FLAMING) || IS_WEAP(wield, WEAPON_ELE_FLAME)))
            {
                wdam = number_range(1,wield->level / 4 + 1);
                if (!IS_SET(ch->act, PLR_BRIEF2)) act("$N is burned by your $p.", ch, wield, victim, TO_CHAR);
                if (!IS_SET(victim->act, PLR_BRIEF2)) act("You are burned by $n's $p.", ch, wield, victim, TO_VICT);
                fire_effect( (void *) victim,wield->level/2,wdam,TARGET_CHAR);
                hurt_person(ch, victim, wdam);
                if (victim->hit <= 0) return;
            }
            if (ch->fighting == victim && (IS_WEAP(wield,WEAPON_FROST) || IS_WEAP(wield, WEAPON_ELE_WATER)))
            {
                int sn = skill_lookup("chill touch");
                if (!is_affected(victim,sn))
                {
                    wdam = number_range(1,wield->level / 6 + 2);
                    if (!IS_SET(ch->act, PLR_BRIEF2)) act("Your $p freezes $N.", ch, wield, victim, TO_CHAR);
                    if (!IS_SET(victim->act, PLR_BRIEF2)) act("You are frozen by $n's $p.", ch, wield, victim, TO_VICT);
                    cold_effect(victim,wield->level/2,wdam,TARGET_CHAR);
                    hurt_person(ch,victim, wdam);
                }
                if (victim->hit <= 0) return;
            }
            if (ch->fighting == victim && IS_WEAP(wield, WEAPON_ELE_EARTH) )
            {
                wdam = number_range(1,wield->level / 5 + 2);
                act("$p covers $n in acid.",victim,wield,NULL,TO_ROOM);
                act("The corrosive touch of $p surrounds you with acid.",
                    victim,wield,NULL,TO_CHAR);
                acid_effect(victim,wield->level/2,wdam,TARGET_CHAR);
                hurt_person(ch,victim, wdam);
                if (victim->hit <= 0) return;
            }
            if (ch->fighting == victim && (IS_WEAP(wield,WEAPON_SHOCKING)  || IS_WEAP(wield, WEAPON_ELE_AIR)))
            {
                wdam = number_range(1,wield->level/5 + 2);
                act("$n is struck by lightning from $p.",victim,wield,NULL,TO_ROOM);
                act("You are shocked by $p.",victim,wield,NULL,TO_CHAR);
                shock_effect(victim,wield->level/2,wdam,TARGET_CHAR);
                hurt_person(ch,victim, wdam);
                if (victim->hit <= 0) return;
            }
        }
    }
    if ( !IS_NPC(ch) && IS_CLASS(ch, CLASS_MONK) && dt != gsn_heavenlyaura )
    {
        if (ch->chi[CURRENT] > 0 && ch->chi[CURRENT] < 3 ) dam *= 1.3;
        if (ch->chi[CURRENT] > 2 && ch->chi[CURRENT] < 7 ) dam *= ((ch->chi[CURRENT]) / 2);
    }
    if ( dt == gsn_garotte )
        dam *= (number_range(10,16));

    if ( dt == gsn_backstab )
    {
        if (IS_CLASS(ch, CLASS_NINJA) && !IS_NPC(ch) && ch->pcdata->powers[NPOWER_NINGENNO] >=2)
            dam *= number_range(50,60);
        else if( IS_CLASS(ch, CLASS_DROW))
        {
            dam += number_range(100,1000);
            dam *= number_range(7,10);
        }
        else dam *= number_range(2,4);
    }

    /*
     * Damage type modifiers.
     */
    if (dt == gsn_stuntubes && IS_CLASS(ch, CLASS_DROID))         dam *= 2.5;
    if (dt == gsn_laserbeam && IS_CLASS(ch, CLASS_DROID))         dam *= 3.0;
    if (dt == gsn_garotte && IS_CLASS(ch, CLASS_DROW))            dam *= 12.8;
    if (dt == gsn_magma && IS_CLASS(ch, CLASS_TANARRI))           dam *= 4.3;
    if (dt == gsn_booming && !IS_NPC(ch))                 dam *= ch->pcdata->rank/2;
    if (dt == gsn_circle && ch->pcdata->rank == BELT_EIGHT)       dam *= 5.0;
    if (dt == gsn_circle && ch->pcdata->rank == BELT_NINE)        dam *= 5.5;
    if (dt == gsn_circle && ch->pcdata->rank == BELT_TEN)         dam *= 6.0;
    if (dt == gsn_shiroken && ch->pcdata->rank == BELT_TEN)       dam *= 6.5;
    if (dt == gsn_heavenlyaura && IS_CLASS(ch, CLASS_MONK))       dam /= 30;
    if (dt == gsn_heavenlyaura && IS_CLASS(ch, CLASS_ANGEL))      dam *= 7.0;
    if (dt == gsn_wrathofgod && IS_CLASS(ch, CLASS_ANGEL))        dam *= 7.5;
    if (dt == gsn_wrathofgod && IS_CLASS(ch, CLASS_MONK))     dam *= 2.5;
    if (dt == gsn_chillhand && IS_CLASS(ch, CLASS_LICH))          dam *= 3.0;
    if (dt == gsn_fireball && IS_CLASS(ch, CLASS_LICH))           dam *= 3.0;
    if (dt == gsn_deathaura && IS_CLASS(ch, CLASS_LICH))          dam *= 3.0;
    if (dt == gsn_lightningkick && IS_CLASS(ch, CLASS_MONK) )     dam *= 3.7;
    if (dt == gsn_lightningslash && IS_CLASS(ch, CLASS_SAMURAI) && IS_CLASS(ch, CLASS_UNDEAD_KNIGHT))  dam *= 3.9;
    if (dt == gsn_deathaura)                      dam /= 1.7;
    if (dt == gsn_potato)                         dam /= 2;
    if (dt == gsn_shred)                          dam *= 10;
    if (dt == gsn_frost && IS_CLASS(ch, CLASS_DEMON))         dam *= 2.5;
    if (dt == gsn_quills && IS_CLASS(ch, CLASS_WEREWOLF))         dam *= 3.2;
    if (dt == gsn_cheapshot && IS_CLASS(ch, CLASS_WEREWOLF))      dam *= 2.2;
    if (dt == gsn_elvenmagic && IS_CLASS(ch, CLASS_ELF))          dam *= 2.5;
    if (dt == gsn_elvenmagic && IS_CLASS(ch, CLASS_WIZARD))       dam *= 2.8;
    if (dt == gsn_elfstones && IS_CLASS(ch, CLASS_ELF))           dam *= 69.5;
    if (dt == gsn_spiket && IS_CLASS(ch, CLASS_DRAGON))           dam *= 2.5;
    if (dt == gsn_mageshield && IS_CLASS(ch, CLASS_MAGE))         dam *= 1.5;
    if (dt == gsn_backstab && IS_CLASS(ch, CLASS_NINJA))                  dam *= 1.75;
    if (dt == gsn_blinky && IS_CLASS(ch, CLASS_DEMON))                    dam *= 2.50;
    if (dt == gsn_swoop && IS_CLASS(ch, CLASS_ANGEL))                     dam *= 75.0;

    // Elf Archers dam modifiers for arrows.

    if (dt == gsn_firearrow && ch->pcdata->classtwo[ELF_ARC_FIREARROW] > 1)   dam *= 2.4;
    if (dt == gsn_firearrow && ch->pcdata->classtwo[ELF_ARC_FIREARROW] > 2)   dam *= 2.6;
    if (dt == gsn_firearrow && ch->pcdata->classtwo[ELF_ARC_FIREARROW] > 3)   dam *= 2.9;
    if (dt == gsn_firearrow && ch->pcdata->classtwo[ELF_ARC_FIREARROW] > 4)   dam *= 3.0;
    if (dt == gsn_icearrow && ch->pcdata->classtwo[ELF_ARC_ICEARROW] > 1)     dam *= 2.4;
    if (dt == gsn_icearrow && ch->pcdata->classtwo[ELF_ARC_ICEARROW] > 2)     dam *= 2.6;
    if (dt == gsn_icearrow && ch->pcdata->classtwo[ELF_ARC_ICEARROW] > 3)     dam *= 2.8;
    if (dt == gsn_icearrow && ch->pcdata->classtwo[ELF_ARC_ICEARROW] > 4)     dam *= 3.0;
    if (dt == gsn_lightningarrow && ch->pcdata->classtwo[ELF_ARC_LIGHTARROW] > 1) dam *= 2.4;
    if (dt == gsn_lightningarrow && ch->pcdata->classtwo[ELF_ARC_LIGHTARROW] > 2) dam *= 2.6;
    if (dt == gsn_lightningarrow && ch->pcdata->classtwo[ELF_ARC_LIGHTARROW] > 3) dam *= 2.8;
    if (dt == gsn_lightningarrow && ch->pcdata->classtwo[ELF_ARC_LIGHTARROW] > 4) dam *= 3.0;

    // Elf warriors dam modifiers

    if (dt == gsn_swordslash && ch->pcdata->classtwo[ELF_WAR_RAGE] > 0)       dam *= 2.8;
    if (dt == gsn_swordslash && ch->pcdata->classtwo[ELF_WAR_RAGE] > 1)       dam *= 2.9;
    if (dt == gsn_swordslash && ch->pcdata->classtwo[ELF_WAR_RAGE] > 2)       dam *= 4.0;
    if (dt == gsn_swordslash && ch->pcdata->classtwo[ELF_WAR_RAGE] > 3)       dam *= 4.1;
    if (dt == gsn_swordslash && ch->pcdata->classtwo[ELF_WAR_RAGE] > 4)       dam *= 4.2;

    if (dt == gsn_wishsong && ch->pcdata->classtwo[ELF_WAR_RAGE] > 0)     dam *= 4;
    if (dt == gsn_wishsong && ch->pcdata->classtwo[ELF_WAR_RAGE] > 1)     dam *= 4.2;
    if (dt == gsn_wishsong && ch->pcdata->classtwo[ELF_WAR_RAGE] > 2)     dam *= 4.4;
    if (dt == gsn_wishsong && ch->pcdata->classtwo[ELF_WAR_RAGE] > 3)     dam *= 4.6;
    if (dt == gsn_wishsong && ch->pcdata->classtwo[ELF_WAR_RAGE] > 4)     dam *= 4.8;
    dam = cap_dam(ch,victim,dam);
    if( IS_NPC(ch) && ch->pIndexData->hitplus > 3) dam *=3;
    else if( IS_NPC(ch) && ch->pIndexData->hitplus > 0 ) dam *= ch->pIndexData->hitplus;

    if( IS_NPC(victim) && !IS_NPC(ch))
    {
        if( victim->pIndexData->hitnodice > 50 && victim->level < 100000)
            victim->pIndexData->hitnodice = 50;
        dam = dam * (100 - victim->pIndexData->hitnodice) / 65;
    }

    attack_modify = dice(1, 150);
    /* damage reduction */
    if (!IS_NPC(ch) && !IS_NPC(victim)) dam /= 3;           /* slow down pk */

    randomize_damage(ch,dam,attack_modify);
                                                            /* trust me, Jobo */
    if (dt == gsn_deathaura && dam >= victim->hit && IS_NPC(victim)) dam = victim->hit-1;
    if(!IS_NPC(victim)&&IS_CLASS(victim,CLASS_PHOENIX)&&victim->pcdata->phoenix[1]>3)dam*=.7;
    if(IS_CLASS(victim,CLASS_NINJA))dam*=.83;
    if(IS_CLASS(ch,CLASS_LICH))dam*=.50;
    if(IS_CLASS(victim,CLASS_LICH))dam*=1.50;

    if(IS_CLASS(ch,CLASS_UNDEAD_KNIGHT))dam*=.65;

    if(IS_CLASS(ch,CLASS_NINJA))dam*=1.2;

    damage( ch, victim, dam, dt );

    tail_chain( );
    if (!is_safe(ch,victim))
    {
        improve_wpn(ch,dt,right_hand);
        improve_wpn(ch,dt,left_hand);
        improve_stance(ch);
    }
}

int randomize_damage( CHAR_DATA *ch, int dam, int am )
{
    dam = dam * (am + 55) / 80;
    return dam;
}

int cap_dam(CHAR_DATA *ch, CHAR_DATA *victim, int dam)
{
    if (is_safe(ch, victim)) return 0;

    //removing your code beorn due to a bug. putting in old fight.c until its fixed.

    // cleaner version below - Beorn
    if (!IS_NPC(victim))
    {
        if (victim->pcdata->upgrade_level == 1)
            dam *= 0.95;
        else if (victim->pcdata->upgrade_level == 2)
            dam *= 0.90;
        else if (victim->pcdata->upgrade_level == 3)
            dam *= 0.85;
        else if (victim->pcdata->upgrade_level == 4)
            dam *= 0.80;
        else if (victim->pcdata->upgrade_level == 5)
            dam *= 0.75;
        else if (victim->pcdata->upgrade_level == 6)
            dam *= 0.70;
        else if (victim->pcdata->upgrade_level == 7)
            dam *= 0.65;
        else if (victim->pcdata->upgrade_level == 8)
            dam *= 0.60;
        else if (victim->pcdata->upgrade_level == 9)
            dam *= 0.55;
        else if (victim->pcdata->upgrade_level == 10)
            dam *= 0.50;

        // dam *= .95-(.5*ch->pcdata->upgrade_level);
        /* 5% less damage per level */

    }

    if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_DROW))
    {
        if (IS_SET(victim->pcdata->powers[1], DPOWER_TOUGHSKIN))
            dam /= 5;
    }
    if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_WIZARD))
    {
        dam *= 0.75;
    }
    if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_LICH))
    {
        dam *= 0.20;
        if (IS_CLASS(ch, CLASS_TANARRI)) dam *= 0.40;
        if (IS_CLASS(ch, CLASS_DROID)) dam *= 0.60;
    }
    if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_DRAGON))
    {
        dam *= (100 - (victim->pcdata->classfour[DRAGON_AGE] * 2));
        dam /= 50;
    }
    if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_ANGEL))
    {
        dam *= (100 - (victim->pcdata->powers[ANGEL_HARMONY] * 12));
        dam /= 100;
    }
    if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_TANARRI))
    {
        dam *= 0.6;
        if (IS_SET(victim->pcdata->powers[TANARRI_POWER], TANARRI_EXOSKELETON)) dam *= 0.20;
        if (IS_CLASS(ch, CLASS_ANGEL)) dam *= 0.75;         /* trained to kill angels */
        if (IS_CLASS(ch, CLASS_DROID)) dam *= 0.7;
        if (IS_CLASS(ch, CLASS_LICH)) dam *= 0.35;
        if (IS_CLASS(ch, CLASS_KAHN)) dam *= 0.65;
        if (IS_CLASS(ch, CLASS_UNDEAD_KNIGHT)) dam *= 0.15;
    }

    if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_DROID))
    {
        if (IS_CLASS(ch, CLASS_SHAPESHIFTER)) dam *= 0.85;
        if (IS_CLASS(ch, CLASS_SAMURAI)) dam *= 0.9;
        if (victim->pcdata->powers[CYBORG_BODY] > 0) dam /= 1.75;
    }
    if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_VAMPIRE))
    {
        if (victim->power[DISC_VAMP_PROT] >= 10) dam /= 1.5;
        if (victim->power[DISC_VAMP_FORT] > 0)
        {
            dam *= (100 - (victim->power[DISC_VAMP_FORT] * 5.5));
            dam /= 150;
        }
        if (IS_CLASS(ch, CLASS_WEREWOLF)) dam /= 1.1;       // Natural resistance to WW.  Gaia vs. Wyrm
    }
    if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_SAMURAI))
    {
        if (IS_CLASS(ch, CLASS_SHAPESHIFTER)) dam /= 1.8;   /* damn shapies */
        dam /= 1.75;
    }
    if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_MAGE))
    {
        if (IS_ITEMAFF(victim, ITEMA_STEELSHIELD)) dam /= 2.8;
        if (IS_ITEMAFF(victim, ITEMA_BEAST))       dam /= 2.8;
    }

    /*  if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_MECHA))
      {
        if (IS_MECHAFF(victim, MECHA_TITANIUMSKIN)) dam /= 1.8;
      }
    */

    if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_ELF))     /*  Reduce damage */
    {
        if (IS_SET(ch->special,SPC_ELF_MAGE))     dam /= 1.8;
        if (IS_SET(ch->special,SPC_ELF_ARCHER))   dam /= 1.3;
    }
    if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_KAHN))
    {
        if (IS_SET(ch->polyaff, POLY_ZULOFORM))
            dam *= .50;
        else
            dam *= .85;
                                                            /* Pk Balance */
        if (!IS_NPC(victim) && IS_CLASS(ch, CLASS_ELF)) dam *= .75;
        //    if (IS_CLASS(ch, CLASS_UNDEAD_KNIGHT)) dam *= 0.95; /* Pk Balance */
    }

    if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_SHAPESHIFTER))
    {
        dam *= .95;
        if (victim->pcdata->powers[SHAPE_FORM] == FAERIE_FORM)
        {
            dam *= (4 + victim->pcdata->powers[FAERIE_LEVEL]*(.1));
        }
        else if (victim->pcdata->powers[SHAPE_FORM] == TIGER_FORM)
        {
            dam *= .99;
            if (victim->pcdata->powers[PHASE_COUNTER] > 0)
            {
                dam *= (100 - victim->pcdata->powers[PHASE_COUNTER] * 2.5);
                dam /= 10;
            }
        }
        else if (victim->pcdata->powers[SHAPE_FORM] == HYDRA_FORM)
        {
            dam *= .95;
        }
        else if (victim->pcdata->powers[SHAPE_FORM] == BULL_FORM)
        {
            dam *= 1.02;
        }
        else dam = dam;
    }
    if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_MILITARY))
    {
        dam /= 1.7;
        //  if (victim->pcdata->Bootcamp[4]>0)
        //dam*=(1-((ch->pcdata->Bootcamp[4])/25));
    }

    if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_MONK))
    {
        if (victim->monkab[BODY] >= 2) dam /= 1.5;
        if (victim->chi[CURRENT] > 0 )
        {
            dam *= (100 - (victim->chi[CURRENT] * 7) );
            dam /= 100;
        }
    }
    if(!IS_NPC(victim) && IS_CLASS(victim, CLASS_PHOENIX)) dam*=.8;

    if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_DEMON))
    {
        if (victim->power[DISC_DAEM_IMMU] > 0)
        {
            dam *= (100 - (victim->power[DISC_DAEM_IMMU] * 4));
            dam /= 100;
        }
        if (IS_DEMPOWER( victim, DEM_TOUGH))    dam *= 0.8;
        if (IS_SET(victim->warp, WARP_STAMINA)) dam *= 0.8;
        if (IS_SET(victim->warp, WARP_SBODY))   dam *= 0.8;
        if (IS_SET(victim->warp, WARP_CBODY))   dam *= 0.8;
    }
    if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_UNDEAD_KNIGHT))
    {
        if (IS_CLASS(ch, CLASS_SHAPESHIFTER)) dam *= 0.75;
        if (IS_CLASS(ch, CLASS_SAMURAI)) dam *= 0.7;
        dam *= (100 - (victim->pcdata->powers[UNDEAD_SPIRIT] * 6));
        dam /= 100;
    }
    if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_NINJA))
    {
        if (victim->pcdata->powers[NPOWER_CHIKYU] >= 1) dam /= 3.8;
    }

    if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_ELF))
    {
        if (victim->pcdata->classtwo[ELF_WAR_RAGE] > 0) dam *= .95;
        if (victim->pcdata->classtwo[ELF_WAR_RAGE] > 1) dam *= .90;
        if (victim->pcdata->classtwo[ELF_WAR_RAGE] > 2) dam *= .85;
        if (victim->pcdata->classtwo[ELF_WAR_RAGE] > 3) dam *= .80;
        if (victim->pcdata->classtwo[ELF_WAR_RAGE] > 4) dam *= .75;
    }

    if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_WEREWOLF))
    {
        if(victim->power[DISC_WERE_BOAR] > 2) dam *= 0.5;
        if(IS_GAR1(victim, WOLF_COCOON))      dam *= 0.5;
        if (IS_CLASS(ch, CLASS_VAMPIRE)) dam *= 0.5;        // Natural resistance to Wyrm.  Gaia vs. Wyrm
    }
    if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_WIZARD))
    {
        dam *= 0.65;
    }
    if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_WRAITH))
    {
        if (victim->pcdata->powers[WRAITH_DEATH] >= 4) dam /= 9.5;
    }
    if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_RUNEMASTER))
    {
        dam *= 0.65;
        if ( is_rune_robe(ch,"000000020") )
            dam *= 0.75;
        if ( is_rune_robe(ch,"000000050") )
            dam *= 0.6;
        if ( is_rune_robe(ch,"0000000x0") )
            dam *= 0.75;
    }
    if (dam >= 10000) dam = dam - (dam / number_range(3,10) + number_range(10,30));
    if (!IS_NPC(victim))
    {
        dam = dam + (dam / number_range(2,5) + number_range(10,50));
        dam /= 2;
        //    dam *= (number_range(1,2) * number_range(1,2) / number_range(4,6));
    }
    if (dam <= 0) dam = 1;
    if (dam > 1000000) dam = 1000000;
    if (IS_NPC(ch) ) dam = number_range(ch->level/2, ch->level*2);
    if (dam > ch->damcap[DAM_CAP]) dam = number_range( (ch->damcap[DAM_CAP] - 200), (ch->damcap[DAM_CAP] + 100) );
    if ( IS_AFFECTED(victim, AFF_SANCTUARY ) ) dam *= 0.65;
    return dam;
}

int up_dam(CHAR_DATA *ch, CHAR_DATA *victim, int dam)
{
    if (!IS_NPC(ch))
    {
        /* class vs class stuff */
        if (IS_CLASS(ch, CLASS_VAMPIRE))
        {
            dam *= 12;
            dam /= 10;
        }
        if (IS_CLASS(ch, CLASS_UNDEAD_KNIGHT) && IS_CLASS(victim, CLASS_SHAPESHIFTER)) dam *= 1.2;
        if (IS_CLASS(ch, CLASS_MAGE) && IS_CLASS(victim, CLASS_NINJA)) dam *= 1.25;

        /* class modifiers */
        if (IS_CLASS(ch, CLASS_TANARRI))
        {
            if (IS_SET(ch->pcdata->powers[TANARRI_POWER], TANARRI_MIGHT))
            {
                dam *= 15;
                dam /= 7;
            }
        }
        if (IS_CLASS(ch, CLASS_DEMON))
        {
            if (IS_DEMPOWER(ch, DEM_MIGHT)) dam *= 1.1;
            if (IS_SET(ch->warp, WARP_STRONGARMS)) dam *= 1.2;
        }
        if (IS_CLASS(ch, CLASS_SHAPESHIFTER))
        {
            dam *= 1.0;
            if (ch->pcdata->powers[SHAPE_FORM] == TIGER_FORM && ch->pcdata->powers[TIGER_LEVEL] > 1) dam *= 1.4;
            else if (ch->pcdata->powers[SHAPE_FORM] == FAERIE_FORM && ch->pcdata->powers[FAERIE_LEVEL] > 1) dam *= 1.0;
            else if (ch->pcdata->powers[SHAPE_FORM] == HYDRA_FORM && ch->pcdata->powers[HYDRA_LEVEL] > 1) dam *= 1.5;
            else if (ch->pcdata->powers[SHAPE_FORM] == BULL_FORM && ch->pcdata->powers[BULL_LEVEL] > 1) dam *= 1.6;
        }
        if (IS_CLASS(ch, CLASS_UNDEAD_KNIGHT))
        {
            if (ch->pcdata->powers[WEAPONSKILL] > 4) dam *= 1.2;
            if (ch->pcdata->powers[WEAPONSKILL] > 8) dam *= 1.3;
        }
        if (IS_CLASS(ch, CLASS_WEREWOLF) && ch->power[DISC_WERE_BEAR] > 5)
            dam *= 2.2;
    }
    return dam;
}

bool can_counter( CHAR_DATA *ch)
{
    if (!IS_NPC(ch) && IS_STANCE(ch, STANCE_MONKEY)) return TRUE;
    /*  if (!IS_NPC(ch) && IS_STANCE(ch, STANCE_WOLF)) return TRUE;*/
    return FALSE;
}

bool can_bypass( CHAR_DATA *ch, CHAR_DATA *victim )
{
    if (IS_NPC(ch) || IS_NPC(victim)) return FALSE;
    else if (IS_STANCE(ch, STANCE_VIPER)) return TRUE;
    else if (IS_STANCE(ch, STANCE_MANTIS)) return TRUE;
    else if (IS_STANCE(ch, STANCE_TIGER)) return TRUE;
    else if (IS_STANCE(ch, STANCE_WOLF)) return TRUE;
    else if (ch->stance[0] > 12 && IS_SET(ch->stance[(ch->stance[0]+6)], STANCEPOWER_BYPASS)) return TRUE;
    return FALSE;
}

void update_damcap( CHAR_DATA *ch, CHAR_DATA *victim )
{
    OBJ_DATA * pWield;
    int max_dam = 1000;
    int i = 0;
    int iType;

    if (get_curr_str(ch) > 0)
    {
        max_dam += get_curr_str(ch) * 5;
    }

    if (!IS_NPC(ch))
    {
        /*   if (ch->pcdata->mod_str > 0) max_dam += ch->pcdata->mod_str * 5;    */
        if (ch->pcdata->upgrade_level > 0) max_dam += ch->pcdata->upgrade_level * 550;
        if (ch->generation == 0) max_dam += 1000;
        if (ch->generation == 1) max_dam += 500;
        if (ch->generation == 2) max_dam += 400;
        if (ch->generation == 3) max_dam += 300;
        if (ch->generation == 4) max_dam += 200;
        if (ch->generation == 5) max_dam += 100;

        if (IS_AVATAR(ch))
        {
            if((pWield = get_eq_char(ch, WEAR_WIELD)) != NULL)
            {
                iType = pWield->value[3];
            }
            else
            {
                iType = 0;
            }

            for (i=0; i < 13 ; i++)
            {
                if(i == iType)
                {
                    max_dam += ch->wpn[i] / 10;
                    max_dam += ch->wpn_lvl[i] * 100;
                }
                else
                {
                    max_dam += ch->wpn[i] / 30;
                    max_dam += ch->wpn_lvl[i] * 10;
                }
            }
        }
        if (IS_CLASS(ch, CLASS_MILITARY))
        {
            max_dam+=(((ch->pcdata->Bootcamp[3])*700)+((ch->pcdata->Bootcamp[0])*100));
        }

        if(IS_CLASS(ch, CLASS_MAGE) || IS_CLASS(ch, CLASS_WIZARD) || IS_CLASS(ch, CLASS_LICH) || (IS_CLASS(ch, CLASS_ELF) && IS_SET(ch->extra, SPC_ELF_MAGE)))
        {
            for(i = 0; i < 5; i++)
            {
                max_dam += ch->spl[i] / 25;
            }
        }
        /*
            if (IS_CLASS(ch, CLASS_MECHA))
            {
              max_dam += 2000;
            }
        */
        if (IS_CLASS(ch, CLASS_WRAITH))
        {
            max_dam += ch->pcdata->powers[WRAITH_DEATH]*400;
            max_dam += ch->pcdata->powers[WRAITH_POWER]*400;
            max_dam += ch->pdeath*100;
        }

        if (IS_CLASS(ch, CLASS_SAMURAI) && (get_eq_char(ch, WEAR_WIELD) != NULL))
        {
            for (i=0; i < 13 ; i++)
                if (ch->wpn[i] >= 10000) max_dam += 250;
            max_dam += 1000;
        }

        if (IS_CLASS(ch, CLASS_MAGE))
        {
            if (IS_ITEMAFF(ch, ITEMA_BEAST)) max_dam += 100;
            max_dam += 250;
        }
        if (IS_CLASS(ch, CLASS_TANARRI))
        {
            if (IS_SET(ch->pcdata->powers[TANARRI_POWER], TANARRI_MIGHT)) max_dam += 1200;
            max_dam += ch->pcdata->rank*300;                /* max rank is 6 btw */
        }
        if (IS_CLASS(ch, CLASS_LICH))
        {
            max_dam += 850;
            if (ch->pcdata->powers[CON_LORE] > 4) max_dam += 150;
            if (ch->pcdata->powers[DEATH_LORE] > 4) max_dam += 150;
            if (ch->pcdata->powers[LIFE_LORE] > 4) max_dam += 150;
            if (ch->pcdata->powers[NECROMANTIC] > 4) max_dam += 150;
            if (ch->pcdata->powers[CHAOS_MAGIC] > 4) max_dam += 150;
        }
        if (IS_CLASS(ch, CLASS_DRAGON))
        {
            max_dam += ch->pcdata->classfour[DRAGON_AGE]*165;
        }

        if (IS_CLASS(ch, CLASS_ANGEL))
        {
            max_dam += ch->pcdata->powers[ANGEL_JUSTICE]*400;
            max_dam += ch->pcdata->powers[ANGEL_PEACE]*100;
            max_dam += ch->pcdata->powers[ANGEL_HARMONY]*150;
            max_dam += ch->pcdata->powers[ANGEL_LOVE]*100;
        }
        if (IS_CLASS(ch, CLASS_SHAPESHIFTER))
        {
            max_dam += 1000;
            if (ch->pcdata->powers[SHAPE_FORM] == TIGER_FORM) max_dam += ch->pcdata->powers[TIGER_LEVEL] * 1100;
            else if (ch->pcdata->powers[SHAPE_FORM] == HYDRA_FORM) max_dam += ch->pcdata->powers[HYDRA_LEVEL] * 1500;
            else if (ch->pcdata->powers[SHAPE_FORM] == BULL_FORM) max_dam += ch->pcdata->powers[BULL_LEVEL] * 1000;
            else if (ch->pcdata->powers[SHAPE_FORM] == FAERIE_FORM) max_dam += ch->pcdata->powers[FAERIE_LEVEL] * 200;
        }
        if (IS_CLASS(ch, CLASS_KAHN))
        {
            max_dam += 1400;
            if (IS_SET(ch->polyaff, POLY_ZULOFORM)) max_dam += 1750;
        }

        if (IS_CLASS(ch, CLASS_DROW))
        {
            max_dam += 1000;
            if (IS_SET(ch->newbits, NEW_DROWHATE)) max_dam += 650;
            if (IS_SET(ch->newbits,NEW_DFORM)) max_dam += 1200;
        }
        if(IS_CLASS(ch, CLASS_PHOENIX))
        {
            max_dam+=ch->pcdata->phoenix[0]*900;
            if(ch->pcdata->phoenix[1]<1)max_dam+=1800;
            max_dam+=1000;
        }
        if (IS_CLASS(ch, CLASS_DEMON) && ch->in_room != NULL)
        {
            max_dam += ch->power[DISC_DAEM_ATTA] * 350;
            max_dam += 700;
            if (ch->pcdata->souls > 0) max_dam += UMIN(350, 70 * ch->pcdata->souls);
            if (ch->in_room->vnum >= 93420 && ch->in_room->vnum <= 93426) max_dam += 6000;
            max_dam += (ch->pcdata->stats[DEMON_POWER] * 2.5);
            if ( IS_DEMPOWER(ch, DEM_MIGHT) ) max_dam += 500;
        }
        if (IS_CLASS(ch, CLASS_DROID))
        {
            if (ch->pcdata->powers[CYBORG_LIMBS] > 0) max_dam += 600;
            if (ch->pcdata->powers[CYBORG_LIMBS] > 1) max_dam += 700;
            if (ch->pcdata->powers[CYBORG_LIMBS] > 2) max_dam += 800;
            if (ch->pcdata->powers[CYBORG_LIMBS] > 3) max_dam += 900;
            if (ch->pcdata->powers[CYBORG_LIMBS] > 4) max_dam += 1000;
        }
        if (IS_CLASS(ch, CLASS_MONK))
        {
            if ( get_eq_char(ch, WEAR_WIELD) == NULL ) max_dam += 0;
            max_dam +=  (ch->monkab[COMBAT] * 400);
            if (ch->chi[CURRENT] > 0) max_dam += ch->chi[CURRENT] * 100;
        }
        if (IS_CLASS(ch, CLASS_VAMPIRE))
        {
            max_dam += (ch->pcdata->condition[COND_THIRST]/10);
            max_dam += (ch->rage * 5);
            if (ch->power[DISC_VAMP_POTE] > 0)
                max_dam += (ch->power[DISC_VAMP_POTE] * 150);
            if( ch->pcdata->rank == AGE_TRUEBLOOD)  max_dam += 1420;
            else if( ch->pcdata->rank == AGE_LA_MAGRA) max_dam += 850;
            else if (ch->pcdata->rank == AGE_METHUSELAH) max_dam += 680;
            else if (ch->pcdata->rank == AGE_ELDER) max_dam += 425;
            else if (ch->pcdata->rank == AGE_ANCILLA) max_dam += 170;
        }
        if (IS_CLASS(ch, CLASS_NINJA))
        {
            max_dam += (ch->rage * 10);
            if(ch->pcdata->powers[NPOWER_CHIKYU] >=6 && ch->pcdata->powers[HARA_KIRI] > 0)
                max_dam +=1500;
            if(ch->pcdata->powers[NPOWER_CHIKYU] >=2)
                max_dam += 750;
        }

        if (IS_CLASS(ch, CLASS_WIZARD))
        {
            max_dam += 500;
            max_dam += ((ch->pcdata->powers[WL_SKILLS]) * 275);
            max_dam += ((ch->pcdata->powers[WL_SPELLS]) * 700);
        }
        if (IS_CLASS(ch, CLASS_ELF))
        {
            if (IS_SET(ch->special,SPC_ELF_MAGE))
            {
                max_dam += 400;
                max_dam += ((ch->pcdata->classtwo[ELF_GREENSTONE]) * 500);
                max_dam += ((ch->pcdata->classtwo[ELF_REDSTONE]) * 500);
                max_dam += ((ch->pcdata->classtwo[ELF_BLUESTONE]) * 500);
            }
            if (IS_SET(ch->special,SPC_ELF_WARRIOR))
            {
                max_dam += 600;
                max_dam += ((ch->pcdata->classtwo[ELF_WAR_RAGE]) * 300);
            }
            if (IS_SET(ch->special,SPC_ELF_ARCHER))
            {
                max_dam += 400;
                max_dam += ((ch->pcdata->classtwo[ELF_ARC_FIREARROW]) * 200);
                max_dam += ((ch->pcdata->classtwo[ELF_ARC_ICEARROW]) * 200);
                max_dam += ((ch->pcdata->classtwo[ELF_ARC_LIGHTARROW]) * 200);
            }
            if (ch->pcdata->classtwo[ELF_GEN] > 5) max_dam += 500;
        }

        if (IS_CLASS(ch, CLASS_WEREWOLF))
        {
            if (ch->rage > 99)
            {
                max_dam += (ch->rage / 2);
                max_dam += 1000;
            }
            if (ch->power[DISC_WERE_PAIN]>9) max_dam += 1250;
        }
        if (IS_CLASS(ch, CLASS_UNDEAD_KNIGHT))
            max_dam += ch->pcdata->powers[WEAPONSKILL] * 350;
        if ( IS_CLASS(ch, CLASS_RUNEMASTER) )
            max_dam += (6-ch->generation) * 450;
    }

    if (IS_ITEMAFF(ch, ITEMA_ARTIFACT)) max_dam += 1000;

    if (IS_NPC(victim) || victim->stance[0] != STANCE_MONKEY )
    {
        if      (ch->stance[0] == STANCE_BULL)   max_dam += 200;
        else if (ch->stance[0] == STANCE_DRAGON) max_dam += 250;
        else if (ch->stance[0] == STANCE_WOLF)   max_dam += 450;
        else if (ch->stance[0] == STANCE_TIGER)  max_dam += 400;
        else if (ch->stance[0] > 12 && IS_SET(ch->stance[(ch->stance[0]+6)], STANCEPOWER_DAMCAP_3)) max_dam += 550;
        else if (ch->stance[0] > 12 && IS_SET(ch->stance[(ch->stance[0]+6)], STANCEPOWER_DAMCAP_2)) max_dam += 400;
        else if (ch->stance[0] > 12 && IS_SET(ch->stance[(ch->stance[0]+6)], STANCEPOWER_DAMCAP_1)) max_dam += 250;
    }

    if (!IS_NPC(victim) && ch->stance[0] != STANCE_MONKEY )
    {
        if (victim->stance[0] > 12 && IS_SET(victim->stance[(victim->stance[0]+6)], STANCEPOWER_REV_DAMCAP_3)) max_dam -= 550;
        else if (victim->stance[0] > 12 && IS_SET(victim->stance[(victim->stance[0]+6)], STANCEPOWER_REV_DAMCAP_2)) max_dam -= 400;
        else if (victim->stance[0] > 12 && IS_SET(victim->stance[(victim->stance[0]+6)], STANCEPOWER_REV_DAMCAP_1)) max_dam -= 250;
        else if (victim->stance[0] == STANCE_WOLF)   max_dam -= 450;
    }
    //if(!IS_NPC(ch))max_dam+=get_curr_str(ch)*15;
    if(!IS_NPC(victim))max_dam-=get_curr_wis(victim)*20;
    if (IS_NPC(ch) ) max_dam += ch->level;

    /* Max Dam Reductions */
    /* Werewolves */

    if (!IS_NPC(victim))
    {
        int silver_tol = (victim->siltol * 2.5);
        if (IS_CLASS(victim, CLASS_WEREWOLF) && IS_ITEMAFF(ch, ITEMA_RIGHT_SILVER))
            max_dam += (450 - silver_tol);
        if (IS_CLASS(victim, CLASS_WEREWOLF) && IS_ITEMAFF(ch, ITEMA_LEFT_SILVER))
            max_dam += (450 - silver_tol);
        /* Vampies */
        if (!IS_NPC(ch) && (IS_CLASS(victim, CLASS_VAMPIRE)))
            max_dam -= (victim->power[DISC_VAMP_FORT] * 50);
        if (IS_NPC(ch) || ch->stance[0] != STANCE_MONKEY)
        {
            if      (victim->stance[0] == STANCE_CRAB)    max_dam -= 250;
            else if (victim->stance[0] == STANCE_DRAGON)  max_dam -= 250;
            else if (victim->stance[0] == STANCE_SWALLOW) max_dam -= 250;
        }
        if (IS_CLASS(victim,CLASS_DEMON) && (victim->in_room->vnum >= 93420 && victim->in_room->vnum <= 93426))
            max_dam *= 0.5;
    }

    max_dam += blood_ratio(ch) * 2;

    //  if (ch->level >= LEVEL_BUILDER) max_dam = 30000;
    //  if (ch->level >= LEVEL_BUILDER && !IS_NPC(ch)) max_dam = 100000; // Det er pisse irreterende Thomas.. jeg kan intet teste mod mobs, de doer automatisk. DIE!
    ch->damcap[DAM_CAP] = max_dam;
    if ( IS_NPC(ch) )
        ch->damcap[DAM_CAP] = ch->level*2;
    ch->damcap[DAM_CHANGE] = 0;
    return;
}

/*
 * Inflict damage from a hit.
 */
void damage( CHAR_DATA *ch, CHAR_DATA *victim, int dam, int dt )
{
    CHAR_DATA *emb;

    if ( victim->position == POS_DEAD ) return;
    if ( ch->embracing != NULL )
    {
        if ( ( emb = ch->embracing ) != NULL ) stop_embrace(ch,emb);
        else stop_embrace(ch,NULL);
    }
    /*
     * Stop up any residual loopholes.
     */
    if (ch->damcap[DAM_CHANGE] == 1) update_damcap(ch,victim);
    if ( dam > ch->damcap[0] ) dam = ch->damcap[0];

    /*
     * Certain attacks are forbidden.
     * Most other attacks are returned.
     */
    if ( victim != ch )
    {
        if (is_safe( ch, victim )) return;
        if ( victim->position > POS_STUNNED )
        {
            if ( victim->fighting == NULL ) set_fighting( victim, ch );
            victim->position = POS_FIGHTING;
        }
        if ( victim->position > POS_STUNNED )
        {
            if ( ch->fighting == NULL )
                set_fighting( ch, victim );
            /*
             * If victim is charmed, ch might attack victim's master.
             */
            if ( IS_NPC(ch)
                &&   IS_NPC(victim)
                &&   IS_AFFECTED(victim, AFF_CHARM)
                &&   victim->master != NULL
                &&   victim->master->in_room == ch->in_room
                &&   number_bits( 3 ) == 0 )
            {
                stop_fighting( ch, FALSE );
                multi_hit( ch, victim->master, TYPE_UNDEFINED );
                return;
            }
        }
        if ( victim->master == ch ) stop_follower( victim );
        /*
         * Damage modifiers.
         */
        if ( IS_AFFECTED(ch, AFF_HIDE) )
        {
            if (!can_see(victim,ch))
            {
                dam *= 1.5;
                send_to_char("You use your concealment to get a surprise attack!\n\r",ch);
            }
            REMOVE_BIT( ch->affected_by, AFF_HIDE );
            act( "$n leaps from $s concealment.", ch, NULL, NULL, TO_ROOM );
        }
        if ( IS_AFFECTED(victim, AFF_PROTECT) && IS_EVIL(ch) && dam > 1  )
            dam -= dam * 0.25;
        if ( IS_AFFECTED(victim, AFF_PROTECT_GOOD) && IS_GOOD(ch) && dam > 1  )
            dam -= dam * 0.25;
        if ( dam < 0 ) dam = 0;
        /*
         * Check for disarm, trip, parry, and dodge.
         */
        if ( dt >= TYPE_HIT )
        {
            if (IS_NPC(ch) && number_percent( ) < ch->level * 0.5)
                disarm( ch, victim );
            if (IS_NPC(ch) && number_percent( ) < ch->level * 0.5)
                trip( ch, victim );
            if (check_dodge( ch, victim, dt))
                return;
            if ( !IS_NPC(victim) && IS_STANCE(victim, STANCE_MONGOOSE) &&
                victim->stance[STANCE_MONGOOSE] > 100 && !can_counter(ch)
                && !can_bypass(ch,victim) && check_dodge( ch, victim, dt ))
                return;
            else if ( !IS_NPC(victim) && IS_STANCE(victim, STANCE_SWALLOW) &&
                victim->stance[STANCE_SWALLOW] > 100 && !can_counter(ch)
                && !can_bypass(ch,victim) && check_dodge( ch, victim, dt ))
                return;

            /* ------------ This is the part for superstances, Jobo ------------------- */
            else if (!IS_NPC(victim) && victim->stance[0] > 12
                && IS_SET(victim->stance[(victim->stance[0]+6)], STANCEPOWER_DODGE)
                && victim->stance[(victim->stance[0])] > 100 && !can_counter(ch)
                && !can_bypass(ch,victim) && check_dodge( ch, victim, dt ))
                return;
            /* ------------ This is the end for superstances, Jobo ------------------- */

            if (check_parry( ch, victim, dt)) return;
            if ( !IS_NPC(victim) && IS_STANCE(victim, STANCE_CRANE) &&
                victim->stance[STANCE_CRANE] > 100 && !can_counter(ch)
                && !can_bypass(ch,victim) && check_parry( ch, victim, dt ))
                return;
            else if (!IS_NPC(victim) && IS_STANCE(victim, STANCE_MANTIS) &&
                victim->stance[STANCE_MANTIS] > 100 && !can_counter(ch)
                && !can_bypass(ch,victim) && check_parry( ch, victim, dt ))
                return;
            else if (!IS_NPC(victim) && (ch->stance[0] == STANCE_WOLF) && !can_counter(ch)
                && !can_bypass(ch,victim) && check_parry( ch, victim, dt))
                return;

            /* ------------ This is the part for superstances, Jobo ------------------- */
            else if (!IS_NPC(victim) && victim->stance[0] > 12
                && IS_SET(victim->stance[(victim->stance[0]+6)], STANCEPOWER_PARRY)
                && victim->stance[(victim->stance[0])] > 100 && !can_counter(ch)
                && !can_bypass(ch,victim) && check_parry( ch, victim, dt ))
                return;
            /* ------------ This is the end for superstances, Jobo ------------------- */

        }
        if (dt == gsn_lightningkick && IS_CLASS(ch, CLASS_MONK) )             dam *= 3.0;
        if (dt == gsn_thrustkick && IS_CLASS(ch, CLASS_MONK) )             dam *= 4.0;
        if (dt == gsn_tornadokick && IS_CLASS(ch, CLASS_MONK) )             dam *= 3.0;
        if (dt == gsn_backfist && IS_CLASS(ch, CLASS_MONK) )             dam *= 1.0;
        dam_message( ch, victim, dam, dt );
    }

    if ( IS_CLASS(victim, CLASS_MONK ) && victim->monkab[SPIRIT] >= 2 )
    {
        if (IS_SET(victim->newbits, NEW_NATURAL))
        {
            stc("You absorb the natural imbalances of your opponent.\n\r",victim);
            act("$n absorbs the natural imbalances of you attack.",victim,NULL,ch,TO_VICT);
            if ( dam > 2000 ) dam /= 2;
            victim->hit += dam;
            REMOVE_BIT(victim->newbits, NEW_NATURAL);
            return;
        }
    }
    if (IS_SET(victim->newbits, NEW_IRONMIND))
    {
        REMOVE_BIT(victim->newbits, NEW_IRONMIND);
        send_to_char("Your focus your full concentration on the attack,\n\r", victim);
        send_to_char("and absorb the full impact into your system, channeling it into a healing force.\n\r",victim);
        victim->hit += dam;
        dam = 0;
    }
    if (IS_SET(victim->act, ACT_DUMMY) && IS_NPC(victim) )
    {
        dam = 0;
    }
    hurt_person(ch,victim,dam);
    dropinvis(ch);
    dropinvis(victim);
    return;
}

void angel_eye( CHAR_DATA *ch, CHAR_DATA *victim, int dam )
{
    char buf[MAX_STRING_LENGTH];
    char buf2[MAX_STRING_LENGTH];
    int new_dam;

    if (IS_SET(victim->pcdata->powers[ANGEL_POWERS], ANGEL_EYE) && dam > 100)
    {
        new_dam = number_range(dam/3,dam/2);
        if (new_dam > 1000) new_dam = number_range(525,775);
        if (new_dam >= victim->hit) new_dam = victim->hit -1;
        if (new_dam < 100) new_dam = 100;
        hurt_person(victim, ch, new_dam);
        sprintf(buf, "God's justice strikes you HARD [#C%d#n]",new_dam);
        sprintf(buf2, "The sinner dares to lay hands on you, God's wrath on the sinner [#C%d#n]", new_dam);
        act(buf, victim, NULL, ch, TO_VICT);
        act(buf2, victim, NULL, ch, TO_CHAR);
    }
    return;
}

void hurt_person( CHAR_DATA *ch, CHAR_DATA *victim, int dam )
{
    char buf[MAX_STRING_LENGTH];
    char buf2[MAX_STRING_LENGTH];

    int temp;
    //  int newdam;
    if (!IS_NPC(victim) && IS_ITEMAFF(victim, ITEMA_BLOCK))
    {
        if (number_range(1,50) > 48)
        {
            dam=0;
            //  do_say(victim,"#sEnguarde you swine!#n");
            sprintf(buf, "#Y%s blocks your attack!#n",victim->name);
            sprintf(buf2, "#YYou block the attack !#n");
            act(buf,victim,NULL,ch, TO_VICT);
            act(buf2,victim,NULL,ch,TO_CHAR);
        }
    }
    victim->hit -= dam;
    if (!IS_NPC(victim) && IS_ITEMAFF(victim, ITEMA_RETORT))
    {
        /*  if (dam >= 8000)
          newdam = number_range(dam *0.8 , dam);
          else newdam = number_range( dam/2, dam);*/
        if (number_range(1,50) > 47)
        {
            one_hit(ch, victim, TYPE_UNDEFINED, 1);
            //  hurt_person(victim,ch,newdam);
            do_say(victim,"#sEnguarde you swine!#n");
            sprintf(buf, "#Y%s retorts and hits you hard!",victim->name);
            sprintf(buf2, "#YYou retort the attack harshly!");
            act(buf,victim,NULL,ch, TO_VICT);
            act(buf2,victim,NULL,ch,TO_CHAR);
        }
    }
    if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_ANGEL) && !IS_CLASS(ch, CLASS_ANGEL))
        angel_eye(ch,victim,dam);                           // check for that eye for an eye.
    if (!IS_NPC(victim) && victim->level >= LEVEL_IMMORTAL && victim->hit < 1)
        victim->hit = 1;
    if (!IS_NPC(victim) && IS_SET(victim->newbits, NEW_CLOAK) &&victim->hit < 1 )
    {
        victim->hit += UMIN((victim->max_hit * .1), 4000);
        if (IS_CLASS(victim, CLASS_MONK)) send_to_char("your cloak of life saves your from certain death.\n\r",victim);
        else send_to_char("your cloak of death prevents the destruction of your body.\n\r",victim);
        send_to_char("#0They escape your final blow.#n\n\r",ch);
        REMOVE_BIT(victim->newbits, NEW_CLOAK);
    }
    update_pos( victim );

    switch( victim->position )
    {
        case POS_MORTAL:
            act( "$n is mortally wounded, and spraying blood everywhere.", victim, NULL, NULL, TO_ROOM );
            send_to_char("You are mortally wounded, and spraying blood everywhere.\n\r", victim );
            break;
        case POS_INCAP:
            act( "$n is incapacitated, and bleeding badly.",victim, NULL, NULL, TO_ROOM );
            send_to_char("You are incapacitated, and bleeding badly.\n\r",victim );
            break;
        case POS_STUNNED:
            act( "$n is stunned, but will soon recover.",victim, NULL, NULL, TO_ROOM );
            send_to_char("You are stunned, but will soon recover.\n\r", victim );
            break;
        case POS_DEAD:
            act( "$n is DEAD!!", victim, 0, 0, TO_ROOM );
            send_to_char( "You have been KILLED!!\n\r\n\r", victim );
            break;
        default:
            if ( dam > victim->max_hit / 4 )
                send_to_char( "That really did HURT!\n\r", victim );
            if ( victim->hit < victim->max_hit / 4 && dam > 0 )
            {
                if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_VAMPIRE) && number_percent() < victim->beast)
                    vamp_rage(victim);
                else
                    send_to_char( "You sure are BLEEDING!\n\r", victim );
            }
            break;
    }

    /*
     * Sleep spells and extremely wounded folks.
     */
    if (!IS_AWAKE(victim)) stop_fighting( victim, FALSE );

    /*
     * Payoff for killing things.
     */
    if (victim->hit <= 0 && IS_NPC(victim))
    {
        group_gain( ch, victim );
        victim->position = POS_DEAD;
    }

    if ( victim->position == POS_DEAD )
    {
        if (IS_NPC(victim) && !IS_NPC(ch))
        {
            ch->mkill += 1;

            if (IS_CLASS(ch, CLASS_DEMON)
                || IS_CLASS(ch, CLASS_DROW) || IS_CLASS(ch,CLASS_DROID)
                || IS_CLASS(ch, CLASS_TANARRI) || IS_CLASS(ch,CLASS_KAHN))
            {
                if (IS_NPC(victim) && !IS_SET(victim->act, ACT_NOEXP))
                {
                    temp = victim->level*2;
                    if (global_exp)
                    {
                        temp += victim->level;
                    }
                    if (IS_CLASS(ch, CLASS_DEMON))
                    {
                        ch->pcdata->stats[DEMON_CURRENT] += temp;
                        ch->pcdata->stats[DEMON_TOTAL] += temp;
                    }
                    else
                        ch->pcdata->stats[DROW_POWER] += temp;
                    sprintf(buf,"You gain #y(#C%d#y)#n class points.\n\r", temp );
                    if (!IS_SET(ch->act, PLR_BRIEF4)) send_to_char(buf,ch);
                }
            }
            if (ch->level == 1 && ch->mkill > 9)
            {
                if ( SET_BIT(ch->act, PLR_SILENCE) ) REMOVE_BIT(ch->act, PLR_SILENCE);
                if ( SET_BIT(ch->act, PLR_LOG) ) REMOVE_BIT(ch->act, PLR_LOG);
                ch->level = 2;
                do_save(ch,"");
            }
        }
        if (!IS_NPC(victim) && IS_NPC(ch))
        {
            victim->mdeath = victim->mdeath + 1;
        }
        raw_kill( victim );
        if (!IS_NPC(ch) && !IS_NPC(victim) && victim->pcdata->bounty > 0)
        {
            sprintf(buf,"You recive a %d QP bounty, for killing %s.\n\r",
                victim->pcdata->bounty, victim->name);
            send_to_char(buf, ch);
            ch->pcdata->quest += victim->pcdata->bounty;
            victim->pcdata->bounty =0;
        }
        if (IS_SET(ch->act2, PLR2_AUTOSLAB) )
            do_get( ch, "all.slab corpse" );
        if (IS_SET(ch->act2, PLR2_AUTOGEM) )
            do_get( ch, "all.gem corpse" );
        if (IS_SET(ch->act2, PLR2_AUTOHILT) )
            do_get( ch, "all.hilt corpse" );
        if (IS_SET(ch->act, PLR_AUTOLOOT) )
            do_get( ch, "all corpse" );
        else
            do_look( ch, "in corpse" );
        if ( !IS_NPC(ch) && IS_NPC(victim) )
        {
            if ( IS_SET(ch->act, PLR_AUTOSAC) )
                do_sacrifice( ch, "corpse" );
        }
        return;
    }
    if ( victim == ch ) return;
    tail_chain( );
    return;
}

bool is_safe( CHAR_DATA *ch, CHAR_DATA *victim )
{
    if (!ch->in_room) return TRUE;                          // when someone calls a one_hit or similar on something/someone who COULD be dead/gone.
    if (!victim->in_room) return TRUE;
    if (!IS_NPC(victim) && !IS_NPC(ch))
    {
        if ( !CAN_PK(ch) || !CAN_PK(victim) )
        {
            send_to_char( "Both players must be avatars to fight.\n\r", ch );
            return TRUE;
        }
        if ( ch->pcdata->upgrade_level == 0 && ((get_age(ch) - 17) < 5) )
        {
            send_to_char( "You can't attack a player if your still have less then 5 hours, use commit if you want to kill.\n\r", ch );
            return TRUE;
        }
        if ( victim->pcdata->upgrade_level == 0 && ((get_age(victim) - 17) < 5) )
        {
            send_to_char( "You can't attack a Newbie with less then 5 hours.\n\r", ch );
            return TRUE;
        }
        /*    if ( !IS_SET (victim->in_room->room_flags, ROOM_ARENA) && IS_SET (ch->in_room->room_flags, ROOM_ARENA) && !in_fortress(victim) && !in_fortress(ch) && (victim->pcdata->upgrade_level != ch->pcdata->upgrade_level))
            {
            send_to_char( "Attack someone your own upgrade, wimp.\n\r", ch );
            return TRUE;
            }*/
    }
    if (!IS_NPC(victim) && worldpk) return TRUE;
    if (!IS_NPC(ch) && !IS_NPC(victim))
    {
        if(ch->pcdata->safe_counter > 0)
        {
            send_to_char("Your are safe for 10 ticks after training avatar.\n\r",ch);
            return TRUE;
        }
        if(victim->pcdata->safe_counter > 0)
        {
            send_to_char("They are safe for 10 ticks after training avatar.\n\r",ch);
            return TRUE;
        }
        if (victim->desc)
        {
            if (victim->desc->connected >= CON_NOTE_TO && victim->desc->connected <= CON_NOTE_FINISH)
            {
                send_to_char("That player is writing a note, be nice.\n\r",ch);
                return TRUE;
            }
        }
    }
    if (IS_AFFECTED(ch, AFF_ETHEREAL))
    {
        send_to_char( "You cannot while ethereal.\n\r", ch );
        return TRUE;
    }
    if (IS_AFFECTED(victim, AFF_ETHEREAL))
    {
        send_to_char( "You cannot fight an ethereal person.\n\r", ch );
        return TRUE;
    }
    if (IS_AFFECTED(ch, AFF_SHADOWPLANE))
    {
        act( "You are too insubstantial!", ch, NULL, victim, TO_CHAR );
        return TRUE;
    }
    if (IS_AFFECTED(victim, AFF_SHADOWPLANE))
    {
        act( "$E is too insubstantial!", ch, NULL, victim, TO_CHAR );
        return TRUE;
    }
    if ( IS_HEAD(ch,LOST_HEAD) || IS_EXTRA(ch,EXTRA_OSWITCH) )
    {
        send_to_char( "Objects cannot fight!\n\r", ch );
        return TRUE;
    }
    if ( IS_HEAD(victim,LOST_HEAD) || IS_EXTRA(victim,EXTRA_OSWITCH))
    {
        send_to_char( "You cannot attack an object.\n\r", ch );
        return TRUE;
    }
    if (IS_SET(victim->extra, EXTRA_AFK))
    {
        send_to_char("They are AFK!\n\r",ch);
        return TRUE;
    }
    if (!IS_NPC(victim) && victim->desc == NULL && victim->timer > 1 && victim->fight_timer == 0)
    {
        // Timer check to avoid people going ld in the first round.
        send_to_char("Nooo, they are linkdead.\n\r",ch);
        do_flee(victim,"");
        do_save(victim,"");
        do_goto(victim,"2");
        return TRUE;
    }
    /*  if (ragnarok && is_upgrade(ch) && !is_upgrade(victim) && !IS_NPC(ch) && !IS_NPC(victim))
      {
        send_to_char("You cannot attack non-upgrades doing apocalypse.\n\r",ch);
        return TRUE;
      }
      if (ragnarok && !is_upgrade(ch) && is_upgrade(victim) && !IS_NPC(ch) && !IS_NPC(victim))
      {
        send_to_char("You cannot attack upgrades doing apocalypse.\n\r",ch);
        return TRUE;
      }*/
    if (victim->fight_timer > 0) return FALSE;
    if ( !IS_NPC(ch ) )
        if (IS_AFFECTED(ch, AFF_PEACE) && ch->fighting != NULL) REMOVE_BIT(ch->affected_by, AFF_PEACE);
    if ( !IS_NPC(victim) )
        if (IS_AFFECTED(victim, AFF_PEACE) && victim->fighting != NULL) REMOVE_BIT(victim->affected_by, AFF_PEACE);
    if ( IS_SET(ch->in_room->room_flags,ROOM_SAFE) && !ragnarok)
    {
        send_to_char( "You cannot fight in a safe room.\n\r", ch );
        return TRUE;
    }
    if(IS_SET(ch->in_room->room_flags, ROOM_IMMSAFE))
    {
        send_to_char("This is an immortals safe room.  If this is a non-immortal room, please report it.\n\r", ch);
        return TRUE;
    }
    if (IS_NPC(ch) || IS_NPC(victim)) return FALSE;

    if (victim->fighting == ch) return FALSE;
    if (IS_AFFECTED(ch, AFF_PEACE))
    {
        send_to_char( "You are unable to attack them.\n\r", ch );
        return TRUE;
    }
    if (IS_AFFECTED(victim, AFF_PEACE))
    {
        send_to_char( "You can't seem to attack them.\n\r", ch );
        return TRUE;
    }
    if (IS_ITEMAFF(ch, ITEMA_PEACE))
    {
        send_to_char( "You are unable to attack them.\n\r", ch );
        return TRUE;
    }
    if (IS_ITEMAFF(victim, ITEMA_PEACE))
    {
        send_to_char( "You can't seem to attack them.\n\r", ch );
        return TRUE;
    }
    return FALSE;
}

/*
 * Check for parry.
 */
bool check_parry( CHAR_DATA *ch, CHAR_DATA *victim, int dt )
{
    OBJ_DATA *obj = NULL;
    int chance = 0;
    bool claws = FALSE;

    if (!IS_AWAKE(victim)) return FALSE;
    if (IS_NPC(victim))  obj = NULL;
    else if (IS_CLASS(victim, CLASS_WEREWOLF) && victim->power[DISC_WERE_BEAR] > 2
        && IS_VAMPAFF(victim, VAM_CLAWS) && get_eq_char(victim, WEAR_WIELD) == NULL
        && get_eq_char(victim, WEAR_HOLD) == NULL)
    {
        obj = NULL;
        claws = TRUE;
    }
    else if ( IS_CLASS(victim, CLASS_MONK) && IS_SET(victim->newbits, NEW_MONKADAM)
        && get_eq_char(victim, WEAR_WIELD) == NULL && get_eq_char(victim, WEAR_HOLD) == NULL)
    {
        obj = NULL;
        claws = TRUE;
    }
    else if( IS_CLASS(victim, CLASS_SHAPESHIFTER) && get_eq_char(victim, WEAR_WIELD) == NULL
        && get_eq_char(victim, WEAR_HOLD) == NULL)
    {
        obj = NULL;
        claws = TRUE;
    }
    else if( IS_CLASS(victim, CLASS_KAHN) && get_eq_char(victim, WEAR_WIELD) == NULL
        && get_eq_char(victim, WEAR_HOLD) == NULL)
    {
        obj = NULL;
        claws = TRUE;
    }
    else
    {
        if ((obj = get_eq_char(victim, WEAR_WIELD)) == NULL || obj->item_type != ITEM_WEAPON)
        {
            if ((obj = get_eq_char(victim, WEAR_HOLD)) == NULL || obj->item_type != ITEM_WEAPON)
                return FALSE;
        }
    }
    if ( ( dt < 1000 || dt > 1012) && !IS_CLASS(ch, CLASS_MONK) ) return FALSE;
    if (  dt >= 1000 && dt <= 1012 )
    {
        if (!IS_NPC(ch)) chance -= (ch->wpn[dt-1000] * 0.01);
        else chance -= (ch->level * 0.02);
        if (!IS_NPC(victim)) chance += (victim->wpn[dt-1000] * 0.05);
        else chance += victim->level;
    }
    else
    {
        if (!IS_NPC(ch)) chance -= (ch->wpn[0] * 0.01);
        else chance -= (ch->level * 0.02);
        if (!IS_NPC(victim)) chance += (victim->wpn[0] * 0.05);
        else chance += victim->level;
    }
    if (!IS_NPC(victim) && IS_STANCE(victim, STANCE_CRANE) && victim->stance[STANCE_CRANE] > 0
        && !can_counter(ch) && !can_bypass(ch,victim))
        chance += (victim->stance[STANCE_CRANE] * 0.025);
    else if (!IS_NPC(victim) && IS_STANCE(victim, STANCE_MANTIS) && victim->stance[STANCE_MANTIS] > 0
        && !can_counter(ch) && !can_bypass(ch,victim))
        chance += (victim->stance[STANCE_MANTIS] * 0.025);

    /* ------------ This is the part for superstances, Jobo ------------------- */
    if (!IS_NPC(victim) && victim->stance[0] > 12
        && IS_SET(victim->stance[(victim->stance[0]+6)], STANCEPOWER_PARRY)
        && victim->stance[(victim->stance[0])] > 100 && !can_counter(ch) && !can_bypass(ch,victim))
        chance += victim->stance[(victim->stance[0])] * 0.025;
    /* ------------ This is the end for superstances, Jobo ------------------- */

    chance -= (char_hitroll(ch) * 0.1);
    if (claws)
    {
        if (victim->power[DISC_WERE_LYNX] > 3) chance += (char_hitroll(victim) * 0.1);
        else chance += (char_hitroll(victim) * 0.075);
    }
    if (!IS_NPC(ch))
    {
        if (IS_CLASS(ch,CLASS_DROID)) chance -= ch->pcdata->powers[CYBORG_LIMBS]*5;
        if (ch->pcdata->upgrade_level > 0) chance -= ch->pcdata->upgrade_level*10;
        if (IS_CLASS(ch, CLASS_LICH)) chance -= 10;
        if (IS_CLASS(ch, CLASS_SHAPESHIFTER))
        {
            if (ch->pcdata->powers[SHAPE_FORM] == TIGER_FORM) chance -= ch->pcdata->powers[TIGER_LEVEL]*7;
            else if (ch->pcdata->powers[SHAPE_FORM] == BULL_FORM) chance -= ch->pcdata->powers[BULL_LEVEL]*5;
            else if (ch->pcdata->powers[SHAPE_FORM] == HYDRA_FORM) chance -= ch->pcdata->powers[HYDRA_LEVEL]*4.5;
            else if (ch->pcdata->powers[SHAPE_FORM] == FAERIE_FORM) chance -= ch->pcdata->powers[FAERIE_LEVEL]*4;
            else chance -= 10;
        }
        if (IS_CLASS(ch, CLASS_TANARRI))
        {
            if (IS_SET(ch->pcdata->powers[TANARRI_POWER], TANARRI_HEAD)) chance -= 15;
            if (IS_SET(ch->pcdata->powers[TANARRI_POWER], TANARRI_SPEED)) chance -= 17;
            if (ch->pcdata->powers[TANARRI_FURY_ON] == 1) chance -= 17;
        }
        if (IS_CLASS(ch, CLASS_MILITARY)) chance -= ((ch->pcdata->Bootcamp[1])*4);
        if (IS_CLASS(ch, CLASS_DRAGON)) chance -= ch->pcdata->classfour[DRAGON_AGE] - 3;
        if (IS_CLASS(ch, CLASS_ANGEL)) chance -= (ch->pcdata->powers[ANGEL_JUSTICE]*9);
        if (IS_CLASS(ch, CLASS_VAMPIRE) && ch->power[DISC_VAMP_CELE] > 0) chance -= (ch->power[DISC_VAMP_CELE]*3);
        if (IS_CLASS(ch, CLASS_NINJA)  && ch->pcdata->powers[NPOWER_CHIKYU] >= 6 && ch->pcdata->powers[HARA_KIRI] > 0) chance -= 35;
        if (ch->pcdata->powers[NPOWER_CHIKYU] >= 5 && IS_CLASS(ch, CLASS_NINJA)) chance -= 40;
        if (IS_CLASS(ch, CLASS_DEMON) && IS_DEMPOWER(ch, DEM_SPEED)) chance -= 25;
        if (IS_CLASS(ch, CLASS_PHOENIX))chance -= 10+(ch->pcdata->phoenix[1]*3);
        if (IS_CLASS(ch, CLASS_MONK) && ch->chi[CURRENT] > 0) chance -= 10;
        if (IS_CLASS(ch, CLASS_UNDEAD_KNIGHT)) chance -= ch->pcdata->powers[WEAPONSKILL]*3.5;
        if (IS_CLASS(ch, CLASS_MAGE) && IS_ITEMAFF(ch, ITEMA_BEAST)) chance -= 30;
        if (IS_CLASS(ch, CLASS_DROW) && IS_SET(ch->pcdata->powers[1], DPOWER_ARMS)) chance -= 50;
        if (IS_SET(ch->newbits, NEW_MONKFLAME)) chance -= 5;
        if (IS_SET(ch->pcdata->classtwo[ELF_REDSTONE], ELFSTONE_FLAMERING)) chance -= 5;
        if (IS_CLASS(ch, CLASS_WEREWOLF)) chance -= (ch->power[DISC_WERE_MANT] * 3);
        if (IS_CLASS(ch, CLASS_SAMURAI) && (get_eq_char(ch, WEAR_HOLD) != NULL)) chance -= 35;
        if (IS_CLASS(ch, CLASS_ELF)) chance -= 13;
        if (IS_CLASS(ch, CLASS_WRAITH)) chance -= 20;
        if (IS_CLASS(ch, CLASS_WIZARD)) chance -= 20;
        if (IS_CLASS(ch, CLASS_KAHN)) chance -= 25;
        if (IS_ITEMAFF(ch, ITEMA_AFFMANTIS)) chance -= 12;
        if (IS_CLASS(ch, CLASS_RUNEMASTER)) chance -= 30;
    }
    if (!IS_NPC(victim))
    {
        if (IS_CLASS(victim,CLASS_DROID)) chance += victim->pcdata->powers[CYBORG_LIMBS]*5;
        if (victim->pcdata->upgrade_level > 0) chance += victim->pcdata->upgrade_level*10;
        if (IS_CLASS(victim, CLASS_LICH)) chance += 10;
        if (IS_CLASS(victim, CLASS_SHAPESHIFTER))
        {
            if (victim->pcdata->powers[SHAPE_FORM] == TIGER_FORM) chance += victim->pcdata->powers[TIGER_LEVEL]*8;
            else if (victim->pcdata->powers[SHAPE_FORM] == BULL_FORM) chance += victim->pcdata->powers[BULL_LEVEL]*9;
            else if (victim->pcdata->powers[SHAPE_FORM] == HYDRA_FORM) chance += victim->pcdata->powers[HYDRA_LEVEL]*8;
            else if (victim->pcdata->powers[SHAPE_FORM] == FAERIE_FORM) chance += victim->pcdata->powers[FAERIE_LEVEL]*13;
            else chance += 10;
        }
        if (IS_CLASS(victim, CLASS_TANARRI))
        {
            if (IS_SET(victim->pcdata->powers[TANARRI_POWER], TANARRI_HEAD)) chance += 15;
            if (IS_SET(victim->pcdata->powers[TANARRI_POWER], TANARRI_SPEED)) chance += 17;
            if (victim->pcdata->powers[TANARRI_FURY_ON] == 1) chance += 12;
        }
        if (IS_CLASS(victim, CLASS_MILITARY)) chance += ((victim->pcdata->Bootcamp[1])*4);
        if (IS_CLASS(victim, CLASS_DRAGON)) chance += victim->pcdata->classfour[DRAGON_AGE] + 3;
        if (IS_CLASS(victim, CLASS_ANGEL)) chance += (victim->pcdata->powers[ANGEL_PEACE]*9);
        if (victim->pcdata->powers[NPOWER_CHIKYU] >= 5 && IS_CLASS(victim, CLASS_NINJA)) chance += 35;
        if (IS_CLASS(victim, CLASS_NINJA) && victim->pcdata->powers[NPOWER_CHIKYU] >= 6 &&
            victim->pcdata->powers[HARA_KIRI] > 0) chance += 40;
        if (victim->power[DISC_VAMP_CELE] > 0 &&  IS_CLASS(victim, CLASS_VAMPIRE)) chance += (victim->power[DISC_VAMP_CELE]*3);
        if (IS_CLASS(victim, CLASS_DROW) && IS_SET(victim->pcdata->powers[1], DPOWER_ARMS)) chance += 50;
        if (IS_CLASS(victim, CLASS_MONK) && ch->chi[CURRENT] > 0 ) chance += 20;
        if (IS_SET(victim->newbits, NEW_MONKFLAME)) chance += 5;
        if (IS_SET(victim->pcdata->classtwo[ELF_REDSTONE], ELFSTONE_FLAMERING)) chance += 7;
        if (IS_CLASS(victim, CLASS_SAMURAI) && (get_eq_char(victim, WEAR_HOLD) != NULL)) chance +=35;
        if (IS_CLASS(victim, CLASS_UNDEAD_KNIGHT)) chance += victim->pcdata->powers[WEAPONSKILL]*4;
        if (IS_CLASS(victim, CLASS_MAGE) && IS_ITEMAFF(victim, ITEMA_DEFLECTOR)) chance += 40;
        if (IS_CLASS(victim, CLASS_DEMON) && IS_DEMPOWER(victim, DEM_SPEED)) chance += 25;
        if (IS_CLASS(victim, CLASS_PHOENIX))chance += 10+(victim->pcdata->phoenix[1]*3);
        if (IS_CLASS(victim, CLASS_WRAITH)) chance += 65;

        if (IS_CLASS(victim, CLASS_ELF)) chance += 13;
        if (IS_CLASS(victim, CLASS_WIZARD)) chance += 20;
        if (IS_CLASS(victim, CLASS_KAHN)) chance += 25;
        if (IS_CLASS(victim, CLASS_RUNEMASTER)) chance += 30;
    }
    if (chance > 80) chance = 80;
    else if (chance < 20) chance = 20;
    if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_LICH)) chance -= 7.5;
    if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_LICH)) chance += 7.5;
    if (!IS_NPC(ch) && ch->power[DISC_VAMP_CELE] > 0 && IS_CLASS(ch, CLASS_VAMPIRE))
    {
        if( ch->pcdata->rank == AGE_TRUEBLOOD)  chance -= 22;
        else if( ch->pcdata->rank == AGE_LA_MAGRA) chance -= 17;
        else if (ch->pcdata->rank == AGE_METHUSELAH) chance -= 15;
        else if (ch->pcdata->rank == AGE_ELDER) chance -= 10;
        else if (ch->pcdata->rank == AGE_ANCILLA) chance -= 5;
    }
    if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_SHAPESHIFTER))
    {
        if (ch->pcdata->powers[SHAPE_FORM] == TIGER_FORM) chance -= ch->pcdata->powers[TIGER_LEVEL]*8;
        else if (ch->pcdata->powers[SHAPE_FORM] == BULL_FORM) chance -= ch->pcdata->powers[BULL_LEVEL]*9;
        else if (ch->pcdata->powers[SHAPE_FORM] == HYDRA_FORM) chance -= ch->pcdata->powers[HYDRA_LEVEL]*8;
        else if (ch->pcdata->powers[SHAPE_FORM] == FAERIE_FORM) chance -= ch->pcdata->powers[FAERIE_LEVEL]*13;
        else chance -= 5;
    }
    if (IS_CLASS(ch, CLASS_TANARRI) && !IS_NPC(ch))
    {
        if (IS_SET(ch->pcdata->powers[TANARRI_POWER], TANARRI_HEAD)) chance -= 7;
        if (IS_SET(ch->pcdata->powers[TANARRI_POWER], TANARRI_SPEED)) chance -= 10;
        if (ch->pcdata->powers[TANARRI_FURY_ON] == 1) chance -= 7;
    }
    if (IS_CLASS(victim, CLASS_TANARRI) && !IS_NPC(victim))
    {
        if (IS_SET(victim->pcdata->powers[TANARRI_POWER], TANARRI_HEAD)) chance += 7;
        if (IS_SET(victim->pcdata->powers[TANARRI_POWER], TANARRI_SPEED)) chance += 10;
        if (victim->pcdata->powers[TANARRI_FURY_ON] == 1) chance += 7;
    }
    if (IS_CLASS(ch, CLASS_MILITARY)) chance -= ((ch->pcdata->Bootcamp[1])*4);
    if (IS_CLASS(victim, CLASS_MILITARY)) chance += ((victim->pcdata->Bootcamp[1])*4);

    if (IS_CLASS(ch, CLASS_DRAGON)) chance -= ch->pcdata->classfour[DRAGON_AGE] - 3;
    if (IS_CLASS(victim, CLASS_DRAGON)) chance += victim->pcdata->classfour[DRAGON_AGE] + 3;
    if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_ANGEL)) chance -= (ch->pcdata->powers[ANGEL_JUSTICE]*3);
    if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_ANGEL)) chance += (victim->pcdata->powers[ANGEL_PEACE]*3);
    if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_SHAPESHIFTER))
    {
        if (victim->pcdata->powers[SHAPE_FORM] == TIGER_FORM) chance += victim->pcdata->powers[TIGER_LEVEL]*2.75;
        else if (victim->pcdata->powers[SHAPE_FORM] == BULL_FORM) chance += victim->pcdata->powers[BULL_LEVEL]*3.25;
        else if (victim->pcdata->powers[SHAPE_FORM] == HYDRA_FORM) chance += victim->pcdata->powers[HYDRA_LEVEL]*3.5;
        else if (victim->pcdata->powers[SHAPE_FORM] == FAERIE_FORM) chance += victim->pcdata->powers[FAERIE_LEVEL]*7;
        else chance += 2;
    }
    if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_MAGE) && IS_ITEMAFF(ch, ITEMA_BEAST)) chance -= 10;
    if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_UNDEAD_KNIGHT)) chance -= ch->pcdata->powers[WEAPONSKILL];
    if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_UNDEAD_KNIGHT)) chance += victim->pcdata->powers[WEAPONSKILL]*1.5;
    if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_NINJA))
    {
        if( ch->pcdata->rank == BELT_TEN)  chance -= 30;
        else if( ch->pcdata->rank == BELT_NINE) chance -= 25;
        else if (ch->pcdata->rank == BELT_EIGHT) chance -= 20;
        else if (ch->pcdata->rank == BELT_SEVEN) chance -= 15;
        else if (ch->pcdata->rank == BELT_SIX) chance -= 10;
        else if (ch->pcdata->rank == BELT_FIVE) chance -= 5;
        else if (ch->pcdata->rank == BELT_FOUR) chance -= 4;
        else if (ch->pcdata->rank == BELT_THREE) chance -= 3;
    }
    if(!IS_NPC(ch) && IS_CLASS(ch, CLASS_DEMON)) chance -= (ch->power[DISC_DAEM_ATTA]);
    if(!IS_NPC(victim) && IS_CLASS(victim, CLASS_DEMON)) chance += (victim->power[DISC_DAEM_ATTA]);
    if(!IS_NPC(victim) && IS_CLASS(victim, CLASS_PHOENIX))chance+=10;
    if (IS_CLASS(ch, CLASS_ELF)) chance -= 25;
    if (IS_CLASS(ch, CLASS_WIZARD)) chance -= 20;
    if (IS_CLASS(ch, CLASS_KAHN)) chance -= 25;
    if (!IS_NPC(victim) && victim->power[DISC_VAMP_CELE] > 0 && IS_CLASS(victim, CLASS_VAMPIRE))
    {
        if( victim->pcdata->rank == AGE_TRUEBLOOD)  chance += 15;
        else if( victim->pcdata->rank == AGE_LA_MAGRA) chance += 13;
        else if (victim->pcdata->rank == AGE_METHUSELAH) chance += 10;
        else if (victim->pcdata->rank == AGE_ELDER) chance += 7;
        else if (victim->pcdata->rank == AGE_ANCILLA) chance += 5;
    }
    if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_NINJA))
    {
        if( victim->pcdata->rank == BELT_TEN)  chance += 30;
        else if( victim->pcdata->rank == BELT_NINE) chance += 25;
        else if (victim->pcdata->rank == BELT_EIGHT) chance += 20;
        else if (victim->pcdata->rank == BELT_SEVEN) chance += 15;
        else if (victim->pcdata->rank == BELT_SIX) chance += 10;
        else if (victim->pcdata->rank == BELT_FIVE) chance += 5;
        else if (victim->pcdata->rank == BELT_FOUR) chance += 4;
        else if (victim->pcdata->rank == BELT_THREE) chance += 3;
        else if (victim->pcdata->rank == BELT_TWO) chance += 2;
        else if (victim->pcdata->rank == BELT_ONE) chance += 1;
    }
    if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_SAMURAI)) chance += 15;
    if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_SAMURAI)) chance -=25;
    if (!IS_NPC(ch) && IS_CLASS(ch,CLASS_DROID)) chance -= ch->pcdata->powers[CYBORG_LIMBS]*3;
    if (!IS_NPC(victim) && IS_CLASS(victim,CLASS_DROID)) chance += victim->pcdata->powers[CYBORG_LIMBS]*4;
    if (IS_CLASS(victim, CLASS_ELF)) chance += 25;
    if (IS_CLASS(victim, CLASS_WIZARD)) chance += 20;
    if (IS_CLASS(victim, CLASS_KAHN)) chance += 25;
    if (victim->monkblock / 4 > 0 ) chance += victim->monkblock / 4;
    if (IS_CLASS(ch, CLASS_RUNEMASTER)) chance -= 20;
    if (IS_CLASS(victim, CLASS_RUNEMASTER)) chance += 20;
    if (IS_NPC(ch) && IS_SET(ch->act,ACT_KILLER))
        chance -= 55;
    if (number_percent( ) < 100 && number_percent( ) >= chance)
        return FALSE;
    if (!IS_NPC(victim))
    {
        if (IS_SET(victim->pcdata->jflags, JFLAG_BULLY) && number_range(1,3) != 2) return FALSE;
    }
    if (!IS_NPC(ch) && IS_NPC(victim)
        && IS_SET(ch->newbits, NEW_FIGHTDANCE)
        && IS_CLASS(ch, CLASS_DROW)
        && number_percent() >=50
        && ((( obj = get_eq_char(ch, WEAR_WIELD ))  != NULL && ( obj->value[3] == 4 || obj->value[3] == 11))
        || ((obj = get_eq_char (ch, WEAR_HOLD))!= NULL &&( obj->value[3] == 4 || obj->value[3] == 11))))
    {
        act( "$n's dark weapon dances around your defences, making a strange sound.",  ch, NULL, victim, TO_VICT );
        act( "Your sacrificial weapon hums in the air as it dances $N's defences.", ch, obj, victim, TO_CHAR );
        return FALSE;
    }
    if (claws)
    {
        if (IS_NPC(victim) || !IS_SET(victim->act, PLR_BRIEF3))
        {
            if (IS_CLASS(victim, CLASS_WEREWOLF))
                act( "You parry $n's blow with your claws.",  ch, NULL, victim, TO_VICT );
            else
                act( "You parry $n's blow with your Adamantium hands.", ch,NULL,victim, TO_VICT);
        }
        if (IS_CLASS(victim, CLASS_MONK) && victim->monkblock < 150 && number_range(1,2)==1)
            victim->monkblock++;
        if (IS_NPC(ch) || !IS_SET(ch->act, PLR_BRIEF3))
        {
            if (IS_CLASS(victim, CLASS_WEREWOLF))
                act( "$N parries your blow with $S claws.", ch, NULL, victim, TO_CHAR );
            else
                act( "$N parries your blow with $S Adamantium claws.", ch,NULL,victim, TO_CHAR);
        }
        return TRUE;
    }
    if (!IS_NPC(victim) && obj != NULL && obj->item_type == ITEM_WEAPON && obj->value[3] >= 0 && obj->value[3] <= 12)
    {
        if (IS_NPC(victim) || !IS_SET(victim->act, PLR_BRIEF3))
            act( "You parry $n's blow with $p.",  ch, obj, victim, TO_VICT );
        if (IS_NPC(ch) || !IS_SET(ch->act, PLR_BRIEF3))
            act( "$N parries your blow with $p.", ch, obj, victim, TO_CHAR );
        return TRUE;
    }
    if (IS_NPC(victim) || !IS_SET(victim->act, PLR_BRIEF3))
        act( "You parry $n's attack.",  ch, NULL, victim, TO_VICT    );
    if (IS_NPC(ch) || !IS_SET(ch->act, PLR_BRIEF3))
        act( "$N parries your attack.", ch, NULL, victim, TO_CHAR    );
    return TRUE;
}

/*
 * Check for dodge.
 */
bool check_dodge( CHAR_DATA *ch, CHAR_DATA *victim, int dt )
{
    int chance = 0;

    if (!IS_AWAKE(victim)) return FALSE;
    if (!IS_NPC(ch)) chance -= (ch->wpn[dt-1000] * 0.005);
    else chance -= (ch->level * 0.01);
    if (!IS_NPC(victim)) chance += (victim->wpn[0] * 0.025);
    else chance += victim->level;
    if (!IS_NPC(victim) && IS_STANCE(victim, STANCE_MONGOOSE) && victim->stance[STANCE_MONGOOSE] > 0
        && !can_counter(ch) &&!can_bypass(ch,victim))
        chance += victim->stance[STANCE_MONGOOSE] * 0.025;
    if (!IS_NPC(victim) && IS_STANCE(victim, STANCE_SWALLOW) && victim->stance[STANCE_SWALLOW] > 0
        && !can_counter(ch) && !can_bypass(ch,victim))
        chance += victim->stance[STANCE_SWALLOW] * 0.025;

    /* ------------ This is the part for superstances, Jobo ------------------- */
    if (!IS_NPC(victim) && victim->stance[0] > 12
        && IS_SET(victim->stance[(victim->stance[0]+6)], STANCEPOWER_DODGE)
        && victim->stance[(victim->stance[0])] > 100 && !can_counter(ch) && !can_bypass(ch,victim))
        chance += victim->stance[(victim->stance[0])] * 0.025;
    /* ------------ This is the end for superstances, Jobo ------------------- */

    if (!IS_NPC(ch))
    {
        if (ch->pcdata->upgrade_level > 0) chance -= ch->pcdata->upgrade_level*10;
        if (IS_CLASS(ch,CLASS_DROID)) chance -= ch->pcdata->powers[CYBORG_LIMBS] * 5;
        if (IS_CLASS(ch, CLASS_LICH))
        {
            chance -= 20;
            chance -= ch->spl[1]/40;
        }
        if (IS_CLASS(ch, CLASS_SHAPESHIFTER))
        {
            if (ch->pcdata->powers[SHAPE_FORM] == TIGER_FORM) chance -= ch->pcdata->powers[TIGER_LEVEL]*7;
            else if (ch->pcdata->powers[SHAPE_FORM] == BULL_FORM) chance -= ch->pcdata->powers[BULL_LEVEL]*5.5;
            else if (ch->pcdata->powers[SHAPE_FORM] == HYDRA_FORM) chance -= ch->pcdata->powers[HYDRA_LEVEL]*5;
            else if (ch->pcdata->powers[SHAPE_FORM] == FAERIE_FORM) chance -= ch->pcdata->powers[FAERIE_LEVEL]*4;
            else chance -= 10;
        }
        if (IS_CLASS(ch, CLASS_TANARRI))
        {
            if (IS_SET(ch->pcdata->powers[TANARRI_POWER], TANARRI_HEAD)) chance -= 15;
            if (IS_SET(ch->pcdata->powers[TANARRI_POWER], TANARRI_SPEED)) chance -= 17;
            if (ch->pcdata->powers[TANARRI_FURY_ON] == 1) chance -= 17;
        }
        if (IS_CLASS(ch, CLASS_MILITARY)) chance -= ((ch->pcdata->Bootcamp[1])*4);
        if (IS_CLASS(ch, CLASS_DRAGON)) chance -= ch->pcdata->classfour[DRAGON_AGE] - 3;
        if (IS_CLASS(ch, CLASS_ANGEL)) chance -= (ch->pcdata->powers[ANGEL_JUSTICE]*9);
        if (IS_CLASS(ch, CLASS_NINJA)  && ch->pcdata->powers[NPOWER_CHIKYU] >= 6 && ch->pcdata->powers[HARA_KIRI] > 0)
            chance -= 35;
        if (IS_CLASS(ch, CLASS_MONK))
        {
            chance -= 10;
            //	if ( get_eq_char(ch,WEAR_WIELD) == NULL )
            //	chance += 20;
        }
        if (ch->pcdata->powers[NPOWER_CHIKYU] >= 5 && IS_CLASS(ch, CLASS_NINJA)) chance -= 40;
        if (ch->power[DISC_VAMP_CELE] > 0 && IS_CLASS(ch, CLASS_VAMPIRE)) chance -= (ch->power[DISC_VAMP_CELE]*4);
        if (IS_CLASS(ch, CLASS_DEMON) && IS_DEMPOWER( ch, DEM_SPEED)) chance -= 25;
        if (IS_CLASS(ch, CLASS_PHOENIX )) chance+=13+(ch->pcdata->phoenix[1]*2);
        if (IS_CLASS(ch, CLASS_MAGE) && IS_ITEMAFF(ch, ITEMA_BEAST)) chance -= 30;
        if (IS_CLASS(ch, CLASS_MAGE) ) chance -= ch->spl[1]/50;
        if (IS_CLASS(ch, CLASS_WEREWOLF) && ch->power[DISC_WERE_MANT] > 6) chance -= (ch->power[DISC_WERE_MANT] * 3);
        if (!IS_CLASS(ch, CLASS_WEREWOLF) && (IS_ITEMAFF(ch,ITEMA_AFFMANTIS))) chance -= 12;
        if (IS_CLASS(ch, CLASS_DROW) && IS_SET(ch->pcdata->powers[1], DPOWER_SPEED)) chance -= 50;
        if (IS_CLASS(ch, CLASS_UNDEAD_KNIGHT)) chance -= ch->pcdata->powers[WEAPONSKILL]*4;
        if (IS_CLASS(ch, CLASS_SAMURAI)) chance -= 10;
        if (IS_CLASS(ch, CLASS_WRAITH)) chance -= 60;
        if (IS_CLASS(ch, CLASS_ELF)) chance -= 13;
        if (IS_CLASS(ch, CLASS_WIZARD)) chance -= 15;
        if (IS_CLASS(ch, CLASS_KAHN)) chance -= 25;
        if (IS_CLASS(ch, CLASS_RUNEMASTER)) chance += 30;
    }
    if (!IS_NPC(victim))
    {
        if (IS_CLASS(victim,CLASS_DROID)) chance += victim->pcdata->powers[CYBORG_LIMBS]*5;
        if (victim->pcdata->upgrade_level > 0) chance += victim->pcdata->upgrade_level*10;
        if (IS_CLASS(victim, CLASS_LICH)) chance += 20;
        if (victim->pcdata->powers[NPOWER_CHIKYU] >= 5 && IS_CLASS(victim, CLASS_NINJA)) chance += 40;
        if (IS_CLASS(victim, CLASS_NINJA) && victim->pcdata->powers[NPOWER_CHIKYU] >= 6 && victim->pcdata->powers[HARA_KIRI] > 0) chance += 35;
        if (IS_CLASS(victim, CLASS_SHAPESHIFTER))
        {
            if (victim->pcdata->powers[SHAPE_FORM] == TIGER_FORM) chance += victim->pcdata->powers[TIGER_LEVEL]*8;
            else if (victim->pcdata->powers[SHAPE_FORM] == BULL_FORM) chance += victim->pcdata->powers[BULL_LEVEL]*9;
            else if (victim->pcdata->powers[SHAPE_FORM] == HYDRA_FORM) chance += victim->pcdata->powers[HYDRA_LEVEL]*8;
            else if (victim->pcdata->powers[SHAPE_FORM] == FAERIE_FORM) chance += victim->pcdata->powers[FAERIE_LEVEL]*13;
            else chance += 10;
        }
        if (IS_CLASS(victim, CLASS_MONK))
        {
            chance += 20;
            //	if ( get_eq_char(victim,WEAR_WIELD) == NULL )
            //	chance -= 20;
        }
        if (IS_CLASS(victim, CLASS_TANARRI))
        {
            if (IS_SET(victim->pcdata->powers[TANARRI_POWER], TANARRI_HEAD)) chance += 15;
            if (IS_SET(victim->pcdata->powers[TANARRI_POWER], TANARRI_SPEED)) chance += 17;
            if (victim->pcdata->powers[TANARRI_FURY_ON] == 1) chance += 12;
        }
        if (IS_CLASS(victim, CLASS_MILITARY)) chance += ((victim->pcdata->Bootcamp[1])*4);

        if (IS_CLASS(victim, CLASS_DRAGON)) chance += victim->pcdata->classfour[DRAGON_AGE] + 3;
        if (IS_CLASS(victim, CLASS_ANGEL)) chance += (victim->pcdata->powers[ANGEL_PEACE]*9);
        if (victim->power[DISC_VAMP_CELE] > 0 && IS_CLASS(victim, CLASS_VAMPIRE)) chance += (victim->power[DISC_VAMP_CELE]*3);
        if (IS_CLASS(victim, CLASS_DEMON) && IS_DEMPOWER( victim, DEM_SPEED)) chance += 25;
        if (IS_CLASS(victim, CLASS_PHOENIX )) chance+=13+(victim->pcdata->phoenix[1]*2);

        if (IS_CLASS(victim, CLASS_WEREWOLF) && victim->power[DISC_WERE_MANT] > 6) chance +=(victim->power[DISC_WERE_MANT] * 3.5);
        if (IS_CLASS(victim, CLASS_DROW) &&  IS_SET(victim->pcdata->powers[1], DPOWER_SPEED)) chance += 50;
        if (IS_CLASS(victim, CLASS_SAMURAI)) chance +=15;
        if (IS_CLASS(victim, CLASS_WRAITH)) chance += 55;
        if (IS_CLASS(victim, CLASS_ELF)) chance += 13;
        if (IS_CLASS(victim, CLASS_WIZARD)) chance += 15;
        if (IS_CLASS(victim, CLASS_KAHN)) chance += 25;
        if (IS_CLASS(victim, CLASS_UNDEAD_KNIGHT)) chance += victim->pcdata->powers[WEAPONSKILL]*4.5;
        if (!IS_CLASS(victim, CLASS_WEREWOLF) && IS_ITEMAFF(victim,ITEMA_AFFMANTIS)) chance += 15;
        if (IS_CLASS(victim, CLASS_MAGE) && IS_ITEMAFF(victim, ITEMA_DEFLECTOR)) chance += 40;
        if (IS_CLASS(victim, CLASS_RUNEMASTER)) chance += 30;
    }
    if (chance > 80) chance = 80;
    else if (chance < 20) chance = 20;
    if (!IS_NPC(ch) && IS_CLASS(ch,CLASS_DROID)) chance -= ch->pcdata->powers[CYBORG_LIMBS]*3;
    if (!IS_NPC(victim) && IS_CLASS(victim,CLASS_DROID)) chance += victim->pcdata->powers[CYBORG_LIMBS]*4;
    if (IS_CLASS(ch, CLASS_LICH) && !IS_NPC(ch)) chance -= 7.5;
    if (IS_CLASS(victim, CLASS_LICH) && !IS_NPC(victim)) chance += 15;
    if (IS_CLASS(ch, CLASS_SHAPESHIFTER) && !IS_NPC(ch))
    {
        if (ch->pcdata->powers[SHAPE_FORM] == TIGER_FORM) chance -= ch->pcdata->powers[TIGER_LEVEL]*4;
        else if (ch->pcdata->powers[SHAPE_FORM] == BULL_FORM) chance -= ch->pcdata->powers[BULL_LEVEL]*3;
        else if (ch->pcdata->powers[SHAPE_FORM] == HYDRA_FORM) chance -= ch->pcdata->powers[HYDRA_LEVEL]*3;
        else if (ch->pcdata->powers[SHAPE_FORM] == FAERIE_FORM) chance -= ch->pcdata->powers[FAERIE_LEVEL]*2;
        else chance -= 2;
    }
    if (IS_CLASS(victim, CLASS_SHAPESHIFTER) && !IS_NPC(victim))
    {
        if (victim->pcdata->powers[SHAPE_FORM] == TIGER_FORM) chance += victim->pcdata->powers[TIGER_LEVEL]*2;
        else if (victim->pcdata->powers[SHAPE_FORM] == BULL_FORM) chance += victim->pcdata->powers[BULL_LEVEL]*3;
        else if (victim->pcdata->powers[SHAPE_FORM] == HYDRA_FORM) chance += victim->pcdata->powers[HYDRA_LEVEL]*3;
        else if (victim->pcdata->powers[SHAPE_FORM] == FAERIE_FORM) chance += victim->pcdata->powers[FAERIE_LEVEL]*6;
        else chance += 2;
    }
    if (IS_CLASS(ch, CLASS_TANARRI) && !IS_NPC(ch))
    {
        if (IS_SET(ch->pcdata->powers[TANARRI_POWER], TANARRI_HEAD)) chance -= 7;
        if (IS_SET(ch->pcdata->powers[TANARRI_POWER], TANARRI_SPEED)) chance -= 10;
        if (ch->pcdata->powers[TANARRI_FURY_ON] == 1) chance -= 7;
    }
    if (IS_CLASS(victim, CLASS_TANARRI) && !IS_NPC(victim))
    {
        if (IS_SET(victim->pcdata->powers[TANARRI_POWER], TANARRI_HEAD)) chance += 7;
        if (IS_SET(victim->pcdata->powers[TANARRI_POWER], TANARRI_SPEED)) chance += 10;
        if (victim->pcdata->powers[TANARRI_FURY_ON] == 1) chance += 7;
    }
    if (IS_CLASS(ch, CLASS_MILITARY)) chance -= ((ch->pcdata->Bootcamp[1])*4);
    if (IS_CLASS(victim, CLASS_MILITARY)) chance += ((victim->pcdata->Bootcamp[1])*4);

    if (IS_CLASS(ch, CLASS_DRAGON)) chance -= ch->pcdata->classfour[DRAGON_AGE] - 3;
    if (IS_CLASS(victim, CLASS_DRAGON)) chance += victim->pcdata->classfour[DRAGON_AGE] + 3;
    if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_ANGEL)) chance -= (ch->pcdata->powers[ANGEL_JUSTICE]*3);
    if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_ANGEL)) chance += (victim->pcdata->powers[ANGEL_PEACE]*3);
    if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_DROW) && IS_SET(victim->pcdata->powers[1], DPOWER_SPEED)) chance += 40;
    if (IS_CLASS(victim, CLASS_DEMON) && IS_DEMPOWER( victim, DEM_SPEED)) chance += 25;
    if (IS_CLASS(victim, CLASS_PHOENIX) ) chance+=13+(victim->pcdata->phoenix[1]*2);

    if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_UNDEAD_KNIGHT)) chance -= ch->pcdata->powers[WEAPONSKILL];
    if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_UNDEAD_KNIGHT)) chance += victim->pcdata->powers[WEAPONSKILL]*2;
    if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_SAMURAI)) chance -= 25;
    if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_SAMURAI)) chance +=25;
    if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_MAGE) && IS_ITEMAFF(ch, ITEMA_BEAST)) chance -= 10;
    if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_MAGE) && IS_ITEMAFF(victim, ITEMA_BEAST)) chance += 10;
    if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_WEREWOLF) && ch->power[DISC_WERE_MANT] > 6) chance -= (ch->power[DISC_WERE_MANT]);
    if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_WEREWOLF) && victim->power[DISC_WERE_MANT] > 6) chance += (victim->power[DISC_WERE_MANT]);
    if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_VAMPIRE))
    {
        if( ch->pcdata->rank == AGE_TRUEBLOOD)  chance -= 15;
        else if( ch->pcdata->rank == AGE_LA_MAGRA) chance -= 13;
        else if (ch->pcdata->rank == AGE_METHUSELAH) chance -= 10;
        else if (ch->pcdata->rank == AGE_ELDER) chance -= 7;
        else if (ch->pcdata->rank == AGE_ANCILLA) chance -= 5;
    }
    if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_NINJA))
    {
        if( ch->pcdata->rank == BELT_TEN)  chance -= 20;
        else if( ch->pcdata->rank == BELT_NINE) chance -= 18;
        else if (ch->pcdata->rank == BELT_EIGHT) chance -= 17;
        else if (ch->pcdata->rank == BELT_SEVEN) chance -= 14;
        else if (ch->pcdata->rank == BELT_SIX) chance -= 12;
        else if (ch->pcdata->rank == BELT_FIVE) chance -= 10;
        else if (ch->pcdata->rank == BELT_FOUR) chance -= 9;
        else if (ch->pcdata->rank == BELT_THREE) chance -= 7;
        else if (ch->pcdata->rank == BELT_TWO) chance -= 4;
        else if (ch->pcdata->rank == BELT_ONE) chance -= 2;
    }
    if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_VAMPIRE))
    {
        if(victim->pcdata->rank == AGE_TRUEBLOOD)  chance += 15;
        else if(victim->pcdata->rank == AGE_LA_MAGRA) chance += 13;
        else if (victim->pcdata->rank == AGE_METHUSELAH) chance += 10;
        else if (victim->pcdata->rank == AGE_ELDER) chance += 7;
        else if (victim->pcdata->rank == AGE_ANCILLA) chance += 5;
    }

    if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_NINJA))
    {
        if( victim->pcdata->rank == BELT_TEN)  chance += 20;
        else if( victim->pcdata->rank == BELT_NINE) chance += 18;
        else if (victim->pcdata->rank == BELT_EIGHT) chance += 17;
        else if (victim->pcdata->rank == BELT_SEVEN) chance += 14;
        else if (victim->pcdata->rank == BELT_SIX) chance += 11;
        else if (victim->pcdata->rank == BELT_FIVE) chance += 9;
        else if (victim->pcdata->rank == BELT_FOUR) chance += 7;
        else if (victim->pcdata->rank == BELT_THREE) chance += 5;
        else if (victim->pcdata->rank == BELT_TWO) chance += 4;
        else if (victim->pcdata->rank == BELT_ONE) chance += 2;
    }
    if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_MONK) && ( victim->monkblock / 2 > 0 )) chance += (victim->monkblock / 2);
    if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_RUNEMASTER)) chance -= 20;
    if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_RUNEMASTER)) chance += 20;
    if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_ELF)) chance -= 25;
    if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_WIZARD)) chance -= 20;
    if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_ELF)) chance += 25;
    if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_WIZARD)) chance += 20;
    if (IS_CLASS(ch, CLASS_KAHN)) chance -= 25;
    if (IS_CLASS(victim, CLASS_KAHN)) chance += 25;
    if ( chance > 90) chance = 90;
    if (!IS_NPC(victim))
    {
        if (IS_SET(victim->pcdata->jflags, JFLAG_BULLY) && number_range(1,3) != 2) return FALSE;
    }
    if (IS_NPC(ch) && IS_SET(ch->act,ACT_KILLER))
        chance -= 55;
    if ( number_percent( ) >= chance )
        return FALSE;
    if (IS_NPC(victim) || !IS_SET(victim->act, PLR_BRIEF3))
        act( "You dodge $n's attack.", ch, NULL, victim, TO_VICT );
    if (IS_NPC(ch) || !IS_SET(ch->act, PLR_BRIEF3))
        act( "$N dodges your attack.", ch, NULL, victim, TO_CHAR );
    return TRUE;
}

/*
 * Set position of a victim.
 */
void update_pos( CHAR_DATA *victim )
{
    CHAR_DATA *mount;
    if ( ((get_age(victim) - 17) < 5) && !IS_NPC(victim))
        victim->hit = victim->max_hit;
    if (victim->hit > 0)
    {
        if (victim->position <= POS_STUNNED)
        {
            bool gm_stance = FALSE;
            victim->position = POS_STANDING;
            if (!IS_NPC(victim) && victim->stance[0] > 0)
            {
                int stance = victim->stance[0];
                if (victim->stance[stance] >= 200) gm_stance = TRUE;
            }
            if (IS_NPC(victim) || victim->max_hit * 0.25 > victim->hit)
            {
                act( "$n clambers back to $s feet.", victim, NULL, NULL, TO_ROOM);
                act( "You clamber back to your feet.", victim, NULL, NULL, TO_CHAR);
            }
            else
            {
                act( "$n flips back up to $s feet.", victim, NULL, NULL, TO_ROOM);
                act( "You flip back up to your feet.", victim, NULL, NULL, TO_CHAR);
            }
        }
        return;
    }
    else if ((mount = victim->mount) != NULL)
    {
        if (victim->mounted == IS_MOUNT)
        {
            act("$n rolls off $N.",mount,NULL,victim,TO_ROOM);
            act("You roll off $N.",mount,NULL,victim,TO_CHAR);
        }
        else if (victim->mounted == IS_RIDING)
        {
            act("$n falls off $N.",victim,NULL,mount,TO_ROOM);
            act("You fall off $N.",victim,NULL,mount,TO_CHAR);
        }
        mount->mount    = NULL;
        victim->mount   = NULL;
        mount->mounted  = IS_ON_FOOT;
        victim->mounted = IS_ON_FOOT;
    }
    if (!IS_NPC(victim) && victim->hit <=-11 && IS_HERO(victim))
    {
        if ( victim->pcdata->upgrade_level == 0 && ((get_age(victim) - 17) < 5) )
        {
            //	victim->hit = victim->max_hit;
            //	victim->mana = victim->max_mana;
            //	victim->move = victim->max_move;
            if (victim->position == POS_FIGHTING) stop_fighting(victim,TRUE);
            return;
        }
        victim->hit = -10;
        if (victim->position == POS_FIGHTING) stop_fighting(victim,TRUE);
        return;
    }
    if (!IS_NPC(victim) && victim->hit <-9 && !IS_HERO(victim))
    {
        victim->hit = 1;

        do_killperson(victim,victim->name);
        victim->mdeath=victim->mdeath+1;
    }
    if(IS_NPC(victim) && IS_SET(victim->act, ACT_DUMMY) && victim->hit <= 30000 )
    {
        victim->hit = 30000;
        victim->max_hit = 30000;
        return;
    }
    if(IS_NPC(victim) && victim->hit < -6 )
    {
        victim->position = POS_DEAD;
        return;
    }
    if (victim->hit <= -6 ) victim->position = POS_MORTAL;
    else if ( victim->hit <= -3 ) victim->position = POS_INCAP;
    else if (victim->hit <= 0 ) victim->position = POS_STUNNED;
    else victim->position = POS_STANDING;
    return;
}

/*
 * Start fights.
 */
void set_fighting( CHAR_DATA *ch, CHAR_DATA *victim )
{
    DESCRIPTOR_DATA * d;
    char buf[MAX_STRING_LENGTH];

    if(ch->fighting != NULL) return;

    if(!IS_NPC(ch) && !IS_NPC(victim))
    {
        sprintf(buf, "%s attacking %s at %d.", ch->name, victim->name, ch->in_room->vnum);
        wiznet( NULL, WIZ_DEATH, 0, buf );
        log_string(buf);

        sprintf(buf, "%s is attacking %s!\n\r",ch->name, victim->name);
        do_info(ch,buf);

        if((victim->fighting != NULL))
        {
            if(IS_NPC(victim->fighting))
            {
                sprintf(buf, "%s attacking %s while they are fighting a NPC.", ch->name, victim->name);
                wiznet( NULL, WIZ_DEATH, 0, buf );
                log_string(buf);
            }
            else if(victim->fighting != ch)
            {
                sprintf(buf, "%s attacking %s while they are fighting %s.", ch->name, victim->name, victim->fighting->name);
                wiznet( NULL, WIZ_DEATH, 0, buf );
                log_string(buf);
            }
        }

        for(d = descriptor_list; d != NULL; d = d->next)
        {
            if((d->character != NULL) && !IS_NPC(d->character) && (d->character != ch) && (d->character->fighting == victim))
            {
                sprintf(buf, "%s grouping with %s against %s in room %d.", ch->name, d->character->name, victim->name, ch->in_room->vnum);
                wiznet( NULL, WIZ_DEATH, 0, buf );
                log_string(buf);
            }
        }
    }

    if ( IS_AFFECTED(ch, AFF_SLEEP) )
        affect_strip( ch, gsn_sleep );
    ch->fighting = victim;
    ch->position = POS_FIGHTING;
    ch->damcap[DAM_CHANGE] = 1;
    autodrop(ch);
    autodrop(victim);
    dropinvis(ch);
    dropinvis(victim);
    return;
}

/*
 * Stop fights.
 */
void stop_embrace( CHAR_DATA *ch, CHAR_DATA *victim )
{
    if ( victim == NULL)
    {
        ch->embracing=NULL;
        return;
    }
    victim->embraced=NULL;
    ch->embracing=NULL;
    return;
}

void stop_fighting( CHAR_DATA *ch, bool fBoth )
{
    CHAR_DATA *fch;
    for ( fch = char_list; fch != NULL; fch = fch->next )
    {
        if ( fch == ch || ( fBoth && fch->fighting == ch ) )
        {
            fch->fighting = NULL;
            fch->position = POS_STANDING;
            update_pos( fch );
        }
    }
    return;
}

/*
 * Make a corpse out of a character.
 */
void make_corpse( CHAR_DATA *ch )
{
    char buf[MAX_STRING_LENGTH];
    char buf2[MAX_STRING_LENGTH];
    OBJ_DATA *corpse;
    OBJ_DATA *obj;
    OBJ_DATA *obj_next;
    char *name;

    if (!ch) return;
    if (ch->in_room == NULL)
    {
        return;
    }
    if ( IS_NPC(ch) )
    {
        name = ch->short_descr;
        corpse = create_object(get_obj_index(OBJ_VNUM_CORPSE_NPC), 0);
        corpse->timer = number_range( 4, 8 );
        corpse->value[2]= ch->pIndexData->vnum;
    }
    else
    {
        name  = ch->name;
        corpse  = create_object(get_obj_index(OBJ_VNUM_CORPSE_PC), 0);
        corpse->timer = number_range( 25, 40 );
    }
    if (IS_SET(ch->extra, EXTRA_ZOMBIE))
        SET_BIT(corpse->quest, QUEST_ZOMBIE);
    sprintf( buf, corpse->short_descr, name );
    free_string( corpse->short_descr );
    corpse->short_descr = str_dup( buf );

    sprintf( buf2, "Corpse of %s", ch->name );
    free_string( corpse->name );
    corpse->name = str_dup( buf2 );

    sprintf( buf, corpse->description, name );
    free_string( corpse->description );
    corpse->description = str_dup( buf );
    for ( obj = ch->carrying; obj != NULL; obj = obj_next )
    {
        obj_next = obj->next_content;
        obj_from_char( obj );
        if (IS_SET( obj->extra_flags, ITEM_VANISH )) extract_obj( obj );
        else
        {
            if (IS_AFFECTED(ch,AFF_SHADOWPLANE) && (!IS_SET(obj->extra_flags, ITEM_SHADOWPLANE)))
                SET_BIT(obj->extra_flags, ITEM_SHADOWPLANE);
            obj_to_obj( obj, corpse );
        }
    }
    if (IS_AFFECTED(ch, AFF_SHADOWPLANE)) SET_BIT(corpse->extra_flags, ITEM_SHADOWPLANE);
    obj_to_room( corpse, ch->in_room );
    return;
}

void make_part( CHAR_DATA *ch, char *argument )
{
    char  arg [MAX_INPUT_LENGTH];
    int vnum;

    argument = one_argument(argument,arg);
    vnum = 0;

    if (arg[0] == '\0') return;
    if (ch->in_room == NULL) return;

    if (!str_cmp(arg,"head")) vnum = OBJ_VNUM_SEVERED_HEAD;
    else if (!str_cmp(arg,"arm")) vnum = OBJ_VNUM_SLICED_ARM;
    else if (!str_cmp(arg,"leg")) vnum = OBJ_VNUM_SLICED_LEG;
    else if (!str_cmp(arg,"heart")) vnum = OBJ_VNUM_TORN_HEART;
    else if (!str_cmp(arg,"turd")) vnum = OBJ_VNUM_TORN_HEART;
    else if (!str_cmp(arg,"entrails")) vnum = OBJ_VNUM_SPILLED_ENTRAILS;
    else if (!str_cmp(arg,"brain")) vnum = OBJ_VNUM_QUIVERING_BRAIN;
    else if (!str_cmp(arg,"eyeball")) vnum = OBJ_VNUM_SQUIDGY_EYEBALL;
    else if (!str_cmp(arg,"blood")) vnum = OBJ_VNUM_SPILT_BLOOD;
    else if (!str_cmp(arg,"face")) vnum = OBJ_VNUM_RIPPED_FACE;
    else if (!str_cmp(arg,"windpipe")) vnum = OBJ_VNUM_TORN_WINDPIPE;
    else if (!str_cmp(arg,"cracked_head")) vnum = OBJ_VNUM_CRACKED_HEAD;
    else if (!str_cmp(arg,"ear")) vnum = OBJ_VNUM_SLICED_EAR;
    else if (!str_cmp(arg,"nose")) vnum = OBJ_VNUM_SLICED_NOSE;
    else if (!str_cmp(arg,"tooth")) vnum = OBJ_VNUM_KNOCKED_TOOTH;
    else if (!str_cmp(arg,"tongue")) vnum = OBJ_VNUM_TORN_TONGUE;
    else if (!str_cmp(arg,"hand")) vnum = OBJ_VNUM_SEVERED_HAND;
    else if (!str_cmp(arg,"foot")) vnum = OBJ_VNUM_SEVERED_FOOT;
    else if (!str_cmp(arg,"thumb")) vnum = OBJ_VNUM_SEVERED_THUMB;
    else if (!str_cmp(arg,"index")) vnum = OBJ_VNUM_SEVERED_INDEX;
    else if (!str_cmp(arg,"middle")) vnum = OBJ_VNUM_SEVERED_MIDDLE;
    else if (!str_cmp(arg,"ring")) vnum = OBJ_VNUM_SEVERED_RING;
    else if (!str_cmp(arg,"little")) vnum = OBJ_VNUM_SEVERED_LITTLE;
    else if (!str_cmp(arg,"toe")) vnum = OBJ_VNUM_SEVERED_TOE;

    if ( vnum != 0 )
    {
        char buf[MAX_STRING_LENGTH];
        OBJ_DATA *obj;
        char *name;

        name = IS_NPC(ch) ? ch->short_descr : ch->name;
        obj = create_object( get_obj_index( vnum ), 0 );
        obj->timer = number_range( 1, 3);                   // try this.. might work. Jobo

        /*
            if (IS_NPC(ch)) obj->timer = number_range(2,5);
            else obj->timer = -1;
        */

        if (!str_cmp(arg,"head") && IS_NPC(ch))
            obj->value[1] = ch->pIndexData->vnum;
        else if (!str_cmp(arg,"head") && !IS_NPC(ch))
            {ch->pcdata->chobj = obj; obj->chobj = ch;obj->timer = number_range(1,2); obj->item_type = ITEM_HEAD;}
            else if (!str_cmp(arg,"arm"))
                SET_BIT(obj->extra_flags2, ITEM_ARM);
        else if (!str_cmp(arg,"brain") && !IS_NPC(ch) &&  IS_AFFECTED(ch,AFF_POLYMORPH) && IS_HEAD(ch, LOST_HEAD))
        {
            if (ch->pcdata->chobj != NULL) ch->pcdata->chobj->chobj = NULL;
            ch->pcdata->chobj = obj;
            obj->chobj = ch;
            obj->timer = number_range(1,2);
            obj->item_type = ITEM_HEAD;
        }
        if (vnum == OBJ_VNUM_SPILT_BLOOD) obj->timer = 2;
        if (!IS_NPC(ch))
        {
            sprintf( buf, obj->name, name );
            free_string( obj->name );
            obj->name = str_dup( buf );
        }
        else
        {
            sprintf( buf, obj->name, "mob" );
            free_string( obj->name );
            obj->name = str_dup( buf );
        }
        sprintf( buf, obj->short_descr, name );
        free_string( obj->short_descr );
        obj->short_descr = str_dup( buf );
        sprintf( buf, obj->description, name );
        free_string( obj->description );
        obj->description = str_dup( buf );
        if (IS_AFFECTED(ch, AFF_SHADOWPLANE))
            SET_BIT(obj->extra_flags, ITEM_SHADOWPLANE);
        obj_to_room( obj, ch->in_room );
    }
    return;
}

void raw_kill( CHAR_DATA *victim )
{
    CHAR_DATA *mount;
    stop_fighting( victim, TRUE );
    make_corpse( victim );
    if ( (mount = victim->mount) != NULL)
    {
        if (victim->mounted == IS_MOUNT)
        {
            act("$n rolls off the corpse of $N.",mount,NULL,victim,TO_ROOM);
            act("You roll off the corpse of $N.",mount,NULL,victim,TO_CHAR);
        }
        else if (victim->mounted == IS_RIDING)
        {
            act("$n falls off $N.",victim,NULL,mount,TO_ROOM);
            act("You fall off $N.",victim,NULL,mount,TO_CHAR);
        }
        mount->mount    = NULL;
        victim->mount   = NULL;
        mount->mounted  = IS_ON_FOOT;
        victim->mounted = IS_ON_FOOT;
    }
    if ( IS_NPC(victim) )
    {
        victim->pIndexData->killed++;
        kill_table[URANGE(0, victim->level, MAX_KILLLEVEL-1)].killed++;
        extract_char( victim, TRUE );
        return;
    }
    extract_char( victim, FALSE );
    while ( victim->affected )
        affect_remove( victim, victim->affected );
    if (IS_AFFECTED(victim,AFF_POLYMORPH) && IS_AFFECTED(victim,AFF_ETHEREAL))
    {
        victim->affected_by = AFF_POLYMORPH + AFF_ETHEREAL;
    }
    else if (IS_AFFECTED(victim,AFF_POLYMORPH)) victim->affected_by = AFF_POLYMORPH;
    else if (IS_AFFECTED(victim,AFF_ETHEREAL)) victim->affected_by = AFF_ETHEREAL;
    else victim->affected_by = 0;
    REMOVE_BIT(victim->immune, IMM_STAKE);
    REMOVE_BIT(victim->extra, TIED_UP);
    REMOVE_BIT(victim->extra, GAGGED);
    REMOVE_BIT(victim->extra, BLINDFOLDED);
    victim->pcdata->stats[DEMON_POWER] = 0;
    victim->itemaffect   = 0;
    victim->loc_hp[0]    = 0;
    victim->loc_hp[1]    = 0;
    victim->loc_hp[2]    = 0;
    victim->loc_hp[3]    = 0;
    victim->loc_hp[4]    = 0;
    victim->loc_hp[5]    = 0;
    victim->loc_hp[6]    = 0;
    victim->armor  = 100;
    victim->position   = POS_RESTING;
    victim->hit    = UMAX( 1, victim->hit  );
    victim->mana   = UMAX( 1, victim->mana );
    victim->move   = UMAX( 1, victim->move );
    victim->hitroll  = 0;
    victim->damroll  = 0;
    victim->saving_throw = 0;
    victim->carry_weight = 0;
    victim->carry_number = 0;
    do_call(victim,"all");
    save_char_obj( victim );
    return;
}

void behead( CHAR_DATA *victim )
{
    char buf [MAX_STRING_LENGTH];
    ROOM_INDEX_DATA *location;

    if (IS_NPC(victim)) return;
    powerdown(victim);                                      /* remove all class shit - Jobo */
    location = victim->in_room;
    stop_fighting( victim, TRUE );
    make_corpse( victim );
    extract_char( victim, FALSE );
    char_from_room(victim);
    char_to_room(victim,location);
    if (victim == NULL)
        {bug( "Behead: Victim no longer exists.", 0 );return;}
        make_part( victim, "head" );
    while ( victim->affected )
        affect_remove( victim, victim->affected );
    victim->affected_by = 0;
    REMOVE_BIT(victim->immune, IMM_STAKE);
    REMOVE_BIT(victim->extra, TIED_UP);
    REMOVE_BIT(victim->extra, GAGGED);
    REMOVE_BIT(victim->extra, BLINDFOLDED);
    REMOVE_BIT(victim->extra, EXTRA_PREGNANT);
    REMOVE_BIT(victim->extra, EXTRA_LABOUR);
    victim->pcdata->stats[DEMON_POWER] = 0;
    victim->itemaffect   = 0;
    victim->loc_hp[0]    = 0;
    victim->loc_hp[1]    = 0;
    victim->loc_hp[2]    = 0;
    victim->loc_hp[3]    = 0;
    victim->loc_hp[4]    = 0;
    victim->loc_hp[5]    = 0;
    victim->loc_hp[6]    = 0;
    victim->affected_by  = 0;
    victim->armor  = 100;
    victim->position   = POS_STANDING;
    victim->hit    = 1;
    victim->mana   = UMAX( 1, victim->mana );
    victim->move   = UMAX( 1, victim->move );
    victim->hitroll  = 0;
    victim->damroll  = 0;
    victim->saving_throw = 0;
    victim->carry_weight = 0;
    victim->carry_number = 0;
    victim->pcdata->sit_safe  = 0;
    //  victim->pcdata->bloodpower[1] = 0;
    victim->fight_timer  = 0;
    SET_BIT(victim->loc_hp[0],LOST_HEAD);
    SET_BIT(victim->affected_by,AFF_POLYMORPH);
    sprintf(buf,"the severed head of %s",victim->name);
    free_string(victim->morph);
    victim->morph = str_dup(buf);
    do_call(victim,"all");
    save_char_obj( victim );
    return;
}

void group_gain( CHAR_DATA *ch, CHAR_DATA *victim )
{
    char buf[MAX_STRING_LENGTH];
    char buf2[MAX_STRING_LENGTH];
    CHAR_DATA *gch;
    CHAR_DATA *lch;
    CHAR_DATA *mount;
    int xp;
    int temp;
    int members;
    int nQuestBonus = 0;
    int nMultiplier = 1;
    int nBase = 1;
    /*
     * Monsters don't get kill xp's or alignment changes.
     * P-killing doesn't help either.
     * Dying of mortal wounds or poison doesn't give xp to anyone!
     */
    if ((IS_NPC(ch) && (mount = ch->mount) == NULL) || victim == ch) return;
    members = 0;
    for ( gch = ch->in_room->people; gch != NULL; gch = gch->next_in_room )
    {
        if ( is_same_group( gch, ch ) )
            members++;
    }
    if ( members == 0 )
    {
        bug( "Group_gain: members.", members );
        members = 1;
    }
    lch = (ch->leader != NULL) ? ch->leader : ch;
    for ( gch = ch->in_room->people; gch != NULL; gch = gch->next_in_room )
    {
        int xp_modifier = 100/(members+1);

        if (!is_same_group( gch, ch )) continue;
        xp = xp_compute( ch, victim );
        nBase = xp;
        nMultiplier = number_range(2,4);

        sprintf(buf2,"#RExp modifiers  #G:");
        if ((IS_EVIL(gch) && IS_GOOD(victim)) || (IS_GOOD(gch) && IS_EVIL(victim)))
        {
            xp_modifier += 1;
            strcat(buf2," #Calignment#n");
        }
        if ((IS_EVIL(gch) && IS_EVIL(victim)) || (IS_GOOD(gch) && IS_GOOD(victim)))
        {
            xp_modifier -= 1;
            strcat(buf2," #palignment#n");
        }
        if (gch != NULL && gch->desc != NULL)
        {
            if (gch->desc->out_compress)
            {
                xp_modifier += 2;
                strcat(buf2," #Cmccp#n");
            }
        }

        if((get_age(gch)-17)<2)                             /* 4 hours worth of newbie exp. */
        {
            xp_modifier += 5;
            strcat(buf2," #Cnewbie#n");
        }
        if (global_exp)
        {
            nMultiplier += 2;
            xp_modifier += 100;
            strcat(buf2," #Chappy hour#n");
        }
        if ( !IS_NPC(gch) && !IS_SET(gch->act, PLR_BRIEF5) )
        {
            xp_modifier -= 10;
            strcat(buf2,"#R no brief5#n");
        }
        if ( !IS_NPC(gch) && !IS_SET(gch->act, PLR_BRIEF6) )
        {
            xp_modifier -= 10;
            strcat(buf2,"#R no brief6#n");
        }

        if ((temp = gch->race ) <= 4)
        {
            xp_modifier += UMIN(100, temp);
            xp_modifier += temp*=3;
            strcat(buf2," #CWimp!#n");
        }
        if ((temp = gch->race ) == 5)
        {
            xp_modifier += UMIN(100, temp);
            xp_modifier += temp+=4;
            strcat(buf2," #CVeteran!#n");
        }
        if ((temp = gch->race ) == 6)
        {
            xp_modifier += UMIN(100, temp);
            xp_modifier += temp+=5;
            strcat(buf2," #CAdventurer!#n");
        }
        if ((temp = gch->race ) == 7)
        {
            xp_modifier += UMIN(100, temp);
            xp_modifier += temp+=6;
            strcat(buf2," #CSoldier!#n");
        }
        if ((temp = gch->race ) == 8)
        {
            xp_modifier += UMIN(100, temp);
            xp_modifier += temp+=7;
            strcat(buf2," #CSoldier!#n");
        }
        if ((temp = gch->race ) == 9)
        {
            xp_modifier += UMIN(100, temp);
            xp_modifier += temp+=8;
            strcat(buf2," #CHero!#n");
        }
        if ((temp = gch->race ) == 10)
        {
            xp_modifier += UMIN(100, temp);
            xp_modifier += temp +=9;
            strcat(buf2," #CLegendary!#n");
        }
        if ((temp = gch->race ) == 11)
        {
            xp_modifier += UMIN(100, temp);
            xp_modifier += temp +=10;
            strcat(buf2," #CLance!#n");
        }

        if (!IS_NPC(gch) && gch->pcdata->kingdom != 0)
        {
            xp_modifier -= 1;
            strcat(buf2," #pkingdom#n");
        }
        if (members > 1)
        {
            strcat(buf2," #pgrouped#n");
            xp_modifier *= 10;
            xp_modifier /= 5 * members;
        }
        strcat(buf2,"\n\r");
        xp = nBase*nMultiplier;
        xp = xp * ((double)xp_modifier / 100);
        xp=xp/2;
        if(ccenter[CCENTER_EXP_LEVEL]>10) ccenter[CCENTER_EXP_LEVEL]=ccenter[CCENTER_EXP_LEVEL]/2;
        xp *= ccenter[CCENTER_EXP_LEVEL];
        //    xp /= 50;
        if (xp > ccenter[CCENTER_MAX_EXP])
            xp = number_range(ccenter[CCENTER_MAX_EXP] * 0.9, ccenter[CCENTER_MAX_EXP] * 1.1);
        if (!IS_SET(gch->act, PLR_BRIEF4)) send_to_char(buf2,gch);
        sprintf(buf2,"#RTotal modifier #G:#n %d percent bonus\n\r",xp_modifier);
        if (!IS_SET(gch->act, PLR_BRIEF4)) send_to_char(buf2,gch);
        if (gch->exp > 2000000000)
        {
            send_to_char("Your experience points has been stored, type store to see what you can do.\n\r",gch);
            do_store(ch,"add");
        }
        if (xp > 200000000 && get_age(gch) - 17 < 2 ) xp = 200000000/5 + number_range(-100,100) * number_range(-100,100);
        else if (xp > 2000000000) xp = 2000000000 + number_range(-100,100) * number_range(-100,100);
        else if (xp > 2000000000) xp = 2000000000;
        if (xp < 0) xp = 2000000000 + number_range(-100,100) * number_range(-100, 100);
        if ( !IS_NPC(gch) )
        {
            sprintf( buf, "Base Exp: %d, Multiplier: %d\n\r",nBase, ccenter[CCENTER_EXP_LEVEL]);
            send_to_char( buf, gch );
            sprintf( buf, "You receive %d experience points.\n\r", xp );
            send_to_char( buf, gch );

            if ( pulse_doubleqp > 0)
            {
                nQuestBonus = number_range(1,ccenter[CCENTER_QPS_LEVEL]*2);
            }
            else
            {
                nQuestBonus = number_range(1,ccenter[CCENTER_QPS_LEVEL]);
            }
            sprintf( buf, "You receive %d qps for the kill.\n\r",nQuestBonus);
            send_to_char( buf, gch );
            gch->pcdata->quest += nQuestBonus;
            gain_exp( gch, xp );
            if ( IS_NPC(victim) && IS_SET(victim->act, ACT_QUEST) )
            {
                GiveBonus(gch,victim,"quest");
                /*		do_info(ch,"#yAnother quest mob has been brutally slain.\n\r");*/
            }
        }
        if ((mount = gch->mount) != NULL) send_to_char( buf, mount );
    }
    return;
}

int xp_compute( CHAR_DATA *gch, CHAR_DATA *victim )
{
    double xp;

    if (victim->level < 100)
        xp = 300 - URANGE(-5, 3 - (victim->level*5/3), 6 ) * 50;
    else
        xp = 300 - URANGE(-10, 3 - (victim->level*5/3), 6 ) * 50;

    /*
     * Adjust for popularity of target:
     *   -1/8 for each target over  'par' (down to -100%)
     *   +1/8 for each target under 'par' (  up to + 25%)
     */
    xp -= xp * number_range( -2, 2 ) / 8;
    xp  = number_range( xp * 3 / 4, xp * 5 / 4 );
    xp  = UMAX( 0, xp );
    xp  = (xp * (victim->level) * 0.60);
    xp  = xp / 2;                                           /* Put in cause players compaling to much exp :P */
    if (nodamcap) xp = xp / 2;
    if (!IS_NPC(gch))
    {
        gch->pcdata->score[SCORE_TOTAL_LEVEL] += victim->level;
        if (victim->level > gch->pcdata->score[SCORE_HIGH_LEVEL])
            gch->pcdata->score[SCORE_HIGH_LEVEL] += victim->level;
        gch->pcdata->score[SCORE_TOTAL_XP] += xp;
        if (xp > gch->pcdata->score[SCORE_HIGH_XP])
            gch->pcdata->score[SCORE_HIGH_XP] += xp;
    }
    if (!IS_NPC(gch) && xp > 499 && gch->pcdata->disc_points != 999 && gch->pcdata->disc_research != -1)
    {
        if (!IS_SET(gch->act, PLR_BRIEF4)) stc("#RYou gained a discipline point.#n\n\r", gch);
        gain_disc_points(gch, 1);
        if (!IS_SET(gch->act, PLR_BRIEF4)) if (victim->level > 200 && gch->pcdata->disc_points != 999)
        {
            stc("#RYou gained a discipline point.#n\n\r", gch);
            gain_disc_points(gch, 1);
        }
        if (!IS_SET(gch->act, PLR_BRIEF4)) if (victim->level > 400 && gch->pcdata->disc_points != 999)
        {
            stc("#RYou gained a discipline point.#n\n\r", gch);
            gain_disc_points(gch, 1);
        }
        xp -= 500;
    }
    if (xp < 0)
        xp = 0;
    if (xp > 0 && xp < ccenter[CCENTER_MIN_EXP])
        xp = number_range(ccenter[CCENTER_MIN_EXP] * 0.9, ccenter[CCENTER_MIN_EXP] * 1.1);
    if (IS_NPC(victim) && (IS_SET(victim->act, ACT_NOEXP)))
        return 0;
    /* if (IS_NPC(victim))
     {
         if (victim->level < 5000)
         {
             xp = victim->level / 2 * xp;
         }
         if (victim->level >= 5000 && victim->level < 10000)
         {
             xp = victim->level / 15 * xp;
         }
         if (victim->level >= 10000 && victim->level < 15000)
         {
             xp = victim->level / 100 * xp;
         }
         if (victim->level >= 15000 && victim->level < 20000)
         {
             xp = victim->level / 200 * xp;
         }
         if (victim->level >= 15000 && victim->level < 20000)
         {
             xp = victim->level / 400 * xp;
         }
     }
     if (xp >= 2000000000)
     {
         xp = 2000000000;
     }*/
    return (int) xp;
}

int makeint(double qpgain)
{
    return (int)qpgain;
}

void dam_message( CHAR_DATA *ch, CHAR_DATA *victim, int dam, int dt )
{
    static char * const attack_table[] =
    {
        "hit", "slice",  "stab",  "slash", "whip", "claw",
        "blast",  "pound", "crush", "grep", "bite", "pierce", "suck"
    };
    static char * const attack_table2[] =
    {
        "hit", "slice",  "stab",  "slash", "whip", "claw",
        "blast",  "pound", "crush", "grep", "bite", "pierce", "suck"
    };
    char buf1[512], buf2[512], buf3[512];
    const char *vs;
    const char *vp;
    const char *attack;
    const char *attack2;
    int damp;
    int bodyloc;
    bool critical = FALSE;
    char punct;

    if (!IS_NPC(ch) && IS_SET(ch->newbits,NEW_REND)&& dam>0 && number_range(1,7) == 5) critical=TRUE;
    if ( dam ==   0 ) { vs = " misses";           vp = " misses";             }
    else if ( dam <=  10 ) { vs = " tickles";           vp = " tickles ";           }
    else if ( dam <= 20 ) { vs = " maims";          vp = " maims";              }
    else if ( dam <= 30 ) { vs = " scratchs";           vp = " scratches";          }
    else if ( dam <= 40 ) { vs = " nicks";          vp = " nicks";              }
    else if ( dam <= 50 ) { vs = " grazes";         vp = " grazes";             }
    else if ( dam <= 60) { vs = " strikes";         vp = " strikes";            }
    else if ( dam <= 70 ) { vs = " injures";            vp = " injures";            }
    else if ( dam <= 80 ) { vs = " wounds";         vp = " wounds";             }
    else if ( dam <= 90 ) { vs = " mauls";          vp = " mauls";              }
    else if ( dam <= 100 ) { vs = " pummels";           vp = " pummels";            }
    else if ( dam <= 120 ) { vs = " decimates";         vp = " decimates";          }
    else if ( dam <= 140 ) { vs = " devastates";            vp = " devastes";           }
    else if ( dam <= 160 ) { vs = " nicks";         vp = " nicks";              }
    else if ( dam <= 180 ) { vs = " shreds";            vp = " shreds";             }
    else if ( dam <= 220 ) { vs = " smashs";            vp = " smashs";             }
    else if ( dam <= 240 ) { vs = " mutilates";         vp = " mutilates";          }
    else if ( dam <= 260 ) { vs = " eliminates";            vp = " eliminates";         }
    else if ( dam <= 280 ) { vs = " slams";         vp = " slams";              }
    else if ( dam <= 300 ) { vs = " disembowels";       vp = " disembowels";            }
    else if ( dam <= 320 ) { vs = " eviscerates";       vp = " eviscerates";            }
    else if ( dam <= 340 ) { vs = " cripples";          vp = " cripples";           }
    else if ( dam <= 360 ) { vs = " massacres";         vp = " massacres";          }
    else if ( dam <= 380 ) { vs = " extrudinites";      vp = " extrudinites";           }
    else if ( dam <= 400 ) { vs = " implodes";          vp = " implodes";           }
    else if ( dam <= 450 ) { vs = " levels";            vp = " levels";             }
    else if ( dam <= 500 ) { vs = " superimplodes";     vp = " superimplodes";          }
    else if ( dam <= 550 ) { vs = " #yDeMoLiShEs#n";                            vp = " #yDeMoLiShEs#n";         }
    else if ( dam <= 600 ) { vs = " #LDiSmEmBeRs#n";                            vp = " #LDiSmEmBeRs#n";         }
    else if ( dam <= 700 ) { vs = " #CDeStRoYs#n";                          vp = " #CDeStRoYs#n";           }
    else if ( dam <= 750 ) { vs = " #RAnNiHiLaTeS#n";                           vp = " #RAnNiHiLaTeS#n";            }
    else if ( dam <= 800 ) { vs = " #0BrUtaLiZeS#n";                            vp = " #0BrUtaLiZeS#n";         }
    else if ( dam <= 850 ) { vs = " #oSlAuGhTeRs#n";                            vp = " #oSlAuGhTeRs#n";         }
    else if ( dam <= 900 ) { vs = " #y#bFOSSILIZES#n";                          vp = " #y#bFOSSILIZES#n";           }
    else if ( dam <= 950 ) { vs = " #L#bLACERATES#n";                           vp = " #L#bLACERATES#n";            }
    else if ( dam <= 1000 ) { vs = " #C#bPULVERIZES#n";                         vp = " #C#bPULVERIZES#n";           }
    else if ( dam <= 1500 ) { vs = " #R#bVAPORIZES#n";                          vp = " #R#bVAPORIZES#n";            }
    else if ( dam <= 2000 ) { vs = " #0#bDISINTEGRATES#n";                      vp = " #0#bDISINTEGRATES#n";            }
    else if ( dam <= 2500 ) { vs = " #o*#0#bERADICATES#n#o*#n";                     vp = " #o*#0#bERADICATES#n#o*#n";       }
    else if ( dam <= 3000 ) { vs = " #y**#R#bULTRAIMPLODES#n#y**#n";                    vp = " #y**#R#bULTRAIMPLODES#n#y**#n";      }
    else if ( dam <= 3500 ) { vs = " #L***#C#bCRUCIFIES#n#L***#n";                  vp = " #L***#C#bCRUCIFIES#n#L***#n";        }
    else if ( dam <= 4000 ) { vs = " #C****#L#bRUPTURES#n#C****#n";                 vp = " #C****#L#bRUPTURES#n#C****#n";       }
    else if ( dam <= 4500 ) { vs = " #R*****#y#bINCINERATES#n#R*****#n";                vp = " #R*****#y#bINCINERATES#n#R*****#n";  }
    else if ( dam <= 5000 ) { vs = " #0******#C#bSHATTERS#n#0******#n";             vp = " #0******#C#bSHATTERS#n#0******#n";       }
    else if ( dam <= 10000 ) { vs = " #R(*)*(*)*(*)#y#bFRIES#n#R(*)*(*)*(*)#n";         vp = " #R(*)*(*)*(*)#y#bFRIES#n#R(*)*(*)*(*)#n";        }
    else             { vs = " #R**-#C**-#L**-#R*#0#bEXTERMINATES#n#R*#L-**#C-**#R-**#n";    vp = " #R**-#C**-#L**-#R*#0#bEXTERMINATES#n#R*#L-**#C-**#R-**#n";   }

    /* If victim's hp are less/equal to 'damp', attacker gets a death blow */
    if (IS_NPC(victim)) damp = 0;
    else damp = -10;
    if ((victim->hit - dam > damp ) || (dt >= 0 && dt < MAX_SKILL) ||
        (IS_NPC(victim) && IS_SET(victim->act, ACT_NOPARTS)))
    {
        punct   = (dam <= 250) ? ' ' : ' ';
        if ( dt == TYPE_HIT && !IS_NPC(ch) && !IS_VAMPAFF(ch,VAM_CLAWS) )
        {
            if (dam == 0)
            {
                if (!IS_SET(ch->act, PLR_BRIEF6))
                {
                    sprintf( buf1, "$n%s $N%c#r[#l**#R%d#l**#r]#n",  vp, punct, dam );
                }
                sprintf( buf2, "You%s $N%c#r[#l**#R%d#l**#r]#n", vs, punct, dam );
                sprintf( buf3, "$n%s you%c", vp, punct );
            }
            else
            {
                if (!IS_SET(ch->act, PLR_BRIEF6))
                {
                    sprintf( buf1, "$n hit%s $N%c#r[#l**#R%d#l**#r]#n",  vp, punct, dam );
                }
                sprintf( buf2, "Your hit%s $N%c#r[#l**#R%d#l**#r]#n", vs, punct, dam );
                sprintf( buf3, "$n hit%s you%c#r[#l**#R%d#l**#r]#n", vp, punct, dam );
                critical = TRUE;
            }
        }
        else if ( dt == TYPE_HIT && !IS_NPC(ch) && IS_SET(ch->pcdata->classtwo[ELF_REDSTONE], ELFSTONE_FLAMERING))
        {
            if (dam >= 1)
            {
                if (!IS_CLASS(ch, CLASS_ELF))
                {
                    if (!IS_SET(ch->act, PLR_BRIEF6))
                    {
                        sprintf( buf1, "$n's flamering hit $N%s%c#r[#l**#R%d#l**#r]#n", vp, punct, dam );
                    }
                    sprintf( buf2, "Your flamering hit $N%s%c#r[#l**#R%d#l**#r]#n", vs, punct, dam );
                    sprintf( buf3, "$n's flamering hit you%s%c#r[#l**#R%d#l**#r]#n", vp, punct, dam );
                }
                else
                {
                    if (!IS_SET(ch->act, PLR_BRIEF6))
                    {
                        sprintf( buf1, "$n's flamering hit%s $N%c#r[#l**#R%d#l**#r]#n", vp, punct, dam );
                    }
                    sprintf( buf2, "Your flamering hit%s $N%c#r[#l**#R%d#l**#r]#n", vs, punct, dam );
                    sprintf( buf3, "$n's flamering hit%s you%c#r[#l**#R%d#l**#r]#n", vp, punct, dam );
                }
            }
            else
            {
                if (!IS_SET(ch->act, PLR_BRIEF6))
                {
                    sprintf( buf1, "$n's hit%s $N%c#r[#l**#R%d#l**#r]#n",  vp, punct, dam );
                }
                sprintf( buf2, "Your hit%s $N%c#r[#l**#R%d#l**#r]#n", vs, punct, dam );
                sprintf( buf3, "$n hit%s you%c#r[#l**#R%d#l**#r]#n", vp, punct, dam );
                critical = TRUE;
            }
        }
        else if ( dt == TYPE_HIT && !IS_NPC(ch) && IS_SET(ch->newbits, NEW_MONKFLAME))
        {
            if (dam >= 1)
            {
                if (!IS_CLASS(ch, CLASS_WEREWOLF))
                {
                    if (!IS_SET(ch->act, PLR_BRIEF6))
                    {
                        sprintf( buf1, "$n's flaming hands hit $N%s%c#r[#l**#R%d#l**#r]#n", vp, punct, dam );
                    }
                    sprintf( buf2, "Your flaming hands hit $N%s%c#r[#l**#R%d#l**#r]#n", vs, punct, dam );
                    sprintf( buf3, "$n's flaming hands hit you%s%c#r[#l**#R%d#l**#r]#n", vp, punct, dam );
                }
                else
                {
                    if (!IS_SET(ch->act, PLR_BRIEF6))
                    {
                        sprintf( buf1, "$n's flaming claws hit%s $N%c#r[#l**#R%d#l**#r]#n", vp, punct, dam );
                    }
                    sprintf( buf2, "Your flaming claws hit%s $N%c#r[#l**#R%d#l**#r]#n", vs, punct, dam );
                    sprintf( buf3, "$n's flaming claws hit%s you%c#r[#l**#R%d#l**#r]#n", vp, punct, dam );
                }
            }
            else
            {
                if (!IS_SET(ch->act, PLR_BRIEF6))
                {
                    sprintf( buf1, "$n's hit%s $N%c#r[#l**#R%d#l**#r]#n",  vp, punct, dam );
                }
                sprintf( buf2, "Your hit%s $N%c#r[#l**#R%d#l**#r]#n", vs, punct, dam );
                sprintf( buf3, "$n hit%s you%c#r[#l**#R%d#l**#r]#n", vp, punct, dam );
                critical = TRUE;
            }
        }
        else
        {
            if ( dt == TYPE_HIT && !IS_NPC(ch) && (IS_VAMPAFF(ch,VAM_CLAWS)))
            {
                attack  = attack_table[dt - TYPE_HIT + 5];
                attack2 = attack_table2[dt - TYPE_HIT + 5];
            }
            else if ( dt >= 0 && dt < MAX_SKILL )
            {
                attack = skill_table[dt].noun_damage;
                attack2 = skill_table[dt].noun_damage;
            }
            else if (dt >= TYPE_HIT && dt < TYPE_HIT + sizeof(attack_table)/sizeof(attack_table[0]))
            {
                attack  = attack_table[dt - TYPE_HIT];
                attack2 = attack_table2[dt - TYPE_HIT];
            }
            else
            {
                dt  = TYPE_HIT;
                attack  = attack_table[0];
                attack2 = attack_table2[0];
            }
            if (dam == 0)
            {
                if (!IS_SET(ch->act, PLR_BRIEF6))
                {
                    sprintf( buf1, "$n's %s%s $N%c#r[#l**#R%d#l**#r]#n",  attack, vp, punct, dam );
                }
                sprintf( buf2, "Your %s%s $N%c#r[#l**#R%d#l**#r]#n",  attack, vp, punct, dam );
                sprintf( buf3, "$n's %s%s you%c#r[#l**#R%d#l**#r]#n", attack, vp, punct, dam );
            }
            else if (IS_SET(ch->newbits, NEW_MONKFLAME) && (dam == 0))
            {
                if (!IS_SET(ch->act, PLR_BRIEF6))
                {
                    sprintf( buf1, "$n's flaming %s%s $N%c#r[#l**#R%d#l**#r]#n", attack2, vp, punct, dam );
                }
                sprintf( buf2, "Your flaming %s%s $N%c#r[#l**#R%d#l**#r]#n",  attack, vp, punct, dam );
                sprintf( buf3, "$n's flaming %s%s you%c#r[#l**#R%d#l**#r]#n", attack2, vp, punct, dam );
            }
            else
            {
                if ( dt >= 0 && dt < MAX_SKILL )
                {
                    if (!IS_SET(ch->act, PLR_BRIEF6))
                    {
                        sprintf( buf1, "$n's %s strike%s $N%c#r[#l**#R%d#l**#r]#n",  attack2, vp, punct, dam );
                    }
                    sprintf( buf2, "Your %s strike%s $N%c#r[#l**#R%d#l**#r]#n",  attack, vp, punct, dam );
                    sprintf( buf3, "$n's %s strike%s you%c#r[#l**#R%d#l**#r]#n", attack2, vp, punct, dam );
                }
                else if (IS_SET(ch->newbits, NEW_MONKFLAME))
                {
                    if (!IS_SET(ch->act, PLR_BRIEF6))
                    {
                        sprintf( buf1, "$n's flaming %s hits $N%s%c#r[#l**#R%d#l**#r]#n", attack2, vp, punct, dam);
                    }
                    sprintf( buf2, "Your flaming %s hits $N%s%c#r[#l**#R%d#l**#r]#n", attack, vp, punct, dam);
                    sprintf( buf3, "$n's flaming %s hits you%s%c#r[#l**#R%d#l**#r]#n", attack2, vp, punct, dam);
                }
                else
                {
                    if (!IS_SET(ch->act, PLR_BRIEF6))
                    {
                        sprintf( buf1, "$n's %s%s $N%c#r[#l**#R%d#l**#r]#n",  attack2, vp, punct, dam );
                    }
                    sprintf( buf2, "Your %s%s $N%c#r[#l**#R%d#l**#r]#n",  attack, vp, punct, dam );
                    sprintf( buf3, "$n's %s%s you%c#r[#l**#R%d#l**#r]#n", attack2, vp, punct, dam );
                    critical = TRUE;
                }
            }
        }
        if (!IS_SET(victim->act, PLR_BRIEF6) && !IS_SET(ch->act, PLR_BRIEF6))
            act( buf1, ch, NULL, victim, TO_NOTVICT );

        if (IS_SET(ch->act, PLR_BRIEF5) && MultiHit > 0)
        {
            ch->pcdata->briefhit++;
            ch->pcdata->briefdam += dam;
        }
        else if (!(IS_SET(ch->act, PLR_BRIEF2) &&  (dam == 0 || dt == skill_lookup("lightning bolt") ||
            dt == skill_lookup("acid blast") || dt == skill_lookup("chill touch") || dt == skill_lookup("fireball"))))
            act( buf2, ch, NULL, victim, TO_CHAR );
        if (IS_SET(victim->act, PLR_BRIEF5) && MultiHit > 0)
        {
            victim->pcdata->briefthit++;
            victim->pcdata->brieftdam += dam;
        }
        else if (!(IS_SET(victim->act, PLR_BRIEF2) && (dam == 0 || dt == skill_lookup("lightning bolt") ||
            dt == skill_lookup("acid blast") || dt == skill_lookup("chill touch") || dt == skill_lookup("fireball"))))
            act( buf3, ch, NULL, victim, TO_VICT );
        if (critical) critical_hit(ch,victim,dt,dam);
        return;
    }
    if ( dt == TYPE_HIT && !IS_NPC( ch ) && !IS_VAMPAFF(ch,VAM_CLAWS) &&!IS_VAMPAFF(ch,VAM_FANGS))
    {
        damp=number_range(1,5);
        if ( damp == 1 )
        {
            act("You ram your fingers into $N's eye sockets and rip $S face off.", ch, NULL, victim, TO_CHAR);
            act("$n rams $s fingers into $N's eye sockets and rips $S face off.", ch, NULL, victim, TO_NOTVICT);
            act("$n rams $s fingers into your eye sockets and rips your face off.", ch, NULL, victim, TO_VICT);
            make_part(victim,"face");
        }
        else if ( damp == 2 )
        {
            act("You grab $N by the throat and tear $S windpipe out.", ch, NULL, victim, TO_CHAR);
            act("$n grabs $N by the throat and tears $S windpipe out.", ch, NULL, victim, TO_NOTVICT);
            act("$n grabs you by the throat and tears your windpipe out.", ch, NULL, victim, TO_VICT);
            make_part(victim,"windpipe");
        }
        else if ( damp == 3 )
        {
            act("You punch your fist through $N's stomach and rip out $S entrails.", ch, NULL, victim, TO_CHAR);
            act("$n punches $s fist through $N's stomach and rips out $S entrails.", ch, NULL, victim, TO_NOTVICT);
            act("$n punches $s fist through your stomach and rips out your entrails.", ch, NULL, victim, TO_VICT);
            make_part(victim,"entrails");
        }
        else if ( damp == 4 )
        {
            if (!IS_BODY(victim,BROKEN_SPINE))
                SET_BIT(victim->loc_hp[1],BROKEN_SPINE);
            act("You hoist $N above your head and slam $M down upon your knee.\n\rThere is a loud cracking sound as $N's spine snaps.", ch, NULL, victim, TO_CHAR);
            act("$n hoists $N above $s head and slams $M down upon $s knee.\n\rThere is a loud cracking sound as $N's spine snaps.", ch, NULL, victim, TO_NOTVICT);
            act("$n hoists you above $s head and slams you down upon $s knee.\n\rThere is a loud cracking sound as your spine snaps.", ch, NULL, victim, TO_VICT);
        }
        else if ( damp == 5 )
        {
            act("You lock your arm around $N's head, and give it a vicious twist.", ch, NULL, victim, TO_CHAR);
            act("$n locks $s arm around $N's head, and gives it a vicious twist.", ch, NULL, victim, TO_NOTVICT);
            act("$n locks $s arm around your head, and gives it a vicious twist.", ch, NULL, victim, TO_VICT);
            if (!IS_BODY(victim,BROKEN_NECK))
            {
                act("There is a loud snapping noise as your neck breaks.", victim, NULL, NULL, TO_CHAR);
                act("There is a loud snapping noise as $n's neck breaks.", victim, NULL, NULL, TO_ROOM);
                SET_BIT(victim->loc_hp[1],BROKEN_NECK);
            }
        }
        return;
    }
    if (dt >= 0 && dt < MAX_SKILL) attack = skill_table[dt].noun_damage;
    else if ( dt >= TYPE_HIT && dt < TYPE_HIT + sizeof(attack_table)/sizeof(attack_table[0]))
        attack = attack_table[dt - TYPE_HIT];
    else
    {
        dt  = TYPE_HIT;
        attack  = attack_table[0];
    }
    if ( attack == "slash" || attack == "slice" )
    {
        damp=number_range(1,8);
        if ( damp == 1 )
        {
            act("You swing your blade in a low arc, rupturing $N's abdominal cavity.\n\r$S entrails spray out over a wide area.", ch, NULL, victim, TO_CHAR);
            act("$n swings $s blade in a low arc, rupturing $N's abdominal cavity.\n\r$S entrails spray out over a wide area.", ch, NULL, victim, TO_NOTVICT);
            act("$n swings $s blade in a low arc, rupturing your abdominal cavity.\n\rYour entrails spray out over a wide area.", ch, NULL, victim, TO_VICT);
            make_part(victim,"entrails");
        }
        else if ( damp == 2 )
        {
            act("You thrust your blade into $N's mouth and twist it viciously.\n\rThe end of your blade bursts through the back of $S head.", ch, NULL, victim, TO_CHAR);
            act("$n thrusts $s blade into $N's mouth and twists it viciously.\n\rThe end of the blade bursts through the back of $N's head.", ch, NULL, victim, TO_NOTVICT);
            act("$n thrusts $s blade into your mouth and twists it viciously.\n\rYou feel the end of the blade burst through the back of your head.", ch, NULL, victim, TO_VICT);
        }
        else if ( damp == 3 )
        {
            if (!IS_BODY(victim,CUT_THROAT)) SET_BIT(victim->loc_hp[1],CUT_THROAT);
            if (!IS_BLEEDING(victim,BLEEDING_THROAT)) SET_BIT(victim->loc_hp[6],BLEEDING_THROAT);
            act("Your blow slices open $N's carotid artery, spraying blood everywhere.", ch, NULL, victim, TO_CHAR);
            act("$n's blow slices open $N's carotid artery, spraying blood everywhere.", ch, NULL, victim, TO_NOTVICT);
            act("$n's blow slices open your carotid artery, spraying blood everywhere.", ch, NULL, victim, TO_VICT);
            make_part(victim,"blood");
        }
        else if ( damp == 4 )
        {
            if (!IS_BODY(victim,CUT_THROAT)) SET_BIT(victim->loc_hp[1],CUT_THROAT);
            if (!IS_BLEEDING(victim,BLEEDING_THROAT)) SET_BIT(victim->loc_hp[6],BLEEDING_THROAT);
            act("You swing your blade across $N's throat, showering the area with blood.", ch, NULL, victim, TO_CHAR);
            act("$n swings $s blade across $N's throat, showering the area with blood.", ch, NULL, victim, TO_NOTVICT);
            act("$n swings $s blade across your throat, showering the area with blood.", ch, NULL, victim, TO_VICT);
            make_part(victim,"blood");
        }
        else if ( damp == 5 )
        {
            if (!IS_HEAD(victim,BROKEN_SKULL))
            {
                act("You swing your blade down upon $N's head, splitting it open.\n\r$N's brains pour out of $S forehead.", ch, NULL, victim, TO_CHAR);
                act("$n swings $s blade down upon $N's head, splitting it open.\n\r$N's brains pour out of $S forehead.", ch, NULL, victim, TO_NOTVICT);
                act("$n swings $s blade down upon your head, splitting it open.\n\rYour brains pour out of your forehead.", ch, NULL, victim, TO_VICT);
                make_part(victim,"brain");
                SET_BIT(victim->loc_hp[0],BROKEN_SKULL);
            }
            else
            {
                act("You plunge your blade deep into $N's chest.", ch, NULL, victim, TO_CHAR);
                act("$n plunges $s blade deep into $N's chest.", ch, NULL, victim, TO_NOTVICT);
                act("$n plunges $s blade deep into your chest.", ch, NULL, victim, TO_VICT);
            }
        }
        else if ( damp == 6 )
        {
            act("You swing your blade between $N's legs, nearly splitting $M in half.", ch, NULL, victim, TO_CHAR);
            act("$n swings $s blade between $N's legs, nearly splitting $M in half.", ch, NULL, victim, TO_NOTVICT);
            act("$n swings $s blade between your legs, nearly splitting you in half.", ch, NULL, victim, TO_VICT);
        }
        else if ( damp == 7 )
        {
            if (!IS_ARM_L(victim,LOST_ARM))
            {
                act("You swing your blade in a wide arc, slicing off $N's arm.", ch, NULL, victim, TO_CHAR);
                act("$n swings $s blade in a wide arc, slicing off $N's arm.", ch, NULL, victim, TO_NOTVICT);
                act("$n swings $s blade in a wide arc, slicing off your arm.", ch, NULL, victim, TO_VICT);
                make_part(victim,"arm");
                SET_BIT(victim->loc_hp[2],LOST_ARM);
                if (!IS_BLEEDING(victim,BLEEDING_ARM_L)) SET_BIT(victim->loc_hp[6],BLEEDING_ARM_L);
                if (IS_BLEEDING(victim,BLEEDING_HAND_L)) REMOVE_BIT(victim->loc_hp[6],BLEEDING_HAND_L);
            }
            else if (!IS_ARM_R(victim,LOST_ARM))
            {
                act("You swing your blade in a wide arc, slicing off $N's arm.", ch, NULL, victim, TO_CHAR);
                act("$n swings $s blade in a wide arc, slicing off $N's arm.", ch, NULL, victim, TO_NOTVICT);
                act("$n swings $s blade in a wide arc, slicing off your arm.", ch, NULL, victim, TO_VICT);
                make_part(victim,"arm");
                SET_BIT(victim->loc_hp[3],LOST_ARM);
                if (!IS_BLEEDING(victim,BLEEDING_ARM_R)) SET_BIT(victim->loc_hp[6],BLEEDING_ARM_R);
                if (IS_BLEEDING(victim,BLEEDING_HAND_R)) REMOVE_BIT(victim->loc_hp[6],BLEEDING_HAND_R);
            }
            else
            {
                act("You plunge your blade deep into $N's chest.", ch, NULL, victim, TO_CHAR);
                act("$n plunges $s blade deep into $N's chest.", ch, NULL, victim, TO_NOTVICT);
                act("$n plunges $s blade deep into your chest.", ch, NULL, victim, TO_VICT);
            }
        }
        else if ( damp == 8 )
        {
            if (!IS_LEG_L(victim,LOST_LEG))
            {
                act("You swing your blade in a low arc, slicing off $N's leg at the hip.", ch, NULL, victim, TO_CHAR);
                act("$n swings $s blade in a low arc, slicing off $N's leg at the hip.", ch, NULL, victim, TO_NOTVICT);
                act("$n swings $s blade in a wide arc, slicing off your leg at the hip.", ch, NULL, victim, TO_VICT);
                make_part(victim,"leg");
                SET_BIT(victim->loc_hp[4],LOST_LEG);
                if (!IS_BLEEDING(victim,BLEEDING_LEG_L)) SET_BIT(victim->loc_hp[6],BLEEDING_LEG_L);
                if (IS_BLEEDING(victim,BLEEDING_FOOT_L)) REMOVE_BIT(victim->loc_hp[6],BLEEDING_FOOT_L);
            }
            else if (!IS_LEG_R(victim,LOST_LEG))
            {
                act("You swing your blade in a low arc, slicing off $N's leg at the hip.", ch, NULL, victim, TO_CHAR);
                act("$n swings $s blade in a low arc, slicing off $N's leg at the hip.", ch, NULL, victim, TO_NOTVICT);
                act("$n swings $s blade in a wide arc, slicing off your leg at the hip.", ch, NULL, victim, TO_VICT);
                make_part(victim,"leg");
                SET_BIT(victim->loc_hp[5],LOST_LEG);
                if (!IS_BLEEDING(victim,BLEEDING_LEG_R)) SET_BIT(victim->loc_hp[6],BLEEDING_LEG_R);
                if (IS_BLEEDING(victim,BLEEDING_FOOT_R)) REMOVE_BIT(victim->loc_hp[6],BLEEDING_FOOT_R);
            }
            else
            {
                act("You plunge your blade deep into $N's chest.", ch, NULL, victim, TO_CHAR);
                act("$n plunges $s blade deep into $N's chest.", ch, NULL, victim, TO_NOTVICT);
                act("$n plunges $s blade deep into your chest.", ch, NULL, victim, TO_VICT);
            }
        }
    }
    else if ( attack == "stab" || attack == "pierce" )
    {
        damp=number_range(1,5);
        if ( damp == 1 )
        {
            act("You defty invert your weapon and plunge it point first into $N's chest.\n\rA shower of blood sprays from the wound, showering the area.", ch, NULL, victim, TO_CHAR);
            act("$n defty inverts $s weapon and plunge it point first into $N's chest.\n\rA shower of blood sprays from the wound, showering the area.", ch, NULL, victim, TO_NOTVICT);
            act("$n defty inverts $s weapon and plunge it point first into your chest.\n\rA shower of blood sprays from the wound, showering the area.", ch, NULL, victim, TO_VICT);
            make_part(victim,"blood");
        }
        else if ( damp == 2 )
        {
            act("You thrust your blade into $N's mouth and twist it viciously.\n\rThe end of your blade bursts through the back of $S head.", ch, NULL, victim, TO_CHAR);
            act("$n thrusts $s blade into $N's mouth and twists it viciously.\n\rThe end of the blade bursts through the back of $N's head.", ch, NULL, victim, TO_NOTVICT);
            act("$n thrusts $s blade into your mouth and twists it viciously.\n\rYou feel the end of the blade burst through the back of your head.", ch, NULL, victim, TO_VICT);
        }
        else if ( damp == 3 )
        {
            act("You thrust your weapon up under $N's jaw and through $S head.", ch, NULL, victim, TO_CHAR);
            act("$n thrusts $s weapon up under $N's jaw and through $S head.", ch, NULL, victim, TO_NOTVICT);
            act("$n thrusts $s weapon up under your jaw and through your head.", ch, NULL, victim, TO_VICT);
        }
        else if ( damp == 4 )
        {
            act("You ram your weapon through $N's body, pinning $M to the ground.", ch, NULL, victim, TO_CHAR);
            act("$n rams $s weapon through $N's body, pinning $M to the ground.", ch, NULL, victim, TO_NOTVICT);
            act("$n rams $s weapon through your body, pinning you to the ground.", ch, NULL, victim, TO_VICT);
        }
        else if ( damp == 5 )
        {
            act("You stab your weapon into $N's eye and out the back of $S head.", ch, NULL, victim, TO_CHAR);
            act("$n stabs $s weapon into $N's eye and out the back of $S head.", ch, NULL, victim, TO_NOTVICT);
            act("$n stabs $s weapon into your eye and out the back of your head.", ch, NULL, victim, TO_VICT);
            if (!IS_HEAD(victim,LOST_EYE_L) && number_percent() < 50) SET_BIT(victim->loc_hp[0],LOST_EYE_L);
            else if (!IS_HEAD(victim,LOST_EYE_R)) SET_BIT(victim->loc_hp[0],LOST_EYE_R);
            else if (!IS_HEAD(victim,LOST_EYE_L)) SET_BIT(victim->loc_hp[0],LOST_EYE_L);
        }
    }
    else if ( attack == "blast" || attack == "pound" || attack == "crush" )
    {
        damp=number_range(1,3);
        bodyloc = 0;
        if ( damp == 1)
        {
            act("Your blow smashes through $N's chest, caving in half $S ribcage.", ch, NULL, victim, TO_CHAR);
            act("$n's blow smashes through $N's chest, caving in half $S ribcage.", ch, NULL, victim, TO_NOTVICT);
            act("$n's blow smashes through your chest, caving in half your ribcage.", ch, NULL, victim, TO_VICT);
            if (IS_BODY(victim,BROKEN_RIBS_1 )) {bodyloc += 1; REMOVE_BIT(victim->loc_hp[1],BROKEN_RIBS_1);}
            if (IS_BODY(victim,BROKEN_RIBS_2 )) {bodyloc += 2; REMOVE_BIT(victim->loc_hp[1],BROKEN_RIBS_2);}
            if (IS_BODY(victim,BROKEN_RIBS_4 )) {bodyloc += 4; REMOVE_BIT(victim->loc_hp[1],BROKEN_RIBS_4);}
            if (IS_BODY(victim,BROKEN_RIBS_8 )) {bodyloc += 8; REMOVE_BIT(victim->loc_hp[1],BROKEN_RIBS_8);}
            if (IS_BODY(victim,BROKEN_RIBS_16)) {bodyloc += 16; REMOVE_BIT(victim->loc_hp[1],BROKEN_RIBS_16);}
            bodyloc += number_range(1,3);
            if (bodyloc > 24) bodyloc = 24;
            if (bodyloc >= 16) {bodyloc -= 16; SET_BIT(victim->loc_hp[1],BROKEN_RIBS_16);}
            if (bodyloc >= 8) {bodyloc -= 8; SET_BIT(victim->loc_hp[1],BROKEN_RIBS_8);}
            if (bodyloc >= 4) {bodyloc -= 4; SET_BIT(victim->loc_hp[1],BROKEN_RIBS_4);}
            if (bodyloc >= 2) {bodyloc -= 2; SET_BIT(victim->loc_hp[1],BROKEN_RIBS_2);}
            if (bodyloc >= 1) {bodyloc -= 1; SET_BIT(victim->loc_hp[1],BROKEN_RIBS_1);}
        }
        else if ( damp == 2)
        {
            act("Your blow smashes $N's spine, shattering it in several places.", ch, NULL, victim, TO_CHAR);
            act("$n's blow smashes $N's spine, shattering it in several places.", ch, NULL, victim, TO_NOTVICT);
            act("$n's blow smashes your spine, shattering it in several places.", ch, NULL, victim, TO_VICT);
            if (!IS_BODY(victim,BROKEN_SPINE)) SET_BIT(victim->loc_hp[1],BROKEN_SPINE);
        }
        else if ( damp == 3)
        {
            if (!IS_HEAD(victim,BROKEN_SKULL))
            {
                act("You swing your weapon down upon $N's head.\n\r$N's head cracks open like an overripe melon, leaking out brains.", ch, NULL, victim, TO_CHAR);
                act("$n swings $s weapon down upon $N's head.\n\r$N's head cracks open like an overripe melon, covering you with brains.", ch, NULL, victim, TO_NOTVICT);
                act("$n swings $s weapon down upon your head.\n\rYour head cracks open like an overripe melon, spilling your brains everywhere.", ch, NULL, victim, TO_VICT);
                make_part(victim,"brain");
                SET_BIT(victim->loc_hp[0],BROKEN_SKULL);
            }
            else
            {
                act("You hammer your weapon into $N's side, crushing bone.", ch, NULL, victim, TO_CHAR);
                act("$n hammers $s weapon into $N's side, crushing bone.", ch, NULL, victim, TO_NOTVICT);
                act("$n hammers $s weapon into your side, crushing bone.", ch, NULL, victim, TO_VICT);
            }
        }
    }
    else if ( !IS_NPC( ch ) && (attack == "bite" ||IS_VAMPAFF(ch,VAM_FANGS)))
    {
        act("You sink your teeth into $N's throat and tear out $S jugular vein.\n\rYou wipe the blood from your chin with one hand.", ch, NULL, victim, TO_CHAR);
        act("$n sink $s teeth into $N's throat and tears out $S jugular vein.\n\r$n wipes the blood from $s chin with one hand.", ch, NULL, victim, TO_NOTVICT);
        act("$n sink $s teeth into your throat and tears out your jugular vein.\n\r$n wipes the blood from $s chin with one hand.", ch, NULL, victim, TO_VICT);
        make_part(victim,"blood");
        if (!IS_BODY(victim,CUT_THROAT)) SET_BIT(victim->loc_hp[1],CUT_THROAT);
        if (!IS_BLEEDING(victim,BLEEDING_THROAT)) SET_BIT(victim->loc_hp[6],BLEEDING_THROAT);
    }
    else if ( !IS_NPC(ch) && (attack == "claw" || IS_VAMPAFF(ch,VAM_CLAWS)))
    {
        damp=number_range(1,2);
        if ( damp == 1 )
        {
            act("You tear out $N's throat, showering the area with blood.", ch, NULL, victim, TO_CHAR);
            act("$n tears out $N's throat, showering the area with blood.", ch, NULL, victim, TO_NOTVICT);
            act("$n tears out your throat, showering the area with blood.", ch, NULL, victim, TO_VICT);
            make_part(victim,"blood");
            if (!IS_BODY(victim,CUT_THROAT)) SET_BIT(victim->loc_hp[1],CUT_THROAT);
            if (!IS_BLEEDING(victim,BLEEDING_THROAT)) SET_BIT(victim->loc_hp[6],BLEEDING_THROAT);
        }
        if ( damp == 2 )
        {
            if (!IS_HEAD(victim,LOST_EYE_L) && number_percent() < 50)
            {
                act("You rip an eyeball from $N's face.", ch, NULL, victim, TO_CHAR);
                act("$n rips an eyeball from $N's face.", ch, NULL, victim, TO_NOTVICT);
                act("$n rips an eyeball from your face.", ch, NULL, victim, TO_VICT);
                make_part(victim,"eyeball");
                SET_BIT(victim->loc_hp[0],LOST_EYE_L);
            }
            else if (!IS_HEAD(victim,LOST_EYE_R))
            {
                act("You rip an eyeball from $N's face.", ch, NULL, victim, TO_CHAR);
                act("$n rips an eyeball from $N's face.", ch, NULL, victim, TO_NOTVICT);
                act("$n rips an eyeball from your face.", ch, NULL, victim, TO_VICT);
                make_part(victim,"eyeball");
                SET_BIT(victim->loc_hp[0],LOST_EYE_R);
            }
            else if (!IS_HEAD(victim,LOST_EYE_L))
            {
                act("You rip an eyeball from $N's face.", ch, NULL, victim, TO_CHAR);
                act("$n rips an eyeball from $N's face.", ch, NULL, victim, TO_NOTVICT);
                act("$n rips an eyeball from your face.", ch, NULL, victim, TO_VICT);
                make_part(victim,"eyeball");
                SET_BIT(victim->loc_hp[0],LOST_EYE_L);
            }
            else
            {
                act("You claw open $N's chest.", ch, NULL, victim, TO_CHAR);
                act("$n claws open $N's chest.", ch, NULL, victim, TO_NOTVICT);
                act("$n claws open $N's chest.", ch, NULL, victim, TO_VICT);
            }
        }
    }
    else if ( attack == "whip" )
    {
        act("You entangle $N around the neck, and squeeze out $S life.", ch, NULL, victim, TO_CHAR);
        act("$n entangles $N around the neck, and squeezes out $S life.", ch, NULL, victim, TO_NOTVICT);
        act("$n entangles you around the neck, and squeezes the life out of you.", ch, NULL, victim, TO_VICT);
        if (!IS_BODY(victim,BROKEN_NECK)) SET_BIT(victim->loc_hp[1],BROKEN_NECK);
    }
    else if ( attack == "suck" || attack == "grep" )
    {
        act("You place your weapon on $N's head and suck out $S brains.", ch, NULL, victim, TO_CHAR);
        act("$n places $s weapon on $N's head and suck out $S brains.", ch, NULL, victim, TO_NOTVICT);
        act("$n places $s weapon on your head and suck out your brains.", ch, NULL, victim, TO_VICT);
    }
    else
    {
        /*bug( "Dam_message: bad dt %d.", dt );*/
    }
    return;
}

/*
 * Disarm a creature.
 * Caller must check for successful attack.
 */
void disarm( CHAR_DATA *ch, CHAR_DATA *victim )
{
    OBJ_DATA *obj;
    char buf [MAX_STRING_LENGTH];

    if (IS_NPC(ch) && victim->level > 2 && number_percent() > 10)
        return;
    if (!IS_IMMUNE(ch, IMM_DISARM))
        return;
    if(((obj = get_eq_char( victim, WEAR_WIELD ) ) == NULL) || obj->item_type != ITEM_WEAPON)
    {
        if(((obj = get_eq_char( victim, WEAR_HOLD ) ) == NULL) || obj->item_type != ITEM_WEAPON)
            return;
    }
    sprintf(buf,"$n disarms you!");
    ADD_COLOUR(ch,buf,WHITE);
    act( buf, ch, NULL, victim, TO_VICT    );
    sprintf(buf,"You disarm $N!");
    ADD_COLOUR(ch,buf,WHITE);
    act( buf,  ch, NULL, victim, TO_CHAR    );
    sprintf(buf,"$n disarms $N!");
    ADD_COLOUR(ch,buf,WHITE);
    act( buf,  ch, NULL, victim, TO_NOTVICT );
    obj_from_char( obj );
    if (IS_SET(obj->extra_flags, ITEM_LOYAL) && ( !IS_NPC(victim) ) )
    {
        act( "$p leaps back into your hand!", victim, obj, NULL, TO_CHAR );
        act( "$p leaps back into $n's hand!",  victim, obj, NULL, TO_ROOM );
        obj_to_char( obj, victim );
        do_wear( victim, obj->name);
    }
    else if (IS_NPC(victim))
        obj_to_char(obj, victim);
    else
        obj_to_room(obj, victim->in_room);
    return;
}

/*
 * Trip a creature.
 * Caller must check for successful attack.
 */
void trip( CHAR_DATA *ch, CHAR_DATA *victim )
{
    char buf [MAX_STRING_LENGTH];

    if (IS_AFFECTED(victim,AFF_FLYING)) return;
    if (IS_NPC(ch) && victim->level > 2 && number_percent() > 5) return;
    if (!IS_NPC(victim))
    {
        if (IS_CLASS(victim, CLASS_VAMPIRE) && IS_VAMPAFF(victim,VAM_FLYING))
            return;
        if (IS_CLASS(victim, CLASS_DEMON) && IS_DEMAFF(victim, DEM_UNFOLDED))
            return;
        if (IS_CLASS(victim, CLASS_ANGEL) && IS_SET(victim->pcdata->powers[ANGEL_POWERS], ANGEL_WINGS))
            return;
        if (IS_CLASS(victim, CLASS_DRAGON) && IS_SET(victim->pcdata->classfour[DRAGON_POWER], DRAG_WINGS))
            return;
        if (IS_CLASS(victim, CLASS_DROW) && IS_SET(victim->pcdata->powers[1], DPOWER_LEVITATION))
            return;
    }
    if ( victim->wait == 0)
    {
        sprintf(buf,"$n trips you and you go down!");
        ADD_COLOUR(ch,buf,WHITE);
        act( buf, ch, NULL, victim, TO_VICT    );
        sprintf(buf,"You trip $N and $E goes down!");
        ADD_COLOUR(ch,buf,WHITE);
        act( buf,  ch, NULL, victim, TO_CHAR    );
        sprintf(buf,"$n trips $N and $E goes down!");
        ADD_COLOUR(ch,buf,WHITE);
        act( buf,  ch, NULL, victim, TO_NOTVICT );
        WAIT_STATE( victim, 2 * PULSE_VIOLENCE );
        victim->position = POS_RESTING;
    }
    return;
}

void do_kill( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;

    one_argument( argument, arg );
    //  if ( IS_NPC(ch) && ch->desc == NULL) return;
    if ( arg[0] == '\0' )
    {
        send_to_char( "Kill whom?\n\r", ch );
        return;
    }
    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
        send_to_char( "They aren't here.\n\r", ch );
        return;
    }
    if ( victim == ch )
    {
        send_to_char( "You cannot kill yourself!\n\r", ch );
        return;
    }
    if ( !IS_NPC(ch) )
        dropinvis(ch);
    if (!IS_NPC(victim) && !IS_NPC(ch))
    {
        if ( ch->pcdata->upgrade_level == 0 && ((get_age(ch) - 17) < 5) )
        {
            send_to_char( "You can't attack a player if your still have less then 5 hours, use commit if you want to kill.\n\r", ch );
            return;
        }
        if ( victim->pcdata->upgrade_level == 0 && ((get_age(victim) - 17) < 5) )
        {
            send_to_char( "You can't attack a Newbie with less then 5 hours.\n\r", ch );
            return;
        }
    }
    if (is_safe( ch, victim ))
    {
        if (victim->fight_timer > 0)
        {
            multi_hit( ch, victim, TYPE_UNDEFINED );
            return;
        }
        else
        {
            send_to_char( "They are safe from attack.\n\r",ch);
            return;
        }
    }
    if (IS_AFFECTED(ch, AFF_CHARM) && ch->master == victim)
    {
        act( "$N is your beloved master.", ch, NULL, victim, TO_CHAR );
        return;
    }
    if ( ch->position == POS_FIGHTING )
    {
        send_to_char( "You do the best you can!\n\r", ch );
        return;
    }
    WAIT_STATE( ch, 8 );
    if(!IS_NPC(ch) && !IS_NPC(victim))
    {
        ch->fight_timer +=3;
        victim->fight_timer +=3;
    }
    if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_WEREWOLF) && number_range(1,3) == 1
        && ch->power[DISC_WERE_BOAR] > 1 && victim->position == POS_STANDING)
    {
        act("You charge into $N, knocking $M from $S feet.",ch,NULL,victim,TO_CHAR);
        act("$n charge into $N, knocking $M from $S feet.",ch,NULL,victim,TO_NOTVICT);
        act("$n charge into you, knocking you from your feet.",ch,NULL,victim,TO_VICT);
        victim->position = POS_STUNNED;
        multi_hit( ch, victim, TYPE_UNDEFINED );
        multi_hit( ch, victim, TYPE_UNDEFINED );
        return;
    }
    /*  if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_ANGEL))
      {
        act("You swoop down from the sky and charge into $N.",ch,NULL,victim,TO_CHAR);
        act("$n swoops down from the sky and charges into $N.",ch,NULL,victim,TO_NOTVICT);
        act("$n swoops down from the sky and charges into you.",ch,NULL,victim,TO_VICT);
        if (ch->pcdata->powers[ANGEL_JUSTICE] >= 5) multi_hit( ch, victim, TYPE_UNDEFINED);
        else if (number_range(1,3)==1) multi_hit( ch, victim, TYPE_UNDEFINED);
     }  */
    multi_hit( ch, victim, TYPE_UNDEFINED );
    return;
}

void do_backstab( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    OBJ_DATA *obj;

    one_argument( argument, arg );
    if ( arg[0] == '\0' )
    {
        send_to_char( "Backstab whom?\n\r", ch );
        return;
    }
    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
        send_to_char( "They aren't here.\n\r", ch );
        return;
    }
    if ( victim == ch )
    {
        send_to_char( "How can you sneak up on yourself?\n\r", ch );
        return;
    }
    if (is_safe( ch, victim )) return;
    if ((( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL || obj->value[3] != 11)
        && ((obj = get_eq_char( ch, WEAR_HOLD ) ) == NULL || obj->value[3] != 11))
    {
        send_to_char( "You need to wield a piercing weapon.\n\r", ch );
        return;
    }
    if ( victim->fighting != NULL )
    {
        send_to_char( "You can't backstab a fighting person.\n\r", ch );
        return;
    }
    WAIT_STATE( ch, skill_table[gsn_backstab].beats );
    if (!IS_NPC(victim) && IS_IMMUNE(victim, IMM_BACKSTAB) && ch->power[DISC_VAMP_QUIE] < 10 &&
        !IS_CLASS(ch, CLASS_NINJA) && !IS_CLASS(ch, CLASS_DROW))
        damage( ch, victim, 0, gsn_backstab );
    else if (!IS_AWAKE(victim) || IS_NPC(ch) || number_percent( ) < ch->pcdata->learned[gsn_backstab])
        multi_hit( ch, victim, gsn_backstab );
    if((ch->pcdata->rank == BELT_SEVEN || ch->pcdata->rank == BELT_EIGHT || ch->pcdata->rank == BELT_NINE
        || ch->pcdata->rank == BELT_TEN) && number_range(1,3) == 1)
        multi_hit( ch, victim, gsn_backstab );
    if((ch->pcdata->rank == BELT_NINE || ch->pcdata->rank == BELT_TEN) && number_range(1,6) == 1)
        multi_hit( ch, victim, gsn_backstab );
    else
        damage( ch, victim, 0, gsn_backstab );
    return;
}

void do_flee( CHAR_DATA *ch, char *argument )
{
    ROOM_INDEX_DATA *was_in;
    ROOM_INDEX_DATA *now_in;
    CHAR_DATA *victim;
    char buf[MAX_STRING_LENGTH];
    int attempt;

    WAIT_STATE(ch,4);                                       /* to prevent spam fleeing */

    if (IS_EXTRA(ch, TIED_UP))
    {
        send_to_char("Not while tied up.\n\r",ch);
        return;
    }
    if ((victim = ch->fighting ) == NULL)
    {
        if (ch->position == POS_FIGHTING) ch->position = POS_STANDING;
        send_to_char( "You aren't fighting anyone.\n\r", ch );
        return;
    }
    if (ch->move <= 0)
    {
        send_to_char("You can't find the energy to leave.\n\r",ch);
        return;
    }
    if (IS_AFFECTED(ch, AFF_WEBBED))
    {
        send_to_char("You are unable to move with all this sticky webbing on.\n\r",ch);
        return;
    }
    if (IS_SET(ch->flag2, AFF_TOTALBLIND) )
    {
        stc("You are unable to see anything, let alone flee!\n\r",victim);
        return;
    }
    /*  if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_TANARRI))
      {
        if (ch->pcdata->powers[TANARRI_FURY_ON] == 1)
        {
          stc("Only cowards retreat from combat.\n\r",ch);
          return;
        }
      }*/
    if (IS_CLASS(victim, CLASS_VAMPIRE) && IS_SET(victim->newbits,NEW_COIL) && number_percent( ) > 30)
    {
        sprintf(buf,"Not with %s coiled around you!",victim->name);
        send_to_char(buf,ch);
        act("$n cant escape with $N coiled around them.",ch,NULL,victim,TO_ROOM);
        return;
    }
    if (!IS_NPC(victim))
    {
        if (IS_CLASS(victim, CLASS_TANARRI) && IS_SET(victim->pcdata->powers[TANARRI_POWER],TANARRI_TENDRILS) && number_percent( ) > 30)
        {
            sprintf(buf,"%s spews a shower of tendrils at you, preventing your escape!",victim->name);
            send_to_char(buf,ch);
            act("$n tries to run, but $N spews forth a shower of tendrils, preventing $n from running.",ch,NULL,victim,TO_ROOM);
            return;
        }
    }
    if (IS_CLASS(victim, CLASS_MAGE) && IS_ITEMAFF(victim,ITEMA_ILLUSIONS) && number_percent( ) > 30)
    {
        sprintf(buf,"Theres to many of %s to escape!",victim->name);
        send_to_char(buf,ch);
        act("$n cant escape with $N blocking his way.",ch,NULL,victim,TO_ROOM);
        return;
    }
    if (IS_CLASS(victim, CLASS_MONK) && IS_SET(victim->newbits,NEW_JAWLOCK) && number_percent( ) > 30)
    {
        sprintf(buf,"Not with god holding you!\n\r");
        send_to_char(buf,ch);
        act("$n cant escape god's mighty hold!",ch,NULL,victim,TO_ROOM);
        return;
    }
    if (IS_CLASS(victim, CLASS_WEREWOLF) && IS_SET(victim->newbits,NEW_JAWLOCK) && number_percent( ) > 30)
    {
        sprintf(buf,"Not with %s's jaws clamped on your neck!",victim->name);
        send_to_char(buf,ch);
        act("$n cant escape $N's clamped jaws!",ch,NULL,victim,TO_ROOM);
        return;
    }
    if (IS_CLASS(victim, CLASS_KAHN) && IS_SET(victim->newbits,NEW_JAWLOCK) && number_percent( ) > 30)
    {
        sprintf(buf,"Not with %s's claws clamped on your body!",victim->name);
        send_to_char(buf,ch);
        act("$n cant escape $N's clamped claws!",ch,NULL,victim,TO_ROOM);
        return;
    }
    if (IS_CLASS(victim, CLASS_UNDEAD_KNIGHT) && IS_SET(victim->pcdata->powers[AURAS], BOG_AURA) && number_percent() > 30)
    {
        sprintf(buf,"Your stuck in the swamp surrounding %s!",victim->name);
        send_to_char(buf,ch);
        act("$n is stuck in the swamp!",ch,NULL,victim,TO_ROOM);
        return;
    }
    was_in = ch->in_room;
    for ( attempt = 0; attempt < 6; attempt++ )
    {
        EXIT_DATA *pexit;
        int door;

        door = number_door( );
        if ((pexit = was_in->exit[door] ) == 0 || pexit->to_room == NULL
            || IS_SET(pexit->exit_info, EX_CLOSED) || ( IS_NPC(ch)
            &&   IS_SET(pexit->to_room->room_flags, ROOM_NO_MOB)))
            continue;
        move_char( ch, door );
        if (( now_in = ch->in_room ) == was_in)
            continue;
        ch->in_room = was_in;
        act( "$n has fled!", ch, NULL, NULL, TO_ROOM );
        ch->in_room = now_in;
        if ( !IS_NPC(ch) )
            send_to_char( "You flee from combat!  Coward!\n\r", ch );
        WAIT_STATE(ch,1*PULSE_VIOLENCE);
        send_to_char( "You try to catch your breath.\n\r", ch );
        if (ch->position == POS_FIGHTING) stop_fighting(ch,TRUE);;
        if (victim->position == POS_FIGHTING) stop_fighting(victim,TRUE);
        if (ch == victim->fighting ) stop_fighting(victim,TRUE);
        update_pos(ch);
        update_pos(victim);
        return;
    }
    send_to_char( "You were unable to escape!\n\r", ch );
    return;
}

void do_rescue( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    CHAR_DATA *fch;

    one_argument( argument, arg );
    if ( arg[0] == '\0' )
    {
        send_to_char( "Rescue whom?\n\r", ch );
        return;
    }
    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
        send_to_char( "They aren't here.\n\r", ch );
        return;
    }
    if ( victim == ch )
    {
        send_to_char( "What about fleeing instead?\n\r", ch );
        return;
    }
    if (!IS_NPC(ch) && IS_NPC(victim))
    {
        send_to_char( "Doesn't need your help!\n\r", ch );
        return;
    }
    if ( ch->fighting == victim )
    {
        send_to_char( "Too late.\n\r", ch );
        return;
    }
    if ( ( fch = victim->fighting ) == NULL )
    {
        send_to_char( "That person is not fighting right now.\n\r", ch );
        return;
    }
    if ( is_safe(ch, fch) || is_safe(ch, victim) ) return;
    WAIT_STATE( ch, skill_table[gsn_rescue].beats );
    if ( !IS_NPC(ch) && number_percent( ) > ch->pcdata->learned[gsn_rescue] )
    {
        send_to_char( "You fail the rescue.\n\r", ch );
        return;
    }
    act( "You rescue $N!",  ch, NULL, victim, TO_CHAR    );
    act( "$n rescues you!", ch, NULL, victim, TO_VICT    );
    act( "$n rescues $N!",  ch, NULL, victim, TO_NOTVICT );
    stop_fighting( fch, FALSE );
    stop_fighting( victim, FALSE );
    set_fighting( ch, fch );
    set_fighting( fch, ch );
    do_humanity(ch,"");
    return;
}

void do_kick( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    OBJ_DATA *boots;
    int dam;
    int stance;
    int hitpoints;

    if (!IS_NPC(ch) && ch->level < skill_table[gsn_kick].skill_level)
    {
        send_to_char("First you should learn to kick.\n\r", ch );
        return;
    }
    if ( ( victim = ch->fighting ) == NULL )
    {
        send_to_char( "You aren't fighting anyone.\n\r", ch );
        return;
    }
    WAIT_STATE( ch, skill_table[gsn_kick].beats );
    if ((IS_NPC(ch) && ch->level<1800))
    {
        dam=500;
        damage(ch,victim,dam,gsn_kick);
        return;
    }
    if ((IS_NPC(ch)) ||  number_percent( ) < ch->pcdata->learned[gsn_kick]) dam = number_range(1,4);
    else
    {
        dam = 0;
        damage( ch, victim, dam, gsn_kick );
        return;
    }
    dam += char_damroll(ch);
    if (dam == 0) dam = 1;
    if (!IS_AWAKE(victim)) dam *= 2;
    if(IS_NPC(ch)) dam *= 100;
    if ( !IS_NPC(victim) && IS_CLASS(victim, CLASS_WEREWOLF) )
    {
        if (IS_SET(victim->special, SPC_WOLFMAN)) dam *= 0.8;
        if (victim->power[DISC_WERE_BOAR] > 2 ) dam *= 0.5;
        if ((boots = get_eq_char( ch, WEAR_FEET ) ) != NULL
            && IS_SET(boots->spectype, SITEM_SILVER)) dam *= 2;
    }
    if (!IS_NPC(ch)) dam = dam + (dam * ((ch->wpn[0]+1) / 100));
    if (!IS_NPC(ch))
    {
        stance = ch->stance[0];
        if ( IS_STANCE(ch, STANCE_NORMAL) ) dam *= 1.25;
        else dam = dambonus(ch,victim,dam,stance);
    }
    if (!IS_NPC(ch) && !IS_NPC(victim) && dam > 750) dam = 750;
    if (dam <= 0) dam = 2;
    dam = cap_dam(ch, victim, dam);
    hitpoints = victim->hit;
    if ( !IS_NPC(victim) && IS_IMMUNE(victim, IMM_KICK) && !IS_NPC(ch)
        && !(IS_CLASS(ch, CLASS_DEMON) && IS_DEMAFF(ch, DEM_HOOVES)) && !(IS_CLASS(ch, CLASS_SHAPESHIFTER)
        && ch->pcdata->powers[SHAPE_FORM] == BULL_FORM))
    {
        if (IS_CLASS(ch, CLASS_DEMON) && IS_DEMAFF(ch, DEM_HOOVES))
            dam = dam /2;
        if (IS_NPC(ch))
            dam = dam * 10;
        victim->hit += dam;
        damage( ch, victim, dam, gsn_kick );
        victim->hit = hitpoints;
    }
    else if IS_IMMUNE(victim, IMM_KICK)
    {
        dam=dam/2;
        damage( ch, victim, dam, gsn_kick );
    }
    else damage( ch, victim, dam, gsn_kick );
    return;
}

int dambonus( CHAR_DATA *ch, CHAR_DATA *victim, int dam, int stance)
{
    if (dam < 1) return 0;
    if (stance < 1) return dam;
    if (!IS_NPC(ch) && !can_counter(victim))
    {
        if ( IS_STANCE(ch, STANCE_MONKEY) )
        {
            int mindam = dam * 0.25;
            dam *= (ch->stance[STANCE_MONKEY]+1) / 200;
            if (dam < mindam) dam = mindam;
        }
        else if ( IS_STANCE(ch, STANCE_BULL) && ch->stance[STANCE_BULL] > 100 )
            dam += dam * (ch->stance[STANCE_BULL] / 100);
        else if ( IS_STANCE(ch, STANCE_DRAGON) && ch->stance[STANCE_DRAGON] > 100 )
            dam += dam * (ch->stance[STANCE_DRAGON] / 100);
        else if ( IS_STANCE(ch, STANCE_WOLF) && ch->stance[STANCE_WOLF] > 100)
            dam += dam*(ch->stance[STANCE_WOLF] /75);
        else if ( IS_STANCE(ch, STANCE_TIGER) && ch->stance[STANCE_TIGER] > 100 )
            dam += dam * (ch->stance[STANCE_TIGER] / 100);
        /* ------------ This is the part for superstances, Jobo ------------------- */
        else if ( ch->stance[0] > 12 && IS_SET(ch->stance[(ch->stance[0]+6)], STANCEPOWER_DAMAGE_3)
            && ch->stance[(ch->stance[0])] >100)
            dam += dam * 3 * (ch->stance[(ch->stance[0])] / 100);
        else if ( ch->stance[0] > 12 && IS_SET(ch->stance[(ch->stance[0]+6)], STANCEPOWER_DAMAGE_2)
            && ch->stance[(ch->stance[0])] >100)
            dam += dam * 2 * (ch->stance[(ch->stance[0])] / 100);
        else if ( ch->stance[0] > 12 && IS_SET(ch->stance[(ch->stance[0]+6)], STANCEPOWER_DAMAGE_1)
            && ch->stance[(ch->stance[0])] >100)
            dam += dam * (ch->stance[(ch->stance[0])] / 100);
        /* ------------ This is the end for superstances, Jobo ------------------- */

        else if ( ch->stance[0] > 0 && ch->stance[stance] < 100 )
            dam *= 0.5;
    }
    if (!IS_NPC(victim) && !can_counter(ch))
    {
        if ( IS_STANCE(victim, STANCE_CRAB) && victim->stance[STANCE_CRAB] > 100 )
            dam /= victim->stance[STANCE_CRAB]/100;
        else if ( IS_STANCE(victim, STANCE_DRAGON) && victim->stance[STANCE_DRAGON] > 100)
            dam /= victim->stance[STANCE_DRAGON]/100;
        else if ( IS_STANCE(victim, STANCE_DRAGON) && victim->stance[STANCE_DRAGON] > 100 )
            dam /= victim->stance[STANCE_DRAGON]/100;
        else if ( IS_STANCE(victim, STANCE_SWALLOW) && victim->stance[STANCE_SWALLOW] > 100 )
            dam /= victim->stance[STANCE_SWALLOW]/100;
        /* ------------ This is the part for superstances, Jobo ------------------- */
        else if ( victim->stance[0] > 12 &&
            IS_SET(victim->stance[(victim->stance[0]+6)], STANCEPOWER_RESIST_3)
            && victim->stance[(victim->stance[0])] >100)
            dam /= 3 * (victim->stance[(victim->stance[0])] / 100);
        else if ( victim->stance[0] > 12 &&
            IS_SET(victim->stance[(victim->stance[0]+6)], STANCEPOWER_RESIST_2)
            && victim->stance[(victim->stance[0])] >100)
            dam /= 2 * (victim->stance[(victim->stance[0])] / 100);
        else if ( victim->stance[0] > 12 &&
            IS_SET(victim->stance[(victim->stance[0]+6)], STANCEPOWER_RESIST_1)
            && victim->stance[(victim->stance[0])] >100)
            dam /= (victim->stance[(victim->stance[0])] / 100);
        /* ------------ This is the end for superstances, Jobo ------------------- */
        else if (ch->stance[0] == STANCE_WOLF) dam /= 3 * (victim->stance[0]+6 /75);
    }

    return dam;
}

void do_punch( CHAR_DATA *ch, char *argument )
{
    char arg [MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    int dam;
    int store;
    bool broke = FALSE;

    one_argument(argument,arg);
    if ( IS_NPC(ch) ) return;
    if ( ch->level < skill_table[gsn_punch].skill_level)
    {
        send_to_char( "First you should learn to punch.\n\r", ch );
        return;
    }
    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
        send_to_char( "They are not here.\n\r", ch );
        return;
    }
    if ( ch == victim )
    {
        send_to_char( "You cannot punch yourself!\n\r", ch );
        return;
    }
    if (is_safe(ch,victim)) return;
    if ( victim->hit < victim->max_hit )
    {
        send_to_char( "They are hurt and suspicious.\n\r", ch );
        return;
    }
    if ( victim->position < POS_FIGHTING )
    {
        send_to_char( "You can only punch someone who is standing.\n\r", ch );
        return;
    }
    if ( victim->played/3600 < 5 )
    {
        stc("You can't punch a newbie.\n\r",ch);
        return;
    }
    act("You draw your fist back and aim a punch at $N.",ch,NULL,victim,TO_CHAR);
    act("$n draws $s fist back and aims a punch at you.",ch,NULL,victim,TO_VICT);
    act("$n draws $s fist back and aims a punch at $N.",ch,NULL,victim,TO_NOTVICT);
    WAIT_STATE( ch, skill_table[gsn_punch].beats );
    if ( IS_NPC(ch) || number_percent( ) < ch->pcdata->learned[gsn_punch] )
        dam = number_range(1,4);
    else
    {
        dam = 0;
        damage( ch, victim, dam, gsn_punch );
        return;
    }
    dam += char_damroll(ch);
    if (dam == 0) dam = 1;
    if (!IS_AWAKE(victim)) dam *= 2;
    if ( !IS_NPC(ch) ) dam = dam + (dam * (ch->wpn[0] / 100));
    if ( dam <= 0 ) dam = 1;
    if (dam > 1000) dam = 1000;
    if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_WEREWOLF) && victim->power[DISC_WERE_BOAR] > 3)
    {
        store = victim->hit;
        victim->hit += dam;
        damage( ch, victim, dam, gsn_punch );
        victim->hit = store;
        if (number_percent() <= 25 && !IS_ARM_L(ch,LOST_ARM) && !IS_ARM_L(ch,LOST_HAND))
        {
            if (!IS_ARM_L(ch, LOST_FINGER_I) && !IS_ARM_L(ch, BROKEN_FINGER_I))
                {SET_BIT(ch->loc_hp[LOC_ARM_L],BROKEN_FINGER_I);broke = TRUE;}
                if (!IS_ARM_L(ch, LOST_FINGER_M) && !IS_ARM_L(ch, BROKEN_FINGER_M))
                    {SET_BIT(ch->loc_hp[LOC_ARM_L],BROKEN_FINGER_M);broke = TRUE;}
                    if (!IS_ARM_L(ch, LOST_FINGER_R) && !IS_ARM_L(ch, BROKEN_FINGER_R))
                        {SET_BIT(ch->loc_hp[LOC_ARM_L],BROKEN_FINGER_R);broke = TRUE;}
                        if (!IS_ARM_L(ch, LOST_FINGER_L) && !IS_ARM_L(ch, BROKEN_FINGER_L))
                            {SET_BIT(ch->loc_hp[LOC_ARM_L],BROKEN_FINGER_L);broke = TRUE;}
                            if (broke)
                            {
                                act("The fingers on your left hand shatter under the impact of the blow!",ch,NULL,NULL,TO_CHAR);
                                act("The fingers on $n's left hand shatter under the impact of the blow! ",ch,NULL,NULL,TO_ROOM);
                            }
        }
        else if (number_percent() <= 25 && !IS_ARM_R(ch,LOST_ARM) && !IS_ARM_R(ch,LOST_HAND))
        {
            if (!IS_ARM_R(ch, LOST_FINGER_I) && !IS_ARM_R(ch, BROKEN_FINGER_I))
                {SET_BIT(ch->loc_hp[LOC_ARM_R],BROKEN_FINGER_I);broke = TRUE;}
                if (!IS_ARM_R(ch, LOST_FINGER_M) && !IS_ARM_R(ch, BROKEN_FINGER_M))
                    {SET_BIT(ch->loc_hp[LOC_ARM_R],BROKEN_FINGER_M);broke = TRUE;}
                    if (!IS_ARM_R(ch, LOST_FINGER_R) && !IS_ARM_R(ch, BROKEN_FINGER_R))
                        {SET_BIT(ch->loc_hp[LOC_ARM_R],BROKEN_FINGER_R);broke = TRUE;}
                        if (!IS_ARM_R(ch, LOST_FINGER_L) && !IS_ARM_R(ch, BROKEN_FINGER_L))
                            {SET_BIT(ch->loc_hp[LOC_ARM_R],BROKEN_FINGER_L);broke = TRUE;}
                            if (broke)
                            {
                                act("The fingers on your right hand shatter under the impact of the blow!",ch,NULL,NULL,TO_CHAR);
                                act("The fingers on $n's right hand shatter under the impact of the blow! ",ch,NULL,NULL,TO_ROOM);
                            }
        }
        stop_fighting(victim,TRUE);
        return;
    }
    damage( ch, victim, dam, gsn_punch );
    if (victim == NULL || victim->position == POS_DEAD || dam < 1) return;
    if (victim->position == POS_FIGHTING) stop_fighting(victim,TRUE);
    if (number_percent() <= 25 && !IS_HEAD(victim,BROKEN_NOSE) && !IS_HEAD(victim,LOST_NOSE))
    {
        act("Your nose shatters under the impact of the blow!",victim,NULL,NULL,TO_CHAR);
        act("$n's nose shatters under the impact of the blow!",victim,NULL,NULL,TO_ROOM);
        SET_BIT(victim->loc_hp[LOC_HEAD],BROKEN_NOSE);
    }
    else if (number_percent() <= 25 && !IS_HEAD(victim,BROKEN_JAW))
    {
        act("Your jaw shatters under the impact of the blow!",victim,NULL,NULL,TO_CHAR);
        act("$n's jaw shatters under the impact of the blow!",victim,NULL,NULL,TO_ROOM);
        SET_BIT(victim->loc_hp[LOC_HEAD],BROKEN_JAW);
    }
    act("You fall to the ground stunned!",victim,NULL,NULL,TO_CHAR);
    act("$n falls to the ground stunned!",victim,NULL,NULL,TO_ROOM);
    victim->position = POS_STUNNED;
    if (dam > 1000) dam = 1000;
    return;
}

void do_berserk3(CHAR_DATA *ch, char *argument)
{
    char arg [MAX_INPUT_LENGTH];
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;
    CHAR_DATA *mount;
    int number_hit = 0;

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if ( ch->level < skill_table[gsn_berserk].skill_level)
    {
        send_to_char( "You are not wild enough to go berserk.\n\r", ch );
        return;
    }

    WAIT_STATE( ch, 5);

    if ( number_percent( ) > ch->pcdata->learned[gsn_berserk] )
    {
        act("You rant and rave, but nothing much happens.",ch,NULL,NULL,TO_CHAR);
        act("$n gets a wild look in $s eyes, but nothing much happens.",ch,NULL,NULL,TO_ROOM);
        return;
    }

    act("You go BERSERK!",ch,NULL,NULL,TO_CHAR);
    act("$n goes BERSERK!",ch,NULL,NULL,TO_ROOM);

    for ( vch = char_list; vch != NULL; vch = vch_next )
    {
        vch_next  = vch->next;
        if (number_hit > 4) continue;
        if (vch->in_room == NULL) continue;
        if (!IS_NPC(vch)) continue;
        if ( ch == vch ) continue;
        if ( vch->in_room == ch->in_room )
        {
            if ( ( mount = ch->mount ) != NULL )
                if ( mount == vch ) continue;
            if (can_see(ch,vch))
            {
                multi_hit( ch, vch, TYPE_UNDEFINED );
                number_hit++;
            }
        }
    }

    return;
}

void do_berserk(CHAR_DATA *ch, char *argument)
{
    CHAR_DATA * victim_table[BERSERK_HITS];
    char arg [MAX_INPUT_LENGTH];
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;
    CHAR_DATA *mount;
    int number_hit = 0;
    int iFound = 0;

    argument = one_argument( argument, arg );

    if (IS_NPC(ch) || (ch->in_room == NULL)) return;

    if ( ch->level < skill_table[gsn_berserk].skill_level)
    {
        send_to_char( "You are not wild enough to go berserk.\n\r", ch );
        return;
    }

    if ( number_percent( ) > ch->pcdata->learned[gsn_berserk] )
    {
        act("You rant and rave, but nothing much happens.",ch,NULL,NULL,TO_CHAR);
        act("$n gets a wild look in $s eyes, but nothing much happens.",ch,NULL,NULL,TO_ROOM);
        WAIT_STATE( ch, 4);
        return;
    }

    act("You go BERSERK!",ch,NULL,NULL,TO_CHAR);
    act("$n goes BERSERK!",ch,NULL,NULL,TO_ROOM);

    for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
    {
        vch_next  = vch->next_in_room;

        if (number_hit >= BERSERK_HITS) break;
        if (!IS_NPC(vch)) continue;
        if ( ch == vch ) continue;
        if ( is_same_group(ch, vch) ) continue;

        if ( vch->in_room == ch->in_room )
        {
            if ( ( mount = ch->mount ) != NULL )
            {
                if ( mount == vch ) continue;
            }

            if (can_see(ch,vch))
            {
                victim_table[number_hit] = vch;
                number_hit++;
            }
        }
    }

    iFound = number_hit;

    for(number_hit = 0; number_hit < iFound; number_hit++)
    {
        multi_hit( ch, victim_table[number_hit], TYPE_UNDEFINED );
    }

    WAIT_STATE(ch, 4);

    return;
}

void do_berserk2( CHAR_DATA *ch, char *argument )
{
    char arg [MAX_INPUT_LENGTH];
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;
    CHAR_DATA *mount;
    int number_hit = 0;

    argument = one_argument( argument, arg );
    if (IS_NPC(ch)) return;
    if ( ch->level < skill_table[gsn_berserk].skill_level)
    {
        send_to_char( "You are not wild enough to go berserk.\n\r", ch );
        return;
    }
    WAIT_STATE( ch, skill_table[gsn_berserk].beats );
    if ( number_percent( ) > ch->pcdata->learned[gsn_berserk] )
    {
        act("You rant and rave, but nothing much happens.",ch,NULL,NULL,TO_CHAR);
        act("$n gets a wild look in $s eyes, but nothing much happens.",ch,NULL,NULL,TO_ROOM);
        return;
    }
    act("You go BERSERK!",ch,NULL,NULL,TO_CHAR);
    act("$n goes BERSERK!",ch,NULL,NULL,TO_ROOM);
    for ( vch = char_list; vch != NULL; vch = vch_next )
    {
        vch_next  = vch->next;
        if (number_hit > 4) continue;
        if (vch->in_room == NULL) continue;
        if (!IS_NPC(vch) && vch->pcdata->chobj != NULL) continue;
        if ( ch == vch ) continue;
        if ( vch->in_room == ch->in_room )
        {
            if ( ( mount = ch->mount ) != NULL )
                if ( mount == vch ) continue;
            if (can_see(ch,vch))
            {
                multi_hit( ch, vch, TYPE_UNDEFINED );
                number_hit++;
            }
        }
    }
    do_beastlike(ch,"");
    return;
}

/* Hurl skill by KaVir */
void do_hurl( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    CHAR_DATA *mount;
    ROOM_INDEX_DATA *to_room;
    EXIT_DATA *pexit;
    EXIT_DATA *pexit_rev;
    char buf [MAX_INPUT_LENGTH];
    char direction [MAX_INPUT_LENGTH];
    char arg1 [MAX_INPUT_LENGTH];
    char arg2 [MAX_INPUT_LENGTH];
    int door;
    int rev_dir;
    int dam;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
    if ( !IS_NPC(ch) && ch->pcdata->learned[gsn_hurl] < 1 )
    {
        send_to_char("Maybe you should learn the skill first?\n\r",ch);
        return;
    }
    if ( arg1[0] == '\0' )
    {
        send_to_char("Who do you wish to hurl?\n\r", ch);
        return;
    }
    if ( ( victim = get_char_room( ch, arg1 ) ) == NULL )
    {
        send_to_char("They are not here.\n\r", ch);
        return;
    }
    if ( victim == ch )
    {
        send_to_char( "How can you hurl yourself?\n\r", ch );
        return;
    }
    if (IS_SET(victim->act, ACT_DUMMY))
    {
        send_to_char("You try to hurl a dummy and almost break your neck!",ch);
        return;
    }
    if (is_safe( ch, victim )) return;
    if ( (mount = victim->mount) != NULL && victim->mounted == IS_MOUNT)
    {
        send_to_char("But they have someone on their back!\n\r",ch);
        return;
    }
    else if ( (mount = victim->mount) != NULL && victim->mounted == IS_RIDING)
    {
        send_to_char("But they are riding!\n\r",ch);
        return;
    }
    if ( !IS_NPC(victim) && IS_IMMUNE(victim, IMM_HURL) )
    {
        send_to_char("You are unable to get their feet of the ground.\n\r",ch);
        return;
    }
    if ( IS_NPC(victim) && victim->level > 900 )
    {
        send_to_char("You are unable to get their feet of the ground.\n\r",ch);
        return;
    }
    if (  ( victim->position == POS_FIGHTING && victim->fighting != ch) )
    {
        act( "$N is hurt and suspicious, and you are unable to approach $M.", ch, NULL, victim, TO_CHAR );
        return;
    }
    WAIT_STATE( ch, skill_table[gsn_hurl].beats );
    if ( !IS_NPC(ch) && number_percent( ) > ch->pcdata->learned[gsn_hurl] )
    {
        send_to_char("You are unable to get their feet of the ground.\n\r",ch);
        multi_hit( victim, ch, TYPE_UNDEFINED );
        return;
    }
    rev_dir = 0;
    if ( arg2[0] == '\0' ) door = number_range(0,3);
    else
    {
        if      (!str_cmp( arg2, "n" ) || !str_cmp( arg2, "north")) door = 0;
        else if (!str_cmp( arg2, "e" ) || !str_cmp( arg2, "east")) door = 1;
        else if (!str_cmp( arg2, "s" ) || !str_cmp( arg2, "south")) door = 2;
        else if (!str_cmp( arg2, "w" ) || !str_cmp( arg2, "west")) door = 3;
        else
        {
            send_to_char("You can only hurl people north, south, east or west.\n\r", ch);
            return;
        }
    }
    if (door == 0) {sprintf(direction,"north");rev_dir = 2;}
    if (door == 1) {sprintf(direction,"east");rev_dir = 3;}
    if (door == 2) {sprintf(direction,"south");rev_dir = 0;}
    if (door == 3) {sprintf(direction,"west");rev_dir = 1;}
    if (( pexit = ch->in_room->exit[door]) == NULL || (to_room = pexit->to_room) == NULL)
    {
        sprintf(buf,"$n hurls $N into the %s wall.", direction);
        act(buf,ch,NULL,victim,TO_NOTVICT);
        sprintf(buf,"You hurl $N into the %s wall.", direction);
        act(buf,ch,NULL,victim,TO_CHAR);
        sprintf(buf,"$n hurls you into the %s wall.", direction);
        act(buf,ch,NULL,victim,TO_VICT);
        dam = number_range(ch->level, (ch->level * 4));
        victim->hit = victim->hit - dam;
        update_pos(victim);
        if (IS_NPC(victim) && !IS_NPC(ch)) ch->mkill = ch->mkill + 1;
        if (!IS_NPC(victim) && IS_NPC(ch)) victim->mdeath = victim->mdeath + 1;
        if (victim->position == POS_DEAD) {raw_kill(victim);return;}
        return;
    }
    pexit = victim->in_room->exit[door];
    if( IS_SET(pexit->exit_info, EX_PRISMATIC_WALL))
    {
        sprintf(buf,"$n hurls $N into the %s wall.", direction);
        act(buf,ch,NULL,victim,TO_NOTVICT);
        sprintf(buf,"You hurl $N into the %s wall.", direction);
        act(buf,ch,NULL,victim,TO_CHAR);
        sprintf(buf,"$n hurls you into the %s wall.", direction);
        act(buf,ch,NULL,victim,TO_VICT);
        dam = number_range(ch->level, (ch->level * 4));
        victim->hit = victim->hit - dam;
        update_pos(victim);
        if (IS_NPC(victim) && !IS_NPC(ch)) ch->mkill = ch->mkill + 1;
        if (!IS_NPC(victim) && IS_NPC(ch)) victim->mdeath = victim->mdeath + 1;
        if (victim->position == POS_DEAD) {raw_kill(victim);return;}
        return;
    }
    if (IS_SET(pexit->exit_info, EX_CLOSED) && !IS_AFFECTED(victim,AFF_PASS_DOOR) && !IS_AFFECTED(victim,AFF_ETHEREAL))
    {
        if (IS_SET(pexit->exit_info, EX_LOCKED))
            REMOVE_BIT(pexit->exit_info, EX_LOCKED);
        if (IS_SET(pexit->exit_info, EX_CLOSED))
            REMOVE_BIT(pexit->exit_info, EX_CLOSED);
        sprintf(buf,"$n hoists $N in the air and hurls $M %s.", direction);
        act(buf,ch,NULL,victim,TO_NOTVICT);
        sprintf(buf,"You hoist $N in the air and hurl $M %s.", direction);
        act(buf,ch,NULL,victim,TO_CHAR);
        sprintf(buf,"$n hurls you %s, smashing you through the %s.", direction,pexit->keyword);
        act(buf,ch,NULL,victim,TO_VICT);
        sprintf(buf,"There is a loud crash as $n smashes through the $d.");
        act(buf,victim,NULL,pexit->keyword,TO_ROOM);
        if ((to_room = pexit->to_room ) != NULL && (pexit_rev = to_room->exit[rev_dir]) != NULL
            && pexit_rev->to_room == ch->in_room && pexit_rev->keyword != NULL)
        {
            if (IS_SET(pexit_rev->exit_info, EX_LOCKED))
                REMOVE_BIT( pexit_rev->exit_info, EX_LOCKED );
            if (IS_SET(pexit_rev->exit_info, EX_CLOSED))
                REMOVE_BIT( pexit_rev->exit_info, EX_CLOSED );
            if (door == 0) sprintf(direction,"south");
            if (door == 1) sprintf(direction,"west");
            if (door == 2) sprintf(direction,"north");
            if (door == 3) sprintf(direction,"east");
            char_from_room(victim);
            char_to_room(victim,to_room);
            sprintf(buf,"$n comes smashing in through the %s $d.", direction);
            act(buf,victim,NULL,pexit->keyword,TO_ROOM);
            dam = number_range(ch->level, (ch->level * 6));
            victim->hit = victim->hit - dam;
            update_pos(victim);
            if (IS_NPC(victim) && !IS_NPC(ch)) ch->mkill = ch->mkill + 1;
            if (!IS_NPC(victim) && IS_NPC(ch)) victim->mdeath = victim->mdeath + 1;
            if (victim->position == POS_DEAD) {raw_kill(victim);return;}
        }
    }
    else
    {
        sprintf(buf,"$n hurls $N %s.", direction);
        act(buf,ch,NULL,victim,TO_NOTVICT);
        sprintf(buf,"You hurl $N %s.", direction);
        act(buf,ch,NULL,victim,TO_CHAR);
        sprintf(buf,"$n hurls you %s.", direction);
        act(buf,ch,NULL,victim,TO_VICT);
        if (door == 0) sprintf(direction,"south");
        if (door == 1) sprintf(direction,"west");
        if (door == 2) sprintf(direction,"north");
        if (door == 3) sprintf(direction,"east");
        char_from_room(victim);
        char_to_room(victim,to_room);
        sprintf(buf,"$n comes flying in from the %s.", direction);
        act(buf,victim,NULL,NULL,TO_ROOM);
        dam = number_range(ch->level, (ch->level * 2));
        victim->hit = victim->hit - dam;
        update_pos(victim);
        if (IS_NPC(victim) && !IS_NPC(ch)) ch->mkill = ch->mkill + 1;
        if (!IS_NPC(victim) && IS_NPC(ch)) victim->mdeath = victim->mdeath + 1;
        if (victim->position == POS_DEAD) {raw_kill(victim);return;}
    }
    return;
}

void do_disarm( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    OBJ_DATA *obj;
    int percent;

    if ( !IS_NPC(ch)
        &&   ch->level < skill_table[gsn_disarm].skill_level)
    {
        send_to_char( "You don't know how to disarm opponents.\n\r", ch );
        return;
    }

    if ( ( get_eq_char( ch, WEAR_WIELD ) == NULL )
        &&   ( get_eq_char( ch, WEAR_HOLD  ) == NULL ) )
    {
        send_to_char( "You must wield a weapon to disarm.\n\r", ch );
        return;
    }

    if ( ( victim = ch->fighting ) == NULL )
    {
        send_to_char( "You aren't fighting anyone.\n\r", ch );
        return;
    }

    if ( ( ( obj = get_eq_char( victim, WEAR_WIELD ) ) == NULL )
        &&   ( ( obj = get_eq_char( victim, WEAR_HOLD  ) ) == NULL ) )
    {
        send_to_char( "Your opponent is not wielding a weapon.\n\r", ch );
        return;
    }

    WAIT_STATE( ch, skill_table[gsn_disarm].beats );
    percent = number_percent( ) + victim->level - ch->level;
    if (!IS_NPC(victim) && IS_IMMUNE(victim, IMM_DISARM))
        send_to_char( "You failed.\n\r", ch );
    else if ( IS_NPC(ch) || percent < ch->pcdata->learned[gsn_disarm] * 2 / 3 )
        disarm( ch, victim );
    else
        send_to_char( "You failed.\n\r", ch );
    return;
}

/* Had problems with people not dying when POS_DEAD...KaVir */
void do_killperson( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];

    one_argument( argument, arg );
    if ( arg[0] == '\0' ) return;
    if ( ( victim = get_char_room( ch, arg ) ) == NULL ) return;
    send_to_char( "You have been KILLED!!\n\r\n\r", victim );
    if (IS_NPC(victim) && !IS_NPC(ch)) ch->mkill = ch->mkill + 1;
    if (!IS_NPC(victim) && IS_NPC(ch)) victim->mdeath = victim->mdeath + 1;
    raw_kill( victim );
    return;
}

/* For decapitating players - KaVir */
void do_decapitate( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    bool can_decap = TRUE;
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    char mur[MAX_STRING_LENGTH];
    int iUpgradeLevel;
    int amount = 0;
    int nDecapPoints = 0;
    bool deathmatch = FALSE;
    ROOM_INDEX_DATA *location;

    if (IS_NPC(ch)) return;

    one_argument( argument, arg );

    if((get_age(ch) - 17) < 5)
    {
        send_to_char("You cannot decap with less than 5 hours.\n\r", ch);
        return;
    }

    if (ch->in_room != NULL)
    {
        if (IS_SET (ch->in_room->room_flags, ROOM_ARENA))
        {
            send_to_char("You're in the arena, use TIE to finish a player.\n\r",ch);
            return;
        }
        if (IS_SET (ch->in_room->room_flags, ROOM_NODECAP))
        {
            send_to_char("You are in a nodecap zone.\n\r",ch);
            return;
        }
    }
    if ( arg[0] == '\0' )
    {
        send_to_char( "Decapitate whom?\n\r", ch );
        return;
    }
    //if (IS_CLASS(ch, CLASS_RPONLY))return;
    //if (IS_CLASS(victim, CLASS_RPONLY))return;
    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
        send_to_char( "They aren't here.\n\r", ch );
        return;
    }
    if ( ch == victim )
    {
        send_to_char( "That might be a bit tricky...\n\r", ch );
        return;
    }
    if (IS_NPC(victim))
    {
        send_to_char( "You can only decapitate other players.\n\r", ch );
        return;
    }
    if ( victim->position > 1 )
    {
        send_to_char( "You can only do this to mortally wounded players.\n\r", ch );
        return;
    }
    if (!fair_fight(ch,victim) && ((get_age(ch) - 17) < 5) ) return stcf(ch,"You dumb ass, thats what the command commit is for.\n\r");
    if (is_safe(ch,victim)) return;
    if (in_fortress(ch) && !arena2death)
    {
        fortresskill(ch, victim);
        return;
    }
    else if (in_fortress(ch) && arena2death)
    {
        deathmatch = TRUE;
        arena2 = TRUE;
        if ((location = get_room_index(ROOM_VNUM_VICTORY)) == NULL)
            return;
        char_from_room(victim);
        char_to_room(victim, location);
        char_from_room(ch);
        char_to_room(ch, location);
        ch->fight_timer = 0;
        ch->pkill++;
        victim->pdeath++;
    }
    if (ragnarok)
    {
        ragnarokdecap(ch,victim);
        return;
    }
    if (reachedDecapLimit(ch))
    {
        send_to_char("You have reached the decap limit for your current size, you need to gain more hps.\n\r",ch);
        return;
    }
    /*
     * can't decide, one or two... sigh - Jobo
     *  if (!str_cmp(ch->pcdata->last_decap[0], victim->name) || !str_cmp(ch->pcdata->last_decap[1], victim->name))
     */
    /*
     *removed for now- Beorb

      if (!str_cmp(ch->pcdata->last_decap[0], victim->name))
      {
        send_to_char("Don't spamcap, it's not nice, find someone else to pick on.\n\r",ch);
        return;
      }
    */

    if (!fair_fight(ch, victim)) can_decap = FALSE;
    if (ch->level > 6 && can_decap == FALSE) can_decap = TRUE;
    if (!can_decap)
    {
        if (IS_CLASS(victim, CLASS_VAMPIRE))       do_mortalvamp(victim,"");
        else if (IS_SET(victim->special, SPC_WOLFMAN)) do_unwerewolf(victim,"");
        behead( victim );
        victim->level = 3;
        dropinvis(ch);
        players_decap++;
        ch->pcdata->mean_paradox_counter++;
        ch->pcdata->bounty += (victim->pcdata->bounty / 2)+(victim->pcdata->upgrade_level*1000)+((10 - victim->generation)*250);
        sprintf(buf,"#P%s #owas torn to pieces by #R%s #c(#0Paradox Counter#c)#n",victim->name, ch->name);
        death_info(buf);
        if (ch->pcdata->mean_paradox_counter > 2)
        {
            ch->pcdata->mean_paradox_counter = 0;
            do_paradox(ch,"self");
        }
        return;
    }
    if(worldtest)
    {
        int qgain2;
        double qgain;
        double pscale;
        double vscale;
        vscale=victim->pcdata->perm_str+victim->pcdata->perm_dex+victim->pcdata->perm_con+victim->pcdata->perm_wis+victim->pcdata->perm_int;
        qgain=vscale*vscale/5;
        pscale=ch->pcdata->perm_str+ch->pcdata->perm_dex+ch->pcdata->perm_con+ch->pcdata->perm_wis+ch->pcdata->perm_int;
        qgain=qgain*(vscale/pscale)*(vscale/pscale)*(vscale/pscale);
        qgain=qgain*ch->pcdata->perm_int/15;
        qgain2= makeint(qgain);
        /*  victim->pcdata->perm_str=victim->pcdata->perm_str-4;
         * victim->pcdata->perm_dex=victim->pcdata->perm_dex-4;
         * victim->pcdata->perm_con=victim->pcdata->perm_con-4;
         * victim->pcdata->perm_int=victim->pcdata->perm_int-4;
         * victim->pcdata->perm_wis=victim->pcdata->perm_wis-4;
         */
        ch->pcdata->quest += qgain2;
        sprintf(buf, "#P%s, has gained %d qps for killing %s!\n\r",ch->pcdata->switchname,qgain2,victim->pcdata->switchname);
        do_info(NULL,buf);
    }
    if (victim->pcdata->bounty > 0)
    {
        sprintf(buf,"You recive a %d QP bounty, for killing %s.\n\r", victim->pcdata->bounty, victim->name);
        send_to_char(buf, ch);
        ch->pcdata->quest += victim->pcdata->bounty;
        victim->pcdata->bounty = 1000;
    }

    ch->exp += victim->exp / 2;
    victim->exp -= victim->exp / 2;

    /*
     * Update the last decaps to prevent spamcapping.
     */
    free_string( ch->pcdata->last_decap[1] );
    ch->pcdata->last_decap[1] = str_dup(ch->pcdata->last_decap[0]);
    free_string( ch->pcdata->last_decap[0] );
    if ( !str_cmp(victim->pcdata->last_decap[0],ch->name) || !str_cmp(victim->pcdata->last_decap[1],ch->name) )
        ch->pcdata->last_decap[0] = str_dup("Retaliation");
    else
        ch->pcdata->last_decap[0] = str_dup(victim->name);
    if ( !str_cmp(victim->pcdata->last_decap[0],ch->name) )
        victim->pcdata->last_decap[0] = str_dup("No One");
    if ( !str_cmp(victim->pcdata->last_decap[1],ch->name) )
        victim->pcdata->last_decap[1] = str_dup("No One");

    victim->pcdata->last_killer = ch;

    act( "A misty white vapour pours from $N's corpse into your body.",  ch, NULL, victim, TO_CHAR    );
    act( "A misty white vapour pours from $N's corpse into $n's body.",  ch, NULL, victim, TO_NOTVICT );
    act( "You double over in agony as raw energy pulses through your veins.",  ch, NULL, NULL, TO_CHAR    );
    act( "$n doubles over in agony as sparks of energy crackle around $m.",  ch, NULL, NULL, TO_NOTVICT );
    if (IS_CLASS(victim, CLASS_VAMPIRE))      do_mortalvamp(victim,"");
    else if (IS_SET(victim->special, SPC_WOLFMAN)) do_unwerewolf(victim,"");
    if (!deathmatch)
        ch->fight_timer = 12;
    if (IS_CLASS(ch, CLASS_DEMON))
    {
        send_to_char("You rip away a part of their soul.\n\r", ch);
        send_to_char("You feel a part of your soul has been torn away.\n\r", victim);
        ch->pcdata->souls += 1;
    }
    behead( victim );
    dropinvis(ch);
    /*  Removes mage powers */
    if (IS_ITEMAFF(victim, ITEMA_STEELSHIELD)) REMOVE_BIT(ch->itemaffect, ITEMA_STEELSHIELD);
    if (IS_ITEMAFF(victim, ITEMA_BEAST)) REMOVE_BIT(victim->itemaffect, ITEMA_BEAST);
    if (IS_ITEMAFF(victim, ITEMA_MAGESHIELD)) REMOVE_BIT(victim->itemaffect, ITEMA_MAGESHIELD);
    if (IS_ITEMAFF(victim, ITEMA_ILLUSIONS)) REMOVE_BIT(victim->itemaffect, ITEMA_ILLUSIONS);
    if (IS_ITEMAFF(victim, ITEMA_DEFLECTOR)) REMOVE_BIT(victim->itemaffect, ITEMA_DEFLECTOR);
    do_beastlike(ch,"");
    if (ch->pcdata->mean_paradox_counter > 0) ch->pcdata->mean_paradox_counter--;
    ch->pkill++;
    victim->pdeath++;
    ch->race++;

    if ((!IS_NPC(victim) && victim->class == 0) && IS_SET(victim->affected_by2, PLR_CLASSME))
    {
        victim->class = CLASS_WRAITH;
        victim->pdeath -= 1;
        sprintf(buf,"%s takes %s's head causing %s to become a wraith!", ch->name, victim->name, victim->name);
        do_info(ch, buf);
        victim->home = 3001;
        victim->max_hit = 2000;
        REMOVE_BIT(victim->affected_by2, PLR_CLASSME);
        //free_string(victim->pcdata->mort);
        do_restore(ch, "self");
        return;
    }
    if ( victim->race > 0 )
    {
        victim->race -= 1;
    }
    do_restore(ch, "self");
    if ( ch->pkill > 0 )

        if(ch->pcdata->pkpoints < 0)
    {
        ch->pcdata->pkpoints = 0;
    }

    if(victim->pcdata->pkpoints < 0)
    {
        victim->pcdata->pkpoints = 0;
    }

    iUpgradeLevel = victim->pcdata->upgrade_level + 1;

    nDecapPoints = (float)UMAX(1,((victim->pcdata->pkpoints / 4) + (float)UMAX(iUpgradeLevel, (iUpgradeLevel * (iUpgradeLevel / (float)2)))));

    stcf(ch,"You gained %d pk points for the decap.\n\r",nDecapPoints);
    ch->pcdata->pkpoints += nDecapPoints;
    nDecapPoints = UMAX(1,(victim->pcdata->pkpoints/4));
    victim->pcdata->pkpoints -= nDecapPoints;
    if ( victim->pcdata->pkpoints < 0 ) victim->pcdata->pkpoints = 0;
    while(downgrade(victim));                               // See bool downgrade() in upgrade.c
    if (!IS_CLASS(victim, CLASS_NINJA)) victim->rage = 0;
    victim->level = 2;
    decap_message(ch,victim);
    sprintf( log_buf, "%s decapitated by %s at %d.",
        victim->pcdata->switchname, ch->pcdata->switchname, victim->in_room->vnum );
    wiznet( NULL, WIZ_DEATH, 0, log_buf );
    log_string( log_buf );
    players_decap++;
    amount = ((ch->pkill - ch->pdeath) * number_range(1500,2000));

    if (amount > 6000)
    {
        amount = 6000;
    }

    else if (amount < 0)
    {
        amount = 0;
    }

    ch->pcdata->bounty += (victim->pcdata->bounty)+(victim->pcdata->upgrade_level * 1000)+((10- victim->generation) * 250);
    /*
     * update kingdoms
     */
    if (ch->pcdata->kingdom != 0 && victim->pcdata->kingdom != 0)
    {
        if (ch->pcdata->kingdom == victim->pcdata->kingdom)
        {
            sprintf(mur, "%s has murdered his own kingdom mate %s from %s. What such horror.\n\r",
                ch->pcdata->switchname, victim->pcdata->switchname, kingdom_table[ch->pcdata->kingdom].whoname);
            send_to_char( "You unwieldly bastard. Your kingdom has been penalized!\n\r",ch);
            kingdom_table[ch->pcdata->kingdom].qps -= number_range(1000,kingdom_table[ch->pcdata->kingdom].qps/10);
            if (kingdom_table[ch->pcdata->kingdom].qps < 0)
            {
                kingdom_table[ch->pcdata->kingdom].qps = 0;
            }
            do_info(NULL,mur);
        }
    }
    // do_sacrifice(victim, "head");
    save_kingdoms();
    do_asave(NULL,"world");
    return;
}

void do_bloodpower( CHAR_DATA *ch, char *argument )
{
    /*    OBJ_DATA *vhead;
        CHAR_DATA *victim;
        DESCRIPTOR_DATA *d;
        bool found = FALSE;
        int amount = 0;
        char buf[MAX_STRING_LENGTH];

        if ( !argument[0] ) return stcf(ch,"#RDrink the blood of whom?#n\n\r");

        for ( d = descriptor_list; d; d = d->next )
        {
        if ( d->connected != CON_PLAYING ) continue;
        if ( !str_prefix(argument,d->character->name) )
            { found = TRUE; break; }
        }

        if ( !found )
        return stcf(ch,"You can't find them!\n\r");

        if ( (victim = d->character)->in_room != ch->in_room )
        return stcf(ch,"They aren't here.\n\r");

        if ( !IS_HEAD(victim,LOST_HEAD) || !(vhead = victim->pcdata->chobj) )
        return stcf(ch,"But they aren't a head!\n\r");

        if ( ch != victim->pcdata->last_killer )
        return stcf(ch,"#RYou did not freshly decapitate them, sorry.#n\n\r");

        victim->pcdata->bloodpower[0] *= 0.5;
        amount = UMAX(50,victim->pcdata->bloodpower[0]);
        if ( victim->pcdata->bloodpower[0] < 50 )
        victim->pcdata->bloodpower[0] = 0;
        ch->pcdata->bloodpower[0] += amount;
        ch->pcdata->bloodpower[1] += amount;
        act("#RYou hold the head of $N above your own, and crush $S skull.#n\n\r"
        "#RAs the blood seeps out of $N's crushed skull, you drink it in.#n",
        ch,NULL,victim,TO_CHAR);
        act("#R$n holds your head above $s own, and crushes your skull.#n\n\r"
        "#RAs the blood seeps out of your crushed skull, $n drinks it in.#n",
        ch,NULL,victim,TO_VICT);
        act("#R$n holds the head of $N above $s own, and crushes $S skull.#n\n\r"
        "#RAs the blood seeps out of $N's crushed skull, $n drinks it in.#n",
        ch,NULL,victim,TO_NOTVICT);
        sprintf(buf,"#R%s#r has drank the blood of #R%s#r!!!#n",
        ch->name, victim->name );
        death_info(buf);
        extract_obj(vhead);*/
}

int blood_ratio( CHAR_DATA *ch )
{
    /*    int ratio = 0;
        int blood = 0;

        if ( IS_NPC(ch) ) return 0;

        blood = ch->pcdata->bloodpower[0] + ch->pcdata->bloodpower[1];
        ratio = blood / 5;
        return ratio;*/
    return 0;
}

void do_kdecapitate( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    CHAR_DATA *gch;
    DESCRIPTOR_DATA * d;
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    char mur[MAX_STRING_LENGTH];
    int iUpgradeLevel;
    int iDecapPoints;
    int iVictKingStatus;
    int iChKingStatus;
    int iBonus;

    if (IS_NPC(ch)) return;
    one_argument( argument, arg );

    if (ch->in_room != NULL)
    {
        if (IS_SET (ch->in_room->room_flags, ROOM_ARENA))
        {
            send_to_char("You're in the arena.\n\r",ch);
            return;
        }
    }
    if ( arg[0] == '\0' )
    {
        send_to_char( "Kdecapitate whom?\n\r", ch );
        return;
    }
    if(in_fortress(ch))
    {
        send_to_char("You are in the Forbidden Fortress.  Please use decapitate.", ch);
        return;
    }
    if (IS_SET (ch->in_room->room_flags, ROOM_NODECAP))
    {
        send_to_char("You are in a nodecap zone.\n\r",ch);
        return;
    }
    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
        send_to_char( "They aren't here.\n\r", ch );
        return;
    }
    if ( ch == victim )
    {
        send_to_char( "That might be a bit tricky...\n\r", ch );
        return;
    }
    if (IS_NPC(victim))
    {
        send_to_char( "You can only kdecapitate other players.\n\r", ch );
        return;
    }
    if ( ch->level < 3)
    {
        send_to_char( "Heh, A mortal, hrm....\n\r",ch);
        return;
    }
    if ( ch->pcdata->kingdom == 0)
    {
        send_to_char( "Yes, kdecap for Null Kingdom, that would be nice...\n\r",ch);
        return;
    }
    if ( victim->pcdata->kingdom == 0)
    {
        stc( " They are not in a kingdom.\n\r",ch);
        return;
    }
    if ( !kingdom_fair_fight(ch,victim) ) return stcf(ch,"You can't kill the person, they aren't in your kingdoms range.\n\r");
    if ( victim->position > 1 )
    {
        send_to_char( "You can only do this to mortally wounded players.\n\r", ch );
        return;
    }
    if (is_safe(ch,victim)) return;
    if (ragnarok)
    {
        ragnarokdecap(ch,victim);
        return;
    }

    if(victim->pcdata->kdecap_timer > 0)
    {
        sprintf(buf, "They can not be kdecapped for another %d minutes.\n\r", victim->pcdata->kdecap_timer / 15);
        send_to_char(buf, ch);
        return;
    }

    if (victim->pcdata->bounty > 0)
    {
        sprintf(buf,"Your kingdom receives a %d QP bounty, for killing %s.\n\r", victim->pcdata->bounty, victim->name);
        send_to_char(buf, ch);
        kingdom_table[ch->pcdata->kingdom].qps += victim->pcdata->bounty;
        victim->pcdata->bounty = 0;
    }

    ch->exp += victim->exp / 2;
    victim->exp -= victim->exp / 2;

    iUpgradeLevel = victim->pcdata->upgrade_level + 1;
    iDecapPoints = UMAX(3, ((iUpgradeLevel * iUpgradeLevel) / 2) + 2);
    iVictKingStatus = kingdom_table[victim->pcdata->kingdom].kills - kingdom_table[victim->pcdata->kingdom].deaths;
    iChKingStatus = kingdom_table[ch->pcdata->kingdom].kills - kingdom_table[ch->pcdata->kingdom].deaths;
    iBonus = UMAX(1, mathproxy_log(UMAX(1, UMAX(iVictKingStatus - iChKingStatus, iChKingStatus - iVictKingStatus))) / mathproxy_log(3));

    iDecapPoints = UMAX(1, (iVictKingStatus > iChKingStatus) ? (iDecapPoints * iBonus) : (iDecapPoints / iBonus));

    sprintf(buf, "You gain %d pkpoints for kingdom decapitating %s.\n\r", iDecapPoints, victim->name);
    send_to_char(buf, ch);
    ch->pcdata->pkpoints += iDecapPoints;
    iDecapPoints = UMAX(1, iDecapPoints / 3);

    if(iDecapPoints > 0)
    {
        for(d = descriptor_list; d != NULL; d = d->next)
        {
            if (IS_PLAYING(d)
                && (gch = d->character) != NULL
                && !IS_NPC(gch)
                && !IS_IMMORTAL(gch)
                && gch->in_room != NULL
                && gch->pcdata->kingdom == ch->pcdata->kingdom)
            {
                sprintf(buf, "You gain %d pkpoints for %s's kingdom decapitation of %s.\n\r", iDecapPoints, ch->name, victim->name);
                send_to_char(buf, gch);
                gch->pcdata->pkpoints += iDecapPoints;
            }
        }
    }

    /*Add QP's for the decap to the kingdoms treasury.*/
    iDecapPoints =  UMAX(100, (((iUpgradeLevel * iUpgradeLevel) / 3) + 2) * 1000);
    iDecapPoints = UMAX(100, (iVictKingStatus > iChKingStatus) ? (iDecapPoints * iBonus) : (iDecapPoints / iBonus));
    kingdom_table[ch->pcdata->kingdom].qps += iDecapPoints;
    sprintf(buf, "%s gains %d QP's for their kdecapitation.  Their treasury now totals %d!", kingdom_table[ch->pcdata->kingdom].whoname, iDecapPoints, kingdom_table[ch->pcdata->kingdom].qps);
    do_info(ch, buf);

    /*
     * Update the last decaps to prevent spamcapping.
     */
    /*  free_string( ch->pcdata->last_decap[1] );
      ch->pcdata->last_decap[1] = str_dup(ch->pcdata->last_decap[0]);
      free_string( ch->pcdata->last_decap[0] );
      ch->pcdata->last_decap[0] = str_dup(victim->name);*/

    act( "A #0m#oi#0s#ot#0y#y white #Cvapour#n pours from $N's corpse into your body.",  ch,
        NULL, victim, TO_CHAR    );
    act( "A #0m#oi#0s#ot#0y#y white #Cvapour#n pours from $N's corpse into $n's body.",  ch,
        NULL, victim, TO_NOTVICT );
    act( "You double over in agony as raw energy #p#bpulses#n through your veins.",  ch, NULL, NULL, TO_CHAR    );
    act( "$n doubles over in agony as #p#bsparks#n of energy crackle around $m.",
        ch, NULL, NULL, TO_NOTVICT );
    if (IS_CLASS(victim, CLASS_VAMPIRE))      do_mortalvamp(victim,"");
    else if (IS_SET(victim->special, SPC_WOLFMAN)) do_unwerewolf(victim,"");
    ch->fight_timer += 10;
    if (IS_CLASS(ch, CLASS_DEMON))
    {
        send_to_char("You rip away a part of their soul.\n\r", ch);
        send_to_char("You feel a part of your soul has been torn away.\n\r",
            victim);
        ch->pcdata->souls += 1;
    }
    behead( victim );
    do_sacrifice(victim, "head");
    victim->pcdata->kdecap_timer = 450;
    dropinvis(ch);
    /*  Removes mage powers */
    if (IS_ITEMAFF(victim, ITEMA_STEELSHIELD)) REMOVE_BIT(ch->itemaffect,
            ITEMA_STEELSHIELD);
    if (IS_ITEMAFF(victim, ITEMA_BEAST)) REMOVE_BIT(victim->itemaffect,
            ITEMA_BEAST);
    if (IS_ITEMAFF(victim, ITEMA_MAGESHIELD)) REMOVE_BIT(victim->itemaffect,
            ITEMA_MAGESHIELD);
    if (IS_ITEMAFF(victim, ITEMA_ILLUSIONS)) REMOVE_BIT(victim->itemaffect,
            ITEMA_ILLUSIONS);
    if (IS_ITEMAFF(victim, ITEMA_DEFLECTOR)) REMOVE_BIT(victim->itemaffect,
            ITEMA_DEFLECTOR);
    do_beastlike(ch,"");
    if (!IS_CLASS(victim, CLASS_NINJA)) victim->rage = 0;
    victim->level = 2;
    decap_message(ch,victim);
    sprintf( log_buf, "%s murdered by %s at %d.",
        victim->pcdata->switchname, ch->pcdata->switchname,
        victim->in_room->vnum );
    wiznet( NULL, WIZ_DEATH, 0, log_buf );
    log_string( log_buf );
    players_decap++;
    do_restore(ch, "self");
    ch->pcdata->bounty += number_range(30,80);
    ch->fight_timer = 12;

    if (ch->pcdata->kingdom != 0 && victim->pcdata->kingdom != 0)
    {
        if(ch->pcdata->kingdom != victim->pcdata->kingdom)
        {
            char ann[MAX_STRING_LENGTH];
            int nKingdomLoss;
            kingdom_table[ch->pcdata->kingdom].kills++;
            kingdom_table[victim->pcdata->kingdom].deaths++;
            nKingdomLoss = number_range(kingdom_table[victim->pcdata->kingdom].qps/100,kingdom_table[victim->pcdata->kingdom].qps/10);
            kingdom_table[ch->pcdata->kingdom].qps += nKingdomLoss;
            kingdom_table[victim->pcdata->kingdom].qps -= nKingdomLoss;
            if (kingdom_table[victim->pcdata->kingdom].qps < 0)
            {
                kingdom_table[victim->pcdata->kingdom].qps = 0;
            }
            sprintf(ann,"%s has taken power from %s\n\r",kingdom_table[ch->pcdata->kingdom].whoname,
                kingdom_table[victim->pcdata->kingdom].whoname);
            do_info(NULL,ann);
        }
        if (ch->pcdata->kingdom == victim->pcdata->kingdom)
        {
            sprintf(mur, "%s has murdered his own kingdom mate %s from %s. What such horror.\n\r",
                ch->pcdata->switchname, victim->pcdata->switchname,
                kingdom_table[ch->pcdata->kingdom].whoname);
            send_to_char( "You unwieldly bastard. Your kingdom has been penalized!\n\r",ch);
            kingdom_table[ch->pcdata->kingdom].qps -= number_range(1000,kingdom_table[ch->pcdata->kingdom].qps/10);
            if (kingdom_table[ch->pcdata->kingdom].qps < 0)
            {
                kingdom_table[ch->pcdata->kingdom].qps = 0;
            }
            do_info(NULL,mur);
        }
    }
    save_kingdoms();
    do_asave(NULL,"world");
    return;
}

void decap_message(CHAR_DATA *ch, CHAR_DATA *victim)
{
    OBJ_DATA  *obj;
    char buf[MAX_STRING_LENGTH];
    bool unarmed = FALSE;

    if (IS_NPC(ch) || IS_NPC(victim)) return;
    if ( (obj = get_eq_char( ch, WEAR_WIELD )) == NULL )
    {
        if (( obj = get_eq_char( ch, WEAR_HOLD )) == NULL )
        {
            unarmed = TRUE;
        }
    }
    if ((obj != NULL && obj->item_type != ITEM_WEAPON)) unarmed = TRUE;

    /* The players own decap message */
    if (IS_SET(ch->pcdata->jflags, JFLAG_SETDECAP))
    {
        special_decap_message(ch,victim);
        return;
    }

    if (unarmed)
    {
        if (victim->sex == SEX_MALE)    sprintf(buf,"#P%s #ygot his head torn off by #R%s#n",victim->pcdata->switchname,ch->pcdata->switchname);
        else if (victim->sex == SEX_FEMALE) sprintf(buf,"#P%s #ygot her head torn off by #R%s#n",victim->pcdata->switchname,ch->pcdata->switchname);
        else                sprintf(buf,"#P%s #ygot its head torn off by #R%s#n",victim->pcdata->switchname,ch->pcdata->switchname);
    }
    else if (obj->value[3] == 1)
    {
        if (victim->sex == SEX_MALE)    sprintf(buf,"#P%s #ygot his head sliced off by #R%s#n",victim->pcdata->switchname,ch->pcdata->switchname);
        else if (victim->sex == SEX_FEMALE) sprintf(buf,"#P%s #ygot her head sliced off by #R%s#n",victim->pcdata->switchname,ch->pcdata->switchname);
        else                sprintf(buf,"#P%s #ygot its head sliced off by #R%s#n",victim->pcdata->switchname,ch->pcdata->switchname);
    }
    else if (obj->value[3] == 2)
    {
        if (victim->sex == SEX_MALE)    sprintf(buf,"#P%s #ygot his heart stabbed through by #R%s#n",victim->pcdata->switchname,ch->pcdata->switchname);
        else if (victim->sex == SEX_FEMALE) sprintf(buf,"#P%s #ygot her heart stabbed through by #R%s#n",victim->pcdata->switchname,ch->pcdata->switchname);
        else                sprintf(buf,"#P%s #ygot its heart stabbed through by #R%s#n",victim->pcdata->switchname,ch->pcdata->switchname);
    }
    else if (obj->value[3] == 3)
    {
        if (victim->sex == SEX_MALE)    sprintf(buf,"#P%s #ygot his head slashed off by #R%s#n",victim->pcdata->switchname,ch->pcdata->switchname);
        else if (victim->sex == SEX_FEMALE) sprintf(buf,"#P%s #ygot her head slashed off by #R%s#n",victim->pcdata->switchname,ch->pcdata->switchname);
        else                sprintf(buf,"#P%s #ygot its head slashed off by #R%s#n",victim->pcdata->switchname,ch->pcdata->switchname);
    }
    else if (obj->value[3] == 4) sprintf(buf,"#P%s #ygot strangled by #R%s",victim->pcdata->switchname,ch->pcdata->switchname);
    else if (obj->value[3] == 5) sprintf(buf,"#R%s #yruns a clawed hand through #P%s #yand pulls out the heart#n",ch->pcdata->switchname,victim->pcdata->switchname);
    else if (obj->value[3] == 6) sprintf(buf,"#R%s #yshoots #P%s #yseveral times and spits on the corpse#n",ch->pcdata->switchname,victim->pcdata->switchname);
    else if (obj->value[3] == 7) sprintf(buf,"#R%s #ypounds #P%s #yon the head and the skull caves in#n",ch->pcdata->switchname,victim->pcdata->switchname);
    else if (obj->value[3] == 8) sprintf(buf,"#R%s #ycrushes #P%s #yto a bloody pulp#n",ch->pcdata->switchname,victim->pcdata->switchname);
    else if (obj->value[3] == 9) sprintf(buf,"#P%s #yhas been grepped by #R%s#y, that's just mean!#n",victim->pcdata->switchname,ch->pcdata->switchname);
    else if (obj->value[3] == 10) sprintf(buf,"#P%s #ywas bitten to death by #R%s#n",victim->pcdata->switchname,ch->pcdata->switchname);
    else if (obj->value[3] == 11) sprintf(buf,"#R%s #yhas punctured the lungs of #P%s#y, what a meanie!#n",ch->pcdata->switchname,victim->pcdata->switchname);
    else if (obj->value[3] == 12) sprintf(buf,"#R%s #ygrabs #P%s #yby the head and sucks the brain out#n",ch->pcdata->switchname,victim->pcdata->switchname);
    else
    {
        if (victim->sex == SEX_MALE)    sprintf(buf,"#P%s #ygot his head sliced off by #R%s#n",victim->pcdata->switchname,ch->pcdata->switchname);
        else if (victim->sex == SEX_FEMALE) sprintf(buf,"#P%s #ygot her head sliced off by #R%s#n",victim->pcdata->switchname,ch->pcdata->switchname);
        else                sprintf(buf,"#P%s #ygot its head sliced off by #R%s#n",victim->pcdata->switchname,ch->pcdata->switchname);
    }
    death_info(buf);
    return;
}

void do_crack( CHAR_DATA *ch, char *argument )
{
    OBJ_DATA  *obj;
    OBJ_DATA  *right;
    OBJ_DATA  *left;

    right = get_eq_char( ch, WEAR_WIELD );
    left = get_eq_char( ch, WEAR_HOLD );
    if (right != NULL && right->pIndexData->vnum == 12) obj = right;
    else if (left != NULL && left->pIndexData->vnum == 12) obj = left;
    else
    {
        send_to_char("You are not holding any heads.\n\r",ch);
        return;
    }
    act("You hurl $p at the floor.", ch, obj, NULL, TO_CHAR);
    act("$n hurls $p at the floor.", ch, obj, NULL, TO_ROOM);
    act("$p cracks open, leaking brains out across the floor.", ch, obj, NULL, TO_CHAR);
    if (obj->chobj != NULL)
    {
        act("$p cracks open, leaking brains out across the floor.", ch, obj, obj->chobj, TO_NOTVICT);
        act("$p crack open, leaking brains out across the floor.", ch, obj, obj->chobj, TO_VICT);
    }
    else
    {
        act("$p cracks open, leaking brains out across the floor.", ch, obj, NULL, TO_ROOM);
    }
    crack_head(ch,obj,obj->name);
    obj_from_char(obj);
    extract_obj(obj);
}

void crack_head( CHAR_DATA *ch, OBJ_DATA *obj, char *argument )
{
    CHAR_DATA *victim;
    MOB_INDEX_DATA *pMobIndex;

    char arg1 [MAX_INPUT_LENGTH];
    char arg2 [MAX_INPUT_LENGTH];
    char buf [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
    if (str_cmp(arg2,"mob") && obj->chobj != NULL && !IS_NPC(obj->chobj) && IS_AFFECTED(obj->chobj,AFF_POLYMORPH))
    {
        victim = obj->chobj;
        make_part(victim,"cracked_head");
        make_part(victim,"brain");
        sprintf(buf,"the quivering brain of %s",victim->name);
        free_string(victim->morph);
        victim->morph = str_dup(buf);
        return;
    }
    else if (!str_cmp(arg2,"mob"))
    {
        if ( ( pMobIndex = get_mob_index( obj->value[1] ) ) == NULL ) return;
        victim = create_mobile( pMobIndex );
        char_to_room(victim,ch->in_room);
        make_part(victim,"cracked_head");
        make_part(victim,"brain");
        extract_char(victim,TRUE);
        return;
    }
    else
    {
        if ( ( pMobIndex = get_mob_index( 30002 ) ) == NULL ) return;
        victim = create_mobile( pMobIndex );
        sprintf( buf, capitalize(arg2) );
        free_string( victim->short_descr );
        victim->short_descr = str_dup( buf );
        char_to_room(victim,ch->in_room);
        make_part(victim,"cracked_head");
        make_part(victim,"brain");
        extract_char(victim,TRUE);
        return;
    }
    return;
}

/* Voodoo skill by KaVir */

void do_voodoo( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    OBJ_DATA  *obj;
    char arg1 [MAX_INPUT_LENGTH];
    char arg2 [MAX_INPUT_LENGTH];
    char buf [MAX_INPUT_LENGTH];
    char part1 [MAX_INPUT_LENGTH];
    char part2 [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
    if ( arg1[0] == '\0')
    {
        send_to_char( "Who do you wish to use voodoo magic on?\n\r", ch );
        return;
    }
    if ( ( obj = get_eq_char( ch, WEAR_HOLD ) ) == NULL )
    {
        send_to_char( "You are not holding a voodoo doll.\n\r", ch );
        return;
    }
    if ( ( victim = get_char_world( ch, arg1 ) ) == NULL )
    {
        send_to_char( "They are not here.\n\r", ch );
        return;
    }
    if ( IS_NPC(victim) )
    {
        send_to_char( "Not on NPC's.\n\r", ch );
        return;
    }
    sprintf(part2,obj->name);
    sprintf(part1,"%s voodoo doll",victim->name);
    if ( str_cmp(part1,part2) )
    {
        sprintf(buf,"But you are holding %s, not %s!\n\r",obj->short_descr,victim->name);
        send_to_char( buf, ch );
        return;
    }
    if ( arg2[0] == '\0')
        send_to_char( "You can 'stab', 'burn' or 'throw' the doll.\n\r", ch );
    else if ( !str_cmp(arg2, "stab") )
    {
        WAIT_STATE(ch,12);
        act("You stab a pin through $p.", ch, obj, NULL, TO_CHAR);
        act("$n stabs a pin through $p.", ch, obj, NULL, TO_ROOM);
        if (!IS_NPC(victim) && IS_IMMUNE(victim, IMM_VOODOO)) return;
        act("You feel an agonising pain in your chest!", victim, NULL, NULL, TO_CHAR);
        act("$n clutches $s chest in agony!", victim, NULL, NULL, TO_ROOM);
    }
    else if ( !str_cmp(arg2, "burn") )
    {
        WAIT_STATE(ch,12);
        act("You set fire to $p.", ch, obj, NULL, TO_CHAR);
        act("$n sets fire to $p.", ch, obj, NULL, TO_ROOM);
        act("$p burns to ashes.", ch, obj, NULL, TO_CHAR);
        act("$p burns to ashes.", ch, obj, NULL, TO_ROOM);
        obj_from_char(obj);
        extract_obj(obj);
        if (!IS_NPC(victim) && IS_IMMUNE(victim, IMM_VOODOO)) return;
        if (IS_AFFECTED(victim,AFF_FLAMING) )
            return;
        SET_BIT(victim->affected_by, AFF_FLAMING);
        act("You suddenly burst into flames!", victim, NULL, NULL, TO_CHAR);
        act("$n suddenly bursts into flames!", victim, NULL, NULL, TO_ROOM);
    }
    else if ( !str_cmp(arg2, "throw") )
    {
        WAIT_STATE(ch,12);
        act("You throw $p to the ground.", ch, obj, NULL, TO_CHAR);
        act("$n throws $p to the ground.", ch, obj, NULL, TO_ROOM);
        obj_from_char(obj);
        obj_to_room(obj, ch->in_room);
        if (!IS_NPC(victim) && IS_IMMUNE(victim, IMM_VOODOO)) return;
        if (victim->position < POS_STANDING) return;
        if (victim->position == POS_FIGHTING)
            stop_fighting(victim, TRUE);
        act("A strange force picks you up and hurls you to the ground!", victim, NULL, NULL, TO_CHAR);
        act("$n is hurled to the ground by a strange force.", victim, NULL, NULL, TO_ROOM);
        victim->position = POS_RESTING;
        victim->hit = victim->hit - number_range(ch->level,(5*ch->level));
        update_pos(victim);
        if (victim->position == POS_DEAD && !IS_NPC(victim))
        {
            do_killperson(ch,victim->name);
            return;
        }
    }
    else
        send_to_char( "You can 'stab', 'burn' or 'throw' the doll.\n\r", ch );
    return;
}

void do_garotte(CHAR_DATA *ch, char *argument)
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    int chance;

    one_argument(argument, arg);
    chance = number_percent();

    if (IS_NPC(ch)) return;
    if (!IS_CLASS(ch, CLASS_DROW) || !IS_SET(ch->pcdata->powers[1], DPOWER_GAROTTE))
    {
        send_to_char("Huh?\n\r", ch );
        return;
    }
    if ((victim = get_char_room(ch, arg)) == NULL)
    {
        send_to_char("Garotte whom?\n\r", ch );
        return;
    }
    if (victim == ch)
    {
        send_to_char("That would be a bad idea.\n\r", ch );
        return;
    }
    WAIT_STATE(ch, 12);
    one_hit(ch, victim, gsn_garotte,1);
    one_hit(ch, victim, gsn_garotte,1);
    one_hit(ch, victim, gsn_garotte,1);
    one_hit(ch, victim, gsn_garotte,1);
    return;
}

void do_dark_garotte(CHAR_DATA *ch, char *argument)
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    int chance;

    one_argument(argument, arg);
    chance = number_percent();
    if (IS_NPC(ch)) return;
    if (ch->in_room != NULL)
    {
        if (IS_SET (ch->in_room->room_flags, ROOM_ARENA))
        {
            send_to_char("Your in the arena.\n\r",ch);
            return;
        }
    }
    if (!IS_CLASS(ch, CLASS_DROW) || !IS_SET(ch->pcdata->powers[1], DPOWER_GAROTTE)
        || !IS_SET(ch->pcdata->powers[1], DPOWER_DGAROTTE))
    {
        send_to_char("Huh?\n\r", ch );
        return;
    }
    if (!IS_SET(ch->newbits, NEW_DARKNESS))
    {
        send_to_char("You have to be surrounded in darkness.\n\r", ch );
        return;
    }
    if ((victim = get_char_room(ch, arg)) == NULL)
    {
        send_to_char("Dark Garotte whom?\n\r", ch );
        return;
    }
    if (victim == ch)
    {
        send_to_char("That would be a bad idea.\n\r", ch );
        return;
    }
    if (IS_NPC(victim) && victim->level>2000)
    {
        send_to_char("That would be a bad idea, too big mob.\n\r", ch );
        return;
    }
    if ( victim->fighting != NULL )
    {
        send_to_char( "You can't garotte a fighting person.\n\r", ch );
        return;
    }
    if (victim->hit < victim->max_hit)
    {
        act("$N is too hurt for you to sneak up.", ch, NULL,victim,TO_CHAR);
        return;
    }
    WAIT_STATE(ch, skill_table[gsn_garotte].beats);
    REMOVE_BIT(ch->newbits, NEW_DARKNESS);
    REMOVE_BIT(ch->in_room->room_flags, ROOM_TOTAL_DARKNESS);
    if ( chance > 95 )
    {
        act("$n wraps a whip around $N's neck! $N is choking to death!",ch,NULL,victim,TO_NOTVICT);
        act("You wrap your whip around $N's neck and refuse to let go!",ch,NULL,victim,TO_CHAR);
        act("$n wraps his whip around your neck, and you can't break free!",ch,NULL,victim,TO_VICT);
        one_hit(ch, victim, gsn_garotte,1);
        one_hit(ch, victim, gsn_garotte,1);
        one_hit(ch, victim, gsn_garotte,1);
        one_hit(ch, victim, gsn_garotte,1);
        one_hit(ch, victim, gsn_garotte,1);
        one_hit(ch, victim, gsn_garotte,1);
    }
    else
    {
        one_hit(ch, victim, gsn_garotte,1);
        one_hit(ch, victim, gsn_garotte,1);
        one_hit(ch, victim, gsn_garotte,1);
    }
    return;
}

bool has_timer(CHAR_DATA *ch)
{
    if (ch->fight_timer > 0 && !IS_NPC(ch))
    {
        send_to_char("Not until your fight timer runs out!\n\r", ch );
        return TRUE;
    }
    return FALSE;
}

void do_circle( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    OBJ_DATA *obj;

    one_argument( argument, arg );
    if (IS_NPC(ch)) return;
    if (!IS_CLASS(ch, CLASS_NINJA) || ch->pcdata->powers[NPOWER_NINGENNO] < 6)
    {
        send_to_char("Huh?\n\r", ch );
        return;
    }
    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
        if (ch->fighting == NULL)
        {
            send_to_char( "They aren't here.\n\r", ch );
            return;
        }
        else victim = ch->fighting;
    }
    if ( victim == ch )
    {
        send_to_char( "How can you circle yourself?\n\r", ch );
        return;
    }
    if (is_safe( ch, victim )) return;
    if ((( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL || obj->value[3] != 11)
        && ((obj = get_eq_char( ch, WEAR_HOLD ) ) == NULL || obj->value[3] != 11))
    {
        send_to_char( "You need to wield a piercing weapon.\n\r", ch );
        return;
    }
    one_hit(ch, victim, gsn_circle, 1);
    if (number_range(1,4) == 2)
    {
        send_to_char("You spin around once more, scoring a second hit.\n\r",ch);
        one_hit(ch, victim, gsn_circle, 1);
    }
    WAIT_STATE( ch, 8 );
    return;
}

void do_autostance(CHAR_DATA *ch, char *argument )
{
    char arg [MAX_INPUT_LENGTH];

    argument = one_argument(argument,arg);
    if (IS_NPC(ch)) return;
    if (!str_cmp(arg,"none"))
    {
        send_to_char("You no longer autostance.\n\r", ch );
        ch->stance[MONK_AUTODROP] = STANCE_NONE;
    }
    else if (!str_cmp(arg, "crane"))
    {
        send_to_char("You now autostance into the crane stance.\n\r",ch );
        ch->stance[MONK_AUTODROP] = STANCE_CRANE;
    }
    else if (!str_cmp(arg, "crab"))
    {
        send_to_char("You now autostance into the crab stance.\n\r", ch );
        ch->stance[MONK_AUTODROP] = STANCE_CRAB;
    }
    else if (!str_cmp(arg, "bull"))
    {
        send_to_char("You now autostance into the bull stance.\n\r", ch );
        ch->stance[MONK_AUTODROP] = STANCE_BULL;
    }
    else if (!str_cmp(arg, "viper"))
    {
        send_to_char("You now autostance into the viper stance.\n\r", ch );
        ch->stance[MONK_AUTODROP] = STANCE_VIPER;
    }
    else if (!str_cmp(arg, "mongoose"))
    {
        send_to_char("You now autostance into the mongoose stance.\n\r", ch);
        ch->stance[MONK_AUTODROP] = STANCE_MONGOOSE;
    }
    else if (!str_cmp(arg, "mantis") && ch->stance[STANCE_CRANE] >= 200 && ch->stance[STANCE_VIPER] >= 200)
    {
        send_to_char("You now autostance into the mantis stance.\n\r", ch);
        ch->stance[MONK_AUTODROP] = STANCE_MANTIS;
    }
    else if (!str_cmp(arg, "monkey") && ch->stance[STANCE_CRANE] >= 200 && ch->stance[STANCE_MONGOOSE] >= 200)
    {
        send_to_char("You now autostance into the monkey stance.\n\r", ch);
        ch->stance[MONK_AUTODROP] = STANCE_MONKEY;
    }
    else if (!str_cmp(arg, "swallow") && ch->stance[STANCE_CRAB] >= 200 && ch->stance[STANCE_MONGOOSE] >= 200)
    {
        send_to_char("You now autostance into the swallow stance.\n\r", ch);
        ch->stance[MONK_AUTODROP] = STANCE_SWALLOW;
    }
    else if (!str_cmp(arg, "tiger") && ch->stance[STANCE_BULL] >= 200 && ch->stance[STANCE_VIPER] >= 200)
    {
        send_to_char("You now autostance into the tiger stance.\n\r", ch);
        ch->stance[MONK_AUTODROP] = STANCE_TIGER;
    }
    else if (!str_cmp(arg, "dragon") && ch->stance[STANCE_CRAB] >= 200 && ch->stance[STANCE_BULL] >= 200)
    {
        send_to_char("You now autostance into the dragon stance.\n\r", ch);
        ch->stance[MONK_AUTODROP] = STANCE_DRAGON;
    }
    else if (!str_cmp(arg, "wolf") && (IS_CLASS(ch, CLASS_WEREWOLF)) && ch->stance[STANCE_TIGER] >= 200 && ch->stance[STANCE_SWALLOW] >= 200 && ch->stance[STANCE_MONKEY] >= 200 && ch->stance[STANCE_MANTIS] >= 200 && ch->stance[STANCE_DRAGON] >= 200)
    {
        send_to_char("You now autostance into the wolf stance.\n\r", ch);
        ch->stance[MONK_AUTODROP] = STANCE_WOLF;
    }
    else if (!str_cmp(arg, "ss1") && ch->stance[19] != -1)
    {
        send_to_char("You now autostance into superstance one.\n\r", ch);
        ch->stance[MONK_AUTODROP] = STANCE_SS1;
    }
    else if (!str_cmp(arg, "ss2") && ch->stance[20] != -1)
    {
        send_to_char("You now autostance into superstance two.\n\r", ch);
        ch->stance[MONK_AUTODROP] = STANCE_SS2;
    }
    else if (!str_cmp(arg, "ss3") && ch->stance[21] != -1)
    {
        send_to_char("You now autostance into superstance three.\n\r", ch);
        ch->stance[MONK_AUTODROP] = STANCE_SS3;
    }
    else if (!str_cmp(arg, "ss4") && ch->stance[22] != -1)
    {
        send_to_char("You now autostance into superstance four.\n\r", ch);
        ch->stance[MONK_AUTODROP] = STANCE_SS4;
    }
    else if (!str_cmp(arg, "ss5") && ch->stance[23] != -1)
    {
        send_to_char("You now autostance into superstance five.\n\r", ch);
        ch->stance[MONK_AUTODROP] = STANCE_SS5;
    }
    else send_to_char("You can't set your autostance to that!\n\r",ch);
}

void autodrop(CHAR_DATA *ch)
{
    char buf [MAX_INPUT_LENGTH];
    char buf2 [MAX_INPUT_LENGTH];
    char stancename [10];

    if (IS_NPC(ch)) return;
    if (ch->stance[MONK_AUTODROP]==STANCE_NONE) return;
    if (ch->stance[MONK_AUTODROP]==STANCE_VIPER) sprintf(stancename,"viper");
    else if (ch->stance[MONK_AUTODROP]==STANCE_CRANE) sprintf(stancename,"crane");
    else if (ch->stance[MONK_AUTODROP]==STANCE_CRAB) sprintf(stancename,"crab");
    else if (ch->stance[MONK_AUTODROP]==STANCE_MONGOOSE) sprintf(stancename,"mongoose");
    else if (ch->stance[MONK_AUTODROP]==STANCE_BULL) sprintf(stancename,"bull");
    else if (ch->stance[MONK_AUTODROP]==STANCE_MANTIS) sprintf(stancename,"mantis");
    else if (ch->stance[MONK_AUTODROP]==STANCE_DRAGON) sprintf(stancename,"dragon");
    else if (ch->stance[MONK_AUTODROP]==STANCE_TIGER) sprintf(stancename,"tiger");
    else if (ch->stance[MONK_AUTODROP]==STANCE_MONKEY) sprintf(stancename,"monkey");
    else if (ch->stance[MONK_AUTODROP]==STANCE_SWALLOW) sprintf(stancename,"swallow");
    else if (ch->stance[MONK_AUTODROP]==STANCE_SS1) sprintf(stancename,"ss1");
    else if (ch->stance[MONK_AUTODROP]==STANCE_SS2) sprintf(stancename,"ss2");
    else if (ch->stance[MONK_AUTODROP]==STANCE_SS3) sprintf(stancename,"ss3");
    else if (ch->stance[MONK_AUTODROP]==STANCE_SS4) sprintf(stancename,"ss4");
    else if (ch->stance[MONK_AUTODROP]==STANCE_SS5) sprintf(stancename,"ss5");
    else if (ch->stance[MONK_AUTODROP]) sprintf(stancename,"wolf");
    else return;
    if (ch->stance[0] < 1)
    {
        ch->stance[0] = ch->stance[MONK_AUTODROP];;
        sprintf(buf, "#7You autodrop into the #3%s#7 stance.", stancename);
        act(buf, ch, NULL, NULL, TO_CHAR);
        sprintf(buf2, "#7$n autodrops into the #3%s#7 stance.",stancename);
        act(buf2, ch, NULL, NULL, TO_ROOM);
    }
}

void dropinvis(CHAR_DATA *ch)
{
    if (ch->level < 7 && IS_SET(ch->act, AFF_HIDE)) REMOVE_BIT(ch->act, AFF_HIDE);
    if (ch->level < 7 && IS_SET(ch->act, PLR_WIZINVIS)) REMOVE_BIT(ch->act, PLR_WIZINVIS);
    if (IS_SET(ch->act,PLR_SUPERINVIS) )
        REMOVE_BIT(ch->act,PLR_SUPERINVIS);
    if ((IS_CLASS(ch, CLASS_DROW) || IS_CLASS(ch, CLASS_DROID)) && IS_SET(ch->newbits, NEW_DARKNESS))
    {
        REMOVE_BIT(ch->newbits, NEW_DARKNESS);
        REMOVE_BIT(ch->in_room->room_flags, ROOM_TOTAL_DARKNESS);
    }
    if ((IS_CLASS(ch,CLASS_VAMPIRE) || IS_CLASS(ch, CLASS_WEREWOLF) || IS_CLASS(ch, CLASS_ELF)
        || IS_CLASS(ch, CLASS_NINJA) || IS_CLASS(ch, CLASS_MONK) ) && IS_SET(ch->act, PLR_SUPERINVIS) )
        REMOVE_BIT(ch->act,PLR_SUPERINVIS);
    return;
}

void ragnarokdecap(CHAR_DATA *ch, CHAR_DATA *victim)
{
    char buf[MAX_STRING_LENGTH];

    if (IS_CLASS(victim, CLASS_VAMPIRE))       do_mortalvamp(victim,"");
    else if (IS_SET(victim->special, SPC_WOLFMAN)) do_unwerewolf(victim,"");
    char_from_room(victim);
    char_to_room(victim,get_room_index(ROOM_VNUM_ALTAR));
    do_clearstats(victim,"");
    victim->level = 2;
    dropinvis(ch);
    players_decap++;
    sprintf(buf,"%s was beheaded by %s, the appocalypse continues",victim->name, ch->name);
    do_info(ch,buf);
    send_to_char("YOU HAVE BEEN KILLED!!!!\n\r",victim);
    do_call(victim, "all");
    do_train(victim,"avatar");
    do_restore(victim,"self");
    return;
}

/*
 * New system to replace status, called fair fight, it measures the
 * difference between two players, giving them points for their
 * stances, powers, and stats. If they are within each others range,
 * the call will return TRUE, if not FALSE. Call for fair_fight when
 * you need to see if a fight is fair (ie. decapping).
 */
bool fair_fight(CHAR_DATA *ch, CHAR_DATA *victim)
{
    //  int iAggr, iDef;

    if (IS_NPC(ch) || IS_NPC(victim)) return TRUE;

    /*
     * All the people that shouldn't be fighting anyway
     */
    if (!str_cmp(victim->pcdata->last_decap[0],ch->name) || !str_cmp(victim->pcdata->last_decap[1],ch->name) ) return TRUE;
    if ((get_age(ch) - 17) < 5 && ch->pkill == 0) return FALSE;
    if ((get_age(victim) - 17) < 5 && victim->pkill == 0) return FALSE;
    if (ch == victim) return FALSE;
    if (ch->level != 3 || victim->level != 3) return FALSE;
    if (reachedDecapLimit(ch)) return FALSE;

    //  iAggr = getMight(ch);
    //  iDef  = getMight(victim);

    /*
     * To weak to fight, should be protected.
     */
    //  if (iDef < 150 || iAggr < 150) return FALSE;

    /*
     * Trying to scale the range depending on the size of the player,
     * allowing larger players a larger range, since theres fewer large players.
     */
    /*  if (iAggr < 1000)
      {
        if (iAggr * 0.77 > iDef) return FALSE;
      }
      else
      {
        if (iAggr * 0.72 > iDef) return FALSE;
      }

      if ( iAggr > iDef )
        if ( (iAggr * .7 ) > iDef ) return FALSE;
      if ( iAggr < iDef )
        if ( ( iAggr*1.3 ) < iDef ) return FALSE;

    if ( (ch->race > 0) && (victim->race <= 0) ) return FALSE;
    if ( ch->pkill > 0 && victim->pkill == 0 ) return FALSE;
    if ( ( (ch->pkill - ch->pdeath ) - 3) > (victim->pkill - victim->pdeath ) ) return FALSE;
    if ( ( (ch->pkill - ch->pdeath ) + 3) < (victim->pkill - victim->pdeath ) ) return FALSE;
    */
    if ( (  ch->race  > 0 ) && (  victim->race  <= 0 ) ) return FALSE;
    if (  victim->race  >  ch->race  )
        if ( ( victim->race - ch->race ) > 4 ) return FALSE;
    if (  victim->race  <  ch->race )
        if ( (  ch->race  -  victim->race ) > 4 ) return FALSE;
    /*
     * They passed the test, FIGHT children.
     */
    return TRUE;
}

bool kingdom_fair_fight(CHAR_DATA *ch, CHAR_DATA *victim)
{
    int nBaseCh = kingdom_table[ch->pcdata->kingdom].kills-kingdom_table[ch->pcdata->kingdom].deaths;
    int nBaseVict = kingdom_table[victim->pcdata->kingdom].kills-kingdom_table[victim->pcdata->kingdom].deaths;
    int nRange = 20;
    if ( IS_NPC(ch) || IS_NPC(victim) ) return FALSE;
    if (ch->pcdata->kingdom == 0 || victim->pcdata->kingdom == 0) return FALSE;
    if (nBaseCh > nBaseVict )
        if ( nBaseCh > ( nBaseVict + nRange ) ) return FALSE;
    if (nBaseCh < nBaseVict )
        if ( (nBaseCh + nRange) < nBaseVict ) return FALSE;
    return TRUE;
}
