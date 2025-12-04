#ifndef EVENT_HH
#define EVENT_HH

#include <G4UserEventAction.hh>
#include <globals.hh>

#include "G4UserEventAction.hh"
#include "G4Event.hh"
#include "G4AnalysisManager.hh"

#include "G4SDManager.hh"
#include "G4THitsMap.hh"

#include "run.hh"

class MyEventAction : public G4UserEventAction
{
    public:
        MyEventAction(MyRunAction*);
        ~MyEventAction();

        virtual void BeginOfEventAction(const G4Event*);
        virtual void EndOfEventAction(const G4Event* event);

        void AddEdep(G4double edep) {fEdep += edep;}
        void AddTrackLen(G4double tracklen) {fTrackLen += tracklen;}

    private:
        G4double fEdep, fTrackLen;
        G4int fLysoID {-1};
};


#endif