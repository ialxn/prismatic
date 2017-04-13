//
// Created by Alan Pryor on 4/12/17.
//

#include "parseInput.h"
#include <iostream>
#include <map>
#include <string>
#include <stdlib.h>

namespace PRISM {
    using namespace std;
    using parseFunction = ParseResult (*)(Metadata<PRISM_FLOAT_PRECISION>& meta,
                                          int& argc, const char*** argv);


    void printSyntax() {
        std::cout << "Syntax is ./prism [options]" << std::endl;
    }
    void printHelp() {
        std::cout << "Options:\n";
        std::cout << "-f interpolation_factor : PRISM interpolation factor\n";
        std::cout << "-g num_GPUs : number of GPUs to use\n";
        std::cout << "-i filename : input file containing atomic coordinates and species. Should be a \
                  comma-separated text file with one row per atom of the form x, y, z, Z where Z is the atomic number.\n";
        std::cout << "-j num_threads : number of CPU threads to use\n";
        std::cout << "-o filename : output filename\n";
        std::cout << "-s num_streams : number of CUDA streams to create per GPU\n";
    }

    ParseResult parse_h(Metadata<PRISM_FLOAT_PRECISION>& meta,
                        int& argc, const char*** argv){
        printHelp();
        return ParseResult::Failure;
    };

    ParseResult parse_i(Metadata<PRISM_FLOAT_PRECISION>& meta,
                        int& argc, const char*** argv){
        if (argc < 2){
            cout << "No filename provided for -i (syntax is -i filename)\n";
            return ParseResult::Failure;
        }
        meta.filename_atoms = std::string((*argv)[1]);
        argc-=2;
        argv[0]+=2;
        return ParseResult::Success;
    };

    ParseResult parse_f(Metadata<PRISM_FLOAT_PRECISION>& meta,
                        int& argc, const char*** argv){
        if (argc < 2){
            cout << "No interpolation factor provided for -f (syntax is -f interpolation_factor)\n";
            return ParseResult::Failure;
        }
        if ( (meta.interpolationFactor = atoi((*argv)[1])) == 0){
            cout << "Invalid value \"" << (*argv)[1] << "\" provided for PRISM interpolation factor (syntax is -f interpolation_factor)\n";
            return ParseResult::Failure;
        }
        argc-=2;
        argv[0]+=2;
        return ParseResult::Success;
    };

    ParseResult parse_j(Metadata<PRISM_FLOAT_PRECISION>& meta,
                        int& argc, const char*** argv){

        if (argc < 2){
            cout << "No number of threads provided (syntax is -j num_threads)\n";
            return ParseResult::Failure;
        }
        if ( (meta.NUM_THREADS = atoi((*argv)[1])) == 0){
            cout << "Invalid value \"" << (*argv)[1] << "\" provided for number of threads  (syntax is -j num_threads)\n";
            return ParseResult::Failure;
        }
        argc-=2;
        argv[0]+=2;
        return ParseResult::Success;
    };

    ParseResult parse_g(Metadata<PRISM_FLOAT_PRECISION>& meta,
                        int& argc, const char*** argv){

        if (argc < 2){
            cout << "No number of GPUs provided (syntax is -g num_GPUs)\n";
            return ParseResult::Failure;
        }
        if ( (meta.NUM_GPUS = atoi((*argv)[1])) == 0){
            cout << "Invalid value \"" << (*argv)[1] << "\" provided for number of GPUs (syntax is -g num_GPUs)\n";
            return ParseResult::Failure;
        }
        argc-=2;
        argv[0]+=2;
        return ParseResult::Success;
    };

    ParseResult parse_s(Metadata<PRISM_FLOAT_PRECISION>& meta,
                        int& argc, const char*** argv){

        if (argc < 2){
            cout << "No number of CUDA streams per GPU provided (syntax is -s num_streams)\n";
            return ParseResult::Failure;
        }
        if ( (meta.NUM_STREAMS_PER_GPU = atoi((*argv)[1])) == 0){
            cout << "Invalid value \"" << (*argv)[1] << "\" provided for number of threads  (syntax is -s num_streams)\n";
            return ParseResult::Failure;
        }
        argc-=2;
        argv[0]+=2;
        return ParseResult::Success;
    };

    ParseResult parse_o(Metadata<PRISM_FLOAT_PRECISION>& meta,
                        int& argc, const char*** argv){

        if (argc < 2){
            cout << "No filename provided for -o (syntax is -o filename)\n";
            return ParseResult::Failure;
        }
        meta.filename_output = std::string((*argv)[1]);
        //cout <<"meta.filename_atoms = " << meta.filename_atoms << endl;
        argc-=2;
        argv[0]+=2;
        return ParseResult::Success;
    };

    ParseResult parse_p(Metadata<PRISM_FLOAT_PRECISION>& meta,
                        int& argc, const char*** argv){
        if (argc < 2){
            cout << "No pixel size provided for -p (syntax is -p pixel_size)\n";
            return ParseResult::Failure;
        }
        if ( (meta.realspace_pixelSize = (PRISM_FLOAT_PRECISION)atof((*argv)[1])) == 0){
            cout << "Invalid value \"" << (*argv)[1] << "\" provided for pixel size  (syntax is -p pixel_size)\n";
            return ParseResult::Failure;
        }
        argc-=2;
        argv[0]+=2;
        return ParseResult::Success;
    };

    bool parseInputs(Metadata<PRISM_FLOAT_PRECISION> &meta,
                     int &argc, const char ***argv) {
        if (argc==1)return true; // case of no inputs to parse
        --argc;++(argv[0]);
        ParseResult result;
        do {
            if (argc==0) return true; // successfully parsed all inputs
            result = parseInput(meta, argc, argv);
        } while (result == ParseResult::Success);
        if (result == ParseResult::Help)printHelp();
        return false;
    }
    static std::map<std::string, parseFunction> parser{
            {"--input_file", parse_i}, {"-i", parse_i},
            {"--interp_factor", parse_f}, {"-f", parse_f},
            {"--output_file", parse_o}, {"-o", parse_o},
            {"--num_threads", parse_j}, {"-j", parse_j},
            {"--num_streams", parse_s}, {"-s", parse_s},
            {"--num_gpus", parse_g}, {"-g", parse_g},
            {"--help", parse_h}, {"-h", parse_h},
            {"--pixel_size", parse_p}, {"-p", parse_p}
    };
    ParseResult parseInput(Metadata<PRISM_FLOAT_PRECISION>& meta,
                           int& argc, const char*** argv){
        parseFunction f = parser[std::string((*argv)[0])];
        if (f != NULL){
            return f(meta, argc, argv);
        } else{
            cout << "Invalid option \"" << (*argv)[0] << "\" provided\n";
            return ParseResult::Failure;
        }
    }
}