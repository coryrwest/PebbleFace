#include <pebble.h>
static TextLayer *s_weather_layer;
#define KEY_TEMPERATURE 0

static void inbox_received_callback(DictionaryIterator *iterator, void *context) {
  // Store incoming information
  static char temperature_buffer[8];
  //static char conditions_buffer[32];
  static char weather_layer_buffer[32];
  
  // Read first item
  Tuple *t = dict_read_first(iterator);

  // For all items
  while(t != NULL) {
    // Which key was received?
    switch(t->key) {
    case KEY_TEMPERATURE:
      snprintf(temperature_buffer, sizeof(temperature_buffer), "%dF", (int)t->value->int32);
      break;
//     case KEY_CONDITIONS:

//       break;
    default:
      APP_LOG(APP_LOG_LEVEL_ERROR, "Key %d not recognized!", (int)t->key);
      break;
    }

    // Look for next item
    t = dict_read_next(iterator);
  }
  
  snprintf(weather_layer_buffer, sizeof(weather_layer_buffer), "%s", temperature_buffer);
  text_layer_set_text(s_weather_layer, weather_layer_buffer);
}
  
static void init_weather(TextLayer *s_weather) {
  s_weather_layer = s_weather;
}


static void inbox_dropped_callback(AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Message dropped!");
}

static void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Outbox send failed!");
}

static void outbox_sent_callback(DictionaryIterator *iterator, void *context) {
  APP_LOG(APP_LOG_LEVEL_INFO, "Outbox send success!");
}