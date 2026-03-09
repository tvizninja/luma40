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
#include "rdmctmzt_common.h"
#include "keymap_japanese.h"

// jis eng mapping
typedef enum {
    SYM_GRV,
    SYM_TILD,
    SYM_AT,
    SYM_CIRC,
    SYM_AMPR,
    SYM_ASTR,
    SYM_LPRN,
    SYM_RPRN,
    SYM_UNDS,
    SYM_EQL,
    SYM_PLUS,
    SYM_LBRC,
    SYM_LCBR,
    SYM_RBRC,
    SYM_RCBR,
    SYM_BSLS,
    SYM_PIPE,
    SYM_COLN,
    SYM_QUOT,
    SYM_DQUO,
    SYM__COUNT
} symbol_t;

#define SEQ_JP_TILD    SS_DOWN(X_LSFT) SS_TAP(X_EQL)  SS_UP(X_LSFT)
#define SEQ_JP_GRV     SS_DOWN(X_LSFT) SS_TAP(X_LBRC) SS_UP(X_LSFT)
#define SEQ_JP_AT      SS_TAP(X_LBRC)
#define SEQ_JP_CIRC    SS_TAP(X_EQL)
#define SEQ_JP_AMPR    SS_DOWN(X_LSFT) SS_TAP(X_6)    SS_UP(X_LSFT)
#define SEQ_JP_ASTR    SS_DOWN(X_LSFT) SS_TAP(X_QUOT) SS_UP(X_LSFT)
#define SEQ_JP_LPRN    SS_DOWN(X_LSFT) SS_TAP(X_8)    SS_UP(X_LSFT)
#define SEQ_JP_RPRN    SS_DOWN(X_LSFT) SS_TAP(X_9)    SS_UP(X_LSFT)
#define SEQ_JP_UNDS    SS_DOWN(X_LSFT) SS_TAP(X_INT1) SS_UP(X_LSFT)
#define SEQ_JP_EQL     SS_DOWN(X_LSFT) SS_TAP(X_MINS) SS_UP(X_LSFT)
#define SEQ_JP_PLUS    SS_DOWN(X_LSFT) SS_TAP(X_SCLN) SS_UP(X_LSFT)
#define SEQ_JP_LBRC    SS_TAP(X_RBRC)
#define SEQ_JP_LCBR    SS_DOWN(X_LSFT) SS_TAP(X_RBRC) SS_UP(X_LSFT)
#define SEQ_JP_RBRC    SS_TAP(X_NUHS)
#define SEQ_JP_RCBR    SS_DOWN(X_LSFT) SS_TAP(X_NUHS) SS_UP(X_LSFT)
#define SEQ_JP_BSLS    SS_TAP(X_INT1)
#define SEQ_JP_PIPE    SS_DOWN(X_LSFT) SS_TAP(X_INT3) SS_UP(X_LSFT)
#define SEQ_JP_COLN    SS_TAP(X_QUOT)
#define SEQ_JP_DQUO    SS_DOWN(X_LSFT) SS_TAP(X_2)    SS_UP(X_LSFT)
#define SEQ_JP_QUOT    SS_DOWN(X_LSFT) SS_TAP(X_7)    SS_UP(X_LSFT)

#define NO_EXTRA_MODS (MOD_MASK_SHIFT | MOD_MASK_CTRL | MOD_MASK_ALT | MOD_MASK_GUI)

#define W_SHIFT(name, trigger_key, symbol_id) static const sym_override_ctx_t name##_ctx = { .sym = (symbol_id), .suppress_mods = MOD_MASK_SHIFT }; static const key_override_t name = { .trigger = (trigger_key), .trigger_mods = MOD_MASK_SHIFT, .layers = ~0, .negative_mod_mask = 0, .suppressed_mods = 0, .replacement = KC_NO, .options = ko_options_default, .custom_action = symbol_override_cb, .context = (void *)&name##_ctx, .enabled = &jis_override_enabled }
#define WO_SHIFT(name, trigger_key, symbol_id) static const sym_override_ctx_t name##_ctx = { .sym = (symbol_id), .suppress_mods = 0 }; static const key_override_t name = { .trigger = (trigger_key), .trigger_mods = 0, .layers = ~0, .negative_mod_mask = NO_EXTRA_MODS, .suppressed_mods = 0, .replacement = KC_NO, .options = ko_options_default, .custom_action = symbol_override_cb, .context = (void *)&name##_ctx, .enabled = &jis_override_enabled }

typedef struct {
    symbol_t sym;
    uint8_t suppress_mods;
} sym_override_ctx_t;

enum {
    JIS_TOG = SAFE_RANGE, JIS_ON, JIS_OFF,
};
bool jis_override_enabled = true;


static const char PROGMEM us_sym_grv[]  = "`";
static const char PROGMEM us_sym_tild[] = "~";
static const char PROGMEM us_sym_at[]   = "@";
static const char PROGMEM us_sym_circ[] = "^";
static const char PROGMEM us_sym_ampr[] = "&";
static const char PROGMEM us_sym_astr[] = "*";
static const char PROGMEM us_sym_lprn[] = "(";
static const char PROGMEM us_sym_rprn[] = ")";
static const char PROGMEM us_sym_unds[] = "_";
static const char PROGMEM us_sym_eql[]  = "=";
static const char PROGMEM us_sym_plus[] = "+";
static const char PROGMEM us_sym_lbrc[] = "[";
static const char PROGMEM us_sym_lcbr[] = "{";
static const char PROGMEM us_sym_rbrc[] = "]";
static const char PROGMEM us_sym_rcbr[] = "}";
static const char PROGMEM us_sym_bsls[] = "\\";
static const char PROGMEM us_sym_pipe[] = "|";
static const char PROGMEM us_sym_coln[] = ":";
static const char PROGMEM us_sym_quot[] = "'";
static const char PROGMEM us_sym_dquo[] = "\"";

static const char PROGMEM jis_sym_grv[]  = SEQ_JP_GRV;
static const char PROGMEM jis_sym_tild[] = SEQ_JP_TILD;
static const char PROGMEM jis_sym_at[]   = SEQ_JP_AT;
static const char PROGMEM jis_sym_circ[] = SEQ_JP_CIRC;
static const char PROGMEM jis_sym_ampr[] = SEQ_JP_AMPR;
static const char PROGMEM jis_sym_astr[] = SEQ_JP_ASTR;
static const char PROGMEM jis_sym_lprn[] = SEQ_JP_LPRN;
static const char PROGMEM jis_sym_rprn[] = SEQ_JP_RPRN;
static const char PROGMEM jis_sym_unds[] = SEQ_JP_UNDS;
static const char PROGMEM jis_sym_eql[]  = SEQ_JP_EQL;
static const char PROGMEM jis_sym_plus[] = SEQ_JP_PLUS;
static const char PROGMEM jis_sym_lbrc[] = SEQ_JP_LBRC;
static const char PROGMEM jis_sym_lcbr[] = SEQ_JP_LCBR;
static const char PROGMEM jis_sym_rbrc[] = SEQ_JP_RBRC;
static const char PROGMEM jis_sym_rcbr[] = SEQ_JP_RCBR;
static const char PROGMEM jis_sym_bsls[] = SEQ_JP_BSLS;
static const char PROGMEM jis_sym_pipe[] = SEQ_JP_PIPE;
static const char PROGMEM jis_sym_coln[] = SEQ_JP_COLN;
static const char PROGMEM jis_sym_quot[] = SEQ_JP_QUOT;
static const char PROGMEM jis_sym_dquo[] = SEQ_JP_DQUO;

static const char *const PROGMEM us_symbol_table[SYM__COUNT] = {
    [SYM_GRV]  = us_sym_grv,
    [SYM_TILD] = us_sym_tild,
    [SYM_AT]   = us_sym_at,
    [SYM_CIRC] = us_sym_circ,
    [SYM_AMPR] = us_sym_ampr,
    [SYM_ASTR] = us_sym_astr,
    [SYM_LPRN] = us_sym_lprn,
    [SYM_RPRN] = us_sym_rprn,
    [SYM_UNDS] = us_sym_unds,
    [SYM_EQL]  = us_sym_eql,
    [SYM_PLUS] = us_sym_plus,
    [SYM_LBRC] = us_sym_lbrc,
    [SYM_LCBR] = us_sym_lcbr,
    [SYM_RBRC] = us_sym_rbrc,
    [SYM_RCBR] = us_sym_rcbr,
    [SYM_BSLS] = us_sym_bsls,
    [SYM_PIPE] = us_sym_pipe,
    [SYM_COLN] = us_sym_coln,
    [SYM_QUOT] = us_sym_quot,
    [SYM_DQUO] = us_sym_dquo,
};

static const char *const PROGMEM jis_symbol_table[SYM__COUNT] = {
    [SYM_GRV]  = jis_sym_grv,
    [SYM_TILD] = jis_sym_tild,
    [SYM_AT]   = jis_sym_at,
    [SYM_CIRC] = jis_sym_circ,
    [SYM_AMPR] = jis_sym_ampr,
    [SYM_ASTR] = jis_sym_astr,
    [SYM_LPRN] = jis_sym_lprn,
    [SYM_RPRN] = jis_sym_rprn,
    [SYM_UNDS] = jis_sym_unds,
    [SYM_EQL]  = jis_sym_eql,
    [SYM_PLUS] = jis_sym_plus,
    [SYM_LBRC] = jis_sym_lbrc,
    [SYM_LCBR] = jis_sym_lcbr,
    [SYM_RBRC] = jis_sym_rbrc,
    [SYM_RCBR] = jis_sym_rcbr,
    [SYM_BSLS] = jis_sym_bsls,
    [SYM_PIPE] = jis_sym_pipe,
    [SYM_COLN] = jis_sym_coln,
    [SYM_QUOT] = jis_sym_quot,
    [SYM_DQUO] = jis_sym_dquo,
};

void emit_symbol(symbol_t sym) {
    if (sym >= SYM__COUNT) {
        return;
    }

    const char *seq;

    if (jis_override_enabled) {
        seq = (const char *)pgm_read_ptr(&jis_symbol_table[sym]);
    } else {
        seq = (const char *)pgm_read_ptr(&us_symbol_table[sym]);
    }

    if (seq != NULL) {
        send_string_P(seq);
    }
}

void emit_symbol_shifted(symbol_t normal_sym, symbol_t shifted_sym) {
    if (get_mods() & MOD_MASK_SHIFT) {
        emit_symbol(shifted_sym);
    } else {
        emit_symbol(normal_sym);
    }
}

//for combo
enum combos{
    CMB_00, CMB_01, CMB_02, CMB_03, CMB_04, CMB_05, CMB_06, CMB_07, CMB_08, CMB_09,
    CMB_10, CMB_11, CMB_12, CMB_13, CMB_14, CMB_15,
};

const uint16_t PROGMEM combo_00[] = {KC_TAB, KC_A, COMBO_END}; 
const uint16_t PROGMEM combo_01[] = {KC_TAB, KC_S, COMBO_END}; 
const uint16_t PROGMEM combo_02[] = {KC_TAB, KC_Q, COMBO_END};
const uint16_t PROGMEM combo_03[] = {KC_TAB, KC_E, COMBO_END};
const uint16_t PROGMEM combo_04[] = {KC_TAB, KC_R, COMBO_END};
const uint16_t PROGMEM combo_05[] = {KC_TAB, KC_T, COMBO_END};
const uint16_t PROGMEM combo_06[] = {KC_ESC, KC_W, COMBO_END};
const uint16_t PROGMEM combo_07[] = {KC_TAB, KC_Z, COMBO_END};
const uint16_t PROGMEM combo_08[] = {KC_TAB, KC_X, COMBO_END};
const uint16_t PROGMEM combo_09[] = {KC_TAB, KC_C, COMBO_END};
const uint16_t PROGMEM combo_10[] = {KC_TAB, KC_V, COMBO_END};
const uint16_t PROGMEM combo_11[] = {KC_LSFT, KC_Z, COMBO_END};
const uint16_t PROGMEM combo_12[] = {COMBO_END};
const uint16_t PROGMEM combo_13[] = {COMBO_END};
const uint16_t PROGMEM combo_14[] = {COMBO_END};
const uint16_t PROGMEM combo_15[] = {COMBO_END};

combo_t key_combos[] = {
    [CMB_00] = COMBO(combo_00, LCTL(LSFT(KC_TAB))),
    [CMB_01] = COMBO(combo_01, LCTL(KC_TAB)),
    [CMB_02] = COMBO(combo_02, LCTL(KC_1)),
    [CMB_03] = COMBO(combo_03, LCTL(KC_9)),
    [CMB_04] = COMBO(combo_04, LCTL(KC_R)),
    [CMB_05] = COMBO(combo_05, LCTL(KC_T)),
    [CMB_06] = COMBO(combo_06, LCTL(KC_W)),
    [CMB_07] = COMBO(combo_07, LCTL(KC_Z)),
    [CMB_08] = COMBO(combo_08, LCTL(KC_X)),
    [CMB_09] = COMBO(combo_09, LCTL(KC_C)),
    [CMB_10] = COMBO(combo_10, LCTL(KC_V)),
    [CMB_11] = COMBO(combo_11, MS_BTN1),
    [CMB_12] = COMBO(combo_12, KC_NO),
    [CMB_13] = COMBO(combo_13, KC_NO),
    [CMB_14] = COMBO(combo_14, KC_NO),
    [CMB_15] = COMBO(combo_15, KC_NO),
};


// override
bool ovrca_del(bool activated, void *context){
    if (activated){
        uint8_t saved_mods = get_mods();
        del_mods(MOD_MASK_SHIFT);
        send_keyboard_report();
        tap_code(KC_DEL);
        set_mods(saved_mods);
        send_keyboard_report();
    }
    return false;
}

const key_override_t override_al_del = {
    .trigger           = KC_BSPC,
    .replacement       = KC_DEL,
    .trigger_mods      = MOD_MASK_SHIFT,
    .suppressed_mods   = MOD_MASK_SHIFT,
    .layers            = ~0,
    .negative_mod_mask = (uint8_t) ~(MOD_MASK_SHIFT),
    .custom_action     = ovrca_del,
    .options           = ko_options_default,
    .enabled           = NULL,
};

static bool symbol_override_cb(bool activated, void *context) {
    if (!activated || context == NULL) {
        return false;
    }
    const sym_override_ctx_t *ctx = (const sym_override_ctx_t *)context;
    uint8_t saved_mods = get_mods();
    if (ctx->suppress_mods) {
        del_mods(ctx->suppress_mods);
        send_keyboard_report();
    }
    emit_symbol(ctx->sym);
    set_mods(saved_mods);
    send_keyboard_report();
    return false;
}

W_SHIFT (ov_grv_tild,   KC_GRV,  SYM_TILD);
WO_SHIFT(ov_grv_grv,    KC_GRV,  SYM_GRV);
W_SHIFT (ov_2_at,       KC_2,    SYM_AT);
W_SHIFT (ov_6_circ,     KC_6,    SYM_CIRC);
W_SHIFT (ov_7_ampr,     KC_7,    SYM_AMPR);
W_SHIFT (ov_8_astr,     KC_8,    SYM_ASTR);
W_SHIFT (ov_9_lprn,     KC_9,    SYM_LPRN);
W_SHIFT (ov_0_rprn,     KC_0,    SYM_RPRN);
W_SHIFT (ov_mins_unds,  KC_MINS, SYM_UNDS);
WO_SHIFT(ov_eql_eq,     KC_EQL,  SYM_EQL);
W_SHIFT (ov_eql_plus,   KC_EQL,  SYM_PLUS);
WO_SHIFT(ov_lbrc_lbrc,  KC_LBRC, SYM_LBRC);
W_SHIFT (ov_lbrc_lcbr,  KC_LBRC, SYM_LCBR);
WO_SHIFT(ov_rbrc_rbrc,  KC_RBRC, SYM_RBRC);
W_SHIFT (ov_rbrc_rcbr,  KC_RBRC, SYM_RCBR);
WO_SHIFT(ov_bsls_bsls,  KC_BSLS, SYM_BSLS);
W_SHIFT (ov_bsls_pipe,  KC_BSLS, SYM_PIPE);
W_SHIFT (ov_scln_coln,  KC_SCLN, SYM_COLN);
W_SHIFT (ov_quot_dquo,  KC_QUOT, SYM_DQUO);
WO_SHIFT(ov_quot_quot,  KC_QUOT, SYM_QUOT);

const key_override_t *key_overrides[] = {
    &override_al_del,
    &ov_grv_tild,
    &ov_grv_grv,
    &ov_2_at,
    &ov_6_circ,
    &ov_7_ampr,
    &ov_8_astr,
    &ov_9_lprn,
    &ov_0_rprn,
    &ov_mins_unds,
    &ov_eql_eq,
    &ov_eql_plus,
    &ov_lbrc_lbrc,
    &ov_lbrc_lcbr,
    &ov_rbrc_rbrc,
    &ov_rbrc_rcbr,
    &ov_bsls_bsls,
    &ov_bsls_pipe,
    &ov_scln_coln,
    &ov_quot_dquo,
    &ov_quot_quot,
    NULL
};

// macro init
static void seed_via_macros_if_empty(void) {
    uint8_t first = 0;
    dynamic_keymap_macro_get_buffer(0, 1, &first);

    if (first == 0) {
        static uint8_t defaults[] =
            "\0" //macro0
            SS_LCTL("a") "\0" //macro1
            "\0" //macro2
            "\0" //macro3
            SS_LGUI("r") "\0" //macro4
            "%UserProfile%/Downloads/\0" //macro5
            "\0" //macro6
            "\0" //macro7
            "\0" //macro8
            "\0" //macro9
            "\0" //macro10
            "\0" //macro11
            "\0" //macro12
            ;
        dynamic_keymap_macro_set_buffer(0, sizeof(defaults), defaults);
    }
}

void keyboard_post_init_userfn(void) {
    seed_via_macros_if_empty();
}

//for tapdance
enum {
    TD_M00, TD_M01, TD_M02, TD_M03, TD_M04, TD_M05, TD_M06, TD_M07, TD_M08, TD_M09,
    TD_M10, TD_M11, TD_M12, TD_M13, TD_M14, TD_M15,
};

typedef enum {
    TD_NONE,
    TD_SINGLE_TAP,
    TD_SINGLE_HOLD,
    TD_DOUBLE_TAP,
    TD_DOUBLE_HOLD,
    TD_TRIPLE_TAP,
    TD_TRIPLE_HOLD
} td_state_t;

static td_state_t td_state = TD_NONE;

static td_state_t cur_dance(tap_dance_state_t *state) {
    if (state->count == 1) {
        return state->pressed ? TD_SINGLE_HOLD : TD_SINGLE_TAP;
    } else if (state->count == 2) {
        return state->pressed ? TD_DOUBLE_HOLD : TD_DOUBLE_TAP;
    } else if (state->count == 3) {
        return state->pressed ? TD_TRIPLE_HOLD : TD_TRIPLE_TAP;
    }
    return TD_NONE;
}

void td_reset(tap_dance_state_t *state, void *user_data) {
    switch (td_state) {
        case TD_SINGLE_HOLD:
        case TD_DOUBLE_HOLD:
        case TD_TRIPLE_HOLD:
            unregister_code(KC_LCTL);
            unregister_code(KC_LALT);
            unregister_code(KC_LGUI);
            break;
        default:
            break;
    }
    td_state = TD_NONE;
}

void td_01(tap_dance_state_t *state, void *user_data){
    td_state = cur_dance(state);
    switch (td_state) {
        case TD_SINGLE_TAP:
        case TD_SINGLE_HOLD:
            tap_code(KC_F1);
            break;
        case TD_DOUBLE_TAP:
        case TD_DOUBLE_HOLD:
            dynamic_keymap_macro_send(1);
            break;
        default:
            break;
    }
}

void td_02(tap_dance_state_t *state, void *user_data){
    td_state = cur_dance(state);
    switch (td_state) {
        case TD_SINGLE_TAP:
        case TD_SINGLE_HOLD:
            tap_code(KC_F2);
            break;
        case TD_DOUBLE_TAP:
        case TD_DOUBLE_HOLD:
            dynamic_keymap_macro_send(2);
            break;
        default:
            break;
    }
}

void td_03(tap_dance_state_t *state, void *user_data){
    td_state = cur_dance(state);
    switch (td_state) {
        case TD_SINGLE_TAP:
        case TD_SINGLE_HOLD:
            tap_code(KC_F3);
            break;
        case TD_DOUBLE_TAP:
        case TD_DOUBLE_HOLD:
            dynamic_keymap_macro_send(3);
            break;
        default:
            break;
    }
}

void td_04(tap_dance_state_t *state, void *user_data){
    td_state = cur_dance(state);
    switch (td_state) {
        case TD_SINGLE_TAP:
        case TD_SINGLE_HOLD:
            tap_code(KC_F4);
            break;
        case TD_DOUBLE_TAP:
        case TD_DOUBLE_HOLD:
            dynamic_keymap_macro_send(4);
            break;
        default:
            break;
    }
}

void td_05(tap_dance_state_t *state, void *user_data){
    td_state = cur_dance(state);
    switch (td_state) {
        case TD_SINGLE_TAP:
        case TD_SINGLE_HOLD:
            tap_code(KC_F5);
            break;
        case TD_DOUBLE_TAP:
        case TD_DOUBLE_HOLD:
            dynamic_keymap_macro_send(5);
            break;
        default:
            break;
    }
}

void td_06(tap_dance_state_t *state, void *user_data){
    td_state = cur_dance(state);
    switch (td_state) {
        case TD_SINGLE_TAP:
        case TD_SINGLE_HOLD:
            tap_code(KC_F6);
            break;
        case TD_DOUBLE_TAP:
        case TD_DOUBLE_HOLD:
            dynamic_keymap_macro_send(6);
            break;
        default:
            break;
    }
}

void td_07(tap_dance_state_t *state, void *user_data){
    td_state = cur_dance(state);
    switch (td_state) {
        case TD_SINGLE_TAP:
        case TD_SINGLE_HOLD:
            tap_code(KC_F7);
            break;
        case TD_DOUBLE_TAP:
        case TD_DOUBLE_HOLD:
            dynamic_keymap_macro_send(7);
            break;
        default:
            break;
    }
}

void td_08(tap_dance_state_t *state, void *user_data){
    td_state = cur_dance(state);
    switch (td_state) {
        case TD_SINGLE_TAP:
        case TD_SINGLE_HOLD:
            tap_code(KC_F8);
            break;
        case TD_DOUBLE_TAP:
        case TD_DOUBLE_HOLD:
            dynamic_keymap_macro_send(8);
            break;
        default:
            break;
    }
}

void td_09(tap_dance_state_t *state, void *user_data){
    td_state = cur_dance(state);
    switch (td_state) {
        case TD_SINGLE_TAP:
        case TD_SINGLE_HOLD:
            tap_code(KC_F9);
            break;
        case TD_DOUBLE_TAP:
        case TD_DOUBLE_HOLD:
            dynamic_keymap_macro_send(9);
            break;
        default:
            break;
    }
}

void td_10(tap_dance_state_t *state, void *user_data){
    td_state = cur_dance(state);
    switch (td_state) {
        case TD_SINGLE_TAP:
        case TD_SINGLE_HOLD:
            tap_code(KC_F10);
            break;
        case TD_DOUBLE_TAP:
        case TD_DOUBLE_HOLD:
            dynamic_keymap_macro_send(10);
            break;
        default:
            break;
    }
}

void td_11(tap_dance_state_t *state, void *user_data){
    td_state = cur_dance(state);
    switch (td_state) {
        case TD_SINGLE_TAP:
        case TD_SINGLE_HOLD:
            tap_code(KC_F11);
            break;
        case TD_DOUBLE_TAP:
        case TD_DOUBLE_HOLD:
            dynamic_keymap_macro_send(11);
            break;
        default:
            break;
    }
}

void td_12(tap_dance_state_t *state, void *user_data){
    td_state = cur_dance(state);
    switch (td_state) {
        case TD_SINGLE_TAP:
        case TD_SINGLE_HOLD:
            tap_code(KC_F12);
            break;
        case TD_DOUBLE_TAP:
        case TD_DOUBLE_HOLD:
            dynamic_keymap_macro_send(12);
            break;
        default:
            break;
    }
}

void td_13(tap_dance_state_t *state, void *user_data){
    td_state = cur_dance(state);
    switch (td_state) {
        case TD_SINGLE_TAP:
        case TD_SINGLE_HOLD:
            emit_symbol(SYM_QUOT);
            break;
        case TD_DOUBLE_TAP:
        case TD_DOUBLE_HOLD:
            tap_code(KC_LNG2);
            break;
        default:
            break;
    }
}

void td_14(tap_dance_state_t *state, void *user_data){
    td_state = cur_dance(state);
    switch (td_state) {
        case TD_SINGLE_TAP:
            tap_code(KC_APP);
            break;
        case TD_SINGLE_HOLD:
            register_code(KC_LSFT);
            tap_code(KC_APP);
            unregister_code(KC_LSFT);
            break;
        case TD_DOUBLE_TAP:
        case TD_DOUBLE_HOLD:
            tap_code(KC_GRV);
            break;
        default:
            break;
    }
}

tap_dance_action_t tap_dance_actions[] = {
//    [TD_M00] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, td_00, td_reset),
    [TD_M01] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, td_01, td_reset),
    [TD_M02] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, td_02, td_reset),
    [TD_M03] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, td_03, td_reset),
    [TD_M04] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, td_04, td_reset),
    [TD_M05] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, td_05, td_reset),
    [TD_M06] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, td_06, td_reset),
    [TD_M07] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, td_07, td_reset),
    [TD_M08] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, td_08, td_reset),
    [TD_M09] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, td_09, td_reset),
    [TD_M10] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, td_10, td_reset),
    [TD_M11] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, td_11, td_reset),
    [TD_M12] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, td_12, td_reset),
    [TD_M13] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, td_13, td_reset),
    [TD_M14] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, td_14, td_reset),
//    [TD_M15] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, td_15, td_reset)
};

// keymap
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT_tkl_ansi(
        KC_ESC,  KC_Q,    KC_W,      KC_E,       KC_R,       KC_T,      KC_Y,     KC_U,    KC_I,        KC_O,     KC_P,     KC_BSPC,  
        KC_TAB,  KC_A,    KC_S,      KC_D,       KC_F,       KC_G,      KC_H,     KC_J,    KC_K,        KC_L,     KC_SCLN,  KC_ENT,  
        KC_LSFT, KC_Z,    KC_X,      KC_C,       KC_V,       KC_B,      KC_N,     KC_M,    KC_COMM,     KC_DOT,   KC_UP ,   KC_SLSH,
        KC_LCTL, KC_LGUI, KC_LALT,   TD(TD_M13), MO(1),                 KC_SPC,   MO(2),   TD(TD_M14),  KC_LEFT,  KC_DOWN,  KC_RGHT
    ),
    [1] = LAYOUT_tkl_ansi(
        KC_ESC,  KC_1,       KC_2,       KC_3,       KC_4,       KC_5,       KC_6,       KC_7,    KC_8,    KC_9,     KC_0,     KC_BSPC,  
        KC_TAB,  TD(TD_M01), TD(TD_M02), TD(TD_M03), TD(TD_M04), TD(TD_M05), TD(TD_M06), KC_MINS, KC_EQL,  KC_LBRC,  KC_RBRC,  KC_ENT,  
        KC_LSFT, TD(TD_M07), TD(TD_M08), TD(TD_M09), TD(TD_M10), TD(TD_M11), TD(TD_M12), KC_SCLN, KC_QUOT, KC_GRV,   KC_UP ,   KC_BSLS,
        KC_LCTL, KC_LGUI,    KC_LALT,    KC_LNG2,    MO(1),                  KC_SPC,     MO(3),   KC_LNG1, KC_LEFT,  KC_DOWN,  KC_RGHT
    ),
    [2] = LAYOUT_tkl_ansi(
        KC_ESC,  MS_WHLL, MS_UP  , MS_WHLR, MS_WHLU, KC_NO,     KC_HOME,  KC_PGDN, KC_PGUP, KC_END,   KC_NO,    KC_DEL,  
        KC_TAB,  MS_LEFT, MS_DOWN, MS_RGHT, MS_WHLD, KC_NO,     KC_LEFT,  KC_DOWN, KC_UP,   KC_RGHT,  KC_NO,    KC_ENT,  
        KC_LSFT, MS_BTN1, MS_BTN3, MS_BTN2, MS_BTN4, KC_NO,     KC_NO,    KC_NO,   KC_NO,   KC_NO,    KC_PGUP , KC_NO,
        KC_LCTL, KC_LGUI, KC_LALT, MS_BTN5,   MO(3),            KC_SPC,   MO(2),   KC_MENU, KC_HOME,  KC_PGDN,  KC_END
    ),
    [3] = LAYOUT_tkl_ansi(
        MO(4),   KC_NO,    KC_NO,    KC_NO,    KC_NO,  KC_NO,  KC_NO,   KC_CPNL,  KC_BRID,  KC_BRIU,  RM_VALU,  KC_DEL,  
        KC_TAB,  KC_NO,    KC_NO,    KC_NO,    KC_NO,  KC_NO,  KC_NO,   KC_MPLY,  KC_MPRV,  KC_MNXT,  RM_VALD,  KC_ENT,  
        KC_LSFT, KC_NO,    KC_NO,    KC_NO,    KC_NO,  KC_NO,  KC_NO,   KC_MUTE,  KC_VOLD,  KC_VOLU,  RM_SATU,  RM_TOGG,
        KC_LCTL, KC_LGUI,  KC_LALT,  KC_NO,    MO(3),          KC_SPC,  MO(3),    KC_NO,    RM_HUED,  RM_SATD,  RM_HUEU
    ),
    [4] = LAYOUT_tkl_ansi(
        MO(4),   MD_BLE1, MD_BLE2,   MD_BLE3,  MD_24G,  KC_NO,  KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,    KC_DEL,  
        KC_TAB,  KC_NO,    KC_NO,    KC_NO,    KC_NO,   KC_NO,  KC_NO,   JIS_TOG, KC_NO,   KC_NO,   KC_NO,    KC_ENT,  
        KC_LSFT, KC_NO,    KC_NO,    KC_NO,    KC_NO,   KC_NO,  KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,    KC_NO,
        KC_LCTL, KC_LGUI,  KC_LALT,  KC_NO,    MO(3),           KC_SPC,  MO(3),   KC_NO,   KC_NO,   KC_NO,    KC_NO
    )
/*
        KC_TAB,  MD_BLE1, MD_BLE2,   MD_BLE3,  MD_24G,     RM_NEXT,   TO(1),    TO(0),   KC_LBRC, KC_RBRC,  KC_BSLS,  RGB_RTOG,  
        KC_LSFT, KC_INS,  KC_DEL,    KC_HOME,  KC_END,     KC_PGUP,   KC_PGDN,  RGB_SAD, RGB_HUD, RGB_HUI,  RGB_VAI , QK_BAT,
        KC_NO,   KC_DEL,  KC_GRV,    KC_LALT,  QK_WLO,                U_EE_CLR, RGB_SAI, KC_EQL,  RGB_SPD,  RGB_VAD,  RGB_SPI
*/
};

bool process_record_userfn(uint16_t keycode, keyrecord_t *record){
    if(!record->event.pressed){
        return true;
    }

    switch (keycode) {
        case JIS_TOG:
            jis_override_enabled = !jis_override_enabled;
            return false;
        case JIS_ON:
            jis_override_enabled = true;
            return false;
        case JIS_OFF:
            jis_override_enabled = false;
            return false;
    }
    return true;
}
