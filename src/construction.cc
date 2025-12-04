#include "construction.hh"

MyDetectorConstruction::MyDetectorConstruction()
{
    //example of messenger definitions
    fMessenger = new G4GenericMessenger(this, "/detector/", "Detector Construction");
    fMessenger->DeclareProperty("nCols", nCols, "Number of columns");
    fMessenger->DeclareProperty("nRows", nRows, "Number of rows");

    nCols = 100;
    nRows = 100;

    DefineMaterial();
}

MyDetectorConstruction::~MyDetectorConstruction()
{}

void MyDetectorConstruction::DefineMaterial()
{
    //material definitions
    G4NistManager *nist = G4NistManager::Instance();

    // single elements
    G4double z, a;
    G4String name, symbol;
    a = 1.0080*g/mole;
    G4Element *elH = new G4Element(name="Hydrogen", symbol="H", z=1., a);

    a = 12.011*g/mole;
    G4Element *elC = new G4Element(name="Carbon", symbol="C", z=6, a);

    a = 14.007*g/mole;
    G4Element *elN = new G4Element(name="Nitrogen", symbol="N", z=7., a);

    a = 15.999*g/mole;
    G4Element *elO = new G4Element(name="Oxygen", symbol="O", z=8., a);

    a = 28.085*g/mole;
    G4Element *elSi = new G4Element(name="Silicon", symbol="Si", z=14., a);

    a = 174.97*g/mole;
    G4Element *elLu = new G4Element(name="Lutetium", symbol="Lu", z=71., a);
    
    a = 88.906*g/mole;
    G4Element *elY = new G4Element(name="Yttrium", symbol="Y", z=39., a);
    
    a = 140.12*g/mole;
    G4Element *elCe = new G4Element(name="Cerium", symbol="Ce", z=58., a);

    //world
    worldMat = nist->FindOrBuildMaterial("G4_AIR"); //world material
        //refraction index
    G4double ph_energy[2] = {1.239841939*eV/0.2,1.239841939*eV/0.9}; // energy of photons from the wavelength in micro-meter
    G4double rindexAir[2] = {1.0,1.0}; // assumed constant
    G4MaterialPropertiesTable *mptAir= new G4MaterialPropertiesTable();
    mptAir->AddProperty("RINDEX", ph_energy, rindexAir, 2);
    worldMat->SetMaterialPropertiesTable(mptAir);

    // normal plastic scintillator (density = 1.032 vs real density = 1.023 g/cc)
    fEJ200Material  = nist->FindOrBuildMaterial("G4_PLASTIC_SC_VINYLTOLUENE");
        // ottical properties
    G4double rindexEj200[2] = {1.58,1.58}; 
    G4MaterialPropertiesTable *mptEj200 = new G4MaterialPropertiesTable();
    mptEj200->AddProperty("RINDEX", ph_energy, rindexEj200, 2);// refrection index assumed constant
    fEJ200Material->SetMaterialPropertiesTable(mptEj200);

    Aluminium = nist->FindOrBuildMaterial("G4_Al");

    // pure silicon
    fAltaiMaterial = nist->FindOrBuildMaterial("G4_Si");

    // kapton
    KaptonMat = new G4Material("Kapton", 1.42*g/cm3, 4, kStateSolid);
    KaptonMat->AddElement(elH, 0.03);
    KaptonMat->AddElement(elC, 0.69);
    KaptonMat->AddElement(elN, 0.07);
    KaptonMat->AddElement(elO, 0.21);

    // copper 
    CopperMat = nist->FindOrBuildMaterial("G4_Cu");

    
    //Epoxy
    EpoxyMat = new G4Material("Epoxy", 1.17*g/cm3, 3, kStateSolid);
    EpoxyMat->AddElement(elH, 24);
    EpoxyMat->AddElement(elC, 21);
    EpoxyMat->AddElement(elO, 4);

    //AralditeHardener
    ArlditeHardenerMat = new G4Material("AraldiHardener", 0.98*g/cm3, 3, kStateSolid);
    ArlditeHardenerMat->AddElement(elH, 18);
    ArlditeHardenerMat->AddElement(elC, 6);
    ArlditeHardenerMat->AddElement(elN, 4);

    // araldite
    AralditeMat = new G4Material("Araldite", 1.05*g/cm3, 2, kStateSolid);
    AralditeMat->AddMaterial(EpoxyMat, 0.556);
    AralditeMat->AddMaterial(ArlditeHardenerMat, 0.444);

        

    //LYSO material
    matLYSOOpt = new G4Material("LYSOOpt", 7.1*g/cm3, 5, kStateSolid);
    matLYSOOpt->AddElement(elLu, 71.43*0.01);
    matLYSOOpt->AddElement(elY, 4.03*0.01);
    matLYSOOpt->AddElement(elSi, 6.37*0.01);
    matLYSOOpt->AddElement(elO, 18.14*0.01);
    matLYSOOpt->AddElement(elCe, 0.02*0.01);

        //LYSO properties
        G4double rindexLyso[2] = {1.81,1.81}; // refrection index assumed constant
        G4MaterialPropertiesTable *mptLyso = new G4MaterialPropertiesTable();
        mptLyso->AddProperty("RINDEX", ph_energy, rindexLyso, 2);
        matLYSOOpt->SetMaterialPropertiesTable(mptLyso);

    // Print materials
    G4cout << *(G4Material::GetMaterialTable()) << G4endl;

}

G4VPhysicalVolume *MyDetectorConstruction::Construct()
{
    // boolean variables
    G4bool AluminiumCheck = false;
    G4bool LysoWidth_2_5 = true;
    G4bool LysoWidth_5 = false;
    G4bool LysoWidth_15 = false;

    //distances definitions
    G4double beam_ej200 = 125*cm; // beam distance from EJ200
    G4double ej200_Al = 5*cm; // EJ200 distance from AL
    G4double Al_altai = 1*cm; // Al distance from ALTAI
    G4double ej200_altai = ej200_Al + Al_altai; // EJ200 distance from ALTAI
    G4double altai_lyso = 7.5*cm; // ALTAI distance from LYSO

    //dimensions definitions
    G4double ej200Width = 0.5*cm; // EJ200 dimensions e non 0.5
    G4double ej200Length = 5*cm;
    G4double ej200Height = 15.0*cm;

    G4double AlWidth = 0.2*cm;
    G4double AlLength = 5*cm;
    G4double AlHeight = 15.0*cm;

    G4double altaiWidth = 0.015*cm; // ALTAI dimensions 50 micron * 3 (sensible + substrato)
    G4double altaiLength = 5.0*cm;
    G4double altaiHeight = 15.0*cm;

    G4double kaptonWidth1 = 0.0025*cm; // width of the silicon support (same lenght and height of ALTAI)
    G4double copperWidth = 0.0018*cm;
    G4double kaptonWidth2 = 0.0075*cm;
    G4double aralditeWidth = 0.0225*cm;

    // LYSO dimensions
    G4double lysoWidth; 
    G4double lysoLength; 
    G4double lysoHeight;

    if(LysoWidth_2_5){
        lysoWidth = (2.5)*cm;
        lysoLength = 5.0*cm;
        lysoHeight = 15.0*cm;
    }
    if(LysoWidth_5){
        lysoWidth = 5.0*cm;
        lysoLength = 2.5*cm;
        lysoHeight = 15.0*cm;
    }
    if(LysoWidth_15){
        lysoWidth = 15.0*cm;
        lysoLength = 2.5*cm;
        lysoHeight = 5.0*cm;
    }
    
   

    //altai position
    G4double x_altai;

    G4double worldLength = (beam_ej200+Al_altai+altai_lyso) + (ej200Width+altaiWidth+lysoWidth)+30.0*cm;

    // Definitions of Solids, Logical Volumes, Physical Volume

    // World
    G4GeometryManager::GetInstance()->SetWorldMaximumExtent(worldLength);

    G4cout << "Computed tolerance = " << G4GeometryTolerance::GetInstance()->GetSurfaceTolerance()/mm  << " mm" << G4endl;

    worldS = new G4Box("world", worldLength/2.,20.0*cm,20.0*cm);
    worldLV = new G4LogicalVolume( worldS, worldMat, "World");
    worldPV  = new G4PVPlacement(0, G4ThreeVector(), worldLV, "World", 0, false, 0, true); //  Must place the World Physical volume unrotated at (0,0,0).

    // EJ200
    G4double x_ej200 = beam_ej200 - worldLength/2 + ej200Width/2;
    G4ThreeVector position_ej200 = G4ThreeVector(x_ej200,0.,0.);

    ej200S = new G4Box("ej200", ej200Width/2, ej200Length/2, ej200Height/2);
    ej200LV = new G4LogicalVolume(ej200S, fEJ200Material,"EJ200LV");
    ej200PV = new G4PVPlacement(0, position_ej200, ej200LV, "EJ200PV", worldLV, false, 0, true);
    G4cout << "EJ200 is " << ej200Width/cm << " cm of " << fEJ200Material->GetName() << G4endl;

    //Alluminio
    if(AluminiumCheck){
        G4double x_Al = x_ej200 + ej200Width/2 + AlWidth/2 + ej200_Al;
        G4ThreeVector position_Al = G4ThreeVector(x_Al,0.,0.);

        AlS = new G4Box("Alum", AlWidth/2, AlLength/2, AlHeight/2);
        AlLV = new G4LogicalVolume(AlS, Aluminium,"AlLV");
        AlPV = new G4PVPlacement(0, position_Al, AlLV, "AlPV", worldLV, false, 0, true);
        G4cout << "Aluminium is " << AlWidth/cm << " cm of " << Aluminium->GetName() << G4endl;

        G4VisAttributes* AlVisAtt= new G4VisAttributes(G4Colour(1.0,1.0,0.0));

        AlLV->SetVisAttributes(AlVisAtt);

        //positioning of the altai detector
        x_altai = x_Al + AlWidth/2 + Al_altai + altaiWidth/2;
    }
    //positioning of the altai detector
    else x_altai = x_ej200 + ej200Width/2 + ej200_altai + altaiWidth/2;

    //ALTAI
    G4ThreeVector position_altai = G4ThreeVector(x_altai,0.,0.);

    altaiS = new G4Box("altai", altaiWidth/2, altaiLength/2, altaiHeight/2);
    altaiLV = new G4LogicalVolume(altaiS, fAltaiMaterial,"ALTAILV");
    altaiPV = new G4PVPlacement(0, position_altai, altaiLV, "ALTAIPV", worldLV, false, 0, true);
    G4cout << "ALTAI is " << altaiWidth/cm << " cm of " << fAltaiMaterial->GetName() << G4endl;

    // SUPPORT OF THE SILICON
    G4double x_kapton1 = x_altai + altaiWidth/2 + kaptonWidth1/2;       // kapton 25 micron
    G4ThreeVector position_kapton1 = G4ThreeVector(x_kapton1,0.,0.);
    kaptonS1 = new G4Box("kapton1", kaptonWidth1/2, altaiLength/2, altaiHeight/2);
    kaptonLV1 = new G4LogicalVolume(kaptonS1,KaptonMat,"KAPTONLV1");
    kaptonPV1 = new G4PVPlacement(0, position_kapton1, kaptonLV1, "KAPTONPV1", worldLV, false, 0, true);

    G4double x_copper1 = x_kapton1 + kaptonWidth1/2 + copperWidth/2;    // copper 18 micron
    G4ThreeVector position_copper1 = G4ThreeVector(x_copper1,0.,0.);
    copperS1 = new G4Box("copper1", copperWidth/2, altaiLength/2, altaiHeight/2);
    copperLV1 = new G4LogicalVolume(copperS1,CopperMat,"COPPERLV1");
    copperPV1 = new G4PVPlacement(0, position_copper1, copperLV1, "COPPERPV1", worldLV, false, 0, true);

    G4double x_kapton2 = x_copper1 + copperWidth/2 + kaptonWidth2/2;    // kapton 75 micron
    G4ThreeVector position_kapton2 = G4ThreeVector(x_kapton2,0.,0.);
    kaptonS2 = new G4Box("kapton2", kaptonWidth2/2, altaiLength/2, altaiHeight/2);
    kaptonLV2 = new G4LogicalVolume(kaptonS2,KaptonMat,"KAPTONLV2");
    kaptonPV2 = new G4PVPlacement(0, position_kapton2, kaptonLV2, "KAPTONPV2", worldLV, false, 0, true);

    G4double x_copper2 = x_kapton2 + kaptonWidth2/2 + copperWidth/2;    // rame 18 micron
    G4ThreeVector position_copper2 = G4ThreeVector(x_copper2,0.,0.);
    copperS2 = new G4Box("copper2", copperWidth/2, altaiLength/2, altaiHeight/2);
    copperLV2 = new G4LogicalVolume(copperS2,CopperMat,"COPPERLV2");
    copperPV2 = new G4PVPlacement(0, position_copper2, copperLV2, "COPPERPV2", worldLV, false, 0, true);

    G4double x_kapton3 = x_copper2 + copperWidth/2 + kaptonWidth1/2;    //kapton 25 micron
    G4ThreeVector position_kapton3 = G4ThreeVector(x_kapton3,0.,0.);
    kaptonS3 = new G4Box("kapton3", kaptonWidth1/2, altaiLength/2, altaiHeight/2);
    kaptonLV3 = new G4LogicalVolume(kaptonS3,KaptonMat,"KAPTONLV3");
    kaptonPV3 = new G4PVPlacement(0, position_kapton3, kaptonLV3, "KAPTONPV3", worldLV, false, 0, true);

    G4double x_araldite = x_kapton3 + kaptonWidth1/2 + aralditeWidth/2; // araldite 25 + 200 micron 
    G4ThreeVector position_araldite = G4ThreeVector(x_araldite,0.,0.);
    aralditeS = new G4Box("araldite", aralditeWidth/2, altaiLength/2, altaiHeight/2);
    aralditeLV = new G4LogicalVolume(aralditeS,AralditeMat,"aralditeLV");
    aralditePV = new G4PVPlacement(0, position_araldite, aralditeLV, "ARALDITEPV", worldLV, false, 0, true);

    //LYSO
    G4double x_lyso = x_altai + altaiWidth/2 + 7.5*cm +lysoWidth/2;
    G4ThreeVector position_lyso = G4ThreeVector(x_lyso,0.,0.);

    lysoS = new G4Box("lyso",lysoWidth/2., lysoLength/2, lysoHeight/2);
    lysoLV = new G4LogicalVolume(lysoS, matLYSOOpt,"LYSOLogic");
    lysoPV = new G4PVPlacement(0, position_lyso, lysoLV, "LYSOPV", worldLV, false, 0, true);
    G4cout << "LYSO is " << lysoWidth/cm << " cm of " << matLYSOOpt->GetName() << G4endl;

    // definition of the LYSO as scoring volume
    fScoringVolume = lysoLV;

    // Visualization attributes
    G4VisAttributes* boxVisAtt= new G4VisAttributes(G4Colour(1.0,1.0,1.0));
    G4VisAttributes* ej200VisAtt= new G4VisAttributes(G4Colour(1.0,0.0,0.0));
    G4VisAttributes* altaiVisAtt= new G4VisAttributes(G4Colour(0.0,1.0,0.0));
    G4VisAttributes* kaptonVisAtt = new G4VisAttributes(G4Colour(1.0, 1.0, 0.0));
    G4VisAttributes* copperVisAtt = new G4VisAttributes(G4Colour(1.0, 0.0, 1.0));
    G4VisAttributes* aralditeVisAtt = new G4VisAttributes(G4Colour(0.0, 1.0, 1.0));
    G4VisAttributes* lysoVisAtt= new G4VisAttributes(G4Colour(0.0,0.0,1.0));

    worldLV->SetVisAttributes(boxVisAtt);
    ej200LV->SetVisAttributes(ej200VisAtt);
    altaiLV->SetVisAttributes(altaiVisAtt);
    kaptonLV1->SetVisAttributes(kaptonVisAtt);
    kaptonLV2->SetVisAttributes(kaptonVisAtt);
    kaptonLV3->SetVisAttributes(kaptonVisAtt);
    copperLV1->SetVisAttributes(copperVisAtt);
    copperLV2->SetVisAttributes(copperVisAtt);
    aralditeLV->SetVisAttributes(aralditeVisAtt);
    lysoLV->SetVisAttributes(lysoVisAtt);

    // Always return the physical world
    return worldPV;

}

void MyDetectorConstruction::ConstructSDandField()
{
    G4SDManager* sdManager = G4SDManager::GetSDMpointer();
    sdManager->SetVerboseLevel(2);

    //creating a multifunctional detector for the LYSO
    G4MultiFunctionalDetector* lysoDetector = new G4MultiFunctionalDetector("lysoDetector");

    //primitive scorer for the energy release inside LYSO
    G4VPrimitiveScorer *lysoEdepScorer = new G4PSEnergyDeposit("LysoEdep");
    lysoDetector->RegisterPrimitive(lysoEdepScorer);

    // setting the name of the LYSO logical volume as a sensitive detctor
    SetSensitiveDetector("LYSOLogic",lysoDetector);

    sdManager->AddNewDetector(lysoDetector);

    // maybe is working for photons only
    /*MySensitiveDetector *sensDet = new MySensitiveDetector("LysoSensitive");
    lysoLV->SetSensitiveDetector(sensDet);*/
}