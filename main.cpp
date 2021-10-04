#include <simplecpp>
#include <string>
#include <cstdlib>
#include <fstream>
#include <ctime>
#include "MovingObject.h"
#include "lasso.h"
#include "coin.h"
using namespace simplecpp;


void controls();

main_program {

  controls(); // to display controls on terminal

  cout<<"\n\n"; // spacing for neatness
  cout<<"Enter your name: ";
  string name;
  cin>>name; // input name of player

  initCanvas("Lasso", WINDOW_X, WINDOW_Y);
  int stepCount = 0;
  float stepTime = STEP_TIME;
  float runTime = -1; // sec; -ve means infinite
  float currTime = 0;

  // Draw lasso at start position
  double release_speed = INIT_RELEASE_SPEED; // m/s
  double release_angle_deg = INIT_RELEASE_ANGLE_DEG; // degrees
  double lasso_ax = 0;
  double lasso_ay = LASSO_G;
  bool paused = true;
  bool rtheta = true;
  Lasso lasso(release_speed, release_angle_deg, lasso_ax, lasso_ay, paused, rtheta);

  Line b1(0, PLAY_Y_HEIGHT, WINDOW_X, PLAY_Y_HEIGHT);
  b1.setColor(COLOR("blue"));
  Line b2(PLAY_X_START, 0, PLAY_X_START, WINDOW_Y);
  b2.setColor(COLOR("blue"));

  string msg("Cmd: _");
  Text charPressed(PLAY_X_START+50, PLAY_Y_HEIGHT+20, msg);
  char coinScoreStr[256];
  sprintf(coinScoreStr, "Coins: %d", lasso.getNumCoins());
  Text coinScore(PLAY_X_START+50, PLAY_Y_HEIGHT+50, coinScoreStr);

  //display player's remaining lives
  char lives_rem[256];
  sprintf(lives_rem, "Lives: %d", lasso.life());
  Text life(PLAY_X_START+50, PLAY_Y_HEIGHT+80, lives_rem) ;

  //display player's level 2 special lives
  char lives_rem2[256];
  sprintf(lives_rem2, "Level 2 Red Lives: %d", lasso.life2());
  Text life2(PLAY_X_START+100, PLAY_Y_HEIGHT-430, lives_rem2) ;
  life2.setColor(COLOR("white"));

  // Display Timer for level 0
  int start_time = clock();
  int end_time = 0;
  int max_time = 1600000;
  char time[256];
  sprintf(time, "Time: %d", end_time/40000);
  Text timer(PLAY_X_START+400, PLAY_Y_HEIGHT-430, time) ;
  timer.setColor(COLOR("blue"));
  Text time_limit(PLAY_X_START+400, PLAY_Y_HEIGHT-460, "Time Limit: 40"); // display time limit text

  paused = true; rtheta = true;
  double coin_speed = COIN_SPEED;
  double coin_angle_deg = COIN_ANGLE_DEG;
  double coin_ax = 0;
  double coin_ay = COIN_G;

  // All types of coins
  Coin coin(coin_speed, coin_angle_deg, coin_ax, coin_ay, paused, rtheta, 255, 215, 0); // normal coin
  Coin bomb(coin_speed + 10, coin_angle_deg, coin_ax, coin_ay, paused, rtheta, 0, 0, 0); // bomb: takes away lives if collected
  bomb.resetCoinY(0); // reset Y coordinate of coin to 0
  bomb.resetCoinColor(255,255,255); // set bomb colour white to hide it until required
  Coin speed_up_coin(coin_speed , coin_angle_deg, coin_ax, coin_ay, paused, rtheta, 0, 255, 255); // speed up coin: increases lasso release speed
  speed_up_coin.resetCoinY(0); // reset Y coordinate of coin to 0
  speed_up_coin.resetCoinColor(255,255,255); // set bomb colour white to hide it until required
  Coin speed_down_coin(coin_speed , coin_angle_deg, coin_ax, coin_ay, paused, rtheta, 100, 0, 100); // speed down coin: decreases lasso release speed
  speed_down_coin.resetCoinY(0); // rest Y coordinate of coin to 0
  speed_down_coin.resetCoinColor(255,255,255); // set bomb colour white to hide it until required

  // After every COIN_GAP sec, make the coin jump
  double last_coin_jump_end = 0;

  int l2_lives = 0; // lives for level 2
  lasso.setPlayerName(name); // set player's name

  // visual life indicator
  Circle redlife(PLAY_X_START+50, PLAY_Y_HEIGHT-400, 10) ; // create a circle which denotes a single life
  Circle redlife1(PLAY_X_START+75, PLAY_Y_HEIGHT-400, 10) ; // create a circle which denotes a single life
  Circle redlife2(PLAY_X_START+100, PLAY_Y_HEIGHT-400, 10) ; // create a circle which denotes a single life
  redlife.setColor(COLOR("white")); // set level 2 lives colour to white to hide them until required
  redlife1.setColor(COLOR("white")); // set level 2 lives colour to white to hide them until required
  redlife2.setColor(COLOR("white")); // set level 2 lives colour to white to hide them until required


  // When t is pressed, throw lasso
  // If lasso within range, make coin stick
  // When y is pressed, yank lasso
  // When l is pressed, loop lasso
  // When q is pressed, quit

  for(;;) {

    // Level 0
    if(lasso.getNumCoins() == 0)
    {
      end_time = clock();
      sprintf(time, "Time: %d", end_time/40000); //edit
      timer.setMessage(time);
      if(end_time - start_time > max_time)
      {
        cout<<"Time Out!! Couldn't clear level in time"; // display message when time limit limit exceeded
        lasso.score(name);
        exit(0); // exit program if time limit exceeded
      }
    }

    // Level 1
    if(lasso.getNumCoins() == 1)
    {
      timer.setColor(COLOR("white")); // remove timer from screen
      time_limit.setColor(COLOR("white"));

      bomb.resetCoinColor(0,0,0); // set colour of bomb to black

      bomb.nextStep(stepTime);
      if(bomb.isPaused()) {
        if((currTime-last_coin_jump_end) >= COIN_GAP) {
          bomb.resetBomb();
          bomb.unpause();
        }
      }


      if(bomb.getYPos() > PLAY_Y_HEIGHT) {
        bomb.resetCoin2();
        last_coin_jump_end = currTime;
      }
    }

    // Level 2
    if(lasso.getNumCoins() == 2)
    {
      bomb.resetCoinColor(255,255,255); // remove bomb from screen
      bomb.resetCoinY(0);

      life2.setColor(COLOR("black"));
      sprintf(lives_rem2, "Level 2 Lives: %d", lasso.life2()); // display remaining special level 2 lives
      life2.setMessage(lives_rem2);

      l2_lives = lasso.life2(); // sets lives for level 2

      // display red dots on screen corresponding to lives remaining
      if(l2_lives==3)
      {
        redlife.setColor(COLOR("red"));
        redlife.setFill(true);

        redlife1.setColor(COLOR("red"));
        redlife1.setFill(true);

        redlife2.setColor(COLOR("red"));
        redlife2.setFill(true);
      }
      else if(l2_lives==2)
      {
        redlife.setColor(COLOR("red"));
        redlife.setFill(true);

        redlife1.setColor(COLOR("red"));
        redlife1.setFill(true);

        redlife2.setColor(COLOR("red"));
        redlife2.setFill(false);

      }
      else if(l2_lives==1)
      {
        redlife.setColor(COLOR("red"));
        redlife.setFill(true);

        redlife1.setColor(COLOR("red"));
        redlife1.setFill(false);

        redlife2.setColor(COLOR("red"));
        redlife2.setFill(false);
      }
    }

    // Level 3
    if(lasso.getNumCoins() == 3)
    {
      // removes red dots from screen
      redlife.setColor(COLOR("white"));
      redlife1.setColor(COLOR("white"));
      redlife2.setColor(COLOR("white"));
      life2.setColor(COLOR("white"));

      speed_up_coin.resetCoinColor(0,255,255); // set speedup coin colour to cyan

      speed_up_coin.nextStep(stepTime );
      if(speed_up_coin.isPaused()) {
        if((currTime-last_coin_jump_end) >= COIN_GAP) {
          speed_up_coin.resetSpeedUpCoin();
          speed_up_coin.unpause();
        }
      }

      if(speed_up_coin.getYPos() > PLAY_Y_HEIGHT) {
        speed_up_coin.resetSpeedUpCoin();
        last_coin_jump_end = currTime;
      }

      speed_down_coin.resetCoinColor(100,0,100); // set speed down coin colour to magenta

      speed_down_coin.nextStep(stepTime);
      if(speed_down_coin.isPaused()) {
        if((currTime-last_coin_jump_end) >= COIN_GAP) {
          speed_down_coin.resetSpeedUpCoin();
          speed_down_coin.unpause();
        }
      }

      if(speed_down_coin.getYPos() > PLAY_Y_HEIGHT) {
        speed_down_coin.resetSpeedDownCoin();
        last_coin_jump_end = currTime;
      }
    }

    //Level 4
    if(lasso.getNumCoins() == 4)
    {
      // remove all new coins from screen
      speed_up_coin.resetCoinColor(255,255,255);
      speed_down_coin.resetCoinColor(255,255,255);
      speed_up_coin.resetCoinY(0);
      speed_down_coin.resetCoinY(0);
    }


    if((runTime > 0) && (currTime > runTime)) { break; }

    XEvent e;
    bool pendingEv = checkEvent(e);
    if(pendingEv) {
      char c = charFromEvent(e);
      msg[msg.length()-1] = c;
      charPressed.setMessage(msg);
      switch(c) {
        case 't':
        lasso.unpause(); // throw lasso
        break;
        case 'y':
        lasso.yank(); // yank lasso (bring back to start point)
        break;
        case 'l':
        lasso.loopit(); // lasso loop is displayed
        lasso.check_for_coin(&coin); // checks for gold coin to be in radius of loop
        lasso.check_for_bomb(&bomb); // checks for bomb (bomb) to be in radius of loop
        lasso.check_for_speed_up_coin(&speed_up_coin); // checks for speed up coin to be in radius of loop
        lasso.check_for_speed_down_coin(&speed_down_coin); // checks for speed down coin to be in radius of loop

        wait(STEP_TIME*5);
        break;
        case '[':
        if(lasso.isPaused()) { lasso.addAngle(-RELEASE_ANGLE_STEP_DEG);	} // decrease lasso release angle
        break;
        case ']':
        if(lasso.isPaused()) { lasso.addAngle(+RELEASE_ANGLE_STEP_DEG); } // increase lasso release angle
        break;
        case '-':
        if(lasso.getNumCoins() != 3) // lock manual speed change at level 3
        if(lasso.isPaused()) { lasso.addSpeed(-RELEASE_SPEED_STEP); } // manually decrease lasso release speed
        break;
        case '=':
        if(lasso.getNumCoins() != 3) // lock manual speed change at level 3
        if(lasso.isPaused()) { lasso.addSpeed(+RELEASE_SPEED_STEP); } // manually increase lasso release speed
        break;
        case 'q':
        lasso.score(name); // set score in text file
        exit(0); // exit program
        default:
        break;
      }
    }

    lasso.nextStep(stepTime);

    coin.nextStep(stepTime);
    if(coin.isPaused()) {
      if((currTime-last_coin_jump_end) >= COIN_GAP) {
        coin.unpause();
      }
    }

    if(coin.getYPos() > PLAY_Y_HEIGHT) {
      if(lasso.getNumCoins() == 3){
        coin.resetCoin2();
      }
      else{
        coin.resetCoin();
      }
      last_coin_jump_end = currTime;
    }

    sprintf(coinScoreStr, "Coins: %d", lasso.getNumCoins());
    coinScore.setMessage(coinScoreStr);

    sprintf(lives_rem, "Lives: %d", lasso.life());
    life.setMessage(lives_rem);

    stepCount++;
    currTime += stepTime;
    wait(stepTime);
  } // End for(;;)


  wait(3);

} // End main_program
