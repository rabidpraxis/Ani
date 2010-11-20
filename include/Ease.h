//  ------------------------------------------------------------------------ // 
//  ===== Easing.h ========================================================= // 
//  ------------------------------------------------------------------------ // 
//   Created:        Kevin Webster                                           // 
//   Date:           10.10.20                                                // 
//   Copyright (c)   2010 All rights reserved.                               // 
//  ------------------------------------------------------------------------ // 
//  Redistribution and use in source and binary forms, with or without       // 
//  modification, are permitted provided thadouble the following conditions       // 
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

/*

  Adapted from cppPennerEasing library - Copyright © 2001 Robert Penner
  All rights reserved
  
  Thank you Mr. Penner!

*/

#pragma once

#include <cmath>

#define PI 3.14159265

namespace rp {

struct Ease
{
  /*
    t = time
    b = beginning
    c = change
    d = duration
  */
  
  // ------ Linear ------------------------------------------------------------
  template <typename T>
  static T NoneLinear (double t, T b , T c, double d) {
  	return c*t/d + b;
  }
  template <typename T>
  static T InLinear (double t, T b , T c, double d) {
  	return c*t/d + b;
  }
  template <typename T>
  static T OutLinear(double t, T b , T c, double d) {	
  	return c*t/d + b;
  }
  template <typename T>
  static T InOutLinear(double t, T b , T c, double d) {
  	return c*t/d + b;
  }
  // ------ Sine --------------------------------------------------------------
  template <typename T>
  static T InSine (double t, T b , T c, double d) {
  	return -c * std::cos(t/d * (PI/2)) + c + b;
  }
  template <typename T>
  static T OutSine(double t, T b , T c, double d) {	
  	return c * std::sin(t/d * (PI/2)) + b;	
  }
  template <typename T>
  static T InOutSine(double t, T b , T c, double d) {
  	return -c/2 * (std::cos(PI*t/d) - 1) + b;
  }
  
  // ------ Back --------------------------------------------------------------
  template <typename T>
  static T InBack (double t, T b , T c, double d) {
  	T s = 1.70158f;
  	T postFix = t/=d;
  	return c*(postFix)*t*((s+1)*t - s) + b;
  }
  template <typename T>
  static T OutBack (double t, T b , T c, double d) {	
  	T s = 1.70158f;
  	return c*((t=t/d-1)*t*((s+1)*t + s) + 1) + b;
  }
  template <typename T>
  static T InOutBack (double t, T b , T c, double d) {
  	T s = 1.70158f;
  	if ((t/=d/2) < 1) return c/2*(t*t*(((s*=(1.525f))+1)*t - s)) + b;
  	T postFix = t-=2;
  	return c/2*((postFix)*t*(((s*=(1.525f))+1)*t + s) + 2) + b;
  }
  // ------ Bounce ------------------------------------------------------------
  // template <typename T>
  // static T InBounce (double t, T b , T c, double d) {
  //  return c - OutBounce(d-t, b, c, d) + b;
  // }
  // template <typename T>
  // static T OutBounce (double t, T b , T c, double d) {
  //  if ((t/=d) < (1/2.75f)) {
  //    return c*(7.5625f*t*t) + b;
  //  } else if (t < (2/2.75f)) {
  //    T postFix = t-=(1.5f/2.75f);
  //    return c*(7.5625f*(postFix)*t + .75f) + b;
  //  } else if (t < (2.5/2.75)) {
  //      T postFix = t-=(2.25f/2.75f);
  //    return c*(7.5625f*(postFix)*t + .9375f) + b;
  //  } else {
  //    T postFix = t-=(2.625f/2.75f);
  //    return c*(7.5625f*(postFix)*t + .984375f) + b;
  //  }
  // }
  // template <typename T>
  // static T InOutBounce (double t, T b , T c, double d) {
  //  if (t < d/2) return InBounce(t*2, b, c, d) * .5f + b;
  //  else return OutBounce(t*2-d, b, c, d) * .5f + c*.5f + b;
  // }
  // ------ Circ --------------------------------------------------------------
  template <typename T>
  static T InCirc (double t, T b , T c, double d) {
  	return -c * (std::sqrt(1 - (t/=d)*t) - 1) + b;
  }
  template <typename T>
  static T OutCirc(double t, T b , T c, double d) {
  	return c * std::sqrt(1 - (t=t/d-1)*t) + b;
  }
  template <typename T>
  static T InOutCirc(double t, T b , T c, double d) {
  	if ((t/=d/2) < 1) return -c/2 * (std::sqrt(1 - t*t) - 1) + b;
  	return c/2 * (std::sqrt(1 - t*(t-=2)) + 1) + b;
  }
  // ------ Cubic -------------------------------------------------------------
  template <typename T>
  static T InCubic (double t, T b , T c, double d) {
  	return c*(t/=d)*t*t + b;
  }
  template <typename T>
  static T OutCubic(double t, T b , T c, double d) {
  	return c*((t=t/d-1)*t*t + 1) + b;
  }
  template <typename T>
  static T InOutCubic(double t,T b , T c, double d) {
  	if ((t/=d/2) < 1) return c/2*t*t*t + b;
  	return c/2*((t-=2)*t*t + 2) + b;	
  }
  // ------ Elastic -----------------------------------------------------------
  // template <typename T>
  // static T InElastic (double t, T b , T c, double d) {
  //  if (t==0) return b;  if ((t/=d)==1) return b+c;  
  //  T p=T(d*.3f);
  //  T a=c; 
  //  T s=p/4;
  //  T postFix =a*std::pow(2,10*(t-=1)); // this is a fix, again, with post-increment operators
  //  return -(postFix * std::sin((t*d-s)*(2*PI)/p )) + b;
  // }
  // template <typename T>
  // static T OutElastic(double t, T b , T c, double d) {
  //  if (t==0) return b;  if ((t/=d)==1) return b+c;  
  //  T p=d*.3f;
  //  T a=c; 
  //  T s=p/4;
  //  return (a*pow(2,-10*t) * sin( (t*d-s)*(2*PI)/p ) + c + b);  
  // }
  // template <typename T>
  // static T InOutElastic(double t, T b , T c, double d) {
  //  if (t==0) return b;  if ((t/=d/2)==2) return b+c; 
  //  T p=d*(.3f*1.5f);
  //  T a=c; 
  //  T s=p/4;
  // 
  //  if (t < 1) {
  //    T postFix =a*std::pow(2,10*(t-=1)); // postIncrement is evil
  //    return -.5f*(postFix* std::sin( (t*d-s)*(2*PI)/p )) + b;
  //  } 
  //  T postFix =  a*std::pow(2,-10*(t-=1)); // postIncrement is evil
  //  return postFix * std::sin( (t*d-s)*(2*PI)/p )*.5f + c + b;
  // }
  // ------ Expo --------------------------------------------------------------
  template <typename T>
  static T InExpo (double t, T b , T c, double d) {
  	return (t==0) ? b : c * std::pow(2, 10 * (t/d - 1)) + b;
  }
  template <typename T>
  static T OutExpo(double t, T b , T c, double d) {
  	return (t==d) ? b+c : c * (-std::pow(2, -10 * t/d) + 1) + b;	
  }
  template <typename T>
  static T InOutExpo(double t, T b , T c, double d) {
  	if (t==0) return b;
  	if (t==d) return b+c;
  	if ((t/=d/2) < 1) return c/2 * std::pow(2, 10 * (t - 1)) + b;
  	return c/2 * (-std::pow(2, -10 * --t) + 2) + b;
  }
  // ------ Quad --------------------------------------------------------------
  template <typename T>
  static T InQuad (double t, T b , T c, double d) {
  	return c*(t/=d)*t + b;
  }
  template <typename T>
  static T OutQuad(double t, T b , T c, double d) {
  	return -c *(t/=d)*(t-2) + b;
  }
  template <typename T>
  static T InOutQuad(double t, T b , T c, double d) {
  	if ((t/=d/2) < 1) return ((c/2)*(t*t)) + b;
  	return -c/2 * (((t-2)*(--t)) - 1) + b;
  	/*
  	originally return -c/2 * (((t-2)*(--t)) - 1) + b;
 
  	I've had to swap (--t)*(t-2) due to diffence in behaviour in 
  	pre-increment operators between java and c++, after hours
  	of joy
  	*/

  }
  // ------ Quart -------------------------------------------------------------
  template <typename T>
  static T InQuart (double t, T b , T c, double d) {
  	return c*(t/=d)*t*t*t + b;
  }
  template <typename T>
  static T OutQuart(double t, T b , T c, double d) {
  	return -c * ((t=t/d-1)*t*t*t - 1) + b;
  }
  template <typename T>
  static T InOutQuart(double t, T b , T c, double d) {
  	if ((t/=d/2) < 1) return c/2*t*t*t*t + b;
  	return -c/2 * ((t-=2)*t*t*t - 2) + b;
  }
  // ------ Quint -------------------------------------------------------------
  template <typename T>
  static T InQuint (double t, T b , T c, double d) {
  	return c*(t/=d)*t*t*t*t + b;
  }
  template <typename T>
  static T OutQuint(double t, T b , T c, double d) {
  	return c*((t=t/d-1)*t*t*t*t + 1) + b;
  }
  template <typename T>
  static T InOutQuint(double t, T b , T c, double d) {
  	if ((t/=d/2) < 1) return c/2*t*t*t*t*t + b;
  	return c/2*((t-=2)*t*t*t*t + 2) + b;
  }
};
} // namespace rp