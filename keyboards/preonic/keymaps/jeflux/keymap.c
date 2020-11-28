/* Copyright 2015-2017 Jack Humbert
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
#include "muse.h"
#include "raw_hid.h"
#include "aurora.h"

#define RL(x) RALT(x)


// How long (in milliseconds) to wait between animation steps for each of the "Solid color breathing" animations
const uint8_t RGBLED_BREATHING_INTERVALS[] PROGMEM = {255, 100, 40, 20};

// How long (in milliseconds) to wait between animation steps for each of the "Cycling rainbow" animations
const uint8_t RGBLED_RAINBOW_MOOD_INTERVALS[] PROGMEM = {255, 150, 50};

// How long (in milliseconds) to wait between animation steps for each of the "Swirling rainbow" animations
const uint8_t RGBLED_RAINBOW_SWIRL_INTERVALS[] PROGMEM = {255, 150, 50};


enum preonic_layers {
	_QWERTY,
	_LOWER,
	_RAISE,
	_GAME,
	_MEDIA,
	_ADJUST
};

enum preonic_keycodes {
	QWERTY = SAFE_RANGE,
	LOWER = _LOWER,
	RAISE = _RAISE,
	BACKLIT,
	RGB_SU,
	RGB_SD
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

	/* Qwerty
	 * ,-----------------------------------------------------------------------------------.
	 * | Esc  |   1  |   2  |   3  |   4  |   5  |   6  |   7  |   8  |   9  |   0  |  +   |
	 * |------+------+------+------+------+------+------+------+------+------+------+------|
	 * | DEL  |   Q  |   W  |   E  |   R  |   T  |   Y  |   U  |   I  |   O  |   P  |  Å   |
	 * |------+------+------+------+------+-------------+------+------+------+------+------|
	 * | Tab  |   A  |   S  |   D  |   F  |   G  |   H  |   J  |   K  |   L  |   Ö  |  Ä   |
	 * |------+------+------+------+------+------|------+------+------+------+------+------|
	 * | Shift|   Z  |   X  |   C  |   V  |   B  |   N  |   M  |   ,  |   .  |   -  |Shift |
	 * |------+------+------+------+------+------+------+------+------+------+------+------|
	 * | CTRL |  <>  | GUI  | ALT  |Lower |    Space    | BSPC |Raise |MEDIA |   '  |Enter |
	 * `-----------------------------------------------------------------------------------'
	 */
	[_QWERTY] = LAYOUT_preonic_grid(\
	  KC_ESC,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,                KC_0,    KC_MINS, \
	  KC_DEL,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,                KC_P,    KC_LBRC, \
	  KC_TAB,  KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,                KC_SCLN, KC_QUOT, \
	  KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,              KC_SLSH, KC_RSFT, \
	  KC_LCTL, KC_NUBS, KC_LGUI, KC_LALT, LOWER,   KC_SPC,  KC_SPC,  KC_BSPC, RAISE,   LT(_MEDIA, KC_APP),  KC_BSLS, KC_ENT  \
	),

	/* Lower
	 * ,-----------------------------------------------------------------------------------.
	 * |  F1  |  F2  |  F3  |  F4  |  F5  |  F6  |  F7  |  F8  |  F9  |  F10 |  F11 |  F12 |
	 * |------+------+------+------+------+-------------+------+------+------+------+------|
	 * |  F13 |  F14 |  F15 |  F16 |  F17 |      |      |  (   |  )   |      | PGUP |      |
	 * |------+------+------+------+------+-------------+------+------+------+------+------|
	 * |  F9  |  F10 |  F11 |      |      |      |      |  [   |  ]   |      | PGDN |      |
	 * |------+------+------+------+------+------|------+------+------+------+------+------|
	 * |      |      |      |      |      |      |      |  {   |  }   |      |      |      |
	 * |------+------+------+------+------+------+------+------+------+------+------+------|
	 * |      |      |      |      |      |             |  DEL |      |      |      |      |
	 * `-----------------------------------------------------------------------------------'
	 */
	[_LOWER] = LAYOUT_preonic_grid(\
	  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,      KC_F9,      KC_F10,  KC_F11,  KC_F12,  \
	  KC_F13,  KC_F14,  KC_F15,  KC_F16,  KC_F17,  _______, _______, RL(KC_8),   RL(KC_9),   _______, KC_PGUP, _______, \
	  KC_F9,   KC_F10,  KC_F11,  _______, _______, _______, _______, LSFT(KC_8), LSFT(KC_9), _______, KC_PGDN, _______, \
	  _______, _______, _______, _______, _______, _______, _______, RL(KC_7),   RL(KC_0),   _______, _______, _______, \
	  _______, _______, _______, _______, _______, _______, _______, KC_DEL,     _______,    _______, _______, _______  \
	),

	/* Raise
	 * ,-----------------------------------------------------------------------------------.
	 * |  §   |  £   |  @   |   $  |   €  |      |   {  |  [   |   ]  |   }  |  \   |  ?   |
	 * |------+------+------+------+------+------+------+------+------+------+------+------|
	 * |      |      |      |      |      |      |      |      |      | HOME |  UP  | END  |
	 * |------+------+------+------+------+-------------+------+------+------+------+------|
	 * |      |      |      |      |      |      |      |      |      | LEFT | DOWN |RIGHT |
	 * |------+------+------+------+------+------|------+------+------+------+------+------|
	 * |      |      |      |      |      |      |      |   ~  |   ^  |   ´  |      |      |
	 * |------+------+------+------+------+------+------+------+------+------+------+------|
	 * |      |  |   |      |      |      |             |      |      |      |      |      |
	 * `-----------------------------------------------------------------------------------'
	 */
	[_RAISE] = LAYOUT_preonic_grid(\
	  KC_GRV,  RL(KC_3),     RL(KC_2), RL(KC_4), RL(KC_5), _______, RL(KC_7), RL(KC_8),    RL(KC_9),      RL(KC_0), RL(KC_MINS), LSFT(KC_MINS), \
	  _______, _______,      _______,  _______,  _______,  _______, _______,  _______,     _______,       KC_HOME,  KC_UP,       KC_END,  \
	  _______, _______,      _______,  _______,  _______,  _______, _______,  _______,     _______,       KC_LEFT,  KC_DOWN,     KC_RIGHT, \
	  _______, _______,      _______,  _______,  _______,  _______, _______,  RL(KC_RBRC), LSFT(KC_RBRC), KC_EQUAL, _______,     _______, \
	  _______, RL(KC_NUBS),  _______,  _______,  _______,  _______, _______,  _______,     _______,       _______,  _______,     _______  \
	),


	/* Game
	 * ,-----------------------------------------------------------------------------------.
	 * |      |      |      |      |      |      |      |      |      |      |      |      |
	 * |------+------+------+------+------+------+------+------+------+------+------+------|
	 * |      |      |      |      |      |      |      |      |      |      |      |      |
	 * |------+------+------+------+------+-------------+------+------+------+------+------|
	 * |      |      |      |      |      |      |      |      |      |      |  UP  |      |
	 * |------+------+------+------+------+------|------+------+------+------+------+------|
	 * |      |      |      |      |      |      |      |      |      | LEFT | DOWN |RIGHT |
	 * |------+------+------+------+------+------+------+------+------+------+------+------|
	 * |      |  |   |      |      |      |             |      |      |      |      |      |
	 * `-----------------------------------------------------------------------------------'
	 */
	[_GAME] = LAYOUT_preonic_grid(\
	  KC_ESC,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,                KC_0,    KC_MINS, \
	  KC_DEL,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,                KC_P,    KC_LBRC, \
	  KC_TAB,  KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,                KC_UP,   KC_QUOT, \
	  KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_LEFT,             KC_DOWN, KC_RIGHT,  \
	  KC_LCTL, KC_NUBS, KC_LGUI, KC_LALT, LOWER,   KC_SPC,  KC_SPC,  KC_BSPC, RAISE,   LT(_MEDIA, KC_APP),  KC_BSLS, KC_ENT  \
	),


	/* Media
	 * ,-----------------------------------------------------------------------------------.
	 * |      |      | NUM/ | NUM* | NUM- |      |      |      |      |      |      |VOL_UP|
	 * |------+------+------+------+------+------+------+------+------+------+------+------|
	 * |      | NUM7 | NUM8 | NUM9 | NUM+ |      |      |      |      | PREV | NEXT |VOL_DN|
	 * |------+------+------+------+------+-------------+------+------+------+------+------|
	 * |      | NUM4 | NUM5 | NUM6 | NUM+ |      |      |      |      | STOP | PLAY | MUTE |
	 * |------+------+------+------+------+------|------+------+------+------+------+------|
	 * |      | NUM1 | NUM2 | NUM3 | NUMR |      |      |      |      |      |      | PAUSE|
	 * |------+------+------+------+------+------+------+------+------+------+------+------|
	 * |      | NUM0 | NUM, |      |      |             |      |      |      |      | PSCR |
	 * `-----------------------------------------------------------------------------------'
	 */
	[_MEDIA] = LAYOUT_preonic_grid(\
	  _______, _______, KC_NUBS, KC_PAST, KC_PMNS, RGB_M_P, RGB_M_B, RGB_M_R, RGB_TOG, _______, _______, KC_VOLU, \
	  _______, KC_P7  , KC_P8  , KC_P9  , KC_PPLS, RGB_SPI,  RGB_HUI, RGB_SAI, RGB_VAI, KC_MPRV, KC_MNXT, KC_VOLD,  \
	  _______, KC_P4  , KC_P5  , KC_P6  , KC_PPLS, RGB_SPD,  RGB_HUD, RGB_SAD, RGB_VAD, KC_MSTP, KC_MPLY, KC_MUTE, \
	  _______, KC_P1  , KC_P2  , KC_P3  , KC_PENT, RGB_M_SW,RGB_M_SN,RGB_RMOD, RGB_MOD, _______, _______, KC_PAUS, \
	  _______, KC_P0  , KC_PDOT, _______, _______, _______, _______, _______, _______, _______, _______, KC_PSCR  \
	),

	/* Adjust (Lower + Raise)
	 * ,-----------------------------------------------------------------------------------.
	 * |      |      |      |      |      |      |      |      |      |      |      |      |
	 * |------+------+------+------+------+------+------+------+------+------+------+------|
	 * | CAPS | Reset|      |      |      |      |      |      |      |      |      |  Del |
	 * |------+------+------+------+------+-------------+------+------+------+------+------|
	 * |      |      |      |Aud on|AudOff|AGnorm|AGswap|      |      |      |      |      |
	 * |------+------+------+------+------+------|------+------+------+------+------+------|
	 * |      |Voice-|Voice+|Mus on|MusOff|MidiOn|MidOff|      |qwerty| game |      |      |
	 * |------+------+------+------+------+------+------+------+------+------+------+------|
	 * |      |      |      |      |      |             |      |      |      |      |      |
	 * `-----------------------------------------------------------------------------------'
	 */
	[_ADJUST] = LAYOUT_preonic_grid(\
	  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,   _______,   _______,  \
	  KC_CAPS, RESET,   DEBUG,   _______, _______, _______, _______, TERM_ON, TERM_OFF,_______,   _______,   KC_DEL,  \
	  _______, _______, MU_MOD,  AU_ON,   AU_OFF,  AG_NORM, AG_SWAP, _______, _______, _______,   _______,   _______, \
	  _______, MUV_DE,  MUV_IN,  MU_ON,   MU_OFF,  MI_ON,   MI_OFF,  _______, QWERTY,  DF(_GAME), _______,   _______, \
	  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,   _______,   _______  \
	)


};


bool LOWERCombo = false;
bool LOWERDown = false;
uint16_t LOWERTimer = 0;
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
	switch (keycode) {
	case QWERTY:
		if (record->event.pressed) {
			set_single_persistent_default_layer(_QWERTY);
		}
		return false;
		break;
	case LOWER:
		if (record->event.pressed) {
			layer_on(_LOWER);
			update_tri_layer(_LOWER, _RAISE, _ADJUST);
			LOWERCombo = false;
			LOWERDown = true;
			LOWERTimer = timer_read();
		}
		else {
			layer_off(_LOWER);
			update_tri_layer(_LOWER, _RAISE, _ADJUST);
			if (LOWERCombo == false && timer_elapsed(LOWERTimer) < TAPPING_TERM)
				SEND_STRING(SS_TAP(X_ENTER));
			LOWERDown = false;
			LOWERCombo = false;
		}
		return false;
		break;
	case RAISE:
		if (record->event.pressed) {
			layer_on(_RAISE);
			update_tri_layer(_LOWER, _RAISE, _ADJUST);
		}
		else {
			layer_off(_RAISE);
			update_tri_layer(_LOWER, _RAISE, _ADJUST);
		}
		LOWERCombo = true;
		return false;
		break;
	case BACKLIT:
		if (record->event.pressed) {
			register_code(KC_RSFT);
#ifdef BACKLIGHT_ENABLE
			backlight_step();
#endif
#ifdef __AVR__
			PORTE &= ~(1 << 6);
#endif
		}
		else {
			unregister_code(KC_RSFT);
#ifdef __AVR__
			PORTE |= (1 << 6);
#endif
		}
		return false;
		break;
	default:
		if (LOWERDown && record->event.pressed)
		  LOWERCombo = true;
		break;
	}
	return true;
};

bool muse_mode = false;
uint8_t last_muse_note = 0;
uint16_t muse_counter = 0;
uint8_t muse_offset = 70;
uint16_t muse_tempo = 50;

void encoder_update_user(uint8_t index, bool clockwise) {
	if (muse_mode) {
		if (IS_LAYER_ON(_RAISE)) {
			if (clockwise) {
				muse_offset++;
			}
			else {
				muse_offset--;
			}
		}
		else {
			if (clockwise) {
				muse_tempo += 1;
			}
			else {
				muse_tempo -= 1;
			}
		}
	}
	else {
		if (clockwise) {
			register_code(KC_PGDN);
			unregister_code(KC_PGDN);
		}
		else {
			register_code(KC_PGUP);
			unregister_code(KC_PGUP);
		}
	}
}

void dip_update(uint8_t index, bool active) {
	switch (index) {
	case 0:
		if (active) {
			layer_on(_ADJUST);
		}
		else {
			layer_off(_ADJUST);
		}
		break;
	case 1:
		if (active) {
			muse_mode = true;
		}
		else {
			muse_mode = false;
#ifdef AUDIO_ENABLE
			stop_all_notes();
#endif
		}
	}
}

void matrix_scan_user(void) {
#ifdef AUDIO_ENABLE
	if (muse_mode) {
		if (muse_counter == 0) {
			uint8_t muse_note = muse_offset + SCALE[muse_clock_pulse()];
			if (muse_note != last_muse_note) {
				stop_note(compute_freq_for_midi_note(last_muse_note));
				play_note(compute_freq_for_midi_note(muse_note), 0xF);
				last_muse_note = muse_note;
			}
		}
		muse_counter = (muse_counter + 1) % muse_tempo;
	}
#endif
}

bool music_mask_user(uint16_t keycode) {
	switch (keycode) {
	case RAISE:
	case LOWER:
		return false;
	default:
		return true;
	}
}



uint8_t led_config[] =
	{ 0x15,
	  0x06, 0xff, 0x05, 0xff, 0x04, 0xff, 0x03,
	  0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff,
	  0x07, 0xff, 0x08, 0xff, 0x01, 0xff, 0x02 };

void raw_hid_receive(uint8_t *data, uint8_t length) {

	switch (data[0]) {
		case 1:
			raw_hid_send(led_config, length);
			break;
		case 2:
			{
				uint8_t count = data[1];
				uint8_t* leds = data + 2;

				for (uint8_t i = 0; i <= count; i++) {
					//uint8_t ii = i * 4;

					uint8_t led_idx = leds[0] - 1;
					leds++;
					uint8_t r = leds[0];
					leds++;
					uint8_t g = leds[0];
					leds++;
					uint8_t b = leds[0];
					leds++;

					// Disabled for now
					//if (led_idx == 0x00) {
					//	continue;
					//}

					aurora_colors[led_idx].r = r;
					aurora_colors[led_idx].g = g;
					aurora_colors[led_idx].b = b;

					//rgb_matrix_set_color(led_idx, r, g, b);
					//setrgb(r, g, b, (LED_TYPE *)&led[led_idx]);
				}

				//rgblight_set();
			}
			break;
		default:
			raw_hid_send(data, length);
			break;
	}
}
