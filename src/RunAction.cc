#include "RunAction.hh"
#include "Run.hh"
#include "DetectorConstruction.hh"
#include "PrimaryGeneratorAction.hh"
#include "G4Run.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4Threading.hh"

#include "Randomize.hh"
#include <iomanip>

RunAction::RunAction(DetectorConstruction* det, PrimaryGeneratorAction* prim)
  : G4UserRunAction(),
    detector(det), primary(prim), run(0),
    print(true)    
{
  G4int verb = 2;
  //Get the G4AnalysisManager (singleton)
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

  //Create all nTuples to store in root file 
  analysisManager->SetVerboseLevel(verb);
  //Set merging true because of multithreading
  analysisManager->SetNtupleMerging(true);
  //default file name: hits.root
  analysisManager->SetFileName("hits.root");

  //Ntuple to store position of n + ³He reaction in detector volume
  analysisManager->CreateNtuple("Hits", "Hits");
  analysisManager->CreateNtupleDColumn("fX");
  analysisManager->CreateNtupleDColumn("fY");
  analysisManager->CreateNtupleDColumn("fZ");
  analysisManager->FinishNtuple(0);

  //Ntuple to visualize areas where most transportations through the detector are happening
  analysisManager->CreateNtuple("Transportation", "Transportation");
  analysisManager->CreateNtupleDColumn("fX");
  analysisManager->CreateNtupleDColumn("fY");
  analysisManager->CreateNtupleDColumn("fZ");
  analysisManager->CreateNtupleDColumn("pathLength");
  analysisManager->FinishNtuple(1);

  //Ntuple to store the energy of the secondaries (proton, trition) of each detector hit
  analysisManager->CreateNtuple("Secondaries", "Secondaries");
  analysisManager->CreateNtupleDColumn("proton");
  analysisManager->CreateNtupleDColumn("triton"); 
  analysisManager->FinishNtuple(2);

  //Ntuple to store the the total energyDeposit of the secondaries per detector hit
  analysisManager->CreateNtuple("Edep", "Edep");
  analysisManager->CreateNtupleDColumn("energyDeposit");
  analysisManager->FinishNtuple(3);
}

RunAction::~RunAction()
{

}

G4Run* RunAction::GenerateRun()
{ 
  run = new Run(detector); 
  return run;
}


void RunAction::BeginOfRunAction(const G4Run*)
{    
  // show Rndm status
  if (isMaster) G4Random::showEngineStatus();
    
  // keep run condition
  if (primary) { 
    G4ParticleDefinition* particle = primary->GetParticleGun()->GetParticleDefinition();
    G4double energy = primary->GetParticleGun()->GetParticleEnergy();
    run->SetPrimary(particle, energy);
  }
  
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  analysisManager->OpenFile();
    
}


void RunAction::EndOfRunAction(const G4Run*)
{
  if (G4Threading::IsMasterThread()) run->EndOfRun(print);
  
  //save histograms and automatically merge Ntuples 
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  analysisManager->Write();
  analysisManager->CloseFile();
      
  //show Rndm status
  if (isMaster) G4Random::showEngineStatus();
}
