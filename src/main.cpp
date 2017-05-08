// Std
#include <iostream>
// ROOT
#include "TFile.h"
#include "TTree.h"
#include "TTreeReader.h"
#include "TTreeReaderValue.h"
// Local
#include "../include/BBDecTreeTool.h"

std::map<std::string, TTreeReaderValue<Double_t>*> get_cols(TTreeReader *myReader) {
    TTreeReaderValue<Double_t> *D0CHI2 = new TTreeReaderValue<Double_t>(*myReader, "D0CHI2");
    TTreeReaderValue<Double_t> *D0FD = new TTreeReaderValue<Double_t>(*myReader, "D0FD");
    TTreeReaderValue<Double_t> *DSTDOCA = new TTreeReaderValue<Double_t>(*myReader, "DSTDOCA");
    TTreeReaderValue<Double_t> *DSTFD = new TTreeReaderValue<Double_t>(*myReader, "DSTFD");
    TTreeReaderValue<Double_t> *SLPCOSTHETA = new TTreeReaderValue<Double_t>(*myReader, "SLPCOSTHETA");
    TTreeReaderValue<Double_t> *SLPPT = new TTreeReaderValue<Double_t>(*myReader, "SLPPT");
    TTreeReaderValue<Double_t> *TRSUMPT = new TTreeReaderValue<Double_t>(*myReader, "TRSUMPT");

    std::map<std::string, TTreeReaderValue<Double_t>*> event_info;

    event_info["D0CHI2"] = D0CHI2;
    event_info["D0FD"] = D0FD;
    event_info["DSTDOCA"] = DSTDOCA;
    event_info["DSTFD"] = DSTFD;
    event_info["SLPCOSTHETA"] = SLPCOSTHETA;
    event_info["SLPPT"] = SLPPT;
    event_info["TRSUMPT"] = TRSUMPT;

    return event_info;
}

int main(int argc, char* argv[])
{
    auto bdt = BBDecTreeTool(1.4550000, "../Hlt2Dst2PiD02HHX_BDTParams_v2r0.txt");

    std::string fn = "../classifier_cols.root";
    TFile *f = TFile::Open(fn.c_str(), "READ");
    if (f == 0) {
        std::cout << "Failed to open: " << fn << std::endl;
        return 1;
    }
    f->ls();
    std::string tree_key = "example";
    TTree *tree = (TTree*) f->Get(tree_key.c_str());
    if (tree == 0) {
        std::cout << "Failed to read: " << tree_key << std::endl;
        return 1;
    }

    for (double threshold = 0.8; threshold < 1.5; threshold += 0.05) {
        TTreeReader myReader(tree);
        auto event_info = get_cols(&myReader);

        unsigned int n_accepted = 0;
        unsigned int n_rejected = 0;
        while (myReader.Next()) {
            Double_t response = bdt(event_info);
            if (response > threshold) {
                ++n_accepted;
            } else {
                ++n_rejected;
            }
       }

       std::cout << "With threshold > " << threshold << std::endl;
       std::cout << "   Accepted " << n_accepted << " out of " << n_accepted+n_rejected
                 << " (" << 100*n_accepted / (n_accepted+n_rejected) << "%)" << std::endl;
    }

    return 0;
}
