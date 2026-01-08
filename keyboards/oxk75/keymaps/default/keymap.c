// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#include "keycodes.h"
#include "quantum_keycodes.h"
#include QMK_KEYBOARD_H

#define KC_FN MO(1) // Function layer key

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /*
     * ┌───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┐
     * │Esc│F1 │F2 │F3 │F4 │F5 │F6 │F7 │F8 │F9 │F10│F11│F12│PSc│Pse│Del│
     * ├───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┴───┼───┤
     * │ ` │ 1 │ 2 │ 3 │ 4 │ 5 │ 6 │ 7 │ 8 │ 9 │ 0 │ - │ = │ Backsp│Hom│
     * ├───┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─────┼───┤
     * │ Tab │ Q │ W │ E │ R │ T │ Y │ U │ I │ O │ P │ [ │ ] │  \  │PgU│
     * ├─────┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┴─────┼───┤
     * │Caps │ A │ S │ D │ F │ G │ H │ J │ K │ L │ ; │ ' │  Enter  │PgD│
     * ├─────┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─────┬───┼───┤
     * │ Shift │ Z │ X │ C │ V │ B │ N │ M │ , │ . │ / │ Shift │ ↑ │End│
     * ├────┬──┴─┬─┴──┬┴───┴───┴───┴───┴───┴┬──┴─┬─┴──┬┴───┬───┼───┼───┤
     * │Ctrl│GUI │Alt │                     │Alt │GUI │Ctrl│ ← │ ↓ │ → │
     * └────┴────┴────┴─────────────────────┴────┴────┴────┴───┴───┴───┘
     */
    [0] = LAYOUT_ansi_75(
        KC_ESC,     KC_BRID,    KC_BRIU,    KC_CPNL,    KC_ASST,    RM_VALD,    RM_VALU,    KC_MPRV,    KC_MPLY,    KC_MNXT,    KC_MUTE,    KC_VOLD,    KC_VOLU,    RM_NEXT,    RM_TOGG,    KC_DEL,
        KC_GRV,     KC_1,       KC_2,       KC_3,       KC_4,       KC_5,       KC_6,       KC_7,       KC_8,       KC_9,       KC_0,       KC_MINS,    KC_EQL,     KC_BSPC,                KC_PGUP,    KC_MUTE,
        KC_TAB,     KC_Q,       KC_W,       KC_E,       KC_R,       KC_T,       KC_Y,       KC_U,       KC_I,       KC_O,       KC_P,       KC_LBRC,    KC_RBRC,                KC_BSLS,    KC_PGDN,
        KC_CAPS,    KC_A,       KC_S,       KC_D,       KC_F,       KC_G,       KC_H,       KC_J,       KC_K,       KC_L,       KC_SCLN,    KC_QUOT,                KC_ENT,                 KC_HOME,
        KC_LSFT,    KC_Z,       KC_X,       KC_C,       KC_V,       KC_B,       KC_N,       KC_M,       KC_COMM,    KC_DOT,     KC_SLSH,                KC_RSFT,                KC_UP,      KC_END,
        KC_LCTL,    KC_LGUI,    KC_LALT,                            KC_SPC,                                         KC_RALT,    KC_FN,                  KC_RCTL,    KC_LEFT,    KC_DOWN,    KC_RGHT
    ),

    [1] = LAYOUT_ansi_75(
        KC_NO,      KC_F1,      KC_F2,      KC_F3,      KC_F4,      KC_F5,      KC_F6,      KC_F7,      KC_F8,      KC_F9,      KC_F10,     KC_F11,     KC_F12,     RM_VALD,    RM_VALU,    RM_HUEU,
        KC_NO,      KC_NO,      KC_NO,      KC_NO,      KC_NO,      KC_NO,      KC_NO,      KC_NO,      KC_NO,      KC_NO,      KC_NO,      KC_NO,      KC_NO,      KC_NO,                  KC_NO,      KC_NO,
        KC_NO,      KC_NO,      KC_NO,      KC_NO,      KC_NO,      KC_NO,      KC_NO,      KC_NO,      KC_NO,      KC_NO,      KC_NO,      KC_NO,      KC_NO,                  KC_NO,      KC_NO,
        KC_NO,      KC_NO,      KC_NO,      KC_NO,      KC_NO,      KC_NO,      KC_NO,      KC_NO,      KC_NO,      KC_NO,      KC_NO,      KC_NO,                  KC_NO,                  KC_NO,
        KC_NO,      KC_NO,      KC_NO,      KC_NO,      KC_NO,      KC_NO,      KC_NO,      KC_NO,      KC_NO,      KC_NO,      KC_NO,                  KC_NO,                  KC_NO,      KC_NO,
        KC_NO,      KC_NO,      KC_NO,                              KC_NO,                                          KC_NO,      KC_NO,                  KC_NO,      KC_NO,      KC_NO,      KC_NO
    )
};

/* RGB Matrix */

// void keyboard_post_init_user(void) {
//     print("Initializing WS2812 RGB Matrix\n");
//     rgb_matrix_enable();
//     // ws2812_init();
// }