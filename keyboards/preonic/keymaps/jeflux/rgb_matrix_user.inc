RGB_MATRIX_EFFECT(RGB_MATRIX_CHRISTMAS)
RGB_MATRIX_EFFECT(RGB_MATRIX_MIAMI)

#ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

//---------------------------------------------------
// Helper functions
//---------------------------------------------------
#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))
#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))

uint8_t lerp(uint8_t a, uint8_t b, double t) {
  return round(a + t * (b - a));
}

HSV rgb_to_hsv(RGB rgb)
{
  HSV hsv;
  uint8_t rgbMin, rgbMax;

  rgbMin = rgb.r < rgb.g ? (rgb.r < rgb.b ? rgb.r : rgb.b) : (rgb.g < rgb.b ? rgb.g : rgb.b);
  rgbMax = rgb.r > rgb.g ? (rgb.r > rgb.b ? rgb.r : rgb.b) : (rgb.g > rgb.b ? rgb.g : rgb.b);

  hsv.v = rgbMax;
  if (hsv.v == 0)
  {
	hsv.h = 0;
	hsv.s = 0;
	return hsv;
  }

  hsv.s = 255 * (rgbMax - rgbMin) / hsv.v;
  if (hsv.s == 0)
  {
	hsv.h = 0;
	return hsv;
  }

  if (rgbMax == rgb.r)
  hsv.h = 0 + 43 * (rgb.g - rgb.b) / (rgbMax - rgbMin);
  else if (rgbMax == rgb.g)
  hsv.h = 85 + 43 * (rgb.b - rgb.r) / (rgbMax - rgbMin);
  else
  hsv.h = 171 + 43 * (rgb.r - rgb.g) / (rgbMax - rgbMin);

  return hsv;
}
//---------------------------------------------------


static HSV RGB_MATRIX_CHRISTMAS_math(HSV hsv, uint8_t i, uint8_t time) {
	uint8_t offset = time >> 6;
	hsv.h = (offset + (g_led_config.point[i].x >> 4)) % 2 == 0 ? 80 : 0;
    return hsv;
}

bool RGB_MATRIX_CHRISTMAS(effect_params_t* params) { return effect_runner_i(params, &RGB_MATRIX_CHRISTMAS_math); }


const uint8_t miami_colors_size = 5;
const RGB miami_colors[5] = { 
	// { .r = 39 , .g = 253, .b = 245 }, // Cyan
	// { .r = 168, .g = 246, .b = 248 }, // Light cyan
	// { .r = 215, .g = 255, .b = 254 }, // Cyan white
	// { .r = 249, .g = 141, .b = 201 }, // Light pink
	// { .r = 248, .g = 77 , .b = 174 }, // Pink
	
	{ .r = 70 , .g = 253, .b = 170 }, // Cyan
	{ .r = 39 , .g = 253, .b = 245 }, // Cyan
	{ .r = 215, .g = 255, .b = 254 }, // Cyan white
	{ .r = 248, .g = 77 , .b = 174 }, // Pink
	{ .r = 248, .g = 87 , .b = 144 }, // Pink
};

bool RGB_MATRIX_MIAMI(effect_params_t* params) {
  double lerpVal = (sin(g_rgb_counters.tick * 3.141592 / 1600) * 0.5) + 0.5;

  // Top
  RGB t_rgb = {
	.r = lerp(miami_colors[0].r, miami_colors[1].r, lerpVal),
	.g = lerp(miami_colors[0].g, miami_colors[1].g, lerpVal),
	.b = lerp(miami_colors[0].b, miami_colors[1].b, lerpVal)
  };

  // Bottom
  RGB b_rgb = {
	.r = lerp(miami_colors[4].r, miami_colors[3].r, lerpVal),
	.g = lerp(miami_colors[4].g, miami_colors[3].g, lerpVal),
	.b = lerp(miami_colors[4].b, miami_colors[3].b, lerpVal)
  };

  double vInv = (double) rgb_matrix_config.hsv.v / (double) 0xFF;
  double sInv = (double) rgb_matrix_config.hsv.s / (double) 0xFF;

  HSV t_hsv = rgb_to_hsv(t_rgb);
  t_hsv.v *= vInv;
  t_hsv.s *= sInv;

  HSV b_hsv = rgb_to_hsv(b_rgb);
  b_hsv.v *= vInv;
  b_hsv.s *= sInv;

  RGB top_rgb = hsv_to_rgb(t_hsv);
  RGB bot_rgb = hsv_to_rgb(b_hsv);

  RGB_MATRIX_USE_LIMITS(led_min, led_max);
  for (int i = led_min; i < led_max; i++)
  {
	double yLerpVal = MAX(0.0, MIN(1.0, g_led_config.point[i].y / 50.0));
	uint8_t r = lerp(top_rgb.r, bot_rgb.r, yLerpVal);
	uint8_t g = lerp(top_rgb.g, bot_rgb.g, yLerpVal);
	uint8_t b = lerp(top_rgb.b, bot_rgb.b, yLerpVal);

	rgb_matrix_set_color(i, r, g, b);
  }

  return led_max < DRIVER_LED_TOTAL;
}




// e.g: A simple effect, self-contained within a single method
// static bool my_cool_effect(effect_params_t* params) {
//   RGB_MATRIX_USE_LIMITS(led_min, led_max);
//   for (uint8_t i = led_min; i < led_max; i++) {
//     rgb_matrix_set_color(i, 0xff, 0xff, 0x00);
//   }
//   return led_max < DRIVER_LED_TOTAL;
// }

// e.g: A more complex effect, relying on external methods and state, with
// dedicated init and run methods
// static uint8_t some_global_state;
// static void my_cool_effect2_complex_init(effect_params_t* params) {
//   some_global_state = 1;
// }
// static bool my_cool_effect2_complex_run(effect_params_t* params) {
//   RGB_MATRIX_USE_LIMITS(led_min, led_max);
//   for (uint8_t i = led_min; i < led_max; i++) {
//     rgb_matrix_set_color(i, 0xff, some_global_state++, 0xff);
//   }
// 
//   return led_max < DRIVER_LED_TOTAL;
// }
// static bool my_cool_effect2(effect_params_t* params) {
//   if (params->init) my_cool_effect2_complex_init(params);
//   return my_cool_effect2_complex_run(params);
// }

#endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS
