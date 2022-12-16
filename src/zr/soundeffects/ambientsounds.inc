/*
 * ============================================================================
 *
 *  Zombie:Reloaded
 *
 *  File:          ambientsounds.inc
 *  Type:          Core 
 *  Description:   Plays ambient sounds to clients.
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
 * Global variable that tells if ambient sound cvar data was loaded successfully.
 */ 
bool g_bAmbientSounds;

/**
 * Global variable to store ambient sounds timer handle.
 */
Handle tAmbientSounds[MAXPLAYERS + 1] = {INVALID_HANDLE, ...};

/**
 * Array for flagging client to play sound.
 */
bool bAmbientSoundsIsPlaying[MAXPLAYERS + 1]; 

/**
 * Load ambient sound data.
 */
void AmbientSoundsLoad()
{
    // Validate cvars.
    g_bAmbientSounds = AmbientSoundsValidateConfig();
}

/**
 * Client is joining the server.
 * 
 * @param client    The client index.
 */
void AmbientSoundsClientInit(client)
{
    // Reset flag to play sound on client.
    bAmbientSoundsIsPlaying[client] = false;
}

/**
 * Validate ambient sounds cvars.
 */
bool AmbientSoundsValidateConfig()
{
    // If ambience is disabled, then stop.
    bool ambience = GetConVarBool(g_hCvarsList.CVAR_AMBIENTSOUNDS);
    if (!ambience)
    {
        g_bAmbientSounds = false;
        return false;
    }
    
    // Get ambient sound file.
    char sound[SOUND_MAX_PATH];
    GetConVarString(g_hCvarsList.CVAR_AMBIENTSOUNDS_FILE, sound, sizeof(sound));
    Format(sound, sizeof(sound), "sound/%s", sound);
    
    // If file doesn't exist, then log error and stop.
    if (!FileExists(sound, true))
    {
        // Log invalid sound file error.
        LogEvent(false, LogType_Error, LOG_CORE_EVENTS, LogModule_SEffects, "Config Validation", "Invalid sound file specified in \"zr_ambientsounds_file\": %s", sound);
        return false;
    }
    
    // If length is invalid, then log error and stop.
    float ambientlength = GetConVarFloat(g_hCvarsList.CVAR_AMBIENTSOUNDS_LENGTH);
    if (ambientlength <= 0.0)
    {
        // Log invalid ambient sound length error.
        LogEvent(false, LogType_Error, LOG_CORE_EVENTS, LogModule_SEffects, "Config Validation", "Ambient sound length specified in \"zr_ambientsounds_length\" is invalid.");
        
        return false;
    }
    
    // Add sound file to downloads table.
    AddFileToDownloadsTable(sound);
    
    g_bAmbientSounds = true;
    return true;
}

/**
 * Map is starting.
 */
void AmbientSoundsOnMapStart()
{
    // Reset timer handle.
    for(int i = 1; i < MaxClients; i++)
        tAmbientSounds[i] = INVALID_HANDLE;
}

/**
 * The round is starting.
 */
void AmbientSoundsOnRoundStart()
{
    // Restart ambient sound for all clients.
    for(int i = 1; i < MaxClients; i++)
    {
        if (IsClientInGame(i))
            AmbientSoundsRestart(i);
    }
}

/**
 * The round is ending.
 */
void AmbientSoundsOnRoundEnd()
{
    // x = client index
    for (int x = 1; x <= MaxClients; x++)
    {
        bAmbientSoundsIsPlaying[x] = false;
    }
}

/**
 * Delete Timer after client disconnected
 */
void AmbientSoundsOnClientDisconnect(int client)
{
    bAmbientSoundsIsPlaying[client] = false;
    delete tAmbientSounds[client];
}

/**
 * Client is spawning into the game.
 * 
 * @param client    The client index.
 */
void AmbientSoundsOnClientSpawnPost(int client)
{
    // If ambience is disabled, then stop.
    if (!g_bAmbientSounds)
    {
        return;
    }
    
    // If flag is enabled, then stop.
    if (bAmbientSoundsIsPlaying[client])
    {
        return;
    }
    
    // Get ambient sound file.
    char sound[SOUND_MAX_PATH];
    GetConVarString(g_hCvarsList.CVAR_AMBIENTSOUNDS_FILE, sound, sizeof(sound));
    
    // Emit ambient sound.
    SEffectsEmitAmbientSound(sound, client);
    
    // Flag client that sound is playing.
    bAmbientSoundsIsPlaying[client] = true;
}

/**
 * Restart ambient sound for all clients.
 */
void AmbientSoundsRestart(int client)
{
    if (!IsClientInGame(client))
        return;

    // If timer is running, kill it.
    delete tAmbientSounds[client];
    
    // If ambience is disabled, then stop.
    if (!g_bAmbientSounds)
    {
        return;
    }
    
    // Get ambient sound length.
    float ambientlength = GetConVarFloat(g_hCvarsList.CVAR_AMBIENTSOUNDS_LENGTH);
    
    // Start ambient sounds timer.
    int volume = CookiesGetInt(client, g_hAmbientCookie);

    if(volume > 0)
        tAmbientSounds[client] = CreateTimer(ambientlength, AmbientSoundsTimer, client, TIMER_FLAG_NO_MAPCHANGE);
}

/**
 * Timer callback, Replays ambient sound on all clients.
 * 
 * @param timer     The timer handle.  
*/
public Action AmbientSoundsTimer(Handle timer, any client)
{
    // If ambience is disabled, then stop.
    if (!g_bAmbientSounds)
    {
        return Plugin_Stop;
    }

    if (!IsClientInGame(client))
    {
        return Plugin_Handled;
    }
    
    // Get ambient sound file.
    char sound[SOUND_MAX_PATH];
    GetConVarString(g_hCvarsList.CVAR_AMBIENTSOUNDS_FILE, sound, sizeof(sound));
    
    // Stop sound before playing again.
    SEffectsStopAmbientSound(client, sound);
    
    // Emit ambient sound.
    SEffectsEmitAmbientSound(sound, client);
    
    // Get ambient sound length.
    float ambientlength = GetConVarFloat(g_hCvarsList.CVAR_AMBIENTSOUNDS_LENGTH);
    
    // Start new timer with sound length as delay.
    if(g_iAmbientVolume[client] > 0)
        tAmbientSounds[client] = CreateTimer(ambientlength, AmbientSoundsTimer, _, TIMER_FLAG_NO_MAPCHANGE);

    return Plugin_Continue;
}
