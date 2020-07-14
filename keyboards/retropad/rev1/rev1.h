#pragma once

#include "retropad.h"


#define LAYOUT_grid( \
	k00, k01, k02, \
	k10, k11, k12, \
	k20, k21, k22, k23,   \
	k30, k31, k32, \
	k40, k41, k42 \
	) \
	{ \
	{ k00, k01, k02, KC_NO }, \
	{ k10, k11, k12, KC_NO }, \
	{ k20, k21, k22, k23   }, \
	{ k30, k31, k32, KC_NO }, \
	{ k40, k41, k42, KC_NO } \
	}
