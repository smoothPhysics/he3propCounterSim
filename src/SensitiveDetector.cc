#include "SensitiveDetector.hh"
#include "Run.hh"
#include "g4root.hh"

#include "G4ParticleTypes.hh"

#define INCIDENT_NEUTRON_TRACK 1    //Geant4 Track ID of the incident particle

SensitiveDetector::SensitiveDetector(G4String name) : G4VSensitiveDetector(name),
totalEDepositPerEvent(0.)
{}

SensitiveDetector::~SensitiveDetector()
{}

/**
* virtual bool ProcessHits() from G4SensitiveDetector()
*   Analysis from primary neutron - ³He-detector interactions stored in root file
*      -position of the n + ³He -> p + ³H reaction
*      -energy of secondaries (proton, triton)
*      -path length oer neutron if transportation through sensitive volume happened
*   Analysis of the total energy deposit of the secondary particles per neutron reaction
*   (if secondary particle hits detector wall track will be killed)
*/
bool SensitiveDetector::ProcessHits(G4Step * aStep, G4TouchableHistory * R0Hist) 
{
    Run* run = static_cast<Run*>(G4RunManager::GetRunManager()->GetNonConstCurrentRun());

    //Get G4RunManager and G4AnalysisManger (both singletons)
    G4RunManager *runManager = G4RunManager::GetRunManager();
    G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();

    const G4StepPoint* postStepPoint = aStep->GetPostStepPoint();
    G4VProcess* process = const_cast<G4VProcess*>(postStepPoint->GetProcessDefinedStep());
    G4ParticleDefinition* particle = aStep->GetTrack()->GetDefinition();
    G4double stepLength = aStep->GetStepLength();

    //Count processes of the incident neutron with the ³He gas volume
    if(aStep->GetTrack()->GetTrackID() == INCIDENT_NEUTRON_TRACK) {

        if(process->GetProcessName() == "neutronInelastic") {

            if(aStep->GetTrack()->GetMomentum().getX() == -1.0) {
                G4cout << "Neutron detection after elastic scattering on wall of detector" << G4endl;
            }
            run->CountDetectorHit();
            run->SumTrack(stepLength);

            const std::vector<const G4Track*>* secondary = aStep->GetSecondaryInCurrentStep();  
            for (size_t lp=0; lp<(*secondary).size(); lp++) {
                particle = (*secondary)[lp]->GetDefinition(); 
                G4String name   = particle->GetParticleName();
                G4String type   = particle->GetParticleType();      
                G4double energy = (*secondary)[lp]->GetKineticEnergy();
    
                if (particle == G4Proton::Proton()) {
                    analysisManager->FillNtupleDColumn(2, 0, energy);
                } 
                else if (particle == G4Triton::Triton()) {       
                    analysisManager->FillNtupleDColumn(2, 1, energy);
                }
            }
            analysisManager->AddNtupleRow(2);
         
            analysisManager->FillNtupleDColumn(0, 0,postStepPoint->GetPosition().getX());
            analysisManager->FillNtupleDColumn(0, 1,postStepPoint->GetPosition().getY());
            analysisManager->FillNtupleDColumn(0, 2,postStepPoint->GetPosition().getZ());
            analysisManager->AddNtupleRow(0);

        } else if(process->GetProcessName() == "Transportation") {

            run->CountTransportationDetector();
            analysisManager->FillNtupleDColumn(1, 0, postStepPoint->GetPosition().getX());
            analysisManager->FillNtupleDColumn(1, 1, postStepPoint->GetPosition().getY());
            analysisManager->FillNtupleDColumn(1, 2, postStepPoint->GetPosition().getZ());
            analysisManager->FillNtupleDColumn(1, 3, stepLength);
            analysisManager->AddNtupleRow(1);

            run->SumTrack(stepLength);
        } else if(process->GetProcessName() == "hadElastic") {

        }

    //Count all processes of the secondary particles (except transportations)
    } else if (!process->GetProcessName().contains("Transportation")) {

        G4double eventID = runManager->GetCurrentEvent()->GetEventID();
        std::map<G4int, G4double>::iterator it = fEnergyDepositMap.find(eventID);
        G4double Edep = aStep->GetTotalEnergyDeposit();

        if (it == fEnergyDepositMap.end()) {
            if(totalEDepositPerEvent != 0.0) {
                analysisManager->FillNtupleDColumn(3,0,totalEDepositPerEvent);
                analysisManager->AddNtupleRow(3);
            }
            fEnergyDepositMap[eventID] = aStep->GetTotalEnergyDeposit();
        } else {
            it->second += Edep;
            totalEDepositPerEvent = it->second;
        }
    }    
    return true;
}