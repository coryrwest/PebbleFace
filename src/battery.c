#include <pebble.h>
static void update_battery(TextLayer *s_charge_state) {
  BatteryChargeState state = battery_state_service_peek();
  static char charge[5];
  snprintf(charge, sizeof(charge), "%d%%", state.charge_percent);
  
  text_layer_set_text(s_charge_state, charge);
}