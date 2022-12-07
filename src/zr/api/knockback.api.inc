/*
 * ============================================================================
 *
 *  Zombie:Reloaded
 *
 *  File:          knockback.api.inc
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

Handle g_hAPIFwdOnclientKnockback = INVALID_HANDLE;
Handle g_hAPIFwdOnclientKnockbacked = INVALID_HANDLE;
Handle g_hAPIFwdOnclientKnockbackEx = INVALID_HANDLE;
Handle g_hAPIFwdOnclientKnockbackedEx = INVALID_HANDLE;

/**
 * Initializes all natives and forwards related to respawn.
 */
void APIKnockbackInit()
{
    g_hAPIFwdOnclientKnockback = CreateGlobalForward("ZR_OnKnockbackClient", ET_Hook, Param_CellByRef, Param_CellByRef, Param_String, Param_CellByRef, Param_CellByRef);
    g_hAPIFwdOnclientKnockbacked = CreateGlobalForward("ZR_OnKnockbackedClient", ET_Ignore, Param_Cell, Param_Cell, Param_String, Param_Cell, Param_Cell);
    g_hAPIFwdOnclientKnockbackEx = CreateGlobalForward("ZR_OnKnockbackClientEx", ET_Hook, Param_CellByRef, Param_CellByRef, Param_String, Param_CellByRef, Param_CellByRef, Param_FloatByRef, Param_FloatByRef, Param_FloatByRef, Param_FloatByRef, Param_FloatByRef);
    g_hAPIFwdOnclientKnockbackedEx = CreateGlobalForward("ZR_OnKnockbackedClientEx", ET_Ignore, Param_Cell, Param_Cell, Param_String, Param_Cell, Param_Cell, Param_Float, Param_Float, Param_Float, Param_Float, Param_Float);
}

Action APIOnKnockbackClient(int &client, int &attacker, const char[] weapon, int &hitgroup, int &dmg_health)
{
    Call_StartForward(g_hAPIFwdOnclientKnockback);

    Call_PushCellRef(client);
    Call_PushCellRef(attacker);
    Call_PushString(weapon);
    Call_PushCellRef(hitgroup);
    Call_PushCellRef(dmg_health);

    Action result;
    Call_Finish(result);
    return result;
}

Action APIOnKnockbackedClient(int client, int attacker, const char[] weapon, int hitgroup, int dmg_health)
{
    Call_StartForward(g_hAPIFwdOnclientKnockbacked);

    Call_PushCell(client);
    Call_PushCell(attacker);
    Call_PushString(weapon);
    Call_PushCell(hitgroup);
    Call_PushCell(dmg_health);

    Call_Finish();
    return Plugin_Continue;
}

Action APIOnKnockbackClientEx(int &client, int &attacker, const char[] weapon, int &hitgroup, int &dmg_health, float &knockback, float &classkb, float &weaponkb, float &hitgroupkb, float &dmg_kb)
{
    Call_StartForward(g_hAPIFwdOnclientKnockbackEx);

    Call_PushCellRef(client);
    Call_PushCellRef(attacker);
    Call_PushString(weapon);
    Call_PushCellRef(hitgroup);
    Call_PushCellRef(dmg_health);
    Call_PushFloatRef(knockback);
    Call_PushFloatRef(classkb);
    Call_PushFloatRef(weaponkb);
    Call_PushFloatRef(hitgroupkb);
    Call_PushFloatRef(dmg_kb);

    Action result;
    Call_Finish(result);
    return result;
}

Action APIOnKnockbackedClientEx(int client, int attacker, const char[] weapon, int hitgroup, int dmg_health, float knockback, float classkb, float weaponkb, float hitgroupkb, float dmg_kb)
{
    Call_StartForward(g_hAPIFwdOnclientKnockbackedEx);

    Call_PushCellRef(client);
    Call_PushCellRef(attacker);
    Call_PushString(weapon);
    Call_PushCellRef(hitgroup);
    Call_PushCellRef(dmg_health);
    Call_PushFloatRef(knockback);
    Call_PushFloatRef(classkb);
    Call_PushFloatRef(weaponkb);
    Call_PushFloatRef(hitgroupkb);
    Call_PushFloatRef(dmg_kb);

    Call_Finish();
    return Plugin_Continue;
}