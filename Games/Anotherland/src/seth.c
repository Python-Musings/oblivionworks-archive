#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "merc.h"
#include <unistd.h>

void save_pkstore();
void Transact_StoreItem(CHAR_DATA *ch, int nNumber );
void ShowDetails(CHAR_DATA *ch, int nNumber );
void Show_StoreList(CHAR_DATA *ch);
double CalculateRounds(CHAR_DATA *ch, CHAR_DATA *victim);
void ShowAnnounce(CHAR_DATA *ch, int nAnnounce);
void EditAnnounce(CHAR_DATA *ch, int nNumber, char *argument);
void ShowAnnounceList(CHAR_DATA *ch);
void SaveAnnounce();
bool remove_forge_affect(OBJ_DATA * pObj);

void InvadeArea(AREA_DATA * pArea, MOB_INDEX_DATA * pMobIndex, int count);

const char *sBuild = "v5.321";
const char *sModification = "All classes with super invis lose 5k move a tick";
const char *typo_table [32] =
{
    "Stop POKING ME!\n\r",
    "I'll do it when I get around to it.\n\r",
    "Yes Sire.\n\r",
    "What was that..Jerky?\n\r",
    "Use BOTH hands when you type.\n\r",
    "Negative Sir, Try again.\n\r",
    "Even elder races get tired of waiting.\n\r",
    "Time is of the essence.\n\r",
    "You never touch the other elves that way.\n\r",
    "System Checklist Initiated -\n\r",
    "Running Level 1 Diagnostic -\n\r",
    "USDA Approved -\n\r",
    "FDIC Insured -\n\r",
    "You want it done when?!?!?\n\r",
    "I see you need some major typing lessons...\n\r",
    "No, we do not give free handjobs.\n\r",
    "Ummm no, I do not do that.. Sorry.\n\r",
    "Dude, are you trying to type hieroglyphics here??\n\r",
    "Hold on, I am still working on your first letter.\n\r",
    "You could try to shaft someone with that...\n\r",
    "Hey, stop trying to gather too much attention.\n\r",
    "Oh thats it, I am going to ignore your ass.\n\r",
    "Riiiite, keep typing that and it just might happen!\n\r",
    "Have you ever heard of Mavis Beacon? It's a typing program...\n\r",
    "Smash thoes keys, do it, do it now!!\n\r",
    "Hey dumb-ass, get a translator.\n\r",
    "You would do better with speech recognition my friend...\n\r",
    "Smashing the keyboard won't help, you need therapy.\n\r",
    "Have you tried looking at your butt and asked how that is similar to you?\n\r",
    "You drunkard, stop hitting the keyboard with your head and you might get stuff done.\n\r",
    "You're one lousy pansy you know that?",
    "It's called computer illiterate, yes, that's you.\n\r",
};

void insert_history(CHAR_DATA *ch, char *channel, char *line)
{
    int nIndex;
    int nEnd = -1;
    char * pLineCopy = (char *)alloc_mem(strlen(line) + 1);
    strcpy(pLineCopy, line);

    if ( !str_cmp(channel,"tell") )
    {
        for(nIndex = 0;nIndex <= 9;nIndex++)
        {
            if(ch->pcdata->historytell[nIndex] == NULL)
            {
                nEnd = nIndex;
                break;
            }
        }
        if(nEnd == -1)
        {
            for(nIndex = 1;nIndex <= 9;nIndex++)
            {
                ch->pcdata->historytell[nIndex - 1] = ch->pcdata->historytell[nIndex];
            }
            nEnd = 9;
            free_string(ch->pcdata->historytell[nEnd]);
        }
        //		sprintf(ch->pcdata->historytell[nEnd],"%s",line);
    }
    if ( !str_cmp(channel,"chat") )
    {
        for(nIndex = 0;nIndex <= 9;nIndex++)
        {
            if(ch->pcdata->historychat[nIndex] == NULL)
            {
                nEnd = nIndex;
                break;
            }
        }
        if(nEnd == -1)
        {
            for(nIndex = 1;nIndex <= 9;nIndex++)
            {
                ch->pcdata->historychat[nIndex - 1] = ch->pcdata->historychat[nIndex];
            }
            nEnd = 9;
            free_string(ch->pcdata->historychat[nEnd]);
        }
        //		sprintf(ch->pcdata->historychat[nEnd],"%s",line);
    }
}

/*
void insert_history(CHAR_DATA *ch, char *channel, char *line)
{
    int nNum;
    if ( !str_cmp(channel,"Chat") )
    {
        for ( nNum = 0; nNum < 9; nNum ++ )
        {
//			sprintf(ch->pcdata->historychat[nNum+1],"%s",str_dup(ch->pcdata->historychat[nNum]));
            ch->pcdata->historychat[nNum+1] = str_dup(ch->pcdata->historychat[nNum]);
        }
        sprintf(ch->pcdata->historychat[0],"%s",str_dup(line));
    }
    if ( !str_cmp(channel,"Tell") )
    {
        for ( nNum = 0; nNum < 9; nNum ++ )
        {
//			sprintf(ch->pcdata->historytell[nNum+1],"%s",str_dup(ch->pcdata->historytell[nNum]));
            ch->pcdata->historytell[nNum+1] = str_dup(ch->pcdata->historytell[nNum]);
        }
        sprintf(ch->pcdata->historytell[0],"%s",str_dup(line));
    }
}*/
void do_history(CHAR_DATA *ch, char *argument)
{
    int nNum = 0;
    if ( IS_NPC(ch) ) return;
    if ( argument[0] == '\0' ) return stcf(ch,"Syntax: history <chat/tell>\n\r");
    if ( !str_cmp(argument,"tell" ) )
        for( nNum = 9; nNum >= 0; nNum-- )
    {
        if ( ch->pcdata->historytell[nNum][0] != '\0' )
            send_to_char(ch->pcdata->historytell[nNum],ch);
    }
    else if ( !str_cmp(argument,"chat" ) )
        for ( nNum = 9; nNum >= 0; nNum-- )
    {
        if ( ch->pcdata->historychat[nNum][0] != '\0' )
            send_to_char(ch->pcdata->historychat[nNum],ch);
    }
    /*	else if ( !str_cmp(argument,"clear") )
            for ( nNum = 0; nNum < 10; nNum++ )
            {
                free_string(ch->pcdata->historychat[nNum]);
                ch->pcdata->historychat[nNum] = NULL;
                free_string(ch->pcdata->historytell[nNum]);
                ch->pcdata->historytell[nNum] = NULL;
            }*/
    return;
}

void do_pkbuy( CHAR_DATA *ch, char *argument )
{
    char sNumber[10];
    char sCommand[100];
    argument = one_argument(argument, sCommand);
    one_argument(argument, sNumber);
    load_pkstore();
    if ( IS_NPC(ch) ) return;
    if ( ch->pcdata->pkpoints < 0 ) return stcf(ch,"Sorry you do not have enough pk-points to enter the store.\n\r");
    if ( sCommand[0] == '\0' ) return stcf(ch, "Syntax: pkbuy <list/buy/detail> <ID>\n\r");
    if ( str_cmp(sCommand,"list") && !is_number(sNumber) ) return stcf(ch, "You didn't type in list/buy/detail and an item ID.\n\r");
    if ( !str_cmp(sCommand,"list" ))
    {
        save_pkstore();
        Show_StoreList(ch);
        return;
    }
    else if ( !str_cmp(sCommand,"detail") )
    {
        if ( !is_number(sNumber) ) return stcf(ch,"You didn't type in a number.\n\r");
        ShowDetails(ch,atoi(sNumber));
        return;
    }
    else if ( !str_cmp(sCommand,"buy") && is_number(sNumber) )
    {
        Transact_StoreItem(ch,atoi(sNumber));
        return;
    }
    return stcf(ch,"You didn't follow something correctly.\n\r");
}

void Show_StoreList(CHAR_DATA *ch)
{
    int nNum;
    load_pkstore();
    stcf(ch,"Welcome to %s:\n\r",pkstore_table.sStoreName);
    stcf(ch,"Please have a look around.\n\r");
    for ( nNum = 0; nNum < 10; nNum ++ )
    {
        if ( pkstore_table.sItem[nNum] == NULL ) return;
        if ( !IS_IMMORTAL(ch) )
            stcf(ch,"#0[#RID:#C%-2d#0][#RItem Name:#C %-50s#0 ] \n\r[#RPkp Req:#C %-4d#0][#RQp Points:#C %-6d#0][#RPkpoint Reduction:#C %-3d#0][#RBought: #C%-4d times#0]#n\n\r\n\r",
                nNum,
                pkstore_table.sItem[nNum],
                pkstore_table.nItemPk[nNum],
                pkstore_table.nItemQp[nNum],
                pkstore_table.nItemDe[nNum],
                pkstore_table.nItemBought[nNum]);
        else
            stcf(ch,"#0[#RID:#C%-2d#0][#RItem Name:#C %-50s#0 ][#RVnum: #C%-6d#0] \n\r#0[#RPkp Req:#C %-4d#0][#RQp Points:#C %-6d#0][#RPkpoint Reduction:#C %-3d#0][#RBought: #C%-4d times#0]#n\n\r\n\r",
                nNum,
                pkstore_table.sItem[nNum],
                pkstore_table.nItemVnum[nNum],
                pkstore_table.nItemPk[nNum],
                pkstore_table.nItemQp[nNum],
                pkstore_table.nItemDe[nNum],
                pkstore_table.nItemBought[nNum]);

    }
    stcf(ch,"#GYou currently have %d pk points and %d quest points to work with.#n\n\r",ch->pcdata->pkpoints,ch->pcdata->quest);
    return;
}

void ShowDetails(CHAR_DATA *ch, int nNumber )
{
    if ( nNumber < 0 || nNumber >= 10 ) return stcf(ch,"Sorry, the ID is out of bounds.\n\r");
    stcf(ch,"#0[#RID:#C%-2d#0][#RItem Name:#C %-50s#0 ] \n\r[#RPkp Req:#C %-4d#0][#RQp Points:#C %-6d#0][#RPkpoint Reduction:#C %-3d#0][#RBought: #C%-4d times#0]#n\n\r\n\r",
        nNumber,
        pkstore_table.sItem[nNumber],
        pkstore_table.nItemPk[nNumber],
        pkstore_table.nItemQp[nNumber],
        pkstore_table.nItemDe[nNumber],
        pkstore_table.nItemBought[nNumber]);
    stcf(ch,"#0[#RDescription:#C\n\r%s #0]#n\n\r",pkstore_table.sItemDescriptions[nNumber]);
}

void Transact_StoreItem(CHAR_DATA *ch, int nNumber )
{
    OBJ_INDEX_DATA *pObjIndex;
    OBJ_DATA *obj;
    int nError = 0;
    if ( nNumber >= 10 || nNumber < 0 ) return stcf(ch,"Your Item ID is out of range. Try again.\n\r");
    if ( pkstore_table.sItem[nNumber][0] == '\0' )
    {
        stc("Warning: No item name. Proceeding.\n\r",ch);
        nError++;
    }
    if ( pkstore_table.nItemPk[nNumber] == 0 )
    {
        stc("Warning: No Pk Point Value. Proceeding.\n\r",ch);
        nError++;
    }
    if ( pkstore_table.nItemQp[nNumber] == 0 )
    {
        stc("Warning: No Qp Point Value. Proceeding.\n\r",ch);
        nError++;
    }
    if ( pkstore_table.nItemVnum[nNumber] == 0 )
    {
        stc("Warning: No Object. Proceeding.\n\r",ch);
        nError += 2;
    }
    if ( nError >= 3 ) return stcf(ch,"Sorry, too many warnings to go on.\n\r");
    if ( ch->pcdata->pkpoints < pkstore_table.nItemPk[nNumber] )
        return stcf(ch, "You don't have the required %d pkpoints to buy this item.\n\r",pkstore_table.nItemPk[nNumber]);
    if ( ch->pcdata->quest < pkstore_table.nItemQp[nNumber] )
        return stcf(ch, "You don't have the required %d quest points to buy this item.\n\r",pkstore_table.nItemQp[nNumber]);
    if ( ch->pcdata->pkpoints < pkstore_table.nItemDe[nNumber] )
        return stcf(ch, "You don't have the required %d pkpoint reduction requirement to buy this item.\n\r",pkstore_table.nItemDe[nNumber]);
    if ( pkstore_table.nItemVnum[nNumber] > 0 )
    {
        pObjIndex = get_obj_index( pkstore_table.nItemVnum[nNumber] );
        if ( pObjIndex == NULL )
        {
            send_to_char( "Seth has majorly fucked with his store command, please report it..\n\r", ch );
            return;
        }
        obj = create_object( pObjIndex, ch->level );
        if ( !CAN_WEAR(obj, ITEM_TAKE) )
        {
            extract_obj(obj);
            return stcf(ch,"That item <vnum:%d> can't be taken, might as well not buy it. Please report it.\n\r",pkstore_table.nItemVnum[nNumber]);
        }
        obj_to_char( obj, ch );
        act( "$p is conjured in $n's hands by his mighty power!", ch, obj, NULL, TO_ROOM );
        if ( !IS_SET(obj->quest, QUEST_ANLA) )
        {
            SET_BIT(obj->quest, QUEST_ANLA);
        }
        if (obj->questmaker != NULL) free_string(obj->questmaker);
        obj->questmaker = str_dup(ch->name);
        obj->questowner = str_dup(ch->name);
        /*Make sure all pkbought items are resistant, as they should be.*/
        obj->resistance = 1;
        if (obj->item_type == ITEM_QUESTCARD ) obj->level = 500;
        act( "With your power, you create $p.", ch, obj, NULL, TO_CHAR );
    }
    ch->pcdata->pkpoints -= pkstore_table.nItemDe[nNumber];
    ch->pcdata->quest -= pkstore_table.nItemQp[nNumber];
    if ( !IS_IMMORTAL(ch) ) pkstore_table.nItemBought[nNumber]++;
    save_pkstore();
    return stcf(ch,"Thank you for buying from the store. Please come again.\n\r");
}

void load_pkstore()
{
    int nNum;
    FILE *fp;
    if ((fp = fopen("../txt/pkstore.txt", "r")) == NULL)
    {
        log_string("Fatal Error: pkstore.txt not found! Making one!");
        save_pkstore();
        return;
    }
    pkstore_table.sStoreName = fread_string ( fp );
    for ( nNum = 0; nNum < 10; nNum++ )
    {
        pkstore_table.sItem[nNum] = str_dup(fread_string( fp ));
        pkstore_table.sItemDescriptions[nNum] = str_dup(fread_string( fp ));
        pkstore_table.nItemVnum[nNum] = fread_number( fp );
        pkstore_table.nItemPk[nNum] = fread_number( fp );
        pkstore_table.nItemQp[nNum] = fread_number( fp );
        pkstore_table.nItemDe[nNum] = fread_number( fp );
        pkstore_table.nItemBought[nNum] = fread_number( fp );
    }
    fclose(fp);
}

void save_pkstore()
{
    int nNum;
    FILE *fp;
    if ((fp = fopen("../txt/pkstore.txt", "w")) == NULL)
    {
        log_string("Fatal Error: pkstore.txt not found! Making one!");
        return;
    }
    fprintf(fp, "%s~\n",pkstore_table.sStoreName);
    for ( nNum = 0; nNum < 10; nNum++ )
    {
        fprintf(fp, "%s~\n",pkstore_table.sItem[nNum]);
        fprintf(fp, "%s~\n",pkstore_table.sItemDescriptions[nNum]);
        fprintf(fp, "%d %d %d %d %d\n",
            pkstore_table.nItemVnum[nNum],
            pkstore_table.nItemPk[nNum],
            pkstore_table.nItemQp[nNum],
            pkstore_table.nItemDe[nNum],
            pkstore_table.nItemBought[nNum]);
    }
    fclose(fp);
}

void do_pkedit(CHAR_DATA *ch, char *argument)
{
    int nNumber, nValue;
    char sNumber[10];
    char sBitVector[100];
    char sValue[1000];
    if ( IS_NPC(ch) ) return;
    load_pkstore();
    if ( str_cmp(ch->name,"Seth") && str_cmp(ch->name,"Silk") && str_cmp(ch->name,"Chrono") ) return stcf(ch, "You're not signed as a person who can legally edit any of the variables... Sorry.\n\r");
    argument = one_argument(argument,sNumber);
    argument = one_argument(argument,sBitVector);
    strcpy(sValue,argument);
    if ( sNumber[0] == '\0' ) return stcf(ch,"You have to enter a number please.\n\r");
    nNumber = atoi(sNumber);
    if ( nNumber >= 10 || nNumber < 0 ) return stcf(ch,"That is an invalid ID number.\n\r");
    if ( sBitVector[0] == '\0' ) return stcf(ch, "You can modify any of these values: sItemDescription, sItem, nItemVnum, nItemPk, nItemQp, nItemDe.\n\r");
    if ( sValue[0] == '\0' ) return stcf(ch, "You have to actually MODIFY them?\n\r");
    if ( !str_cmp(sBitVector,"sItemDescription"      ))
    {
        pkstore_table.sItemDescriptions[nNumber] = str_dup(sValue);
    }
    else if ( !str_cmp(sBitVector,"sItem"        ))
    {
        pkstore_table.sItem[nNumber] = str_dup(sValue);
    }
    else if ( !str_cmp(sBitVector,"nItemVnum"        ))
    {
        nValue = atoi(sValue);
        pkstore_table.nItemVnum[nNumber] = nValue;
    }
    else if ( !str_cmp(sBitVector,"nItemPk"      ))
    {
        nValue = atoi(sValue);
        pkstore_table.nItemPk[nNumber] = nValue;
    }
    else if ( !str_cmp(sBitVector,"nItemQp"      ))
    {
        nValue = atoi(sValue);
        pkstore_table.nItemQp[nNumber] = nValue;
    }
    else if ( !str_cmp(sBitVector,"nItemDe"      ))
    {
        nValue = atoi(sValue);
        pkstore_table.nItemDe[nNumber] = nValue;
    }
    save_pkstore();
    return stcf(ch,"Item number %d 's %s has been changed.\n\r",nNumber,sBitVector);
}

void do_deforge( CHAR_DATA *ch, char *argument)
{
    OBJ_DATA *obj;
    char sCommand[100];
    int nValues[8] = { -3, -6, -9, -12, -24, -36, -48, -96 };
    int temp;

    if ( IS_NPC(ch) ) return;
    one_argument(argument, sCommand);
    if ( sCommand[0] == '\0' ) return stcf(ch,"Syntax: deforge <item>\n\r");
    if ( ( obj = get_obj_carry( ch, sCommand ) ) == NULL )
    {
        return stcf( ch,"You do not have that item.\n\r");
    }
    if( !IS_FORGED(obj))
    {
        return stcf(ch,"That item doesn't have any forging on it.\n\r");
    }
    if ( IS_SET(obj->spectype, SITEM_COPPER) )
    {
        REMOVE_BIT(obj->spectype, SITEM_COPPER);
        if(!remove_forge_affect(obj))
        {
            forge_affect(obj, nValues[0]);
        }
        temp = ch->pcdata->security;
        ch->pcdata->security = 6;
        do_oload(ch,"30049");
        do_claim(ch,"slab");
        ch->pcdata->security = temp;
        return stcf(ch,"With pain-staking labor, you remove the copper off your item.\n\r",ch);
    }
    else if ( IS_SET(obj->spectype, SITEM_IRON) )
    {
        REMOVE_BIT(obj->spectype, SITEM_IRON);
        if(!remove_forge_affect(obj))
        {
            forge_affect(obj, nValues[1]);
        }
        temp = ch->pcdata->security;
        ch->pcdata->security = 6;
        do_oload(ch,"30050");
        do_claim(ch,"slab");
        ch->pcdata->security = temp;
        return stcf(ch,"With pain-staking labor, you remove the iron off your item.\n\r",ch);
    }
    else if ( IS_SET(obj->spectype, SITEM_STEEL) )
    {
        REMOVE_BIT(obj->spectype, SITEM_STEEL);
        if(!remove_forge_affect(obj))
        {
            forge_affect(obj, nValues[2]);
        }
        temp = ch->pcdata->security;
        ch->pcdata->security = 6;
        do_oload(ch,"30051");
        do_claim(ch,"slab");
        ch->pcdata->security = temp;
        return stcf(ch,"With pain-staking labor, you remove the steel off your item.\n\r",ch);
    }
    else if ( IS_SET(obj->spectype, SITEM_ADAMANTITE) )
    {
        REMOVE_BIT(obj->spectype, SITEM_ADAMANTITE);
        if(!remove_forge_affect(obj))
        {
            forge_affect(obj, nValues[3]);
        }
        temp = ch->pcdata->security;
        ch->pcdata->security = 6;
        do_oload(ch,"30052");
        do_claim(ch,"slab");
        ch->pcdata->security = temp;
        return stcf(ch,"With pain-staking labor, you remove the adamantite off your item.\n\r",ch);
    }
    else if ( IS_SET(obj->spectype, SITEM_TITANIUM) )
    {
        REMOVE_BIT(obj->spectype, SITEM_TITANIUM);
        if(!remove_forge_affect(obj))
        {
            forge_affect(obj, nValues[4]);
        }
        temp = ch->pcdata->security;
        ch->pcdata->security = 6;
        do_oload(ch,"101017");
        do_claim(ch,"slab");
        ch->pcdata->security = temp;
        return stcf(ch,"With pain-staking labor, you remove the titanium off your item.\n\r",ch);
    }
    else if ( IS_SET(obj->spectype, SITEM_DIAMOND) )
    {
        REMOVE_BIT(obj->spectype, SITEM_DIAMOND);
        if(!remove_forge_affect(obj))
        {
            forge_affect(obj, nValues[5]);
        }
        temp = ch->pcdata->security;
        ch->pcdata->security = 6;
        do_oload(ch,"101018");
        do_claim(ch,"slab");
        ch->pcdata->security = temp;
        return stcf(ch,"With pain-staking labor, you remove the zirconium off your item.\n\r",ch);

    }
    else if ( IS_SET(obj->spectype, SITEM_EXCAVIER) )
    {
        REMOVE_BIT(obj->spectype, SITEM_EXCAVIER);
        if(!remove_forge_affect(obj))
        {
            forge_affect(obj, nValues[6]);
        }
        temp = ch->pcdata->security;
        ch->pcdata->security = 6;
        do_oload(ch,"101019");
        do_claim(ch,"slab");
        ch->pcdata->security = temp;
        return stcf(ch,"With pain-staking labor, you remove the excavier off your item.\n\r",ch);
    }
    else if ( IS_SET(obj->spectype, SITEM_LIQUID_SHADOW) )
    {
        REMOVE_BIT(obj->spectype, SITEM_LIQUID_SHADOW);
        if(!remove_forge_affect(obj))
        {
            forge_affect(obj, nValues[7]);
        }
        temp = ch->pcdata->security;
        ch->pcdata->security = 6;
        do_oload(ch,"101024");
        do_claim(ch,"slab");
        ch->pcdata->security = temp;
        return stcf(ch,"With pain-staking labor, you remove the mithril off your item.\n\r",ch);
    }

    else
        return stcf(ch,"Something went very wrong.\n\r");
}

bool remove_forge_affect(OBJ_DATA * pObj)
{
    if(!is_affected_obj(pObj, gsn_forge))
    {
        return FALSE;
    }

    affect_strip_obj(pObj, gsn_forge);

    return TRUE;
}

void do_immlocate( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char type[10];
    OBJ_DATA *obj;
    OBJ_DATA *in_obj;
    CHAR_DATA *victim;
    bool found;
    int count=0;
    int nVnum = 0;

    if ( IS_NPC(ch) ) return;
    argument = one_argument(argument,type);
    if ( argument[0] == '\0' || type[0] == '\0' ) return stcf(ch, "Syntax: immlocate <mob/obj> <vnum>.\n\r");

    if ( !is_number(argument) ) return stcf(ch, "A Number.\n\r");

    nVnum = atoi(argument);

    found = FALSE;
    if ( !str_cmp(type,"obj" ) )
    {
        for ( obj = object_list; obj != NULL; obj = obj->next )
        {
            if (obj->pIndexData->vnum != nVnum)
                continue;
            found = TRUE;
            for ( in_obj = obj; in_obj->in_obj != NULL; in_obj = in_obj->in_obj );
            if ( in_obj->carried_by != NULL )
            {
                sprintf( buf, "%s carried by %s.\n\r",
                    obj->short_descr, PERS(in_obj->carried_by, ch) );
            }
            else
            {
                sprintf( buf, "%s in %s.\n\r",
                    obj->short_descr, in_obj->in_room == NULL
                    ? "somewhere" : in_obj->in_room->name );
            }
            buf[0] = UPPER(buf[0]);
            send_to_char( buf, ch );
            if (count > 50 )
                break;
            else
                count++;
        }
        if ( !found )
            send_to_char( "Nothing like that in hell, earth, or heaven.\n\r", ch );
    }
    if ( !str_cmp(type,"mob" ) )
    {
        for ( victim = char_list; victim != NULL; victim = victim->next )
        {
            if (!IS_NPC(victim) || victim->pIndexData->vnum != nVnum)
                continue;
            found = TRUE;
            sprintf( buf, "%s at %s #R[#yVnum:#L%d#R]#n.\n\r",
                victim->short_descr, victim->in_room->name, victim->in_room->vnum);
            buf[0] = UPPER(buf[0]);
            send_to_char( buf, ch );
            if (count > 50 )
                break;
            else
                count++;
        }
        if ( !found )
            send_to_char( "Nothing like that in hell, earth, or heaven.\n\r", ch );
    }
    return;
}

void GiveBonus( CHAR_DATA *ch, CHAR_DATA *victim,char *type)
{
    const int nBONUS_QP_ROUND = 25;
    const int nBONUS_XP_ROUND = 1;
    int nBonus;
    double dRounds;

    if ( IS_NPC(ch) ) return;
    if ( !str_cmp(type,"quest") )
    {
        dRounds = CalculateRounds(ch,victim);
        if ( number_range(1,2) == 1 )
        {
            nBonus = nBONUS_QP_ROUND*dRounds;
            if ( nBonus > 10000 ) nBonus = number_range(9000,11000);
            if ( nBonus <= 0 ) nBonus = number_range(1,10);
            if ( pulse_doubleqp > 0)
            {
                nBonus=nBonus*10;
            }
            else
            {
                nBonus=nBonus*5;
            }
            stcf(ch,"You receive a bonus of %d qps for killing %s!!\n\r",nBonus,victim->name);
            ch->pcdata->quest += nBonus;
            ch->pcdata->questtotal += nBonus/2;
        }
        else
        {
            nBonus = nBONUS_XP_ROUND*dRounds;
            if ( nBonus > 10 ) nBonus = number_range(5,15);
            if ( nBonus <= 0 ) nBonus = number_range(1,2);
            stcf(ch,"You recieve a bonus of %d stores for killing %s!!\n\r",nBonus,victim->name);
            ch->pcdata->store += nBonus;
        }
    }
    return;
}

double CalculateRounds(CHAR_DATA *ch, CHAR_DATA *victim)
{
    double dReduction = 0;
    double dRounds;
    int nPerRound = 0;
    if ( !IS_NPC(ch) )
    {
        dReduction = (100-victim->pIndexData->hitnodice)/(double)(100);
        nPerRound = ch->damcap[DAM_CAP]*number_attacks(ch,victim)*dReduction;
        dRounds =(double) victim->max_hit/(nPerRound+1);
        dRounds += number_range(.5,2);
        return dRounds;
    }
    return 0;
}

void do_invade( CHAR_DATA *ch , char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    char arg3[MAX_INPUT_LENGTH];
    AREA_DATA *tarea;
    int count;
    bool found=FALSE;
    MOB_INDEX_DATA *pMobIndex;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
    argument = one_argument( argument, arg3 );
    count = atoi( arg2 );
    if ( arg1[0] == '\0' || arg2[0] == '\0' )
    {
        send_to_char( "Invade <area> <# of invaders> <mob vnum>\n\r", ch );
        return;
    }
    for ( tarea = area_first; tarea; tarea = tarea->next )
        if ( !str_cmp( tarea->filename, arg1 ) )
    {
        found = TRUE;
        break;
    }
    if ( !found )
    {
        send_to_char( "Area not found.\n\r", ch );
        return;
    }
    if ( count > 300)
    {
        send_to_char( "Whoa...Less than 300 please.\n\r", ch );
        return;
    }
    if ( ( pMobIndex = get_mob_index( atoi(arg3) ) ) == NULL )
    {
        send_to_char( "No mobile has that vnum.\n\r", ch );
        return;
    }

    InvadeArea(tarea, pMobIndex, count);

    send_to_char( "The invasion was successful!\n\r", ch );

    return;
}

void do_scatter( CHAR_DATA *ch , char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    char arg3[MAX_INPUT_LENGTH];
    char buffer[MAX_STRING_LENGTH];
    char *victimname;
    AREA_DATA *tarea;
    int count, created;
    bool found=FALSE;
    OBJ_DATA *victim;
    OBJ_INDEX_DATA *pObjIndex;
    ROOM_INDEX_DATA *location;
    int nLvnum = 0;
    int nUvnum = 0;
    char arg4[100];
    char arg5[100];
    bool bRange = FALSE;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
    argument = one_argument( argument, arg3 );
    count = atoi( arg2 );
    if ( argument[0] != '\0' )
    {
        argument = one_argument( argument, arg4 );
        argument = one_argument( argument, arg5 );
        nLvnum = atoi( arg4 );
        nUvnum = atoi( arg5 );
        if ( nLvnum > nUvnum )
        {
            stc("Upper Vnum has to be higher then Lower Vnum.\n\r",ch);
            return;
        }
        else if ( ( nLvnum == nUvnum ) || ( nLvnum == 0 ) || ( nUvnum == 0 ) )
        {
            stc("Invalid vnums.\n\r",ch);
            return;
        }
        bRange = TRUE;
    }

    if ( arg1[0] == '\0' || arg2[0] == '\0' || arg3[0] == '\0')
    {
        send_to_char( "Invade <area> <# of invaders> <obj vnum> (optional <lvnum> <uvnum>)\n\r", ch );
        return;
    }

    for ( tarea = area_first; tarea != NULL; tarea = tarea->next )
    {
        if ( !str_cmp( tarea->filename, arg1 ) )
        {
            found = TRUE;
            break;
        }
    }

    if ( !found )
    {
        send_to_char( "Area not found.\n\r", ch );
        return;
    }

    if ( bRange == FALSE )
    {
        nLvnum = tarea->lvnum;
        nUvnum = tarea->uvnum;
    }
    if ( count > 100 )
    {
        send_to_char( "Whoa...Less than 100 please.\n\r", ch );
        return;
    }

    if ( ( pObjIndex = get_obj_index( atoi(arg3) ) ) == NULL )
    {
        send_to_char( "No object has that vnum.\n\r", ch );
        return;
    }

    victimname = pObjIndex->short_descr;

    for ( created=0; created < count; created++ )
    {
        if ( (location = get_room_index(number_range(nLvnum, nUvnum ))) == NULL )
        {
            --created;
            continue;
        }
        if (IS_SET( location->room_flags, ROOM_SAFE))
        {
            --created;
            continue;
        }
        victim = create_object(  pObjIndex, number_range(10,1000) );
        if ( !IS_SET(victim->quest, QUEST_ANLA) )
        {
            SET_BIT(victim->quest, QUEST_ANLA);
        }
        obj_to_room( victim, location );
        act( "$P appears as part of an invasion force!", ch, NULL, victim, TO_ROOM );
    }
    send_to_char( "The invasion was successful!\n\r", ch );
    sprintf( buffer, "#R%d objects of name %s gets scattered across #y %s #R's !!!\n\r",
        created, victimname, tarea->name );
    do_questchat2( buffer );

    return;
}

void do_mudannounce(CHAR_DATA *ch, char *argument)
{
    char sCommand[100];
    char sNumber[10];
    if ( IS_NPC(ch) ) return;
    argument = one_argument(argument, sCommand);
    argument = one_argument(argument, sNumber);
    if ( sCommand[0] == '\0' ) return stcf(ch,"Syntax: mudannounce <read/list/edit> <ID>\n\r");
    if ( !str_cmp(sCommand,"read") )
    {
        if ( !is_number(sNumber) || sNumber[0] == '\0' )
        {
            return stcf(ch,"Please enter a number.\n\r");
        }
        return ShowAnnounce(ch,atoi(sNumber));
    }
    if ( !str_cmp(sCommand,"list") )
        return ShowAnnounceList(ch);
    if ( !str_cmp(sCommand,"edit") )
    {
        if ( !is_number(sNumber) || sNumber[0] == '\0' )
        {
            return stcf(ch,"Please enter a number.\n\r");
        }
        return EditAnnounce(ch,atoi(sNumber),argument);
    }
    return stcf(ch,"Follow the syntax.\n\r");
}

void ShowAnnounce(CHAR_DATA *ch, int nAnnounce)
{
    if ( nAnnounce < 0 || nAnnounce >= 10 ) return stcf(ch,"Bad ID");
    if ( mudannounce_table[nAnnounce].caAnnounceName == NULL ) return stcf(ch,"No announcement for that ID.\n\r");
    stcf(ch,"#LAnnouncement#G[#y%-2d#G]#n\n\r#G[#LTopic:#y%-50s#G]#n\n\r",nAnnounce,mudannounce_table[nAnnounce].caAnnounceName);
    stcf(ch,"#G[#LDetails:#y\n\r%s#G]#n\n\r",mudannounce_table[nAnnounce].caAnnounceDescription);
    stcf(ch,"Done.\n\r");
    return;
}

void EditAnnounce(CHAR_DATA *ch, int nNumber, char *argument)
{
    char sCommand[20];
    char sString[10000];
    if ( !IS_IMMORTAL(ch) ) return stcf(ch,"You're not an immortal.\n\r");
    if ( nNumber < 0 || nNumber >= 10 ) return stcf(ch,"Error ID.\n\r");
    argument = one_argument(argument,sCommand);
    strcpy(sString,argument);
    if ( sCommand[0] == '\0' || argument[0] == '\0' )
        return stcf(ch,"Syntax mudannounce edit <ID> <name/description> <name/description infomation> \n\r");

    if ( !str_cmp(sCommand,"name") )
        mudannounce_table[nNumber].caAnnounceName = str_dup(sString);
    else if ( !str_cmp(sCommand,"description") )
        mudannounce_table[nNumber].caAnnounceDescription = str_dup(sString);
    else if ( !str_cmp(sCommand,"save") )
        SaveAnnounce();
    else if ( !str_cmp(sCommand,"load") )
        LoadAnnounce();
    else
    {
        return stcf(ch,"You didn't follow the syntax.\n\r");
    }
    SaveAnnounce();
    return stcf(ch, "You successfully set the parameters for the announce board, and it has been saved.\n\r");
}

void ShowAnnounceList(CHAR_DATA *ch)
{
    int nNum;
    LoadAnnounce();
    stcf(ch,"Annoucements:\n\r");
    for( nNum = 0; nNum < 10; nNum++ )
    {
        stcf(ch,"#LAnnoucement#G[#y%-2d#G]:#y%-50s#n\n\r",nNum,mudannounce_table[nNum].caAnnounceName);
    }
    return stcf(ch,"End of announcement list. Type mudannounce to find out how to use this.\n\r");
}

void LoadAnnounce()
{
    int nNum;
    FILE *fpi;
    if ((fpi = fopen("../txt/announce.txt", "r")) == NULL)
    {
        log_string("Fatal Error: announce.txt not found! Making one!");
        SaveAnnounce();
        return;
    }
    for ( nNum = 0; nNum < 10; nNum++ )
    {
        mudannounce_table[nNum].caAnnounceName = str_dup(fread_string( fpi ));
        mudannounce_table[nNum].caAnnounceDescription = str_dup(fread_string( fpi ));
    }
    fclose(fpi);
}

void SaveAnnounce()
{
    int nNum;
    FILE *fpi;
    if ((fpi = fopen("../txt/announce.txt", "w")) == NULL)
    {
        log_string("Fatal Error: announce.txt could not be opened for writing.");
        return;
    }
    for ( nNum = 0; nNum < 10; nNum++ )
    {
        if ( mudannounce_table[nNum].caAnnounceName == NULL )
            fprintf(fpi, "~\n");
        else
            fprintf(fpi, "%s~\n",mudannounce_table[nNum].caAnnounceName);
        if ( mudannounce_table[nNum].caAnnounceDescription == NULL )
            fprintf(fpi, "~\n");
        else
            fprintf(fpi, "%s~\n",mudannounce_table[nNum].caAnnounceDescription);
    }
    fclose(fpi);
}

void do_defect(CHAR_DATA *ch, char *argument)
{
    char arg1[MAX_STRING_LENGTH];
    char buf[MAX_STRING_LENGTH];

    argument = one_argument(argument, arg1);

    if (ch->pcdata->kingdom == 0)
    {
        send_to_char("Defect from what?  You are not part of a kingdom!",ch);
        return;
    }

    if (str_cmp(arg1,"defect"))
    {
        send_to_char("If you want to defect you MUST type it twice.\n\rI.e defect defect\n\r",ch);
        return;
    }

    if ( !str_cmp(kingdom_table[ch->pcdata->kingdom].general,ch->name) || !str_cmp(kingdom_table[ch->pcdata->kingdom].leader,ch->name))
    {
        send_to_char("You may not defect without first renouncing your title.\n\r",ch);
        return;
    }

    send_to_char("You defect from your kingdom...",ch);
    sprintf(buf,"%s has defected.",ch->name);
    do_info(ch,buf);
    ch->pcdata->kingdom = 0;
    save_char_obj(ch);
    save_kingdoms();
    return;
}

void do_renounce(CHAR_DATA *ch, char *argument)
{
    char arg1[MAX_STRING_LENGTH];
    char buf[MAX_STRING_LENGTH];

    argument = one_argument(argument, arg1);

    if (ch->pcdata->kingdom == 0)
    {
        send_to_char("Renounce what?  You are not part of a kingdom!",ch);
        return;
    }

    if (str_cmp(arg1,"renounce"))
    {
        send_to_char("If you want to renounce your title you MUST type it twice.\n\rI.e renounce renounce\n\r",ch);
        return;
    }

    if ( str_cmp(kingdom_table[ch->pcdata->kingdom].general,ch->name) && str_cmp(kingdom_table[ch->pcdata->kingdom].leader,ch->name))
    {
        send_to_char("You may not renounce without first possessing a title.\n\r",ch);
        return;
    }

    if ( !str_cmp(kingdom_table[ch->pcdata->kingdom].general,ch->name) )
    {
        free_string(kingdom_table[ch->pcdata->kingdom].general);
        kingdom_table[ch->pcdata->kingdom].general = str_dup("No One!");
    }
    else if( !str_cmp(kingdom_table[ch->pcdata->kingdom].leader,ch->name) )
    {
        free_string(kingdom_table[ch->pcdata->kingdom].leader);
        kingdom_table[ch->pcdata->kingdom].leader = str_dup("No One!");
    }
    send_to_char("You renounce your title...",ch);
    sprintf(buf,"%s has renounced their title..",ch->name);
    do_info(ch,buf);
    save_char_obj(ch);
    save_kingdoms();
    return;
}

void do_wrlist( CHAR_DATA *ch, char *argument )
{
    ROOM_INDEX_DATA *room;
    ROOM_INDEX_DATA *in_room;
    MOB_INDEX_DATA *mob;
    OBJ_INDEX_DATA *obj;
    char arg[MAX_STRING_LENGTH];
    char arg1[MAX_STRING_LENGTH];
    char arg2[MAX_STRING_LENGTH];
    long uvnum;
    long lvnum;
    long MR = 0;
    int type = -1;

    argument = one_argument( argument, arg );
    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
    uvnum = ( is_number( arg2 ) ) ? atoi( arg2 ) : 0;
    lvnum = ( is_number( arg1 ) ) ? atoi( arg1 ) : 0;

    if ( !str_cmp( arg, "o" ) )
        type = 2;
    if ( !str_cmp( arg, "m" ) )
        type = 1;
    if ( !str_cmp( arg, "r" ) )
        type = 0;
    if ( !str_cmp( arg, "n" ) )
        type = 3;

    if ( ( uvnum - lvnum ) > 200 )
    {
        send_to_char( "{WThat range is too large.\n\r{x", ch );
        return;
    }

    if ( ( ( uvnum == 0 ) && ( lvnum == 0 ) ) || ( arg[0] == '\0' )
        || ( type == -1 ) )
    {
        send_to_char( "Syntax: wrlist [type] [lvnum] [uvnum]\n\r", ch );
        send_to_char( "Types are (O)bjects, (M)obs, (R)esets and Room(N)ames\n\r", ch );
        return;
    }

    if ( uvnum < 1 || lvnum < 1 || lvnum > uvnum )
    {
        send_to_char( "#RInvalid level(s).\n\r#n", ch );
        return;
    }

    in_room = ch->in_room;
    if ( type == 0 )
    {
        char_from_room( ch );
    }
    for ( MR = lvnum; MR <= uvnum; MR++ )
    {
        if ( type == 0 )
        {
            if ( ( room = get_room_index( MR ) ) )
            {
                sprintf( log_buf, "#R%-5d  #w%-20s#n\n\r", room->vnum, room->name);
                send_to_char( log_buf, ch );
                char_to_room( ch, room );
                do_resets( ch, "" );
                char_from_room( ch );
            }
        }
        if ( type == 3 )
        {
            if ( ( room = get_room_index( MR ) ) )
            {
                sprintf( log_buf, "#R%-5d  #w%-20s#n\n\r", room->vnum, room->name);
                send_to_char( log_buf, ch );
            }
        }
        if ( type == 2 )
        {
            if ( ( obj = get_obj_index( MR ) ) )
            {
                sprintf( log_buf, "#R%-5d  #w%-20s#n\n\r",  obj->vnum, obj->name );
                send_to_char( log_buf, ch );
            }
        }
        if ( type == 1 )
        {
            if ( ( mob = get_mob_index( MR ) ) )
            {
                sprintf( log_buf, "#R%-5d  #w%-20s#n\n\r", mob->vnum,mob->player_name );
                send_to_char( log_buf, ch );
            }
        }
    }
    if ( type == 0 )
        char_to_room( ch, in_room );
    return;
}

const char *random_huh()
{
    return typo_table[number_range(0,31)];
}

void InvadeArea(AREA_DATA * pArea, MOB_INDEX_DATA * pMobIndex, int count)
{
    int created;
    int nTries = 5000;                                      //Limiting factor to keep from going on contiously.
    ROOM_INDEX_DATA * location;
    CHAR_DATA * victim;
    char * victimname;
    char buffer[MAX_STRING_LENGTH];

    if(pArea == NULL || pMobIndex == NULL)
    {
        return;
    }

    victimname = pMobIndex->short_descr;
    if ( !IS_SET(pArea->area_flags,AREA_QUESTOK) )
    {
        log_string( "Autoquest: Illegal zone." );
        wiznet( NULL, WIZ_DEBUG, 0, "Autoquest: Illegal zone...retrying." );
        pulse_autoquest = 1;
        return;
    }

    for ( created=0; (created < count) && (nTries > 0); created++ )
    {
        if ( (location = get_room_index(number_range(pArea->lvnum, pArea->uvnum ))) == NULL )
        {
            --created;
            nTries--;
            continue;
        }
        if (IS_SET( location->room_flags, ROOM_SAFE) || IS_SET(location->room_flags, ROOM_ASTRAL) || IS_SET(location->room_flags, ROOM_NO_MOB))
        {
            --created;
            nTries--;
            continue;
        }

        victim = create_mobile( pMobIndex );
        char_to_room( victim, location );
        if ( !IS_SET(victim->act, ACT_QUEST) )
            SET_BIT(victim->act,ACT_QUEST);
    }

    if(nTries == 0)
    {
        sprintf( buffer, "#RA half-hearted force of %d %s's has invaded #y%s.\n\r", created, victimname, pArea->name);
        do_questchat2( buffer );
    }
    else
    {
        sprintf( buffer, "#RAn Invasion on the area#y %s #Rhas occurred by %d deadly %s's !!!\n\r", pArea->name, created, victimname );
        do_questchat2( buffer );
    }

    return;
}

void do_autoinvade()
{
    AREA_DATA *tarea;
    int count = number_range(5,100);
    int nPick = number_range(10,90);
    int nMobVnum;
    int nTries = 100;
    int nTimes = number_range(1,15);
    bool found=FALSE;
    MOB_INDEX_DATA *pMobIndex;
    //    count = number_range(1,100);

    do
    {
        nMobVnum = number_range(1,100000);
    }while(( pMobIndex = get_mob_index( nMobVnum ) ) == NULL && nTries-- >= 0);

    for ( tarea = area_first; tarea; tarea = tarea->next )
        if ( nTimes++ == nPick )
    {
        found = TRUE;
        break;
    }
    if ( !found )
    {
        log_string("Autoquest Failed." );
        wiznet( NULL, WIZ_DEBUG, 0, "Autoquest failed to start." );
        return;
    }
    if ( !IS_SET(tarea->area_flags,AREA_QUESTOK) )
    {
        log_string( "Autoquest: Illegal zone." );
        wiznet( NULL, WIZ_DEBUG, 0, "Autoquest: Illegal zone...retrying." );
        pulse_autoquest = 1;
        return;
    }

    if ( ( pMobIndex = get_mob_index( nMobVnum ) ) == NULL )
    {
        log_string( "Autoquest: No mobile has that vnum." );
        wiznet( NULL, WIZ_DEBUG, 0, "Autoquest: No mobile has that vnum." );
        pulse_autoquest = 1;
        return;
    }

    if ( IS_SET(pMobIndex->act, ACT_TRAIN))
    {
        log_string( "Autoquest: Illegal Mobile." );
        wiznet( NULL, WIZ_DEBUG, 0, "Autoquest: Illegal Mobile...retrying." );
        pulse_autoquest = 1;
        return;
    }

    if ( pMobIndex->level > 1000 )
    {
        do_autoinvade();
        return;
    }
    /*	if ( count < 20)
    //	{
    //		count=20;
    //	}
    //	else if ( count == 0)
    //	{
    //		count=20;
    //	}
    */
    if ( count==0)
    {
        wiznet( NULL, WIZ_DEBUG, 0, "Autoquest: Count came up 0.");
        pulse_autoquest = 1;
        return;
    }

    count = number_range(20,100);
    count = count+1;
    InvadeArea(tarea, pMobIndex, count);

    return;
}

void do_build(CHAR_DATA *ch, char *argument)
{
    stcf(ch,"The mud is running on build: %s\n\r",sBuild);
    stcf(ch,"Last modification: %s.\n\r",sModification);
    stcf(ch,"The first number is revision number, second is change number of that build.\n\r");
}

void do_showkills(CHAR_DATA *ch, char *argument)
{
    char sCommand[10];
    char sNumber[6];
    int nNum = 0;
    int nMax;
    int nPos = 0;
    if ( argument[0] == '\0' ) return stcf(ch,"Syntax: showkills <range/number/killed/made> \n\r< a number > IE showkills range 1/ showkills number 10.\n\r");
    argument = one_argument(argument,sCommand);
    if ( argument[0] == '\0' ) return stcf(ch,"Please put in a number, for range it will be += 10, otherwise, a number.\n\r");
    one_argument(argument,sNumber);
    nMax = atoi(sNumber);
    if ( !str_cmp(sCommand,"range") )
    {
        if ( nMax >= ( MAX_KILLLEVEL-10 ) || nMax < 10 )
            return stcf(ch,"Range can be between: 10-%d\n\r",MAX_KILLLEVEL-11);
        else
            for ( nPos = (nMax-10);nPos <= (nMax+9); nPos++ )
                stcf(ch,"#R[#yLevel:#L%-6d#R][#yMade:#L%-6d #yKilled:#L%-6d#R]\n\r",
                    nPos, kill_table[nPos].number, kill_table[nPos].killed );

    }
    else if ( !str_cmp(sCommand,"number") )
    {
        if ( nMax < 0 || nMax >= MAX_KILLLEVEL )
            return stcf(ch,"Range is 0-%d.\n\r",MAX_KILLLEVEL-1);
        else
            stcf(ch,"#R[#yLevel:#L%-6d#R][#yMade:#L%-6d #yKilled:#L%-6d#R]\n\r",
                nMax, kill_table[nMax].number, kill_table[nMax].killed );
    }
    else if ( !str_cmp(sCommand,"killed") )
    {
        nNum = 0;
        for ( nPos = 0; nPos < MAX_KILLLEVEL; nPos++ )
        {
            if ( kill_table[nPos].killed == nMax )
            {
                stcf(ch,"#R[#yLevel:#L%-6d#R][#yMade:#L%-6d #yKilled:#L%-6d#R]\n\r",
                    nPos, kill_table[nPos].number, kill_table[nPos].killed );
                nNum++;
                if ( nNum >= 100 ) break;
            }
        }
    }
    else if ( !str_cmp(sCommand,"made") )
    {
        int nNum = 0;
        for ( nPos = 0; nPos < MAX_KILLLEVEL; nPos++ )
        {
            if ( kill_table[nPos].number == nMax )
            {
                stcf(ch,"#R[#yLevel:#L%-6d#R][#yMade:#L%-6d #yKilled:#L%-6d#R]\n\r",
                    nPos, kill_table[nPos].number, kill_table[nPos].killed );
                nNum++;
                if ( nNum >= 100 ) break;
            }
        }
    }

    else
        return stcf(ch,"Follow directions.\n\r");
    return;
}

void do_questfind(CHAR_DATA *ch, char *argument)
{
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *victim;
    bool found = FALSE;
    int count = 0;
    if ( IS_NPC(ch) ) return;
    for ( victim = char_list; victim != NULL; victim = victim->next )
    {
        if (!IS_NPC(victim) || !IS_SET(victim->act, ACT_QUEST))
            continue;
        found = TRUE;
        sprintf( buf, "#y%s #Lat #R%s#n.\n\r",
            victim->name, victim->in_room->name);
        buf[0] = UPPER(buf[0]);
        send_to_char( buf, ch );
        if (count > 50 )
            break;
        else
            count++;
    }
    if ( !found )
        send_to_char( "Nothing like that in hell, earth, or heaven.\n\r", ch );
    return;
}

void do_neededhelpfiles(CHAR_DATA *ch, char *argument)
{
    HELP_DATA *pHelp;
    bool bIsHelp = TRUE;
    char *agg;
    sh_int col = 0;
    int star = 0;
    int cmd;
    int nLimit = 0;
    bool bChanged = FALSE;
    bool foundstar = FALSE;
    char buf[MAX_INPUT_LENGTH];

    if ( argument[0] == '\0' ) return stcf(ch,"Syntax: neededhelpfiles <max amount of help files>\n\r");
    nLimit = atoi(argument);
    for ( cmd = 0; cmd_table[cmd].name[0] != '\0'; cmd++ )
    {
        foundstar = TRUE;
        agg = cmd_table[cmd].name;
        if ( (pHelp = get_help( ch, agg )) == NULL )
        {
            bIsHelp = FALSE;
            star++;
            col++;
            if ( star >= nLimit ) break;
            bChanged = TRUE;
        }
        else
            bIsHelp = TRUE;
        sprintf( buf, "%-15s#n",cmd_table[cmd].name );
        if ( bIsHelp == FALSE )
            send_to_char( buf, ch );
        if ( col % 5 == 0 && bChanged == TRUE)
            send_to_char( "\n\r", ch );
        bChanged = FALSE;
    }
    if (col % 5 != 0 && foundstar)
        send_to_char("\n\r", ch);

    if (foundstar && star != 0)
    {
        sprintf(buf, "\n%d command%s found.\n\r", star,
            (star > 1) ? "s" : "");
        stc(buf, ch);
    }

    if (!foundstar)
    {
        stc("No commands found.\n\r", ch);
    }
    return;
}

void do_mobimplode(CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim;
    CHAR_DATA *temp = NULL;
    int count = 0;
    int nFound = 0;
    if ( IS_NPC(ch) ) return;
    for ( victim = char_list;  ; victim = temp )
    {
        nFound++;
        if ( victim == NULL ) break;
        temp = victim->next;
        if (!IS_NPC(victim) || victim->in_room == NULL)
            continue;
        extract_char( victim, TRUE );
        count++;
    }
    stcf(ch,"%d out of %d characters have been purged.\n\r", count, nFound);
    return;
}

void SetupAreas()
{
    int nNoAccessAreas[2] = {68,69};
    int nNum = 0;
    AREA_DATA *pArea;
    for ( nNum = 0; nNum < 2; nNum ++ )
    {
        for ( pArea = area_first; pArea; pArea = pArea->next )
            if ( pArea->vnum == nNoAccessAreas[nNum] )
                if ( !IS_SET(pArea->area_flags,AREA_NOPLAYER) )
                    SET_BIT(pArea->area_flags,AREA_NOPLAYER);
    }
}

void do_prename(CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *vict; OBJ_DATA *obj;
    char arg1[MIL];
    argument = one_argument(argument, arg1);
    if ((vict = get_char_world(ch, arg1)) == NULL || IS_NPC(vict))
    {
        send_to_char("No player by that name.\n\r", ch);
        return;
    }
    if (!*argument)
    {
        send_to_char("Rename them to what? syntax: prename originalname newname\n\r", ch);
        return;
    }
    free_string(vict->pcdata->switchname);
    free_string(vict->name);
    vict->pcdata->switchname = str_dup(argument);
    vict->name = str_dup(argument);
    for (obj = vict->carrying; obj; obj = obj->next_content)
    {
        if (!str_cmp(obj->questowner, arg1))
        {
            free_string(obj->questowner);
            obj->questowner = str_dup(argument);
        }
    }
    sprintf(log_buf, "../player/%s", arg1);
    unlink(log_buf);
    sprintf(arg1, "You have been renamed to %s.\n\r", argument);
    send_to_char(arg1, vict);
    save_char_obj(vict);
    return;
}

/*void do_reason(CHAR_DATA *ch, char *argument)
{
    char sReason[1000];
    if ( argument[0] == '\0' ) return stcf(ch,"Syntax: Reason ( Reason why you don't want to play ) .\n\r");
    strcpy(sReason,argument);
    make_note( "Immortal", ch->name, "imm", "Reason to why player didn't want to play", 5, sReason );
    stcf(ch,"Thank you very much for spending 10 seconds of your time to tell us how to better our mud. Now logging you out.\n\r");
    ch->level = 0;
    do_quit(ch,"");
}*/

double CalculateRaw(CHAR_DATA *ch, CHAR_DATA *victim)
{
    int nVictHp = victim->hit;
    int nChDamRound = ch->damcap[DAM_CAP]*number_attacks(ch,victim);
    if ( IS_NPC(victim) ) nChDamRound *= (100-victim->pIndexData->hitnodice)/(double) 100;
    if ( IS_IMMORTAL(ch) || !str_cmp(ch->name,"Pithound"))
        stcf(ch,"Victims HP: %d Damage Per Hit %d Damage Per Round %d Rounds %f.\n\r",nVictHp, ch->damcap[DAM_CAP],
            number_attacks(ch,victim), nVictHp/(double)nChDamRound);
    return nVictHp/(double)nChDamRound;
}

void do_activequest(CHAR_DATA *ch, char *argument)
{
    sh_int nQuestObjects = 0;
    char cBuffer[100];
    OBJ_DATA *obj = NULL;
    for ( obj = object_list; obj != NULL; obj = obj->next )
    {
        if ( obj != NULL && IS_SET(obj->extra_flags,ITEM_QUESTED) )
        {
            if ( nQuestObjects < 20 )
            {
                sprintf(cBuffer,"%s still needs to be found.\n\r",obj->short_descr);
                stc(cBuffer,ch);
            }
            nQuestObjects++;
        }
    }
    sprintf(cBuffer,"#GThere are %d quest objects left to find.\n\r",nQuestObjects);
    stc(cBuffer,ch);
    WAIT_STATE(ch,5);
    return;
}
