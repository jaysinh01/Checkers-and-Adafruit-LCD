#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#include <Adafruit_ILI9341.h>
#include <Adafruit_GFX.h>
#include <TouchScreen.h>
#define TFT_DC 9
#define TFT_CS 10
#define SD_CS 6

#define YP A2  // must be an analog pin, use "An" notation!
#define XM A3  // must be an analog pin, use "An" notation!
#define YM  5  // can be a digital pin
#define XP  4  // can be a digital pin

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);
// thresholds to determine if there was a touch
#define MINPRESSURE   10
#define MAXPRESSURE 1000

#define DISPLAY_WIDTH  320
#define DISPLAY_HEIGHT 240
#define LCD_WIDTH 2048

#define JOY_VERT  A1 // should connect A1 to pin VRx
#define JOY_HORIZ A0 // should connect A0 to pin VRy
#define JOY_SEL   2

#define JOY_CENTER   512
#define JOY_DEADZONE 64

#define CURSOR_SIZE 9
#define JOY_SPEED 10

uint16_t BROWN = tft.color565(139,69,19);
#define BLACK    0x0000
#define BLUE     0x001F
#define RED      0xF800
#define GREEN    0x07E0
#define CYAN     0x07FF
#define MAGENTA  0xF81F
#define YELLOW   0xFFE0 
#define WHITE    0xFFFF

int cursorX, cursorY;
int selectedx=0;
int selectedy=0;
int x1 = -1;
int y1 = -1;
int x2 = -1;
int y2 = -1;
int player = 0;
const uint16_t buf_size = 256;
uint16_t buf_len = 0;
char* buffer = (char *) malloc(buf_size);
char* buffer2 = (char *) malloc(buf_size);
int mode=0;
int finish=0;
int buttonVal;

//drawpieces function : draws the red and white checker pieces on the screen
void drawpieces()
{
    int w = DISPLAY_WIDTH / 8;
    int h = DISPLAY_HEIGHT / 8;
    int i=0;
    while(i<8)
    {
        if(i==3)
            i=5;
        for(int j=0;j<8;j++)
        {
            if((i+j)%2!=0 && i<5)
                tft.fillCircle((j * w)+DISPLAY_WIDTH/16, (i * h)+DISPLAY_HEIGHT/16 ,10, WHITE);
            else if((i+j)%2!=0 && i>=5)
                tft.fillCircle((j * w)+DISPLAY_WIDTH/16, (i * h)+DISPLAY_HEIGHT/16 ,10, RED);

        }
        i++;
    }


}

//drawboard function: draw the checker board made of white and black boxes on the screen.
void drawBoard()
{
  int w = DISPLAY_WIDTH / 8;
  int h = DISPLAY_HEIGHT / 8;

  for(int i=0;i<8;i++)
  {
    for(int j=0;j<8;j++)
    {
        if((i+j)%2==0)
            tft.fillRect(j * w, i * h , w, h, WHITE);
        else
            tft.fillRect(j * w, i * h, w, h, BLACK);
    }
  }
}

//redrawSelected function: Draws the highlighted box for the joystick position, making it able to track its movement.
void redrawSelected(int OldX, int OldY, int newX, int newY) {
  if ( (((8 * OldX) / DISPLAY_WIDTH) % 2 == 0 && ((8 * OldY) / DISPLAY_HEIGHT) % 2 == 0) || (((8 * OldX) / DISPLAY_WIDTH) % 2 != 0 && ((8 * OldY) / DISPLAY_HEIGHT) % 2 != 0) ) {
    tft.drawRect(OldX, OldY, DISPLAY_WIDTH / 8, DISPLAY_HEIGHT / 8, WHITE);
    tft.drawRect(OldX+1, OldY+1, DISPLAY_WIDTH / 8, DISPLAY_HEIGHT / 8, WHITE);
    tft.drawRect(OldX+2, OldY+2, DISPLAY_WIDTH / 8, DISPLAY_HEIGHT / 8, WHITE);
  }
  else if ( (((8 * OldX) / DISPLAY_WIDTH) % 2 != 0 && ((8 * OldY) / DISPLAY_HEIGHT) % 2 == 0) || (((8 * OldX) / DISPLAY_WIDTH) % 2 == 0 && ((8 * OldY) / DISPLAY_HEIGHT) % 2 != 0) ) {
    tft.drawRect(OldX, OldY, DISPLAY_WIDTH / 8, DISPLAY_HEIGHT / 8, BLACK);
    tft.drawRect(OldX+1, OldY+1, DISPLAY_WIDTH / 8, DISPLAY_HEIGHT / 8, BLACK);
    tft.drawRect(OldX+2, OldY+2, DISPLAY_WIDTH / 8, DISPLAY_HEIGHT / 8, BLACK);

  }

  tft.drawRect(newX, newY, DISPLAY_WIDTH / 8, DISPLAY_HEIGHT / 8, RED);
  tft.drawRect(newX+1, newY+1, DISPLAY_WIDTH / 8, DISPLAY_HEIGHT / 8, RED);
  tft.drawRect(newX+2, newY+2, DISPLAY_WIDTH / 8, DISPLAY_HEIGHT / 8, RED);

  selectedx = newX/(DISPLAY_WIDTH / 8);
  selectedy  = newY/(DISPLAY_HEIGHT / 8);
  //delay(300);
}

//highlight function: makes a blue box on the selected box when the joystick is pressed.
void highlight(int x,int y)
{
    tft.drawRect(x*DISPLAY_WIDTH/8, y*DISPLAY_HEIGHT/8, DISPLAY_WIDTH / 8, DISPLAY_HEIGHT / 8, CYAN);
    tft.drawRect((x)*DISPLAY_WIDTH/8+1, (y)*DISPLAY_HEIGHT/8+1, DISPLAY_WIDTH / 8, DISPLAY_HEIGHT / 8, CYAN);
    tft.drawRect((x)*DISPLAY_WIDTH/8+2, (y)*DISPLAY_HEIGHT/8+2, DISPLAY_WIDTH / 8, DISPLAY_HEIGHT / 8, CYAN);
}

//setup function: draws the game and initalizes the needed variables.
void setup() {
  init();
  Serial.begin(9600);
  pinMode(JOY_SEL, INPUT_PULLUP);
  tft.begin();
  if (!SD.begin(SD_CS)) {
      while (true) {}
  }

  tft.setRotation(3);
  tft.fillScreen(WHITE);

  
  cursorX = 0;
  cursorY = 5*(DISPLAY_HEIGHT/8);
  int OldX = 0;
  int OldY = 0;

  drawBoard();
  drawpieces();

  redrawSelected(OldX, OldY, cursorX, cursorY);
  delay(300);
}

//readline function: reads the line into an char array stopping at /n or /r.
uint16_t readline(char buffer[])
{
    uint16_t buf_len = 0;
    while(true)
    {   
        char in_char;

        if (Serial.available()) 
        {
          // read the incoming byte:
          char in_char = Serial.read();

          // if end of line is received, waiting for line is done:
          if (in_char == '\n' || in_char == '\r') {
              // now we process the buffer
            Serial.println(buffer);
            return buf_len;
              }
          else {
              // add character to buffer, provided that we don't overflow.
              // drop any excess characters.
              if ( buf_len < buf_size-1 ) {
                  buffer[buf_len] = in_char;
                  buf_len++;
                  buffer[buf_len] = 0;

              }
            }
        }

    }
}

//redrawcoin function: draws the checker piece again when move is executed.
void redrawcoin(int x1, int x2,int y1,int y2,uint16_t color)
{
    int w = DISPLAY_WIDTH / 8;
    int h = DISPLAY_HEIGHT / 8;
    tft.fillRect(x1* w, y1 * h, w, h, BLACK);
    tft.fillCircle((x2 * w)+DISPLAY_WIDTH/16, (y2 * h)+DISPLAY_HEIGHT/16 ,10, color);

 

}

void deletepiece(int x1, int x2,int y1,int y2,uint16_t color)
{
    int w = DISPLAY_WIDTH / 8;
    int h = DISPLAY_HEIGHT / 8;
    tft.fillRect((x1+(x2-x1)/2)* w, (y1+(y2-y1)/2) * h, w, h, BLACK);

}

//aipoints function: gets the move coordinates from the server for the AI.
void get_aipoints(int coords[]) {
  // Variable to hold what is read from serial-mon
  char in_char;
  // A struct that holds the longtitude and latitude, will be inserted
  // into the shared.waypoints[] array
  int i=0;
  while(true) {
    if (Serial.available()) {
      // read the incoming byte:
      char in_char = Serial.read();

      // if space is received, we know the latitude has been received
      if (in_char == ' ') {
        // insert the latitude into struct after converting it into an int
        coords[i] = atoi(buffer2);
        // Reset the buffer
        i++;
        buf_len = 0;
        buffer2[buf_len] = 0;
      }
      // if newline is received, we know the longitude has been received 
      else if(in_char == '\n') {
        // insert the longitude into struct after converting it into an int
        coords[i] = atoi(buffer2);
        // Reset the buffer
        i++;
        buf_len = 0;
        buffer2[buf_len] = 0;
        break;
      }
      else {
        // add character to buffer, provided that we don't overflow.
        // drop any excess characters.
        if ( buf_len < buf_size-1 && (in_char != '\t' || in_char != '\0')) {
            buffer2[buf_len] = in_char;
            buf_len++;
            buffer2[buf_len] = 0;
        }
      }
    }
  }
}

  // insert the struct into the array
bool wait_for_ack(char ack, int timeout) {
  // Variable to hold what is read from serial-mon
  char in_char;
  // Holds the start time
  long long start = millis();
  // While time has not exceeded the timeout
  while((millis() - start) <= timeout) {
    if (Serial.available()) {
      // read the incoming byte:
      char in_char = Serial.read();

      // if end of line, space or carriage return  is received,
      // waiting for line is done:
      if (in_char == ' ' || in_char == '\n' || in_char == '\r') {
        // checks to see if the acknowledgement recieved was the
        // correct one
        if(buffer[buf_len-1] == ack) {
          // Resets the buffer
          buf_len = 0;
          buffer[buf_len] = 0;
          // Returns true
          return true;
        }
        // Resets Buffer
        buf_len = 0;
        buffer[buf_len] = 0;
      }
      else {
        // add character to buffer, provided that we don't overflow.
        // drop any excess characters.
        if ( buf_len < buf_size-1 ) {
            buffer[buf_len] = in_char;
            buf_len++;
            buffer[buf_len] = 0;
        }
      }
    }
  }
  // Resets Buffer
  buf_len = 0;
  buffer[buf_len] = 0;
  return false;
}


void processJoystick() 
{
    int xVal = analogRead(JOY_HORIZ);
    int yVal = analogRead(JOY_VERT);
    buttonVal = digitalRead(JOY_SEL);
    int OldX = cursorX;
    int OldY = cursorY;
    bool move = false;

    if (yVal < JOY_CENTER - JOY_DEADZONE) {
    move = true;
    cursorY -= DISPLAY_HEIGHT / 8;  
    }

    else if (yVal > JOY_CENTER + JOY_DEADZONE) {
    move = true;
    cursorY += DISPLAY_HEIGHT / 8;
    }

    if (xVal > JOY_CENTER + JOY_DEADZONE) {
    move = true;
    cursorX -= DISPLAY_WIDTH / 8;  
    }

    else if (xVal < JOY_CENTER - JOY_DEADZONE) {
    move = true;
    cursorX += DISPLAY_WIDTH / 8;   
    }

    cursorX = constrain(cursorX, 0, DISPLAY_WIDTH - DISPLAY_WIDTH / 8);
    cursorY = constrain(cursorY, 0, DISPLAY_HEIGHT - DISPLAY_HEIGHT / 8);
    if(move)
    redrawSelected(OldX, OldY, cursorX, cursorY);
    delay(100);

  
} 

//machine: simple state machine
void machine()
{
    int hold=0;
    switch(mode)
    {
        case 0:
        {
            processJoystick();
            if(digitalRead(JOY_SEL)==HIGH &&hold==1)
                hold=0;
            if(buttonVal==LOW && hold==0)
            {
                hold=1;
                if(finish)
                {
                    x2=selectedx;
                    y2=selectedy;
                    highlight(x2,y2);
                    mode=1;
                    finish=0;
                    break;
                    
                    x1 = -1;
                    y1 = -1;
                    x2 = -1;
                    y2 = -1;
                }

                else if(!finish)
                {
                    x1=selectedx;
                    y1=selectedy;
                    highlight(x1,y1);
                    finish=1;
                    delay(300);
                }
            }
            delay(100);
            break;
        }

        case  1:
        {
            Serial.println("M");
            Serial.println(x1);
            Serial.println(y1);
            Serial.println(x2);
            Serial.println(y2);
            bool con = wait_for_ack('B', 1000);
            if(con)
            {
                int props[2];
                get_aipoints(props);
                if(props[0])
                {
                    redrawcoin(x1,x2,y1,y2,YELLOW);

                }
                else
                    redrawcoin(x1,x2,y1,y2,RED);
                if(props[1])
                {
                    deletepiece(x1,x2,y1,y2,RED);

                }
                delay(300);

            }

            bool go = wait_for_ack('A', 1000);
            if(go)
            {
                int coords[6];
                get_aipoints(coords);
                if(coords[4])
                {
                    redrawcoin(int(coords[0]),int(coords[2]),int(coords[1]),int(coords[3]),YELLOW);

                }
                else
                    redrawcoin(int(coords[0]),int(coords[2]),int(coords[1]),int(coords[3]),WHITE);
                if(coords[5])
                {
                    deletepiece(int(coords[0]),int(coords[2]),int(coords[1]),int(coords[3]),WHITE);

                }

            }
            Serial.println('A');

            mode=0;

        }
    }   

    
    
}
     

int main() {
  setup();
  
  while (true) {
    machine();
  }

  Serial.end();
  return 0;
}