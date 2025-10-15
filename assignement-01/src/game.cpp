#include "game.h"
#include "setup.h"
#include <Arduino.h>

bool gameactivation = true;
int seq[4];         


void generateSequence(int seq[4]) {
  int pool[4] = {1, 2, 3, 4};
  for (int i = 3; i > 0; --i) {
    int j = random(i + 1);
    int tmp = pool[i];
    pool[i] = pool[j];
    pool[j] = tmp;
  }
  for (int i = 0; i < 4; i++) seq[i] = pool[i];
}

void gameModeOperations() {
  if(gameactivation) {
    generateSequence(seq);
    gameactivation = false;
  }
}


