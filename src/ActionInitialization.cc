#include "ActionInitialization.hh"
#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "SteppingAction.hh"

ActionInitialization::ActionInitialization(DetectorConstruction* detector): 
  G4VUserActionInitialization(),
  detector(detector)
{}


ActionInitialization::~ActionInitialization()
{}


void ActionInitialization::BuildForMaster() const
{
  RunAction* runAction = new RunAction(detector, 0);
  SetUserAction(runAction);
}

void ActionInitialization::Build() const
{
  PrimaryGeneratorAction* primary = new PrimaryGeneratorAction(detector);
  SetUserAction(primary);
    
  RunAction* runAction = new RunAction(detector, primary);
  SetUserAction(runAction);
  
  SteppingAction* steppingAction = new SteppingAction();
  SetUserAction(steppingAction);
}  
