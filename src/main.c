#include <pebble.h>

// Main Window
static Window *s_main_window;
static Layer *s_main_window_layer;

// TextLayer to hold the day
static TextLayer *s_dayname_layer;

// TextLayer to hold the hours
static TextLayer *s_hours_layer;

// TextLayer to hold the separator
static TextLayer *s_separator_layer;

// TextLayer to hold the minutes
static TextLayer *s_minutes_layer;

// TextLayer to hold the AMPM
static TextLayer *s_ampm_layer;

// TextLayer to hold the date
static TextLayer *s_date_layer;

// TextLayer to hold the month
static TextLayer *s_month_layer;

// Fonts
static GFont s_dayname_font;
static GFont s_hours_font;
static GFont s_minutes_font;
static GFont s_date_font;


// Days of the week
static char *daynames[7] = {
  "SUNDAY", 
  "MONDAY",
  "TUESDAY",
  "WEDNESDAY",
  "THURSDAY",
  "FRIDAY",
  "SATURDAY"};


// Months of the year
static char *monthnames[12] = {
  "JANUARY", 
  "FEBRUARY",
  "MARCH",
  "APRIL",
  "MAY",
  "JUNE",
  "JULY",
  "AUGUST",
  "SEPTEMBER",
  "OCTOBER",
  "NOVEMBER",
  "DECEMBER"};




static void update_minutes(struct tm *tick_time){

  // Create a long-lived buffer
  static char buffer[] = "00";

  //Use 12 hour format
  strftime(buffer, sizeof("00"), "%M", tick_time);

  // Display this time on the TextLayer
  text_layer_set_text(s_minutes_layer, buffer);  

}

  
static void update_hours(struct tm *tick_time){

  // Create a long-lived buffer
  static char buffer[] = "00";

/*  
  // Write the current hours and minutes into the buffer
  if(clock_is_24h_style() == true) {
    //Use 2h hour format
    strftime(buffer, sizeof("00"), "%H", tick_time);
  } else 
  {
*/
    //Use 12 hour format
    strftime(buffer, sizeof("00"), "%I", tick_time);
/*  }
*/

  // Display this time on the TextLayer
  if (buffer[0] == '0'){
    text_layer_set_text(s_hours_layer, buffer+1);
  }
  else
    {
    text_layer_set_text(s_hours_layer, buffer);  
  }

  // Set if AM or PM
  if (tick_time->tm_hour < 12){
    // AM
    text_layer_set_text(s_ampm_layer, "AM");  
  }
  else
  {
    // PM
    text_layer_set_text(s_ampm_layer, "PM");  
  }

  
}


static void update_days(struct tm *tick_time){
  
  // Update dayname

  text_layer_set_text(s_dayname_layer, daynames[tick_time->tm_wday]);

  // Get the date
  // Create a long-lived buffer
  static char buffer[] = "00";

  strftime(buffer, sizeof("00"), "%d", tick_time);

  // Set the date textlayer
  text_layer_set_text(s_date_layer, buffer);
  
}


static void update_month(struct tm *tick_time){
  
  // Update Month name
  text_layer_set_text(s_month_layer, monthnames[tick_time->tm_mon]);
  
}


static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {

  if (units_changed & MONTH_UNIT) 
  {
    update_month(tick_time);
  }
  if (units_changed & DAY_UNIT) 
  {
    update_days(tick_time);
  }
  if (units_changed & HOUR_UNIT) 
  {
    update_hours(tick_time);
  }
  if (units_changed & MINUTE_UNIT) 
  {
    update_minutes(tick_time);
  }
  
}




static void main_window_load(Window *window) {

  // Load the Fonts
  s_dayname_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_GOTHAM_BOOK_12));
  s_hours_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_GOTHAM_BOLD_58));
  s_minutes_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_GOTHAM_BOOK_34));
  s_date_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_GOTHAM_BOOK_10));
  
  // Get a handle to the root window layer
  s_main_window_layer = window_get_root_layer(window);

  // Create dayname TextLayer
  s_dayname_layer = text_layer_create(GRect(0, 20, 144, 18));
  text_layer_set_background_color(s_dayname_layer, GColorClear);
  text_layer_set_text_color(s_dayname_layer, GColorWhite);
  //text_layer_set_text(s_dayname_layer, "MONDAY");
    
  // Set the dayname font and alignment
  text_layer_set_font(s_dayname_layer, s_dayname_font);
  text_layer_set_text_alignment(s_dayname_layer, GTextAlignmentCenter);

  // Add the dayname layer to the main layer
  layer_add_child(s_main_window_layer, text_layer_get_layer(s_dayname_layer));

  
  // Create hours TextLayer
  s_hours_layer = text_layer_create(GRect(-7, 43, 70, 80));
  text_layer_set_background_color(s_hours_layer, GColorClear);
  text_layer_set_text_color(s_hours_layer, GColorWhite);
  //text_layer_set_text(s_hours_layer, "12");
    
  // Set the hours font and alignment
  text_layer_set_font(s_hours_layer, s_hours_font);
  text_layer_set_text_alignment(s_hours_layer, GTextAlignmentRight);

  // Add the hours layer to the main layer
  layer_add_child(s_main_window_layer, text_layer_get_layer(s_hours_layer));

  
  // Create time separator layer
  s_separator_layer = text_layer_create(GRect(65,53,10,50));
  text_layer_set_background_color(s_separator_layer, GColorClear);
  text_layer_set_text_color(s_separator_layer, GColorWhite);
  text_layer_set_text(s_separator_layer, ":");
  
  // Set the font and alignment
  text_layer_set_font(s_separator_layer, s_minutes_font);
  text_layer_set_text_alignment(s_separator_layer, GTextAlignmentCenter);

  // Add the separator layer to the main layer
  layer_add_child(s_main_window_layer, text_layer_get_layer(s_separator_layer));


  // Create minutes TextLayer
  s_minutes_layer = text_layer_create(GRect(71, 53, 50, 40));
  text_layer_set_background_color(s_minutes_layer, GColorClear);
  text_layer_set_text_color(s_minutes_layer, GColorWhite);
  //text_layer_set_text(s_minutes_layer, "30");
    
  // Set the minutes font and alignment
  text_layer_set_font(s_minutes_layer, s_minutes_font);
  text_layer_set_text_alignment(s_minutes_layer, GTextAlignmentLeft);

  // Add the minutes layer to the main layer
  layer_add_child(s_main_window_layer, text_layer_get_layer(s_minutes_layer));


  // Create the AM/PM  TextLayer
  s_ampm_layer = text_layer_create(GRect(120, 75, 20, 16));
  text_layer_set_background_color(s_ampm_layer, GColorClear);
  #ifdef PBL_COLOR
    text_layer_set_text_color(s_ampm_layer, GColorVividCerulean);
  #else
    text_layer_set_text_color(s_ampm_layer, GColorWhite);
  #endif
  //text_layer_set_text(s_ampm_layer, "AM");
    
  // Set the AM/PM font and alignment
  text_layer_set_font(s_ampm_layer, s_date_font);
  text_layer_set_text_alignment(s_ampm_layer, GTextAlignmentLeft);

  // Add the AM/PM layer to the main layer
  layer_add_child(s_main_window_layer, text_layer_get_layer(s_ampm_layer));


  // Create date TextLayer
  s_date_layer = text_layer_create(GRect(62, 91, 14, 14));
  text_layer_set_background_color(s_date_layer, GColorClear);
  text_layer_set_text_color(s_date_layer, GColorWhite);
  //text_layer_set_text(s_date_layer, "05");
    
  // Set the date font and alignment
  text_layer_set_font(s_date_layer, s_date_font);
  text_layer_set_text_alignment(s_date_layer, GTextAlignmentRight);

  // Add the date layer to the main layer
  layer_add_child(s_main_window_layer, text_layer_get_layer(s_date_layer));

  
  // Create month TextLayer
  s_month_layer = text_layer_create(GRect(80, 91, 64, 14));
  text_layer_set_background_color(s_month_layer, GColorClear);
  text_layer_set_text_color(s_month_layer, GColorWhite);
  //text_layer_set_text(s_month_layer, "JANUARY");
    
  // Set the month font and alignment
  text_layer_set_font(s_month_layer, s_date_font);
  text_layer_set_text_alignment(s_month_layer, GTextAlignmentLeft);

  // Add the month layer to the main layer
  layer_add_child(s_main_window_layer, text_layer_get_layer(s_month_layer));

  
  // Make sure the time is displayed from the start (avoids blank time on start)
  time_t now = time(NULL);
  struct tm *tick_time = localtime(&now);
  tick_handler(tick_time, MONTH_UNIT + HOUR_UNIT + MINUTE_UNIT + DAY_UNIT);
  
}

static void main_window_unload(Window *window) {
  
  // Unsubscribe from tick handler
  tick_timer_service_unsubscribe();
  
  // Destroy TextLayers
  text_layer_destroy(s_month_layer);
  text_layer_destroy(s_date_layer);
  text_layer_destroy(s_ampm_layer);
  text_layer_destroy(s_minutes_layer);
  text_layer_destroy(s_separator_layer);    
  text_layer_destroy(s_hours_layer);
  text_layer_destroy(s_dayname_layer);
  
  //Unload GFont
  fonts_unload_custom_font(s_date_font);
  fonts_unload_custom_font(s_minutes_font);
  fonts_unload_custom_font(s_hours_font);
  fonts_unload_custom_font(s_dayname_font);

}




static void init() {
  
  light_enable(true);
    
  
  // Create main Window element and assign to pointer
  s_main_window = window_create();

  // Make the main window background black
  window_set_background_color(s_main_window, GColorBlack);
  
  // Set handlers to manage the elements inside the Window
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });

  // Show the Window on the watch, with animated=true
  window_stack_push(s_main_window, true);
  
  // Register with TickTimerService
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
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

