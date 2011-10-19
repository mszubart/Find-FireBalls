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

#ifndef VIDEOPARTITIONER_HPP
#define	VIDEOPARTITIONER_HPP

#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <boost/filesystem.hpp>
#include <boost/ptr_container/ptr_vector.hpp>

#include "CmdOpt.hpp"
#include "FireballFinder.hpp"
#include "VideoPartition.hpp"

class VideoPartitioner {
public:

    VideoPartitioner(CmdOptions opt) : _opt(opt) {
    }
    virtual ~VideoPartitioner();

    bool partition_file(int parts);
    bool partition_file(boost::thread_group &worker_group, boost::ptr_vector<FireballFinder> &fb_finders, int parts);
    boost::ptr_vector<VideoPartition>* get_partitions();

private:
    CmdOptions _opt;
    cv::VideoCapture _cap;
    boost::ptr_vector<VideoPartition> _partitions;
    long int _partition_length;
};

#endif	/* VIDEOPARTITIONER_HPP */

