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

#include "VideoBond.hpp"

using namespace boost::filesystem;
using namespace cv;

VBNoClipsException NoClipsException;
VBVideoOutStreamException VideoOutStreamException;
VBVideoInStreamException VideoInStreamException;

void VideoBond::bond() {
    std::ostringstream o;
    o << "ffb_" << path(this->_opt.get_input_file()).stem().string() << ".avi";

    if (this->_clips.empty()) throw NoClipsException;

    this->_cap.open(this->_opt.get_input_file());
    if (!this->_cap.isOpened()) throw VideoInStreamException;

    Size siz = Size((int) this->_cap.get(CV_CAP_PROP_FRAME_WIDTH), (int) this->_cap.get(CV_CAP_PROP_FRAME_HEIGHT));

    VideoWriter wrt(o.str(), this->_cap.get(CV_CAP_PROP_FOURCC), this->_cap.get(CV_CAP_PROP_FPS), siz, true);
    if (!wrt.isOpened()) throw VideoOutStreamException;

    Mat frame;

    int pos;
    for (auto clip : this->_clips) {

        pos = this->_cap.get(CV_CAP_PROP_POS_FRAMES);

        if (pos != clip.getBegin()) {
            TimeSpace::skipFrames(this->_cap, clip.getBegin() - pos);
        }

        for (int i = 0; i < clip.getLength(); i++) {
            if (!this->_cap.read(frame)) {
                break;
            }
            wrt.write(frame);
        }
    }

    this->_cap.release();
}
