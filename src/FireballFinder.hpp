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

#ifndef FIREBALLFINDER_HPP
#define	FIREBALLFINDER_HPP

#include <opencv2/video/tracking.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

#pragma GCC diagnostic ignored "-Wstrict-aliasing"
#include <boost/thread.hpp>
#pragma GCC diagnostic warning "-Wstrict-aliasing"

#include <boost/ptr_container/ptr_vector.hpp>
#include <math.h>

#include "TimeSpace.hpp"
#include "CmdOpt.hpp"
#include "VideoPartition.hpp"

#define DEFAULT_CLIP_SP_DIFF 1

class FireballFinder {
public:

    FireballFinder(std::string partition_path, long int offset, CmdOptions opt) : _partition_path(partition_path), _offset(offset), _opt(opt) {
        __init__();
    }

    FireballFinder(VideoPartition vp, CmdOptions opt) : _partition_path(vp.get_path()), _offset(vp.get_offset()), _opt(opt) {
        __init__();
    }

    virtual ~FireballFinder();

    void execute();
    void process_frames();
    boost::ptr_vector<TimeSpace>* get_clips();
    boost::thread* get_worker() const;

private:

    void __init__() { 
        /* Sadly gcc4.6 and 4.7 does not support 'delegating_constructors' */
        boost::mutex open_mutex;
        boost::mutex::scoped_lock open_lock;

        open_lock = boost::mutex::scoped_lock(open_mutex, boost::defer_lock);
        open_lock.lock();
        this->_cap.open(this->_partition_path);
        open_lock.unlock();
    }

    bool is_fireball(const cv::Mat& flow);

    std::string _partition_path;
    long int _offset;
    CmdOptions _opt;
    boost::thread* _worker;
    cv::VideoCapture _cap;
    boost::ptr_vector<TimeSpace> _results;
};

/* Replaced by C++11 keyword auto */
//typedef boost::ptr_vector<FireballFinder>::iterator FBFinderIterator;

#endif	/* FIREBALLFINDER_HPP */

