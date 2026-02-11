#include "gui-fletuino-application-demo.h"

#ifdef FLETUINO_APPLICATION_DEMO
uint16_t BUTTON_EMERGENCY_OFF, SLIDER_SPEED_REF, SLIDER_MOMENTUM,MODE_SELECTOR, 
            NUMERIC_RPM, NUMERIC_DEBUG1, NUMERIC_DEBUG2, NUMERIC_DEBUG3, NUMERIC_CURRENT, NUMERIC_VOLT, DIRECTION_SELECTOR;

static uint8_t direction = CLOCKWISE;
static uint8_t speed_control_activated = 0;
static int16_t rpm = 0;
static int16_t momentum = 0;

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
        momentum = (int16_t)(atoi(value));
        momentum = (momentum > 4095)? 4095 : momentum; 
        g.input.gas = momentum;
}

static void on_emergency_off(const char* event, const char* value){
    g.input.gas = 0;
    fletuino_set_value_int(SLIDER_MOMENTUM, 0);
    Drive_Stop();  
}

bool on_any_event(const uint16_t id,const char* event, const char* value)
{
    // printf("#[%d] - @[%s] - $[%s]\n\r", id, event, value); 
    return false;
}

static void on_direction_switch(const char* event, const char* value){
    direction = (uint8_t)(atoi(value));
    g.input.f_r = !direction;
}

static void on_switch(const char* event, const char* value)
{
    speed_control_activated = (uint8_t)(atoi(value));
    g.input.a_m = speed_control_activated;
}

void start_page(){ 
    g.demo = 1;
    g.input.gas = momentum;
    g.input.f_r = direction;
    g.input.a_m = speed_control_activated;
    g.input.speedRpm = rpm;

    fletunio_page("BLDC Drive - ET@HTL-Hollabrunn", "default", "dark");
    fletuino_text("Drive Demonstration", 40);
    fletuino_divider(3);
    BUTTON_EMERGENCY_OFF = fletuino_button("Emergency OFF", "tag3", 40, on_emergency_off); 
    //fletuino_numeric(const char* value, const char* scale, const char* offset, int decimals, const char* unit, uint16_t size);
    NUMERIC_DEBUG1 = fletuino_numeric(0, 1, 0, 0 ,"",30);
    NUMERIC_DEBUG2 = fletuino_numeric(0, 1, 0, 0 ,"",30);
    NUMERIC_DEBUG3= fletuino_numeric(0, 1, 0, 0 ,"",30);
    fletuino_bar((CONTROLS){NUMERIC_DEBUG1, NUMERIC_DEBUG2,NUMERIC_DEBUG3},3,"center-space-evenly");
    NUMERIC_RPM = fletuino_numeric(/*value*/0,/*scale*/1.0,/*offset*/0,/*decimals*/0,/*unit*/"rpm",/*size*/30);
    NUMERIC_CURRENT = fletuino_numeric(0, ADC_FACTOR_CURRENT, 0, 3 ," A",30);
    NUMERIC_VOLT = fletuino_numeric(0, ADC_FACTOR_VLINK, 0, 1 ," V",30);
    fletuino_bar((CONTROLS){NUMERIC_VOLT, NUMERIC_RPM, NUMERIC_CURRENT},3,"center-space-evenly");
    SLIDER_MOMENTUM = fletuino_slider("Momentum-GAS", /*init*/0, /*min*/0, /*max*/4095 ,/*size*/30, /*event*/on_momentum_changed);
    fletuino_set_property_int(SLIDER_MOMENTUM, "width", 600);
    fletuino_bar((CONTROLS){SLIDER_MOMENTUM},1,"center-space-evenly");
    fletuino_divider(3);
    MODE_SELECTOR = fletuino_switch("SpeedControl OFF/ON", 30, speed_control_activated, on_switch);
    DIRECTION_SELECTOR = fletuino_switch("Forward/Reverse (F/R)", 30, 0, on_direction_switch);
    fletuino_bar((CONTROLS){MODE_SELECTOR,DIRECTION_SELECTOR},2,"center-space-evenly");
    fletuino_divider(3);
    SLIDER_SPEED_REF = fletuino_slider("Speed RPM",(int16_t)g.speed.ref_ramped, 0, 5000, 30, on_speed_changed);
    fletuino_set_property_int(SLIDER_SPEED_REF, "width", 600);
    fletuino_bar((CONTROLS){SLIDER_SPEED_REF},1,"center-space-evenly");
    fletuino_add_any_event_callback(on_any_event);
}

void gui_update(void){
    fletuino_set_value_int(NUMERIC_RPM, g.speed.value);
    fletuino_set_value_int(NUMERIC_CURRENT, g.current.value);
    fletuino_set_value_int(NUMERIC_VOLT, g.vlink); 
    fletuino_set_value_int(NUMERIC_DEBUG1, g.temperature);
    fletuino_set_value_int(NUMERIC_DEBUG2, ADC_Result(_TEMPERATURE)); 
    // fletuino_set_value_int(NUMERIC_DEBUG3, g.input.speedRpm);
}
#endif