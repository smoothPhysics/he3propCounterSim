#ifndef RunAction_h
#define RunAction_h 1

#include "G4UserRunAction.hh"
#include "G4VProcess.hh"
#include "globals.hh"
#include <map>
#include "g4root.hh"


class DetectorConstruction;
class Run;
class RunMessenger;
class PrimaryGeneratorAction;
class HistoManager;
class G4Run;


class RunAction : public G4UserRunAction
{
  public:
    RunAction(DetectorConstruction*, PrimaryGeneratorAction*);
   ~RunAction();

  public:
    virtual G4Run* GenerateRun();   
    virtual void   BeginOfRunAction(const G4Run*);
    virtual void   EndOfRunAction(const G4Run*);
    
                                
  private:
    DetectorConstruction*      detector;
    PrimaryGeneratorAction*    primary;
    Run*                       run;        
     
    G4bool print;  //optional printing           
};

#endif

