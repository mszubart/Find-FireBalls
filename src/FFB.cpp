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
#include "FFB.hpp"

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

    boost::ptr_vector<TimeSpace> clips;
    boost::thread_group workers;
    boost::ptr_vector<FireballFinder> finders;

    if (opt.get_threads_number() > 1) {
        multithreaded_processing(opt, clips, workers, finders);
    }
    else {
        linear_processing(opt, clips, finders);
    }

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
    for (auto finder : finders) {

        boost::ptr_vector<TimeSpace> *fc = finder.get_clips();
        clips.insert(clips.end(), fc->begin(), fc->end());
    }
}

void linear_processing(CmdOptions &opt, boost::ptr_vector<TimeSpace> &clips, boost::ptr_vector<FireballFinder> &finders) {
    FireballFinder *ff = new FireballFinder(opt.get_input_file(), 1, opt);
    finders.push_back(ff);
    
    ff->process_frames();

    boost::ptr_vector<TimeSpace> *fc = ff->get_clips();
    clips.insert(clips.end(), fc->begin(), fc->end());
}

void multithreaded_processing(CmdOptions &opt, boost::ptr_vector<TimeSpace> &clips, boost::thread_group &workers, boost::ptr_vector<FireballFinder> &finders) {
    VideoPartitioner *vp = new VideoPartitioner(opt);

    if (!vp->partition_file(workers, finders, opt.get_threads_number())) {
        std::cerr << "Fatal error: Cannot partition video.";
        exit(EXIT_FAILURE);
    }

    workers.join_all();
    delete vp;

    merge_clips(finders, clips);
    std::sort(clips.begin(), clips.end(), TimeSpace::TimeSpaceComparer());
}