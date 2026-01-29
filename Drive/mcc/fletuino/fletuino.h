#pragma once
#include <stdio.h> 
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#define VERSION_NUMBER 1
#define DEFAULT_PAGE_ID 65535
#define EVENT_CLICKED "clicked"
#define EVENT_CHANGED "changed"
#define EVENT_LONGCLICKED "long_press"
#define EVENT_SUBMITTED "submitted"

typedef const uint16_t CONTROLS[];
typedef const char* RADIOS[];
typedef const char* CHECKBOXES[];
typedef const char* OPTIONS[];

typedef bool (*UART_AVAILABLE_FUNCTION)(void);
typedef uint8_t (*UART_READ_FUNCTION)(void);
typedef void (*UART_WRITE_FUNCTION)(const uint8_t c);
typedef void (*VOID_FUNCTION_VOID)(void);
typedef void (*EVENTCALLBACK)(const char* event, const char* value);
typedef bool (*EVENTCALLBACK_ANY)(const uint16_t id, const char* event, const char* value);
typedef bool (*EVENTCALLBACK_ANYCLICK)(const uint16_t id);
typedef bool (*EVENTCALLBACK_ANYLONGCLICK)(const uint16_t id);

void fletuino_init(UART_AVAILABLE_FUNCTION _charAvailable, UART_READ_FUNCTION _charRead, UART_WRITE_FUNCTION _charWrite, VOID_FUNCTION_VOID page_update_function);
void fletuino_loop(void);
void fletunio_page(const char* title, const char* color_schema, const char* color_mode);
void fletuino_remove(uint16_t id);

uint16_t fletuino_text(const char* text, uint16_t size);
uint16_t fletuino_textbar(const char* text, uint16_t size, const char* alignment);
//uint16_t fletuino_numeric(const char* value, const char* scale, const char* offset, int decimals, const char* unit, uint16_t size);
uint16_t fletuino_numeric(const float value, const float scale, const float offset, int decimals, const char* unit, uint16_t size);
uint16_t fletuino_slider(const char* text, int value, int min, int max, uint16_t size, EVENTCALLBACK event);
uint16_t fletuino_divider(uint16_t thickness);
uint16_t fletuino_button(const char* text, const char* tag, uint16_t size, EVENTCALLBACK event);
uint16_t fletuino_radio(RADIOS, uint16_t count, uint16_t selected, uint16_t size, EVENTCALLBACK event);
uint16_t fletuino_switch(const char* text, uint16_t size, uint16_t value, EVENTCALLBACK event);
uint16_t fletuino_dropdown(const char* text, OPTIONS options, uint16_t count, uint16_t selected, uint16_t heigh, EVENTCALLBACK event);
uint16_t fletuino_bar(CONTROLS, uint16_t count, const char* alignment);
uint16_t fletuino_chart(const char* type);

void fletuino_set_text(uint16_t id, const char* text);
void fletuino_set_size(uint16_t id, uint16_t size);
void fletuino_set_visible(uint16_t id, uint16_t visible);
void fletuino_set_value_int(uint16_t id, int value);
void fletuino_set_value_str(uint16_t id, const char* value);
void fletuino_set_value_float(uint16_t id, float value);
void fletuino_set_property_int(uint16_t id, const char* property, int value);
void fletuino_set_property_str(uint16_t id, const char* property, const char* value);

void fletuino_add_any_event_callback(EVENTCALLBACK_ANY callback);
