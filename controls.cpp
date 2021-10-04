#include <simplecpp>
using namespace simplecpp;

void controls(){
  cout<<""<<endl;
  cout<<"\033[1;31mWelcome to The Lasso game!!\033[0m"<<endl;
  cout<<""<<endl;
  cout<<"\033[1;33mBefore we start the game, here are the instructions:\033[0m"<<endl;
  cout<<""<<endl;
  cout<<"\033[1;32mT\033[0m : Throw lasso"<<endl;
  cout<<"\033[1;32mL\033[0m : Loop lasso\n"<<endl;
  cout<<"If lasso is  within the range of the loop, make the coin stick\n"<<endl;
  cout<<"\033[1;32mY\033[0m : Yank lasso\n"<<endl;
  cout<<"When the Lasso is paused then: "<<endl;
  cout<<"\033[1;32m[\033[0m : decrease angle of release"<<endl;
  cout<<"\033[1;32m]\033[0m : increase angle of release"<<endl;
  cout<<"\033[1;32m-\033[0m : decrease speed"<<endl;
  cout<<"\033[1;32m=\033[0m : increase speed"<<endl;
  cout<<"\033[1;32mQ\033[0m : Quit game"<<endl;
  return;
}

// to run use the following: s++ -o lasso lasso.cpp MovingObject.cpp coin.cpp main.cpp controls.cpp
