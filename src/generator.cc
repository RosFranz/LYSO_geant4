#include "generator.hh"

MyPrimaryGenerator::MyPrimaryGenerator()
{
    fParticleGun = new G4ParticleGun(1); //number of prmary vertex per event

    //it is read when a run starts
    G4ParticleTable *particleTable = G4ParticleTable::GetParticleTable();
    G4String particleName = "proton";
    G4ParticleDefinition *particle = particleTable->FindParticle("proton");

    G4ThreeVector mom(1.,0.,0.); //momentum direction

    fParticleGun->SetParticleDefinition(particle);
    fParticleGun->SetParticleMomentumDirection(mom);
    fParticleGun->SetParticleEnergy(200.*MeV);

    ParticleNumber = 0;
}

MyPrimaryGenerator::~MyPrimaryGenerator()
{
    delete fParticleGun;
}

void MyPrimaryGenerator::GeneratePrimaries(G4Event *anEvent)
{
    //setting the initial position of the primary particle
    G4double worldXHalfLength = 0;
    G4LogicalVolume* worldLV = G4LogicalVolumeStore::GetInstance()->GetVolume("World");
    G4Box* worldBox = nullptr;
    if ( worldLV ) worldBox = dynamic_cast<G4Box*>(worldLV->GetSolid());
    if ( worldBox ) worldXHalfLength = worldBox->GetXHalfLength();
    else  {
        G4cerr << "World volume of box not found." << G4endl;
        G4cerr << "Perhaps you have changed geometry." << G4endl;
        G4cerr << "The gun will be place in the center." << G4endl;
    }
    G4cout.precision(2);
    //G4cout << "World half length in X direction: " << worldXHalfLength/cm << G4endl;
    //G4cout << "\n Starting point of the primary particle along X axis: " << (worldXHalfLength-15.*cm)/cm << G4endl;

    G4ThreeVector pos(-(worldXHalfLength-15.*cm),0.,0.); //starting position of the primary particle
    fParticleGun->SetParticlePosition(pos);
    fParticleGun->GeneratePrimaryVertex(anEvent);
    ++ParticleNumber;

    //if(ParticleNumber%10000 == 0) std::cout << ParticleNumber << std::endl;


}