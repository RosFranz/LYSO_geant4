#include "run.hh"

MyRunAction::MyRunAction()
{
    G4AnalysisManager *man = G4AnalysisManager::Instance();

    // creation of the tree
    man->CreateNtuple("Hits","Hits");
    man->CreateNtupleIColumn("Event");
    man->FinishNtuple(0);

    man->CreateNtuple("Protons","Protons");
    man->CreateNtupleIColumn("Event");
    man->FinishNtuple(1);

    man->CreateNtuple("Scoring","Scoring");
    man->CreateNtupleDColumn("pre_Ek");
    man->CreateNtupleDColumn("post_Ek");
    man->CreateNtupleDColumn("fEdep");
    man->CreateNtupleDColumn("fTrackLength");
    man->FinishNtuple(2);

    // wighted histogram Ek before LYSO and deposited energy as weigth
    // the problem is... which are the extremes of the histo?
    // check th dfferent constructor of the histograms in G4...
    //man->CreateH1("eDep", "Energy deposition")
}

MyRunAction::~MyRunAction()
{}

void MyRunAction::BeginOfRunAction (const G4Run*run)
{
    //opens an output file for every run!
    G4AnalysisManager *man = G4AnalysisManager::Instance();
    man->SetDefaultFileType("root");
    man->SetVerboseLevel(1);

    G4int runID = run->GetRunID();
    std::stringstream strRunID;
    strRunID << runID;

    //general use
    man->OpenFile("../output_files/output_run_"+strRunID.str()+".root");

    //proton_2_5_Al_1mm.mac
    //man->OpenFile("../output_files/proton_2_5_Al_1mm_run_"+strRunID.str()+".root");

    //proton_5.mac
    //man->OpenFile("../output_files/proton_5_run_"+strRunID.str()+".root");

    //proton_15.mac
    //man->OpenFile("../output_files/proton_15_run_"+strRunID.str()+".root");

    //carbon_2_5.mac
    //man->OpenFile("../output_files/carbon_2_5_run_"+strRunID.str()+".root");

    //proton_2_5.mac
    //man->OpenFile("../output_files/proton_2_5_run_"+strRunID.str()+".root");

    //proton_5_CNAO.mac
    //man->OpenFile("../output_files/proton_5_CNAO_run_"+strRunID.str()+".root");

    //electron beam
    //man->OpenFile("../output_files/electron_2_5_run_"+strRunID.str()+".root");
}

void MyRunAction::EndOfRunAction(const G4Run*run)
{
    G4AnalysisManager *man = G4AnalysisManager::Instance();
    man->Write();
    man->CloseFile();

    G4cout << "\nEND OF RUN NUMBER " << run->GetRunID() << "\n" << G4endl;
}