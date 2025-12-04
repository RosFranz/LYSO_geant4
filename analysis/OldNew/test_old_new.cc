#include <TFile.h>
#include <TNtuple.h>
#include <TMath.h>
#include "TGraphErrors.h"
#include "TH1.h"
#include "TCanvas.h"
#include <cmath>
#include <cassert>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

void test_old_new()
{
    
    TFile *file = new TFile("beam_test1_OLD.root");
    TGraphErrors *old = (TGraphErrors*)file->Get("g_edep");

    //destructing file, tree and histograms
    delete file;

    TFile *filen = new TFile("beam_test1_NEW.root");
    TGraphErrors *newg = (TGraphErrors*)filen->Get("g_edep");


    //plotting the result

    TCanvas *c4 = new TCanvas("c4","Mean energy deposition",600,600);
    //old->SetTitle("Mean energy deposition;E_{K} before LYSO [MeV];E deposited in LYSO[MeV]");
    old->SetMarkerColor(6);
    old->SetMarkerStyle(21);
    old->Draw("APL");
    newg->Draw("PL");


}