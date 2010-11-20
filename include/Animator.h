//  ------------------------------------------------------------------------ // 
//  ===== Animator.h ======================================================= // 
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

#include <deque>

#include "Timing.h"
#include "Ease.h"
#include "Animation.h"
#include "Ani.h"

namespace rp {

using std::deque;


/*=============================================================================
          Animator Base class
===============================================================================
  
  This base class is purely for storage within rp::Ani

*/
class Animator
{
public:
  Animator () {}
  virtual void update(const double time) {}
  virtual void destroy() {}
};

/*=============================================================================
          Animator Implementation
===============================================================================

  Here is the meat, not sure if Impl is reserved for classes that don't
  actually implement anything. If it does you can email me:
  me@kdoubleyou.com
  
*/

template <typename T>
class AnimatorImpl : public Animator
{
public:
  AnimatorImpl () {}
  virtual ~AnimatorImpl () { destroy(); }
  
  // ------ Animation on queue push back --------------------------------------
  AnimatorImpl<T>* go() {
    animations_.push_back(initialAnim_);
    return this;
  }
  virtual AnimatorImpl<T>* go(double d, T f, T (*e)(double t, T b, T c, double d),
                              TimeBase* t) { return this; }
  
  // ------ General Setters ---------------------------------------------------
  AnimatorImpl<T>* setFinalValue(T finalVal) { 
    initialAnim_->setFinalValue(finalVal);
    return this;
  }
  AnimatorImpl<T>* setEasingMethod(T (*easingMethod)(double t, T b, T c, double d)) {
    initialAnim_->setEasingMethod(easingMethod);
    return this;
  }
  AnimatorImpl<T>* setTimeMethod(TimeBase* timer) { 
    initialAnim_->setTimeMethod(timer); 
    return this; 
  }
  AnimatorImpl<T>* setDuration(double duration) { 
    initialAnim_->setDuration(duration);
    return this; 
  }
  AnimatorImpl<T>* setDelay(double delay) {
    initialAnim_->setDelay(delay);
    return this;
  }
  
  // ------ Callbacks ---------------------------------------------------------
  template <typename clT>
  AnimatorImpl<T>* setCallbackFinish(clT* obj, void(clT::*fnct)()) {
    initialAnim_->setCallbackFinish(obj, fnct);
    return this;
  }
  // Allow passing of one variable
  template <typename clT, typename T1>
  AnimatorImpl<T>* setCallbackFinish(clT* obj, void(clT::*fnct)(T1), T1 arg) {
    initialAnim_->setCallbackFinish(obj, fnct, arg);
    return this;
  }
  template <typename clT>
  AnimatorImpl<T>* setCallbackStep(clT* obj, void(clT::*fnct)()) {
    initialAnim_->setCallbackStep(obj, fnct);
    return this;
  }
  template <typename clT>
  AnimatorImpl<T>* setCallbackStart(clT* obj, void(clT::*fnct)()) {
    initialAnim_->setCallbackStart(obj, fnct);
    return this;
  }
  
  // ------ Queries -----------------------------------------------------------
  bool isAnimating() {
    return !animations_.empty();
  }
  AnimationBase<T>* getCurrentAnimation() { 
    if (!animations_.empty()) {
      return animations_.front();
    }
  }
  
  // ------ Buttons -----------------------------------------------------------
  AnimatorImpl<T>* pause() {
    paused_ = true;
    return this;
  }
  AnimatorImpl<T>* play() {
    paused_ = false;
    return this;
  }
  AnimatorImpl<T>* stop() {
    if (!animations_.empty()) 
      animations_.clear();
    return this;
  }
  AnimatorImpl<T>* reverse() {}
  
  // ------ Updater -----------------------------------------------------------
  void update(const double ttime) {
    if (!animations_.empty()) {
      animations_.front()->update(ttime);
      
      // ------ Pop animation off stack if completed --------------------------
      if (animations_.front()->isComplete()) {
        delete animations_.front(); // delete pointer
        animations_.pop_front();    // remove from stack
      }
    }
  }
  
 protected:
  void init() {
    paused_ = false;
  }
  
  void destroy() {
    for ( typename std::deque<AnimationBase<T>* >::iterator it = animations_.begin(); 
      it != animations_.end(); ++it )
    {
      delete *it;
    }
  }
  
 protected:
  bool paused_;
  
  std::deque<AnimationBase<T>* > animations_;
  AnimationBase<T>* initialAnim_;  
};


/*=============================================================================
          Animator: Animation variable container
=============================================================================*/
template <typename T>
class varAnimator : public AnimatorImpl<T>
{
 public:
  varAnimator () {}
  varAnimator (T* var) : var_(var) { this->init(); }
  
  
  // ------ Setup new Animation -----------------------------------------------
  varAnimator<T>* anim(double duration, T finalVal) {
    this->initialAnim_ = new Animation<T>(var_, duration, finalVal, 
                                          Ease::NoneLinear, new Timing::Linear());
    return this;
  }
  
  // ------ All in one method -------------------------------------------------
  varAnimator<T>* go(double duration, 
                  T finalVal, 
                  T (*easingMethod)(double t, T b, T c, double d) = Ease::NoneLinear,
                  TimeBase* timeMethod = new Timing::Linear()) 
  {
    // TimeBase* timeMethod = &timeMethod;
    this->animations_.push_back(new Animation<T>(var_, duration, finalVal,
                                                 easingMethod, timeMethod));
    return this;
  }
  
 protected:
  T* var_;
};


/*=============================================================================
          fnctAnimator: Animation Function container
=============================================================================*/
template <typename clT, typename T, typename fnrt>
class fnctAnimator : public AnimatorImpl<T>
{
 public:
  fnctAnimator () {}
  fnctAnimator (clT* obj, fnrt(clT::*fnct)(T)) : 
    fnct_(fnct),
    obj_(obj) { this->init(); }

  // ------ Setup new Animation -----------------------------------------------
  fnctAnimator<clT, T, fnrt>* anim(double duration, T beginVal, T finalVal) {
    this->initialAnim_ = 
      new fnctAnimation<clT, T, fnrt>(obj_, fnct_, duration, beginVal, finalVal, 
                                      Ease::NoneLinear, new Timing::Linear());
    return this;
  }
  
 protected:  
  fnrt(clT::*fnct_)(T);
  clT*                                        obj_;
  std::deque<fnctAnimation<clT, T, fnrt>* >   animations_;
};




} // namespace rp

