#ifndef HUD_H
#define HUD_H

var vEmoHealthX = 0;
var vPlayerHealth = 0;
var vPlayerOldHealth = 0;
var vBombCooldown = 103;
var vDummy = 0;

FONT* fontHud = NULL;
FONT* fontDialog = NULL;

BMAP* bmapEmoHealth = "emohealthsmall.tga";
BMAP* bmapHealthHud = "healthHud.tga";
BMAP* bmapBombSlots = "bombSlots.tga";
BMAP* bmapDialog = "dialog.tga";
PANEL* panEmoHealth = NULL;
PANEL* panDialog = NULL;

void hud_ingame_init();
void hud_ingame_align();
void hud_ingame_update();

#include "hud.c"

#endif