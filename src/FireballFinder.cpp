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

#include "FireballFinder.hpp"
#include "cmdopt.hpp"

FireballFinder::~FireballFinder() {
    if (this->_worker != nullptr)
        delete this->_worker;
}

void FireballFinder::execute() {
    this->_worker = new boost::thread(&FireballFinder::process_frames, this);
}

boost::ptr_vector<TimeSpace>* FireballFinder::get_clips() {
    return &this->_results;
}

boost::thread* FireballFinder::get_worker() const {
    return this->_worker;
}

bool FireballFinder::is_fireball(const cv::Mat& flow) {
    for (int y = this->_opt.get_blind_top(); y < (flow.rows - this->_opt.get_blind_bottom()); y += this->_opt.get_density()){
        
        for (int x = this->_opt.get_blind_left(); x < (flow.cols - this->_opt.get_blind_right()); x += this->_opt.get_density()) {
            const cv::Point2f& fxy = flow.at<cv::Point2f > (y, x);
            int vect_len = sqrt(pow(fxy.x, 2) + pow(fxy.y, 2));

            if (vect_len >= this->_opt.get_min_vector_len())
                return true;
        }
    }
    return false;
}

void FireballFinder::process_frames() {
    boost::mutex close_mutex;
    boost::mutex::scoped_lock close_lock;
    close_lock = boost::mutex::scoped_lock(close_mutex, boost::defer_lock);
    
    if (!this->_cap.isOpened())
        return;

    int frame_second = this->_cap.get(CV_CAP_PROP_FPS);

    cv::Mat prevgray, gray, flow, frame;

    for (;;) {
        if (!this->_cap.read(frame))break;

        cv::cvtColor(frame, gray, CV_BGR2GRAY);

        if (prevgray.data) {
            cv::calcOpticalFlowFarneback(prevgray, gray, flow,
                                         this->_opt.get_pyrScale(), this->_opt.get_levels(),
                                         this->_opt.get_winsize(), this->_opt.get_iterations(),
                                         this->_opt.get_polyN(), this->_opt.get_polySigma(),
                                         this->_opt.will_use_farneback_gaussian() ? cv::OPTFLOW_FARNEBACK_GAUSSIAN : 0);

        }

        std::swap(prevgray, gray);

        if (flow.data) {
            if (this->is_fireball(flow)) {
                int len = frame_second * this->_opt.get_clip_len();
                long int begin = this->_offset + this->_cap.get(CV_CAP_PROP_POS_FRAMES) - (frame_second * DEFAULT_CLIP_SP_DIFF);
                TimeSpace *tm = new TimeSpace(begin, len);
                this->_results.push_back(tm);

                TimeSpace::skipFrames(this->_cap, len);
                prevgray.release();
                continue;
            }
        }

        TimeSpace::skipFrames(this->_cap, this->_opt.get_skipframes());

    }
    close_lock.lock();
    this->_cap.release();
    close_lock.unlock();
}
