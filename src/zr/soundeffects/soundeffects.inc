/*
 * ============================================================================
 *
 *  Zombie:Reloaded
 *
 *  File:          soundeffects.inc
 *  Type:          Core 
 *  Description:   Basic sound-management API.
 *
 *  Copyright (C) 2009-2013  Greyscale, Richard Helgeby
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * ============================================================================
 */

/**
 * Maximum sound path length.
 */
#define SOUND_MAX_PATH 128

/**
 * Ambient sound channel.
 */
#define SOUND_AMBIENT_CHANNEL 8

/**
 * Sound Volume Cookie
 */
Handle g_hSEffectsCookie = INVALID_HANDLE;
Handle g_hAmbientCookie = INVALID_HANDLE;
 
int g_iSEffectsVolume[MAXPLAYERS+1];
int g_iAmbientVolume[MAXPLAYERS+1];
int g_iCurrentType[MAXPLAYERS+1];

/**
 * Type for Sound
 */
#define SOUND_TYPE_ZOMBIE 0
#define SOUND_TYPE_AMBIENT 1

#include "zr/soundeffects/voice"
#include "zr/soundeffects/ambientsounds"
#include "zr/soundeffects/zombiesounds"

/**
 * Load sound effects data.
 */
void SEffectsLoad()
{
    // Load ambient sound cvars.
    AmbientSoundsLoad();
}

/**
 * Map is starting.
 */ 
void SEffectsOnMapStart()
{
    // Forward event to sub-modules.
    AmbientSoundsOnMapStart();
}

/**
 * Create Command for ZVolume.
 */ 
void SEffectsOnCommandCreate()
{
    RegConsoleCmd(SAYHOOKS_KEYWORD_ZVOLUME, ZVolumeCommand);
    RegConsoleCmd("sm_zombie_volume", ZombieVoiceLevel);
    RegConsoleCmd("sm_ambient_volume", AmbeintLevel);
}

/**
 * Client is joining the server.
 * 
 * @param client    The client index.
 */
void SEffectsClientInit(int client)
{
    // Forward event to sub-modules.
    AmbientSoundsClientInit(client);
    ZombieSoundsClientInit(client);
}

/**
 * Cookie for sound settings.
 */
void SEffectsOnCookiesCreate()
{
    if(g_hSEffectsCookie == INVALID_HANDLE)
    {
        g_hSEffectsCookie = RegClientCookie("zr_zvolume_zombie", "The Zombie Volume adjust option", CookieAccess_Protected);
    }

    if(g_hAmbientCookie == INVALID_HANDLE)
    {
        g_hAmbientCookie = RegClientCookie("zr_zvolume_ambient", "The Ambient Volume adjust option", CookieAccess_Protected);
    }
}

/**
 * Client's Cookie for sound settings.
 * 
 * @param client    The client index.
 */
void SEffectsOnClientCookiesCached(int client)
{
    char sBuffer[8];
    GetClientCookie(client, g_hSEffectsCookie, sBuffer, sizeof(sBuffer));

    if(!sBuffer[0])
    {
        CookiesSetInt(client, g_hSEffectsCookie, 100);
    }

    g_iSEffectsVolume[client] = CookiesGetInt(client, g_hSEffectsCookie);

    char sAmbient[8]
    GetClientCookie(client, g_hAmbientCookie, sAmbient, sizeof(sAmbient));

    if(!sAmbient[0])
    {
        CookiesSetInt(client, g_hAmbientCookie, 100);
    }

    g_iAmbientVolume[client] = CookiesGetInt(client, g_hAmbientCookie);
}

/**
 * The round is starting.
 */
void SEffectsOnRoundStart()
{
    // Forward event to sub-modules.
    VoiceOnRoundStart();
    AmbientSoundsOnRoundStart();
}

/**
 * The round is ending.
 */
void SEffectsOnRoundEnd()
{
    // Forward event to sub-modules.
    VoiceOnRoundEnd();
    AmbientSoundsOnRoundEnd();
    ZombieSoundsOnRoundEnd();
}

/**
 * Client is spawning into the game.
 * 
 * @param client    The client index.
 */
void SEffectsOnClientSpawn(int client)
{
    // Forward event to sub-modules.
    VoiceOnClientSpawn(client);
    ZombieSoundsOnClientSpawn(client);
}

/**
 * Client is spawning into the game. *Post
 * 
 * @param client    The client index.
 */
void SEffectsOnClientSpawnPost(int client)
{
    // Forward event to sub-modules.
    AmbientSoundsOnClientSpawnPost(client);
}

/**
 * Client has been killed.
 * 
 * @param client    The client index.
 */
void SEffectsOnClientDeath(int client)
{
    // Forward event to sub-modules.
    ZombieSoundsOnClientDeath(client);
}

/**
 * Client has been hurt.
 * 
 * @param client    The client index.
 */
void SEffectsOnClientHurt(int client)
{
    // Forward event to sub-modules.
    ZombieSoundsOnClientHurt(client);
}

/**
 * Client has been infected.
 * 
 * @param client    The client index.
 */
void SEffectsOnClientInfected(int client)
{
    // Forward event to sub-modules.
    VoiceOnClientInfected(client);
    ZombieSoundsOnClientInfected(client);
}

/**
 * Client has been turned back human.
 * 
 * @param client    The client index.
 */
void SEffectsOnClientHuman(int client)
{
    // Forward event to sub-modules.
    VoiceOnClientHuman(client);
}

/**
 * Emits an ambient sound
 * 
 * @param sound         The path to the sound file (relative to sounds/)
 * @param soundvolume   The volume of the sound (0.0 - 1.0)
 * @param client        (Optional) Client index to play sound to.
 */
void SEffectsEmitAmbientSound(const char[] sound, int client)
{
    // Precache sound before playing.
    PrecacheSound(sound);
    
    if (IsClientInGame(client))
    {
        // Emit ambient sound.

        if(g_iAmbientVolume[client] > 0)
            EmitSoundToClient(client, sound, SOUND_FROM_PLAYER, SOUND_AMBIENT_CHANNEL, _, _, float(g_iAmbientVolume[client] / 100));
        
        // Flag client that sound is playing.
        bAmbientSoundsIsPlaying[client] = true;
    }
}

/**
 * Stop an ambient sound
 *  
 * @param sound     The path to the sound file (relative to sounds/) 
 */
void SEffectsStopAmbientSound(int client, const char[] sound)
{
    if (!IsClientInGame(client))
    {
        return;
    }
        
    // Stop ambient sound.
    StopSound(client, SOUND_AMBIENT_CHANNEL, sound);
}

/**
 * Replay an ambient sound
 * 
 * @param sound     The path to the sound file (relative to sounds/)
 */ 

/**
 * Emits a sound from a client.
 * 
 * @param client    The client index.
 * @param sound     The sound file relative to the sound/ directory.
 * @param level     The attenuation of the sound.
 */
void SEffectsEmitSoundFromClient(int client, const char[] sound, int level = SNDLEVEL_NORMAL)
{
    // Precache sound before playing.
    PrecacheSound(sound);
    
    // Emit sound from client.
    for(int i = 1; i < MaxClients; i++)
    {
        float volume;

        if(IsClientInGame(i) && g_iSEffectsVolume[i] > 0)
        {
            volume = float(g_iSEffectsVolume[i] / 100);
            EmitSoundToClient(i, sound, client, _, level, _, volume);
        }
    }
}

/**
 * Command callback (zhp)
 * Setup ZR volume.
 * 
 * @param client    The client index.
 * @param argc      Argument count.
 */
public Action ZVolumeCommand(int client, int argc)
{
    // If client is console, then stop and tell them this feature is for players only.
    if (ZRIsConsole(client))
    {
        TranslationPrintToServer("Must be player");
        return Plugin_Handled;
    }

    // ZVolume Menu
    ZVolumeMenu(client);

    // This stops the "Unknown command" message in client's console.
    return Plugin_Handled;
}

void ZVolumeMenu(int client)
{
    Handle menu = CreateMenu(ZVloumeMenuHandle, MENU_ACTIONS_ALL);

    SetGlobalTransTarget(client);

    char title[MENU_LINE_TITLE_LENGTH];
    char zombievolume[MENU_LINE_REG_LENGTH];
    char ambientvolume[MENU_LINE_REG_LENGTH];

    Format(title, MENU_LINE_TITLE_LENGTH, "%t", "ZVolume Title");
    Format(zombievolume, MENU_LINE_REG_LENGTH, "%t", "Zombie Volume", g_iSEffectsVolume[client]);
    Format(ambientvolume, MENU_LINE_REG_LENGTH, "%t", "Ambient Volume", g_iAmbientVolume[client]);

    SetMenuTitle(menu, title);
    AddMenuItem(menu, "Zombie_Volume", zombievolume);
    AddMenuItem(menu, "Ambient_Volume", ambientvolume);

    SetMenuExitButton(menu, true);
    DisplayMenu(menu, client, MENU_TIME_FOREVER);
}

public int ZVloumeMenuHandle(Handle menu, MenuAction action, int client, int slot)
{
    switch (action)
    {
        case MenuAction_Select:
        {
            ZVolumeSelectLevel(client, slot);
        }
        case MenuAction_End:
        {
            CloseHandle(menu);
        }
    }
    return 0;
}

void ZVolumeSelectLevel(int client, int type)
{
    g_iCurrentType[client] = type;

    Handle menu = CreateMenu(ZVloumeSelectLevelHandle, MENU_ACTIONS_ALL);

    SetGlobalTransTarget(client);

    char title[MENU_LINE_TITLE_LENGTH];

    Format(title, MENU_LINE_TITLE_LENGTH, "%t", "Change Volume");

    SetMenuTitle(menu, title);
    AddMenuItem(menu, "100", "100%");
    AddMenuItem(menu, "80", "80%");
    AddMenuItem(menu, "60", "60%");
    AddMenuItem(menu, "40", "40%");
    AddMenuItem(menu, "20", "20%");
    AddMenuItem(menu, "0", "0%");

    SetMenuExitButton(menu, true);
    DisplayMenu(menu, client, MENU_TIME_FOREVER);
}

public int ZVloumeSelectLevelHandle(Handle menu, MenuAction action, int client, int slot)
{
    switch (action)
    {
        case MenuAction_Select:
        {
            char info[16];
            GetMenuItem(menu, slot, info, sizeof(info));

            int volume = StringToInt(info);
            ChangeSoundVolume(client, g_iCurrentType[client], volume);
            ZVolumeMenu(client);
        }
        case MenuAction_End:
        {
            CloneHandle(menu);
        }
        case MenuAction_Cancel:
        {
            if(slot == MenuCancel_ExitBack)
            {
                ZVolumeMenu(client);
            }
        }
    }
    return 0;
}

public Action ZombieVoiceLevel(int client, int args)
{
    char sArgs[16];
    GetCmdArg(1, sArgs, sizeof(sArgs));

    int volume = StringToInt(sArgs);
    ChangeSoundVolume(client, SOUND_TYPE_ZOMBIE, volume);
    return Plugin_Handled;
}

public Action AmbeintLevel(int client, int args)
{
    char sArgs[16];
    GetCmdArg(1, sArgs, sizeof(sArgs));

    int volume = StringToInt(sArgs);
    ChangeSoundVolume(client, SOUND_TYPE_AMBIENT, volume);
    return Plugin_Handled;
}

void ChangeSoundVolume(int client, int type, int volume)
{
    if(volume > 100)
        volume = 100;

    if(volume < 0)
        volume = 0;

    if(type == SOUND_TYPE_ZOMBIE)
    {
        g_iSEffectsVolume[client] = volume;
        TranslationPrintToChat(client, "Zombie Voice Changed", g_iSEffectsVolume[client]);
    }

    else
    {
        g_iAmbientVolume[client] = volume;
        TranslationPrintToChat(client, "Ambient Changed", g_iAmbientVolume[client]);

        char sound[SOUND_MAX_PATH];
        GetConVarString(g_hCvarsList.CVAR_AMBIENTSOUNDS_FILE, sound, sizeof(sound));

        if(volume <= 0)
        {
            SEffectsStopAmbientSound(client, sound);
        }
        else
        {
            AmbientSoundsRestart(client);
        }
    }
}