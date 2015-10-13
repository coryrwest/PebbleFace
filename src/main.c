// 144 x 168 pixels
#include <pebble.h>
#include <time.c>
#include <date.c>
#include <battery.c>
#include <weather.c>
static Window *s_main_window;
static TextLayer *s_time_layer;
static TextLayer *s_day_layer;
static TextLayer *s_date_layer;
static TextLayer *s_charge_state;
static TextLayer *s_bt_state;
static TextLayer *s_weather;
static GFont s_font;
static GFont s_time_font;
static Layer *s_canvas_layer;

static void bt_handler(bool connected) {
  // Cancel vibration
  vibes_cancel();
  // Show current connection state
  if (connected) {
    text_layer_set_text(s_bt_state, "B");
  } else {
    text_layer_set_text(s_bt_state, "-");
    vibes_short_pulse();
  }
}

static void canvas_update_proc(Layer *this_layer, GContext *ctx) {
  // Draw things here using ctx
//   GPoint p0 = GPoint(45, 50);
//   graphics_context_set_fill_color(ctx, (GColor8){ 
//     .a = 0b10,
//     .r = 0b00, 
//     .g = 0b01, 
//     .b = 0b10 
//   });
//   graphics_fill_circle(ctx, p0, 25);
  
//   GPoint p1 = GPoint(55, 80);
//   graphics_context_set_fill_color(ctx, (GColor8){ 
//     .a = 0b10,
//     .r = 0b00, 
//     .g = 0b01, 
//     .b = 0b11 
//   });
//   graphics_fill_circle(ctx, p1, 20);
  
//   GPoint p2 = GPoint(100, 105);
//   graphics_context_set_fill_color(ctx, (GColor8){ 
//     .a = 0b11,
//     .r = 0b00, 
//     .g = 0b11, 
//     .b = 0b11 
//   });
//   graphics_fill_circle(ctx, p2, 10);
  
//   GPoint p3 = GPoint(40, 125);
//   graphics_context_set_fill_color(ctx, (GColor8){ 
//     .a = 0b00,
//     .r = 0b10, 
//     .g = 0b01, 
//     .b = 0b11 
//   });
//   graphics_fill_circle(ctx, p3, 10);
}

static void main_window_load(Window *window) {
  // GRect(X, Y, Width, Height)
  
  // Set font
  s_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_TREK_TNG_24));
  s_time_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_TREK_TNG_70));
  // Set window background color
  window_set_background_color(window, GColorBlack);
  
  
  // GRAPHICS
  s_canvas_layer = layer_create(GRect(0, 0, 144, 168));
  layer_set_update_proc(s_canvas_layer, canvas_update_proc);
  layer_add_child(window_get_root_layer(window), s_canvas_layer);
  
  
  // --------------
  // ---- Time ----
  // --------------
  // Create time TextLayer
  s_time_layer = text_layer_create(GRect(0, 35, 144, 70));
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_text_color(s_time_layer, GColorPictonBlue);
  
  // Make sure the time is displayed from the start
  update_time(s_time_layer);

  // Improve the layout to be more like a watchface
  text_layer_set_font(s_time_layer, s_time_font);
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);

  // Add it as a child layer to the Window's root layer
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer));
  
  // ---------------------
  // ---- Day and Date----
  // ---------------------
  // Create day TextLayer
  s_day_layer = text_layer_create(GRect(0, 110, 144, 28));
  text_layer_set_background_color(s_day_layer, GColorClear);
  text_layer_set_text_color(s_day_layer, GColorPictonBlue);
  // Create date TextLayer
  s_date_layer = text_layer_create(GRect(0, 140, 144, 28));
  text_layer_set_background_color(s_date_layer, GColorClear);
  text_layer_set_text_color(s_date_layer, GColorPictonBlue);

  // Improve the layout to be more like a watchface
  text_layer_set_font(s_day_layer, s_font);
  text_layer_set_text_alignment(s_day_layer, GTextAlignmentCenter);
  text_layer_set_font(s_date_layer, s_font);
  text_layer_set_text_alignment(s_date_layer, GTextAlignmentCenter);
  
  // Make sure the date is displayed from the start
  update_date(s_day_layer, s_date_layer);

  // Add it as a child layer to the Window's root layer
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_day_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_date_layer));
  
  // -----------------
  // ---- Battery ----
  // -----------------
  // Create battery TextLayer
  s_charge_state = text_layer_create(GRect(5, 0, 55, 26));
  text_layer_set_background_color(s_charge_state, GColorClear);
  text_layer_set_text_color(s_charge_state, GColorLightGray);
  
  // Make sure the time is displayed from the start
  update_battery(s_charge_state);

  // Improve the layout to be more like a watchface
  text_layer_set_font(s_charge_state, s_font);
  text_layer_set_text_alignment(s_charge_state, GTextAlignmentLeft);

  // Add it as a child layer to the Window's root layer
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_charge_state));
  
  // -------------------
  // ---- Bluetooth ----
  // -------------------
  // Create bt TextLayer
  s_bt_state = text_layer_create(GRect(105, 0, 30, 26));
  text_layer_set_background_color(s_bt_state, GColorClear);
  text_layer_set_text_color(s_bt_state, GColorLightGray);
  
  // Make sure the status is displayed from the start
  bt_handler(bluetooth_connection_service_peek());

  // Improve the layout to be more like a watchface
  text_layer_set_font(s_bt_state, s_font);
  text_layer_set_text_alignment(s_bt_state, GTextAlignmentRight);

  // Add it as a child layer to the Window's root layer
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_bt_state));
  
  // -----------------
  // ---- Weather ----
  // -----------------
  // Create time TextLayer
  s_weather = text_layer_create(GRect(50, 0, 60, 26));
  text_layer_set_background_color(s_weather, GColorClear);
  text_layer_set_text_color(s_weather, GColorLightGray);
  
  // Make sure the weather is displayed from the start
  text_layer_set_text(s_weather, "L...");
  init_weather(s_weather);

  // Improve the layout to be more like a watchface
  text_layer_set_font(s_weather, s_font);
  text_layer_set_text_alignment(s_weather, GTextAlignmentCenter);

  // Add it as a child layer to the Window's root layer
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_weather));
}

static void main_window_unload(Window *window) {
  // Destroy everything
  text_layer_destroy(s_time_layer);
  text_layer_destroy(s_day_layer);
  text_layer_destroy(s_date_layer);
  text_layer_destroy(s_charge_state);
  text_layer_destroy(s_bt_state);
  text_layer_destroy(s_weather);
  fonts_unload_custom_font(s_font);
  fonts_unload_custom_font(s_time_font);
  layer_destroy(s_canvas_layer);
}

static void time_handler(struct tm *tick_time, TimeUnits units_changed) {  
  if( (units_changed & MINUTE_UNIT) != 0 ) {
    /* Minutes changed */
    update_time(s_time_layer);
    update_battery(s_charge_state);
  }
  if( (units_changed & DAY_UNIT) != 0 ) {
    /* Days changed */
    update_date(s_day_layer, s_date_layer);
  }
  
  // Get weather update every 30 minutes
  if(tick_time->tm_min % 30 == 0) {
    // Begin dictionary
    DictionaryIterator *iter;
    app_message_outbox_begin(&iter);
  
    // Add a key-value pair
    dict_write_uint8(iter, 0, 0);
  
    // Send the message!
    app_message_outbox_send();
  }
}

static void init() {
  // Create main Window element and assign to pointer
  s_main_window = window_create();

  // Set handlers to manage the elements inside the Window
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });

  // Show the Window on the watch, with animated=true
  window_stack_push(s_main_window, true);
  
  // Register with TickTimerService
  tick_timer_service_subscribe(MINUTE_UNIT | DAY_UNIT, time_handler);
  // Subscribe to Bluetooth updates
  bluetooth_connection_service_subscribe(bt_handler);
  
  // WEATHER
  // Register callbacks
  app_message_register_inbox_received(inbox_received_callback);
  app_message_register_inbox_dropped(inbox_dropped_callback);
  app_message_register_outbox_failed(outbox_failed_callback);
  app_message_register_outbox_sent(outbox_sent_callback);
  // Open AppMessage
  app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
}

static void deinit() {
  // Destroy Window
  window_destroy(s_main_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}