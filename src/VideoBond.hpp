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

#ifndef VIDEOBOND_HPP
#define	VIDEOBOND_HPP

#include <opencv2/highgui/highgui.hpp>
#include <boost/filesystem.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include <exception>

#include "CmdOpt.hpp"
#include "TimeSpace.hpp"

class VBNoClipsException : public std::exception {

    virtual const char* what() const throw () {
        return "No fireballs found.";
    }
};
extern VBNoClipsException NoClipsException;

class VBVideoInStreamException : public std::exception {

    virtual const char* what() const throw () {
        return "Cannot open input video stream!";
    }
};
extern VBVideoInStreamException VideoInStreamException;

class VBVideoOutStreamException : public std::exception {

    virtual const char* what() const throw () {
        return "Cannot open output video stream!";
    }
};
extern VBVideoOutStreamException VideoOutStreamException;

class VideoBond {
public:

    VideoBond(boost::ptr_vector<TimeSpace> &clips, CmdOptions opt) : _clips(clips), _opt(opt) {
    }

    void bond();

private:
    boost::ptr_vector<TimeSpace> &_clips;
    CmdOptions _opt;
    cv::VideoCapture _cap;
};

/* Replaced by C++11 keyword auto */
//typedef boost::ptr_vector<TimeSpace>::iterator TimeSpaceIterator;

#endif	/* VIDEOBOND_HPP */

