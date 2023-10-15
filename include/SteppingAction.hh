#ifndef SteppingAction_h
#define SteppingAction_h 1

#include "G4UserSteppingAction.hh"
#include "globals.hh"
#include <map>

class G4ParticleDefinition;

class SteppingAction : public G4UserSteppingAction
{
  public:
    SteppingAction();
   ~SteppingAction();

    virtual void UserSteppingAction(const G4Step*);
    
  private:
    std::map<G4ParticleDefinition*,G4int> particleFlag;    

    bool printProgress = false;
    G4int previousEventID = 0;
};

#endif
