//  ------------------------------------------------------------------------ // 
//  ===== Ani.h ============================================================ // 
//  ------------------------------------------------------------------------ // 
//   Created:        Kevin Webster                                           // 
//   Date:           10.10.20                                                // 
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
#include <list>
#include <map>
#include <cmath>
#include <iostream>

#include "Ease.h"
#include "Animator.h"
#include "Animation.h"
#include "Timing.h"


namespace rp {
/*=============================================================================
          Ani: Animator Manager
=============================================================================*/
class Ani
{
 public:
  Ani () {}
  
  typedef std::map<uintptr_t, Animator*> animatorMap;

  // ------ mate(): retreive or create a new variable Animator ----------------
  template <typename T>
  varAnimator<T>* mate(T* var) {
    // ------ Check for inclusion ---------------------------------------------
    uintptr_t ptr = reinterpret_cast<uintptr_t>(var);
    animatorMap::iterator it = animators_.find(ptr);
    
    if (it == animators_.end()) {
      varAnimator<T>* anim = new varAnimator<T>(var);
      animators_[ptr] = anim;
      return anim;
    } else {
      return static_cast<varAnimator<T>* >(it->second);
    }
  }
  
  // ------ mate(): retreive or create a new function Animator ----------------
  template <typename clT, typename T, typename fnrt>
  fnctAnimator<clT, T, fnrt>* mate(clT* obj, fnrt(clT::*fnct)(T)) {
    // Add object to function pointer to get unique address for key
    uintptr_t ptr = reinterpret_cast<uintptr_t>(&fnct) + 
                    reinterpret_cast<uintptr_t>(obj);
                    
    animatorMap::iterator it = animators_.find(ptr);
    
    if (it == animators_.end()) {
      fnctAnimator<clT, T, fnrt>* anim = new fnctAnimator<clT, T, fnrt>(obj, fnct);
      animators_[ptr] = anim;
      return anim;
    } else {
      return static_cast<fnctAnimator<clT, T, fnrt>* >(it->second);
    }
  }
  
  void update(const double ttime) {
    for ( animatorMap::iterator it = animators_.begin(); 
      it != animators_.end(); ++it )
    {
      (*(it->second)).update(ttime);
    }
  }
  
  template <typename T>
  void remove(T* var) {
    removePtr(reinterpret_cast<uintptr_t>(var));
  }
  
  template <typename clT, typename T>
  void remove(clT* obj, void(clT::*fnct)(T)) {
    removePtr(reinterpret_cast<uintptr_t>(&fnct) + 
              reinterpret_cast<uintptr_t>(obj));
  }
  
 private:
  void removePtr(uintptr_t ptr) {
   animatorMap::iterator it = animators_.find(ptr);
   if (it != animators_.end()) {
     animators_.erase(animators_.find(ptr));
     delete (it->second); // remove pointer
   }
  }
 
 protected:
  animatorMap animators_;
   
};        
} // namespace rp