/**
* Stepping action from geant4-10.7.4/examples/extendes/hadronic/Hadr03 and adapted for
* ³He neutron detector simulation which implements a sensitive detector volume
*/

#include "SteppingAction.hh"
#include "Run.hh"

#include "G4ParticleTypes.hh"
#include "G4RunManager.hh"
#include "G4HadronicProcess.hh"
#include "G4Threading.hh"

SteppingAction::SteppingAction()
: G4UserSteppingAction()
{ }

SteppingAction::~SteppingAction()
{ }

void SteppingAction::UserSteppingAction(const G4Step* aStep)
{
 Run* run = static_cast<Run*>(G4RunManager::GetRunManager()->GetNonConstCurrentRun());
         
  //count processes
  const G4StepPoint* endPoint = aStep->GetPostStepPoint();
  G4VProcess* process = const_cast<G4VProcess*>(endPoint->GetProcessDefinedStep());
  run->CountProcesses(process);
  
  //check that an real interaction occured (eg. not a transportation)
  G4StepStatus stepStatus = endPoint->GetStepStatus();
  G4bool transmit = (stepStatus==fGeomBoundary || stepStatus==fWorldBoundary);
  if (transmit) return;
                      
  G4double stepLength = aStep->GetStepLength();
  
  //energy-momentum balance initialisation
  const G4StepPoint* prePoint = aStep->GetPreStepPoint();
  G4double Q                = - prePoint->GetKineticEnergy();
  G4ThreeVector Pbalance    = - prePoint->GetMomentum();
  
  //initialisation of the nuclear channel identification
  //G4HadronicProcesses returns NULL for elastic processes, should be fixed with Geant4 Version > 11.2
  //https://geant4-forum.web.cern.ch/t/is-there-an-alternative-to-gettargetisotope-with-neutronhp/621
  G4ParticleDefinition* particle = aStep->GetTrack()->GetDefinition();
  G4String partName = particle->GetParticleName();
  G4String nuclearChannel = partName;
  G4HadronicProcess* hproc = dynamic_cast<G4HadronicProcess*>(process);
  const G4Isotope* target = NULL;
  if (hproc) target = hproc->GetTargetIsotope();
  G4String targetName = "XXXX";  
  if (target) targetName = target->GetName();
  nuclearChannel += " + " + targetName + " --> ";
  if (targetName == "XXXX") run->SetTargetXXX(true);

  if (aStep->GetTrack()->GetTrackStatus() == fAlive) {
    G4double energy = endPoint->GetKineticEnergy();      
 
    G4ThreeVector momentum = endPoint->GetMomentum();
    Q        += energy;
    Pbalance += momentum;
  
    nuclearChannel += partName + " + ";
  }  
  
  //secondaries
  const std::vector<const G4Track*>* secondary = aStep->GetSecondaryInCurrentStep();  
  for (size_t lp=0; lp<(*secondary).size(); lp++) {
    particle = (*secondary)[lp]->GetDefinition(); 
    G4String name   = particle->GetParticleName();
    G4String type   = particle->GetParticleType();      
    G4double energy = (*secondary)[lp]->GetKineticEnergy();
    if(process->GetProcessName() != "Transportation" && process->GetProcessName() != "hadElastic")
      run->ParticleCount(name,energy);
    particleFlag[particle]++;
  }  
  
  // nuclear channel
  const G4int kMax = 16;  
  const G4String conver[] = {"0","","2 ","3 ","4 ","5 ","6 ","7 ","8 ","9 ",
                             "10 ","11 ","12 ","13 ","14 ","15 ","16 "};
  std::map<G4ParticleDefinition*,G4int>::iterator ip;               
  for (ip = particleFlag.begin(); ip != particleFlag.end(); ip++) {
    particle = ip->first;
    G4String name = particle->GetParticleName();      
    G4int nb = ip->second;
    if (nb > kMax) nb = kMax;   
    G4String Nb = conver[nb];    
    if (particle == G4Gamma::Gamma()) {
     run->CountGamma(nb);
     Nb = "N ";
     name = "gamma";
    } 
    if (ip != particleFlag.begin()) 
      nuclearChannel += " + ";
    nuclearChannel += Nb + name;
  }
 
  run->CountNuclearChannel(nuclearChannel, Q);
    
  particleFlag.clear();

  //Print progress of current run
  G4int eventID = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
  (eventID != previousEventID) ? printProgress = true : printProgress = false;  
  if((eventID % 10000 == 0) && printProgress) {
    G4cout << "Current event:\t" << eventID << G4endl;
    printProgress = false;
    previousEventID = eventID;
  }
}

