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

#ifndef CMDOPT_H
#define	CMDOPT_H

#include <iostream>
#include <boost/program_options.hpp>

#define CMDOPT_DEFAULT_THREADS_NUMBER 8
#define CMDOPT_DEFAULT_PYR_SCALE 0.5
#define CMDOPT_DEFAULT_LEVELS 3
#define CMDOPT_DEFAULT_WINSIZE 15
#define CMDOPT_DEFAULT_ITERATIONS 3
#define CMDOPT_DEFAULT_POLYN 5
#define CMDOPT_DEFAULT_POLYSIGMA 1.2
#define CMDOPT_DEFAULT_USE_FBGAUSSIAN false
#define CMDOPT_DEFAULT_DENSITY 16
#define CMDOPT_DEFAULT_MIN_VECT_LEN 6
#define CMDOPT_DEFAULT_SKIPFRAMES 12
#define COMOPT_DEFAULT_CLIP_LENGTH 11
#define CMDOPT_DEFAULT_VERBOSE false
#define CMDOPT_DEFAULT_BLIND_BOT 20
#define CMDOPT_DEFAULT_BLIND_TOP 0
#define CMDOPT_DEFAULT_BLIND_LEFT 0
#define CMDOPT_DEFAULT_BLIND_RIGHT 0


namespace po = boost::program_options;

class CmdOptions {
public:
    CmdOptions(int ac, char* av[]);

    std::string get_input_file() const;
    int get_threads_number() const;
    double get_pyrScale() const;
    int get_levels() const;
    int get_winsize() const;
    int get_iterations() const;
    int get_polyN() const;
    double get_polySigma() const;
    bool will_use_farneback_gaussian() const;
    int get_density() const;
    int get_min_vector_len() const;
    int get_skipframes() const;
    int get_clip_len() const;
    bool get_verbose() const;
    int get_blind_bottom() const;
    int get_blind_top() const;
    int get_blind_left() const;
    int get_blind_right() const;

private:
    void _init_options();
    void _parse_options(int ac, char* av[]);

    std::string _input_file;
    int _threads_number;
    double _pyrScale;
    int _levels;
    int _winsize;
    int _iterations;
    int _polyN;
    int _density;
    int _min_vector_len;
    int _skipframes;
    int _clip_len;
    double _polySigma;
    bool _use_farneback_gaussian;
    bool _verbose;
    int _blind_bottom;
    int _blind_top;
    int _blind_left;
    int _blind_right;
};


#endif	/* CMDOPT_H */

