#include "fletuino.h"
#include "json/json_decoder.h"

#define UART_BUFFER_SIZE 256  //max length of line received from uart
char buf[UART_BUFFER_SIZE];

jsonNode_t *root;
uint16_t id=0;
VOID_FUNCTION_VOID pageUpdateFunction;
EVENTCALLBACK_ANY anyEventCallback = (EVENTCALLBACK_ANY)NULL;
UART_AVAILABLE_FUNCTION charAvailable = (UART_AVAILABLE_FUNCTION)NULL;
UART_READ_FUNCTION charRead = (UART_READ_FUNCTION)NULL;
UART_WRITE_FUNCTION charWrite = (UART_WRITE_FUNCTION)NULL;

void fletuino_handle_response(uint8_t *command);
void sendBufferToUart(bool appendLF);

void sendBufferToUart(bool appendLF){
    uint8_t append = (uint8_t)appendLF * '\n';
    for (uint8_t* c = (uint8_t*)buf; *c != '\0'; c++)
        charWrite(*c);
    if (append) charWrite(append);
}

void fletuino_loop(void){
    static uint8_t line[UART_BUFFER_SIZE];  
    static uint8_t pointer = 0;
    if (charAvailable == NULL || charRead == NULL) return;
    while(charAvailable())
    {
        char rxChar = charRead();
        // printf("%c", rxChar);
        line[pointer] = (rxChar==13 || rxChar==10)?0x00:rxChar;
        if (line[pointer]){
            if (++pointer > (UART_BUFFER_SIZE - 1)) pointer--; // avoids buffer overflow
            continue;
        }
        fletuino_handle_response(line);
        pointer = 0;
    }
}

void fletuino_init(UART_AVAILABLE_FUNCTION _charAvailable, UART_READ_FUNCTION _charRead, UART_WRITE_FUNCTION _charWrite, VOID_FUNCTION_VOID page_update_function){
    id=0;
    charAvailable = _charAvailable;
    charRead = _charRead;
    charWrite = _charWrite;    
    if (page_update_function != NULL){
        pageUpdateFunction = page_update_function;
        pageUpdateFunction();
    } 
}
   
void fletuino_add_any_event_callback(EVENTCALLBACK_ANY callback){
    anyEventCallback = callback;
}

void fletunio_page(const char* title, const char* color_schema, const char* color_mode){
    id=0;
    snprintf(buf, UART_BUFFER_SIZE, "{\"Page\":{\"version\":%d,\"#\":%d,\"action\":\"clear\", \"title\":\"%s\",\"color_schema\":\"%s\",\"color_mode\":\"%s\",\"*\":%d}}", VERSION_NUMBER, (int)DEFAULT_PAGE_ID, title, color_schema, color_mode,(int)pageUpdateFunction);
    sendBufferToUart(true);
}

void fletuino_remove(uint16_t id)
{
    // do not work on embedded controls in a row at the moment
    snprintf(buf, UART_BUFFER_SIZE, "{\"#\":%d,\"action\":\"remove\"}", (int)id);
    sendBufferToUart(true);
}

uint16_t fletuino_slider(const char* text, int value, int min, int max, uint16_t size, EVENTCALLBACK event){
    id++;
    snprintf(buf, UART_BUFFER_SIZE,"{\"Slider\":{\"#\":%d,\"text\":\"%s\",\"size\":%d,\"min\":%d,\"max\":%d, \"$\":%d,\"*\":%d}}", (int)id , text, size, min, max, value, (int)event);
    sendBufferToUart(true);
    return id;
}

//uint16_t fletuino_numeric(const char* value, const char*  scale, const char*  offset, int decimals, const char* unit, uint16_t size){
uint16_t fletuino_numeric(const float value, const float scale, const float offset, int decimals, const char* unit, uint16_t size){
    id++;
    snprintf(buf, UART_BUFFER_SIZE,"{\"Numeric\":{\"#\":%d,\"$\":\"%f\",\"size\":%d,\"scale\":%f,\"offset\":%f, \"decimals\":%d, \"unit\":\"%s\"}}", (int)id, value, size, scale, offset, decimals, unit);
    sendBufferToUart(true);
    return id;
}

uint16_t fletuino_dropdown(const char* text, OPTIONS options, uint16_t count, uint16_t selected, uint16_t width, EVENTCALLBACK event){
    id++;
    snprintf(buf, UART_BUFFER_SIZE,"{\"Dropdown\":{\"#\":%d,\"options\":[", (int)id);
    sendBufferToUart(false);
    for(int i=0; i<count; i++){
        snprintf(buf, UART_BUFFER_SIZE,"\"%s\"", options[i]);
        sendBufferToUart(false);
        if(i<count-1){
            snprintf(buf, UART_BUFFER_SIZE,",");
            sendBufferToUart(false);
        }
    }
    if (selected<1 || selected>count) selected=1;
    snprintf(buf, UART_BUFFER_SIZE,"],\"$\":\"%s\",\"text\":\"%s\",\"width\":%d,\"*\":%d}}", options[selected-1], text, width, (int)event);
    sendBufferToUart(true);
    return id;
}

uint16_t fletuino_divider(uint16_t thickness){
    id++;
    snprintf(buf, UART_BUFFER_SIZE,"{\"Divider\":{\"#\":%d,\"thickness\":%d}}", (int)id, thickness);
    sendBufferToUart(true);
    return id;
}

/**
 * @brief Sends a button control message via UART and registers a callback.
 *
 * Creates a JSON message representing a button control with the specified name, text, tag, and size,
 * and sends it to the UART interface. Registers the provided callback function to handle events for
 * this button control.
 *
 * @param name The identifier for the button control.
 * @param text The display text for the button.
 * @param tag  An optional tag for additional button identification/context.
 * @param size The size parameter for the button control.
 * @param callback A function pointer to handle events triggered by this button.
 */
uint16_t fletuino_button(const char* text, const char* tag, uint16_t size, EVENTCALLBACK event){
    ++id;
    snprintf(buf, UART_BUFFER_SIZE,"{\"Button\":{\"#\":%d,\"text\":\"%s\",\"tag\":\"%s\",\"size\":%d,\"*\":%d}}", (int)id, text, tag, (int)size, (int)event);
    sendBufferToUart(true);
    return(id);
}

uint16_t fletuino_radio(RADIOS radios, uint16_t count, uint16_t selected, uint16_t size, EVENTCALLBACK event){
    ++id;
    snprintf(buf, UART_BUFFER_SIZE,"{\"Radiogroup\":{\"#\":%d,\"options\":[", (int)id);
    sendBufferToUart(false);
    for(int i=0; i<count; i++){
        snprintf(buf, UART_BUFFER_SIZE,"\"%s\"", radios[i]);
        sendBufferToUart(false);
        if(i<count-1){
            snprintf(buf, UART_BUFFER_SIZE,",");
            sendBufferToUart(false);
        }
    }
    if (selected<1 || selected>count) selected=1;
    snprintf(buf, UART_BUFFER_SIZE,"],\"$\":\"%s\",\"size\":%d,\"*\":%d}}", radios[selected-1], size, (int)event);
    sendBufferToUart(true);
    return id;
}

uint16_t fletuino_bar(CONTROLS controls, uint16_t count, const char* alignment){
    id++;
    snprintf(buf, UART_BUFFER_SIZE,"{\"Row\": [");
    sendBufferToUart(false);
    for(int i=0; i<count; i++){
        snprintf(buf, UART_BUFFER_SIZE,"%d", controls[i]);
        sendBufferToUart(false);
        if(i<count-1){
            snprintf(buf, UART_BUFFER_SIZE,",");
            sendBufferToUart(false);
        }
    }
    snprintf(buf, UART_BUFFER_SIZE,"],\"#\":%d, \"alignment\": \"%s\"}", (int)id, alignment);
    sendBufferToUart(true);
    return id;
}

uint16_t fletuino_text(const char* text, uint16_t size){
    id++;
    snprintf(buf, UART_BUFFER_SIZE,"{\"Text\":{\"#\":%d,\"text\":\"%s\",\"size\":%d}}", (int)id, text, size);
    sendBufferToUart(true);
    return id;
}

uint16_t fletuino_textbar(const char* text, uint16_t size, const char* alignment){
    id++;
    snprintf(buf, UART_BUFFER_SIZE,"{\"TextBar\":{\"#\":%d,\"text\":\"%s\",\"size\":%d,\"alignment\": \"%s\"}}", (int)id, text, size, alignment);
    sendBufferToUart(true);
    return id;
}

void fletuino_set_text(uint16_t id, const char* text){
    snprintf(buf, UART_BUFFER_SIZE,"{\"#\":%d,\"text\":\"%s\"}", (int)id, text);
    sendBufferToUart(true);
}

void fletuino_set_size(uint16_t id, uint16_t size){
    snprintf(buf, UART_BUFFER_SIZE,"{\"#\":%d,\"size\":%d}", (int)id, size);
    sendBufferToUart(true);
}

void fletuino_set_visible(uint16_t id, uint16_t visible){
    snprintf(buf, UART_BUFFER_SIZE,"{\"#\":%d,\"visible\":%d}", (int)id, visible);
    sendBufferToUart(true);
}

void fletuino_set_value_int(uint16_t id, int value){
    snprintf(buf, UART_BUFFER_SIZE,"{\"#\":%d,\"$\":%d}", (int)id, value);
    sendBufferToUart(true);
}

void fletuino_set_value_str(uint16_t id, const char* value){
    snprintf(buf, UART_BUFFER_SIZE,"{\"#\":%d,\"$\":\"%s\"}", (int)id, value);
    sendBufferToUart(true);
}

void fletuino_set_value_float(uint16_t id, float value){
    snprintf(buf, UART_BUFFER_SIZE,"{\"#\":%d,\"$\":\"%f\"}", (int)id, value);
    sendBufferToUart(true);
}

void fletuino_set_property_int(uint16_t id, const char* property, int value){
    if (strcmp(property, "value") == 0) fletuino_set_value_int(id, value); 
    snprintf(buf, UART_BUFFER_SIZE,"{\"#\":%d,\"%s\":%d}", (int)id, property, value);
    sendBufferToUart(true);
}

void fletuino_set_property_str(uint16_t id, const char* property, const char* value){
    if (strcmp(property, "value") == 0) fletuino_set_value_str(id, value); 
    snprintf(buf, UART_BUFFER_SIZE,"{\"#\":%d,\"%s\":\"%s\"}", (int)id, property, value);
    sendBufferToUart(true);
}

uint16_t fletuino_switch(const char* text, uint16_t size, uint16_t value, EVENTCALLBACK event){
    id++;
    snprintf(buf, UART_BUFFER_SIZE,"{\"Switch\":{\"#\":%d,\"text\":\"%s\",\"size\":%d,\"$\":%d,\"*\":%d}}", (int)id, text, size, value, (int)event);
    sendBufferToUart(true);
    return id;
}

uint16_t fletuino_chart(const char* type){
    id++;
    snprintf(buf, UART_BUFFER_SIZE,"{\"Chart\":{\"#\":%d,\"type\":\"%s\"}}", (int)id, type);
    sendBufferToUart(true);
    return id;
}

void fletuino_handle_response(uint8_t *command){
    // printf("Command received: %s\n\r", command);
    jsonDecoderStatus_t ret;
    ret = JSON_DECODER_fromString((char*)command);
    if (ret != JSON_DECODER_OK) return;  // error in json 
    JSON_DECODER_getRoot(&root);
    // --------------------------------------------------------------------------
    //  dispatching events
    // --------------------------------------------------------------------------
    char event[16], value[64];
    int id=0;
    int callbackInt = 0;
    EVENTCALLBACK callback;
    // key "id" is checked - must be present
    ret = JSON_DECODER_getNumber(root, "#", &id);
    if (ret != JSON_DECODER_OK) return;  // id not present 
    // key "event" is checked - must be present
    ret = JSON_DECODER_getString(root, "@", 16, event);
    if (ret != JSON_DECODER_OK) return;  // event not present 
    strcpy(value, "");
    ret = JSON_DECODER_getString(root, "$", 64, value);
    // printf("= %s %d\n", name, callbackList.size );
    ret = JSON_DECODER_getNumber(root, "*", &callbackInt);
    // printf("Event: id=%d event=%s value=%s callback=%d\n\r", (uint16_t)id, event, value, callbackInt);
    // any callback first priority
    // printf("any callback first priority\n\r");

    if (anyEventCallback != NULL){
        if (anyEventCallback((uint16_t)id, event, value)){
            // if true - already handled - stop further processing
            return;
        }
    }
    
    // printf("individual callback second priority\n\r");
    if ((uint16_t)id != DEFAULT_PAGE_ID){
        // printf("Control update\n\r"); 
        if (callbackInt){
            callback = (EVENTCALLBACK)callbackInt;
            callback(event, value);
        }
    }
    else{
        // printf("Page update\n\r");
        if (pageUpdateFunction != NULL){
            pageUpdateFunction();
        }
    }
}
