/**
 * ====================
 *   Zombie:Reloaded
 *   File: weapons.inc
 *   Author: Greyscale
 * ==================== 
 */

/**
 * Maximum length of a weapon name string
 */
#define WEAPONS_MAX_LENGTH 32

/**
 * @endsection
 */
  
/**
 * Array to store keyvalue data.
 */
new Handle:kvWeapons = INVALID_HANDLE;

#include "zr/weapons/restrict"
#include "zr/weapons/markethandler"
#include "zr/weapons/menu_weapons"

WeaponsInit()
{
    // Forward event to sub-module
    RestrictInit();
}

/**
 * Loads weapon data from file.
 */
WeaponsOnMapStart()
{
    // Load weapon data
    if (kvWeapons != INVALID_HANDLE)
    {
        CloseHandle(kvWeapons);
    }
    
    kvWeapons = CreateKeyValues("weapons");
    
    decl String:path[PLATFORM_MAX_PATH];
    BuildPath(Path_SM, path, sizeof(path), "configs/zr/weapons/weapons.txt");
    
    // If file isn't found, stop plugin
    if (!FileToKeyValues(kvWeapons, path))
    {
        SetFailState("\"%s\" missing from server", path);
    }
    
    // Validate weapons config
    WeaponsValidateWeaponsConfig();
    
    // Forward event to sub-module
    RestrictOnMapStart();
}

WeaponsValidateWeaponsConfig()
{
    KvRewind(kvWeapons);
    if (KvGotoFirstSubKey(kvWeapons))
    {
        decl String:weapon[WEAPONS_MAX_LENGTH];
        decl String:restrict[8];
        decl String:menu[8];
        
        do
        {
            KvGetSectionName(kvWeapons, weapon, sizeof(weapon));
            KvGetString(kvWeapons, "restrict", restrict, sizeof(restrict), "no");
            KvGetString(kvWeapons, "menu", menu, sizeof(menu), "yes");
            
            // VALIDATE
            
        } while (KvGotoNextKey(kvWeapons));
    }
}

WeaponsClientInit(client)
{
    // Forward event to sub-module
    RestrictClientInit(client);
}

WeaponsOnClientDisconnect(client)
{
    // Forward event to sub-module
    RestrictOnClientDisconnect(client);
}

/**
 * Creates an array of all listed weapons in weapons.txt.
 * @param arrayWeapons  The handle of the array, don't forget to call CloseHandle
 *                      on it when finished!
 * @return              The size of the array.
 */
WeaponsCreateWeaponArray(&Handle:arrayWeapons, maxlen = WEAPONS_MAX_LENGTH)
{
    arrayWeapons = CreateArray(maxlen);
    new count = 0;
    
    KvRewind(kvWeapons);
    if (KvGotoFirstSubKey(kvWeapons))
    {
        decl String:weapon[maxlen];
        
        do
        {
            KvGetSectionName(kvWeapons, weapon, maxlen);
            
            PushArrayString(arrayWeapons, weapon);
            count++;
        } while (KvGotoNextKey(kvWeapons));
    }
    
    return count;
}

/**
 * Checks if a weapon is valid (aka listed in weapons.txt)
 * @param weapon    The weapon name.
 * @return          Returns true if valid, false it not.
 */
bool:WeaponsIsValidWeapon(const String:weapon[])
{
    KvRewind(kvWeapons);
    if (KvGotoFirstSubKey(kvWeapons))
    {
        decl String:validweapon[WEAPONS_MAX_LENGTH];
        
        do
        {
            KvGetSectionName(kvWeapons, validweapon, sizeof(validweapon));
            
            if (StrEqual(validweapon, weapon, false))
            {
                return true;
            }
            
        } while (KvGotoNextKey(kvWeapons));
    }
    
    return false;
}

/**
 * Looks up a weapon in weapons.txt and returns exact display name
 * @param weapon    The weapon name.
 * @param display   Returns with the display name, is not changed if weapon is invalid. 
 */
WeaponGetDisplayName(const String:weapon[], String:display[])
{
    KvRewind(kvWeapons);
    if (KvGotoFirstSubKey(kvWeapons))
    {
        decl String:validweapon[WEAPONS_MAX_LENGTH];
        
        do
        {
            KvGetSectionName(kvWeapons, validweapon, sizeof(validweapon));
            
            if (StrEqual(validweapon, weapon, false))
            {
                strcopy(display, WEAPONS_MAX_LENGTH, validweapon);
            }
            
        } while (KvGotoNextKey(kvWeapons));
    }
}
    
/**
 * Checks if a weapon is restricted by default.
 * @param weapon    The weapon name.
 * @return          Returns true if restricted, false it not.
 */
bool:WeaponsIsRestrict(const String:weapon[])
{
    KvRewind(kvWeapons);
    if (KvGotoFirstSubKey(kvWeapons))
    {
        decl String:validweapon[WEAPONS_MAX_LENGTH];
        decl String:restrict[8];
        
        do
        {
            KvGetSectionName(kvWeapons, validweapon, sizeof(validweapon));
            
            if (StrEqual(validweapon, weapon, false))
            {
                KvGetString(kvWeapons, validweapon, restrict, sizeof(restrict), "no");
                
                return ConfigOptionToBool(restrict);
            }
        } while (KvGotoNextKey(kvWeapons));
    }
    
    return false;
}

/**
 * Checks if a weapon restriction can be toggled by the admin menu.
 * @param weapon    The weapon name.
 * @return          Returns true if restricted, false it not.
 */
bool:WeaponsIsWeaponMenu(const String:weapon[])
{
    KvRewind(kvWeapons);
    if (KvGotoFirstSubKey(kvWeapons))
    {
        decl String:validweapon[WEAPONS_MAX_LENGTH];
        decl String:menu[8];
        
        do
        {
            KvGetSectionName(kvWeapons, validweapon, sizeof(validweapon));
            
            if (StrEqual(validweapon, weapon, false))
            {
                KvGetString(kvWeapons, validweapon, menu, sizeof(menu), "yes");
                
                return ConfigOptionToBool(menu);
            }
        } while (KvGotoNextKey(kvWeapons));
    }
    
    return false;
}