/*
 * Demonstrate how to display a two color, black and white bitmap image with no
 * transparency.
 *
 * The original source image is from:
 *  <http://openclipart.org/detail/26728/aiga-litter-disposal-by-anonymous>
 *
 * The source image was converted from an SVG into a RGB bitmap using Inkscape.
 * It has no transparency and uses only black and white as colors.
 */

#include "pebble.h"

static Window *s_main_window;
GBitmap *battery_icon;
BitmapLayer *battery_icon_layer;
TextLayer *battery_layer;


static void handle_battery(BatteryChargeState charge_state) {
  	
  	static char battery_text[] = "100%";
	
  	battery_icon = gbitmap_create_with_resource(RESOURCE_ID_BATTERY_FULL);
  
  	bitmap_layer_set_bitmap(battery_icon_layer, battery_icon);
	snprintf(battery_text, sizeof(battery_text), " %d%%", charge_state.charge_percent);
	text_layer_set_text(battery_layer, battery_text);
}

void tick_handler(struct tm *tick_time, TimeUnits units_changed)
{    					
	handle_battery(battery_state_service_peek());
}

static void main_window_load(Window *window) {
  	Layer *window_layer = window_get_root_layer(window);
  	GRect bounds = layer_get_frame(window_layer);

  	battery_icon_layer = bitmap_layer_create(bounds);
	
  	battery_layer = text_layer_create(GRect(105, 0, /* width */ 50,20 /* height */));
  	text_layer_set_background_color(battery_layer, GColorClear);	
  
 	text_layer_set_text(battery_layer, "100%");	
	
	layer_add_child(window_layer, (Layer*) battery_layer);
  	layer_add_child(window_layer, bitmap_layer_get_layer(battery_icon_layer)); 
  	time_t now = time(NULL);
  	struct tm *current_time = localtime(&now);
  	tick_handler(current_time, SECOND_UNIT | HOUR_UNIT | DAY_UNIT | MONTH_UNIT | YEAR_UNIT);
	//Subscribe to the tick event
  	tick_timer_service_subscribe(SECOND_UNIT | HOUR_UNIT | DAY_UNIT | MONTH_UNIT | YEAR_UNIT, (TickHandler) &tick_handler);
  	battery_state_service_subscribe(&handle_battery);
}

static void main_window_unload(Window *window) {
  tick_timer_service_unsubscribe();
  gbitmap_destroy(battery_icon);
  bitmap_layer_destroy(battery_icon_layer);
  battery_state_service_unsubscribe();
}

static void init() {
  s_main_window = window_create();
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload,
  });
  window_stack_push(s_main_window, true);
}

static void deinit() {
  window_destroy(s_main_window);
	
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
