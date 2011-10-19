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

#include "cmdopt.hpp"

CmdOptions::CmdOptions(int ac, char* av[]) {
    this->_init_options();
    this->_parse_options(ac, av);
}

void CmdOptions::_init_options() {
    this->_threads_number = CMDOPT_DEFAULT_THREADS_NUMBER;
    this->_pyrScale = CMDOPT_DEFAULT_PYR_SCALE;
    this->_levels = CMDOPT_DEFAULT_LEVELS;
    this->_winsize = CMDOPT_DEFAULT_WINSIZE;
    this->_iterations = CMDOPT_DEFAULT_ITERATIONS;
    this->_polyN = CMDOPT_DEFAULT_POLYN;
    this->_polySigma = CMDOPT_DEFAULT_POLYSIGMA;
    this->_use_farneback_gaussian = CMDOPT_DEFAULT_USE_FBGAUSSIAN;
    this->_density = CMDOPT_DEFAULT_DENSITY;
    this->_min_vector_len = CMDOPT_DEFAULT_MIN_VECT_LEN;
    this->_skipframes = CMDOPT_DEFAULT_SKIPFRAMES;
    this->_clip_len = COMOPT_DEFAULT_CLIP_LENGTH;
    this->_verbose = CMDOPT_DEFAULT_VERBOSE;
    this->_blind_bottom = CMDOPT_DEFAULT_BLIND_BOT;
    this->_blind_top = CMDOPT_DEFAULT_BLIND_TOP;
    this->_blind_left = CMDOPT_DEFAULT_BLIND_LEFT;
    this->_blind_right = CMDOPT_DEFAULT_BLIND_RIGHT;
}

void CmdOptions::_parse_options(int ac, char* av[]) {
    try {
        po::options_description _desc("Some descriptions were take from OpenCV documentation (http://opencv.itseez.com/modules/refman.html)\nAllowed options:");
        _desc.add_options()
            ("help,h", "Show this message.\n")
            ("input-file", po::value<std::string > (), "File with video material.\n")
            ("skipframes,S", po::value<int>(), "Number of frames to be skipped. Every skipframes-th frame will be processed.\nDefault value = 12\n")
            ("clip-len,L", po::value<int>(), "Single clip length in seconds.\nDefault value = 11\n")
            ("density", po::value<int>(), "Density of samples is space (in pixels) between the samples.\nDefault value = 16\n")
            ("min-vect-len", po::value<int>(), "Minimal flow vector lenght to be assumed as movement.\nDefault value = 6\n")
            ("blind_top", po::value<int>(), "Blind field, from frame edge to arg.\nDefault value = 0\n")
            ("blind_left", po::value<int>(), "Blind field, from frame edge to arg.\nDefault value = 0\n")
            ("blind_right", po::value<int>(), "Blind field, from frame edge to arg.\nDefault value = 0\n")
            ("blind_bottom", po::value<int>(), "Blind field, from frame edge to arg.\nDefault value = 20\n")
            ("threads,T", po::value<int>(), "Set number of threads used for video processing.\nBest value is 1 thread per core thread (eg. Intel i7 has 4 cores and 8 threads, best value is 8). Warning: Video material is divided for each thread so you need at least 2x video size, free hdd space.\nDefault value = 8\n")
            ("pyr-scale", po::value<double>(), "Parameter specifying the image scale (<1) to build pyramids for each image. \npyrScale=0.5 means a classical pyramid, where each next layer is twice smaller than the previous one.\nDefault value = 0.5\n")
            ("levels", po::value<int>(), "Number of pyramid layers including the initial image. \nlevels=1 means that no extra layers are created and only the original images are used.\nDefault value = 3\n")
            ("winsize", po::value<int>(), "Averaging window size. \nLarger values increase the algorithm robustness to image noise and give more chances for fast motion detection, but yield more blurred motion field.\nDefault value = 15\n")
            ("iterations", po::value<int>(), "Number of iterations the algorithm does at each pyramid level.\nDefault value = 3\n")
            ("polyn", po::value<int>(), "Size of the pixel neighborhood used to find polynomial expansion in each pixel. \nLarger values mean that the image will be approximated with smoother surfaces, yielding more robust algorithm and more blurred motion field. \nTypically, polyN =5 or 7.\nDefault value = 5\n")
            ("poly-sigma", po::value<double>(), "Standard deviation of the Gaussian that is used to smooth derivatives used as a basis for the polynomial expansion. \nFor polyN=5 , you can set polySigma=1.1. For polyN=7, a good value would be polySigma=1.5\nDefault value = 1.2\n")
            ("use-farneback-gaussian,G", "Use the Gaussian filter instead of a box filter of the same size for optical flow estimation. \nUsually, this option gives z more accurate flow than with a box filter, at the cost of lower speed.\nNormally, winsize for a Gaussian window should be set to a larger value to achieve the same level of robustness.\nDefault value = false\n")
            ("verbose,V", "Be verbose.\nDefault value = false\n")
            ;

        po::positional_options_description p;
        p.add("input-file", -1);

        po::variables_map _vm;
        po::store(po::command_line_parser(ac, av).options(_desc).positional(p).run(), _vm);

        po::notify(_vm);

        if (_vm.count("help")) {
            std::cout << "Usage: ffb input.avi [options]\n";
            std::cout << _desc;
            exit(EXIT_SUCCESS);
        }

        if (_vm.count("input-file")) {
            this->_input_file = _vm["input-file"].as<std::string > ();
        }
        else {
            std::cerr << "No input file specified.\nUsage: ffb input.avi [options]\n";
            std::cout << _desc;
            exit(EXIT_FAILURE);
        }

        if (_vm.count("threads")) {
            this->_threads_number = _vm["threads"].as<int>();

            if (this->_threads_number < 1) {
                std::cerr << "No threads? Really? Give up, 1 thread is minimum...\n";
                exit(EXIT_FAILURE);
            }
        }

        if (_vm.count("pyr-scale")) {
            this->_pyrScale = _vm["pyr-scale"].as<double>();
        }

        if (_vm.count("levels")) {
            this->_levels = _vm["levels"].as<int>();
        }

        if (_vm.count("winsize")) {
            this->_winsize = _vm["winsize"].as<int>();
        }

        if (_vm.count("iterations")) {
            this->_iterations = _vm["iterations"].as<int>();
        }

        if (_vm.count("polyn")) {
            this->_polyN = _vm["polyn"].as<int>();
        }

        if (_vm.count("density")) {
            this->_density = _vm["density"].as<int>();
        }

        if (_vm.count("blind_top")) {
            this->_blind_top = _vm["blind_top"].as<int>();
        }

        if (_vm.count("blind_left")) {
            this->_blind_top = _vm["blind_top"].as<int>();
        }

        if (_vm.count("blind_right")) {
            this->_blind_right = _vm["blind_right"].as<int>();
        }

        if (_vm.count("blind_bottom")) {
            this->_blind_bottom = _vm["blind_bottom"].as<int>();
        }

        if (_vm.count("min-vect-len")) {
            this->_min_vector_len = _vm["min-vect-len"].as<int>();
        }

        if (_vm.count("skipframes")) {
            this->_skipframes = _vm["skipframes"].as<int>();
        }

        if (_vm.count("clip-len")) {
            this->_clip_len = _vm["clip-len"].as<int>();
        }

        if (_vm.count("poly-sigma")) {
            this->_polySigma = _vm["poly-sigma"].as<double>();
        }

        if (_vm.count("use-farneback-gaussian")) {
            this->_use_farneback_gaussian = true;
        }

        if (_vm.count("verbose")) {
            this->_verbose = true;
        }
    }
    catch (std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        exit(EXIT_FAILURE);
    }
    catch (...) {
        std::cerr << "Exception of unknown type!\n";
        exit(EXIT_FAILURE);
    }

}

std::string CmdOptions::get_input_file() const {
    return this->_input_file;
}

int CmdOptions::get_threads_number() const {
    return this->_threads_number;
}

double CmdOptions::get_pyrScale() const {
    return this->_pyrScale;
}

int CmdOptions::get_levels() const {
    return this->_levels;
}

int CmdOptions::get_winsize() const {
    return this->_winsize;
}

int CmdOptions::get_iterations() const {
    return this->_iterations;
}

int CmdOptions::get_polyN() const {
    return this->_polyN;
}

double CmdOptions::get_polySigma() const {
    return this->_polySigma;
}

bool CmdOptions::will_use_farneback_gaussian() const {
    return this->_use_farneback_gaussian;
}

int CmdOptions::get_density() const {
    return this->_density;
}

int CmdOptions::get_min_vector_len() const {
    return this->_min_vector_len;
}

int CmdOptions::get_skipframes() const {
    return this->_skipframes;
}

int CmdOptions::get_clip_len() const {
    return this->_clip_len;
}

bool CmdOptions::get_verbose() const {
    return this->_verbose;
}

int CmdOptions::get_blind_bottom() const {
    return this->_blind_bottom;
}

int CmdOptions::get_blind_top() const {
    return this->_blind_top;
}

int CmdOptions::get_blind_left() const {
    return this->_blind_left;
}

int CmdOptions::get_blind_right() const {
    return this->_blind_right;
}
