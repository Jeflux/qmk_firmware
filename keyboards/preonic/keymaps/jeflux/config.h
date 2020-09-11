#ifndef CONFIG_USER_H
#define CONFIG_USER_H

#include "config_common.h"

#ifdef AUDIO_ENABLE
    // #define STARTUP_SONG SONG(PREONIC_SOUND)
    #define STARTUP_SONG SONG(NO_SOUND)

    //#define DEFAULT_LAYER_SONGS { SONG(QWERTY_SOUND), 
    //                              SONG(COLEMAK_SOUND), 
    //                              SONG(DVORAK_SOUND) 
    //                            }
#endif

#define TAPPING_TERM 160

#undef DEBOUNCE
#define DEBOUNCE 6

//#define MUSIC_MASK (keycode != KC_NO)

/*
 * MIDI options
 */

/* Prevent use of disabled MIDI features in the keymap */
//#define MIDI_ENABLE_STRICT 1

/* enable basic MIDI features:
   - MIDI notes can be sent when in Music mode is on
*/

//#define MIDI_BASIC

/* enable advanced MIDI features:
   - MIDI notes can be added to the keymap
   - Octave shift and transpose
   - Virtual sustain, portamento, and modulation wheel
   - etc.
*/
//#define MIDI_ADVANCED

/* override number of MIDI tone keycodes (each octave adds 12 keycodes and allocates 12 bytes) */
//#define MIDI_TONE_KEYCODE_OCTAVES 2

//#define DISABLE_RGB_MATRIX_SOLID_REACTIVE_SIMPLE
//#define DISABLE_RGB_MATRIX_SOLID_REACTIVE_WIDE
//#define DISABLE_RGB_MATRIX_SOLID_REACTIVE_MULTIWIDE
//#define DISABLE_RGB_MATRIX_SOLID_REACTIVE_CROSS
//#define DISABLE_RGB_MATRIX_SOLID_REACTIVE_MULTICROSS
//#define DISABLE_RGB_MATRIX_SOLID_REACTIVE_NEXUS
//#define DISABLE_RGB_MATRIX_SOLID_REACTIVE_MULTINEXUS
//#define DISABLE_RGB_MATRIX_SPLASH
//#define DISABLE_RGB_MATRIX_MULTISPLASH
//#define DISABLE_RGB_MATRIX_SOLID_SPLASH
//#define DISABLE_RGB_MATRIX_SOLID_MULTISPLASH


#define RGB_DI_PIN A1
#define RGBLED_NUM 9
#define RGBLIGHT_ANIMATIONS
#define DRIVER_LED_TOTAL RGBLED_NUM

#endif
