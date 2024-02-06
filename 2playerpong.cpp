

/*
  A simple Pong game:
 */

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define UP_BUTTON 2
#define DOWN_BUTTON 3

#define UP_BUTTON2 4
#define DOWN_BUTTON2 5

const unsigned long PADDLE_RATE = 63;
const unsigned long BALL_RATE = 0.01;
const uint8_t PADDLE_HEIGHT = 24;

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void drawCourt();

uint8_t ball_x = 64, ball_y = 32;
uint8_t ball_dir_x = 1, ball_dir_y = 1;
unsigned long ball_update;

unsigned long paddle_update;
const uint8_t player_2x = 12;
uint8_t player_2y = 16;

const uint8_t PLAYER_X = 115;
uint8_t player_y = 16;

void setup() {
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

    
    display.display();
    unsigned long start = millis();

    pinMode(UP_BUTTON, INPUT);
    pinMode(DOWN_BUTTON, INPUT);
    digitalWrite(UP_BUTTON,1);
    digitalWrite(DOWN_BUTTON,1);

    //player 2
    pinMode(UP_BUTTON2, INPUT);
    pinMode(DOWN_BUTTON2, INPUT);
    digitalWrite(UP_BUTTON2,1);
    digitalWrite(DOWN_BUTTON2,1);
    display.clearDisplay();
    drawCourt();

    while(millis() - start < 2000);

    display.display();

    ball_update = millis();
    paddle_update = ball_update;
}

void gameWin(){
  display.clearDisplay();
  while(1){
  display.drawRect(55, 0, 2, 32, WHITE);
  display.drawRect(56, 31, 14, 2, WHITE);
  }
}

void loop() {
    bool update = false;
    unsigned long time = millis();

    static bool up_state = false;
    static bool down_state = false;

    static bool up_state2 = false;
    static bool down_state2 = false;

    up_state |= (digitalRead(UP_BUTTON) == LOW);
    down_state |= (digitalRead(DOWN_BUTTON) == LOW);

    up_state2 |= (digitalRead(UP_BUTTON2) == LOW);
    down_state2 |= (digitalRead(DOWN_BUTTON2) == LOW);

    if(time > ball_update) {
        uint8_t new_x = ball_x + ball_dir_x;
        uint8_t new_y = ball_y + ball_dir_y;

        // Check if we hit the vertical walls
        if(new_x == 0 || new_x == 127) {
            gameWin();
        }

        // Check if we hit the horizontal walls.
        if(new_y == 0 || new_y == 63) {
            ball_dir_y = -ball_dir_y;
            new_y += ball_dir_y + ball_dir_y;
        }

        // Check if we hit the CPU paddle
        if(new_x == player_2x && new_y >= player_2y && new_y <= player_2y + PADDLE_HEIGHT) {
            ball_dir_x = -ball_dir_x;
            new_x += ball_dir_x + ball_dir_x;
        }

        // Check if we hit the player paddle
        if(new_x == PLAYER_X
           && new_y >= player_y
           && new_y <= player_y + PADDLE_HEIGHT)
        {
            ball_dir_x = -ball_dir_x;
            new_x += ball_dir_x + ball_dir_x;
        }

        display.drawPixel(ball_x, ball_y, BLACK);
        display.drawPixel(new_x, new_y, WHITE);
        ball_x = new_x;
        ball_y = new_y;

        ball_update += BALL_RATE;

        update = true;
    }

        if(time > ball_update) {
        
        uint8_t new_x = ball_x + ball_dir_x;
        uint8_t new_y = ball_y + ball_dir_y;

        if(new_x == 0 || new_x == 127) {
            gameWin();
        }
        display.drawFastVLine(player_2x, player_2y, PADDLE_HEIGHT, BLACK);
        if(up_state2) {
            player_2y -= 1;
        }
        if(down_state2) {
            player_2y += 1;
        }
        up_state2 = down_state2 = false;
        if(player_2y < 1) player_2y = 1;
        if(player_2y + PADDLE_HEIGHT > 63) player_2y = 63 - PADDLE_HEIGHT;
        display.drawFastVLine(player_2x, player_2y, PADDLE_HEIGHT, WHITE);    
          
          

        // Check if we hit the horizontal walls.
        if(new_y == 0 || new_y == 63) {
            ball_dir_y = -ball_dir_y;
            new_y += ball_dir_y + ball_dir_y;
        }

        //player 1
        display.drawFastVLine(PLAYER_X, player_y, PADDLE_HEIGHT, BLACK);
        if(up_state) {
            player_y -= 1;
        }
        if(down_state) {
            player_y += 1;
        }
        up_state = down_state = false;
        if(player_y < 1) player_y = 1;
        if(player_y + PADDLE_HEIGHT > 63) player_y = 63 - PADDLE_HEIGHT;
        display.drawFastVLine(PLAYER_X, player_y, PADDLE_HEIGHT, WHITE);

        // // Check if we hit the CPU paddle
        // if(new_x == player_2x && new_y >= player_2y && new_y <= player_2y + PADDLE_HEIGHT) {
        //     ball_dir_x = -ball_dir_x;
        //     new_x += ball_dir_x + ball_dir_x;
        // }

        // Check if we hit the player paddle
        if(new_x == player_2x
           && new_y >= player_2y
           && new_y <= player_2y + PADDLE_HEIGHT)
        {
            ball_dir_x = -ball_dir_x;
            new_x += ball_dir_x + ball_dir_x;
        }

        display.drawPixel(ball_x, ball_y, BLACK);
        display.drawPixel(new_x, new_y, WHITE);
        ball_x = new_x;
        ball_y = new_y;

        ball_update += BALL_RATE;

        update = true;
    }

    if(update)
        display.display();
}


void drawCourt() {
    display.drawRect(0, 0, 128, 64, WHITE);
}