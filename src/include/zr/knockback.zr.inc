/*
 * ============================================================================
 *
 *  Zombie:Reloaded
 *
 *  File:          knockback.zr.inc
 *  Type:          Include
 *  Description:   Knockback-related natives/forwards.
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
 * Called right before player getting Knockback from damaging.
 * Here you can modify any variable or stop the action entirely.
 * 
 * @param client            The client index.
 * @param attacker          The attacker index.
 * @param weapon            Weapon entity name.
 * @param hitgroup          Hitbox that client get hit.
 * @param dmg_health        Total Damage amount.         
 *
 * @return      Plugin_Handled to block knockback.
 */
forward Action ZR_OnKnockbackClient(int &client, int &attacker, const char[] weapon, int &hitgroup, int &dmg_health);

/**
 * Called after player getting Knockback from damaging.
 * 
 * @param client            The client index.
 * @param attacker          The attacker index.
 * @param weapon            Weapon entity name.
 * @param hitgroup          Hitbox that client get hit.
 * @param dmg_health        Total Damage amount.         
 *
 */
forward void ZR_OnKnockbackedClient(int client, int attacker, const char[] weapon, int hitgroup, int dmg_health);

/**
 * Called right before set Knockback function to player.
 * Here you can modify any variable or stop the action entirely.
 * 
 * @param client            The client index.
 * @param attacker          The attacker index.
 * @param weapon            Weapon entity name.
 * @param hitgroup          Hitbox that client get hit.
 * @param dmg_health        Total Damage amount.        
 * @param knockback         Total Knockback value.
 * @param classkb           Knockback multipler based on client class.
 * @param weaponkb          Knockback multipler based on attacker weapon.
 * @param hitgroup          Knockback multipler based on hitgroup.
 * @param dmg_kb            Knockback multipler based on damage value. 
 *
 * @return      Plugin_Handled to block knockback.
 */
forward Action ZR_OnKnockbackClientEx(int &client, int &attacker, const char[] weapon, int &hitgroup, int &dmg_health, float &knockback, float &classkb, float &weaponkb, float &hitgroupkb, float &dmg_kb);

/**
 * Called after set Knockback function to player.
 * 
 * @param client            The client index.
 * @param attacker          The attacker index.
 * @param weapon            Weapon entity name.
 * @param hitgroup          Hitbox that client get hit.
 * @param dmg_health        Total Damage amount.        
 * @param knockback         Total Knockback value.
 * @param classkb           Knockback multipler based on client class.
 * @param weaponkb          Knockback multipler based on attacker weapon.
 * @param hitgroup          Knockback multipler based on hitgroup.
 * @param dmg_kb            Knockback multipler based on damage value. 
 *
 */
forward void ZR_OnKnockbackedClientEx(int client, int attacker, const char[] weapon, int hitgroup, int dmg_health, float knockback, float classkb, float weaponkb, float hitgroupkb, float dmg_kb);

