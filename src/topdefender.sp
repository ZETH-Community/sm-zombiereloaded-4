#pragma semicolon 1

#include <sourcemod>
#include <sdktools>
#include <sdkhooks>
#include <cstrike>
#include <zombiereloaded>

#pragma newdecls required

#define MAXTOPTHREE 3

int g_iDamaged[MAXPLAYERS+1] = {0, ...};
int g_iInfected[MAXPLAYERS+1] = {0, ...};

int g_iSortDamage[MAXPLAYERS+1][2];
int g_iSortInfect[MAXPLAYERS+1][2];

int g_iSortCount;

int g_TopDefender[MAXTOPTHREE] = {-1, ...};
int g_TopInfection[MAXTOPTHREE] = {-1, ...};

public Plugin myinfo =
{
    name = "Top Defender and Top Infector",
    author = "Oylsister",
    description = "Showing player who deal the most damage to zombie and player who infect most of the player in round.",
    version = "1.0",
    url = ""
};

public void OnPluginStart()
{
    HookEvent("player_hurt", OnPlayerHurt);
    HookEvent("round_start", OnRoundStart);
    HookEvent("round_end", OnRoundEnd);
}

public APLRes AskPluginLoad2(Handle myself, bool late, char[] error, int err_max)
{
    CreateNative("TDNTI_GetClientTopDefenderRank", Native_GetClientTopDefenderRank);
    CreateNative("TDNTI_GetClientTopInfectionRank", Native_GetClientTopInfectionRank);

    return APLRes_Success;
}

public void OnClientPutInServer(int client)
{
    g_iInfected[client] = 0;
    g_iDamaged[client] = 0;
}

public void OnClientDisconnect(int client)
{
    int clientrank[2];

    clientrank[0] = GetClientTopDefenderRank(client);
    clientrank[1] = GetClientTopInfectionRank(client);

    if(clientrank[0] != -1)
        g_TopDefender[clientrank[0]] = -1;

    if(clientrank[1] != -1)
        g_TopInfection[clientrank[1]] = -1;

    g_iInfected[client] = 0;
    g_iDamaged[client] = 0;
}

public void OnRoundStart(Event event, const char[] name, bool dontBroadcast)
{
    for(int i = 1; i <= MaxClients; i++)
    {
        if(IsClientInGame(i))
        {
            g_iDamaged[i] = 0;
            g_iInfected[i] = 0;
        }
    }
}

public void OnRoundEnd(Event event, const char[] name, bool dontBroadcast)
{
    SortDefender();
    SortInfecter();
    PrintTopDefender();
    PrintTopInfection();
}

public void OnPlayerHurt(Event event, const char[] name, bool dontBroadcast)
{
    int client = GetClientOfUserId(event.GetInt("userid"));
    int attacker = GetClientOfUserId(event.GetInt("attacker"));
    int dmg = event.GetInt("dmg_health");

    if(ZR_IsClientHuman(attacker) && ZR_IsClientZombie(client))
        g_iDamaged[attacker] += dmg;
}

public void ZR_OnClientInfected(int client, int attacker, bool motherinfect, bool override, bool respawn)
{
    if(motherinfect || attacker == -1)
        return;

    if(ZR_IsClientZombie(attacker) && ZR_IsClientHuman(client))
        g_iInfected[attacker]++;
}

void SortDefender()
{
    if(IsCSGOWarmUp())
        return;

    for(int i = 0; i < sizeof(g_iSortDamage); i++)
    {
        g_iSortDamage[i][0] = -1;
        g_iSortDamage[i][1] = 0;
    }

    g_iSortCount = 0;

    for(int i = 1; i <= MaxClients; i++)
    {
        if(!IsClientInGame(i))
            continue;
            
        g_iSortDamage[g_iSortCount][0] = i;
        g_iSortDamage[g_iSortCount][1] = g_iDamaged[i];
        g_iSortCount++;
    }

    SortCustom2D(g_iSortDamage, g_iSortCount, SortAnyList);
}

void PrintTopDefender()
{
    PrintToChatAll("+++ Top 3 Defending +++");
    for(int i = 0; i < MAXTOPTHREE; i++)
    {
        PrintToChatAll("%d. %N - %d Damage", i, g_iSortDamage[i][0], g_iSortDamage[i][1]);
        g_TopDefender[i] = i;
    }
}

void SortInfecter()
{
    if(IsCSGOWarmUp())
        return;

    for(int i = 0; i < sizeof(g_iSortInfect); i++)
    {
        g_iSortInfect[i][0] = -1;
        g_iSortInfect[i][1] = 0;
    }

    g_iSortCount = 0;

    for(int i = 1; i <= MaxClients; i++)
    {
        if(!IsClientInGame(i))
            continue;
            
        g_iSortInfect[g_iSortCount][0] = i;
        g_iSortInfect[g_iSortCount][1] = g_iInfected[i];
        g_iSortCount++;
    }

    SortCustom2D(g_iSortDamage, g_iSortCount, SortAnyList);
}

void PrintTopInfection()
{
    PrintToChatAll("+++ Top 3 Infection +++");
    for(int i = 0; i < MAXTOPTHREE; i++)
    {
        PrintToChatAll("%d. %N - %d Infected", i, g_iSortInfect[i][0], g_iSortInfect[i][1]);
        g_TopInfection[i] = i;
    }
}

bool IsCSGOWarmUp()
{
    if(GameRules_GetProp("m_bWarmupPeriod") == 1)
        return true;

    return false;
}

public int SortAnyList(int[] elem1, int[] elem2, const int[][] array, Handle hndl)
{
	if (elem1[1] > elem2[1]) return -1;
	if (elem1[1] < elem2[1]) return 1;

	return 0;
}

// Native
public int Native_GetClientTopDefenderRank(Handle plugin, int param)
{
    return GetClientTopDefenderRank(GetNativeCell(1));
}

public int Native_GetClientTopInfectionRank(Handle plugin, int param)
{
    return GetClientTopInfectionRank(GetNativeCell(1));
}

int GetClientTopDefenderRank(int client)
{
    for(int i = 0; i < MAXTOPTHREE; i++)
    {
        if(g_TopDefender[i] == client)
            return i;
    }

    return -1;
}

int GetClientTopInfectionRank(int client)
{
    for(int i = 0; i < MAXTOPTHREE; i++)
    {
        if(g_TopInfection[i] == client)
            return i;
    }

    return -1;
}