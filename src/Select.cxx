#include "Select.h"

Select::Select() {}

Select::~Select() {}

Int_t Select::ValidHits(const string& file, const string& tree)
{
    DisableImplicitMT();
    RDataFrame* dm = new RDataFrame(tree, file);
    string outname = file;
    outname = outname.substr(outname.find_last_of('/') + 1);
    outname = "sel_" + outname;
    auto fout = dm->Define("nhits", "(Int_t) Hit_X.size()")
    /*
    .Define("Hit_Time_nonzero", [] (vector<Double_t> Hit_Time, vector<Double_t> Hit_Energy, Int_t nhits)
    {
        vector<Double_t> Hit_Time_nonzero;
        for (Int_t i = 0; i < nhits; i++)
            if (Hit_Energy.at(i) > 0)
                Hit_Time_nonzero.emplace_back(Hit_Time.at(i));
        return Hit_Time_nonzero;
    }, {"Hit_Time", "Hit_Energy", "nhits"})
    */
    .Define("Hit_X_nonzero", [] (vector<Double_t> Hit_X, vector<Double_t> Hit_Energy, Int_t nhits)
    {
        vector<Double_t> Hit_X_nonzero;
        for (Int_t i = 0; i < nhits; i++)
            if (Hit_Energy.at(i) > 0)
                Hit_X_nonzero.emplace_back(Hit_X.at(i));
        return Hit_X_nonzero;
    }, {"Hit_X", "Hit_Energy", "nhits"})
    .Define("Hit_Y_nonzero", [] (vector<Double_t> Hit_Y, vector<Double_t> Hit_Energy, Int_t nhits)
    {
        vector<Double_t> Hit_Y_nonzero;
        for (Int_t i = 0; i < nhits; i++)
            if (Hit_Energy.at(i) > 0)
                Hit_Y_nonzero.emplace_back(Hit_Y.at(i));
        return Hit_Y_nonzero;
    }, {"Hit_Y", "Hit_Energy", "nhits"})
    .Define("Hit_Z_nonzero", [] (vector<Double_t> Hit_Z, vector<Double_t> Hit_Energy, Int_t nhits)
    {
        vector<Double_t> Hit_Z_nonzero;
        for (Int_t i = 0; i < nhits; i++)
            if (Hit_Energy.at(i) > 0)
                Hit_Z_nonzero.emplace_back(Hit_Z.at(i));
        return Hit_Z_nonzero;
    }, {"Hit_Z", "Hit_Energy", "nhits"})
    .Define("Hit_Energy_nonzero", [] (vector<Double_t> Hit_Energy, Int_t nhits)
    {
        vector<Double_t> Hit_Energy_nonzero;
        for (Int_t i = 0; i < nhits; i++)
            if (Hit_Energy.at(i) > 0)
                Hit_Energy_nonzero.emplace_back(Hit_Energy.at(i));
        return Hit_Energy_nonzero;
    }, {"Hit_Energy", "nhits"})
    .Snapshot(tree, outname);
    delete dm;

    TFile* f = new TFile((TString) outname, "READ");
    TTree* t = f->Get<TTree>((TString) tree);
    t->SetBranchStatus("*", 1);
    vector<TString> deactivate = { "Hit_Energy", /*"Hit_Time",*/ "Hit_X", "Hit_Y", "Hit_Z", "nhits" };
    for (TString de : deactivate)
        t->SetBranchStatus(de, 0);
    TFile* fnew = new TFile((TString) outname, "RECREATE");
    TTree* tnew = t->CloneTree();
    tnew->Write(0, TObject::kWriteDelete, 0);
    f->Close();
    fnew->Close();

    return 1;
}
