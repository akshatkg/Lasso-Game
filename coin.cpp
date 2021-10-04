#include <simplecpp>
#include <string>
#include <cstdlib>
#include "MovingObject.h"
#include "lasso.h"
#include "coin.h"
using namespace simplecpp;

// initialising coin variables
void Coin::initCoin(unsigned int r, unsigned int g, unsigned int b) {
  if(r == 0 && g == 0 && b == 0) // conditions to be executed if coins is black
  {
    int x_add = rand() % 200 + 50; // randomising x coordinate
    int mult = pow(-1, x_add);
    int x_add_final = mult * x_add;
    coin_start_x = (PLAY_X_START+WINDOW_X)/2 + x_add_final ; // initialising x cooridnate of bomb (black coin)
    int coin_rad = 3 ;
    coin_start_y = PLAY_Y_HEIGHT;
    coin_circle.reset(coin_start_x, coin_start_y, COIN_SIZE  + coin_rad);
  }
  else
  {
    coin_start_y = PLAY_Y_HEIGHT;
    coin_start_x = (PLAY_X_START+WINDOW_X)/2 ;
    coin_circle.reset(coin_start_x, coin_start_y, COIN_SIZE );
  }
  coin_circle.setColor(COLOR(r, g, b)); // set colour of coin to respective r, g, b values
  coin_circle.setFill(true);
  addPart(&coin_circle);
}

// reset parameters of the coin
void Coin::resetCoin() {
  double coin_speed = COIN_SPEED;
  double coin_angle_deg = COIN_ANGLE_DEG;
  coin_ax = 0;
  coin_ay = COIN_G;
  bool paused = true, rtheta = true;
  reset_all(coin_start_x, coin_start_y, coin_speed, coin_angle_deg, coin_ax, coin_ay, paused, rtheta);
}

// reset parameters of coin such that it moves in a parabolic path
void Coin::resetCoin2() {
  double coin_speed = COIN_SPEED;
  int rand_angle;
  int raise;
  raise = rand() % 10;
  int mult = pow(-1, raise);
  if(mult < 0){
    rand_angle = mult * rand()%20;
  }
  else {
    rand_angle = mult * rand()%60;
  }
  double coin_angle_deg = COIN_ANGLE_DEG + rand_angle;
  coin_ax = 0;
  coin_ay = COIN_G;
  bool paused = true, rtheta = true;
  reset_all(coin_start_x + raise*10, coin_start_y, coin_speed, coin_angle_deg, coin_ax, coin_ay, paused, rtheta);
}

void Coin::resetCoinColor(unsigned int r, unsigned int g, unsigned int b){ // reset colour of the coin to respective r,g,b values
  coin_circle.setColor(COLOR(r, g, b));
}

// reset parameters of bomb
void Coin::resetBomb() {
  double coin_speed = COIN_SPEED + 10;
  coin_start_y = PLAY_Y_HEIGHT;
  int rand_angle;
  int raise;
  raise = rand() % 10;
  int mult = pow(-1, raise);
  if(mult < 0){
    rand_angle = mult * rand()%20;
  }
  else {
    rand_angle = mult * rand()%60;
  }
  double coin_angle_deg = COIN_ANGLE_DEG + rand_angle;
  coin_ax = 0;
  coin_ay = COIN_G;
  bool paused = true, rtheta = true;
  reset_all(coin_start_x - raise*10, coin_start_y, coin_speed, mult*coin_angle_deg, coin_ax, coin_ay, paused, rtheta);
}

// reset Y coordinate of a coin
void Coin::resetCoinY(int y){
  coin_start_y = y;
  double coin_speed = COIN_SPEED;
  double coin_angle_deg = COIN_ANGLE_DEG;
  coin_ax = 0;
  coin_ay = COIN_G;
  bool paused = true, rtheta = true;
  reset_all(coin_start_x, coin_start_y, coin_speed, coin_angle_deg, coin_ax, coin_ay, paused, rtheta);
}

// reset parameters for speed up coin
void Coin::resetSpeedUpCoin() {
  coin_start_y = PLAY_Y_HEIGHT;
  int add_speed = rand() % 50 ; // randomised speed
  int raise;
  raise = rand() % 10;
  int mult = pow(-1, raise);
  double coin_speed = COIN_SPEED + 20 + add_speed; // randomised speed increase
  double coin_angle_deg = COIN_ANGLE_DEG;
  coin_ax = rand() % 10; // randomised acceleration in x direction
  coin_ay = COIN_G + 20; // randomised acceleration in y direction
  bool paused = true, rtheta = true;
  reset_all(coin_start_x - raise*20, coin_start_y, coin_speed, coin_angle_deg, mult*coin_ax, coin_ay, paused, rtheta);
}

// reset parameters for speed down coin
void Coin::resetSpeedDownCoin() {
  coin_start_y = PLAY_Y_HEIGHT;
  int add_speed = rand() % 20 ; // randomised speed
  int raise;
  raise = rand() % 10;
  int mult = pow(-1, raise);
  double coin_speed = COIN_SPEED + 20 - add_speed; // randomised speed decrease
  double coin_angle_deg = COIN_ANGLE_DEG;
  coin_ax = rand() % 10; // randomised acceleration in x direction
  coin_ay = COIN_G;
  bool paused = true, rtheta = true;
  reset_all(coin_start_x - raise*20, coin_start_y, coin_speed, coin_angle_deg, mult*coin_ax, coin_ay, paused, rtheta);
}
