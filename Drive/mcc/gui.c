#include "gui.h"


extern TGlobal g;

uint16_t BUTTON_EMERGENCY_OFF, SLIDER_SPEED_REF, SLIDER_IREF, IREF_RESET, BUTTON_SPEED_REF_RESET, CONTROLLER_SELECTOR, NUMERIC_RPM, NUMERIC_CURRENT, NUMERIC_VOLT;

static void on_speed_changed(const char* event, const char* value);
static void on_iref_changed(const char* event, const char* value);
static void on_kp_current_changed(const char* event, const char* value);
static void on_ki_current_changed(const char* event, const char* value);
static void on_switch(const char* event, const char* value);

void on_speed_changed(const char* event, const char* value) {
    if (strcmp(event, EVENT_CHANGED)==0) {
        // slider change
        int16_t rpm = drive_set_speed_rpm((int16_t)atoi(value));
        if (rpm >= g.speed.max){
            fletuino_set_property_int(SLIDER_SPEED_REF, "value", g.speed.max);
        }
    }
}

void on_kp_speed_changed(const char* event, const char* value){
    double kp = (double)atof(value)*0.0001;
    PIController_Synthetise_kp(&g.speed.controller, double_to_fixed32(kp));
}
void on_ki_speed_changed(const char* event, const char* value){
    double ki = (double)atof(value)*0.0001;
    PIController_Synthetise_ki(&g.speed.controller, double_to_fixed32(ki));
    PIController_Reset_integrator(&g.speed.controller);
}

static void on_iref_changed(const char* event, const char* value){
        g.current.ref = (int32_t)(atoi(value)) * 1;
        g.mode_selector = MODE_SELECTOR_IREF;
        fletuino_set_property_int(CONTROLLER_SELECTOR, "value", 0);
}

static void on_kp_current_changed(const char* event, const char* value){
    double kp = (double)atof(value)*0.0001;
    PIController_Synthetise_kp(&g.current.controller, double_to_fixed32(kp));
}

static void on_ki_current_changed(const char* event, const char* value){
    double ki = (double)atof(value)*0.0001;
    PIController_Synthetise_ki(&g.current.controller, double_to_fixed32(ki));
    PIController_Reset_integrator(&g.current.controller);
}

static void on_emergency_off(const char* event, const char* value){
    drive_stop();  
}

static void on_zero_moment(const char* event, const char* value){
    drive_stop(1);
    fletuino_set_property_int(CONTROLLER_SELECTOR, "value", 0);
    fletuino_set_property_int(SLIDER_IREF, "value", 0);
    fletuino_set_property_int(SLIDER_SPEED_REF, "value", 0);
}

static void on_zero_speed(const char* event, const char* value){
    g.speed.ref = 0;
    g.speed.ramp.target = 0;
    fletuino_set_property_int(SLIDER_SPEED_REF, "value", 0);
}

bool on_any_event(const uint16_t id,const char* event, const char* value)
{
    // printf("#[%d] - @[%s] - $[%s]\n\r", id, event, value); 
    return false;
}


static void on_switch(const char* event, const char* value)
{
    g.speed.controller_activated = (uint8_t)(atoi(value));
    if (g.speed.controller_activated){
        g.mode_selector = MODE_SELECTOR_SPEEDCONTROLLER;
        if (g.current.ref >= 0){
            g.speed.ramp.target = g.speed.value;
            g.speed.controller.integrator = g.current.ref;
            fletuino_set_property_int(SLIDER_SPEED_REF, "value", g.speed.value);
        }
        else{
            // no speed control in reverse direction
            g.mode_selector = MODE_SELECTOR_IREF;
            g.speed.controller_activated = 0;
            fletuino_set_property_int(CONTROLLER_SELECTOR, "value", 0);
        }
    }
    else{
        fletuino_set_property_int(SLIDER_IREF, "value", g.current.ref);
    }
}

void start_page(){ 
    fletunio_page("BLDC Drive - ET@HTL-Hollabrunn", "default", "dark");
    fletuino_text("BLDC Motor Drive Dashboard", 40);
    fletuino_divider(3);
    BUTTON_EMERGENCY_OFF = fletuino_button("OFF", "tag3", 40, on_emergency_off); 
    //fletuino_numeric(const char* value, const char* scale, const char* offset, int decimals, const char* unit, uint16_t size);
    NUMERIC_RPM = fletuino_numeric(/*value*/0,/*scale*/1.0,/*offset*/0,/*decimals*/0,/*unit*/"rpm",/*size*/30);
    NUMERIC_CURRENT = fletuino_numeric(0, ADC_FACTOR_CURRENT, 0, 3 ," A",30);
    NUMERIC_VOLT = fletuino_numeric(0, ADC_FACTOR_VLINK, 0, 1 ," V",30);
    fletuino_bar((CONTROLS){NUMERIC_VOLT, NUMERIC_RPM, NUMERIC_CURRENT},3,"center-space-evenly");
    SLIDER_IREF = fletuino_slider("Iref - Momentum", /*init*/0, /*min*/-2048, /*max*/2048 ,/*size*/30, /*event*/on_iref_changed);
    fletuino_set_property_int(SLIDER_IREF, "width", 600);
   IREF_RESET = fletuino_button("Reset momentum", "tag1", 30, on_zero_moment);
    fletuino_bar((CONTROLS){SLIDER_IREF,IREF_RESET},2,"center-space-evenly");
    fletuino_slider("KP Current", fixed32_to_double(g.current.controller.kp)*10000, 0, 30000, 30, on_kp_current_changed);
    fletuino_slider("KI Current", fixed32_to_double(g.current.controller.ki)*10000, 0, 1000, 30, on_ki_current_changed);
    fletuino_divider(3);
    CONTROLLER_SELECTOR = fletuino_switch("Speed Controller (cruiser) ON/OFF ", 30, 0, on_switch);
    fletuino_divider(3);
    SLIDER_SPEED_REF = fletuino_slider("Speed RPM",(int16_t)g.speed.ref, 0, 3000, 30, on_speed_changed);
    fletuino_set_property_int(SLIDER_SPEED_REF, "width", 600);
    BUTTON_SPEED_REF_RESET = fletuino_button("Reset speed", "tag2", 30, on_zero_speed);
    fletuino_bar((CONTROLS){SLIDER_SPEED_REF, BUTTON_SPEED_REF_RESET},2,"center-space-evenly");
    fletuino_slider("KP Speed", fixed32_to_double(g.speed.controller.kp)*10000, 0, 10000, 30, on_kp_speed_changed);
    fletuino_slider("KI Speed", fixed32_to_double(g.speed.controller.ki)*10000, 0, 1000, 30, on_ki_speed_changed);
    fletuino_add_any_event_callback(on_any_event);
}

void gui_update(void){
    fletuino_set_value_int(NUMERIC_RPM, g.speed.value);
    fletuino_set_value_int(NUMERIC_CURRENT, g.current.value);
    fletuino_set_value_int(NUMERIC_VOLT, g.vlink);
}