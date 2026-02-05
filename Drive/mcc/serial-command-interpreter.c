#include "serial-command-interpreter.h"

char Senderadress[10];

extern TGlobal g;

int getBlankposition(const char *s)
{
    const char *p = strchr(s, ' ');
    return p ? (int)(p - s) : -1;
}

int getSeparatorposition(const char *s)
{
    const char *p = strchr(s, ':');
    return p ? (int)(p - s) : -1;
}

void sendoutput(uint8_t state_correct, char* from_address){
    const char ack[]={"*ACK!"};
    const char nack[]={"*DCK!"};
    char buffer[50];
    if      (state_correct) sprintf(buffer,"%s:%s:%s\n",from_address,g.myaddress,ack);
    else    sprintf(buffer,"%s:%s:%s\n",from_address,g.myaddress,nack);
    #if defined(DEBUG) && defined(DEBUG_SERIAL_COMMAND_HANDLING)
        char debugBuffer[255];
        sprintf(debugBuffer,"%sout:%s%s\n\r",COL_YELLOW,buffer,COL_WHITE); // debug output
        UART2_WriteNoneBlockingString(debugBuffer); 
    #endif
    UART1_WriteNoneBlockingString(buffer);
}

void behandlungsfunction(char * line){
    // SCC 3.88[00]
    static char command[255];  
    static char stringCommandvalue [255];
    char from_address[10];
    #if defined(DEBUG) && defined(DEBUG_SERIAL_COMMAND_HANDLING)
        char debugBuffer[255];
        sprintf(debugBuffer,"\n\r%sin:%s%s\r\n",COL_YELLOW,line,COL_WHITE);
        UART2_WriteNoneBlockingString(debugBuffer); 
    #endif
    int32_t intCommandValue = 0;
    float floatCommandValue = 0;
    int16_t posSeparator = getSeparatorposition(line);
    if (posSeparator>0){
        line[posSeparator]='\0';
        char * myadress = line;
        line = line + posSeparator + 1; //point to the value part
        posSeparator = getSeparatorposition(line);
        line[posSeparator]='\0';
        strcpy(from_address,line);

        if(strcmp(myadress,(char*)g.myaddress)!=0){          // reaction for incorrect address
            #if defined(DEBUG) && defined(DEBUG_SERIAL_COMMAND_HANDLING)
                sprintf(debugBuffer, "%sNot addressed%s\r\n",COL_RED,COL_WHITE);  // rot
                UART2_WriteNoneBlockingString(debugBuffer); 
            #endif
            return;
        }
        #if defined(DEBUG) && defined(DEBUG_SERIAL_COMMAND_HANDLING)
            sprintf(debugBuffer, "%sAddressed%s\r\n",COL_GREEN,COL_WHITE); //grün
            UART2_WriteNoneBlockingString(debugBuffer); 
        #endif
        line = line + posSeparator + 1; //point to the value part
        posSeparator = getSeparatorposition(line);

        if(posSeparator>0){ // command with value
            line[posSeparator]='\0';
            strcpy(command,line);
            line = line + posSeparator + 1; //point to the value part
            floatCommandValue = atof(line); //convert from string to float
            strcpy(stringCommandvalue,line);
            intCommandValue = atoi(line); //convert from string to integer
        }
        else{ // command without value
            strcpy(command,line);
        }

        if (strcmp(command,"*TBC!")==0){        // Test Bus Connection
            sendoutput(1, from_address);  // 1 correct
        }
        else if (strcmp(command,"*SDD!")==0){   // Shut Down Device 
            sendoutput(1, from_address);  // 1 correct
        }
        else if (strcmp(command,"*GAW!")==0){   // Groyßergleichmethode
            sendoutput(1, from_address);  // 1 correct
        }
        else if (strcmp(command,"*RTW?")==0){   // Reset Warnings, get bool to reset
            sendoutput(1, from_address);  // 1 correct
        }
        else if (strcmp(command,"*GDM?")==0){   // Get Drive Mode, bool...
            sendoutput(1, from_address);  // 1 correct
        }
        else if (strcmp(command,"*GDD?")==0){   // Get Drive Direction, bool...
            sendoutput(1, from_address);  // 1 correct
        }
        else if (strcmp(command,"*GAM?")==0){   // Get Automatic Mode, bool...
            sendoutput(1, from_address);  // 1 correct
        }
        else if (strcmp(command,"*SCC?")==0){   // Save Charging Current
            sendoutput(1, from_address);  // 1 correct
        }
        else if (strcmp(command,"*SDC?")==0){   // Save Discharging Current
            Drive_SetCurrentLimit(intCommandValue);
            sendoutput(1, from_address);  // 1 correct
        }
        else if (strcmp(command,"*RPM?")==0){   // Groyßergleichmethode, get speed in rpm
            sendoutput(1, from_address);  // 1 correct
        }
        else{
            sendoutput(0, from_address);  // 0 not correct
        }
    }
}

void SerialCommandRxService(void){
    char rxChar;
    #if defined(DEBUG) && defined(DEBUG_SERIAL_COMMAND_HANDLING)
        char debugBuffer[256];
    #endif
    static char line[256];  
    static uint8_t pointer = 0;
    // printf(".");
    while(UART1_RxBufferedAvailable())
    {   
        // RingBuffer_Read(&g.read_buffer,&rxChar);
        rxChar = UART1_RxBufferedReadByte();
        
        if (!rxChar) continue;
        #if defined(DEBUG) && defined(DEBUG_SERIAL_COMMAND_HANDLING)
            sprintf(debugBuffer,"+%c",rxChar);
            UART2_WriteNoneBlockingString(debugBuffer); 
        #endif
        line[pointer] = (rxChar==13 || rxChar==10)?0x00:rxChar;
        if (line[pointer]){
            if (++pointer > (256 - 1)) pointer--; // avoids buffer overflow 
            continue;
        }
        behandlungsfunction(line);
        #if defined(DEBUG) && defined(DEBUG_SERIAL_COMMAND_HANDLING)
            sprintf(debugBuffer,"%s*%s",COL_VIOLETT,COL_WHITE);
            UART2_WriteNoneBlockingString(debugBuffer); 
        #endif
        pointer = 0;
    }
}

void SerialCommandTxService(void){
    // sending with enable pin 
    char c;
  
    static uint8_t state = 0;
    static uint8_t counter = 0;
    switch (state){
        // initial
        case 0: if (UART1_TxBufferedAvailable()) state=1;
                break;
        // set enable
        case 1: TXENABLE_SetHigh();
                counter = 0;
                state = 2;
                break; 
        // wait to switch for secure
        case 2: counter++;
                if (counter == 10) {
                    state = 3;
                    counter = 0;
                }
                break;
        // write uartdata U1STAHbits.UTXBF
        case 3: while (UART1_TxBufferedAvailable() && (!U1STAHbits.UTXBF)){                  
                    c = UART1_TxReadBufferedByte();
                    if (c) U1TXREG = c;
                }//end while
                if (!UART1_TxBufferedAvailable()) state = 4;
                break;

        // wait last byte written
        case 4: if (U1STAbits.TRMT) state = 5;
                break;
        // switch off
        case 5: TXENABLE_SetLow();
                state = 0;
                break;
    }
}