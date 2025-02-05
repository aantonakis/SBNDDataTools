#include <TFile.h>
#include <TNtuple.h>
#include <TTree.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <cmath>


void pmt_tophat(const char* input_file, const char* output_file) {

  
    TFile *infile = TFile::Open(input_file, "READ");
    if (!infile || infile->IsZombie()) {
        std::cerr << "Error: Cannot open the ROOT file!" << std::endl;
        return;
    }

    TFile* outfile = new TFile(output_file, "RECREATE");

    

    TNtuple* event_tree = (TNtuple*)infile->Get("pmt_tree");
      if (!event_tree) {
        std::cerr << "File doesn't have the tree you are looking for mate!" << std::endl;
        infile->Close();
        return;
    }




    std::cout << "Finished Analysis" << std::endl;
    outfile->Close();
    infile->Close();

    delete outfile;     // Clean up the output file
    delete infile;      // Clean up the input file

    std::cout << "Finished Closing Files" << std::endl;


} // end of main





