/* Copyright 2025 Epomaker
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

#include QMK_KEYBOARD_H
#include "dynamic_keymap.h"
#include "rdmctmzt_common.h"

// --- struct and define ---
enum custom_keycodes {
    JE_TOG = SAFE_RANGE, JE_ON, JE_OF,
    JE_2, JE_6, JE_7, JE_8, JE_9, JE_0,
    JE_MINS, JE_EQL, JE_LBRC, JE_RBRC,
    JE_BSLS, JE_SCLN, JE_QUOT, JE_GRV,
    MCR_00, MCR_01, MCR_02, MCR_03, MCR_04, MCR_05, MCR_06, MCR_07,
    MCR_08, MCR_09, MCR_10, MCR_11, MCR_12, MCR_13, MCR_14, MCR_15,
};

#define MCR(idx) (MCR_00 + idx)
static bool is_jp_mode = true;

// --- tapdance ---
typedef enum { AT_KEY, AT_MO, AT_MACRO } act_type_t;

typedef struct {
    uint16_t kc;
    bool s;
} jis_t;

typedef struct {
    act_type_t type;
    uint16_t val;
} td_action_t;

typedef struct {
    td_action_t t1, h1, t2, h2;
    td_action_t active;
    uint16_t    sent_kc;
    uint8_t     sent_mods;
} td_quad_ctx_t;

typedef enum { TD_NONE, TD_T1, TD_H1, TD_T2, TD_H2 } td_state_t;

td_state_t get_td_state(tap_dance_state_t *state) {
    if (state->count == 1) {
        if (!state->pressed) return TD_T1;
        return TD_H1;
    }
    if (state->count == 2) {
        if (!state->pressed) return TD_T2;
        return TD_H2;
    }
    return TD_NONE;
}

void perform_quad_act(td_quad_ctx_t *ctx, bool pressed) {
    td_action_t act = ctx->active;
    if (act.val == KC_NO) return;
    if (pressed) {
        if (act.type == AT_KEY) {
            uint8_t mods = get_mods();
            jis_t out = {KC_NO, false};
            if (out.kc != KC_NO) {
                ctx->sent_kc = out.kc;
                ctx->sent_mods = mods;
                del_mods(MOD_MASK_SHIFT);
                send_keyboard_report();
                if (out.s) add_mods(MOD_BIT_LSHIFT);
                register_code(out.kc);
                if (out.s) del_mods(MOD_BIT_LSHIFT);
            } else {
                ctx->sent_kc = act.val;
                ctx->sent_mods = mods;
                register_code16(act.val);
            }
        } else if (act.type == AT_MO) {
            layer_on(act.val);
        } else if (act.type == AT_MACRO) {
            dynamic_keymap_macro_send(act.val);
        }
    } else {
        if (act.type == AT_KEY && ctx->sent_kc != KC_NO) {
            unregister_code(ctx->sent_kc & 0xFF);
            set_mods(ctx->sent_mods);
            send_keyboard_report();
            ctx->sent_kc = KC_NO;
        } else if (act.type == AT_MO) {
            layer_off(act.val);
        }
    }
}

void generic_quad_finished(tap_dance_state_t *state, void *user_data) {
    td_quad_ctx_t *ctx = (td_quad_ctx_t *)user_data;
    td_state_t s = get_td_state(state);
    if      (s == TD_T1) ctx->active = ctx->t1;
    else if (s == TD_H1) ctx->active = ctx->h1;
    else if (s == TD_T2) ctx->active = ctx->t2;
    else if (s == TD_H2) ctx->active = ctx->h2;
    perform_quad_act(ctx, true);
}

void generic_quad_reset(tap_dance_state_t *state, void *user_data) {
    td_quad_ctx_t *ctx = (td_quad_ctx_t *)user_data;
    perform_quad_act(ctx, false);
    ctx->active = (td_action_t){AT_KEY, KC_NO};
}

#define K(kc)  AT_KEY, kc
#define L(lyr) AT_MO, lyr
#define M(idx) AT_MACRO, idx
#define XX     AT_KEY, KC_NO

// id, tap, hold, dtap, dhold
#define TD_LIST \
    X( 0,  K(KC_F1),    K(KC_F1),    M(1),      M(1)   ) \
    X( 1,  K(KC_F2),    K(KC_F2),    M(2),      M(2)   ) \
    X( 2,  K(KC_F3),    K(KC_F3),    M(3),      M(3)   ) \
    X( 3,  K(KC_F4),    K(KC_F4),    M(4),      M(4)   ) \
    X( 4,  K(KC_F5),    K(KC_F5),    M(5),      M(5)   ) \
    X( 5,  K(KC_F6),    K(KC_F6),    M(6),      M(6)   ) \
    X( 6,  K(KC_F7),    K(KC_F7),    M(7),      M(7)   ) \
    X( 7,  K(KC_F8),    K(KC_F8),    M(8),      M(8)   ) \
    X( 8,  K(KC_F9),    K(KC_F9),    M(9),      M(9)   ) \
    X( 9,  K(KC_F10),   K(KC_F10),   M(10),     M(10)  ) \
    X(10,  K(KC_F11),   K(KC_F11),   M(11),     M(11)  ) \
    X(11,  K(KC_F12),   K(KC_F12),   M(12),     M(12)  ) \
    X(12,  K(KC_QUOT),  K(KC_DQUO),  K(KC_LNG2),K(KC_LNG1)) \
    X(13,  K(KC_SPC),   L(1),        K(KC_LNG2),L(1)   ) \
    X(14,  K(KC_ENT),   L(2),        K(KC_LNG1),L(2)   ) \
    X(15,  K(KC_APP),   M(0),        K(S(KC_APP)),M(0))

#define X(id, t1, h1, t2, h2) \
    static td_quad_ctx_t td_q_##id = { {t1}, {h1}, {t2}, {h2}, {AT_KEY, KC_NO} };
TD_LIST
#undef X

tap_dance_action_t tap_dance_actions[] = {
    #define X(id, t1, h1, t2, h2) \
        [id] = { .fn = { NULL, generic_quad_finished, generic_quad_reset }, .user_data = (void *)&td_q_##id },
    TD_LIST
    #undef X
};

// --- combo (max:20 COMBO_COUNT) ---
#define COMBOS_LIST \
    X(CB_CSTAB, C(S(KC_TAB)), KC_TAB,  KC_A) \
    X(CB_CTAB,  C(KC_TAB),    KC_TAB,  KC_S) \
    X(CB_C1,    C(KC_1),      KC_TAB,  KC_Q) \
    X(CB_C9,    C(KC_9),      KC_TAB,  KC_E) \
    X(CB_CR,    C(KC_R),      KC_TAB,  KC_R) \
    X(CB_CT,    C(KC_T),      KC_TAB,  KC_T) \
    X(CB_CW,    C(KC_W),      KC_ESC,  KC_W) \
    X(CB_BSPC,  KC_BSPC,      KC_S,    KC_D) \
    X(CB_DEL ,  KC_DEL,       KC_D,    KC_F) \
    X(CB_HNZN,  MCR(0),       KC_J,    KC_K) \
    X(CB_CUT ,  C(KC_X),      KC_Z,    KC_X) \
    X(CB_CPY ,  C(KC_C),      KC_Z,    KC_C) \
    X(CB_PST ,  C(KC_V),      KC_Z,    KC_V) \
    X(CB_ENT ,  KC_ENT,       KC_K,    KC_L) \
    X(CB_BTN1,  MS_BTN1,      KC_LSFT, KC_Z) \
    X(CB_BTN3,  MS_BTN3,      KC_LSFT, KC_X) \
    X(CB_BTN2,  MS_BTN2,      KC_LSFT, KC_C)

//    X(CB_ESC ,  KC_ESC,       KC_A,    KC_S,    KC_D)
//    X(CB_TAB ,  KC_TAB,       KC_S,    KC_D,    KC_F)

enum combo_names {
    #define X(name, res, ...) name,
    COMBOS_LIST
    #undef X
    COMBO_TOTAL_COUNT
};

#define X(name, res, ...) const uint16_t PROGMEM name##_keys[] = {__VA_ARGS__, COMBO_END};
COMBOS_LIST
#undef X

combo_t key_combos[] = {
    #define X(name, res, ...) [name] = COMBO(name##_keys, res),
    COMBOS_LIST
    #undef X
};

// --- override ---
/*
static bool b_num_lock = false;
bool insertnumlock(bool key_down, void *layer) {
    if (key_down) {
        b_num_lock = host_keyboard_led_state().num_lock;
        if (!b_num_lock) {
            tap_code(KC_NUM_LOCK);
            wait_ms(10);
        }
    } else {
        if (!b_num_lock) {
            tap_code(KC_NUM_LOCK);
        }
    }
    return true;
}
    X(KO_0_NP,  KC_0,    KC_P0,   0, MOD_MASK_SHIFT, 0) \
    X(KO_1_NP,  KC_1,    KC_P1,   0, MOD_MASK_SHIFT, 0) \
    X(KO_2_NP,  JE_2,    KC_P2,   0, MOD_MASK_SHIFT, 0) \
    X(KO_3_NP,  KC_3,    KC_P3,   0, MOD_MASK_SHIFT, 0) \
    X(KO_4_NP,  KC_4,    KC_P4,   0, MOD_MASK_SHIFT, 0) \
    X(KO_5_NP,  KC_5,    KC_P5,   0, MOD_MASK_SHIFT, 0) \
    X(KO_6_NP,  JE_6,    KC_P6,   0, MOD_MASK_SHIFT, 0) \
    X(KO_7_NP,  JE_7,    KC_P7,   0, MOD_MASK_SHIFT, 0) \
    X(KO_8_NP,  JE_8,    KC_P8,   0, MOD_MASK_SHIFT, 0) \
    X(KO_9_NP,  JE_9,    KC_P9,   0, MOD_MASK_SHIFT, 0)
        .custom_action    = insertnumlock, \
        .enabled          = &is_jp_mode \
*/
#define OVERRIDES_LIST \
    X(KO_S_BS,  KC_DEL,  KC_DEL, MOD_MASK_SHIFT, 0, MOD_MASK_SHIFT)

#define X(name, trig, repl, mask, neg, suppressed) \
    static const key_override_t name = { \
        .trigger          = trig, \
        .replacement      = repl, \
        .layers           = ~(layer_state_t)0, \
        .trigger_mods     = mask, \
        .negative_mod_mask = neg, \
        .suppressed_mods  = suppressed, \
        .options          = ko_options_default, \
        .custom_action    = NULL, \
        .enabled          = NULL \
    };
OVERRIDES_LIST
#undef X

const key_override_t *const key_overrides[] = {
    #define X(name, trig, repl, mask, neg, suppressed) &name,
    OVERRIDES_LIST
    #undef X
    NULL
};

// --- macro ---
static uint8_t default_macros[] PROGMEM =
    SS_DOWN(X_LALT) SS_TAP(X_GRAVE) SS_UP(X_LALT) "\0" //macro0
    SS_LCTL("a") "\0" //macro1
    SS_DOWN(X_LGUI) SS_DOWN(X_LSFT) SS_TAP(X_S) SS_UP(X_LSFT) SS_UP(X_LGUI) "\0" //macro2
    SS_LGUI("e") "\0" //macro3
    SS_LGUI("r") "\0" //macro4
    "%UserProfile%/Downloads/\0" //macro5
    "\0" //macro6
    "\0" //macro7
    "\0" //macro8
    "\0" //macro9
    "\0" //macro10
    "\0" //macro11
    "\0" //macro12
    SS_DOWN(X_LGUI) SS_DOWN(X_LSFT) SS_TAP(X_F23) SS_UP(X_LSFT) SS_UP(X_LGUI) "\0" //macro13
    "\0" //macro14
    "\0" //macro15
;

void eeconfig_init_user(void) {
    dynamic_keymap_macro_set_buffer(0, sizeof(default_macros), default_macros);
}

// --- proc ---
static uint8_t physical_shift_mask = 0;

void sync_physical_shift(void) {
    del_mods(MOD_MASK_SHIFT);
    add_mods(physical_shift_mask);
    send_keyboard_report();
}

void send_alt_code(uint16_t ascii_code) {
    bool initial_num_lock = host_keyboard_led_state().num_lock;
    if (!initial_num_lock) {
        tap_code(KC_NUM_LOCK);
        wait_ms(10);
    }
    clear_mods();
    register_code(KC_LALT);
    send_keyboard_report();
    char buf[5];
    itoa(ascii_code, buf, 10);
    for (char *p = buf; *p; p++) {
        uint16_t num_kc = (*p == '0') ? KC_P0 : (KC_P1 + (*p - '1'));
        tap_code(num_kc);
    }
    unregister_code(KC_LALT);
    send_keyboard_report();
    if (!initial_num_lock) {
        tap_code(KC_NUM_LOCK);
    }
    sync_physical_shift();
}

void exec_je_send(uint16_t target_kc, bool should_shift) {
    if (target_kc == KC_INT1 && should_shift) {
        send_alt_code(95);
        return;
    }
    if (target_kc == KC_INT3) {
        if (should_shift) {
            send_alt_code(124);
        } else {
            send_alt_code(92);
        }
        return;
    }
    del_mods(MOD_MASK_SHIFT);
    send_keyboard_report();
    if (should_shift) {
        add_mods(MOD_BIT_LSHIFT);
        send_keyboard_report();
    }
    register_code(target_kc);
}

typedef struct {
    uint8_t us_kc;
    uint8_t jp_kc;
    uint8_t jp_sh_kc;
    uint8_t flags;
} je_action_t;

#define JEFLGN 0x01
#define JEFLGS 0x02

#define JE_INDEX(kc) (kc - JE_2)
static const je_action_t je_table[] PROGMEM = {
    // usbase, jp, jpshift, flags(JEFLGN,JEFLGS)
    [JE_INDEX(JE_2)]    = {KC_2,    KC_2,    KC_LBRC, 0}, 
    [JE_INDEX(JE_6)]    = {KC_6,    KC_6,    KC_EQL,  0},
    [JE_INDEX(JE_7)]    = {KC_7,    KC_7,    KC_6,    JEFLGS},
    [JE_INDEX(JE_8)]    = {KC_8,    KC_8,    KC_QUOT, JEFLGS},
    [JE_INDEX(JE_9)]    = {KC_9,    KC_9,    KC_8,    JEFLGS},
    [JE_INDEX(JE_0)]    = {KC_0,    KC_0,    KC_9,    JEFLGS},
    [JE_INDEX(JE_MINS)] = {KC_MINS, KC_MINS, KC_INT1,   JEFLGS},
    [JE_INDEX(JE_EQL)]  = {KC_EQL,  KC_MINS, KC_SCLN, JEFLGN | JEFLGS},
    [JE_INDEX(JE_LBRC)] = {KC_LBRC, KC_RBRC, KC_RBRC, JEFLGS},
    [JE_INDEX(JE_RBRC)] = {KC_RBRC, KC_BSLS, KC_BSLS, JEFLGS},
    [JE_INDEX(JE_BSLS)] = {KC_BSLS, KC_INT3, KC_INT3, JEFLGS},
    [JE_INDEX(JE_SCLN)] = {KC_SCLN, KC_SCLN, KC_QUOT, 0},
    [JE_INDEX(JE_QUOT)] = {KC_QUOT, KC_7,    KC_2,    JEFLGN | JEFLGS},
    [JE_INDEX(JE_GRV)]  = {KC_GRV,  KC_LBRC, KC_EQL,  JEFLGN | JEFLGS}
};

bool process_record_userfn(uint16_t keycode, keyrecord_t *record) {
    if (keycode == KC_LSFT || keycode == KC_RSFT) {
        if (record->event.pressed) physical_shift_mask |= (keycode == KC_LSFT ? MOD_BIT_LSHIFT : MOD_BIT_RSHIFT);
        else physical_shift_mask &= ~(keycode == KC_LSFT ? MOD_BIT_LSHIFT : MOD_BIT_RSHIFT);
        return true; 
    }
    if (keycode == JE_TOG || keycode == JE_ON || keycode == JE_OF) {
        if (record->event.pressed) {
            if (keycode == JE_TOG) is_jp_mode = !is_jp_mode;
            else is_jp_mode = (keycode == JE_ON);
        }
        return false;
    }
    if (MCR_00 <= keycode && keycode <= MCR_15) {
        if (record->event.pressed) {
            dynamic_keymap_macro_send(keycode - MCR_00);
        }
        return false;
    }
    if (JE_2 <= keycode && keycode <= JE_GRV) {
        if (record->event.pressed) {
            je_action_t action;
            memcpy_P(&action, &je_table[JE_INDEX(keycode)], sizeof(je_action_t));
            bool is_shifted = (physical_shift_mask != 0);
            if (is_jp_mode) {
                if (is_shifted) {
                    exec_je_send(action.jp_sh_kc, (action.flags & JEFLGS));
                } else {
                    exec_je_send(action.jp_kc, (action.flags & JEFLGN));
                }
            } else {
                if (is_shifted) {
                    add_mods(physical_shift_mask);
                }
                register_code(action.us_kc);
            }
        } else {
            je_action_t action;
            memcpy_P(&action, &je_table[JE_INDEX(keycode)], sizeof(je_action_t));
            unregister_code(is_jp_mode ? action.jp_kc : action.us_kc);
            unregister_code(action.jp_sh_kc);
            sync_physical_shift();
        }
        return false;
    }
    return true; 
}

// keymap
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT_tkl_ansi(
        KC_ESC,   KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_BSPC,
        KC_TAB,   KC_A,     KC_S,     KC_D,     KC_F,     KC_G,     KC_H,     KC_J,     KC_K,     KC_L,     JE_SCLN,  KC_ENT,
        KC_LSFT,  KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_UP ,   KC_SLSH,
        KC_LCTL,  KC_LGUI,  KC_LALT,  TD(12),   TD(13),             KC_SPC,   TD(14),   TD(15),   KC_LEFT,  KC_DOWN,  KC_RGHT
    ),
    [1] = LAYOUT_tkl_ansi(
        KC_ESC,   KC_1,     JE_2,     KC_3,     KC_4,     KC_5,     JE_6,     JE_7,     JE_8,     JE_9,     JE_0,     KC_BSPC,
        KC_TAB,   TD( 0),   TD( 1),   TD( 2),   TD( 3),   TD( 4),   TD( 5),   JE_MINS,  JE_EQL,   JE_LBRC,  JE_RBRC,  KC_ENT,
        KC_LSFT,  TD( 6),   TD( 7),   TD( 8),   TD( 9),   TD(10),   TD(11),   JE_SCLN,  JE_QUOT,  JE_GRV,   KC_UP ,   JE_BSLS,
        KC_LCTL,  KC_LGUI,  KC_LALT,  KC_LNG2,  MO(1),              KC_SPC,   MO(3),    KC_LNG1,  KC_LEFT,  KC_DOWN,  KC_RGHT
    ),
    [2] = LAYOUT_tkl_ansi(
        KC_ESC,   MS_WHLL,  MS_UP  ,  MS_WHLR,  MS_WHLU,  KC_NO,    KC_HOME,  KC_PGDN,  KC_PGUP,  KC_END,   KC_NO,    KC_DEL,  
        KC_TAB,   MS_LEFT,  MS_DOWN,  MS_RGHT,  MS_WHLD,  KC_NO,    KC_LEFT,  KC_DOWN,  KC_UP,    KC_RGHT,  KC_NO,    KC_ENT,  
        KC_LSFT,  MS_BTN1,  MS_BTN3,  MS_BTN2,  MS_BTN4,  KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_PGUP , KC_NO,
        KC_LCTL,  KC_LGUI,  KC_LALT,  MS_BTN5,  MO(3),              KC_SPC,   MO(2),    KC_MENU,  KC_HOME,  KC_PGDN,  KC_END
    ),
    [3] = LAYOUT_tkl_ansi(
        MO(4),    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_CPNL,  KC_BRID,  KC_BRIU,  RM_VALU,  RM_PREV,  
        KC_TAB,   KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_MPLY,  KC_MPRV,  KC_MNXT,  RM_VALD,  RM_NEXT,  
        KC_LSFT,  KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_MUTE,  KC_VOLD,  KC_VOLU,  RM_SATU,  RM_TOGG,
        KC_LCTL,  KC_LGUI,  KC_LALT,  KC_NO,    MO(3),              KC_SPC,   MO(3),    KC_NO,    RM_HUED,  RM_SATD,  RM_HUEU
    ),
    [4] = LAYOUT_tkl_ansi(
        MO(4),    MD_BLE1,  MD_BLE2,  MD_BLE3,  MD_24G,   MD_USB,   KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_DEL,  
        KC_TAB,   KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    JE_TOG,   KC_NO,    KC_NO,    KC_NO,    KC_ENT,  
        KC_LSFT,  KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,
        KC_LCTL,  KC_LGUI,  KC_LALT,  KC_NO,    MO(3),              QK_BAT,   MO(3),    KC_NO,    KC_NO,    KC_NO,    KC_NO
    )
};