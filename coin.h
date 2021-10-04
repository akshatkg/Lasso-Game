#ifndef __COIN_H__
#define __COIN_H__

#include "MovingObject.h"
#include "lasso.h"

class Coin : public MovingObject {
  double coin_start_x; // x coordinate of coin
  double coin_start_y; // y coordinate of coin
  double release_speed; // release speed of coin
  double release_angle_deg; // angle of release of coin
  double coin_ax; // acceleration in x direction
  double coin_ay; // acceleration in y direction

  // Moving parts
  Circle coin_circle;

public:
  Coin(double speed, double angle_deg, double argax, double argay, bool argpaused, bool rtheta, unsigned int r, unsigned int g, unsigned int b) : MovingObject(speed, angle_deg, argax, argay, argpaused, rtheta) {
    release_speed = speed;
    release_angle_deg = angle_deg;
    coin_ax = argax;
    coin_ay = argay;
    initCoin(r, g, b);
  }

  void initCoin(unsigned int r, unsigned int g, unsigned int b);
  void resetCoin();
  void resetCoin2();
  void resetBomb();
  void resetCoinColor(unsigned int r, unsigned int g, unsigned int b);
  void resetCoinY(int y);
  void resetSpeedUpCoin();
  void resetSpeedDownCoin();
}; // End class Coin


#endif
