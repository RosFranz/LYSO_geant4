#include "detector.hh"

MySensitiveDetector::MySensitiveDetector(G4String name) : G4VSensitiveDetector(name)
{}

MySensitiveDetector::~MySensitiveDetector()
{}

G4bool MySensitiveDetector::ProcessHits(G4Step *aStep, G4TouchableHistory *ROhist)
{
    //G4Track *track = aStep->GetTrack();
    //track->SetTrackStatus(fStopAndKill);
    
    //probabily is working just for photons    
    /*G4StepPoint *preStepPoint = aStep->GetPreStepPoint();
    G4StepPoint *postStepPoint = aStep->GetPostStepPoint();

    G4ThreeVector posPhoton = preStepPoint->GetPosition();
    G4cout << "Photon position: " << posPhoton << G4endl;*/

    /*G4int evt = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
    G4cout << "Current event: " << evt << G4endl;

    G4AnalysisManager *man = G4AnalysisManager::Instance();

    //Hits
    man->FillNtupleIColumn(0,0,evt);
    man->AddNtupleRow(0);
    
    //Protons
    man->FillNtupleIColumn(1,0,evt);
    man->AddNtupleRow(0);*/

    return true;
}