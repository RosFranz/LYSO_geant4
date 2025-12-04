#include "event.hh"

MyEventAction::MyEventAction(MyRunAction*)
{
    fEdep = 0.;
}

MyEventAction::~MyEventAction()
{}

void MyEventAction::BeginOfEventAction(const G4Event*)
{
    fEdep = 0.;
    fTrackLen = 0;
}

void MyEventAction::EndOfEventAction(const G4Event*event)
{
    G4SDManager* sdm = G4SDManager::GetSDMpointer();
    G4AnalysisManager *man = G4AnalysisManager::Instance();

    // Get the hit collection of the event
    G4HCofThisEvent* hcofEvent = event->GetHCofThisEvent();
    // nothing to be done without the hit collection
    if(hcofEvent==nullptr) return;

    //We just have one histogram
    //G4int histogramID = 1;

    // Getting the Collection ID during the first event
        // fLysoID is initialized at -1.
    if(fLysoID<0){
        fLysoID = sdm->GetCollectionID("lysoDetector/LysoEdep");
        G4cout << "EventAction: LYSO energy scorer ID: " << sdm->GetCollectionID("lysoDetector/LysoEdep") << G4endl;
    }
    if(fLysoID!=-1) {
        G4THitsMap<G4double>* hitMapLyso = nullptr;
        hitMapLyso = dynamic_cast<G4THitsMap<G4double>*>(hcofEvent->GetHC(fLysoID));
        if(hitMapLyso){
            for(auto pair : *(hitMapLyso->GetMap())){
                G4double energyDep = *(pair.second);
                //filling the wighted histogram
                //man->FillH1(histogramID, fSteppingAction->GetPreLYSOEk(), energyDep);
                //filling the tree
                man->FillNtupleDColumn(2,2,energyDep);
            }
        }
    }

    //G4cout << "Energy deposition: " << fEdep << G4endl;

    //man->FillNtupleDColumn(2,2,fEdep);
    man->FillNtupleDColumn(2,3, fTrackLen);
    man->AddNtupleRow(2);
}