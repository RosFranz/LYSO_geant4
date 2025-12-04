#ifndef CONSTRUCTION_HH
#define CONSTRUCTION_HH

#include "globals.hh"
#include "tls.hh"
#include "G4VUserDetectorConstruction.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4PVPlacement.hh"

#include "G4GeometryTolerance.hh"
#include "G4GeometryManager.hh"

#include "G4SDManager.hh"
#include "G4MultiFunctionalDetector.hh"
#include "G4VPrimitiveScorer.hh"
#include "G4PSEnergyDeposit.hh"

#include "G4NistManager.hh"
#include "G4Material.hh"
#include "G4OpticalParameters.hh"

#include "G4SystemOfUnits.hh"
#include "G4GenericMessenger.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "detector.hh"

class MyDetectorConstruction : public G4VUserDetectorConstruction
{
    public:
        MyDetectorConstruction();
        ~MyDetectorConstruction();

        G4LogicalVolume *GetScoringVolume() const {return fScoringVolume;}

        virtual G4VPhysicalVolume *Construct() override;

        void ConstructSDandField() override; //sensitive detectors

    private:
        //example of messenger definition
        G4int nCols, nRows;
        G4GenericMessenger *fMessenger;

        //solids
        G4Box *worldS, *ej200S, *AlS, *altaiS, *lysoS;
        G4Box *kaptonS1, *copperS1, *kaptonS2, *copperS2, *kaptonS3, *aralditeS; 

        //logical volumes
        G4LogicalVolume *worldLV, *ej200LV, *AlLV, *altaiLV, *lysoLV;
        G4LogicalVolume *kaptonLV1, *copperLV1, *kaptonLV2, *copperLV2, *kaptonLV3, *aralditeLV;
            //scoring
        G4LogicalVolume *fScoringVolume;

        //physical volumes
        G4VPhysicalVolume *worldPV, *ej200PV, *AlPV, *altaiPV, *lysoPV;
        G4VPhysicalVolume *kaptonPV1, *copperPV1, *kaptonPV2, *copperPV2, *kaptonPV3, *aralditePV;

        //materials
        G4Material *worldMat, *fEJ200Material, *Aluminium, *fAltaiMaterial, *matLYSOOpt;
        G4Material *KaptonMat, *AralditeMat, *CopperMat;
        G4Material *EpoxyMat, *ArlditeHardenerMat;
        //defines the materials
        void DefineMaterial();
};

#endif