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

#include "CmdOpt.hpp"
#include "VideoPartitioner.hpp"
#include "FireballFinder.hpp"
#include "VideoBond.hpp"
#include "TimeSpace.hpp"

#define FFB_VERSION "0.2"
#define FFB_AUTHOR "Mateusz Szubart"
#define FFB_AUTHOR_MAIL "<mszubart@gmail.com>"

void merge_clips(boost::ptr_vector<FireballFinder> &finders, boost::ptr_vector<TimeSpace> &clips);

int main(int ac, char** av) {
    CmdOptions opt(ac, av);

    if (opt.get_verbose()) {
        std::cout << "Find FireBalls by " << FFB_AUTHOR << " " << FFB_AUTHOR_MAIL << "\n";
        std::cout << "Version: " << FFB_VERSION << "\n\n";

        std::cout << "Configuration\n=============\n";
        std::cout << "Infput file:\t\t" << opt.get_input_file() << "\n";
        std::cout << "Number of threads:\t" << opt.get_threads_number() << "\n";
        std::cout << "Skip frames:\t\t" << opt.get_skipframes() << "\n";
        std::cout << "Clip length:\t\t" << opt.get_clip_len() << "\n";
        std::cout << "Grid density:\t\t" << opt.get_density() << "\n";
        std::cout << "Min vect length:\t" << opt.get_min_vector_len() << "\n";
        std::cout << "Blind region:\t\tTOP: " << opt.get_blind_top()
            << " RIGHT: " << opt.get_blind_right()
            << " BOTTOM: " << opt.get_blind_bottom()
            << " LEFT: " << opt.get_blind_left() << "\n\n";

        std::cout << "Starting video finder ...\n";
    }

    boost::thread_group workers;
    boost::ptr_vector<FireballFinder> finders;

    VideoPartitioner *vp = new VideoPartitioner(opt);

    if (!vp->partition_file(workers, finders, opt.get_threads_number())) {
        std::cerr << "Fatal error: Cannot partition video.";
        exit(EXIT_FAILURE);
    }

    workers.join_all();
    delete vp;

    boost::ptr_vector<TimeSpace> clips;

    merge_clips(finders, clips);
    std::sort(clips.begin(), clips.end(), TimeSpace::TimeSpaceComparer());

    if (opt.get_verbose())
        if (!clips.empty())
            std::cout << "Found " << clips.size() << " fireballs\n";

    VideoBond vb(clips, opt);

    try {
        vb.bond();
    }
    catch (std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}

void merge_clips(boost::ptr_vector<FireballFinder> &finders, boost::ptr_vector<TimeSpace> &clips) {
    for (FBFinderIterator finder = finders.begin();
         finder != finders.end(); ++finder) {

        boost::ptr_vector<TimeSpace> *fc = (*finder).get_clips();
        clips.insert(clips.end(), fc->begin(), fc->end());
    }
}