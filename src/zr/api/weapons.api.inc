/*
 * ============================================================================
 *
 *  Zombie:Reloaded
 *
 *  File:          weapons.api.inc
 *  Type:          Core
 *  Description:   Native handlers for the ZR API. (Weapons module)
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

Handle g_hAPIFwdOnclientPurchase = INVALID_HANDLE;
Handle g_hAPIFwdOnclientPurchased = INVALID_HANDLE;

/**
 * Initializes all natives and forwards related to respawn.
 */
void APIWeaponInit()
{
    CreateNative("ZR_PurchaseWeaponClient", APIPurchasWeaponClient);

    g_hAPIFwdOnclientPurchase = CreateGlobalForward("ZR_OnClientPurchaseWeapon", ET_Hook, Param_CellByRef, Param_String, Param_CellByRef, Param_CellByRef);
    g_hAPIFwdOnclientPurchased = CreateGlobalForward("ZR_OnClientPurchasedWeapon", ET_Ignore, Param_Cell, Param_String, Param_Cell, Param_Cell);
}

public int APIPurchasWeaponClient(Handle plugin, int numParams)
{
    int client = GetNativeCell(1);

    int len;
    GetNativeStringLength(2, len);

    if(len <= 0)
        return -1;

    char[] weapon = new char[len + 1];
    GetNativeString(2, weapon, len + 1);

    bool rebuy = GetNativeCell(3);
    bool free = GetNativeCell(4);

    ZMarketEquip(client, weapon, rebuy, free);
    return 0;
}

Action APIOnclientPurchaseWeapon(int &client, const char[] weapon, bool &rebuy, bool &free)
{
    Call_StartForward(g_hAPIFwdOnclientPurchase);

    Call_PushCellRef(client);
    Call_PushString(weapon);
    Call_PushCellRef(rebuy);
    Call_PushCellRef(free);

    Action result;
    Call_Finish(result);
    return result;
}

Action APIOnclientPurchasedWeapon(int client, const char[] weapon, bool rebuy, bool free)
{
    Call_StartForward(g_hAPIFwdOnclientPurchased);

    Call_PushCell(client);
    Call_PushString(weapon);
    Call_PushCell(rebuy);
    Call_PushCell(free);

    Call_Finish();
    return Plugin_Continue;
}
