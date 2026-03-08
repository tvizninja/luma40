/* Copyright 2021 QMK
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#define DYNAMIC_KEYMAP_EEPROM_MAX_ADDR  1151
#define EEPROM_SIZE 1152

#define FEE_PAGE_SIZE (0x200)
#define FEE_PAGE_COUNT (8)

#define FEE_PAGE_BASE_ADDRESS (0x1F000)
#define FEE_MCU_FLASH_SIZE (0x1000)

#define EECONFIG_USER_DATA_SIZE 4

#define EECONFIG_KB_DATA_SIZE 1

#define TRANSIENT_EEPROM_SIZE 4096

// LED
#define RGB_MATRIX_DEFAULT_MODE RGB_MATRIX_SOLID_REACTIVE_CROSS
#define RGB_MATRIX_DEFAULT_VAL  16
#define RGB_MATRIX_DEFAULT_HUE  128
#define RGB_MATRIX_DEFAULT_SAT  255

// MOUSEKEY
#define MOUSEKEY_DELAY 10
#define MOUSEKEY_INTERVAL 20
#define MOUSEKEY_MOVE_DELTA 2
#define MOUSEKEY_MAX_SPEED 16
#define MOUSEKEY_TIME_TO_MAX 30
#define MOUSEKEY_WHEEL_DELAY 10
#define MOUSEKEY_WHEEL_INTERVAL 80
#define MOUSEKEY_WHEEL_DELTA 1
#define MOUSEKEY_WHEEL_MAX_SPEED 8
#define MOUSEKEY_WHEEL_TIME_TO_MAX 100

// OVERRIDE
#define DUMMY_MOD_NEUTRALIZER_KEYCODE KC_F18
#define MODS_TO_NEUTRALIZE { MOD_BIT(KC_LEFT_SHIFT) , MOD_BIT(KC_RIGHT_SHIFT) }

// MACRO
#define DYNAMIC_KEYMAP_MACRO_DELAY 8
