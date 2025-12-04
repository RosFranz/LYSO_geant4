#include <iostream>

#include "G4RunManager.hh"
//#include "G4MTRunManager.hh"
#include "G4UImanager.hh"
#include "G4VisManager.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

#include "construction.hh"
#include "physics.hh"
#include "action.hh"

#include "FTFP_BERT.hh"

int main(int argc, char** argv){

    //MULTITHREADING COMPATIBILITY
    /*#ifdef G4MULTITHREADED
        G4MTRunManager *runManager = new G4MTRunManager();
    #else
        G4RunManager *runManager = new G4RunManager();
    #endif*/

    G4VModularPhysicsList *physicsList = new FTFP_BERT();
    G4RunManager *runManager = new G4RunManager();
    runManager->SetUserInitialization(new MyDetectorConstruction());
    runManager->SetUserInitialization(physicsList);
    runManager->SetUserInitialization(new MyActionInitialization());

    runManager->Initialize();

    G4UIExecutive *ui = 0;
    if(argc == 1) ui = new G4UIExecutive(argc,argv); // show the graphics only if no argument is given

    G4VisManager *visManager = new G4VisExecutive();
    visManager->Initialize();

    G4UImanager *UImanager = G4UImanager::GetUIpointer();

    if(ui){
        UImanager->ApplyCommand("/control/execute vis.mac");
        ui->SessionStart();
        delete ui;
    }
    else{
        G4String command = "/control/execute ";
        G4String fileName = argv[1];
        UImanager->ApplyCommand(command+fileName);
    }

    delete visManager;
    delete runManager;

    return 0;
}