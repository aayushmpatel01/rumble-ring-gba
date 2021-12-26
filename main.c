#include "main.h"
#include "images/player.h" //includes player art
#include "images/opponent.h" //includes opponent art
#include "images/ring.h" //includes in-game background art
#include "images/start.h" //includes start screen background art
#include "images/win.h" //includes win screen background art


#include <stdio.h>
#include <stdlib.h>

#include "gba.h"

enum gba_state {
  START,
  PLAY,
  WIN,
  LOSE,
};

extern int points;
extern int timer;


u32 *oldVBlankCounter = &vBlankCounter;

int main(void) {
  /* TODO: */
  // Manipulate REG_DISPCNT here to set Mode 3. //
	REG_DISPCNT = MODE3 | BG2_ENABLE;

  struct opponent opps[6];

  struct player1 p;

  p.row = 80;
  p.col = 120;
  p.rowDisp = 1;
  p.colDisp = 1;

  for (int i = 0; i < 6; i++) {
    opps[i].row = randint(12, 148);
    opps[i].col = randint(12, 228);
    opps[i].alive = 1;
    opps[i].move = randint(0, 5);
  }

  // Save current and previous state of button input.
  u32 previousButtons = BUTTONS;
  u32 currentButtons = BUTTONS;

  // Load initial application state
  enum gba_state state = START;

  while (1) {
    currentButtons = BUTTONS; // Load the current state of the buttons

  

    switch (state) {
      case START:
        waitForVBlank();
        drawFullScreenImageDMA(start); //draws full screen of start image 
        drawCenteredString(120, 100, 20, 20, "Press start to play!", WHITE);
        if (KEY_DOWN(BUTTON_START, currentButtons)) {
          timer = 0;
          points = 0;
          for (int i = 0; i < 6; i++) {
            opps[i].alive = 1;
          }
          state = PLAY;
        }
        if (KEY_DOWN(BUTTON_SELECT, currentButtons)) {
          timer = 0;
          points = 0;
          for (int i = 0; i < 6; i++) {
            opps[i].alive = 1;
          }
          state = START;
        }
        break;
      case PLAY:
        waitForVBlank();
        fillScreenDMA(BLACK);
        char timeLeft[9];
        snprintf(timeLeft, 32, "Time: %i", ((1800-timer)/60));
        drawCenteredString(120, 50, 20, 20, timeLeft, WHITE);
        if (KEY_DOWN(BUTTON_RIGHT, currentButtons)) {
          if (p.col + 1 < WIDTH - P_WIDTH) {
            p.col++;
          }
        }

        if (KEY_DOWN(BUTTON_LEFT, currentButtons)) {
          if (p.col - 1 > 0) {
            p.col--;
          }
        }

        if (KEY_DOWN(BUTTON_UP, currentButtons)) {
          if (p.row - 25 > 0) {
            p.row--;
          }
        }

        if (KEY_DOWN(BUTTON_DOWN, currentButtons)) {
          if (p.row + 1 < HEIGHT - P_HEIGHT) {
            p.row++;
          }
        }

        drawImageDMA(p.row, p.col, P_WIDTH, P_HEIGHT, player);

        for (int i = 0; i < 6; i++) {
          if (opps[i].move == 0) {
            opps[i].row -= 1;
          } else if (opps[i].move == 1) {
            opps[i].row += 1;
          } else if (opps[i].move == 2) {
            opps[i].col += 1;
          } else if (opps[i].move == 3) {
            opps[i].col -= 1;
          } else if (opps[i].move == 4) {
            opps[i].row -= 1;
            opps[i].col += 1;
          } else if (opps[i].move == 5) {
            opps[i].row += 1;
            opps[i].col -= 1;
          }
        }

        for (int i = 0; i < 6; i++) {
          if (opps[i].move == 0 && opps[i].row <= 30) {
            opps[i].move = 1;
          } else if (opps[i].move == 1 && opps[i].row >= HEIGHT - O_HEIGHT - 1) {
            opps[i].move = 0;
          } else if (opps[i].move == 2 && opps[i].col >= WIDTH - O_WIDTH - 1) {
            opps[i].move = 3;
          } else if (opps[i].move == 3 && opps[i].col <= 1) {
            opps[i].move = 2;
          } else if (opps[i].move == 4 && ((opps[i].row <= 30) || (opps[i].col >= WIDTH - O_WIDTH - 1))) {
            opps[i].move = 5;
          } else if (opps[i].move == 5 && ((opps[i].row >= HEIGHT - O_HEIGHT - 1) || (opps[i].col <= 1))) {
            opps[i].move = 4;
          }
        }

        for (int i = 0; i < 6; i++) {
          if (opps[i].alive != 0) {
            drawImageDMA(opps[i].row, opps[i].col, 7, 12, opponent); //draws 6 opponents
          }
        }

        for (int i = 0; i < 6; i++) {
          if (opps[i].alive != 0 && (opps[i].row - p.row <= 5 && opps[i].row - p.row >= -5) && (opps[i].col - p.col <= 5 && opps[i].col - p.col >= -5)) {
            opps[i].alive = 0;
            points++;
          }
        }

        if (timer == 1800) {
          state = LOSE;
        }

        if (points == 6) {
          state = WIN;
        }

        timer++;

        if (KEY_DOWN(BUTTON_SELECT, currentButtons)) {
          timer = 0;
          state = START;
        }

        break;
      case WIN:
        waitForVBlank();
        drawFullScreenImageDMA(win);
        drawCenteredString(140, 100, 20, 20, "You win! Press select to replay!", WHITE);
        if (KEY_DOWN(BUTTON_SELECT, currentButtons)) {
          timer = 0;
          points = 0;
          for (int i = 0; i < 6; i++) {
            opps[i].alive = 1;
          }
          state = START;
        }

        break;
      case LOSE:
        waitForVBlank();
        fillScreenDMA(BLACK);
        drawCenteredString(120, 100, 20, 20, "You lose! Press select to replay!", WHITE);
        if (KEY_DOWN(BUTTON_SELECT, currentButtons)) {
          timer = 0;
          points = 0;
          for (int i = 0; i < 6; i++) {
            opps[i].alive = 1;
          }
          state = START;
        }
        break;
    }

    previousButtons = currentButtons; // Store the current state of the buttons
  }

  UNUSED(previousButtons); // You can remove this once previousButtons is used

  return 0;
}
