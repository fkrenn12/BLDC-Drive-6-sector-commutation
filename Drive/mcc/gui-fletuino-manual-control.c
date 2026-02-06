#include "gui-fletuino-manual-control.h"

#ifdef FLETUINO_MANUAL_CONTROL
uint16_t VECTOR1, VECTOR2, VECTOR3, VECTOR4, VECTOR5, VECTOR6;
uint16_t NUMERIC_ACTUAL_SECTOR, SLIDER_DC;
uint16_t FLOATING, CLAMPED;
uint16_t SECTOR1_2, SECTOR2_3, SECTOR3_4,SECTOR4_5 ,SECTOR5_6 ,SECTOR6_1;

bool on_any_event(const uint16_t id,const char* event, const char* value)
{
    // printf("#[%d] - @[%s] - $[%s]\n\r", id, event, value); 
    return false;
}

static void on_floating(const char* event, const char* value){
    PWM_override(7);
    
}

static void on_clamped(const char* event, const char* value){
    PWM_override(0);
}

static void on_vector1(const char* event, const char* value){
    PWM_override(1);
    __delay_ms(10); 
    char t[10];
    sprintf(t,"%d",g.position_sector);
    fletuino_set_property_str(SECTOR6_1,"text",t);
}

static void on_vector2(const char* event, const char* value){
    PWM_override(2);
    __delay_ms(10); 
    char t[10];
    sprintf(t,"%d",g.position_sector);
    fletuino_set_property_str(SECTOR1_2,"text",t);
}

static void on_vector3(const char* event, const char* value){
    PWM_override(3);
    __delay_ms(10); 
    char t[10];
    sprintf(t,"%d",g.position_sector);
    fletuino_set_property_str(SECTOR2_3,"text",t);
}

static void on_vector4(const char* event, const char* value){
    PWM_override(4);
    __delay_ms(10); 
    char t[10];
    sprintf(t,"%d",g.position_sector);
    fletuino_set_property_str(SECTOR3_4,"text",t);
}

static void on_vector5(const char* event, const char* value){
    PWM_override(5);
    __delay_ms(10); 
    char t[10];
    sprintf(t,"%d",g.position_sector);
    fletuino_set_property_str(SECTOR4_5,"text",t);
}

static void on_vector6(const char* event, const char* value){
    PWM_override(6);
    __delay_ms(10); 
    char t[10];
    sprintf(t,"%d",g.position_sector);
    fletuino_set_property_str(SECTOR5_6,"text",t);
}

/*
static void on_direction_switch(const char* event, const char* value){
    uint8_t direction = (uint8_t)(atoi(value));
    g.input.f_r = !direction;
}

static void on_switch(const char* event, const char* value)
{
    uint8_t speed_control_activated = (uint8_t)(atoi(value));
    g.input.a_m = speed_control_activated;
}
*/
void on_dc_changed(const char* event, const char* value) {
    if (strcmp(event, EVENT_CHANGED)==0) {
        // slider change
        MDC = (int16_t)atoi(value);
        PG1STATbits.UPDREQ = 1; 
        char t[50];
        sprintf(t, "Duty Cycle %d", MDC);
        fletuino_set_property_str(SLIDER_DC, "text", t);
    }
}
void start_page(){ 
    g.demo = 1;
    MDC = 0;
    PG1STATbits.UPDREQ = 1; 
    fletunio_page("BLDC Drive - ET@HTL-Hollabrunn", "default", "dark");
    fletuino_text("Drive Manual control", 40);
    fletuino_divider(3);
    SLIDER_DC = fletuino_slider("Duty Cycle 0",0, 0, 8191, 30, on_dc_changed);
    FLOATING = fletuino_button("FLOATING", "f1", 40, on_floating); 
    CLAMPED = fletuino_button("CLAMPED", "c1", 40, on_clamped); 
    fletuino_bar((CONTROLS){FLOATING, CLAMPED},2,"center-space-evenly");
    fletuino_divider(3);
    fletuino_text("Vector selection", 20);
    VECTOR1 = fletuino_button("1", "v1", 40, on_vector1); 
    SECTOR1_2 = fletuino_text("-", 40);
    fletuino_set_property_str(SECTOR1_2, "color", "#c9369d"); 
    VECTOR2 = fletuino_button("2", "v2", 40, on_vector2); 
    SECTOR2_3 = fletuino_text("-", 40);
    fletuino_set_property_str(SECTOR2_3, "color", "#c9369d"); 
    VECTOR3 = fletuino_button("3", "v3", 40, on_vector3); 
    SECTOR3_4 = fletuino_text("-", 40);
    fletuino_set_property_str(SECTOR3_4, "color", "#c9369d"); 
    VECTOR4 = fletuino_button("4", "v4", 40, on_vector4); 
    SECTOR4_5 = fletuino_text("-", 40);
    fletuino_set_property_str(SECTOR4_5, "color", "#c9369d"); 
    VECTOR5 = fletuino_button("5", "v5", 40, on_vector5); 
    SECTOR5_6 = fletuino_text("-", 40);
    fletuino_set_property_str(SECTOR5_6, "color", "#c9369d"); 
    VECTOR6 = fletuino_button("6", "v6", 40, on_vector6); 
    SECTOR6_1 = fletuino_text("-", 40);
    fletuino_set_property_str(SECTOR6_1, "color", "#c9369d"); 
    fletuino_bar((CONTROLS){VECTOR1,SECTOR1_2,VECTOR2,SECTOR2_3,VECTOR3,SECTOR3_4,VECTOR4,SECTOR4_5,VECTOR5,SECTOR5_6,VECTOR6, SECTOR6_1,},12,"center-space-evenly");
    fletuino_divider(3);
    fletuino_text("Actual position sector", 20);
    NUMERIC_ACTUAL_SECTOR = fletuino_numeric(0, 1, 0, 0 ,"",20);
    //fletuino_numeric(const char* value, const char* scale, const char* offset, int decimals, const char* unit, uint16_t size);
}

void gui_update(void){
    
    fletuino_set_value_int(NUMERIC_ACTUAL_SECTOR, g.position_sector);
}
#endif