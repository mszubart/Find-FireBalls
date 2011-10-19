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

#include "VideoPartitioner.hpp"

namespace bfs = boost::filesystem;

VideoPartitioner::~VideoPartitioner() {
    for (PartitionIterator partition = this->_partitions.begin(); partition != this->_partitions.end(); ++partition) {
        bfs::path tmp_file((*partition).get_path());

        if (bfs::exists(tmp_file))
            bfs::remove(tmp_file);
    }
}

bool VideoPartitioner::partition_file(int parts) {
    this->_cap.open(this->_opt.get_input_file());

    if (!_cap.isOpened())
        return false;

    cv::Size siz = cv::Size((int) this->_cap.get(CV_CAP_PROP_FRAME_WIDTH), (int) this->_cap.get(CV_CAP_PROP_FRAME_HEIGHT));

    cv::VideoWriter wrt;

    cv::Mat frame;

    long int fctr = 0;
    int partnum = 1;
    long int divnum = this->_cap.get(CV_CAP_PROP_FRAME_COUNT) / parts;

    for (;;) {

        if (!wrt.isOpened() || (divnum == fctr && partnum <= parts)) {
            fctr = 0;
            std::ostringstream o;
            long int ofs = (divnum * partnum) - divnum;
            o << "_tmp_" << bfs::path(this->_opt.get_input_file()).stem().string() << "_" << partnum++ << ".avi";
            wrt.open(o.str(), this->_cap.get(CV_CAP_PROP_FOURCC), this->_cap.get(CV_CAP_PROP_FPS), siz, true);
            if (!wrt.isOpened()) return false;

            VideoPartition *prt = new VideoPartition(o.str(), ofs);
            this->_partitions.push_back(prt);

        }

        if (!this->_cap.read(frame))break;
        wrt.write(frame);
        fctr++;
    }

    this->_cap.release();
    return true;
}

void exec_finder(boost::thread_group &worker_group, boost::ptr_vector<FireballFinder> &fb_finders, VideoPartition *prt, CmdOptions *opt) {
    FireballFinder *ff = new FireballFinder(*prt, *opt);
    ff->execute();
    worker_group.add_thread(ff->get_worker());
    fb_finders.push_back(ff);
}

bool VideoPartitioner::partition_file(boost::thread_group &worker_group, boost::ptr_vector<FireballFinder> &fb_finders, int parts) {
    this->_cap.open(this->_opt.get_input_file());

    if (!_cap.isOpened())
        return false;

    cv::Size siz = cv::Size((int) this->_cap.get(CV_CAP_PROP_FRAME_WIDTH), (int) this->_cap.get(CV_CAP_PROP_FRAME_HEIGHT));
    cv::Mat frame;
    cv::VideoWriter wrt;

    VideoPartition *prt = NULL;

    long int divnum = this->_cap.get(CV_CAP_PROP_FRAME_COUNT) / parts;
    long int partlen = 0;

    for (int partnum = 1; partnum <= parts; partnum++) {

        std::ostringstream o;
        long int ofs = (divnum * partnum) - divnum;
        o << "_tmp_" << bfs::path(this->_opt.get_input_file()).stem().string() << "_" << partnum << ".avi";
        wrt.open(o.str(), this->_cap.get(CV_CAP_PROP_FOURCC), this->_cap.get(CV_CAP_PROP_FPS), siz, true);
        if (!wrt.isOpened()) return false;

        prt = new VideoPartition(o.str(), ofs);
        this->_partitions.push_back(prt);

        if (partnum < parts)
            partlen = divnum;
        else
            partlen = this->_cap.get(CV_CAP_PROP_FRAME_COUNT) - (divnum * (parts - 1));

        for (long int i = 0; i < partlen; i++) {
            if (!this->_cap.read(frame))break;
            wrt.write(frame);
        }

        exec_finder(worker_group, fb_finders, prt, &this->_opt);
    }
    this->_cap.release();


    return true;
}

boost::ptr_vector<VideoPartition>* VideoPartitioner::get_partitions() {
    return &this->_partitions;
}
