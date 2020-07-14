#include QMK_KEYBOARD_H
#include "bitmaps.h"
#include "glcdfont.c"

enum layers {
  _DEFAULT,
  _NUMRAISE,
  _MEDIA,

  _ADJUST
};

enum keycodes {
  DEFAULT = SAFE_RANGE,
  LAYER_NEXT,
  LOGO_TOGGLE
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

[_DEFAULT] = LAYOUT_grid( \
  KC_PPLS, KC_PMNS,     MO(_NUMRAISE),
  KC_P7,   KC_P8,       KC_P9,
  KC_P4,   KC_P5,       KC_P6, LT(_ADJUST, KC_MUTE),
  KC_P1,   KC_P2,       KC_P3,
  KC_P0,   KC_PDOT,     KC_PENT
),

[_NUMRAISE] = LAYOUT_grid( \
  KC_PAST, KC_PSLS,  _______,
  _______, _______,  _______,
  _______, _______,  _______, _______,
  _______, _______,  _______,
  KC_NLCK, KC_CAPS,  KC_SLCK
),

[_MEDIA] = LAYOUT_grid( \
  KC_MPRV, KC_MPLY,   KC_MNXT,
  _______, KC_MSTP,   _______,
  KC_WBAK, KC_WFWD,   KC_WREF, _______,
  _______, _______,   _______,
  KC_PSCR, KC_PAUSE,  _______
),

[_ADJUST] = LAYOUT_grid(\
  LAYER_NEXT,     _______, _______,
  _______,        _______, _______,
  _______,        _______, _______, _______,
  _______,        _______, _______,
  LOGO_TOGGLE,    _______, RESET
)

};

bool showLogo = false;

void keyboard_post_init_user(void) {
    oled_init(OLED_ROTATION_90);
    oled_clear();
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case DEFAULT:
            if (record->event.pressed) {
                set_single_persistent_default_layer(_DEFAULT);
            }
            return false;

        case LAYER_NEXT:
            if (record->event.pressed) {
                showLogo = false;
                switch (biton32(default_layer_state)) {
                    case _MEDIA:
                        set_single_persistent_default_layer(_DEFAULT);
                        break;
                    case _DEFAULT:
                        set_single_persistent_default_layer(_MEDIA);
                        break;
                }
            }
            return false;
            break;
        case LOGO_TOGGLE:
            if (record->event.pressed) {
                oled_clear();
                showLogo = !showLogo;
            }
            return false;
            break;
    }

    return true;
}

void encoder_update_user(uint8_t index, bool clockwise) {
    oled_on();
    switch (biton32(default_layer_state)) {
        case _MEDIA:
        case _DEFAULT:
        default:
            if (clockwise) {
                tap_code(KC_VOLU);
            } else {
                tap_code(KC_VOLD);
            }
        break;
    }
}


//#ifdef OLED_DRIVER_ENABLE

oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    return OLED_ROTATION_90;
}

//static void add_bitmap_to_buf(char* buf, const char* bitmap)
//{
//    int bitmapSize = sizeof(bitmap) / sizeof(char);
//    for (int i = MIN(bitmapSize, oled_buf_size); i-- > 0;) {
//        buf[i] = buf[i] | bitmap[i];
//    }
//}


void render_char(const char c, const char* out, int x, int y)
{
    uint8_t cast_data = (uint8_t)c;  // font based on unsigned type for index
    const uint8_t *glyph = &font[(cast_data - OLED_FONT_START) * OLED_FONT_WIDTH];
    memcpy_P((void*) (out + x + (y * 32)), glyph, OLED_FONT_WIDTH);
}

void render_string(const char *data, const char* out, int x, int y) {
    const char *end = data + strlen(data);
    while (data < end) {
        render_char(*data, out, x, y);
        x += OLED_FONT_WIDTH;
        data++;
    }
}

void render_string_P(const char *data, const char* out, int x, int y) {
    uint8_t c = pgm_read_byte(data);
    while (c != 0) {
        render_char(c, out, x, y);
        x += OLED_FONT_WIDTH;
        c = pgm_read_byte(++data);
    }
}

void oled_task_user(void) {
    oled_set_cursor(0, 0);

    led_t led_state = host_keyboard_led_state();

    char bitmap[512];
    memset((void*) bitmap, 0x00, 512);

    for (int i = sizeof(bitmap_bg); i-- > 0;) {
        bitmap[i] |= pgm_read_byte_near(bitmap_bg + i);
    }

    if (led_state.num_lock) {
        for (int i = sizeof(bitmap_num_lock); i-- > 0;) {
            const char mask = pgm_read_byte_near(bitmap_num_lock_mask + i);
            bitmap[i + 448] = (pgm_read_byte_near(bitmap_num_lock + i) & mask) | (bitmap[i + 448] & ~mask);
        }
    }

    if (led_state.caps_lock) {
        for (int i = sizeof(bitmap_caps_lock); i-- > 0;) {
            const char mask = pgm_read_byte_near(bitmap_caps_lock_mask + i);
            bitmap[i + 448] = (pgm_read_byte_near(bitmap_caps_lock + i) & mask) | (bitmap[i + 448] & ~mask);
        }
    }


    int y = 1;
    render_string_P(PSTR("Layer"), bitmap, 0, y++);

    switch (get_highest_layer(default_layer_state)) {
        case _DEFAULT:
            render_string_P(PSTR(">Def"), bitmap, 0, y++);
            render_string_P(PSTR(" Mus"), bitmap, 0, y++);
            break;
        case _MEDIA:
            render_string_P(PSTR(" Def"), bitmap, 0, y++);
            render_string_P(PSTR(">Mus"), bitmap, 0, y++);
            break;
        default:
            render_string_P(PSTR("Undef"), bitmap, 0, y++);
    }

    oled_write_raw(bitmap, sizeof(bitmap));

    //oled_render();

    // Host Keyboard LED Status

    //oled_set_cursor(0, oled_max_lines() - 4);
    //oled_write_P(led_state.num_lock ? PSTR("NUM\n") : PSTR("\n"), false);
    //oled_write_P(led_state.caps_lock ? PSTR("CAP\n") : PSTR("\n"), false);
    //oled_write_P(led_state.scroll_lock ? PSTR("SCR\n") : PSTR("\n"), false);
}
//#endif
