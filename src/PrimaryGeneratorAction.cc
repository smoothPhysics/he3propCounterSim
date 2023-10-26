#include "PrimaryGeneratorAction.hh"

#include "DetectorConstruction.hh"

#include "G4Event.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"

PrimaryGeneratorAction::PrimaryGeneratorAction(DetectorConstruction* det)
:G4VUserPrimaryGeneratorAction(),
 particleGun(0), detector(det)                                               
{
  particleGun  = new G4ParticleGun(1);
  G4ParticleDefinition* particle = G4ParticleTable::GetParticleTable()->FindParticle("neutron");
  particleGun->SetParticleDefinition(particle);
  particleGun->SetParticleEnergy(0.025*eV); //default: thermal neutrons 25meV 
  particleGun->SetParticleMomentumDirection(G4ThreeVector(-1.,0.,0.)); //Momentum of neutron beam in negative x-direction
}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete particleGun;
}

/**
* GeneratePrimaries method is called at the beginning of each event
* Sets the randomized position of the particle gun  
* Two options:
*   - Neutron bombardment over the full length and height of the counter tube
*   - Focused neutron beam with round cross sectional area
*/
void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
    //Focused neutron beam with round cross sectional area
    
    G4double r, theta, y, z;
    ///*
    r = detector->GetRadius() * G4UniformRand();
    theta = twopi * G4UniformRand();
    y = r * sin(theta);
    z = r * cos(theta);
    G4ThreeVector pos((r + 10.0)*mm, y*mm, ((detector->GetLength())+z)*mm);
    //*/
    

    //Neutron beam over the full length and height of the counter tube
    /*
    G4double r, y, z;
    r = detector->GetRadius();
    y = 25.2 * G4UniformRand();
    z = 100 * G4UniformRand(); 
    G4ThreeVector pos((r + 10.0)*mm, (y-r)*mm, z*mm);
    */

    particleGun->SetParticlePosition(pos);
    particleGun->GeneratePrimaryVertex(anEvent);
}

