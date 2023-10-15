#ifndef PrimaryGeneratorAction_h
#define PrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "globals.hh"

class G4Event;
class DetectorConstruction;

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  public:
    PrimaryGeneratorAction(DetectorConstruction*);    
   ~PrimaryGeneratorAction();

    virtual void GeneratePrimaries(G4Event*);
    G4ParticleGun* GetParticleGun() {return particleGun;};

  private:
    G4ParticleGun*  particleGun;
    DetectorConstruction* detector;
};

#endif

