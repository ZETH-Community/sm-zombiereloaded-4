/*
 * ============================================================================
 *
 *  Zombie:Reloaded
 *
 *  File:          respawn.zr.inc
 *  Type:          Include
 *  Description:   Weapons-related natives/forwards.
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
 * Purchase weapon for client.
 * 
 * @param client            The client index.
 * @param weapon            Weapon name.
 * @param rebuy             Specify if this is rebuy.
 * @param free              Should this purchase be totally free.     
 * 
 * @error                   Invalid client index, not connected or invalid weapon name.
 */
native void ZR_PurchaseWeaponClient(int client, const char[] weapon, bool rebuy, bool free);

/**
 * Called right before ZR is about to give player a weapon.
 * Here you can modify any variable or stop the action entirely.
 * 
 * @param client            The client index.
 * @param weapon            Weapon name.
 * @param rebuy             Specify if this is rebuy.
 * @param free              Should this purchase be totally free.         
 *
 * @return      Plugin_Handled to block purchase.
 */
forward Action ZR_OnClientPurchaseWeapon(int &client, const char[] weapon, bool &rebuy, bool &free);

/**
 * Called right after ZR gave player a weapon.
 * 
 * @param client            The client index.
 * @param weapon            Weapon name.
 * @param rebuy             Specify if this is rebuy.
 * @param free              Should this purchase be totally free.         
 *
 */
forward void ZR_OnClientPurchasedWeapon(int client, const char[] weapon, bool rebuy, bool free);