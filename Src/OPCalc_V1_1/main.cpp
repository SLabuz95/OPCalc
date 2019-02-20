#include"appclass.hpp"

int main(int argc, char *argv[]){
  App *dp = new App(argc, argv);
  if(dp->isCloseApp())
      return 0;
  return dp->exec();
}
