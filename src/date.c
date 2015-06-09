#include <pebble.h>
static void update_date(TextLayer *s_day_layer, TextLayer* s_date_layer) {
  // Get a tm structure
  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);

  // Create a long-lived buffer
  static char day[] = "Wednesday";
  static char date[] = "Jan 01 2015";

  // Write the current day into the buffer
  strftime(day, sizeof("Wednesday"), "%A", tick_time);
  strftime(date, sizeof("Jan 01 2015"), "%b %d %Y", tick_time);

  // Display this time on the TextLayer
  text_layer_set_text(s_day_layer, day);
  text_layer_set_text(s_date_layer, date);
}