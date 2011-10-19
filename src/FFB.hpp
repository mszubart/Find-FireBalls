/*
 * Find FireBalls
 * Copyright (c) 2011 Mateusz Szubart
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy of 
 * this software and associated documentation files (the "Software"), to deal in 
 * the Software without restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell copies of 
 * the Software, and to permit persons to whom the Software is furnished to do so, 
 * subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all 
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS 
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR 
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER 
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN 
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE. 
 */

#ifndef FFB_HPP
#define	FFB_HPP

#include "CmdOpt.hpp"
#include "VideoPartitioner.hpp"
#include "FireballFinder.hpp"
#include "VideoBond.hpp"
#include "TimeSpace.hpp"

#define FFB_VERSION "0.2"
#define FFB_AUTHOR "Mateusz Szubart"
#define FFB_AUTHOR_MAIL "<mszubart@gmail.com>"

void merge_clips(boost::ptr_vector<FireballFinder> &finders, boost::ptr_vector<TimeSpace> &clips);
void linear_processing(CmdOptions &opt, boost::ptr_vector<TimeSpace> &clips);
void multithreaded_processing(CmdOptions &opt, boost::ptr_vector<TimeSpace> &clips, boost::thread_group &workes, boost::ptr_vector<FireballFinder> &finders);

#endif	/* FFB_HPP */

