//  ------------------------------------------------------------------------ // 
//  ===== Animation.h ====================================================== // 
//  ------------------------------------------------------------------------ // 
//   Created:        Kevin Webster                                           // 
//   Date:           10.10.22                                                // 
//   Copyright (c)   2010 All rights reserved.                               // 
//  ------------------------------------------------------------------------ // 
//  Redistribution and use in source and binary forms, with or without       // 
//  modification, are permitted provided that the following conditions       // 
//  are met:                                                                 // 
//                                                                           // 
//     * Redistributions of source code must retain the above copyright      // 
//       notice, this list of conditions and the following disclaimer.       // 
//     * Redistributions in binary form must reproduce the above copyright   // 
//       notice, this list of conditions and the following disclaimer in     // 
//       the documentation and/or other materials provided with the          // 
//       distribution.                                                       // 
//     * Stealing is also kinda lame.                                        // 
//                                                                           // 
//  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS      // 
//  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT        // 
//  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR    // 
//  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT     // 
//  HOLDER OR CONTRIBUTORS BE LIABLEFOR ANY DIRECT, INDIRECT, INCIDENTAL,    // 
//  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED // 
//  TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR   // 
//  PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF   // 
//  LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING     // 
//  NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS       // 
//  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.             // 
//  ------------------------------------------------------------------------ // 

#pragma once

#include "Timing.h"

namespace rp {

// ============================================================================

/*
  Simple callback class for storing and executing callbacks
*/

class callbackBase {
 public:
  virtual void exec() = 0;
};

template <typename clT>
class callback : public callbackBase
{
 public:
  callback() {}
  callback(clT* obj, void(clT::*fnct)()) : obj_(obj), fnct_(fnct) {}
  
  void exec() { (obj_->*fnct_)(); }
  
private:
  clT* obj_;
  void(clT::*fnct_)();
};

template <typename clT, typename T1>
class callbackA1 : public callbackBase
{
 public:
  callbackA1() {}
  callbackA1(clT* obj, void(clT::*fnct)(T1), T1 arg) 
    : obj_(obj), arg_(arg), fnct_(fnct) {}
  
  void exec() { (obj_->*fnct_)(arg_); }
  
private:
  clT* obj_;
  T1 arg_;
  void(clT::*fnct_)(T1);
};


/*=============================================================================
          AnimationBase: base class for animations (duh.)
=============================================================================*/

template <typename T>
class AnimationBase 
{
 public:
  AnimationBase () {}
  AnimationBase (double duration, 
                 T finalVal, 
                 T (*easing)(double t, T b, T c, double d),
                 TimeBase* timer)
            : started_(false), finished_(false), delaying_(false), 
              duration_(duration), delay_(0), final_val_(finalVal), 
              easingMethod_(easing), timeMethod_(timer), 
              doCallbackFinish_(false),
              doCallbackStep_(false),
              doCallbackStart_(false) {}
  
  virtual ~AnimationBase() { destroy(); }
  
  AnimationBase<T>* setDelay(double delay) {
    delay_ = delay;
    return this;
  }
  AnimationBase<T>* setTimeMethod(TimeBase* timer) { 
    timeMethod_ = timer; 
    return this; 
  }
  virtual AnimationBase<T>* setFinalValue(T finalVal) = 0;
  AnimationBase<T>* setEasingMethod(T (*easingMethod)(double t, T b, T c, double d)) {
   easingMethod_ = easingMethod;
   return this;
  }
  AnimationBase<T>* setDuration(double duration) { 
    duration_ = duration; 
    return this; 
  }
  
  // ------ Setup various callback functions ----------------------------------
  template <typename clT>
  AnimationBase<T>* setCallbackFinish(clT* obj, void(clT::*fnct)()) {
    doCallbackFinish_ = true;
    callbackFinish_ = new callback<clT>(obj, fnct);
    return this;
  }
  // One argument to pass along
  template <typename clT, typename T1>
  AnimationBase<T>* setCallbackFinish(clT* obj, void(clT::*fnct)(T1), T1 arg) {
    doCallbackFinish_ = true;
    callbackFinish_ = new callbackA1<clT, T1>(obj, fnct, arg);
    return this;
  }
  template <typename clT>
  AnimationBase<T>* setCallbackStart(clT* obj, void(clT::*fnct)()) {
    doCallbackStart_ = true;
    callbackStart_ = new callback<clT>(obj, fnct);
    return this;
  }
  template <typename clT>
  AnimationBase<T>* setCallbackStep(clT* obj, void(clT::*fnct)()) {
    doCallbackStep_ = true;
    callbackStep_ = new callback<clT>(obj, fnct);
    return this;
  }
  
  virtual void update(const double ttime) = 0;
  
  bool isComplete() { 
   return finished_;
  }

// ====== Protected Methods ===================================================
 protected:
  void callbackFinish() {
   if (finished_) { // only execute if we are finishing the animation
     if (doCallbackFinish_) 
       callbackFinish_->exec();
   }
  };
  void callbackStart() {
    if (doCallbackStart_) 
      callbackStart_->exec();
  }
  void callbackStep() {
    if (doCallbackStep_)
      callbackStep_->exec();
  }
  
  /*
    A little bit complicated, but gets the job done
  */
  bool isDelaying(const double ttime) {
   if (delaying_ || (!started_ && (delay_ > 0))) {
     if (!delaying_) {
       delaying_ = true;
       delayEnd_ = ttime + delay_;
     }
     
     if (ttime >= delayEnd_) {
       delaying_ = false;
       return false;
     }
     
     return true;
   }
   return false;
  }

  // ------ Call easing and time methods --------------------------------------
  T updateVar(const double ttime) {
   return easingMethod_(
             (*timeMethod_)(ttime, start_, duration_, finished_),
             beginning_, 
             change_,
             1);
  }
  
  void destroy() {
    if (doCallbackFinish_) 
      delete callbackFinish_;
    if (doCallbackStart_) 
      delete callbackStart_;
    if (doCallbackStep_) 
      delete callbackStep_;
  }

// ====== Protected properties ================================================
 protected:
  bool    started_;
  bool    finished_;
  bool    delaying_;

  double  duration_;
  double  start_;
  double  delay_;
  double  delayEnd_;

  T       beginning_;
  T       final_val_;
  T       change_;

  // ------ Easing & Time methods ---------------------------------------------
  T (*easingMethod_)(double t, T b, T c, double d);
  TimeBase* timeMethod_;
  
  // ------ Callbacks ---------------------------------------------------------
  bool          doCallbackFinish_;
  callbackBase* callbackFinish_;
  bool          doCallbackStep_;
  callbackBase* callbackStep_;
  bool          doCallbackStart_;
  callbackBase* callbackStart_;

};


/* ============================================================================
          Animation: Animation executor
=============================================================================*/
template <typename T>
class Animation : public AnimationBase<T>
{
 public:
  // ------ Buildable animation constructor -----------------------------------
  Animation (T* var) : var_(var) {}
  
  // ------ Mammoth singular animation constructor ----------------------------
  Animation (T* var,
             double duration, 
             T finalVal, 
             T (*easing)(double t, T b, T c, double d),
             TimeBase* timer)
        : AnimationBase<T>(duration, finalVal, easing, timer), var_(var) {}
  
  Animation<T>* setFinalValue(T finalVal) { 
   this->final_val_ = finalVal; 
   this->change_ = finalVal - *var_;
   return this;
  }
  
  // ------ Custom update for variables ---------------------------------------
  void update(const double ttime) {
    if (AnimationBase<T>::isDelaying(ttime))
      return;
    
    // ------ Set beginning values --------------------------------------------
    if (!this->started_) {
     AnimationBase<T>::callbackStart();
     this->started_ = true;
     this->start_ = ttime;
     this->change_ = this->final_val_ - *var_;
     this->beginning_ = *var_;
    }
        
    *var_ = this->updateVar(ttime);

    AnimationBase<T>::callbackStep();
    AnimationBase<T>::callbackFinish();
  }

 private:  
  T* var_;
};

/*=============================================================================
          fnctAnimation: Animation function executor
=============================================================================*/
template <typename clT, typename T, typename fnrt>
class fnctAnimation : public AnimationBase<T>
{
 public:
  // ------ Buildable animation constructor -----------------------------------
  fnctAnimation () {}
  
  // ------ Mammoth singular animation constructor ----------------------------
  fnctAnimation (clT* obj,
                 fnrt(clT::*fnct)(T),
                 double duration,
                 T initialVal,
                 T finalVal, 
                 T (*easing)(double t, T b, T c, double d),
                 TimeBase* timer)
            : AnimationBase<T>(duration, finalVal, easing, timer), 
              fnct_(fnct),
              obj_(obj) 
  {
    this->beginning_ = initialVal;
    setFinalValue(finalVal);
  }
  
  fnctAnimation<clT, T, fnrt>* setFinalValue(T finalVal) { 
   this->final_val_ = finalVal;
   this->change_ = finalVal - this->beginning_;
   return this;
  }
  
  // ------ Custom update for member functions --------------------------------
  void update(const double ttime) {
     if (AnimationBase<T>::isDelaying(ttime))
       return;
     
     // ------ Set beginning values --------------------------------------------
     if (!this->started_) {
       AnimationBase<T>::callbackStart();
       // beginning_ is predetermined by caller at the Animation instancing
       this->started_ = true;
       this->start_ = ttime;
       // this->change_ = this->final_val_ - this->beginning_;
     }
     (obj_->*fnct_)(this->updateVar(ttime));

     AnimationBase<T>::callbackStep();
     AnimationBase<T>::callbackFinish();
  }

 private:  
  fnrt(clT::*fnct_)(T);
  clT*                                  obj_;
};

} // namespace rp

