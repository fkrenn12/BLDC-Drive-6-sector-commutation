#include "gui-fletuino-application-demo.h"

#ifdef FLETUINO_APPLICATION_DEMO
uint16_t BUTTON_EMERGENCY_OFF, SLIDER_SPEED_REF, SLIDER_MOMENTUM, MOMENTUM_RESET, BUTTON_SPEED_REF_RESET, MODE_SELECTOR, 
            NUMERIC_RPM, NUMERIC_DEBUG, NUMERIC_CURRENT, NUMERIC_VOLT, DIRECTION_SELECTOR;

static uint8_t direction = CLOCKWISE;
static uint8_t momentum_activated = 1;
static int16_t rpm = 0;
static void on_speed_changed(const char* event, const char* value);
static void on_momentum_changed(const char* event, const char* value);
static void on_switch(const char* event, const char* value);

void on_speed_changed(const char* event, const char* value) {
    if (strcmp(event, EVENT_CHANGED)==0) {
        // slider change
        rpm = Drive_setSpeedRpm((int16_t)atoi(value));
        if (rpm >= g.speed.max){
            fletuino_set_property_int(SLIDER_SPEED_REF, "value", g.speed.max);
        }
    }
}

static void on_momentum_changed(const char* event, const char* value){
        Drive_setSoftwareCurrentRef((int16_t)(atoi(value)) * CURRENT_USAGE_OF_MAX_CURRENT);
        // g.mode_selector = MODE_SELECTOR_IREF;
        // fletuino_set_property_int(MODE_SELECTOR, "value", 0);
}

static void on_emergency_off(const char* event, const char* value){
    Drive_Stop();  
}

static void on_zero_moment(const char* event, const char* value){
    Drive_Stop();
    fletuino_set_property_int(SLIDER_MOMENTUM, "value", 0);
    fletuino_set_property_int(SLIDER_SPEED_REF, "value", 0);
}

static void on_zero_speed(const char* event, const char* value){
    Drive_setSpeedRpm(0);
    fletuino_set_property_int(SLIDER_SPEED_REF, "value", 0);
}

bool on_any_event(const uint16_t id,const char* event, const char* value)
{
    // printf("#[%d] - @[%s] - $[%s]\n\r", id, event, value); 
    return false;
}

static void on_direction_switch(const char* event, const char* value){
    direction = (uint8_t)(atoi(value));
    if (direction) Drive_RunBackward();
    else Drive_RunForward();
}

static void on_switch(const char* event, const char* value)
{
    momentum_activated = (uint8_t)(atoi(value));
    if (momentum_activated){
        Drive_RunModeMomentum();
        if (direction) Drive_RunBackward();
        else Drive_RunForward();
    }
    else {
        Drive_RunModeCruiser();
        Drive_setSpeedRpm(rpm);
        fletuino_set_property_int(SLIDER_SPEED_REF, "value", rpm);
        // g.speed.controller.integrator = 0;
        // g.speed.controller.output = 0;
    }
}

void start_page(){ 
    g.demo = 1;
    Drive_RunModeMomentum();
    Drive_RunForward();
    fletunio_page("BLDC Drive - ET@HTL-Hollabrunn", "default", "dark");
    fletuino_text("Drive Demonstration", 40);
    fletuino_divider(3);
    BUTTON_EMERGENCY_OFF = fletuino_button("OFF", "tag3", 40, on_emergency_off); 
    //fletuino_numeric(const char* value, const char* scale, const char* offset, int decimals, const char* unit, uint16_t size);
    NUMERIC_RPM = fletuino_numeric(/*value*/0,/*scale*/1.0,/*offset*/0,/*decimals*/0,/*unit*/"rpm",/*size*/30);
    NUMERIC_CURRENT = fletuino_numeric(0, ADC_FACTOR_CURRENT, 0, 3 ," A",30);
    NUMERIC_VOLT = fletuino_numeric(0, ADC_FACTOR_VLINK, 0, 1 ," V",30);
    NUMERIC_DEBUG = fletuino_numeric(0, 1, 0, 0 ,"DIG",30);
    fletuino_bar((CONTROLS){NUMERIC_VOLT, NUMERIC_RPM, NUMERIC_CURRENT},4,"center-space-evenly");
    SLIDER_MOMENTUM = fletuino_slider("Momentum-GAS", /*init*/0, /*min*/0, /*max*/2047 ,/*size*/30, /*event*/on_momentum_changed);
    fletuino_set_property_int(SLIDER_MOMENTUM, "width", 600);
    MOMENTUM_RESET = fletuino_button("Reset momentum", "tag1", 30, on_zero_moment);
    fletuino_bar((CONTROLS){SLIDER_MOMENTUM,MOMENTUM_RESET},2,"center-space-evenly");
    fletuino_divider(3);
    MODE_SELECTOR = fletuino_switch("Automatic/Momentum (A/M)", 30, momentum_activated, on_switch);
    DIRECTION_SELECTOR = fletuino_switch("Forward/Reverse (F/R)", 30, 0, on_direction_switch);
    fletuino_bar((CONTROLS){MODE_SELECTOR,DIRECTION_SELECTOR},2,"center-space-evenly");
    fletuino_divider(3);
    SLIDER_SPEED_REF = fletuino_slider("Speed RPM",(int16_t)g.speed.ref, 0, 3000, 30, on_speed_changed);
    fletuino_set_property_int(SLIDER_SPEED_REF, "width", 600);
    BUTTON_SPEED_REF_RESET = fletuino_button("Reset speed", "tag2", 30, on_zero_speed);
    fletuino_bar((CONTROLS){SLIDER_SPEED_REF, BUTTON_SPEED_REF_RESET},2,"center-space-evenly");
    fletuino_add_any_event_callback(on_any_event);
}

void gui_update(void){
    fletuino_set_value_int(NUMERIC_RPM, g.speed.value);
    fletuino_set_value_int(NUMERIC_CURRENT, g.current.value);
    fletuino_set_value_int(NUMERIC_VOLT, g.vlink); 
    fletuino_set_value_int(NUMERIC_DEBUG, g.speed.out);
}
#endif