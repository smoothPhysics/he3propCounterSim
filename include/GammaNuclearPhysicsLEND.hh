#ifndef GammaNuclearPhysicsLEND_h
#define GammaNuclearPhysicsLEND_h 1

#include "globals.hh"
#include "G4VPhysicsConstructor.hh"

class GammaNuclearPhysicsLEND : public G4VPhysicsConstructor
{
  public:
    GammaNuclearPhysicsLEND(const G4String& name="gamma");
   ~GammaNuclearPhysicsLEND();

  public:
    virtual void ConstructParticle() { };
    virtual void ConstructProcess();
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

