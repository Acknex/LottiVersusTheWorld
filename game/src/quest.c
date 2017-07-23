
#define textTimer skill20
#define TEXTDURATION 80

var QUEST__solved = 0;
var QUEST__started = 0;

var QUEST__count = 5;
var QUEST__id = 0;

void QUEST__masterEvent();
void QUEST__itemEvent();

#include "entity_defs.h"
#include "materials.h"
#include "itembase.h"
#include "teleporter.h"
#include "camera.h"
#include "font.h"
#include "marker.h"

SOUND* sndQuestStarted = "turret_up.wav";
SOUND* sndQuestDone = "turret_down.wav";

TEXT* txtQuestTasks = 
{
	strings = 5;
		
	string(	
	/* 0 */  "Grab a jetpack",
	/* 1 */ 	"Compile with Acknex3",
	/* 2 */  "Obtain Acknex2 sources",
	/* 3 */  "Retrieve a racecar",
	/* 4 */  "A wild Lottifant appeared"
	);
}

action questmaster()
{
	my->type = TypeQuestmaster;
	vec_scale(my->scale_x, 2.5);
	my->event = QUEST__masterEvent;
	my->material = HologramMaterial;
	my->emask |= ENABLE_TRIGGER;
	my->trigger_range = 80;
	my->z = 80;
	my->pan = -135;
	while(player == NULL)
	{
		wait(1);
	}
	while(my->textTimer < TEXTDURATION)
	{
		MARKER_update(me);
		VECTOR vecTemp;
		if (is(me, is_collected))
		{
			if (my->textTimer < TEXTDURATION)
			{
				VIEW* view = get_camera();
				my->textTimer += time_step;
				vec_set(&vecTemp, my->x);
				if ((vec_to_screen(vecTemp, get_camera()) != NULL))
				{
					STRING* str = str_create("New Mission obtained:\n--------------------\n");
					str_cat(str ,(txtQuestTasks->pstring)[QUEST__id]);
					var alpha = minv(my->textTimer * 20, 100);
					FONT_big(alpha);
					draw_text(str, vecTemp.x + 30, vecTemp.y - 70, vector(255,255,255));
					FONT_regular();
				}
			}
		}
		else
		{
			vec_set(vecTemp, player->x);
			vec_sub(vecTemp, my->x);
			ANGLE angTemp;
			vec_to_angle(angTemp, vecTemp);
			my->pan = angTemp.pan;
		}
		wait(1);
		my->animCounter = cycle(my->animCounter += 2 * time_step,0,100);
		ent_animate(me, "stand", my->animCounter, ANM_CYCLE);
	}
	while(my->alpha > 0)
	{
		MARKER_update(me);
		wait(1);
		my->alpha -= 7 * time_step;
	}
	my->alpha = 0;
	wait(1);
	ptr_remove(me);
}

action questitem()
{
	vec_scale(my->scale_x, 2.5);
	my->type = TypeQuestitem;
	my->z = 80;
	switch (QUEST__id)
	{
		case 0:
			ent_morph(me, "jetpack_lotter.mdl");
			break;
			
		case 1:
			ent_morph(me, "item_a3_collectable.mdl");
			break;
			
		case 2:
			ent_morph(me, "tronding1.mdl");
			break;
			
		case 3:
			ent_morph(me, "gocart01.mdl");
			break;
			
		case 4:
			ent_morph(me, "lottifant.mdl");
			break;
			
	}
	set(my, TRANSLUCENT);
	my->alpha = 0;
	my->material = HologramMaterial;
	my->emask |= ENABLE_TRIGGER;
	my->trigger_range = 80;
	while (QUEST__started == 0)
	{
		wait(1);
	}
	while (my->alpha < 100)
	{
		MARKER_update(me);
		wait(1);
		my->alpha += 10* time_step;
	}
	my->alpha = 100;
	reset(my, TRANSLUCENT);
	my->event = QUEST__itemEvent;
	

	//var vZ = my->z;
	var vOffset = random(500);
	while(my->textTimer < TEXTDURATION)
	{
		//my->z = vZ + 10 * sinv(total_ticks * 20 + vOffset);
		my->pan = 135 * sinv(total_ticks * 2 - vOffset);
		my->tilt = 30 * sinv(total_ticks * 10 + vOffset);
		VECTOR vecTemp;

		if (my->textTimer < TEXTDURATION && is(me, is_collected))
		{
			VIEW* view = get_camera();
			my->textTimer += time_step;
			vec_set(&vecTemp, my->x);
			if ((vec_to_screen(vecTemp, get_camera()) != NULL))
			{
				STRING* str = str_create("Mission accomplished!");
				var alpha = minv(my->textTimer * 20, 100);
				FONT_big(alpha);
				draw_text(str, vecTemp.x + 30, vecTemp.y - 70, vector(255,255,255));
				FONT_regular();
			}
		}
		else
		{
			MARKER_update(me);
		}
		wait(1);
	}


	QUEST__solved = 1;
	teleporter_enable(); //switch to end boss?
	ptr_remove(me);
	
}

/*void questspawn()
{
	you = ent_create("warlock.mdl", player.x, questmaster);
	you.x+= 100;
}
void my_startup(){on_x= questspawn;}
*/

void QUEST_init()
{
	QUEST__id = integer(random(5));
}

void QUEST_reset()
{
	QUEST__solved = 0;
	QUEST__started = 0;
}

var QUEST_isSolved()
{
	return QUEST__solved;
}

void QUEST__masterEvent()
{
	if (event_type == EVENT_TRIGGER)
	{
		ITEM_collect();
		QUEST__started = 1;
		set(my, PASSABLE);		
		snd_play(sndQuestStarted, 100, 0);
		
	}
}

void QUEST__itemEvent()
{
	if (event_type == EVENT_TRIGGER)
	{
		//ITEM_collect();
		my->event = NULL;
		set (me, is_collected);
		set(my, PASSABLE);
		snd_play(sndQuestDone, 100, 0);
		ITEM_fade();
		
	}
}
