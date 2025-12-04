#ifndef STEPPING_HH
#define STEPPING_HH

#include "G4UserSteppingAction.hh"
#include "G4Step.hh"
#include "G4SystemOfUnits.hh"

#include "construction.hh"
#include "event.hh"

class MySteppingAction : public G4UserSteppingAction
{
    public:
        MySteppingAction(MyEventAction*eventAction);
        ~MySteppingAction();

        virtual void UserSteppingAction(const G4Step*);

        G4double GetPreLYSOEk() const { return fpreLYSOEk; }
        G4double GetPostLYSOEk() const { return fpostLYSOEk; }

    private:
        MyEventAction *fEventAction;
        G4double fpreLYSOEk = 0;
        G4double fpostLYSOEk = 0;

};

#endif