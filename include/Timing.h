//  ------------------------------------------------------------------------ // 
//  ===== Timing.h ========================================================= // 
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

#include <cmath>

namespace rp {
  
class TimeBase
{
 public:
  TimeBase () {}
  virtual double operator()(double ttime, double start, double end, bool& finished) = 0;
};

/*
// ====== Time Function Objects ===============================================
These get passed into the go() method of the Animator class

Use:
  Time::Linear()     // base
  Time::Repeat()     // repeat forever
  Time::Repeat(2)    // repeat twice

  I think you get it.

*/

struct Timing
{
  class Linear : public TimeBase
  {
  public:
    Linear() {}
    double operator()(double ttime, double start, double end, bool& finished) {
      double nT = (ttime - start) / end;
      if (nT >= 1.0) {
        nT = 1.0;
        finished = true;
      } 
      return nT;
    }
  };
  
  class Repeat : public TimeBase
  {
   public:
    Repeat () : repeatForever_(true), repeatCt_(0) {}
    Repeat (int repeats) : 
      repeatForever_(false), 
      repeats_(repeats),
      repeatCt_(0) {}
    
    double operator()(double ttime, double start, double end, bool& finished) {
      double s = ((ttime - start) / end) - repeatCt_;
            
      if (s >= 0.9999999) { // doubles are quite freaking precise..
        s = 1.0;
        repeatCt_++;
        
        if (!repeatForever_) {
          if (repeatCt_ == repeats_) finished = true; // done!
        }
      }
      return s;
    }
   private:
    bool repeatForever_;
    double repeats_;
    double repeatCt_;
  };
  
  class PingPong : public TimeBase
  {
  public:
    PingPong () : repeatForever_(true) {}
    PingPong (int repeats) : 
      repeatForever_(false),
      repeats_(repeats),
      repeatCt_(0) {}
    
    double operator()(double ttime, double start, double end, bool& finished) {
     double nT = (ttime - start) / end;
     double mnT = fmod(nT, 1);
     if (fmod(nT, 2) >= 1) {
       mnT = 1 - mnT;
     }
    
     /* TODOK: this is unreliable with more precise jumps in time */
     if (!repeatForever_ && (mnT <= .00001)) {
      repeatCt_++;
      if (repeatCt_-1 == repeats_) finished = true; // done!
     }
  
     return mnT;
    }
    
  private:
    bool repeatForever_;
    double repeats_;
    double repeatCt_;
    
    bool ping_;
    bool pong_;
  };
};

} // namespace rp