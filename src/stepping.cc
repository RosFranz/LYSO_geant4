#include "stepping.hh"


MySteppingAction::MySteppingAction(MyEventAction *eventAction)
{
    fEventAction = eventAction;
}
MySteppingAction::~MySteppingAction()
{}

void MySteppingAction::UserSteppingAction(const G4Step *step)
{
    // the particle is going through this volume
    G4LogicalVolume *volume = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume();

    // scoring volume defined in the construction
    const MyDetectorConstruction *detectorConstruction = static_cast<const MyDetectorConstruction*> (G4RunManager::GetRunManager()->GetUserDetectorConstruction());
    G4LogicalVolume *fScoringVolume = detectorConstruction->GetScoringVolume();

    G4String particleName = "proton";
    // informations are stored only if the particle is in the lyso and it is a primary proton
    if(volume == fScoringVolume && step->GetTotalEnergyDeposit()!=0)
        
        /*step->GetTrack()->GetParticleDefinition()->GetParticleName() == particleName 
        && volume == fScoringVolume
        && step->GetTrack()->GetTrackID()==1
        && step->GetTotalEnergyDeposit()!=0)*/ 
    {
        //analysis manager
        G4AnalysisManager *man = G4AnalysisManager::Instance();

        // kinetic energy of the proton before the lyso
        if(step->IsFirstStepInVolume()==true && step->GetTrack()->GetTrackID()==1){
            //man->FillNtupleDColumn(2,0,step->GetPreStepPoint()->GetKineticEnergy());
            fpreLYSOEk = step->GetPreStepPoint()->GetKineticEnergy();
            man->FillNtupleDColumn(2,0,fpreLYSOEk);
        }

        // kinetic energy of the proton after the lyso
        if(step->IsLastStepInVolume()==true && step->GetTrack()->GetTrackID()==1){
            //man->FillNtupleDColumn(2,1,step->GetPostStepPoint()->GetKineticEnergy());
            fpostLYSOEk = step->GetPostStepPoint()->GetKineticEnergy();
            man->FillNtupleDColumn(2,1,fpostLYSOEk);
        }

        G4double edep = step->GetTotalEnergyDeposit();
        G4double tracklen = step->GetStepLength()*mm;
        fEventAction->AddTrackLen(tracklen);
        fEventAction->AddEdep(edep);
    }
}