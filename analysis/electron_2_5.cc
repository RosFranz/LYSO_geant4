#include <TFile.h>
#include <TNtuple.h>
#include <TMath.h>
#include "TGraphErrors.h"
#include "TGraphAsymmErrors.h"
#include "TH1.h"
#include "TF1.h"
#include "TCanvas.h"
#include <cmath>
#include <cassert>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

void electron_2_5()
{
    gStyle->SetLineWidth(2);
    const int n_values = 7;
    const int n_bins = 240;
    const int n_quantiles = 3;

    double energies[n_values] = {240,120,90,60,50,40,30};
    double energies_err[n_values] = {0};

    //double quantiles_90[n_quantiles] = {0.1, 0.5, 0.9};
    double quantiles_68[n_quantiles] = {0.16, 0.5, 0.84};
    double pre_Ek_Qpos[n_quantiles];
    double Qpos_68[n_quantiles];
    //double Qpos_90[n_quantiles];
    
    double pre_Ek_means[n_values], pre_Ek_std_dev[n_values];

    double edep_means[n_values]; //media
    double edep_std_dev[n_values]; // deviazione standard della media

    double edep_median[n_values]; //mediana
    double edep_Qup[n_values], edep_Qdown[n_values]; // quantili up e down

    double edep_mode[n_values]; //moda
    double edep_deltaMode[n_values]; // differenza moda prima e dopo lo smoothing

    std::stringstream strRunID;
    TString n_run;

    for(int run_number=0; run_number<n_values; run_number++)
    {
        strRunID << run_number;
        n_run = strRunID.str();

        TFile *file = new TFile("../output_files/electron_2_5_run_"+n_run+".root");

        TTree *scoring = (TTree*)file->Get("Scoring");
        int entries = scoring->GetEntries();
        cout << "\n Run number = "+n_run+" " << endl;
        cout << "\tProton energy = " << energies[run_number] << " MeV" << endl;
        cout << "\tNumber of entries = " << entries << endl;

        // KINETIC ENERGY AT THE LYSO ENTRANCE

        //getting the mean values from the branches
        scoring->Draw("pre_Ek>>his1", "pre_Ek>0");
        TH1F *his1 = (TH1F*)gPad->GetPrimitive("his1");
        his1->GetQuantiles(n_quantiles, pre_Ek_Qpos, quantiles_68);

        pre_Ek_std_dev[run_number] = (pre_Ek_Qpos[2] - pre_Ek_Qpos[0])/2;
        pre_Ek_means[run_number] = pre_Ek_Qpos[1];

        cout << "\tProton kinetic energy before LYSO = " << pre_Ek_means[run_number] << "+-" << pre_Ek_std_dev[run_number] << endl;


        //ENERGY DEPOSITION IN LYSO

        scoring->Draw("fEdep>>his3", "fEdep>0");
        TH1F *his3 = (TH1F*)gPad->GetPrimitive("his3");
        
        his3->GetQuantiles(n_quantiles, Qpos_68, quantiles_68);
        edep_Qdown[run_number] = Qpos_68[1]-Qpos_68[0];
        edep_median[run_number] = Qpos_68[1];
        edep_Qup[run_number] = Qpos_68[2]-Qpos_68[1];

        edep_means[run_number] = his3->GetMean();
        edep_std_dev[run_number] = his3->GetStdDev()/sqrt(his3->GetEntries());

        edep_mode[run_number] = his3->GetBinCenter(his3->GetMaximumBin());
        his3->Rebin(2);
        edep_deltaMode[run_number] = abs(his3->GetBinCenter(his3->GetMaximumBin()) - edep_mode[run_number]) + his3->GetBinWidth(his3->GetMaximumBin());
        edep_mode[run_number] = his3->GetBinCenter(his3->GetMaximumBin());

        //Fitting...
        /*
        double Edep_max_pos = his3->GetBinCenter(his3->GetMaximumBin());
        his3->GetQuantiles(n_quantiles, Qpos_90, quantiles_90);
        
        TF1* fit = new TF1("fit", "abs([0])+abs(gaus(1))+[4]*x", 0, 300);
        fit->SetParameter(0, 1);
        fit->SetParameter(1, his3->GetBinContent(his3->GetMaximumBin()));
        fit->FixParameter(2, his3->GetBinCenter(his3->GetMaximumBin()));
        fit->SetParameter(3, his3->GetRMS());
        fit->SetParameter(4, 0);

        double x_left = (Qpos_68[1]-Qpos_68[0]<Qpos_68[2]-Qpos_68[1]) ? max(Qpos_90[0], Edep_max_pos - 1.*his3->GetRMS()) : Edep_max_pos - 1.5*his3->GetRMS();
        double x_right = (Qpos_68[1]-Qpos_68[0]>Qpos_68[2]-Qpos_68[1]) ? min(Qpos_90[2], Edep_max_pos + 1.*his3->GetRMS()) : Edep_max_pos + 1.5*his3->GetRMS();

        cout << "Run number = " << run_number << " " << x_left << " " << x_right << endl;
        
        his3->Fit("fit", "Q", "", x_left, x_right);
        fit->ReleaseParameter(2);

        his3->Fit("fit", "W", "", x_left, x_right);

        TCanvas *c5 = new TCanvas("c5","Mean energy deposition",600,600);
        his3->Draw();
        c5->SaveAs(Form("fit_plot/electron/fit%d.png",run_number));
        

        edep_means[run_number] = fit->GetMaximumX(x_left, x_right, 0.001);
        edep_std_dev[run_number] = abs(fit->GetMaximumX(x_left, x_right, 0.001)-his3->GetBinCenter(his3->GetMaximumBin()))/2;*/

        cout << "\tEnergy deposition in LYSO " << endl;
        cout << "\t mediana = " << edep_median[run_number] << "+" << edep_Qup[run_number] << "-" << edep_Qdown[run_number] << endl;
        cout << "\t media = " << edep_means[run_number] << "+-" << edep_std_dev[run_number] << endl;
        cout << "\t moda = " << edep_mode[run_number] << "+-" << edep_deltaMode[run_number] << endl;

        // resetting run id
        strRunID.str(std::string());

        delete file;
    }

    TCanvas *c4 = new TCanvas("c4","Mean energy deposition",600,600);

    TGraphAsymmErrors *g_edepMedian = new TGraphAsymmErrors(n_values,pre_Ek_means,edep_median,pre_Ek_std_dev,pre_Ek_std_dev, edep_Qdown, edep_Qup);
    g_edepMedian->SetTitle("Energy deposition (median);E_{K} before LYSO [MeV];E deposited in LYSO[MeV]");
    g_edepMedian->SetMarkerColor(kGreen);
    g_edepMedian->SetLineColor(kGreen);
    g_edepMedian->SetMarkerStyle(21);
    g_edepMedian->Draw("AP");

    TGraphErrors *g_edepMean = new TGraphErrors(n_values,pre_Ek_means,edep_means,pre_Ek_std_dev,edep_std_dev);
    g_edepMean->SetTitle("Energy deposition (mean);E_{K} before LYSO [MeV];E deposited in LYSO[MeV]");
    g_edepMean->SetMarkerColor(kBlue);
    g_edepMean->SetLineColor(kBlue);
    g_edepMean->SetMarkerStyle(21);
    g_edepMean->Draw("P");

    TGraphErrors *g_edepMode = new TGraphErrors(n_values,pre_Ek_means,edep_mode,pre_Ek_std_dev,edep_deltaMode);
    g_edepMode->SetTitle("Energy deposition (mode);E_{K} before LYSO [MeV];E deposited in LYSO[MeV]");
    g_edepMode->SetMarkerColor(kRed);
    g_edepMode->SetLineColor(kRed);
    g_edepMode->SetMarkerStyle(21);
    g_edepMode->Draw("P");

    c4->BuildLegend();

    

    /*TCanvas *c5 = new TCanvas("c5","Mean distance in LYSO",600,600);
    TGraphErrors *g_track_len = new TGraphErrors(n_values,pre_Ek_means,track_len_means,pre_Ek_std_dev,track_len_std_dev);
    g_track_len->SetTitle("Mean distance inside the LYSO;E_{K} before LYSO [MeV]; distance in LYSO [mm]");
    g_track_len->SetMarkerColor(4);
    g_track_len->SetMarkerStyle(21);
    g_track_len->Draw("AP");*/

    TFile *output = new TFile("./electron_2_5.root","RECREATE");
    g_edepMean->Write();
    g_edepMode->Write();
    g_edepMedian->Write();
    //g_track_len->Write();
    c4->Write();
    //c5->Write();
    output->Close();
}