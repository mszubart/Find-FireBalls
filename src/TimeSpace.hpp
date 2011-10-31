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

#ifndef TIMESPACE_HPP
#define	TIMESPACE_HPP

#include <opencv2/highgui/highgui.hpp>
#include <algorithm>
#include <functional>

class TimeSpace {
public:
    TimeSpace(long int begin, int len);

    int getLength() const;
    long int getBegin() const;

    static bool skipFrames(cv::VideoCapture &cap, int frames) {
        bool result = true;

        for (int i = 0; i < frames; i++) {
            result = cap.grab();

            if (result == false)
                break;
        }

        return result;
    }

    struct TimeSpaceComparer : public std::binary_function<TimeSpace, TimeSpace, bool> {

        bool operator()(TimeSpace lhs, TimeSpace rhs) {
            return (lhs._begin < rhs._begin);
        }
    };

private:
    long int _begin;
    int _len;
};

#endif	/* TIMESPACE_HPP */
