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

/*
 * Local functions.
 */
#define CD CHAR_DATA
void    get_obj     args( ( CHAR_DATA *ch, OBJ_DATA *obj,
OBJ_DATA *container ) );
CD *    find_keeper args( ( CHAR_DATA *ch ) );
int get_cost    args( ( CHAR_DATA *keeper, OBJ_DATA *obj, bool fBuy ) );
void    sheath      args( ( CHAR_DATA *ch, bool right ) );
void    draw        args( ( CHAR_DATA *ch, bool right ) );
void    call_all    args( ( CHAR_DATA *ch ) );
#undef  CD

void do_connect( CHAR_DATA *ch, char *argument )
{
    OBJ_DATA * first_ob;
    OBJ_DATA * second_ob;
    OBJ_DATA * new_ob;

    char arg1[MAX_STRING_LENGTH], arg2[MAX_STRING_LENGTH];
    char buf[MAX_STRING_LENGTH];
    int fuckup;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if ( arg1[0] == '\0' || arg2[0] == '\0' )
    {
        send_to_char( "Syntax: Connect <firstobj> <secondobj>.\n\r", ch );
        return;
    }

    if ( ( first_ob =  get_obj_carry( ch, arg1 ) )  == NULL )
    {
        sprintf( buf, "You must be holding both parts!!\n\r" );
        send_to_char( buf, ch );
        return;
    }

    if ( ( second_ob =  get_obj_carry( ch, arg2 ) )  == NULL )
    {
        sprintf( buf, "You must be holding both parts!!\n\r" );
        send_to_char( buf, ch );
        return;
    }

    if ( first_ob->item_type != ITEM_PIECE || second_ob->item_type !=ITEM_PIECE )
    {
        send_to_char( "Both items must be pieces of another item!\n\r", ch );
        return;
    }

    if ( number_range(1,3) == 1)                            //FUCKUP CODE BY DERT
    {
        fuckup = number_range(1,3);
        if ( fuckup == 3)
        {
            extract_obj( first_ob );
            extract_obj( second_ob );
            act( "$n tries to fit two pieces together but breaks them.", ch, NULL, NULL, TO_ROOM);
            stc("Through your lack of skill you manage to break both pieces.\n\r",ch);
            return;
        }
        else if ( fuckup == 1)
        {
            extract_obj( first_ob );
            act( "$n tries to fit two pieces together but breaks one of them.", ch, NULL, NULL, TO_ROOM);
            stc("Through your lack of skill you manage to break one of the pieces.\n\r",ch);
            return;
        }
        else if ( fuckup == 2)
        {
            extract_obj( second_ob );
            act( "$n tries to fit two pieces together but breaks one of them.", ch, NULL, NULL, TO_ROOM);
            stc("Through your lack of skill you manage to break one of the pieces.\n\r",ch);
            return;
        }
    }
    if ( first_ob == second_ob )
    {
        stc("You try helplessly as you twirl the same item together, trying to figure out how the piece fits into itself...\n\r",ch);
        return;
    }

    /* check to see if the pieces connect */
    if (  ( first_ob->value[0] == second_ob->pIndexData->vnum )
        || ( first_ob->value[1] == second_ob->pIndexData->vnum )  )
        /* good connection  */
    {
        new_ob = create_object( get_obj_index( first_ob->value[2] ), ch->level );
        extract_obj( first_ob );
        extract_obj( second_ob );
        obj_to_char( new_ob , ch );
        if ( IS_SET(new_ob->extra_flags,ITEM_AUTOENGRAVE) && !IS_SET(new_ob->extra_flags,ITEM_ENGRAVED))
        {
            REMOVE_BIT(new_ob->extra_flags,ITEM_AUTOENGRAVE);
            SET_BIT(new_ob->extra_flags,ITEM_ENGRAVED);
            new_ob->questowner = str_dup(ch->pcdata->switchname);
        }
        act( "$n jiggles some pieces together...\r\n ...suddenly they snap in place, creating $p!", ch, new_ob,NULL,  TO_ROOM );
        act( "You jiggle the pieces together...\r\n ...suddenly they snap into place, creating $p!",  ch, new_ob, NULL, TO_CHAR);

    }
    else
    {
        act( "$n tries to fit two pieces together but breaks them.", ch, NULL, NULL, TO_ROOM);
        act( "You try to fit them together every which way, but they just don't want to fit together. The pieces are destroyed through your effort.", ch, NULL, NULL, TO_CHAR);
        extract_obj( first_ob );
        extract_obj( second_ob );

        return;
    }

    return;
}

void do_call( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    OBJ_DATA *in_obj;
    CHAR_DATA *gch;
    CHAR_DATA *victim = NULL;
    ROOM_INDEX_DATA *chroom;
    ROOM_INDEX_DATA *objroom;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
        send_to_char( "What object do you wish to call?\n\r", ch );
        return;
    }

    if (IS_NPC(ch)) {send_to_char("Not while switched.\n\r",ch); return;}

    if (!IS_HEAD(ch, LOST_HEAD))
    {
        act("Your eyes flicker with yellow energy.",ch,NULL,NULL,TO_CHAR);
        act("$n's eyes flicker with yellow energy.",ch,NULL,NULL,TO_ROOM);
    }

    if (!str_cmp(arg,"all"))
    {
        call_all(ch);
        return;
    }

    if ( ( obj = get_obj_world( ch, arg ) ) == NULL )
    {
        send_to_char( "Nothing like that in hell, earth, or heaven.\n\r", ch );
        return;
    }

    if (obj->questowner == NULL || strlen(obj->questowner) < 2 ||
        str_cmp(obj->questowner,ch->pcdata->switchname) || obj->item_type == ITEM_PAGE)
    {
        send_to_char( "Nothing happens.\n\r", ch );
        return;
    }

    for ( in_obj = obj; in_obj->in_obj != NULL; in_obj = in_obj->in_obj )
        ;

    if (in_obj->carried_by != NULL)
    {
        if (in_obj->carried_by == ch) return;
        if ((gch = in_obj->carried_by) != NULL)
        {
            if (gch->desc && gch->desc->connected != CON_PLAYING) return;
        }
    }

    if (obj->carried_by != NULL && obj->carried_by != ch)
    {
        victim = obj->carried_by;
        if (!IS_NPC(victim) && victim->desc != NULL && victim->desc->connected != CON_PLAYING) return;
        act("$p suddenly vanishes from your hands!",victim,obj,NULL,TO_CHAR);
        act("$p suddenly vanishes from $n's hands!",victim,obj,NULL,TO_ROOM);
        obj_from_char(obj);
    }
    else if (obj->in_room != NULL)
    {
        chroom = ch->in_room;
        objroom = obj->in_room;
        char_from_room(ch);
        char_to_room(ch,objroom);
        act("$p vanishes from the ground!",ch,obj,NULL,TO_ROOM);
        if (chroom == objroom) act("$p vanishes from the ground!",ch,obj,NULL,TO_CHAR);
        char_from_room(ch);
        char_to_room(ch,chroom);
        obj_from_room(obj);
    }
    else if (obj->in_obj != NULL) obj_from_obj(obj);
    else
    {
        if (!IS_HEAD(ch, LOST_HEAD))
            send_to_char( "Nothing happens.\n\r", ch );
        return;
    }

    obj_to_char(obj,ch);
    if (IS_SET(obj->extra_flags,ITEM_SHADOWPLANE))
        REMOVE_BIT(obj->extra_flags,ITEM_SHADOWPLANE);
    act("$p materializes in your hands.",ch,obj,NULL,TO_CHAR);
    act("$p materializes in $n's hands.",ch,obj,NULL,TO_ROOM);
    do_autosave(ch,"");
    if (victim != NULL) do_autosave(victim,"");
    return;
}

void call_all( CHAR_DATA *ch )
{
    CHAR_DATA *gch;
    OBJ_DATA *obj;
    OBJ_DATA *in_obj;
    CHAR_DATA *victim = NULL;
    DESCRIPTOR_DATA *d;
    ROOM_INDEX_DATA *chroom;
    ROOM_INDEX_DATA *objroom;
    bool found = FALSE;

    for ( obj = object_list; obj != NULL; obj = obj->next )
    {
        if ( obj->questowner == NULL || strlen(obj->questowner) < 2 ||
            str_cmp( ch->pcdata->switchname, obj->questowner ) || obj->item_type == ITEM_PAGE)
            continue;

        found = TRUE;

        for ( in_obj = obj; in_obj->in_obj != NULL; in_obj = in_obj->in_obj )
            ;

        if (in_obj->carried_by != NULL)
        {
            if (in_obj->carried_by == ch) continue;
            if ((gch = in_obj->carried_by) != NULL)
            {
                if (gch->desc && gch->desc->connected != CON_PLAYING) continue;
            }
        }

        if (obj->carried_by != NULL && obj->carried_by != ch)
        {
            if (obj->carried_by == ch || obj->carried_by->desc == NULL ||
                obj->carried_by->desc->connected != CON_PLAYING)
            {
                if (!IS_NPC(obj->carried_by)) return;
            }
            act("$p suddenly vanishes from your hands!",obj->carried_by,obj,NULL,TO_CHAR);
            act("$p suddenly vanishes from $n's hands!",obj->carried_by,obj,NULL,TO_ROOM);
            SET_BIT(obj->carried_by->extra, EXTRA_CALL_ALL);
            obj_from_char(obj);
        }
        else if (obj->in_room != NULL)
        {
            chroom = ch->in_room;
            objroom = obj->in_room;
            char_from_room(ch);
            char_to_room(ch,objroom);
            act("$p vanishes from the ground!",ch,obj,NULL,TO_ROOM);
            if (chroom == objroom) act("$p vanishes from the ground!",ch,obj,NULL,TO_CHAR);
            char_from_room(ch);
            char_to_room(ch,chroom);
            obj_from_room(obj);
        }
        else if (obj->in_obj != NULL) obj_from_obj(obj);
        else continue;
        obj_to_char(obj,ch);
        if (IS_SET(obj->extra_flags,ITEM_SHADOWPLANE))
            REMOVE_BIT(obj->extra_flags,ITEM_SHADOWPLANE);
        if (!IS_HEAD(ch, LOST_HEAD))
        {
            act("$p materializes in your hands.",ch,obj,NULL,TO_CHAR);
            act("$p materializes in $n's hands.",ch,obj,NULL,TO_ROOM);
        }
    }

    if ( !found && !IS_HEAD(ch, LOST_HEAD) )
        send_to_char( "Nothing happens.\n\r", ch );

    for ( d = descriptor_list; d != NULL; d = d->next )
    {
        if ( d->connected != CON_PLAYING ) continue;
        if ( (victim = d->character) == NULL ) continue;
        if ( IS_NPC(victim) ) continue;
        if ( ch != victim && !IS_EXTRA(victim,EXTRA_CALL_ALL) ) continue;
        REMOVE_BIT(victim->extra, EXTRA_CALL_ALL);
        do_autosave(victim,"");
    }
    return;
}

void get_obj( CHAR_DATA *ch, OBJ_DATA *obj, OBJ_DATA *container )
{
    OBJ_DATA *obj2;
    OBJ_DATA *obj_next;
    ROOM_INDEX_DATA *objroom;
    bool move_ch = FALSE;
    int iArtifacts = 0;

    /* Objects should only have a shadowplane flag when on the floor */
    if  ( IS_AFFECTED(ch,AFF_SHADOWPLANE) &&
        obj->in_room != NULL &&
        (!IS_SET(obj->extra_flags, ITEM_SHADOWPLANE) ) )
    {
        send_to_char( "Your hand passes right through it!\n\r", ch );
        return;
    }
    if  (!IS_AFFECTED(ch,AFF_SHADOWPLANE) &&
        obj->in_room != NULL &&
        ( IS_SET(obj->extra_flags, ITEM_SHADOWPLANE) ) )
    {
        send_to_char( "Your hand passes right through it!\n\r", ch );
        return;
    }

    /*Artifact checking*/
    if((!IS_IMMORTAL(ch)) && ((container == NULL) || ((container != NULL) && (container->carried_by != ch))))
    {
        if(IS_SET(obj->quest, QUEST_ARTIFACT))
        {
            iArtifacts += 1;
        }

        iArtifacts += recursive_artifact_count(obj);

        if((get_artifact_count(ch) + iArtifacts) > MAX_ARTIFACTS)
        {
            send_to_char("You may only possess three artifacts at any given time.\n\r", ch);
            return;
        }
    }

    if ( !CAN_WEAR(obj, ITEM_TAKE) )
    {
        send_to_char( "You can't take that.\n\r", ch );
        return;
    }

    if ( ch->carry_number + 1 > can_carry_n( ch ) )
    {
        act( "$d: you can't carry that many items.",
            ch, NULL, obj->name, TO_CHAR );
        return;
    }

    if ( ch->carry_weight + get_obj_weight( obj ) > can_carry_w(ch) )
    {
        act( "$d: you can't carry that much weight.",
            ch, NULL, obj->name, TO_CHAR );
        return;
    }

    if ( container != NULL )
    {

        if  ( IS_AFFECTED(ch,AFF_SHADOWPLANE) &&
            !IS_SET(container->extra_flags, ITEM_SHADOWPLANE) &&
            (container->carried_by == NULL || container->carried_by != ch) )
        {
            send_to_char( "Your hand passes right through it!\n\r", ch );
            return;
        }
        if  (!IS_AFFECTED(ch,AFF_SHADOWPLANE) &&
            IS_SET(container->extra_flags, ITEM_SHADOWPLANE) &&
            (container->carried_by == NULL || container->carried_by != ch) )
        {
            send_to_char( "Your hand passes right through it!\n\r", ch );
            return;
        }
        act( "You get $p from $P.", ch, obj, container, TO_CHAR );
        act( "$n gets $p from $P.", ch, obj, container, TO_ROOM );
        for ( obj2 = container->contains; obj2 != NULL; obj2 = obj_next )
        {
            obj_next = obj2->next_content;
            if ( obj2->chobj != NULL )
            {
                act( "A hand reaches inside $P and takes $p out.", obj2->chobj, obj, container, TO_CHAR );
                move_ch = TRUE;
            }
        }
        obj_from_obj( obj );
    }
    else
    {
        act( "You pick up $p.", ch, obj, container, TO_CHAR );
        act( "$n picks $p up.", ch, obj, container, TO_ROOM );
        if (obj != NULL) obj_from_room( obj );
    }

    if ( obj->item_type == ITEM_MONEY )
    {
        ch->gold += obj->value[0];
        extract_obj( obj );
    }
    else
    {
        obj_to_char( obj, ch );
        if ( move_ch && obj->chobj != NULL )
        {
            if (obj->carried_by != NULL && obj->carried_by != obj->chobj)
                objroom = get_room_index(obj->carried_by->in_room->vnum);
            else objroom = NULL;
            if (objroom != NULL && get_room_index(obj->chobj->in_room->vnum) != objroom)
            {
                char_from_room(obj->chobj);
                char_to_room(obj->chobj,objroom);
                do_look(obj->chobj,"auto");
            }
        }
        if (IS_AFFECTED(ch,AFF_SHADOWPLANE) &&
            (IS_SET(obj->extra_flags, ITEM_SHADOWPLANE) ) )
            REMOVE_BIT(obj->extra_flags, ITEM_SHADOWPLANE);
    }

    return;
}

void do_newbiepack( CHAR_DATA *ch, char *argument )
{
    int temp;
    if (ch->level == 1)
    {
        ch->pcdata->security = 6;
        ch->level = 12;
        ch->trust = 12;
        do_oload(ch,"30333");
        do_oload(ch,"30334");
        do_oload(ch,"30335");
        do_oload(ch,"30336");
        do_oload(ch,"30337");
        do_oload(ch,"30338");
        do_oload(ch,"30339");
        do_oload(ch,"30339");
        do_oload(ch,"30340");
        do_oload(ch,"30340");
        do_oload(ch,"30342");
        do_oload(ch,"30342");
        do_oload(ch,"30343");
        do_oload(ch,"30343");
        //	do_oload(ch,"101018");
        //        do_claim(ch,"zirconium");
        ch->level = 1;
        ch->trust = 0;
        ch->pcdata->security = 0;
    }
    if (ch->level >= 2)
    {
        temp = ch->pcdata->security;
        ch->pcdata->security = 6;
        do_oload(ch,"30333");
        do_oload(ch,"30334");
        do_oload(ch,"30335");
        do_oload(ch,"30336");
        do_oload(ch,"30337");
        do_oload(ch,"30338");
        do_oload(ch,"30339");
        do_oload(ch,"30339");
        do_oload(ch,"30340");
        do_oload(ch,"30340");
        do_oload(ch,"30342");
        do_oload(ch,"30342");
        do_oload(ch,"30343");
        do_oload(ch,"30343");
        ch->pcdata->security = temp;
    }
    WAIT_STATE(ch, 48 );
    return;
}

void do_get( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    char buffer[MAX_STRING_LENGTH];
    OBJ_DATA *obj;
    OBJ_DATA *obj_next;
    OBJ_DATA *container;
    bool found;
    bool bChatted = FALSE;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if (IS_AFFECTED(ch,AFF_ETHEREAL) )
    {
        send_to_char( "You cannot pick things up while ethereal.\n\r", ch );
        return;
    }

    /* Get type. */
    if ( arg1[0] == '\0' )
    {
        send_to_char( "Get what?\n\r", ch );
        return;
    }

    if ( arg2[0] == '\0' )
    {
        if ( str_cmp( arg1, "all" ) && str_prefix( "all.", arg1 ) )
        {
            /* 'get obj' */
            obj = get_obj_list( ch, arg1, ch->in_room->contents );
            if ( obj == NULL )
            {
                act( "I see no $T here.", ch, NULL, arg1, TO_CHAR );
                return;
            }
            if ( IS_SET(obj->extra_flags,ITEM_QUESTED) )
            {
                sprintf(buffer,"#R%s#y finds #R%s#y for quest!#n\n\r",ch->name,obj->short_descr);
                do_questchat2(buffer);
                REMOVE_BIT(obj->extra_flags,ITEM_QUESTED);
            }
            if ( IS_SET(obj->extra_flags,ITEM_AUTOCLAIM) && !str_cmp(obj->questowner,"") )
            {
                if (obj->questowner != NULL) free_string(obj->questowner);
                obj->questowner = str_dup(ch->pcdata->switchname);
            }
            if ( IS_SET(obj->extra_flags,ITEM_AUTOENGRAVE) && !IS_SET(obj->extra_flags,ITEM_ENGRAVED))
            {
                REMOVE_BIT(obj->extra_flags,ITEM_AUTOENGRAVE);
                SET_BIT(obj->extra_flags,ITEM_ENGRAVED);
                obj->questowner = str_dup(ch->pcdata->switchname);
            }
            if (IS_NPC(ch) && IS_SET(obj->quest, QUEST_ARTIFACT))
            {
                send_to_char("You can't pick that up.\n\r", ch );
                return;
            }

            get_obj( ch, obj, NULL );
        }
        else
        {
            /* 'get all' or 'get all.obj' */
            found = FALSE;
            for ( obj = ch->in_room->contents; obj != NULL; obj = obj_next )
            {
                obj_next = obj->next_content;
                if ( ( arg1[3] == '\0' || is_name( &arg1[4], obj->name ) )
                    &&   can_see_obj( ch, obj ) )
                {
                    found = TRUE;
                    get_obj( ch, obj, NULL );
                    if ( (obj != NULL ) && IS_SET(obj->extra_flags,ITEM_QUESTED) )
                    {
                        if ( bChatted == FALSE )
                        {
                            sprintf(buffer,"#R%s#y finds #R%s#y for quest!#n\n\r",ch->name,obj->short_descr);
                            do_questchat2(buffer);
                        }
                        REMOVE_BIT(obj->extra_flags,ITEM_QUESTED);
                        bChatted = TRUE;
                    }
                    if ( (obj != NULL ) && IS_SET(obj->extra_flags,ITEM_AUTOCLAIM) && !str_cmp(obj->questowner,"") )
                    {
                        if (obj->questowner != NULL) free_string(obj->questowner);
                        obj->questowner = str_dup(ch->pcdata->switchname);
                    }
                    if ( IS_SET(obj->extra_flags,ITEM_AUTOENGRAVE) && !IS_SET(obj->extra_flags,ITEM_ENGRAVED))
                    {
                        REMOVE_BIT(obj->extra_flags,ITEM_AUTOENGRAVE);
                        SET_BIT(obj->extra_flags,ITEM_ENGRAVED);
                        obj->questowner = str_dup(ch->pcdata->switchname);
                    }
                }
            }

            if ( !found )
            {
                if ( arg1[3] == '\0' )
                    send_to_char( "I see nothing here.\n\r", ch );
                else
                    act( "I see no $T here.", ch, NULL, &arg1[4], TO_CHAR );
            }
        }
    }
    else
    {
        /* 'get ... container' */
        if ( !str_cmp( arg2, "all" ) || !str_prefix( "all.", arg2 ) )
        {
            send_to_char( "You can't do that.\n\r", ch );
            return;
        }

        if ( ( container = get_obj_here( ch, arg2 ) ) == NULL )
        {
            act( "I see no $T here.", ch, NULL, arg2, TO_CHAR );
            return;
        }

        switch ( container->item_type )
        {
            default:
                send_to_char( "That's not a container.\n\r", ch );
                return;

            case ITEM_CONTAINER:
            case ITEM_CORPSE_NPC:
                break;

            case ITEM_CORPSE_PC:
            {
                char name[MAX_INPUT_LENGTH];
                char *pd;

                if ( IS_NPC(ch) )
                {
                    send_to_char( "You can't do that.\n\r", ch );
                    return;
                }

                pd = container->short_descr;
                pd = one_argument( pd, name );
                pd = one_argument( pd, name );
                pd = one_argument( pd, name );
                /*
                        if ( str_cmp( name, ch->name ) && !IS_IMMORTAL(ch) )
                        {
                            send_to_char( "You can't do that.\n\r", ch );
                            return;
                        }
                */
            }
        }

        if ( IS_SET(container->value[1], CONT_CLOSED) )
        {
            act( "The $d is closed.", ch, NULL, container->name, TO_CHAR );
            return;
        }

        if ( str_cmp( arg1, "all" ) && str_prefix( "all.", arg1 ) )
        {
            /* 'get obj container' */
            obj = get_obj_list( ch, arg1, container->contains );
            if ( obj == NULL)
            {
                act( "I see nothing like that in the $T.",
                    ch, NULL, arg2, TO_CHAR );
                return;
            }
            if ( (obj != NULL ) && IS_SET(obj->extra_flags,ITEM_AUTOCLAIM) && !str_cmp(obj->questowner,""))
            {
                if (obj->questowner != NULL) free_string(obj->questowner);
                obj->questowner = str_dup(ch->pcdata->switchname);
            }
            if ( IS_SET(obj->extra_flags,ITEM_AUTOENGRAVE) && !IS_SET(obj->extra_flags,ITEM_ENGRAVED))
            {
                REMOVE_BIT(obj->extra_flags,ITEM_AUTOENGRAVE);
                SET_BIT(obj->extra_flags,ITEM_ENGRAVED);
                obj->questowner = str_dup(ch->pcdata->switchname);
            }
            get_obj( ch, obj, container );
        }
        else
        {
            /* 'get all container' or 'get all.obj container' */
            found = FALSE;
            for ( obj = container->contains; obj != NULL; obj = obj_next )
            {
                obj_next = obj->next_content;
                if ( ( arg1[3] == '\0' || is_name( &arg1[4], obj->name ) )
                    &&   can_see_obj( ch, obj ) )
                {
                    found = TRUE;
                    get_obj( ch, obj, container );
                    if ( (obj != NULL ) && IS_SET(obj->extra_flags,ITEM_AUTOCLAIM) && !str_cmp(obj->questowner,"") )
                    {
                        if (obj->questowner != NULL) free_string(obj->questowner);
                        obj->questowner = str_dup(ch->pcdata->switchname);
                    }
                    if ( IS_SET(obj->extra_flags,ITEM_AUTOENGRAVE) && !IS_SET(obj->extra_flags,ITEM_ENGRAVED))
                    {
                        REMOVE_BIT(obj->extra_flags,ITEM_AUTOENGRAVE);
                        SET_BIT(obj->extra_flags,ITEM_ENGRAVED);
                        obj->questowner = str_dup(ch->pcdata->switchname);
                    }
                }
            }
            if ( !found )
            {
                if ( arg1[3] == '\0' )
                    act( "I see nothing in the $T.",
                        ch, NULL, arg2, TO_CHAR );
                else
                    act( "I see nothing like that in the $T.",
                        ch, NULL, arg2, TO_CHAR );
            }
        }
    }
    do_autosave(ch,"");
    return;
}

void do_put( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    OBJ_DATA *container;
    OBJ_DATA *obj;
    OBJ_DATA *obj2;
    OBJ_DATA *obj_next;
    OBJ_DATA *obj_next2;
    ROOM_INDEX_DATA *objroom = get_room_index(ROOM_VNUM_IN_OBJECT);

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if ( arg1[0] == '\0' || arg2[0] == '\0' )
    {
        send_to_char( "Put what in what?\n\r", ch );
        return;
    }

    if ( !str_cmp( arg2, "all" ) || !str_prefix( "all.", arg2 ) )
    {
        send_to_char( "You can't do that.\n\r", ch );
        return;
    }
    /* I'll leave this out for now - KaVir
        if ( ( ( container = get_obj_carry( ch, arg2 ) ) == NULL ) &&
         ( ( container = get_obj_wear(	ch, arg2 ) ) == NULL ) &&
           ( IS_AFFECTED(ch,AFF_ETHEREAL) ) )
        {
        send_to_char( "You can't let go of it!\n\r", ch );
        return;
        }
    */
    if ( ( container = get_obj_here( ch, arg2 ) ) == NULL )
    {
        act( "I see no $T here.", ch, NULL, arg2, TO_CHAR );
        return;
    }

    if ( container->item_type != ITEM_CONTAINER )
    {
        send_to_char( "That's not a container.\n\r", ch );
        return;
    }

    if ( IS_SET(container->value[1], CONT_CLOSED) )
    {
        act( "The $d is closed.", ch, NULL, container->name, TO_CHAR );
        return;
    }

    if ( str_cmp( arg1, "all" ) && str_prefix( "all.", arg1 ) )
    {
        /* 'put obj container' */
        if ( ( obj = get_obj_carry( ch, arg1 ) ) == NULL )
        {
            send_to_char( "You do not have that item.\n\r", ch );
            return;
        }

        if ( obj == container )
        {
            send_to_char( "You can't fold it into itself.\n\r", ch );
            return;
        }

        if ( (IS_SET( obj->quest, QUEST_ARTIFACT)) && (!IS_IMMORTAL(ch)) )
        {
            send_to_char("You cannot put artifacts in a container.\n\r",ch);
            return;
        }

        if ( !can_drop_obj( ch, obj ) )
        {
            send_to_char( "You can't let go of it.\n\r", ch );
            return;
        }

        if ( get_obj_weight( obj ) + get_obj_weight( container )
            > container->value[0] )
        {
            send_to_char( "It won't fit.\n\r", ch );
            return;
        }

        for ( obj2 = container->contains; obj2 != NULL; obj2 = obj_next2 )
        {
            obj_next2 = obj2->next_content;
            if ( obj2->chobj != NULL && obj != obj2)
                act( "A hand reaches inside $P and drops $p.", obj2->chobj, obj, container, TO_CHAR );
        }
        obj_from_char( obj );
        obj_to_obj( obj, container );
        act( "$n puts $p in $P.", ch, obj, container, TO_ROOM );
        act( "You put $p in $P.", ch, obj, container, TO_CHAR );
    }
    else
    {
        /* 'put all container' or 'put all.obj container' */
        for ( obj = ch->carrying; obj != NULL; obj = obj_next )
        {
            obj_next = obj->next_content;

            if ( ( arg1[3] == '\0' || is_name( &arg1[4], obj->name ) )
                &&   can_see_obj( ch, obj )
                &&   obj->wear_loc == WEAR_NONE
                &&   obj != container
                &&   !IS_SET( obj->quest, QUEST_ARTIFACT)
                &&   can_drop_obj( ch, obj )
                &&   get_obj_weight( obj ) + get_obj_weight( container )
                <= container->value[0] )
            {
                for ( obj2 = container->contains; obj2 != NULL; obj2 = obj_next2 )
                {
                    obj_next2 = obj2->next_content;
                    if ( obj2->chobj != NULL && obj2->chobj->in_room != NULL)
                    {
                        if (objroom != get_room_index(obj2->chobj->in_room->vnum))
                        {
                            char_from_room(obj2->chobj);
                            char_to_room(obj2->chobj,objroom);
                            do_look(obj2->chobj,"auto");
                        }
                        if (obj != obj2)
                            act( "A hand reaches inside $P and drops $p.", obj2->chobj, obj, container, TO_CHAR );
                    }
                }
                obj_from_char( obj );
                obj_to_obj( obj, container );
                act( "$n puts $p in $P.", ch, obj, container, TO_ROOM );
                act( "You put $p in $P.", ch, obj, container, TO_CHAR );
            }
        }
    }
    do_autosave(ch,"");
    return;
}

void do_drop( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    OBJ_DATA *obj_next;
    bool found;
    int dropcount;

    if( IS_NPC(ch)) return;
    dropcount=0;
    argument = one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
        send_to_char( "Drop what?\n\r", ch );
        return;
    }
    /*if(!str_cmp( arg, "20")){
            'drop all' or 'drop all.obj'
            found = FALSE;
            for ( obj = ch->carrying; obj != NULL; obj = obj_next )
            {dropcount=dropcount+1;
            obj_next = obj->next_content;

             if( dropcount>20)
               return;
                obj_next = obj->next_content;

                if ( can_see_obj( ch, obj )
                &&   obj->wear_loc == WEAR_NONE
                &&   can_drop_obj( ch, obj ) )
                {
                    found = TRUE;
                    obj_from_char( obj );
                    obj_to_room( obj, ch->in_room );
                     Objects should only have a shadowplane flag when on the floor
                    if (IS_AFFECTED(ch,AFF_SHADOWPLANE) &&
                        (!IS_SET(obj->extra_flags, ITEM_SHADOWPLANE) ) )
                        SET_BIT(obj->extra_flags, ITEM_SHADOWPLANE);
                    act( "$n drops $p.", ch, obj, NULL, TO_ROOM );
                    act( "You drop $p.", ch, obj, NULL, TO_CHAR );
                }}        if ( !found )
            {
                if ( arg[3] == '\0' )
                    act( "You are not carrying anything.",
                        ch, NULL, arg, TO_CHAR );
                else
                    act( "You are not carrying any $T.",
                        ch, NULL, &arg[4], TO_CHAR );
            }
    }
    */

    else if ( str_cmp( arg, "all" ) && str_prefix( "all.", arg ) )
    {
        /* 'drop obj' */
        if ( ( obj = get_obj_carry( ch, arg ) ) == NULL )
        {
            send_to_char( "You do not have that item.\n\r", ch );
            return;
        }

        if ( !can_drop_obj( ch, obj ) )
        {
            send_to_char( "You can't let go of it.\n\r", ch );
            return;
        }
        if ( can_see_obj( ch, obj )
            &&   obj->wear_loc == WEAR_NONE
            &&   can_drop_obj( ch, obj ) )
        {

            obj_from_char( obj );
            obj_to_room( obj, ch->in_room );
            /* Objects should only have a shadowplane flag when on the floor */
            if (IS_AFFECTED(ch,AFF_SHADOWPLANE) &&
                (!IS_SET(obj->extra_flags, ITEM_SHADOWPLANE) ) )
                SET_BIT(obj->extra_flags, ITEM_SHADOWPLANE);
            act( "$n drops $p.", ch, obj, NULL, TO_ROOM );
            act( "You drop $p.", ch, obj, NULL, TO_CHAR );
        }
    }
    else
    {                                                       /*send_to_char("Try the new command '#Rdrop 20#n' instead.\n\r",ch);*/
        /* 'drop all' or 'drop all.obj' */
        found = FALSE;
        for ( obj = ch->carrying; obj != NULL; obj = obj_next )
        {
            obj_next = obj->next_content;

            if ( ( arg[3] == '\0' || is_name( &arg[4], obj->name ) )
                &&   can_see_obj( ch, obj )
                &&   obj->wear_loc == WEAR_NONE
                &&   can_drop_obj( ch, obj ) )
            {
                found = TRUE;
                obj_from_char( obj );
                obj_to_room( obj, ch->in_room );
                // Objects should only have a shadowplane flag when on the floor
                if (IS_AFFECTED(ch,AFF_SHADOWPLANE) &&
                    (!IS_SET(obj->extra_flags, ITEM_SHADOWPLANE) ) )
                    SET_BIT(obj->extra_flags, ITEM_SHADOWPLANE);
                act( "$n drops $p.", ch, obj, NULL, TO_ROOM );
                act( "You drop $p.", ch, obj, NULL, TO_CHAR );
            }
        }

        if ( !found )
        {
            if ( arg[3] == '\0' )
                act( "You are not carrying anything.",
                    ch, NULL, arg, TO_CHAR );
            else
                act( "You are not carrying any $T.",
                    ch, NULL, &arg[4], TO_CHAR );
        }
    }

    do_autosave(ch,"");
    return;
}

void do_discard( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    OBJ_DATA *obj_next;
    bool found = 0;

    if (IS_NPC(ch))
        return;

    argument = one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
        send_to_char( "Drop what?\n\r", ch );
        return;
    }

    if ( str_cmp( arg, "all" ) && str_prefix( "all.", arg ) )
    {
        /* 'drop obj' */
        if ( ( obj = get_obj_carry( ch, arg ) ) == NULL )
        {
            send_to_char( "You do not have that item.\n\r", ch );
            return;
        }

        if ( !can_drop_obj( ch, obj ) )
        {
            send_to_char( "You can't let go of it.\n\r", ch );
            return;
        }
        obj_from_char( obj );
        obj_to_room( obj, ch->in_room );
        // Objects should only have a shadowplane flag when on the floor
        if (IS_AFFECTED(ch,AFF_SHADOWPLANE) &&
            (!IS_SET(obj->extra_flags, ITEM_SHADOWPLANE) ) )
            SET_BIT(obj->extra_flags, ITEM_SHADOWPLANE);
        act( "$n drops $p.", ch, obj, NULL, TO_ROOM );
        act( "You drop $p.", ch, obj, NULL, TO_CHAR );
    }
    else
    {
        /* 'drop all' or 'drop all.obj' */
        found = FALSE;
        for ( obj = ch->carrying; obj != NULL; obj = obj_next )
        {
            obj_next = obj->next_content;

            if ( ( arg[3] == '\0' || is_name( &arg[4], obj->name ) )
                &&   can_see_obj( ch, obj )
                &&   obj->wear_loc == WEAR_NONE
                &&   can_drop_obj( ch, obj ) )
            {
                found = TRUE;
                obj_from_char( obj );
                if(!IS_SET(obj->quest, QUEST_ARTIFACT))
                {
                    obj_to_room( obj, ch->in_room );
                }
                /* Objects should only have a shadowplane flag when on the floor */
                if (IS_AFFECTED(ch,AFF_SHADOWPLANE))
                    act( "$n drops $p.", ch, obj, NULL, TO_ROOM );
                act( "You drop $p.", ch, obj, NULL, TO_CHAR );
            }
        }
    }
    if ( !found )
    {
        if ( arg[3] == '\0' )
            act( "You are not carrying anything.",
                ch, NULL, arg, TO_CHAR );
        else
            act( "You are not carrying any $T.",
                ch, NULL, &arg[4], TO_CHAR );
    }

    do_autosave(ch,"");
    return;
}

void do_give( CHAR_DATA *ch, char *argument )
{
    char arg1 [MAX_INPUT_LENGTH];
    char arg2 [MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    OBJ_DATA  *obj;

    if( IS_NPC(ch)) return;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if ( arg1[0] == '\0' || arg2[0] == '\0' )
    {
        send_to_char( "Give what to whom?\n\r", ch );
        return;
    }

    if ( ( obj = get_obj_carry( ch, arg1 ) ) == NULL )
    {
        send_to_char( "You do not have that item.\n\r", ch );
        return;
    }

    if ( obj->wear_loc != WEAR_NONE )
    {
        send_to_char( "You must remove it first.\n\r", ch );
        return;
    }
    if ( ( victim = get_char_room( ch, arg2 ) ) == NULL )
    {
        send_to_char( "They aren't here.\n\r", ch );
        return;
    }
    if ( IS_SET(obj->quest, QUEST_ARTIFACT) && IS_NPC(victim) )
    {
        stc("Behave.  Don't give artis to mobs.\n\r",ch);
        return;
    }

    if ( !can_drop_obj( ch, obj ) )
    {
        send_to_char( "You can't let go of it.\n\r", ch );
        return;
    }

    if (IS_AFFECTED(victim,AFF_ETHEREAL) )
    {
        send_to_char( "You cannot give things to ethereal people.\n\r", ch );
        return;
    }

    if ( victim->carry_number + 1 > can_carry_n( victim ) )
    {
        act( "$N has $S hands full.", ch, NULL, victim, TO_CHAR );
        return;
    }

    if ( victim->carry_weight + get_obj_weight( obj ) > can_carry_w(victim)  )
    {
        act( "$N can't carry that much weight.", ch, NULL, victim, TO_CHAR );
        return;
    }

    if ( !can_see_obj( victim, obj ) )
    {
        act( "$N can't see it.", ch, NULL, victim, TO_CHAR );
        return;
    }

    obj_from_char( obj );
    obj_to_char( obj, victim );
    act( "$n gives $p to $N.", ch, obj, victim, TO_NOTVICT );
    act( "$n gives you $p.",   ch, obj, victim, TO_VICT    );
    act( "You give $p to $N.", ch, obj, victim, TO_CHAR    );
    WAIT_STATE(ch, 6);
    return;
}

void do_fill( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    OBJ_DATA *fountain;
    bool found;
    int liquid;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
        send_to_char( "Fill what?\n\r", ch );
        return;
    }

    if ( ( obj = get_obj_carry( ch, arg ) ) == NULL )
    {
        send_to_char( "You do not have that item.\n\r", ch );
        return;
    }

    found = FALSE;
    for ( fountain = ch->in_room->contents; fountain != NULL;
        fountain = fountain->next_content )
    {
        if ( fountain->item_type == ITEM_FOUNTAIN )
        {
            found = TRUE;
            break;
        }
    }

    if ( !found )
    {
        send_to_char( "There is no fountain here!\n\r", ch );
        return;
    }

    if (IS_AFFECTED(ch, AFF_SHADOWPLANE) &&
        fountain->in_room != NULL &&
        !IS_SET(fountain->extra_flags, ITEM_SHADOWPLANE) )
    {
        send_to_char( "You are too insubstantual.\n\r", ch );
        return;
    }
    else if (!IS_AFFECTED(ch, AFF_SHADOWPLANE) &&
        fountain->in_room != NULL &&
        IS_SET(fountain->extra_flags, ITEM_SHADOWPLANE) )
    {
        send_to_char( "It is too insubstantual.\n\r", ch );
        return;
    }
    else if (IS_AFFECTED(ch,AFF_ETHEREAL) )
    {
        send_to_char( "You cannot fill containers while ethereal.\n\r", ch );
        return;
    }

    if ( obj->item_type != ITEM_DRINK_CON )
    {
        send_to_char( "You can't fill that.\n\r", ch );
        return;
    }

    if ( obj->value[1] >= obj->value[0] )
    {
        send_to_char( "Your container is already full.\n\r", ch );
        return;
    }

    if ( (obj->value[2] != fountain->value[2]) && obj->value[1] > 0)
    {
        send_to_char( "You cannot mix two different liquids.\n\r", ch );
        return;
    }

    act( "$n dips $p into $P.", ch, obj, fountain, TO_ROOM );
    act( "You dip $p into $P.", ch, obj, fountain, TO_CHAR );
    obj->value[2] = fountain->value[2];
    obj->value[1] = obj->value[0];
    liquid = obj->value[2];
    act( "$n fills $p with $T.", ch, obj, liq_table[liquid].liq_name, TO_ROOM );
    act( "You fill $p with $T.", ch, obj, liq_table[liquid].liq_name, TO_CHAR );
    return;
}

void do_drink( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    int amount;
    int liquid;
    int bloodpool;

    one_argument( argument, arg );

    {
        if (ch->pcdata->rank == AGE_ELDER) bloodpool = (500 / ch->generation);
        else if (ch->pcdata->rank == AGE_METHUSELAH) bloodpool = (750 / ch->generation);
        else if (ch->pcdata->rank == AGE_LA_MAGRA) bloodpool = (1000 / ch->generation);
        else if (ch->pcdata->rank == AGE_TRUEBLOOD) bloodpool = (1250 / ch->generation);
        else bloodpool = (250 / ch->generation);
    }

    if ( arg[0] == '\0' )
    {
        for ( obj = ch->in_room->contents; obj; obj = obj->next_content )
        {
            if ( obj->item_type == ITEM_FOUNTAIN )
                break;
        }

        if ( obj == NULL )
        {
            send_to_char( "Drink what?\n\r", ch );
            return;
        }
    }
    else
    {
        if ( ( obj = get_obj_here( ch, arg ) ) == NULL )
        {
            send_to_char( "You can't find it.\n\r", ch );
            return;
        }
    }

    if ( !IS_NPC(ch) && ch->pcdata->condition[COND_DRUNK] > 10 )
    {
        send_to_char( "You fail to reach your mouth.  *Hic*\n\r", ch );
        return;
    }

    switch ( obj->item_type )
    {
        default:
            send_to_char( "You can't drink from that.\n\r", ch );
            break;

        case ITEM_POTION:
            do_quaff(ch,obj->name);
            return;
        case ITEM_FOUNTAIN:
            if ( ( liquid = obj->value[2] ) >= LIQ_MAX )
            {
                bug( "Do_drink: bad liquid number %d.", liquid );
                liquid = obj->value[2] = 0;
            }

            if (IS_AFFECTED(ch, AFF_SHADOWPLANE) &&
                obj->in_room != NULL &&
                !IS_SET(obj->extra_flags, ITEM_SHADOWPLANE) )
            {
                send_to_char( "You are too insubstantual.\n\r", ch );
                break;
            }
            else if (!IS_AFFECTED(ch, AFF_SHADOWPLANE) &&
                obj->in_room != NULL &&
                IS_SET(obj->extra_flags, ITEM_SHADOWPLANE) )
            {
                send_to_char( "It is too insubstantual.\n\r", ch );
                break;
            }
            else if (IS_AFFECTED(ch,AFF_ETHEREAL) )
            {
                send_to_char( "You can only drink from things you are carrying while ethereal.\n\r", ch );
                return;
            }

            if ( liquid != 13 && liquid != 16 && liquid != 17 && liquid != 19 && liquid != 18 && IS_CLASS(ch,CLASS_VAMPIRE) )
            {
                send_to_char( "You can only drink blood.\n\r", ch );
                break;
            }

            if ( liquid == 19 )
            {
                if ( ch->fighting != NULL || ch->fight_timer > 0 ) return stcf(ch,"Not while fighting you dumb ass.\n\r");
                if ( (ch->hit+(ch->max_hit/25)) <= ch->max_hit )
                    ch->hit += ch->max_hit/25;
                else ch->hit = ch->max_hit;
                if ( (ch->mana+(ch->max_mana/25)) <= ch->max_mana )
                    ch->mana += ch->max_mana/25;
                else ch->mana = ch->max_mana;
                if ( (ch->move+(ch->max_move/25)) <= ch->max_move )
                    ch->move += ch->max_move/25;
                else ch->move = ch->max_move;
                WAIT_STATE(ch,6);
            }

            else if ( liquid == 13 && IS_CLASS(ch, CLASS_VAMPIRE))
            {
                ch->pcdata->condition[COND_THIRST] += number_range(2000,3500);
            }

            if (ch->pcdata->rank == AGE_ELDER) bloodpool = (500 / ch->generation);
            else if (ch->pcdata->rank == AGE_METHUSELAH) bloodpool = (750 / ch->generation);
            else if (ch->pcdata->rank == AGE_LA_MAGRA) bloodpool = (1000 / ch->generation);
            else if (ch->pcdata->rank == AGE_TRUEBLOOD) bloodpool = (1250 / ch->generation);
            else bloodpool = (250 / ch->generation);

            act( "$n drinks $T from $p.", ch, obj, liq_table[liquid].liq_name, TO_ROOM );
            act( "You drink $T from $p.", ch, obj, liq_table[liquid].liq_name, TO_CHAR );

            amount = number_range(25, 50);
            amount = UMIN(amount, obj->value[1]);

            gain_condition( ch, COND_DRUNK,
                amount * liq_table[liquid].liq_affect[COND_DRUNK  ] );
            gain_condition( ch, COND_FULL,
                amount * liq_table[liquid].liq_affect[COND_FULL   ] );
            gain_condition( ch, COND_THIRST,
                amount * liq_table[liquid].liq_affect[COND_THIRST ] );

            if ( !IS_NPC(ch) && !IS_CLASS(ch, CLASS_VAMPIRE) &&
                ch->pcdata->condition[COND_DRUNK]  > 10 )
                send_to_char( "You feel drunk.\n\r", ch );
            if ( !IS_NPC(ch) && !IS_CLASS(ch, CLASS_VAMPIRE) &&
                ch->pcdata->condition[COND_FULL]   > 50 )
                send_to_char( "You are full.\n\r", ch );
            if ( !IS_NPC(ch) && !IS_CLASS(ch, CLASS_VAMPIRE) &&
                ch->pcdata->condition[COND_THIRST] > ((25000/ch->generation)+bloodpool ))
                send_to_char( "You do not feel thirsty.\n\r", ch );
            if (( !IS_NPC(ch) && IS_CLASS(ch, CLASS_VAMPIRE) &&
                ch->pcdata->condition[COND_THIRST] >= ((25000/ch->generation)+bloodpool)))
                send_to_char( "Your blood thirst is sated.\n\r", ch );
            else if(( !IS_NPC(ch) && IS_CLASS(ch, CLASS_VAMPIRE) && (ch->pcdata->condition[COND_THIRST] >= ((25000/ch->generation)+bloodpool))))
                send_to_char( "Your blood thirst is sated.\n\r", ch );

            if ( obj->value[3] != 0 && (!IS_NPC(ch) && !IS_CLASS(ch, CLASS_VAMPIRE)))
            {
                /* The shit was poisoned ! */
                AFFECT_DATA af;

                act( "$n chokes and gags.", ch, NULL, NULL, TO_ROOM );
                send_to_char( "You choke and gag.\n\r", ch );
                af.type  = gsn_poison;
                af.duration  = 3 * amount;
                af.location  = APPLY_NONE;
                af.modifier  = 0;
                af.bitvector = AFF_POISON;
                affect_join( ch, &af );
            }
            break;

        case ITEM_DRINK_CON:
            if ( obj->value[1] <= 0 )
            {
                send_to_char( "It is already empty.\n\r", ch );
                return;
            }

            if ( ( liquid = obj->value[2] ) >= LIQ_MAX )
            {
                bug( "Do_drink: bad liquid number %d.", liquid );
                liquid = obj->value[2] = 0;
            }

            if ( liquid != 13 && liquid != 19  && IS_CLASS(ch,CLASS_VAMPIRE) )
            {
                send_to_char( "You can only drink blood.\n\r", ch );
                break;
            }

            if ( liquid == 19 )
            {
                if ( ch->fighting != NULL || ch->fight_timer > 0 ) return stcf(ch,"Not while fighting you dumb ass.\n\r");
                if ( (ch->hit+(ch->max_hit/25)) <= ch->max_hit )
                    ch->hit += ch->max_hit/25;
                else ch->hit = ch->max_hit;
                if ( (ch->mana+(ch->max_mana/25)) <= ch->max_mana )
                    ch->mana += ch->max_mana/25;
                else ch->mana = ch->max_mana;
                if ( (ch->move+(ch->max_move/25)) <= ch->max_move )
                    ch->move += ch->max_move/25;
                else ch->move = ch->max_move;
                WAIT_STATE(ch,4);
            }

            if ( liquid == 13 && IS_CLASS(ch, CLASS_VAMPIRE))
            {
                ch->pcdata->condition[COND_THIRST] += number_range(2000,5000);
            }

            act( "$n drinks $T from $p.", ch, obj, liq_table[liquid].liq_name, TO_ROOM );
            act( "You drink $T from $p.",
                ch, obj, liq_table[liquid].liq_name, TO_CHAR );

            amount = number_range(25, 50);
            amount = UMIN(amount, obj->value[1]);

            gain_condition( ch, COND_DRUNK,
                amount * liq_table[liquid].liq_affect[COND_DRUNK  ] );
            gain_condition( ch, COND_FULL,
                amount * liq_table[liquid].liq_affect[COND_FULL   ] );
            gain_condition( ch, COND_THIRST,
                amount * liq_table[liquid].liq_affect[COND_THIRST ] );

            if ( !IS_NPC(ch) && !IS_CLASS(ch, CLASS_VAMPIRE) &&
                ch->pcdata->condition[COND_DRUNK]  > 10 )
                send_to_char( "You feel drunk.\n\r", ch );
            if ( !IS_NPC(ch) && !IS_CLASS(ch, CLASS_VAMPIRE) &&
                ch->pcdata->condition[COND_FULL]   > 50 )
                send_to_char( "You are full.\n\r", ch );
            if ( !IS_NPC(ch) && !IS_CLASS(ch, CLASS_VAMPIRE) &&
                ch->pcdata->condition[COND_THIRST] > (25000/ch->generation)+bloodpool )
                send_to_char( "You do not feel thirsty.\n\r", ch );
            if ( !IS_NPC(ch) && IS_CLASS(ch, CLASS_VAMPIRE) &&
                ch->pcdata->condition[COND_THIRST] >= (25000 / ch->generation) + bloodpool )
                send_to_char( "Your blood thirst is sated.\n\r", ch );

            if ( obj->value[3] != 0 && (!IS_NPC(ch) && !IS_CLASS(ch, CLASS_VAMPIRE)))
            {
                /* The shit was poisoned ! */
                AFFECT_DATA af;

                act( "$n chokes and gags.", ch, NULL, NULL, TO_ROOM );
                send_to_char( "You choke and gag.\n\r", ch );
                af.type  = gsn_poison;
                af.duration  = 3 * amount;
                af.location  = APPLY_NONE;
                af.modifier  = 0;
                af.bitvector = AFF_POISON;
                affect_join( ch, &af );
            }

            obj->value[1] -= amount;
            if ( obj->value[1] <= 0 )
            {
                obj->value[1] = 0;
            }
            break;
    }

    return;
}

void do_empty( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    int liquid;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
        send_to_char( "Empty what?\n\r", ch );
        return;
    }

    if ( ( obj = get_obj_here( ch, arg ) ) == NULL )
    {
        send_to_char( "You can't find it.\n\r", ch );
        return;
    }

    switch ( obj->item_type )
    {
        default:
            send_to_char( "You cannot empty that.\n\r", ch );
            break;

        case ITEM_DRINK_CON:
            if ( obj->value[1] <= 0 )
            {
                send_to_char( "It is already empty.\n\r", ch );
                return;
            }

            if ( ( liquid = obj->value[2] ) >= LIQ_MAX )
            {
                bug( "Do_drink: bad liquid number %d.", liquid );
                liquid = obj->value[2] = 0;
            }

            act( "$n empties $T from $p.", ch, obj, liq_table[liquid].liq_name, TO_ROOM );
            act( "You empty $T from $p.",
                ch, obj, liq_table[liquid].liq_name, TO_CHAR );

            obj->value[1] = 0;
            break;
    }

    return;
}

void do_eat( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    int level;

    one_argument( argument, arg );
    if ( arg[0] == '\0' )
    {
        send_to_char( "Eat what?\n\r", ch );
        return;
    }

    if ( ( obj = get_obj_carry( ch, arg ) ) == NULL )
    {
        send_to_char( "You do not have that item.\n\r", ch );
        return;
    }

    if ( !IS_IMMORTAL(ch) )
    {
        if ( !IS_NPC(ch) && IS_CLASS(ch, CLASS_VAMPIRE) && obj->item_type == ITEM_FOOD)
        {
            send_to_char( "You are unable to stomach it.\n\r", ch );
            return;
        }

        if ( obj->item_type != ITEM_FOOD && obj->item_type != ITEM_PILL && obj->item_type != ITEM_FLAKE &&
            obj->item_type != ITEM_EGG && obj->item_type != ITEM_QUEST && obj->item_type != ITEM_DRAGONGEM
            && obj->item_type != ITEM_DTOKEN && obj->item_type != ITEM_XPTOKEN && obj->item_type != ITEM_PRTOKEN && obj->item_type != ITEM_STORETOKEN)
        {
            if (IS_NPC(ch) || !IS_SET(ch->special,SPC_WOLFMAN) ||
                obj->item_type != ITEM_TRASH)
            {
                send_to_char( "That's not edible.\n\r", ch );
                return;
            }
        }

        if ( !IS_NPC(ch) && ch->pcdata->condition[COND_FULL] > 50 &&
            obj->item_type != ITEM_TRASH && obj->item_type != ITEM_QUEST &&
            obj->item_type != ITEM_PILL && obj->item_type != ITEM_DRAGONGEM && obj->item_type != ITEM_XPTOKEN && obj->item_type != ITEM_PRTOKEN
            && obj->item_type != ITEM_STORETOKEN)
        {
            send_to_char( "You are too full to eat more.\n\r", ch );
            return;
        }
        if( !IS_NPC(ch) && obj->item_type != ITEM_QUEST && obj->item_type != ITEM_PILL && obj->item_type != ITEM_FOOD && obj->item_type != ITEM_XPTOKEN
            && obj->item_type != ITEM_PRTOKEN && obj->item_type != ITEM_FLAKE && obj->item_type != ITEM_STORETOKEN)
        {
            send_to_char( "You can't eat that.\n\r", ch );
            return;
        }
    }

    act( "$n eats $p.",  ch, obj, NULL, TO_ROOM );
    act( "You eat $p.", ch, obj, NULL, TO_CHAR );

    if(obj->pIndexData->vnum==110999)
    {

        if(ch->max_hit-ch->hit>=10000)
            ch->hit+=10000;
        else send_to_char("Not hurt enougth to heal hp.\n\r",ch);
        if(ch->max_mana-ch->mana>=10000)
            ch->mana+=10000;
        else send_to_char("Not enougth mana burnt to replenish mana.\n\r",ch);
        if(ch->max_mana-ch->mana>=10000)
            ch->move+=10000;
        else send_to_char("Not exausted enougth to refreshen move.\n\r",ch);

        send_to_char("Medkit used.\n\r",ch);
        if(ch->pcdata->Bootcamp[5]<0)
            ch->pcdata->Bootcamp[5]+=1;
    }
    switch ( obj->item_type )
    {
        default:
            break;

        case ITEM_FOOD:
            if ( !IS_NPC(ch) )
            {
                int condition;

                condition = ch->pcdata->condition[COND_FULL];
                gain_condition( ch, COND_FULL, obj->value[0] );
                if ( condition == 0 && ch->pcdata->condition[COND_FULL] > 10 )
                    send_to_char( "You are no longer hungry.\n\r", ch );
                else if ( ch->pcdata->condition[COND_FULL] > 50 )
                    send_to_char( "You are full.\n\r", ch );
            }

            if ( obj->value[3] != 0 )
            {
                /* The shit was poisoned! */
                AFFECT_DATA af;

                act( "$n chokes and gags.", ch, NULL, NULL, TO_ROOM );
                send_to_char( "You choke and gag.\n\r", ch );

                af.type  = gsn_poison;
                af.duration  = 2 * obj->value[0];
                af.location  = APPLY_NONE;
                af.modifier  = 0;
                af.bitvector = AFF_POISON;
                affect_join( ch, &af );
            }
            break;

        case ITEM_PILL:
            level = obj->value[0];
            if (level < 1) level = 1;
            if (level > MAX_SPELL) level = MAX_SPELL;

            obj_cast_spell( obj->value[1], level, ch, ch, NULL );
            obj_cast_spell( obj->value[2], level, ch, ch, NULL );
            obj_cast_spell( obj->value[3], level, ch, ch, NULL );
            if (ch->position == POS_FIGHTING)
            {
                if (!IS_IMMORTAL( ch))
                    WAIT_STATE(ch, 6 );
            }
            break;

        case ITEM_FLAKE:
            if ( !IS_NPC(ch) )
            {
                //	ch->race +=obj->value[0];
                send_to_char("Temporarily removed.  -Beorn.\n\r",ch);

            }
            break;

        case ITEM_QUEST:
            if ( !IS_NPC(ch) )
            {
                if(obj->level == 69&&obj->value[0]<5)
                    ch->race +=obj->value[0];
                else
                    ch->pcdata->quest += obj->value[0];
            }
            break;

        case ITEM_XPTOKEN:
            if ( !IS_NPC(ch) )
            {
                if(obj->level == 69)
                    ch->max_hit +=obj->value[1];
                else
                    gain_exp(ch,obj->value[1]);
            }
            break;

        case ITEM_PRTOKEN:
            if ( !IS_NPC(ch) )
            {
                if(obj->level == 69)
                    ch->max_mana +=obj->value[2];
                else
                    ch->practice += obj->value[2];
            }
            if ( ch->practice < 0 && !IS_NPC(ch) ) ch->practice = 2000000000;
            break;

        case ITEM_STORETOKEN:
            if ( !IS_NPC(ch) )
            {
                if(obj->level == 69)
                    ch->max_move +=obj->value[3];
                else
                    ch->pcdata->store += obj->value[1];
            }
            if ( ch->pcdata->store > 2000000000 ) ch->pcdata->store = 2000000000;
            break;

        case ITEM_DTOKEN:
            if ( !IS_NPC(ch) && IS_CLASS(ch, CLASS_DEMON) )
                ch->pcdata->stats[DEMON_CURRENT] += obj->value[0];
            break;

        case ITEM_EGG:
            if ( !IS_NPC(ch) )
            {
                int condition;

                condition = ch->pcdata->condition[COND_FULL];
                gain_condition( ch, COND_FULL, obj->value[1] );
                if ( condition == 0 && ch->pcdata->condition[COND_FULL] > 10 )
                    send_to_char( "You are no longer hungry.\n\r", ch );
                else if ( ch->pcdata->condition[COND_FULL] > 50 )
                    send_to_char( "You are full.\n\r", ch );
            }

            /* Note to myself...remember to set v2 for mobiles that hatch within
             * the player (like aliens ;).	KaVir.
             */

            if ( obj->value[3] != 0 )
            {
                /* The shit was poisoned! */
                AFFECT_DATA af;

                act( "$n chokes and gags.", ch, NULL, NULL, TO_ROOM );
                send_to_char( "You choke and gag.\n\r", ch );

                af.type  = gsn_poison;
                af.duration  = 2 * obj->value[0];
                af.location  = APPLY_NONE;
                af.modifier  = 0;
                af.bitvector = AFF_POISON;
                affect_join( ch, &af );
            }
            break;
    }

    if (obj != NULL) extract_obj( obj );
    return;
}

/*
 * Remove an object.
 */
bool remove_obj( CHAR_DATA *ch, int iWear, bool fReplace )
{
    OBJ_DATA *obj;

    if ( ( obj = get_eq_char( ch, iWear ) ) == NULL )
        return TRUE;

    if ( !fReplace )
        return FALSE;

    if ( IS_SET(obj->extra_flags, ITEM_NOREMOVE) )
    {
        act( "You can't remove $p.", ch, obj, NULL, TO_CHAR );
        return FALSE;
    }

    unequip_char( ch, obj );
    act( "$n stops using $p.", ch, obj, NULL, TO_ROOM );
    act( "You stop using $p.", ch, obj, NULL, TO_CHAR );
    return TRUE;
}

/*
 * Wear one object.
 * Optional replacement of existing objects.
 * Big repetitive code, ick.
 */
void wear_obj( CHAR_DATA *ch, OBJ_DATA *obj, bool fReplace )
{
    bool wolf_ok = FALSE;

    if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_WEREWOLF) &&
        IS_SET(obj->spectype, SITEM_WOLFWEAPON)) wolf_ok = TRUE;
    if (IS_SET(obj->extra_flags,ITEM_ENGRAVED) && str_cmp(obj->questowner,ch->pcdata->switchname) )
    {
        stc("You can't wear that.\n\r",ch);
        return;
    }

    if ( CAN_WEAR( obj, ITEM_WIELD ) || CAN_WEAR( obj, ITEM_HOLD ) ||
        CAN_WEAR( obj, ITEM_WEAR_SHIELD ) || obj->item_type == ITEM_LIGHT )
    {
        if ( get_eq_char( ch, WEAR_WIELD    ) != NULL
            &&   get_eq_char( ch, WEAR_HOLD     ) != NULL
            &&   get_eq_char( ch, WEAR_LIGHT    ) != NULL
            &&   get_eq_char( ch, WEAR_SHIELD   ) != NULL
            &&   !remove_obj( ch, WEAR_LIGHT , fReplace )
            &&   !remove_obj( ch, WEAR_SHIELD, fReplace )
            &&   !remove_obj( ch, WEAR_WIELD , fReplace )
            &&   !remove_obj( ch, WEAR_HOLD  , fReplace ) )
            return;
        if (!IS_NPC(ch) && !IS_FORM(ch,ITEM_WIELD))
        {
            send_to_char("You are unable to use it.\n\r",ch);
            return;
        }
        if (get_eq_char( ch, WEAR_WIELD ) == NULL)
        {
            if(!IS_NPC (ch) && IS_CLASS(ch, CLASS_MONK))
            {
                send_to_char( "You must keep your hands free for combat!\n\r", ch);
                return;
            }

            if ( obj->item_type == ITEM_LIGHT )
            {
                act( "$n lights $p and clutches it in $s right hand.", ch, obj, NULL, TO_ROOM );
                act( "You light $p and clutch it in your right hand.",  ch, obj, NULL, TO_CHAR );
            }
            else
            {
                act( "$n clutches $p in $s right hand.",    ch, obj, NULL, TO_ROOM );
                act( "You clutch $p in your right hand.",  ch, obj, NULL, TO_CHAR );
            }
            equip_char( ch, obj, WEAR_WIELD );
            return;
        }
        else if (get_eq_char( ch, WEAR_HOLD ) == NULL)
        {
            if(IS_CLASS(ch, CLASS_MONK) && ch->pcdata->upgrade_level > 3)
            {
                send_to_char( "You must keep your hands free for combat!\n\r", ch);
                return;
            }

            if ( obj->item_type == ITEM_LIGHT )
            {
                act( "$n lights $p and clutches it in $s left hand.", ch, obj, NULL, TO_ROOM );
                act( "You light $p and clutch it in your left hand.",  ch, obj, NULL, TO_CHAR );
            }
            else
            {
                act( "$n clutches $p in $s left hand.",    ch, obj, NULL, TO_ROOM );
                act( "You clutch $p in your left hand.",  ch, obj, NULL, TO_CHAR );
            }
            if (obj->item_type == ITEM_WEAPON)
            {
                if ( !IS_NPC(ch) && (obj->pIndexData->vnum == 30000 || IS_OBJ_STAT(obj,ITEM_LOYAL)))
                {
                    if ( obj->questowner != NULL && str_cmp(ch->pcdata->switchname,obj->questowner) && strlen(obj->questowner) > 1 )
                    {
                        act( "$p leaps out of $n's hand.", ch, obj, NULL, TO_ROOM );
                        act( "$p leaps out of your hand.", ch, obj, NULL, TO_CHAR );
                        obj_from_char(obj);
                        obj_to_room(obj,ch->in_room);
                        return;
                    }
                }
                equip_char( ch, obj, WEAR_HOLD );
                if (!IS_NPC(ch)) do_skill(ch, ch->name);
                return;
            }
            equip_char( ch, obj, WEAR_HOLD );
            return;
        }
        else if (get_eq_char( ch, WEAR_THIRD ) == NULL && is_ok_to_wear(ch,wolf_ok,"third_hand"))
        {
            if ( obj->item_type == ITEM_LIGHT )
            {
                act( "$n lights $p and clutches it in $s right hand.", ch, obj, NULL, TO_ROOM );
                act( "You light $p and clutch it in your right hand.", ch, obj, NULL, TO_CHAR );
            }
            else
            {
                act( "$n clutches $p in $s third hand.",    ch, obj, NULL, TO_ROOM );
                act( "You clutch $p in your third hand.",  ch, obj, NULL, TO_CHAR );
            }
            equip_char( ch, obj, WEAR_THIRD );
            return;
        }
        else if (get_eq_char( ch, WEAR_FOURTH ) == NULL && is_ok_to_wear(ch,wolf_ok,"fourth_hand"))
        {
            if ( obj->item_type == ITEM_LIGHT )
            {
                act( "$n lights $p and clutches it in $s fourth hand.", ch, obj, NULL, TO_ROOM );
                act( "You light $p and clutch it in your fourth hand.", ch, obj, NULL, TO_CHAR );
            }
            else
            {
                act( "$n clutches $p in $s fourth hand.",    ch, obj, NULL, TO_ROOM );
                act( "You clutch $p in your fourth hand.",  ch, obj, NULL, TO_CHAR );
            }
            if (obj->item_type == ITEM_WEAPON)
            {
                if ( !IS_NPC(ch) && ( obj->pIndexData->vnum == 30000 || IS_OBJ_STAT(obj,ITEM_LOYAL)))
                {
                    if ( obj->questowner != NULL && str_cmp(ch->pcdata->switchname,obj->questowner) && strlen(obj->questowner) > 1 )
                    {
                        act( "$p leaps out of $n's hand.", ch, obj, NULL, TO_ROOM );
                        act( "$p leaps out of your hand.", ch, obj, NULL, TO_CHAR );
                        obj_from_char(obj);
                        obj_to_room(obj,ch->in_room);
                        return;
                    }
                }
                equip_char( ch, obj, WEAR_FOURTH );
                if (!IS_NPC(ch)) do_skill(ch, ch->name);
                return;
            }
            equip_char( ch, obj, WEAR_FOURTH );
            return;
        }
        send_to_char( "You have no free hands.\n\r", ch );
        return;
    }
    if ( obj->item_type == ITEM_LIGHT )
    {
        if ( !remove_obj( ch, WEAR_LIGHT, fReplace ) )
            return;
        act( "$n lights $p and holds it.", ch, obj, NULL, TO_ROOM );
        act( "You light $p and hold it.",  ch, obj, NULL, TO_CHAR );
        equip_char( ch, obj, WEAR_LIGHT );
        return;
    }

    if ( CAN_WEAR( obj, ITEM_WEAR_FINGER ) )
    {
        if ( get_eq_char( ch, WEAR_FINGER_L ) != NULL
            &&   get_eq_char( ch, WEAR_FINGER_R ) != NULL
            &&   !remove_obj( ch, WEAR_FINGER_L, fReplace )
            &&   !remove_obj( ch, WEAR_FINGER_R, fReplace ) )
            return;
        if (!IS_NPC(ch) && !IS_FORM(ch,ITEM_WEAR_FINGER))
        {
            send_to_char("You are unable to wear it.\n\r",ch);
            return;
        }

        if ( get_eq_char( ch, WEAR_FINGER_L ) == NULL && is_ok_to_wear(ch,wolf_ok,"left_finger"))
        {
            act( "$n wears $p on $s left finger.",    ch, obj, NULL, TO_ROOM );
            act( "You wear $p on your left finger.",  ch, obj, NULL, TO_CHAR );
            equip_char( ch, obj, WEAR_FINGER_L );
            return;
        }
        else if ( get_eq_char( ch, WEAR_FINGER_R ) == NULL && is_ok_to_wear(ch,wolf_ok,"right_finger"))
        {
            act( "$n wears $p on $s right finger.",   ch, obj, NULL, TO_ROOM );
            act( "You wear $p on your right finger.", ch, obj, NULL, TO_CHAR );
            equip_char( ch, obj, WEAR_FINGER_R );
            return;
        }
        if (!is_ok_to_wear(ch,wolf_ok,"left_finger") && !is_ok_to_wear(ch,wolf_ok,"right_finger"))
            send_to_char( "You cannot wear any rings.\n\r", ch );
        else
            send_to_char( "You cannot wear any more rings.\n\r", ch );
        return;
    }

    if ( CAN_WEAR( obj, ITEM_WEAR_NECK ) )
    {
        if ( get_eq_char( ch, WEAR_NECK_1 ) != NULL
            &&   get_eq_char( ch, WEAR_NECK_2 ) != NULL
            &&   !remove_obj( ch, WEAR_NECK_1, fReplace )
            &&   !remove_obj( ch, WEAR_NECK_2, fReplace ) )
            return;
        if (!IS_NPC(ch) && !IS_FORM(ch,ITEM_WEAR_NECK))
        {
            send_to_char("You are unable to wear it.\n\r",ch);
            return;
        }

        if ( get_eq_char( ch, WEAR_NECK_1 ) == NULL )
        {
            act( "$n slips $p around $s neck.",   ch, obj, NULL, TO_ROOM );
            act( "You slip $p around your neck.", ch, obj, NULL, TO_CHAR );
            equip_char( ch, obj, WEAR_NECK_1 );
            return;
        }

        if ( get_eq_char( ch, WEAR_NECK_2 ) == NULL )
        {
            act( "$n slips $p around $s neck.",   ch, obj, NULL, TO_ROOM );
            act( "You slip $p around your neck.", ch, obj, NULL, TO_CHAR );
            equip_char( ch, obj, WEAR_NECK_2 );
            return;
        }
        bug( "Wear_obj: no free neck.", 0 );
        send_to_char( "You are already wearing two things around your neck.\n\r", ch );
        return;
    }

    if ( CAN_WEAR( obj, ITEM_WEAR_BODY ) )
    {
        if ( !remove_obj( ch, WEAR_BODY, fReplace ) )
            return;

        if (!IS_NPC(ch) && !IS_FORM(ch,ITEM_WEAR_BODY))
        {
            send_to_char("You are unable to wear it.\n\r",ch);
            return;
        }
        act( "$n fits $p on $s body.",   ch, obj, NULL, TO_ROOM );
        act( "You fit $p on your body.", ch, obj, NULL, TO_CHAR );
        equip_char( ch, obj, WEAR_BODY );
        return;
    }

    if ( CAN_WEAR( obj, ITEM_WEAR_HEAD ) )
    {
        if ( !remove_obj( ch, WEAR_HEAD, fReplace ) )
            return;
        if (!IS_NPC(ch) && !IS_FORM(ch,ITEM_WEAR_HEAD))
        {
            send_to_char("You are unable to wear it.\n\r",ch);
            return;
        }
        if (!is_ok_to_wear(ch,wolf_ok,"head"))
        {
            send_to_char("You have no head to wear it on.\n\r",ch);
            return;
        }
        act( "$n places $p on $s head.",   ch, obj, NULL, TO_ROOM );
        act( "You place $p on your head.", ch, obj, NULL, TO_CHAR );
        equip_char( ch, obj, WEAR_HEAD );
        return;
    }

    if ( CAN_WEAR( obj, ITEM_WEAR_FACE ) )
    {
        if ( !remove_obj( ch, WEAR_FACE, fReplace ) )
            return;
        if (!IS_NPC(ch) && !IS_FORM(ch,ITEM_WEAR_HEAD))
        {
            send_to_char("You are unable to wear it.\n\r",ch);
            return;
        }
        if (!is_ok_to_wear(ch,wolf_ok,"face"))
        {
            send_to_char("You have no face to wear it on.\n\r",ch);
            return;
        }
        act( "$n places $p on $s face.",   ch, obj, NULL, TO_ROOM );
        act( "You place $p on your face.", ch, obj, NULL, TO_CHAR );
        equip_char( ch, obj, WEAR_FACE );
        return;
    }

    if ( CAN_WEAR( obj, ITEM_WEAR_LEGS ) )
    {
        if ( !remove_obj( ch, WEAR_LEGS, fReplace ) )
            return;
        if (!IS_NPC(ch) && !IS_FORM(ch,ITEM_WEAR_LEGS))
        {
            send_to_char("You are unable to wear it.\n\r",ch);
            return;
        }
        if (!is_ok_to_wear(ch,wolf_ok,"legs"))
        {
            send_to_char("You have no legs to wear them on.\n\r",ch);
            return;
        }
        act( "$n slips $s legs into $p.",   ch, obj, NULL, TO_ROOM );
        act( "You slip your legs into $p.", ch, obj, NULL, TO_CHAR );
        equip_char( ch, obj, WEAR_LEGS );
        return;
    }

    if ( CAN_WEAR( obj, ITEM_WEAR_FEET ) )
    {
        if ( !remove_obj( ch, WEAR_FEET, fReplace ) )
            return;
        if (!IS_NPC(ch) && !IS_FORM(ch,ITEM_WEAR_FEET))
        {
            send_to_char("You are unable to wear it.\n\r",ch);
            return;
        }
        if (!is_ok_to_wear(ch,wolf_ok,"feet"))
        {
            send_to_char("You have no feet to wear them on.\n\r",ch);
            return;
        }
        act( "$n slips $s feet into $p.",   ch, obj, NULL, TO_ROOM );
        act( "You slip your feet into $p.", ch, obj, NULL, TO_CHAR );
        equip_char( ch, obj, WEAR_FEET );
        return;
    }

    if ( CAN_WEAR( obj, ITEM_WEAR_HANDS ) )
    {
        if ( !remove_obj( ch, WEAR_HANDS, fReplace ) )
            return;
        if (!IS_NPC(ch) && !IS_FORM(ch,ITEM_WEAR_HANDS))
        {
            send_to_char("You are unable to wear it.\n\r",ch);
            return;
        }
        if (!is_ok_to_wear(ch,wolf_ok,"hands"))
        {
            send_to_char("You have no hands to wear them on.\n\r",ch);
            return;
        }
        act( "$n pulls $p onto $s hands.",   ch, obj, NULL, TO_ROOM );
        act( "You pull $p onto your hands.", ch, obj, NULL, TO_CHAR );
        equip_char( ch, obj, WEAR_HANDS );
        return;
    }

    if ( CAN_WEAR( obj, ITEM_WEAR_ARMS ) )
    {
        if ( !remove_obj( ch, WEAR_ARMS, fReplace ) )
            return;
        if (!IS_NPC(ch) && !IS_FORM(ch,ITEM_WEAR_ARMS))
        {
            send_to_char("You are unable to wear it.\n\r",ch);
            return;
        }
        if (!is_ok_to_wear(ch,wolf_ok,"arms"))
        {
            send_to_char("You have no arms to wear them on.\n\r",ch);
            return;
        }
        act( "$n slides $s arms into $p.",   ch, obj, NULL, TO_ROOM );
        act( "You slide your arms into $p.", ch, obj, NULL, TO_CHAR );
        equip_char( ch, obj, WEAR_ARMS );
        return;
    }

    if ( CAN_WEAR( obj, ITEM_WEAR_ABOUT ) )
    {
        if ( !remove_obj( ch, WEAR_ABOUT, fReplace ) )
            return;
        if (!IS_NPC(ch) && !IS_FORM(ch,ITEM_WEAR_ABOUT))
        {
            send_to_char("You are unable to wear it.\n\r",ch);
            return;
        }
        act( "$n pulls $p about $s body.",   ch, obj, NULL, TO_ROOM );
        act( "You pull $p about your body.", ch, obj, NULL, TO_CHAR );
        equip_char( ch, obj, WEAR_ABOUT );
        return;
    }

    if ( CAN_WEAR( obj, ITEM_WEAR_WAIST ) )
    {
        if ( !remove_obj( ch, WEAR_WAIST, fReplace ) )
            return;
        if (!IS_NPC(ch) && !IS_FORM(ch,ITEM_WEAR_WAIST))
        {
            send_to_char("You are unable to wear it.\n\r",ch);
            return;
        }
        act( "$n ties $p around $s waist.",   ch, obj, NULL, TO_ROOM );
        act( "You tie $p around your waist.", ch, obj, NULL, TO_CHAR );
        equip_char( ch, obj, WEAR_WAIST );
        return;
    }

    if ( CAN_WEAR( obj, ITEM_WEAR_SPECIAL ) )
    {
        if ( !remove_obj( ch, WEAR_SPECIAL, fReplace ) )
            return;
        if (!IS_NPC(ch) && !IS_FORM(ch,ITEM_WEAR_SPECIAL))
        {
            send_to_char("You are unable to wear it.\n\r",ch);
            return;
        }
        act( "$n holds $p in $s hands.",   ch, obj, NULL, TO_ROOM );
        act( "You hold onto $p.", ch, obj, NULL, TO_CHAR );
        equip_char( ch, obj, WEAR_SPECIAL );
        return;
    }

    if ( CAN_WEAR( obj, ITEM_WEAR_FLOAT ) )
    {
        if ( !remove_obj( ch, WEAR_FLOAT, fReplace ) )
            return;
        if (!IS_NPC(ch) && !IS_FORM(ch,ITEM_WEAR_FLOAT))
        {
            send_to_char("You are unable to wear it, perhaps you suck\n\r",ch);
            return;
        }
        act( "$n throws $p into the air and it starts flying about.",   ch, obj, NULL, TO_ROOM );
        act( "You throw $p into the air and it starts circling around.", ch, obj, NULL, TO_CHAR );
        equip_char( ch, obj, WEAR_FLOAT );
        return;
    }

    if ( CAN_WEAR( obj, ITEM_WEAR_MEDAL ) )
    {
        if ( !remove_obj( ch, WEAR_MEDAL, fReplace ) )
            return;
        if (!IS_NPC(ch) && !IS_FORM(ch,ITEM_WEAR_MEDAL))
        {
            send_to_char("You are unable to wear it.\n\r",ch);
            return;
        }
        act( "$n puts $p on $s uniform.",   ch, obj, NULL, TO_ROOM );
        act( "You put $p on your uniform.", ch, obj, NULL, TO_CHAR );
        equip_char( ch, obj, WEAR_MEDAL );
        return;
    }

    if ( CAN_WEAR( obj, ITEM_WEAR_BODYART ) )
    {
        if ( !remove_obj( ch, WEAR_BODYART, fReplace ) )
            return;
        if (!IS_NPC(ch) && !IS_FORM(ch,ITEM_WEAR_BODYART))
        {
            send_to_char("You are unable to wear it.\n\r",ch);
            return;
        }
        act( "$n screams in agony as $p bores into $s body.",   ch, obj, NULL, TO_ROOM );
        act( "You scream in agony as $p bores into your body.", ch, obj, NULL, TO_CHAR );
        equip_char( ch, obj, WEAR_BODYART );
        return;
    }

    if ( CAN_WEAR( obj, ITEM_WEAR_WRIST ) )
    {
        if ( get_eq_char( ch, WEAR_WRIST_L ) != NULL
            &&   get_eq_char( ch, WEAR_WRIST_R ) != NULL
            &&   !remove_obj( ch, WEAR_WRIST_L, fReplace )
            &&   !remove_obj( ch, WEAR_WRIST_R, fReplace ) )
            return;
        if (!IS_NPC(ch) && !IS_FORM(ch,ITEM_WEAR_WRIST))
        {
            send_to_char("You are unable to wear it.\n\r",ch);
            return;
        }

        if ( get_eq_char( ch, WEAR_WRIST_L ) == NULL && is_ok_to_wear(ch,wolf_ok,"right_wrist"))
        {
            act( "$n slides $s left wrist into $p.",
                ch, obj, NULL, TO_ROOM );
            act( "You slide your left wrist into $p.",
                ch, obj, NULL, TO_CHAR );
            equip_char( ch, obj, WEAR_WRIST_L );
            return;
        }
        else if ( get_eq_char( ch, WEAR_WRIST_R ) == NULL && is_ok_to_wear(ch,wolf_ok,"left_wrist"))
        {
            act( "$n slides $s left wrist into $p.",
                ch, obj, NULL, TO_ROOM );
            act( "You slide your right wrist into $p.",
                ch, obj, NULL, TO_CHAR );
            equip_char( ch, obj, WEAR_WRIST_R );
            return;
        }
        if (!is_ok_to_wear(ch,wolf_ok,"left_wrist") && !is_ok_to_wear(ch,wolf_ok,"right_wrist"))
            send_to_char( "You cannot wear anything on your wrists.\n\r", ch );
        else
            send_to_char( "You cannot wear any more on your wrists.\n\r", ch );
        return;
    }

    if ( CAN_WEAR( obj, ITEM_WEAR_SHIELD ) )
    {
        if ( !remove_obj( ch, WEAR_SHIELD, fReplace ) )
            return;
        if (!IS_NPC(ch) && !IS_FORM(ch,ITEM_WEAR_SHIELD))
        {
            send_to_char("You are unable to wear it.\n\r",ch);
            return;
        }
        act( "$n straps $p onto $s shield arm.", ch, obj, NULL, TO_ROOM );
        act( "You strap $p onto your shield arm.", ch, obj, NULL, TO_CHAR );
        equip_char( ch, obj, WEAR_SHIELD );
        return;
    }

    if ( CAN_WEAR( obj, ITEM_WIELD ) )
    {
        if ( !remove_obj( ch, WEAR_WIELD, fReplace ) )
            return;
        if (!IS_NPC(ch) && !IS_FORM(ch,ITEM_WIELD))
        {
            send_to_char("You are unable to wield it.\n\r",ch);
            return;
        }

        /*if ( get_obj_weight( obj ) > str_app[get_curr_str(ch)].wield )
        {
            send_to_char( "It is too heavy for you to wield.\n\r", ch );
            return;
        }*/

        act( "$n wields $p.", ch, obj, NULL, TO_ROOM );
        act( "You wield $p.", ch, obj, NULL, TO_CHAR );

        if ( !IS_NPC(ch) && ( obj->pIndexData->vnum == 30000 || IS_OBJ_STAT(obj,ITEM_LOYAL)))
        {
            if ( obj->questowner != NULL && str_cmp(ch->pcdata->switchname,obj->questowner) && strlen(obj->questowner) > 1 )
            {
                act( "$p leaps out of $n's hand.", ch, obj, NULL, TO_ROOM );
                act( "$p leaps out of your hand.", ch, obj, NULL, TO_CHAR );
                obj_from_char(obj);
                obj_to_room(obj,ch->in_room);
                return;
            }
        }
        equip_char( ch, obj, WEAR_WIELD );
        if (!IS_NPC(ch)) do_skill(ch, ch->name);
        return;
    }

    if ( fReplace )
        send_to_char( "You can't wear, wield or hold that.\n\r", ch );

    return;
}

void do_wear( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;

    one_argument( argument, arg );

    if(IS_NPC(ch))
    {
        return;
    }

    if (!IS_NPC(ch) && IS_POLYAFF(ch,POLY_ZULOFORM) && IS_CLASS(ch, CLASS_VAMPIRE) && ch->power[DISC_VAMP_OBEA] < 9)
    {
        send_to_char( "You are too mad to decide what you want to wear!\n\r",ch);
        return;
    }
    if (!IS_NPC(ch) && IS_POLYAFF(ch,POLY_ZULOFORM) && IS_CLASS(ch, CLASS_VAMPIRE) && ch->power[DISC_VAMP_OBEA] > 8 && IS_CLASS(ch, CLASS_PHOENIX))
    {
        send_to_char( "#rYou force down your beast#n.\n\r",ch);
    }

    if (IS_POLYAFF(ch,POLY_SERPENT)||IS_POLYAFF(ch,POLY_BAT)||IS_POLYAFF(ch,POLY_WOLF))
    {
        send_to_char( "You cannot wear anything in serpentform because Joshua thinks it makes vamps rigged.\n\r", ch);
        return;
    }
    if (IS_AFFECTED(ch,AFF_POLYMORPH) && !IS_NPC(ch) && !IS_CLASS(ch, CLASS_ANGEL) &&
        !IS_VAMPAFF(ch,VAM_DISGUISED) && !IS_CLASS(ch,CLASS_SHAPESHIFTER) && !IS_CLASS(ch, CLASS_WEREWOLF) &&
        !IS_CLASS(ch, CLASS_DEMON) && ch->power[DISC_VAMP_OBEA] < 9 && !IS_CLASS(ch, CLASS_DROID)
        && !IS_CLASS(ch, CLASS_KAHN))
    {
        send_to_char( "You cannot wear anything in this form.\n\r", ch );
        return;
    }

    if ( arg[0] == '\0' )
    {
        send_to_char( "Wear, wield, or hold what?\n\r", ch );
        return;
    }

    if ( !str_cmp( arg, "all" ) )
    {
        OBJ_DATA *obj_next;

        for ( obj = ch->carrying; obj != NULL; obj = obj_next )
        {
            obj_next = obj->next_content;
            if ( obj->wear_loc == WEAR_NONE && can_see_obj( ch, obj ) )
                wear_obj( ch, obj, FALSE );
        }
        return;
    }
    else
    {
        if ( ( obj = get_obj_carry( ch, arg ) ) == NULL )
        {
            send_to_char( "You do not have that item.\n\r", ch );
            return;
        }

        wear_obj( ch, obj, TRUE );
    }

    return;
}

void do_remove( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    int sn;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
        send_to_char( "Remove what?\n\r", ch );
        return;
    }

    if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_SAMURAI) && IS_POLYAFF(ch, POLY_ZULOFORM))
    {
        send_to_char("Your too mad to remove anything!\n\r",ch);
        return;
    }

    if( !IS_NPC(ch) && IS_CLASS(ch, CLASS_VAMPIRE) && IS_POLYAFF(ch, POLY_ZULOFORM) && ch->generation > 1 &&
        ch->power[DISC_VAMP_OBEA] < 9)
    {
        stc( "You are too mad to remove anything!\n\r", ch);
        return;
    }
    if (!IS_NPC(ch) && IS_POLYAFF(ch,POLY_ZULOFORM) && IS_CLASS(ch, CLASS_VAMPIRE) && ch->power[DISC_VAMP_OBEA] > 8)
    {
        send_to_char( "#rYou force down yer beast#n.\n\r",ch);
    }

    if ( !str_cmp( arg, "all" ) )
    {
        OBJ_DATA *obj_next;

        for ( obj = ch->carrying; obj != NULL; obj = obj_next )
        {
            obj_next = obj->next_content;
            if ( obj->wear_loc != WEAR_NONE && can_see_obj( ch, obj ) )
            {
                if (obj->item_type == ITEM_ARMOR)
                    sn = obj->value[3];
                else
                    sn = obj->value[0] / 1000;
                if( sn != 25 )
                    remove_obj( ch, obj->wear_loc, TRUE);
            }
        }
        return;
    }
    if ( ( obj = get_obj_wear( ch, arg ) ) == NULL )
    {
        send_to_char( "You do not have that item.\n\r", ch );
        return;
    }
    if (obj->item_type == ITEM_ARMOR)
        sn = obj->value[3];
    else
        sn = obj->value[0] / 1000;
    if( sn != 25)
        remove_obj( ch, obj->wear_loc, TRUE );
    else if(!IS_CLASS(ch, CLASS_WEREWOLF))
        remove_obj( ch, obj->wear_loc, TRUE );
    else
    {
        stc( "Do to abuse you can't remove wear raging items as a werewolf.\n\r",ch);
    }
    return;
}

void do_sacrifice( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    OBJ_DATA *next_obj;
    char buf[MAX_INPUT_LENGTH];
    char imm2[MAX_INPUT_LENGTH];
    int questb;
    int i=0;
    int b2;
    int nCount = 0;
    one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if ( arg[0] == '\0' )
    {
        send_to_char("Sacrifice what?\n\r",ch);
        return;
    }
    if( IS_AFFECTED(ch, AFF_CHARM) )
    {
        stc( "Not while charmed.\n\r", ch);
        return;
    }
    if (!str_cmp(arg, "all") || !str_prefix("all." ,arg))
    {
        next_obj = ch->in_room->contents;
        for (obj = ch->in_room->contents; next_obj != NULL; obj = next_obj )
        {
            next_obj = obj->next_content;
            if (arg[3] != '\0' && !is_name(&arg[4], obj->name)) continue;
            if (++i > 35) break;
            nCount++;
            if (!CAN_WEAR(obj, ITEM_TAKE) || (obj->item_type == ITEM_XPTOKEN) || (obj->item_type == ITEM_PRTOKEN)
                || (obj->item_type == ITEM_QUEST) || IS_SET(obj->quest, QUEST_ARTIFACT) || (obj->item_type == ITEM_STORETOKEN)
                || ((obj->questowner != NULL) && (strlen(obj->questowner) > 1) && str_cmp(ch->name,obj->questowner)))
            {
                act("You are unable to drain any energy from $p.", ch, obj, 0, TO_CHAR );
                nCount--;
                continue;
            }
            else if (obj->chobj != NULL && !IS_NPC(obj->chobj) && obj->chobj->pcdata->obj_vnum != 0)
            {
                act( "You are unable to drain any energy from $p.", ch, obj, 0, TO_CHAR );
                nCount--;
                continue;
            }
            if (obj->points > 0 && !IS_NPC(ch) && obj->item_type != ITEM_PAGE)
            {
                sprintf( buf, "You receive a refund of %d quest points from $p.", obj->points);
                act( buf, ch, obj, NULL, TO_CHAR );
                ch->pcdata->quest += obj->points;
            }
            if (!IS_NPC(ch) && obj->pIndexData->vnum > 813 && obj->pIndexData->vnum < 826)
            {
                ch->pcdata->rune_count--;
                send_to_char("you get back your quest points from the rune eq.\n\r",ch);
                ch->pcdata->quest += 25000;
            }
            if (obj->pIndexData->vnum >= 101000 && obj->pIndexData->vnum <= 101016)
            {
                ch->pcdata->quest += 150000;
                send_to_char("You get back your quest points for the relic.\n\r",ch);
            }
            if (obj->pIndexData->vnum > 91 && obj->pIndexData->vnum < 93)
            {
                ch->pcdata->quest += 15000;
                send_to_char("You get a bonus for sacrificing a status flake.\n\r",ch);
            }
            if (obj->pIndexData->vnum > 89 && obj->pIndexData->vnum < 92)
            {
                ch->pcdata->quest += 6000;
                send_to_char("You get a bonus for sacrificing a status piece.\n\r",ch);
            }
            else if (obj->pIndexData->vnum == 101021 )
            {
                ch->pcdata->quest += 75000;
                send_to_char("Object is GONE.\n\r",ch);
            }
            else if (obj->pIndexData->vnum == 101020 )
            {
                ch->pcdata->quest += 47500;
                send_to_char("Object is GONE.\n\r",ch);
            }
            else if (obj->pIndexData->vnum == 72016 )
            {
                ch->pcdata->quest += 37500;
                send_to_char("Object is GONE.\n\r",ch);
            }
            extract_obj( obj );
        }
        if (i == 0)
        {
            send_to_char("Nothing found.\n\r",ch);
            return;
        }
        else
        {
            act( "$n destroys most of the items in the room.", ch, NULL, NULL, TO_ROOM);
            questb = nCount*number_range(0,10);
            b2 = nCount*number_range(1,100);
            sprintf(imm2, "You get a bonus of %d qps, and %d exp for sacrificing to Jinnai.\n\r", questb, b2);
            send_to_char(imm2,ch);
            ch->pcdata->quest += questb;
            ch->exp += b2;
        }
        return;
    }
    obj = get_obj_list( ch, arg, ch->in_room->contents );
    if ( obj == NULL )
    {
        send_to_char( "You can't find it.\n\r", ch );
        return;
    }
    if ( !CAN_WEAR(obj, ITEM_TAKE) || obj->item_type == ITEM_QUEST ||
        obj->item_type == ITEM_XPTOKEN || obj->item_type == ITEM_PRTOKEN ||
        obj->item_type == ITEM_MONEY || obj->item_type == ITEM_TREASURE ||
        IS_SET(obj->quest, QUEST_ARTIFACT) || obj->item_type == ITEM_STORETOKEN ||
        ( obj->questowner != NULL && strlen(obj->questowner) > 1 && str_cmp(ch->name,obj->questowner)))
    {
        act( "You are unable to drain any energy from $p.", ch, obj, 0, TO_CHAR );
        return;
    }
    else if (obj->chobj != NULL && !IS_NPC(obj->chobj) &&
        obj->chobj->pcdata->obj_vnum != 0)
    {
        act( "You are unable to drain any energy from $p.", ch, obj, 0, TO_CHAR );
        return;
    }
    act( "$p gets sacrificed to an immortal.", ch, obj, NULL, TO_CHAR );
    act( "$n sacrifices $p to an immortal.", ch, obj, NULL, TO_ROOM );
    act( "$p gets turned into dust.", ch, obj, NULL, TO_ROOM );
    questb = number_range(0,10);
    b2 = number_range(1,1000);
    sprintf(imm2, "You get a bonus of %d qps, and %d exp for sacrificing to Jinnai.\n\r", questb, b2);
    send_to_char(imm2,ch);
    ch->pcdata->quest += questb;
    ch->exp += b2;
    if (obj->points > 0 && !IS_NPC(ch) && obj->item_type != ITEM_PAGE)
    {
        sprintf( buf, "You receive a refund of %d quest points from $p.", obj->points);
        act( buf, ch, obj, NULL, TO_CHAR );
        ch->pcdata->quest += obj->points;
    }
    if (obj->pIndexData->vnum > 813 && obj->pIndexData->vnum < 826)
    {
        ch->pcdata->rune_count--;
        send_to_char("You get back your quest points from the rune eq.\n\r",ch);
        ch->pcdata->quest += 50000;
    }
    if (obj->pIndexData->vnum >= 101000 && obj->pIndexData->vnum <= 101016)
    {
        ch->pcdata->quest += 150000;
        send_to_char("You get back your quest points for the relic.\n\r",ch);
    }
    else if (obj->pIndexData->vnum == 101021 )
    {
        ch->pcdata->quest += 75000;
        send_to_char("Object is GONE.\n\r",ch);
    }
    else if (obj->pIndexData->vnum == 101020 )
    {
        ch->pcdata->quest += 47500;
        send_to_char("Object is GONE.\n\r",ch);
    }
    else if (obj->pIndexData->vnum == 72016 )
    {
        ch->pcdata->quest += 37500;
        send_to_char("Object is GONE.\n\r",ch);
    }
    extract_obj( obj );
    return;
}

void do_quaff( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    int level;

    one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if ( arg[0] == '\0' )
    {
        send_to_char( "Quaff what?\n\r", ch );
        return;
    }

    if ( ( obj = get_obj_carry( ch, arg ) ) == NULL )
    {
        send_to_char( "You do not have that potion.\n\r", ch );
        return;
    }

    if ( obj->item_type != ITEM_POTION )
    {
        send_to_char( "You can quaff only potions.\n\r", ch );
        return;
    }
    if (IS_NPC(ch)) return;

    act( "$n quaffs $p.", ch, obj, NULL, TO_ROOM );
    act( "You quaff $p.", ch, obj, NULL ,TO_CHAR );

    level = obj->value[0];
    if (level < 1) level = 1;
    if (level > MAX_SPELL) level = MAX_SPELL;

    obj_cast_spell( obj->value[1], level, ch, ch, NULL );
    obj_cast_spell( obj->value[2], level, ch, ch, NULL );
    obj_cast_spell( obj->value[3], level, ch, ch, NULL );

    extract_obj( obj );
    if (ch->position == POS_FIGHTING)
    {
        if (!IS_IMMORTAL( ch))
            WAIT_STATE(ch, 6 );
    }
    return;
}

void do_recite( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    OBJ_DATA *scroll;
    OBJ_DATA *obj;
    int level;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if (IS_NPC(ch)) return;

    if ( ( scroll = get_obj_carry( ch, arg1 ) ) == NULL )
    {
        send_to_char( "You do not have that scroll.\n\r", ch );
        return;
    }

    if ( scroll->item_type != ITEM_SCROLL )
    {
        send_to_char( "You can recite only scrolls.\n\r", ch );
        return;
    }

    obj = NULL;
    if ( arg2[0] == '\0' )
    {
        victim = NULL;
    }
    else
    {
        if ( ( victim = get_char_room ( ch, arg2 ) ) == NULL
            &&   ( obj    = get_obj_here  ( ch, arg2 ) ) == NULL )
        {
            send_to_char( "You can't find it.\n\r", ch );
            return;
        }
        if( IS_SET(victim->act, ACT_DUMMY) && IS_NPC(victim) )
        {
            stc("Dummys are immune to pettyless scrolls.\n\r",ch);
            return;
        }
    }
    if (IS_NPC(ch)) return;

    act( "$n recites $p.", ch, scroll, NULL, TO_ROOM );
    act( "You recite $p.", ch, scroll, NULL, TO_CHAR );

    level = scroll->value[0];
    if (level < 1) level = 1;
    if (level > MAX_SPELL) level = MAX_SPELL;

    obj_cast_spell( scroll->value[1], level, ch, victim, obj );
    obj_cast_spell( scroll->value[2], level, ch, victim, obj );
    obj_cast_spell( scroll->value[3], level, ch, victim, obj );

    extract_obj( scroll );
    if (ch->position == POS_FIGHTING)
    {
        if (!IS_IMMORTAL( ch))
            WAIT_STATE(ch, 6 );
    }
    return;
}

void do_brandish( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;
    OBJ_DATA *temp;
    OBJ_DATA *staff;
    int sn;
    int level;

    staff = get_eq_char( ch, WEAR_WIELD );
    temp = get_eq_char( ch, WEAR_HOLD );

    if (IS_NPC(ch)) return;

    if ( staff == NULL && temp == NULL )
    {
        send_to_char( "You hold nothing in your hand.\n\r", ch );
        return;
    }

    if ( staff == NULL ) staff = temp;
    if ( temp == NULL  ) temp = staff;

    if ( staff->item_type != ITEM_STAFF ) staff=temp;

    if ( staff->item_type != ITEM_STAFF )
    {
        send_to_char( "You can brandish only with a staff.\n\r", ch );
        return;
    }

    if ( ( sn = staff->value[3] ) < 0
        ||   sn >= MAX_SKILL
        ||   skill_table[sn].spell_fun == 0 )
    {
        bug( "Do_brandish: bad sn %d.", sn );
        return;
    }
    if (IS_NPC(ch)) return;

    if (!IS_IMMORTAL( ch))
        WAIT_STATE( ch, 2 * PULSE_VIOLENCE );

    if ( staff->value[2] > 0 )
    {
        act( "$n brandishes $p.", ch, staff, NULL, TO_ROOM );
        act( "You brandish $p.",  ch, staff, NULL, TO_CHAR );
        for ( vch = ch->in_room->people; vch; vch = vch_next )
        {
            vch_next    = vch->next_in_room;

            switch ( skill_table[sn].target )
            {
                default:
                    bug( "Do_brandish: bad target for sn %d.", sn );
                    return;

                case TAR_IGNORE:
                    if ( vch != ch )
                        continue;
                    break;

                case TAR_CHAR_OFFENSIVE:
                    if ( IS_NPC(ch) ? IS_NPC(vch) : !IS_NPC(vch) )
                        continue;
                    break;

                case TAR_CHAR_DEFENSIVE:
                    if ( IS_NPC(ch) ? !IS_NPC(vch) : IS_NPC(vch) )
                        continue;
                    break;

                case TAR_CHAR_SELF:
                    if ( vch != ch )
                        continue;
                    break;
            }

            level = staff->value[0];
            if (level < 1) level = 1;
            if (level > MAX_SPELL) level = MAX_SPELL;

            obj_cast_spell( staff->value[3], level, ch, vch, NULL );
        }
    }

    if ( --staff->value[2] <= 0 )
    {
        act( "$n's $p blazes bright and is gone.", ch, staff, NULL, TO_ROOM );
        act( "Your $p blazes bright and is gone.", ch, staff, NULL, TO_CHAR );
        extract_obj( staff );
    }

    return;
}

void do_zap( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    char arg1[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    OBJ_DATA *wand;
    OBJ_DATA *obj;
    int level;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if ( arg[0] == '\0' || arg1[0] == '\0' )
    {
        send_to_char( "Zap whom with what?\n\r", ch );
        return;
    }

    if ( ( wand = get_obj_carry( ch, arg1 ) ) == NULL )
    {
        send_to_char( "You do not have that wand.\n\r", ch );
        return;
    }
    if ( wand->item_type != ITEM_WAND )
    {
        send_to_char( "You can zap only with a wand.\n\r", ch );
        return;
    }

    obj = NULL;
    if ( ( victim = get_char_room ( ch, arg ) ) == NULL &&  ( obj  = get_obj_here  ( ch, arg ) ) == NULL )
    {
        send_to_char( "You can't find it.\n\r", ch );
        return;
    }

    WAIT_STATE( ch, 2 * PULSE_VIOLENCE );

    if ( wand->value[2] > 0 )
    {
        if ( victim != NULL )
        {
            act( "$n zaps $N with $p.", ch, wand, victim, TO_ROOM );
            act( "You zap $N with $p.", ch, wand, victim, TO_CHAR );
        }
        else
        {
            act( "$n zaps $P with $p.", ch, wand, obj, TO_ROOM );
            act( "You zap $P with $p.", ch, wand, obj, TO_CHAR );
        }

        level = wand->value[0];
        if (level < 1) level = 1;
        if (level > MAX_SPELL) level = MAX_SPELL;

        obj_cast_spell( wand->value[3], level, ch, victim, obj );
    }

    if ( --wand->value[2] <= 0 )
    {
        act( "$n's $p explodes into fragments.", ch, wand, NULL, TO_ROOM );
        act( "Your $p explodes into fragments.", ch, wand, NULL, TO_CHAR );
        extract_obj( wand );
    }

    return;
}

void do_steal( CHAR_DATA *ch, char *argument )
{
    char buf  [MAX_STRING_LENGTH];
    char arg1 [MAX_INPUT_LENGTH];
    char arg2 [MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    OBJ_DATA *obj;
    int percent;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if ( arg1[0] == '\0' || arg2[0] == '\0' )
    {
        send_to_char( "Steal what from whom?\n\r", ch );
        return;
    }

    if ( ( victim = get_char_room( ch, arg2 ) ) == NULL )
    {
        send_to_char( "They aren't here.\n\r", ch );
        return;
    }

    if ( victim == ch )
    {
        send_to_char( "That's pointless.\n\r", ch );
        return;
    }

    if ( IS_IMMORTAL( victim ))
    {
        send_to_char("Steal from an immortal are you crazy!\n\r", ch);
        return;
    }

    if (!IS_IMMORTAL( ch))
        WAIT_STATE( ch, skill_table[gsn_steal].beats );

    percent  = number_percent( ) + ( IS_AWAKE(victim) ? 10 : -50 );

    if ( ( ch->level + number_range(1,20) < victim->level      )
        ||   ( !IS_NPC(ch) && !IS_NPC(victim) && ch->level < 3     )
        ||   ( !IS_NPC(ch) && !IS_NPC(victim) && victim->level < 3     )
        ||   ( victim->position == POS_FIGHTING            )
        ||   ( !IS_NPC(victim) && IS_IMMUNE(victim,IMM_STEAL)      )
        ||   ( !IS_NPC(victim) && IS_IMMORTAL(victim)          )
        ||   ( !IS_NPC(ch) && percent > ch->pcdata->learned[gsn_steal] ) )
    {
        /*
         * Failure.
         */
        send_to_char( "Oops.\n\r", ch );
        act( "$n tried to steal from you.\n\r", ch, NULL, victim, TO_VICT    );
        act( "$n tried to steal from $N.\n\r",  ch, NULL, victim, TO_NOTVICT );
        sprintf( buf, "%s is a bloody thief!", ch->name );
        do_yell( victim, buf );
        if ( !IS_NPC(ch) )
        {
            if ( IS_NPC(victim) )
            {
                multi_hit( victim, ch, TYPE_UNDEFINED );
            }
            else
            {
                wiznet( NULL, WIZ_DEATH, 0, buf );
                log_string( buf );
                save_char_obj( ch );
            }
        }

        return;
    }

    if ( ( obj = get_obj_carry( victim, arg1 ) ) == NULL )
    {
        send_to_char( "You can't find it.\n\r", ch );
        return;
    }

    if ( !can_drop_obj( ch, obj )
        ||   IS_SET(obj->extra_flags, ITEM_LOYAL)
        ||   IS_SET(obj->extra_flags, ITEM_INVENTORY) )
    {
        send_to_char( "You can't pry it away.\n\r", ch );
        return;
    }

    if ( ch->carry_number + 1 > can_carry_n( ch ) )
    {
        send_to_char( "You have your hands full.\n\r", ch );
        return;
    }

    if ( ch->carry_weight + get_obj_weight( obj ) > can_carry_w(ch) )
    {
        send_to_char( "You can't carry that much weight.\n\r", ch );
        return;
    }

    obj_from_char( obj );
    obj_to_char( obj, ch );
    send_to_char( "You got it!\n\r", ch );
    do_autosave(ch,"");
    do_autosave(victim,"");
    return;
}

/*
 * Shopping commands.
 */
CHAR_DATA *find_keeper( CHAR_DATA *ch )
{
    CHAR_DATA *keeper;
    SHOP_DATA *pShop;
    char buf [MAX_STRING_LENGTH];

    pShop = NULL;
    for ( keeper = ch->in_room->people; keeper; keeper = keeper->next_in_room )
    {
        if ( IS_NPC(keeper) && (pShop = keeper->pIndexData->pShop) != NULL )
            break;
    }

    if ( pShop == NULL )
    {
        send_to_char( "You can't do that here.\n\r", ch );
        return NULL;
    }

    /*
     * Shop hours.
     */
    if ( time_info.hour < pShop->open_hour )
    {
        sprintf( buf, "Sorry, come back later." );
        do_say( keeper, buf );
        return NULL;
    }

    if ( time_info.hour > pShop->close_hour )
    {
        sprintf( buf, "Sorry, come back tomorrow." );
        do_say( keeper, buf );
        return NULL;
    }

    /*
     * Invisible or hidden people.
     */
    if ( !can_see( keeper, ch ) )
    {
        sprintf( buf, "I don't trade with folks I can't see." );
        do_say( keeper, buf );
        return NULL;
    }

    return keeper;
}

int get_cost( CHAR_DATA *keeper, OBJ_DATA *obj, bool fBuy )
{
    SHOP_DATA *pShop;
    int cost;

    if ( obj == NULL || ( pShop = keeper->pIndexData->pShop ) == NULL )
        return 0;

    if ( fBuy )
    {
        cost = obj->cost * pShop->profit_buy  / 100;
    }
    else
    {
        OBJ_DATA *obj2;
        int itype;

        cost = 0;
        for ( itype = 0; itype < MAX_TRADE; itype++ )
        {
            if ( obj->item_type == pShop->buy_type[itype] )
            {
                cost = obj->cost * pShop->profit_sell / 100;
                break;
            }
        }

        for ( obj2 = keeper->carrying; obj2; obj2 = obj2->next_content )
        {
            if ( obj->pIndexData == obj2->pIndexData )
            {
                cost = 0;
                break;
            }
        }
    }

    if ( obj->item_type == ITEM_STAFF || obj->item_type == ITEM_WAND )
        cost = cost * obj->value[2] / obj->value[1];

    return cost;
}

void do_activate( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    OBJ_DATA *obj2;
    CHAR_DATA *victim;
    CHAR_DATA *victim_next;
    CHAR_DATA *mount;
    CHAR_DATA *mob;
    ROOM_INDEX_DATA *pRoomIndex;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    //  if (IS_NPC(ch)) return;
    //if(obj->pIndexData->vnum ==110970){
    //send_to_char("#CJoshua#n reminds you that he #Rloves#n you with all his #Rheart#n.\n\r",ch);
    //return;}
    if (IS_SET(ch->in_room->room_flags,ROOM_NO_OTRANS))
    {
        send_to_char("A strange force is preventing the item from working.\n\r",ch);
        return;
    }
    if ( arg1[0] == '\0' )
    {
        send_to_char( "Which item do you wish to activate?\n\r", ch );
        return;
    }
    if ( (obj = get_obj_wear(ch,arg1)) == NULL )
    {
        if ( ( obj = get_obj_here( ch, arg1 ) ) == NULL )
        {
            send_to_char( "You can't find that item.\n\r", ch );
            return;
        }
        /* You should only be able to use nontake items on floor */
        if (CAN_WEAR(obj, ITEM_TAKE))
        {
            send_to_char( "But you are not wearing it!\n\r", ch );
            return;
        }
    }

    if ( obj == NULL || !IS_SET(obj->spectype, SITEM_ACTIVATE) )
    {
        send_to_char( "This item cannot be activated.\n\r", ch );
        return;
    }
    if ( IS_SET(ch->in_room->room_flags, ROOM_NOTRANS) )
    {
        send_to_char( " Where is the button to this thing.\n\r",ch );
        return;
    }
    if ( IS_SET(ch->in_room->room_flags, ROOM_NO_OTRANS) )
    {
        send_to_char( " Where is the button to this thing.\n\r",ch );
        return;
    }

    if ( IS_SET(obj->spectype, SITEM_TARGET) && arg2[0] == '\0' )
    {
        send_to_char( "Who do you wish to activate it on?\n\r", ch );
        return;
    }
    if ( IS_SET(obj->spectype, SITEM_TARGET) )
    {
        if ( ( victim = get_char_room( ch, arg2 ) ) == NULL )
        {
            send_to_char( "Nobody here by that name.\n\r", ch );
            return;
        }
    }
    else victim = ch;
    if (obj->chpoweruse != NULL && obj->chpoweruse != '\0' && str_cmp(obj->chpoweruse,"(null)") && str_cmp(obj->chpoweruse,"normal"))
        kavitem(obj->chpoweruse,ch,obj,NULL,TO_CHAR);
    if (obj->victpoweruse != NULL && obj->victpoweruse != '\0' && str_cmp(obj->victpoweruse,"(null)") && str_cmp(obj->victpoweruse,"normal"))
        kavitem(obj->victpoweruse,ch,obj,NULL,TO_ROOM);
    if ( IS_SET(obj->spectype, SITEM_SPELL))
    {
        int castlevel = obj->level;
        if (castlevel < 1) castlevel = 1;
        else if (castlevel > 60) castlevel = 60;
        obj_cast_spell( obj->specpower, castlevel, ch, victim, NULL );
        if (!IS_IMMORTAL( ch))
            WAIT_STATE(ch,6);
        if ( IS_SET(obj->spectype, SITEM_DELAY1)) WAIT_STATE(ch,6);
        if ( IS_SET(obj->spectype, SITEM_DELAY2)) WAIT_STATE(ch,12);
        return;
    }
    else if ( IS_SET(obj->spectype, SITEM_TRANSPORTER))
    {
        if (obj->chpoweron != NULL && obj->chpoweron != '\0' && str_cmp(obj->chpoweron,"(null)") && str_cmp(obj->chpoweron,"normal"))
            kavitem(obj->chpoweron,ch,obj,NULL,TO_CHAR);
        if (obj->victpoweron != NULL && obj->victpoweron != '\0' && str_cmp(obj->victpoweron,"(null)") && str_cmp(obj->victpoweron,"normal"))
            kavitem(obj->victpoweron,ch,obj,NULL,TO_ROOM);
        pRoomIndex     = get_room_index(obj->specpower);
        obj->specpower = ch->in_room->vnum;
        if ( pRoomIndex == NULL ) return;
        char_from_room(ch);
        char_to_room(ch,pRoomIndex);
        do_look(ch,"auto");
        if (obj->chpoweroff != NULL && obj->chpoweroff != '\0' && str_cmp(obj->chpoweroff,"(null)") && str_cmp(obj->chpoweroff,"normal"))
            kavitem(obj->chpoweroff,ch,obj,NULL,TO_CHAR);
        if (obj->victpoweroff != NULL && obj->victpoweroff != '\0' && str_cmp(obj->victpoweroff,"(null)") && str_cmp(obj->victpoweroff,"normal"))
            kavitem(obj->victpoweroff,ch,obj,NULL,TO_ROOM);
        if (!IS_SET(obj->quest, QUEST_ARTIFACT) && ( IS_SET(ch->in_room->room_flags, ROOM_NO_TELEPORT) ||
            ch->in_room->vnum == 30000 || IS_SET(ch->in_room->room_flags, ROOM_ASTRAL) ) && CAN_WEAR(obj,ITEM_TAKE))
        {
            send_to_char("A powerful force hurls you from the room.\n\r",ch);
            act("$n is hurled from the room by a powerful force.",ch,NULL,NULL,TO_ROOM);
            ch->position = POS_STUNNED;
            char_from_room(ch);
            char_to_room(ch,get_room_index(ROOM_VNUM_TEMPLE));
            act("$n appears in the room, and falls to the ground stunned.",ch,NULL,NULL,TO_ROOM);
        }
        if ( (mount = ch->mount) == NULL) return;
        char_from_room( mount );
        char_to_room( mount, ch->in_room );
        do_look( mount, "auto" );
        return;
    }
    else if ( IS_SET(obj->spectype, SITEM_TELEPORTER))
    {
        if (obj->chpoweron != NULL && obj->chpoweron != '\0'
            && str_cmp(obj->chpoweron,"(null)") && str_cmp(obj->chpoweron,"normal"))
            kavitem(obj->chpoweron,ch,obj,NULL,TO_CHAR);
        if (obj->victpoweron != NULL && obj->victpoweron != '\0'
            && str_cmp(obj->victpoweron,"(null)") && str_cmp(obj->victpoweron,"normal"))
            kavitem(obj->victpoweron,ch,obj,NULL,TO_ROOM);
        pRoomIndex     = get_room_index(obj->specpower);
        if ( pRoomIndex == NULL ) return;
        char_from_room(ch);
        char_to_room(ch,pRoomIndex);
        do_look(ch,"auto");
        if (obj->chpoweroff != NULL && obj->chpoweroff != '\0'
            && str_cmp(obj->chpoweroff,"(null)") && str_cmp(obj->chpoweroff,"normal"))
            kavitem(obj->chpoweroff,ch,obj,NULL,TO_CHAR);
        if (obj->victpoweroff != NULL && obj->victpoweroff != '\0'
            && str_cmp(obj->victpoweroff,"(null)") && str_cmp(obj->victpoweroff,"normal"))
            kavitem(obj->victpoweroff,ch,obj,NULL,TO_ROOM);
        if (!IS_SET(obj->quest, QUEST_ARTIFACT) &&
            ( IS_SET(ch->in_room->room_flags, ROOM_NO_TELEPORT) || IS_SET(ch->in_room->room_flags, ROOM_ASTRAL) )
            &&
            CAN_WEAR(obj,ITEM_TAKE))
        {
            send_to_char("A powerful force hurls you from the room.\n\r",ch);
            act("$n is hurled from the room by a powerful force.",ch,NULL,NULL,TO_ROOM);
            ch->position = POS_STUNNED;
            char_from_room(ch);
            char_to_room(ch,get_room_index(ROOM_VNUM_TEMPLE));
            act("$n appears in the room, and falls to the ground stunned.",ch,NULL,NULL,TO_ROOM);
        }
        if ( (mount = ch->mount) == NULL) return;
        char_from_room( mount );
        char_to_room( mount, ch->in_room );
        do_look( mount, "auto" );
        return;
    }
    else if ( IS_SET(obj->spectype, SITEM_OBJECT))
    {
        if ( get_obj_index(obj->specpower) == NULL ) return;
        obj2 = create_object(get_obj_index(obj->specpower), ch->level);
        if ( CAN_WEAR(obj2,ITEM_TAKE) ) obj_to_char(obj2,ch);
        else                   obj_to_room(obj2,ch->in_room);
    }
    else if ( IS_SET(obj->spectype, SITEM_MOBILE))
    {
        if ( get_mob_index(obj->specpower) == NULL ) return;
        mob = create_mobile(get_mob_index(obj->specpower));
        char_to_room(mob,ch->in_room);
    }
    else if ( IS_SET(obj->spectype, SITEM_ACTION))
    {
        interpret(ch,obj->victpoweron);
        if (obj->victpoweroff != NULL && str_cmp(obj->victpoweroff,"(null)") &&
            obj->victpoweroff != '\0')
        {
            for ( victim = char_list; victim != NULL; victim = victim_next )
            {
                victim_next = victim->next;
                if ( victim->in_room == NULL ) continue;
                if ( victim == ch ) continue;
                if ( victim->in_room == ch->in_room )
                {
                    interpret(victim,obj->victpoweroff);
                    continue;
                }
            }
        }
    }
    return;
}

void do_press( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    OBJ_DATA *obj2;
    CHAR_DATA *victim;
    CHAR_DATA *victim_next;
    CHAR_DATA *mount;
    CHAR_DATA *mob;
    ROOM_INDEX_DATA *pRoomIndex;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if (IS_NPC(ch)) return;
    if (IS_SET(ch->in_room->room_flags,ROOM_NO_OTRANS))
    {
        send_to_char("A strange force is preventing the item from working.\n\r",ch);
        return;
    }

    if ( arg1[0] == '\0' )
    {
        send_to_char( "Which item do you wish to press?\n\r", ch );
        return;
    }
    if ( (obj = get_obj_wear(ch,arg1)) == NULL )
    {
        if ( ( obj = get_obj_here( ch, arg1 ) ) == NULL )
        {
            send_to_char( "You can't find that item.\n\r", ch );
            return;
        }
        /* You should only be able to use nontake items on floor */
        if (CAN_WEAR(obj, ITEM_TAKE))
        {
            send_to_char( "But you are not wearing it!\n\r", ch );
            return;
        }
    }
    if ( obj == NULL || !IS_SET(obj->spectype, SITEM_PRESS) )
    {
        send_to_char( "There is nothing on this item to press.\n\r", ch );
        return;
    }
    if ( IS_SET(ch->in_room->room_flags, ROOM_NOTRANS) )
    {
        send_to_char( " Where is the button to this thing.\n\r",ch );
        return;
    }
    if ( IS_SET(ch->in_room->room_flags, ROOM_NO_OTRANS) )
    {
        send_to_char( " Where is the button to this thing.\n\r",ch );
        return;
    }
    if ( IS_SET(obj->spectype, SITEM_TARGET) && arg2[0] == '\0' )
    {
        send_to_char( "Who do you wish to use it on?\n\r", ch );
        return;
    }
    if ( IS_SET(obj->spectype, SITEM_TARGET) )
    {
        if ( ( victim = get_char_room( ch, arg2 ) ) == NULL )
        {
            send_to_char( "Nobody here by that name.\n\r", ch );
            return;
        }
    }
    else victim = ch;
    if (obj->chpoweruse != NULL && obj->chpoweruse != '\0'
        && str_cmp(obj->chpoweruse,"(null)") && str_cmp(obj->chpoweruse,"normal"))
        kavitem(obj->chpoweruse,ch,obj,NULL,TO_CHAR);
    if (obj->victpoweruse != NULL && obj->victpoweruse != '\0'
        && str_cmp(obj->victpoweruse,"(null)") && str_cmp(obj->victpoweruse,"normal"))
        kavitem(obj->victpoweruse,ch,obj,NULL,TO_ROOM);
    if ( IS_SET(obj->spectype, SITEM_SPELL))
    {
        int castlevel = obj->level;
        if (castlevel < 1) castlevel = 1;
        else if (castlevel > 60) castlevel = 60;
        obj_cast_spell( obj->specpower, castlevel, ch, victim, NULL );
        if (!IS_IMMORTAL( ch))
            WAIT_STATE(ch,6);
        if ( IS_SET(obj->spectype, SITEM_DELAY1))
        {
            if (!IS_IMMORTAL( ch))
                WAIT_STATE(ch,6);
        }
        if ( IS_SET(obj->spectype, SITEM_DELAY2))
        {
            if (!IS_IMMORTAL( ch))
                WAIT_STATE(ch,12);
        }
        return;
    }
    else if ( IS_SET(obj->spectype, SITEM_TRANSPORTER))
    {
        if (obj->chpoweron != NULL && obj->chpoweron != '\0'
            && str_cmp(obj->chpoweron,"(null)") && str_cmp(obj->chpoweron,"normal"))
            kavitem(obj->chpoweron,ch,obj,NULL,TO_CHAR);
        if (obj->victpoweron != NULL && obj->victpoweron != '\0'
            && str_cmp(obj->victpoweron,"(null)") && str_cmp(obj->victpoweron,"normal"))
            kavitem(obj->victpoweron,ch,obj,NULL,TO_ROOM);
        pRoomIndex     = get_room_index(obj->specpower);
        obj->specpower = ch->in_room->vnum;
        if ( pRoomIndex == NULL ) return;
        char_from_room(ch);
        char_to_room(ch,pRoomIndex);
        do_look(ch,"auto");
        if (obj->chpoweroff != NULL && obj->chpoweroff != '\0'
            && str_cmp(obj->chpoweroff,"(null)") && str_cmp(obj->chpoweroff,"normal"))
            kavitem(obj->chpoweroff,ch,obj,NULL,TO_CHAR);
        if (obj->victpoweroff != NULL && obj->victpoweroff != '\0'
            && str_cmp(obj->victpoweroff,"(null)") && str_cmp(obj->victpoweroff,"normal"))
            kavitem(obj->victpoweroff,ch,obj,NULL,TO_ROOM);
        if (!IS_SET(obj->quest, QUEST_ARTIFACT) &&
            ( IS_SET(ch->in_room->room_flags, ROOM_NO_TELEPORT) || IS_SET(ch->in_room->room_flags, ROOM_ASTRAL) )
            &&
            CAN_WEAR(obj,ITEM_TAKE))
        {
            send_to_char("A powerful force hurls you from the room.\n\r",ch);
            act("$n is hurled from the room by a powerful force.",ch,NULL,NULL,TO_ROOM);
            ch->position = POS_STUNNED;
            char_from_room(ch);
            char_to_room(ch,get_room_index(ROOM_VNUM_TEMPLE));
            act("$n appears in the room, and falls to the ground stunned.",ch,NULL,NULL,TO_ROOM);
        }
        if ( (mount = ch->mount) == NULL) return;
        char_from_room( mount );
        char_to_room( mount, ch->in_room );
        do_look( mount, "auto" );
        return;
    }
    else if ( IS_SET(obj->spectype, SITEM_TELEPORTER))
    {
        if (obj->chpoweron != NULL && obj->chpoweron != '\0'
            && str_cmp(obj->chpoweron,"(null)") && str_cmp(obj->chpoweron,"normal"))
            kavitem(obj->chpoweron,ch,obj,NULL,TO_CHAR);
        if (obj->victpoweron != NULL && obj->victpoweron != '\0'
            && str_cmp(obj->victpoweron,"(null)") && str_cmp(obj->victpoweron,"normal"))
            kavitem(obj->victpoweron,ch,obj,NULL,TO_ROOM);
        pRoomIndex     = get_room_index(obj->specpower);
        if ( pRoomIndex == NULL ) return;
        char_from_room(ch);
        char_to_room(ch,pRoomIndex);
        do_look(ch,"auto");
        if (obj->chpoweroff != NULL && obj->chpoweroff != '\0'
            && str_cmp(obj->chpoweroff,"(null)") && str_cmp(obj->chpoweroff,"normal"))
            kavitem(obj->chpoweroff,ch,obj,NULL,TO_CHAR);
        if (obj->victpoweroff != NULL && obj->victpoweroff != '\0'
            && str_cmp(obj->victpoweroff,"(null)") && str_cmp(obj->victpoweroff,"normal"))
            kavitem(obj->victpoweroff,ch,obj,NULL,TO_ROOM);
        if (!IS_SET(obj->quest, QUEST_ARTIFACT) &&
            ( IS_SET(ch->in_room->room_flags, ROOM_NO_TELEPORT) || IS_SET(ch->in_room->room_flags, ROOM_ASTRAL) )
            &&
            CAN_WEAR(obj,ITEM_TAKE))
        {
            send_to_char("A powerful force hurls you from the room.\n\r",ch);
            act("$n is hurled from the room by a powerful force.",ch,NULL,NULL,TO_ROOM);
            ch->position = POS_STUNNED;
            char_from_room(ch);
            char_to_room(ch,get_room_index(ROOM_VNUM_TEMPLE));
            act("$n appears in the room, and falls to the ground stunned.",ch,NULL,NULL,TO_ROOM);
        }
        if ( (mount = ch->mount) == NULL) return;
        char_from_room( mount );
        char_to_room( mount, ch->in_room );
        do_look( mount, "auto" );
        return;
    }
    else if ( IS_SET(obj->spectype, SITEM_OBJECT))
    {
        if ( get_obj_index(obj->specpower) == NULL ) return;
        obj2 = create_object(get_obj_index(obj->specpower), ch->level);
        if ( CAN_WEAR(obj2,ITEM_TAKE) ) obj_to_char(obj2,ch);
        else                   obj_to_room(obj2,ch->in_room);
    }
    else if ( IS_SET(obj->spectype, SITEM_MOBILE))
    {
        if ( get_mob_index(obj->specpower) == NULL ) return;
        mob = create_mobile(get_mob_index(obj->specpower));
        char_to_room(mob,ch->in_room);
    }
    else if ( IS_SET(obj->spectype, SITEM_ACTION))
    {
        interpret(ch,obj->victpoweron);
        if (obj->victpoweroff != NULL && str_cmp(obj->victpoweroff,"(null)") &&
            obj->victpoweroff != '\0')
        {
            for ( victim = char_list; victim != NULL; victim = victim_next )
            {
                victim_next = victim->next;
                if ( victim->in_room == NULL ) continue;
                if ( victim == ch ) continue;
                if ( victim->in_room == ch->in_room )
                {
                    interpret(victim,obj->victpoweroff);
                    continue;
                }
            }
        }
    }
    return;
}

void do_twist( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    OBJ_DATA *obj2;
    CHAR_DATA *victim;
    CHAR_DATA *victim_next;
    CHAR_DATA *mount;
    CHAR_DATA *mob;
    ROOM_INDEX_DATA *pRoomIndex;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if (IS_NPC(ch)) return;
    if (IS_SET(ch->in_room->room_flags,ROOM_NO_OTRANS))
    {
        send_to_char("A strange force is preventing the item from working.\n\r",ch);
        return;
    }

    if ( arg1[0] == '\0' )
    {
        send_to_char( "Which item do you wish to twist?\n\r", ch );
        return;
    }
    if ( (obj = get_obj_wear(ch,arg1)) == NULL )
    {
        if ( ( obj = get_obj_here( ch, arg1 ) ) == NULL )
        {
            send_to_char( "You can't find that item.\n\r", ch );
            return;
        }
        /* You should only be able to use nontake items on floor */
        if (CAN_WEAR(obj, ITEM_TAKE))
        {
            send_to_char( "But you are not wearing it!\n\r", ch );
            return;
        }
    }
    if ( obj == NULL || !IS_SET(obj->spectype, SITEM_TWIST) )
    {
        send_to_char( "This item cannot be twisted.\n\r", ch );
        return;
    }
    if ( IS_SET(ch->in_room->room_flags, ROOM_NOTRANS) )
    {
        send_to_char( " Where is the button to this thing.\n\r",ch );
        return;
    }
    if ( IS_SET(ch->in_room->room_flags, ROOM_NO_OTRANS) )
    {
        send_to_char( " Where is the button to this thing.\n\r",ch );
        return;
    }
    if ( IS_SET(obj->spectype, SITEM_TARGET) && arg2[0] == '\0' )
    {
        send_to_char( "Who do you wish to use it on?\n\r", ch );
        return;
    }
    if ( IS_SET(obj->spectype, SITEM_TARGET) )
    {
        if ( ( victim = get_char_room( ch, arg2 ) ) == NULL )
        {
            send_to_char( "Nobody here by that name.\n\r", ch );
            return;
        }
    }
    else victim = ch;
    if (obj->chpoweruse != NULL && obj->chpoweruse != '\0'
        && str_cmp(obj->chpoweruse,"(null)") && str_cmp(obj->chpoweruse,"normal"))
        kavitem(obj->chpoweruse,ch,obj,NULL,TO_CHAR);
    if (obj->victpoweruse != NULL && obj->victpoweruse != '\0'
        && str_cmp(obj->victpoweruse,"(null)") && str_cmp(obj->victpoweruse,"normal"))
        kavitem(obj->victpoweruse,ch,obj,NULL,TO_ROOM);
    if ( IS_SET(obj->spectype, SITEM_SPELL))
    {
        int castlevel = obj->level;
        if (castlevel < 1) castlevel = 1;
        else if (castlevel > 60) castlevel = 60;
        obj_cast_spell( obj->specpower, castlevel, ch, victim, NULL );
        if (!IS_IMMORTAL( ch))
            WAIT_STATE(ch,6);
        if ( IS_SET(obj->spectype, SITEM_DELAY1))
        {
            if (!IS_IMMORTAL( ch))
                WAIT_STATE(ch,6);
        }
        if ( IS_SET(obj->spectype, SITEM_DELAY2))
        {
            if (!IS_IMMORTAL( ch))
                WAIT_STATE(ch,12);
        }
        return;
    }
    else if ( IS_SET(obj->spectype, SITEM_TRANSPORTER))
    {
        if (obj->chpoweron != NULL && obj->chpoweron != '\0'
            && str_cmp(obj->chpoweron,"(null)") && str_cmp(obj->chpoweron,"normal"))
            kavitem(obj->chpoweron,ch,obj,NULL,TO_CHAR);
        if (obj->victpoweron != NULL && obj->victpoweron != '\0'
            && str_cmp(obj->victpoweron,"(null)") && str_cmp(obj->victpoweron,"normal"))
            kavitem(obj->victpoweron,ch,obj,NULL,TO_ROOM);
        pRoomIndex     = get_room_index(obj->specpower);
        obj->specpower = ch->in_room->vnum;
        if ( pRoomIndex == NULL ) return;
        char_from_room(ch);
        char_to_room(ch,pRoomIndex);
        do_look(ch,"auto");
        if (obj->chpoweroff != NULL && obj->chpoweroff != '\0'
            && str_cmp(obj->chpoweroff,"(null)") && str_cmp(obj->chpoweroff,"normal"))
            kavitem(obj->chpoweroff,ch,obj,NULL,TO_CHAR);
        if (obj->victpoweroff != NULL && obj->victpoweroff != '\0'
            && str_cmp(obj->victpoweroff,"(null)") && str_cmp(obj->victpoweroff,"normal"))
            kavitem(obj->victpoweroff,ch,obj,NULL,TO_ROOM);
        if (!IS_SET(obj->quest, QUEST_ARTIFACT) &&
            ( IS_SET(ch->in_room->room_flags, ROOM_NO_TELEPORT) || IS_SET(ch->in_room->room_flags, ROOM_ASTRAL)) &&
            CAN_WEAR(obj,ITEM_TAKE))
        {
            send_to_char("A powerful force hurls you from the room.\n\r",ch);
            act("$n is hurled from the room by a powerful force.",ch,NULL,NULL,TO_ROOM);
            ch->position = POS_STUNNED;
            char_from_room(ch);
            char_to_room(ch,get_room_index(ROOM_VNUM_TEMPLE));
            act("$n appears in the room, and falls to the ground stunned.",ch,NULL,NULL,TO_ROOM);
        }
        if ( (mount = ch->mount) == NULL) return;
        char_from_room( mount );
        char_to_room( mount, ch->in_room );
        do_look( mount, "auto" );
        return;
    }
    else if ( IS_SET(obj->spectype, SITEM_TELEPORTER))
    {
        if (obj->chpoweron != NULL && obj->chpoweron != '\0'
            && str_cmp(obj->chpoweron,"(null)") && str_cmp(obj->chpoweron,"normal"))
            kavitem(obj->chpoweron,ch,obj,NULL,TO_CHAR);
        if (obj->victpoweron != NULL && obj->victpoweron != '\0'
            && str_cmp(obj->victpoweron,"(null)") && str_cmp(obj->victpoweron,"normal"))
            kavitem(obj->victpoweron,ch,obj,NULL,TO_ROOM);
        pRoomIndex     = get_room_index(obj->specpower);
        if ( pRoomIndex == NULL ) return;
        char_from_room(ch);
        char_to_room(ch,pRoomIndex);
        do_look(ch,"auto");
        if (obj->chpoweroff != NULL && obj->chpoweroff != '\0'
            && str_cmp(obj->chpoweroff,"(null)") && str_cmp(obj->chpoweroff,"normal"))
            kavitem(obj->chpoweroff,ch,obj,NULL,TO_CHAR);
        if (obj->victpoweroff != NULL && obj->victpoweroff != '\0'
            && str_cmp(obj->victpoweroff,"(null)") && str_cmp(obj->victpoweroff,"normal"))
            kavitem(obj->victpoweroff,ch,obj,NULL,TO_ROOM);
        if (!IS_SET(obj->quest, QUEST_ARTIFACT) &&
            ( IS_SET(ch->in_room->room_flags, ROOM_NO_TELEPORT) || IS_SET(ch->in_room->room_flags, ROOM_ASTRAL) )
            &&
            CAN_WEAR(obj,ITEM_TAKE))
        {
            send_to_char("A powerful force hurls you from the room.\n\r",ch);
            act("$n is hurled from the room by a powerful force.",ch,NULL,NULL,TO_ROOM);
            ch->position = POS_STUNNED;
            char_from_room(ch);
            char_to_room(ch,get_room_index(ROOM_VNUM_TEMPLE));
            act("$n appears in the room, and falls to the ground stunned.",ch,NULL,NULL,TO_ROOM);
        }
        if ( (mount = ch->mount) == NULL) return;
        char_from_room( mount );
        char_to_room( mount, ch->in_room );
        do_look( mount, "auto" );
        return;
    }
    else if ( IS_SET(obj->spectype, SITEM_OBJECT))
    {
        if ( get_obj_index(obj->specpower) == NULL ) return;
        obj2 = create_object(get_obj_index(obj->specpower), ch->level);
        if ( CAN_WEAR(obj2,ITEM_TAKE) ) obj_to_char(obj2,ch);
        else                   obj_to_room(obj2,ch->in_room);
    }
    else if ( IS_SET(obj->spectype, SITEM_MOBILE))
    {
        if ( get_mob_index(obj->specpower) == NULL ) return;
        mob = create_mobile(get_mob_index(obj->specpower));
        char_to_room(mob,ch->in_room);
    }
    else if ( IS_SET(obj->spectype, SITEM_ACTION))
    {
        interpret(ch,obj->victpoweron);
        if (obj->victpoweroff != NULL && str_cmp(obj->victpoweroff,"(null)") &&
            obj->victpoweroff != '\0')
        {
            for ( victim = char_list; victim != NULL; victim = victim_next )
            {
                victim_next = victim->next;
                if ( victim->in_room == NULL ) continue;
                if ( victim == ch ) continue;
                if ( victim->in_room == ch->in_room )
                {
                    interpret(victim,obj->victpoweroff);
                    continue;
                }
            }
        }
    }
    return;
}

void do_pull( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    OBJ_DATA *obj2;
    CHAR_DATA *victim;
    CHAR_DATA *victim_next;
    CHAR_DATA *mount;
    CHAR_DATA *mob;
    ROOM_INDEX_DATA *pRoomIndex;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if (IS_NPC(ch)) return;
    if (IS_SET(ch->in_room->room_flags,ROOM_NO_OTRANS))
    {
        send_to_char("A strange force is preventing the item from working.\n\r",ch);
        return;
    }

    if ( arg1[0] == '\0' )
    {
        send_to_char( "What do you wish to pull?\n\r", ch );
        return;
    }
    if ( (obj = get_obj_wear(ch,arg1)) == NULL )
    {
        if ( ( obj = get_obj_here( ch, arg1 ) ) == NULL )
        {
            send_to_char( "You can't find that item.\n\r", ch );
            return;
        }
        /* You should only be able to use nontake items on floor */
        if (CAN_WEAR(obj, ITEM_TAKE))
        {
            send_to_char( "But you are not wearing it!\n\r", ch );
            return;
        }
    }
    if ( obj == NULL || !IS_SET(obj->spectype, SITEM_PULL) )
    {
        send_to_char( "This item cannot be pulled.\n\r", ch );
        return;
    }
    if ( IS_SET(ch->in_room->room_flags, ROOM_NOTRANS) )
    {
        send_to_char( " Where is the button to this thing.\n\r",ch );
        return;
    }
    if ( IS_SET(ch->in_room->room_flags, ROOM_NO_OTRANS) )
    {
        send_to_char( " Where is the button to this thing.\n\r",ch );
        return;
    }
    if ( IS_SET(obj->spectype, SITEM_TARGET) && arg2[0] == '\0' )
    {
        send_to_char( "Who do you wish to use it on?\n\r", ch );
        return;
    }
    if ( IS_SET(obj->spectype, SITEM_TARGET) )
    {
        if ( ( victim = get_char_room( ch, arg2 ) ) == NULL )
        {
            send_to_char( "Nobody here by that name.\n\r", ch );
            return;
        }
    }
    else victim = ch;
    if (obj->chpoweruse != NULL && obj->chpoweruse != '\0'
        && str_cmp(obj->chpoweruse,"(null)") && str_cmp(obj->chpoweruse,"normal"))
        kavitem(obj->chpoweruse,ch,obj,NULL,TO_CHAR);
    if (obj->victpoweruse != NULL && obj->victpoweruse != '\0'
        && str_cmp(obj->victpoweruse,"(null)") && str_cmp(obj->victpoweruse,"normal"))
        kavitem(obj->victpoweruse,ch,obj,NULL,TO_ROOM);
    if ( IS_SET(obj->spectype, SITEM_SPELL))
    {
        int castlevel = obj->level;
        if (castlevel < 1) castlevel = 1;
        else if (castlevel > 60) castlevel = 60;
        obj_cast_spell( obj->specpower, castlevel, ch, victim, NULL );
        if (!IS_IMMORTAL( ch))
            WAIT_STATE(ch,6);
        if ( IS_SET(obj->spectype, SITEM_DELAY1))
        {
            if (!IS_IMMORTAL( ch))
                WAIT_STATE(ch,6);
        }
        if ( IS_SET(obj->spectype, SITEM_DELAY2))
        {
            if (!IS_IMMORTAL( ch))
                WAIT_STATE(ch,12);
        }
        return;
    }
    else if ( IS_SET(obj->spectype, SITEM_TRANSPORTER))
    {
        if (obj->chpoweron != NULL && obj->chpoweron != '\0'
            && str_cmp(obj->chpoweron,"(null)") && str_cmp(obj->chpoweron,"normal"))
            kavitem(obj->chpoweron,ch,obj,NULL,TO_CHAR);
        if (obj->victpoweron != NULL && obj->victpoweron != '\0'
            && str_cmp(obj->victpoweron,"(null)") && str_cmp(obj->victpoweron,"normal"))
            kavitem(obj->victpoweron,ch,obj,NULL,TO_ROOM);
        pRoomIndex     = get_room_index(obj->specpower);
        obj->specpower = ch->in_room->vnum;
        if ( pRoomIndex == NULL ) return;
        char_from_room(ch);
        char_to_room(ch,pRoomIndex);
        do_look(ch,"auto");
        if (obj->chpoweroff != NULL && obj->chpoweroff != '\0'
            && str_cmp(obj->chpoweroff,"(null)") && str_cmp(obj->chpoweron,"normal"))
            kavitem(obj->chpoweroff,ch,obj,NULL,TO_CHAR);
        if (obj->victpoweroff != NULL && obj->victpoweroff != '\0'
            && str_cmp(obj->victpoweroff,"(null)") && str_cmp(obj->victpoweron,"normal"))
            kavitem(obj->victpoweroff,ch,obj,NULL,TO_ROOM);
        if (!IS_SET(obj->quest, QUEST_ARTIFACT) &&
            (IS_SET(ch->in_room->room_flags, ROOM_NO_TELEPORT) || IS_SET(ch->in_room->room_flags, ROOM_ASTRAL))  &&
            CAN_WEAR(obj,ITEM_TAKE))
        {
            send_to_char("A powerful force hurls you from the room.\n\r",ch);
            act("$n is hurled from the room by a powerful force.",ch,NULL,NULL,TO_ROOM);
            ch->position = POS_STUNNED;
            char_from_room(ch);
            char_to_room(ch,get_room_index(ROOM_VNUM_TEMPLE));
            act("$n appears in the room, and falls to the ground stunned.",ch,NULL,NULL,TO_ROOM);
        }
        if ( (mount = ch->mount) == NULL) return;
        char_from_room( mount );
        char_to_room( mount, ch->in_room );
        do_look( mount, "auto" );
        return;
    }
    else if ( IS_SET(obj->spectype, SITEM_TELEPORTER))
    {
        if (obj->chpoweron != NULL && obj->chpoweron != '\0'
            && str_cmp(obj->chpoweron,"(null)") && str_cmp(obj->chpoweron,"normal"))
            kavitem(obj->chpoweron,ch,obj,NULL,TO_CHAR);
        if (obj->victpoweron != NULL && obj->victpoweron != '\0'
            && str_cmp(obj->victpoweron,"(null)") && str_cmp(obj->victpoweron,"normal"))
            kavitem(obj->victpoweron,ch,obj,NULL,TO_ROOM);
        pRoomIndex     = get_room_index(obj->specpower);
        if ( pRoomIndex == NULL ) return;
        char_from_room(ch);
        char_to_room(ch,pRoomIndex);
        do_look(ch,"auto");
        if (obj->chpoweroff != NULL && obj->chpoweroff != '\0'
            && str_cmp(obj->chpoweroff,"(null)") && str_cmp(obj->chpoweroff,"normal"))
            kavitem(obj->chpoweroff,ch,obj,NULL,TO_CHAR);
        if (obj->victpoweroff != NULL && obj->victpoweroff != '\0'
            && str_cmp(obj->victpoweroff,"(null)") && str_cmp(obj->victpoweroff,"normal"))
            kavitem(obj->victpoweroff,ch,obj,NULL,TO_ROOM);
        if (!IS_SET(obj->quest, QUEST_ARTIFACT) &&
            (IS_SET(ch->in_room->room_flags, ROOM_NO_TELEPORT) || IS_SET(ch->in_room->room_flags, ROOM_ASTRAL))  &&
            CAN_WEAR(obj,ITEM_TAKE))
        {
            send_to_char("A powerful force hurls you from the room.\n\r",ch);
            act("$n is hurled from the room by a powerful force.",ch,NULL,NULL,TO_ROOM);
            ch->position = POS_STUNNED;
            char_from_room(ch);
            char_to_room(ch,get_room_index(ROOM_VNUM_TEMPLE));
            act("$n appears in the room, and falls to the ground stunned.",ch,NULL,NULL,TO_ROOM);
        }
        if ( (mount = ch->mount) == NULL) return;
        char_from_room( mount );
        char_to_room( mount, ch->in_room );
        do_look( mount, "auto" );
        return;
    }
    else if ( IS_SET(obj->spectype, SITEM_OBJECT))
    {
        if ( get_obj_index(obj->specpower) == NULL ) return;
        obj2 = create_object(get_obj_index(obj->specpower), ch->level);
        if ( CAN_WEAR(obj2,ITEM_TAKE) ) obj_to_char(obj2,ch);
        else                   obj_to_room(obj2,ch->in_room);
    }
    else if ( IS_SET(obj->spectype, SITEM_MOBILE))
    {
        if ( get_mob_index(obj->specpower) == NULL ) return;
        mob = create_mobile(get_mob_index(obj->specpower));
        char_to_room(mob,ch->in_room);
    }
    else if ( IS_SET(obj->spectype, SITEM_ACTION))
    {
        interpret(ch,obj->victpoweron);
        if (obj->victpoweroff != NULL && str_cmp(obj->victpoweroff,"(null)") &&
            obj->victpoweroff != '\0')
        {
            for ( victim = char_list; victim != NULL; victim = victim_next )
            {
                victim_next = victim->next;
                if ( victim->in_room == NULL ) continue;
                if ( victim == ch ) continue;
                if ( victim->in_room == ch->in_room )
                {
                    interpret(victim,obj->victpoweroff);
                    continue;
                }
            }
        }
    }
    return;
}

bool is_ok_to_wear( CHAR_DATA *ch, bool wolf_ok, char *argument )
{
    char arg [MAX_INPUT_LENGTH];
    int count;
    argument = one_argument( argument, arg );

    if (!str_cmp(arg,"head"))
    {
        if (IS_HEAD(ch,LOST_HEAD)) return FALSE;
    }
    else if (!str_cmp(arg,"face"))
    {
        if (IS_HEAD(ch,LOST_HEAD)) return FALSE;
    }
    else if (!str_cmp(arg,"left_hand"))
    {
        if (!IS_NPC(ch) && IS_SET(ch->special, SPC_WOLFMAN) && !wolf_ok && !IS_CLASS(ch, CLASS_VAMPIRE))
            return FALSE;
        if (IS_ARM_L(ch,LOST_ARM)) return FALSE;
        else if (IS_ARM_L(ch,BROKEN_ARM)) return FALSE;
        else if (IS_ARM_L(ch,LOST_HAND)) return FALSE;
        else if (IS_ARM_L(ch,BROKEN_THUMB)) return FALSE;
        else if (IS_ARM_L(ch,LOST_THUMB)) return FALSE;
        count = 0;
        if (IS_ARM_L(ch,LOST_FINGER_I)||IS_ARM_L(ch,BROKEN_FINGER_I)) count+= 1;
        if (IS_ARM_L(ch,LOST_FINGER_M)||IS_ARM_L(ch,BROKEN_FINGER_M)) count+= 1;
        if (IS_ARM_L(ch,LOST_FINGER_R)||IS_ARM_L(ch,BROKEN_FINGER_R)) count+= 1;
        if (IS_ARM_L(ch,LOST_FINGER_L)||IS_ARM_L(ch,BROKEN_FINGER_L)) count+= 1;
        if (count > 2) return FALSE;
    }
    else if (!str_cmp(arg,"right_hand"))
    {
        if (!IS_NPC(ch) && IS_SET(ch->special, SPC_WOLFMAN) && !wolf_ok && !IS_CLASS(ch, CLASS_VAMPIRE))
            return FALSE;
        if (IS_ARM_R(ch,LOST_ARM)) return FALSE;
        else if (IS_ARM_R(ch,BROKEN_ARM)) return FALSE;
        else if (IS_ARM_R(ch,LOST_HAND)) return FALSE;
        else if (IS_ARM_R(ch,BROKEN_THUMB)) return FALSE;
        else if (IS_ARM_R(ch,LOST_THUMB)) return FALSE;
        count = 0;
        if (IS_ARM_R(ch,LOST_FINGER_I)||IS_ARM_R(ch,BROKEN_FINGER_I)) count+= 1;
        if (IS_ARM_R(ch,LOST_FINGER_M)||IS_ARM_R(ch,BROKEN_FINGER_M)) count+= 1;
        if (IS_ARM_R(ch,LOST_FINGER_R)||IS_ARM_R(ch,BROKEN_FINGER_R)) count+= 1;
        if (IS_ARM_R(ch,LOST_FINGER_L)||IS_ARM_R(ch,BROKEN_FINGER_L)) count+= 1;
        if (count > 2) return FALSE;
    }
    else if (!str_cmp(arg,"third_hand"))
    {
        if (!IS_SET(ch->newbits, THIRD_HAND)) return FALSE;
        if (!IS_NPC(ch) && IS_SET(ch->special, SPC_WOLFMAN) && !wolf_ok && !IS_CLASS(ch, CLASS_VAMPIRE))
            return FALSE;
        /*
            if (IS_ARM_T(ch,LOST_ARM)) return FALSE;
            else if (IS_ARM_T(ch,BROKEN_ARM)) return FALSE;
            else if (IS_ARM_T(ch,LOST_HAND)) return FALSE;
            else if (IS_ARM_T(ch,BROKEN_THUMB)) return FALSE;
            else if (IS_ARM_T(ch,LOST_THUMB)) return FALSE;
            count = 0;
            if (IS_ARM_T(ch,LOST_FINGER_I)||IS_ARM_T(ch,BROKEN_FINGER_I)) count+= 1;
            if (IS_ARM_T(ch,LOST_FINGER_M)||IS_ARM_T(ch,BROKEN_FINGER_M)) count+= 1;
            if (IS_ARM_T(ch,LOST_FINGER_R)||IS_ARM_T(ch,BROKEN_FINGER_R)) count+= 1;
            if (IS_ARM_T(ch,LOST_FINGER_L)||IS_ARM_T(ch,BROKEN_FINGER_L)) count+= 1;
            if (count > 2) return FALSE;
        */
    }
    else if (!str_cmp(arg,"fourth_hand"))
    {
        if (!IS_SET(ch->newbits, FOURTH_HAND)) return FALSE;
        if (!IS_NPC(ch) && IS_SET(ch->special, SPC_WOLFMAN) && !wolf_ok && !IS_CLASS(ch, CLASS_VAMPIRE))
            return FALSE;
        /*
            if (IS_ARM_F(ch,LOST_ARM)) return FALSE;
            else if (IS_ARM_F(ch,BROKEN_ARM)) return FALSE;
            else if (IS_ARM_F(ch,LOST_HAND)) return FALSE;
            else if (IS_ARM_F(ch,BROKEN_THUMB)) return FALSE;
            else if (IS_ARM_F(ch,LOST_THUMB)) return FALSE;
            count = 0;
            if (IS_ARM_F(ch,LOST_FINGER_I)||IS_ARM_F(ch,BROKEN_FINGER_I)) count+= 1;
            if (IS_ARM_F(ch,LOST_FINGER_M)||IS_ARM_F(ch,BROKEN_FINGER_M)) count+= 1;
            if (IS_ARM_F(ch,LOST_FINGER_R)||IS_ARM_F(ch,BROKEN_FINGER_R)) count+= 1;
            if (IS_ARM_F(ch,LOST_FINGER_L)||IS_ARM_F(ch,BROKEN_FINGER_L)) count+= 1;
            if (count > 2) return FALSE;
        */
    }
    else if (!str_cmp(arg,"left_wrist"))
    {
        if (IS_ARM_L(ch,LOST_ARM)) return FALSE;
        else if (IS_ARM_L(ch,LOST_HAND)) return FALSE;
    }
    else if (!str_cmp(arg,"right_wrist"))
    {
        if (IS_ARM_R(ch,LOST_ARM)) return FALSE;
        else if (IS_ARM_R(ch,LOST_HAND)) return FALSE;
    }
    else if (!str_cmp(arg,"left_finger"))
    {
        if (IS_ARM_L(ch,LOST_ARM)) return FALSE;
        else if (IS_ARM_L(ch,LOST_HAND)) return FALSE;
        else if (IS_ARM_L(ch,LOST_FINGER_R)) return FALSE;
    }
    else if (!str_cmp(arg,"right_finger"))
    {
        if (IS_ARM_R(ch,LOST_ARM)) return FALSE;
        else if (IS_ARM_R(ch,LOST_HAND)) return FALSE;
        else if (IS_ARM_R(ch,LOST_FINGER_R)) return FALSE;
    }
    else if (!str_cmp(arg,"arms"))
    {
        if (IS_ARM_L(ch,LOST_ARM) && IS_ARM_R(ch,LOST_ARM)) return FALSE;
    }
    else if (!str_cmp(arg,"hands"))
    {
        if (IS_ARM_L(ch,LOST_ARM) && IS_ARM_R(ch,LOST_ARM)) return FALSE;
        if (IS_ARM_L(ch,LOST_HAND) || IS_ARM_R(ch,LOST_HAND)) return FALSE;
    }
    else if (!str_cmp(arg,"legs"))
    {
        if (IS_LEG_L(ch,LOST_LEG) && IS_LEG_R(ch,LOST_LEG)) return FALSE;
    }
    else if (!str_cmp(arg,"feet"))
    {
        if (IS_LEG_L(ch,LOST_LEG) && IS_LEG_R(ch,LOST_LEG)) return FALSE;
        if (IS_LEG_L(ch,LOST_FOOT) || IS_LEG_R(ch,LOST_FOOT)) return FALSE;
    }
    return TRUE;
}

void do_qmake( CHAR_DATA *ch, char *argument )
{
    OBJ_INDEX_DATA *pObjIndex;
    OBJ_DATA *obj;
    char arg[MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if (arg[0] == '\0')
    {
        send_to_char("Do you wish to qmake a MACHINE or a CARD?\n\r",ch);
        return;
    }
    if (!str_cmp(arg,"card"))
    {
        if ( (pObjIndex = get_obj_index( OBJ_VNUM_QUESTCARD )) == NULL)
        {
            send_to_char("Missing object, please inform KaVir.\n\r",ch);
            return;
        }
        if (ch->in_room == NULL) return;
        obj = create_object(pObjIndex, 0);
        obj_to_char(obj, ch);
        quest_object(ch,obj);
    }
    else if (!str_cmp(arg,"machine"))
    {
        if ( (pObjIndex = get_obj_index( OBJ_VNUM_QUESTMACHINE )) == NULL)
        {
            send_to_char("Missing object, please inform KaVir.\n\r",ch);
            return;
        }
        if (ch->in_room == NULL) return;
        obj = create_object(pObjIndex, 0);
        obj_to_room(obj, ch->in_room);
    }
    else
    {
        send_to_char("You can only qmake a MACHINE or a CARD.\n\r",ch);
        return;
    }
    send_to_char("Ok.\n\r",ch);
    return;
}

void do_recharge( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_INPUT_LENGTH];
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    OBJ_DATA *qobj;
    int count = 0;
    int value = 1;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if (IS_NPC(ch)) return;

    if ( arg1[0] == '\0' || arg2[0] == '\0' )
    {
        send_to_char( "Syntax: recharge <quest card> <quest machine>\n\r", ch );
        return;
    }
    if ( (obj = get_obj_carry(ch,arg1)) == NULL )
    {
        send_to_char( "You are not carrying that object.\n\r", ch );
        return;
    }
    if (obj->item_type != ITEM_QUESTCARD)
    {
        send_to_char( "That is not a quest card.\n\r", ch );
        return;
    }
    if ( (qobj = get_obj_here(ch,arg2)) == NULL )
    {
        send_to_char( "There is nothing for you to recharge it with.\n\r", ch );
        return;
    }
    if (qobj->item_type != ITEM_QUESTMACHINE)
    {
        send_to_char( "That is not a quest machine.\n\r", ch );
        return;
    }
    if (obj->value[0] == -1) count += 1;
    if (obj->value[1] == -1) count += 1;
    if (obj->value[2] == -1) count += 1;
    if (obj->value[3] == -1) count += 1;
    if (count == 4) quest_object( ch, obj );
    else
    {
        send_to_char( "You have not yet completed the current quest.\n\r", ch );
        return;
    }
    if (str_cmp(ch->pcdata->switchname,obj->questowner) && !IS_SET(obj->quest, QUEST_ARTIFACT))
    {
        send_to_char("But you don't own it!\n\r",ch);
        return;
    }
    act("You place $p into a small slot in $P.",ch,obj,qobj,TO_CHAR);
    act("$n places $p into a small slot in $P.",ch,obj,qobj,TO_ROOM);
    act("$P makes a few clicks and returns $p.",ch,obj,qobj,TO_CHAR);
    act("$P makes a few clicks and returns $p.",ch,obj,qobj,TO_ROOM);
    value = obj->level;
    if (value < 1) value = 1; else if (value > 250 && !IS_SET(obj->quest, QUEST_ANLA) ) value = 250;
    if (global_qp == TRUE)
        value *= 2;
    ch->pcdata->questsrun += 1;
    ch->pcdata->questtotal += value;
    if ( IS_SET(obj->quest, QUEST_ANLA ) ) obj->level -= 5;

    obj = create_object(get_obj_index( OBJ_VNUM_PROTOPLASM ), 0);
    obj->value[0] = value;
    obj->level = value;
    obj->cost = value*1000;
    obj->item_type = ITEM_QUEST;
    obj_to_char(obj,ch);
    if (obj->questmaker != NULL) free_string(obj->questmaker);
    obj->questmaker = str_dup(ch->name);
    free_string( obj->name );
    obj->name = str_dup( "quest token" );
    free_string( obj->short_descr );
    sprintf(buf,"a %d point quest token",value);
    obj->short_descr = str_dup( buf );
    free_string( obj->description );
    sprintf(buf,"A %d point quest token lies on the floor.",value);
    obj->description = str_dup( buf );
    act("You take $p from $P.",ch,obj,qobj,TO_CHAR);
    act("$n takes $p from $P.",ch,obj,qobj,TO_ROOM);
    if (!IS_NPC(ch))
    {
        ch->pcdata->score[SCORE_NUM_QUEST]++;
        ch->pcdata->score[SCORE_QUEST] += value;
        //	sprintf(buf,"%s has completed a quest!.",ch->name);
    }
    //    else sprintf(buf,"%s has completed a quest!.",ch->pcdata->switchname);
    //    buf[0] = UPPER(buf[0]);
    //    do_info(ch,buf);
    do_autosave(ch,"");
    return;
}

void quest_object( CHAR_DATA *ch, OBJ_DATA *obj )
{
    static const int quest_selection[] =
    {
        /* Newer Items */
        303,
        310,
        915,
        304,
        663,
        301,
        608,
        300,
        908,
        205,
        615,
        100,
        311,
        5301,
        5230,
        5306,
        5228,
        5231,
        5300,
        5305,
        5232,
        5302,
        6000,
        6003,
        6001,
        5324,
        6005,
        6010,
        5329,
        5311,
        5310,
        5309,
        5319,
        5328,
        4100,
        5224,
        4050,
        5214,
        5003,
        4701,
        5005,
        3610,
        3423,
        3417,
        3604,
        3425,
        3428,
        5207,
        5217,
        3412,
        5011,
        5220,
        5223,
        5110,
        5022,
        5013,
        5012,
        5028,
        5030,
        8903,
        6304,
        8603,
        8602,
        9105,
        8911,
        9203,
        8127,
        6106,
        8119,
        8601,
        8306,
        8130,
        8600,
        9317,
        9211,
        9313,
        9213,
        9322,
        9321,
        9325,
        9311,
        9231,
        9226,
        9307,
        9302,
        9306,
        7200,
        7223,
        7190,
        8118,
        7206,
        7203,
        7216,
        6646,
        6509,
        9328,
        6601,
        6512,
        6600,
        8005,
        7303,
        8003,
        8115,
        8007,
        8006,
        8110,
        7824,
        7704,
        7310,
        7801,
        7711,
        7714,
        1711,
        2005,
        2003,
        2015,
        2008,
        2013,
        1652,
        1602,
        1510,
        3414,
        1612,
        1614,
        2331,
        2333,
        2332,
        2102,
        2342,
        2348,
        2304,
        2202,
        2106,
        2360,
        3410,
        2276,
        1307,
        1332,
        1330,
        1401,
        1333,
        1336,
        1304,
        1107,
        1100,
        1402,
        1109,
        1114,
        1352,
        1384,
        1380,
        1342,
        1388,
        1394,
        1374,
        1373,
        1356,
        1361,
        1372,
        2804,
        2624,
        2805,
        2901,
        2467,
        2614,
        2605,
        2462,
        2361,
        2275,
        3050,
        3402,
        2915,
        3003,
        3135,
        307,
        2461,
        2902,
        2392,
        2393,
        2371,
        2382,
        2365,
        2442,
        2449,
        2448,
        2402,
        2389,
        2404,
        31057,
        11107,
        11111,
        31067,
        31059,
        4000,
        31052,
        31065,
        11015,
        93005,
        11012,
        11005,
        11008,
        32001,
        11104,
        93004,
        11007,
        11101,
        14030,
        50000,
        14029,
        14028,
        14023,
        14031,
        14014,
        2322,
        2210,
        77011,
        50201,
        50204,
        30325,
        50008,
        50007,
        30315,
        30319,
        30316,
        5011,
        30302
    };
    int object;
    if (obj == NULL || obj->item_type != ITEM_QUESTCARD) return;

    object = number_range(0,56);
    obj->value[0] = quest_selection[object];

    object = number_range(57,113);
    obj->value[1] = quest_selection[object];

    object = number_range(113,170);
    obj->value[2] = quest_selection[object];

    object = number_range(170,225);
    obj->value[3] = quest_selection[object];
    return;
}

void do_complete( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    OBJ_DATA *qobj;
    OBJ_DATA *obj;
    OBJ_INDEX_DATA *pObjIndex;
    int count = 0;
    int count2 = 0;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if ( arg1[0] == '\0' )
    {
        send_to_char( "Syntax: complete <quest card> <object>\n\r", ch );
        return;
    }

    if ( (qobj = get_obj_carry(ch,arg1)) == NULL )
    {
        send_to_char( "You are not carrying that object.\n\r", ch );
        return;
    }
    if (str_cmp(ch->pcdata->switchname,qobj->questowner) && !IS_SET(qobj->quest, QUEST_ARTIFACT))
    {
        send_to_char("But you don't own it!\n\r",ch);
        return;
    }
    else if (qobj->item_type != ITEM_QUESTCARD)
    {
        send_to_char( "That is not a quest card.\n\r", ch );
        return;
    }
    if (qobj->value[0] == -1) count += 1;
    if (qobj->value[1] == -1) count += 1;
    if (qobj->value[2] == -1) count += 1;
    if (qobj->value[3] == -1) count += 1;

    if ( arg2[0] == '\0' )
    {
        if (count == 4) {send_to_char("This quest card has been completed.\n\r",ch);return;}
        send_to_char("You still need to find the following:\n\r",ch);
        if (qobj->value[0] != -1)
        {
            pObjIndex = get_obj_index( qobj->value[0] );
            if ( pObjIndex != NULL )
                sprintf(buf,"	  %s.\n\r",pObjIndex->short_descr);
            buf[5] = UPPER(buf[5]);send_to_char(buf,ch);
        }
        if (qobj->value[1] != -1)
        {
            pObjIndex = get_obj_index( qobj->value[1] );
            if ( pObjIndex != NULL )
                sprintf(buf,"	  %s.\n\r",pObjIndex->short_descr);
            buf[5] = UPPER(buf[5]);send_to_char(buf,ch);
        }
        if (qobj->value[2] != -1)
        {
            pObjIndex = get_obj_index( qobj->value[2] );
            if ( pObjIndex != NULL )
                sprintf(buf,"	  %s.\n\r",pObjIndex->short_descr);
            buf[5] = UPPER(buf[5]);send_to_char(buf,ch);
        }
        if (qobj->value[3] != -1)
        {
            pObjIndex = get_obj_index( qobj->value[3] );
            if ( pObjIndex != NULL )
                sprintf(buf,"	  %s.\n\r",pObjIndex->short_descr);
            buf[5] = UPPER(buf[5]);send_to_char(buf,ch);
        }
        return;
    }

    if (count == 4)
    {
        act( "But $p has already been completed!",ch,qobj,NULL,TO_CHAR );
        return;
    }

    if ( (obj = get_obj_carry(ch,arg2)) == NULL )
    {
        send_to_char( "You are not carrying that object.\n\r", ch );
        return;
    }
    if (obj->questmaker != NULL && strlen(obj->questmaker) > 1)
    {
        send_to_char( "You cannot use that item.\n\r", ch );
        return;
    }
    if (obj->pIndexData->vnum == 30037 || obj->pIndexData->vnum == 30041)
    {
        send_to_char( "That item has lost its quest value, you must collect a new one.\n\r", ch );
        return;
    }
    if (qobj->value[0] != -1)
    {
        pObjIndex = get_obj_index( qobj->value[0] );
        if ( pObjIndex != NULL &&
            !str_cmp(obj->short_descr, pObjIndex->short_descr))
            qobj->value[0] = -1;
    }
    if (qobj->value[1] != -1)
    {
        pObjIndex = get_obj_index( qobj->value[1] );
        if ( pObjIndex != NULL &&
            !str_cmp(obj->short_descr, pObjIndex->short_descr))
            qobj->value[1] = -1;
    }
    if (qobj->value[2] != -1)
    {
        pObjIndex = get_obj_index( qobj->value[2] );
        if ( pObjIndex != NULL &&
            !str_cmp(obj->short_descr, pObjIndex->short_descr))
            qobj->value[2] = -1;
    }
    if (qobj->value[3] != -1)
    {
        pObjIndex = get_obj_index( qobj->value[3] );
        if ( pObjIndex != NULL &&
            !str_cmp(obj->short_descr, pObjIndex->short_descr))
            qobj->value[3] = -1;
    }
    if (qobj->value[0] == -1) count2 += 1;
    if (qobj->value[1] == -1) count2 += 1;
    if (qobj->value[2] == -1) count2 += 1;
    if (qobj->value[3] == -1) count2 += 1;
    if (count == count2)
    {
        send_to_char( "That item is not required.\n\r", ch );
        return;
    }

    act("You touch $p to $P, and $p vanishes!",ch,obj,qobj,TO_CHAR);
    act("$n touches $p to $P, and $p vanishes!",ch,obj,qobj,TO_ROOM);
    obj_from_char(obj);
    extract_obj(obj);
    if (count >= 3) {act("$p has been completed!",ch,qobj,NULL,TO_CHAR);}
    else if (count == 2) {act("$p now requires one more object!",ch,qobj,NULL,TO_CHAR);}
    else if (count == 1) {act("$p now requires two more objects!",ch,qobj,NULL,TO_CHAR);}
    else if (count == 0) {act("$p now requires three more objects!",ch,qobj,NULL,TO_CHAR);}
    return;
}

void do_sheath( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];

    one_argument(argument,arg);

    if (arg[0] == '\0') send_to_char("Which hand, left or right?\n\r",ch);
    else if (!str_cmp(arg,"all") || !str_cmp(arg,"both"))
    {
        sheath(ch,TRUE );
        sheath(ch,FALSE);
    }
    else if (!str_cmp(arg,"l") || !str_cmp(arg,"left" )) sheath(ch,FALSE);
    else if (!str_cmp(arg,"r") || !str_cmp(arg,"right")) sheath(ch,TRUE );
    else send_to_char("Which hand, left or right?\n\r",ch);
    return;
}

void do_draw( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];

    one_argument(argument,arg);

    if (arg[0] == '\0') send_to_char("Which hand, left or right?\n\r",ch);
    else if (!str_cmp(arg,"all") || !str_cmp(arg,"both"))
    {
        if (IS_ARM_L(ch, LOST_ARM) || IS_ARM_R(ch, LOST_ARM))
        {
            send_to_char("You don't have both of your arms.\n\r",ch);
            return;
        }
        draw(ch,TRUE );
        draw(ch,FALSE);
    }
    else if (!str_cmp(arg,"l") || !str_cmp(arg,"left" ))
    {
        if (IS_ARM_L(ch, LOST_ARM))
        {
            send_to_char("You have lost your left arm.\n\r",ch);
            return;
        }
        draw(ch,FALSE);
    }
    else if (!str_cmp(arg,"r") || !str_cmp(arg,"right"))
    {
        if (IS_ARM_R(ch, LOST_ARM))
        {
            send_to_char("You have lost your right arm.\n\r",ch);
            return;
        }
        draw(ch,TRUE );
    }
    else send_to_char("Which hand, left or right?\n\r",ch);
    return;
}

void sheath( CHAR_DATA *ch, bool right )
{
    OBJ_DATA *obj;
    OBJ_DATA *obj2;
    int scabbard;
    int sn;

    if (right)
    {
        scabbard = WEAR_SCABBARD_R;
        if ( (obj = get_eq_char(ch,WEAR_WIELD)) == NULL )
        {
            send_to_char("You are not holding anything in your right hand.\n\r",ch);
            return;
        }
        else if ( (obj2 = get_eq_char(ch,scabbard)) != NULL)
        {
            act("You already have $p in your right scabbard.",ch,obj2,NULL,TO_CHAR);
            return;
        }
        act("You slide $p into your right scabbard.",ch,obj,NULL,TO_CHAR);
        act("$n slides $p into $s right scabbard.",ch,obj,NULL,TO_ROOM);
    }
    else
    {
        scabbard = WEAR_SCABBARD_L;
        if ( (obj = get_eq_char(ch,WEAR_HOLD)) == NULL )
        {
            send_to_char("You are not holding anything in your left hand.\n\r",ch);
            return;
        }
        else if ( (obj2 = get_eq_char(ch,scabbard)) != NULL)
        {
            act("You already have $p in your left scabbard.",ch,obj2,NULL,TO_CHAR);
            return;
        }
        act("You slide $p into your left scabbard.",ch,obj,NULL,TO_CHAR);
        act("$n slides $p into $s left scabbard.",ch,obj,NULL,TO_ROOM);
    }
    if (obj->item_type == ITEM_ARMOR)
        sn = obj->value[3];
    else
        sn = obj->value[0] / 1000;
    if( sn == 25 )
    {
        stc( "You can't sheath a rage weapon.\n\r",ch);
        return;
    }
    if (obj->item_type != ITEM_WEAPON)
    {
        act("$p is not a weapon.",ch,obj,NULL,TO_CHAR);
        return;
    }
    unequip_char(ch,obj);
    obj->wear_loc = scabbard;
    return;
}

void draw( CHAR_DATA *ch, bool right )
{
    OBJ_DATA *obj;
    OBJ_DATA *obj2;
    int scabbard;
    int worn;
    if (right)
    {
        scabbard = WEAR_SCABBARD_R;
        worn = WEAR_WIELD;
        if ( (obj = get_eq_char(ch,scabbard)) == NULL )
        {
            send_to_char("Your right scabbard is empty.\n\r",ch);
            return;
        }
        else if ( (obj2 = get_eq_char(ch,WEAR_WIELD)) != NULL)
        {
            act("You already have $p in your right hand.",ch,obj2,NULL,TO_CHAR);
            return;
        }
        act("You draw $p from your right scabbard.",ch,obj,NULL,TO_CHAR);
        act("$n draws $p from $s right scabbard.",ch,obj,NULL,TO_ROOM);
    }
    else
    {
        scabbard = WEAR_SCABBARD_L;
        worn = WEAR_HOLD;
        if ( (obj = get_eq_char(ch,scabbard)) == NULL )
        {
            send_to_char("Your left scabbard is empty.\n\r",ch);
            return;
        }
        else if ( (obj2 = get_eq_char(ch,WEAR_HOLD)) != NULL)
        {
            act("You already have $p in your left hand.",ch,obj2,NULL,TO_CHAR);
            return;
        }
        act("You draw $p from your left scabbard.",ch,obj,NULL,TO_CHAR);
        act("$n draws $p from $s left scabbard.",ch,obj,NULL,TO_ROOM);
    }
    obj->wear_loc = -1;
    equip_char(ch,obj,worn);
    return;
}

void do_thirdeye(CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim;
    DESCRIPTOR_DATA *d;
    char      buf [MAX_STRING_LENGTH];
    if (IS_NPC(ch)) return;

    if (!IS_ITEMAFF(ch, ITEMA_AFFEYE))
    {
        send_to_char("Huh?\n\r", ch);
        return;
    }
    send_to_char("#CYou experience the world through your third eye.#n\n\r", ch);
    for (d = descriptor_list; d != NULL; d = d->next)
    {
        if (d->connected != CON_PLAYING) continue;
        if ((victim = d->character) == NULL) continue;
        if (IS_NPC(victim) || victim->in_room == NULL) continue;
        if (victim == ch) continue;
        if (!can_see(ch,victim)) continue;
        if (victim->level > 6) continue;
        sprintf(buf,"#n%-14s - %s\n\r", victim->name, victim->in_room->name);
        send_to_char(buf,ch);
    }
}

void do_artisteal(CHAR_DATA * ch, char * argument)
{
    CHAR_DATA * pVictim;
    OBJ_DATA * pObj;
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];

    argument = one_argument(argument, arg1);
    argument = one_argument(argument, arg2);

    if((arg1[0] == '\0') || (arg2[0] == '\0'))
    {
        send_to_char("Syntax: artisteal <character> <object>\n\r", ch);
        return;
    }

    if((pVictim = get_char_room(ch, arg1)) == NULL)
    {
        send_to_char("Artisteal from who?\n\r", ch);
        return;
    }

    if(IS_NPC(pVictim))
    {
        send_to_char("Not from NPC's.\n\r", ch);
        return;
    }

    if(ch == pVictim)
    {
        send_to_char("From yourself?\n\r", ch);
        return;
    }

    if(ragnarok == TRUE)
    {
        send_to_char("You cannot artisteal during Apocalypse.\n\r", ch);
        return;
    }

    if((pVictim->position > 1) && (!IS_IMMORTAL(ch)))
    {
        send_to_char("You can only artisteal from mortally wounded characters.\n\r", ch);
        return;
    }

    if(pVictim->level < LEVEL_AVATAR)
    {
        send_to_char("You can only steal from avatars.\n\r", ch);
        return;
    }

    if(IS_SET(ch->in_room->room_flags, ROOM_ARENA))
    {
        send_to_char("Not in the arena!\n\r", ch);
        return;
    }

    if(in_fortress(ch))
    {
        send_to_char("Not in the Forbidden Fortress!\n\r", ch);
        return;
    }

    for(pObj = pVictim->carrying; pObj != NULL; pObj = pObj->next_content)
    {
        if(is_name(arg2, pObj->name) && IS_SET(pObj->quest, QUEST_ARTIFACT))
        {
            break;
        }
    }

    if(pObj == NULL)
    {
        send_to_char("They are not carrying that object.\n\r", ch);
        return;
    }

    obj_from_char(pObj);
    obj_to_char(pObj, ch);

    act("$n steals $p from you!", ch, pObj, pVictim, TO_VICT);
    act("You steal $p from $N!", ch, pObj, pVictim, TO_CHAR);
    act("$n steals $p from $N!", ch, pObj, pVictim, TO_NOTVICT);

    sprintf(buf, "%s#n has stolen %s#n from %s#n!", ch->name, pObj->short_descr, pVictim->name);
    do_info(ch, buf);

    sprintf(buf, "%s artisteals object \"%s\" from %s.", ch->name, pObj->short_descr, pVictim->name);
    wiznet( NULL, WIZ_DEATH, 0, buf );
    log_string(buf);

    pVictim->fight_timer = 0;
    do_goto(pVictim, "3054");
    do_restore(pVictim, pVictim->name);

    return;
}
