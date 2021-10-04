#include <simplecpp>
#include <string>
#include <fstream>
#include "MovingObject.h"
#include "lasso.h"
#include "coin.h"

using namespace simplecpp;

// draw lasso band
void Lasso::draw_lasso_band() {
  double len = (release_speed/MAX_RELEASE_SPEED)*LASSO_BAND_LENGTH;
  double arad = release_angle_deg*PI/180.0;
  double xlen = len*cos(arad);
  double ylen = len*sin(arad);
  lasso_band.reset(lasso_start_x, lasso_start_y, (lasso_start_x-xlen), (lasso_start_y+ylen));
  lasso_band.setThickness(LASSO_THICKNESS);
} // End Lasso::draw_lasso_band()

// initialise lasso parameters
void Lasso::initLasso() {
  lasso_start_x = (PLAY_X_START+LASSO_X_OFFSET);
  lasso_start_y = (PLAY_Y_HEIGHT-LASSO_Y_HEIGHT);
  lasso_circle.reset(lasso_start_x, lasso_start_y, LASSO_SIZE);
  lasso_circle.setColor(COLOR("red"));
  lasso_circle.setFill(true);
  lasso_loop.reset(lasso_start_x, lasso_start_y, LASSO_SIZE/2);
  lasso_loop.setColor(COLOR("brown"));
  lasso_loop.setFill(true);
  addPart(&lasso_circle);
  addPart(&lasso_loop);
  lasso_looped = false;
  the_coin = NULL;
  the_bomb = NULL;
  the_speed_up_coin = NULL;
  the_speed_down_coin = NULL;
  num_coins = 0; // set initial amount of coins to 0
  lives = 7; // set initial amount of lives
  l2_lives = 3; // set initial amount of level 2 lives
  lasso_line.reset(lasso_start_x, lasso_start_y, lasso_start_x, lasso_start_y);
  lasso_line.setColor(COLOR("brown"));

  lasso_band.setColor(COLOR("BlueViolet"));
  draw_lasso_band();

} // End Lasso::initLasso()

// Yank lasso
void Lasso::yank() {
  bool paused = true, rtheta = true;
  reset_all(lasso_start_x, lasso_start_y, release_speed, release_angle_deg, lasso_ax, lasso_ay, paused, rtheta);
  lasso_loop.reset(lasso_start_x, lasso_start_y, LASSO_SIZE/2);
  lasso_loop.setFill(true);
  lasso_looped = false;

  if(the_coin != NULL){
    if(num_coins == 2){
      the_coin->resetCoin2(); // reset coin such that it moves in parabolic path on level 2
    }
    else{
      the_coin->resetCoin();
    }
  }
}// End Lasso::yank()


void Lasso::loopit() {
  if(lasso_looped) { return; } // Already looped
  lasso_loop.reset(getXPos(), getYPos(), LASSO_RADIUS);
  lasso_loop.setFill(false);
  lasso_looped = true;
} // End Lasso::loopit()

// store score along with name in a text file
void Lasso::score(string name) {
  std::ofstream outfile;
  outfile.open("score.txt", std::ios_base::app);  // append text instead of overwriting
  string user_score = to_string(getNumCoins());
  outfile << name  << ":" << user_score << "\n";
} //End Lasso::score()

void Lasso::addAngle(double angle_deg) {
  release_angle_deg += angle_deg;
  if(release_angle_deg < MIN_RELEASE_ANGLE_DEG) { release_angle_deg = MIN_RELEASE_ANGLE_DEG; }
  if(release_angle_deg > MAX_RELEASE_ANGLE_DEG) { release_angle_deg = MAX_RELEASE_ANGLE_DEG; }
  bool paused = true, rtheta = true;
  reset_all(lasso_start_x, lasso_start_y, release_speed, release_angle_deg, lasso_ax, lasso_ay, paused, rtheta);
} // End Lasso::addAngle()

void Lasso::addSpeed(double speed) {
  release_speed += speed;
  if(release_speed < MIN_RELEASE_SPEED) { release_speed = MIN_RELEASE_SPEED; }
  if(release_speed > MAX_RELEASE_SPEED) { release_speed = MAX_RELEASE_SPEED; }
  bool paused = true, rtheta = true;
  reset_all(lasso_start_x, lasso_start_y, release_speed, release_angle_deg, lasso_ax, lasso_ay, paused, rtheta);
} // End Lasso::addSpeed()

void Lasso::nextStep(double stepTime) {
  draw_lasso_band();
  MovingObject::nextStep(stepTime);
  if(getYPos() > PLAY_Y_HEIGHT) { yank(); }
  lasso_line.reset(lasso_start_x, lasso_start_y, getXPos(), getYPos());
} // End Lasso::nextStep()

// check if gold coin is in lasso loop's radius
void Lasso::check_for_coin(Coin *coinPtr) {
  double lasso_x = getXPos();
  double lasso_y = getYPos();
  double coin_x = coinPtr->getXPos();
  double coin_y = coinPtr->getYPos();
  double xdiff = (lasso_x - coin_x);
  double ydiff = (lasso_y - coin_y);
  double distance = sqrt((xdiff*xdiff)+(ydiff*ydiff));

  if(distance <= LASSO_RADIUS && distance != 0){
    num_coins++; // increase number of coins
  }

  if(distance <= LASSO_RADIUS) {
    the_coin = coinPtr;
    the_coin->getAttachedTo(this);
  }
  else if(lives==1){ //edit
    cout<<"Out of Lives :("<<endl;
    score(PlayerName());
    exit(0);
  }
  else if(getNumCoins() == 2){
    l2_lives--; // decrease level 2 lives
    if(l2_lives == 0){
      cout<<"Couldn't clear level 2"<<endl;
      score(PlayerName());
      exit(0); // exit game if out of level 2 lives
    }
  }
  else{
    lives--; // decrease lives if missed a coin
  }

} // End Lasso::check_for_coin()

// check if bomb is in lasso loop's radius
void Lasso::check_for_bomb(Coin *bombPtr)
{
  double lasso_x = getXPos();
  double lasso_y = getYPos();
  double coin_x = bombPtr->getXPos();
  double coin_y = bombPtr->getYPos();
  double xdiff = (lasso_x - coin_x);
  double ydiff = (lasso_y - coin_y);
  double distance = sqrt((xdiff*xdiff)+(ydiff*ydiff));

  if(distance <= LASSO_RADIUS && distance != 0){
    if(lives==1){
      cout<<"Out of Lives :("<<endl;
      score(PlayerName());
      exit(0); // end game if out of lives
    }
    lives--; // decrease lives if bomb was collected
    cout<<"<< Bomb Collected >>"<<endl; // display message when bomb collected
    the_bomb = bombPtr;
    the_bomb->resetBomb();
  }
}// End Lasso::check_for_bomb()

// check if speed up coin is in lasso loop's radius
void Lasso::check_for_speed_up_coin(Coin *speed_up_coinPtr)
{
  double lasso_x = getXPos();
  double lasso_y = getYPos();
  double coin_x = speed_up_coinPtr->getXPos();
  double coin_y = speed_up_coinPtr->getYPos();
  double xdiff = (lasso_x - coin_x);
  double ydiff = (lasso_y - coin_y);
  double distance = sqrt((xdiff*xdiff)+(ydiff*ydiff));

  if(distance <= LASSO_RADIUS && distance != 0){
    addSpeed(80); //increase lasso speed
    cout<<"Speed Increased ++"<<endl; //display message when speed up coin collected
    the_speed_up_coin = speed_up_coinPtr;
    the_speed_up_coin->resetSpeedUpCoin();
  }
}// End Lasso::check_for_speed_up_coin()

// check if speed down coin is in lasso loop's radius
void Lasso::check_for_speed_down_coin(Coin *speed_down_coinPtr)
{
  double lasso_x = getXPos();
  double lasso_y = getYPos();
  double coin_x = speed_down_coinPtr->getXPos();
  double coin_y = speed_down_coinPtr->getYPos();
  double xdiff = (lasso_x - coin_x);
  double ydiff = (lasso_y - coin_y);
  double distance = sqrt((xdiff*xdiff)+(ydiff*ydiff));

  if(distance <= LASSO_RADIUS && distance != 0){
    addSpeed(-20); // decrease lasso speed
    cout<<"Speed Decreased --"<<endl; //display message when speed down coin collected
    the_speed_down_coin = speed_down_coinPtr;
    the_speed_down_coin->resetSpeedDownCoin();
  }
}// End Lasso::check_for_speed_down_coin()
