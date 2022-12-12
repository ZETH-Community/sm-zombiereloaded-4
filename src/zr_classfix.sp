#include <sourcemod>
#include <sdktools>
#include <sdkhooks>
#include <cstrike>
#include <zombiereloaded>

ConVar g_Cvar_HumanClass;
ConVar g_Cvar_ZombieClass;

public Plugin myinfo =
{
    name = "ZR Class-Fix",
    author = "Oylsister",
    description = "Much more configure for fixing zombie having human class attribute ",
    version = "1.0",
    url = ""
};

public void OnPluginStart()
{
    HookEvent("player_spawn", OnPlayerSpawn);

    g_Cvar_HumanClass = CreateConVar("zr_human_classname", "Normal Human", "Human Class to detect");
    g_Cvar_ZombieClass = CreateConVar("zr_zombie_classname", "Classic", "Zombie Class to apply");

    AutoExecConfig(true, "zr_classfix", "sourcemod/zombiereloaded");
}

public void OnPlayerSpawn(Event event, const char[] name, bool dontBroadcast)
{
    char humanclassname[64];
    char zombieclassname[64];

    int client = GetClientOfUserId(event.GetInt("userid"));
    int clientclass = ZR_GetZombieClass(client);

    g_Cvar_HumanClass.GetString(humanclassname, 64);
    g_Cvar_ZombieClass.GetString(zombieclassname, 64);

    int humanclass = ZR_GetClassByName(humanclassname);
    int zombieclass = ZR_GetClassByName(zombieclassname);

    if(humanclass == clientclass)
    {
        ZR_SelectClientClass(client, zombieclass, true, true);
    }

    return;
}

public void ZR_OnClientInfected(int client, int attacker, bool motherinfect, bool override, bool respawn)
{
    char humanclassname[64];
    char zombieclassname[64];

    int clientclass = ZR_GetZombieClass(client);

    g_Cvar_HumanClass.GetString(humanclassname, 64);
    g_Cvar_ZombieClass.GetString(zombieclassname, 64);

    int humanclass = ZR_GetClassByName(humanclassname);
    int zombieclass = ZR_GetClassByName(zombieclassname);

    if(humanclass == clientclass)
    {
        ZR_SelectClientClass(client, zombieclass, true, true);
    }

    return;
}
