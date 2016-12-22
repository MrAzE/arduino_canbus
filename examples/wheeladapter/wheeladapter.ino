// demo: Wheel Adapter
#include <mcp_can.h>
#include <SPI.h>

long unsigned int rxId;
unsigned char len = 0;
unsigned char rxBuf[8];



MCP_CAN CAN0(9);                               // Set CS CAN ON PIN=9

//variables
long NXTbut = 0;
long PREVbut = 0;
long Depress = 0;
boolean forcebutton = 0; //Should we force button change
byte lastpressed = 0; // The last button pressed


//VAR PARAMS
long interv = 1000; 
long presstime = 1000;
short int DEBUGMODE = 1; //Debug mode level 0-off 1-basic debug 2-extended debug messages




void setup()
{
  Serial.begin(115200);
  delay(100);
  CAN0.begin(CAN_33K3BPS,2);                       // init can bus : baudrate = 33.3k 
  pinMode(2, INPUT);  // Setting pin 2 for /INT input
  Serial.println("Test Wheel Adapter");
}

void loop()
{
    if(Depress!=0){ if((millis()-Depress)>=presstime) {buttonPress(0,0);}; }; //Reset any active button press emulations if needed.
    if(!digitalRead(2))                         // If pin 2 is low, read receive buffer
    {
      CAN0.readMsgBuf(&len, rxBuf);              // Read data: len = data length, buf = data byte(s)
      rxId = CAN0.getCanId();                    // Get message ID
      if (DEBUGMODE==2)                          // Check if extended Debug mode is on
      
      { 

        Serial.print("ID: ");
        Serial.print(rxId, HEX);
        Serial.print("  Data: ");
        for(int i = 0; i<len; i++)                // Print each byte of the data
        {
          if(rxBuf[i] < 0x10)                     // If data byte is less than 0x10, add a leading zero
          {
            Serial.print("0");
          }
        Serial.print(rxBuf[i], HEX);
        Serial.print(" ");
        }
        Serial.println();
      }
      
      if (rxId == 0x290) {
        if (rxBuf[3]!=lastpressed){ 
             if (DEBUGMODE) {Serial.print("Button positions differ from previous,  - "); Serial.println(rxBuf[3],HEX);}
                  lastpressed=rxBuf[3];
                  switch (lastpressed) {
              //volup
              case 1:
              if (DEBUGMODE) {Serial.println("VolUp Button pressed");}
              buttonPress(1,0);
              break;
              //voldown
              case 2:
              if (DEBUGMODE) {Serial.println("VolDown Button pressed");}
              buttonPress(2,0);
              break;
              //PhoneUp
              case 0x11:
              if (DEBUGMODE) {Serial.println("PhoneUp Button");}
              buttonPress(3,0);
              break;
              //PhoneDown
              case 0x12:
              buttonPress(4,0);
              if (DEBUGMODE) {Serial.println("PhoneDown Button");}
              break;
              //nexttrack
              case 3:
              if (DEBUGMODE) {Serial.println("NextTrackButton Pressed");}
              NXTbut=millis();
              break;
              //prevtrack
              case 4:
              if (DEBUGMODE) {Serial.println("PreviousTrackButton");}
              PREVbut=millis();
              break;
              
              //Button released on wheels.
              case 0:
              if (DEBUGMODE) {Serial.println("Button released");}
              
              if (NXTbut!=0) //Next button is depressed
              {
                if ((millis()-NXTbut)>interv) //Check how long button @next@ was pressed
                {
                 buttonPress(5,1);             
                 if (DEBUGMODE) {Serial.println("NXT bigger");};
                }
                else
                {
                 buttonPress(6,1);
                 if (DEBUGMODE) {Serial.println("NXT smaller");};
                }  
              NXTbut=0;
              }
              
              
                if (PREVbut!=0) //Отпустили кнопку PREV
              {
                if ((millis()-PREVbut)>interv) //Check how long button @prev@ was pressed
                {
                buttonPress(7,1);
                if (DEBUGMODE) {Serial.println("PREV bigger than ");};
                }
                else
                {
                 buttonPress(8,1);
                if (DEBUGMODE) {Serial.println("PREV smaller ");};
                }

             PREVbut=0; 
             }
              lastpressed=0;
               if (NXTbut==0&&PREVbut==0) 
               PREVbut=0;
               NXTbut=0;
              
              break;
              //Unknown button was pressed
              default: 
              if (DEBUGMODE) {Serial.println("Unknown button");}
              break;
              }

            } //End of buttons checking
      } //End of cim message checking

      if (rxId == 0x060) { //If message is this one than this messages goes with info from ISM
            if (DEBUGMODE) {Serial.println("Message with ISM info ");}
            if (rxBuf[1]==0x00){
             if (DEBUGMODE) {Serial.println("Key is taken out");}  
              //Here we should place action which should be taken 
            }
      }//End of ISM message
      
    }// End of If recieved new can message end.
}




void buttonPress(byte BTN, boolean tPress) //This function will be used with digital potentiometer to emulate resistive buttons
{
if (tPress==0) {
              Depress = 0;
              }//Depress button if button works without the delay
              else
              {
              Depress=millis();
              }//Set the time of the button depress to know the start time of button press emulation.
switch (BTN)
{
case 1:

break;

case 2:

break;

case 3:
break;

case 4:
break;

case 5:
break;

case 6:
break;

case 7:
break;

case 8:
break;

case 0:
break;
}
}



/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
