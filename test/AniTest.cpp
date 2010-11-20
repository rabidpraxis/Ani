#include "Ani.h"
#include "Ease.h"
#include "Timing.h"
#include <map>

using namespace std;
using namespace rp;

class tClass
{
public:
  tClass () {}
  // virtual ~tClass() { cout << "killing tClass" << endl; }
  void setVar(float var) {
    cout << "var: " << var << endl;
  }
  
  void animCallbackFinish() {
    cout << "CallbackFinish: Animation Finalized Callback" << endl;
  }
  void animCallbackStart() {
    cout << "CallbackStart: Animation Started Callback" << endl;
  }
  void animCallbackStep() {
    cout << "CallbackStep: Animation Stepping Callback" << endl;
  }
  void animCallbackStarting() {
    cout << "CallbackStart: um, yeah, I'm starting now" << endl;
  }
};


int main (int argc, char const *argv[])
{
  Ani ani;
    
  // ------ Massive go() ------------------------------------------------------
  cout << "Massive go()\n" << endl;
  float var = 2;
  
  ani.mate(&var)->go(.8, 6, Ease::OutExpo, new Timing::Linear());
  
  for (double i = 1; i <= 3; i += .1) {
    ani.update(i);
    cout << "time: " << i << ", Var: " << var << endl;
  }
  
  // ------ Built up Animation ------------------------------------------------
  cout << "\n\nBuild up and go()\n" << endl;
  float var2 = 2;
  
  ani.mate(&var2)->anim(0.43f, 6)
                 ->setEasingMethod(Ease::NoneLinear)
                 ->setTimeMethod(new Timing::PingPong(1))
                 ->go();
  
  for (double i = 1; i <= 5; i += .1) {
    ani.update(i);
    cout << "time: " << i << ", Var: " << var2 << endl;
  }
  
  // ------ Built up Animation ------------------------------------------------
  cout << "\n\nBuild up and delay go()\n" << endl;
  float var3 = 2;
  tClass test;

  ani.mate(&var3)->anim(0.6f, 6)
                 ->setDelay(0.4f)
                 ->setCallbackStart(&test, &tClass::animCallbackStarting)
                 ->setEasingMethod(Ease::NoneLinear)
                 ->setTimeMethod(new Timing::Repeat())
                 ->go();
  
  for (double i = 1; i <= 3; i += .1) {
    ani.update(i);
    cout << "time: " << i << ", Var: " << var3 << endl;
  }
  
  // ------ Function execution go ---------------------------------------------
  cout << "\n\nFunction Exectution & Finish Callback go()" << endl;
  
  ani.mate(&test, &tClass::setVar)
          ->anim(1.0f, 10, 20)
          ->setCallbackFinish(&test, &tClass::animCallbackFinish)
          ->setEasingMethod(Ease::InBack)
          ->go();
  
  for (double i = 1; i <= 3; i += .1) {
    ani.update(i);
  }
  
  // ------ Function execution go ---------------------------------------------
  cout << "\n\nFunction Exectution & Start Callback go()" << endl;
  
  tClass test2;
  ani.mate(&test2, &tClass::setVar)
          ->anim(1.0f, 10, 20)
          ->setCallbackStart(&test, &tClass::animCallbackStart)
          ->setCallbackStep(&test, &tClass::animCallbackStep)
          ->setCallbackFinish(&test, &tClass::animCallbackFinish)
          ->go();
  
  for (double i = 1; i <= 3; i += .1) {
    ani.update(i);
  }
  
  
  return 0;
}