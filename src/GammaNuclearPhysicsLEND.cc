/**
*  Gamma physics for nuclear processes from geant4-10.7.4/examples/extendes/hadronic/Hadr03
*/

#include "GammaNuclearPhysicsLEND.hh"

#include "G4ParticleDefinition.hh"
#include "G4ProcessManager.hh"

// Processes
#include "G4PhotoNuclearProcess.hh"
#include "G4LENDorBERTModel.hh"
#include "G4LENDCombinedCrossSection.hh"

#include "G4SystemOfUnits.hh"

GammaNuclearPhysicsLEND::GammaNuclearPhysicsLEND(const G4String& name)
:  G4VPhysicsConstructor(name)
{ }

GammaNuclearPhysicsLEND::~GammaNuclearPhysicsLEND()
{ }

void GammaNuclearPhysicsLEND::ConstructProcess()
{
   G4ProcessManager* pManager = G4Gamma::Gamma()->GetProcessManager();
   
   G4PhotoNuclearProcess* process = new G4PhotoNuclearProcess();
   
   G4LENDorBERTModel* lend = new G4LENDorBERTModel(G4Gamma::Gamma());
   lend->SetMaxEnergy(20*MeV);
   process->RegisterMe(lend);
   
   G4LENDCombinedCrossSection* lendXS = new G4LENDCombinedCrossSection(G4Gamma::Gamma());
   process->AddDataSet(lendXS);
   pManager->AddDiscreteProcess(process);
}
