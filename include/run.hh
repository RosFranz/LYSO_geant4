#ifndef RUN_HH
#define RUN_HH

#include "G4UserRunAction.hh"
#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4AnalysisManager.hh"

#include "globals.hh"

class MyRunAction : public G4UserRunAction
{
    public:
        MyRunAction();
        ~MyRunAction();

        virtual void BeginOfRunAction(const G4Run*run);
        virtual void EndOfRunAction(const G4Run*run);
};


#endif