//
// Created by Adina Neda on 26/12/2019.
// Released under license: Apache-2.0 License
//

#ifndef PART2_PROJCODE_JOBSINPUTANALYSIS_H
#define PART2_PROJCODE_JOBSINPUTANALYSIS_H

#include <iostream>
#include <climits>
#include <filesystem>
#include <string>
#include <fstream>

using namespace std;

class JobsInputAnalysis {
public:

    /**
     * Run statistics about the input file
     * @param generated_input
     * @param output_file_results
     */
    static void input_statistics(
            ifstream& generated_input,
            ofstream& output_file_results
            );

};


#endif //PART2_PROJCODE_JOBSINPUTANALYSIS_H
