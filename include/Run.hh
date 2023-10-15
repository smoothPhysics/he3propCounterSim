#ifndef Run_h
#define Run_h 1

#include "G4Run.hh"
#include "G4VProcess.hh"
#include "globals.hh"
#include <map>
#include "g4root.hh"

class DetectorConstruction;
class G4ParticleDefinition;


class Run : public G4Run
{
  public:
    Run(DetectorConstruction*);
   ~Run();

  public:
    void SetPrimary(G4ParticleDefinition* particle, G4double energy);
    void SetTargetXXX(G4bool);        
    void CountProcesses(G4VProcess* process);
    void SumTrack (G4double); 
    void SumDetectorTrack (G4double);   
    void CountDetectorHit();
    void CountTransportationDetector();
    void CountNuclearChannel(G4String, G4double);                          
    void ParticleCount(G4String, G4double);
    void EnergyDepositPerEvent(G4int, G4double);
    void Balance(G4double);
    void CountGamma(G4int);
        
    virtual void Merge(const G4Run*);      
    void EndOfRun(G4bool); 
   
  private:
    struct ParticleData {
     ParticleData()
       : fCount(0), fEmean(0.), fEmin(0.), fEmax(0.) {}
     ParticleData(G4int count, G4double ekin, G4double emin, G4double emax)
       : fCount(count), fEmean(ekin), fEmin(emin), fEmax(emax) {}
     G4int     fCount;
     G4double  fEmean;
     G4double  fEmin;
     G4double  fEmax;
    };
    
    struct NuclChannel {
     NuclChannel()
       : fCount(0), fQ(0.) {}
     NuclChannel(G4int count, G4double Q)
       : fCount(count), fQ(Q) {}
     G4int     fCount;
     G4double  fQ;
    };
         
  private:
    DetectorConstruction* fDetector;
    G4ParticleDefinition* fParticle;
    G4double              fEkin;
        
    std::map<G4String,G4int> fProcCounter;            
    
    G4int fTotalCount;      //detector hit process counter
    G4int totalTransportationDetectorCount; //transportation through detector counter
    G4int fGammaCount;      //nb of events with gamma
    G4double fSumTrack;     //sum of trackLength
    G4double fSumTrack2;    //sum of trackLength*trackLength
    G4double sumDetectorTrack;     //sum of trackLength
    G4double sumDetectorTrack2;    //sum of trackLength*trackLength
         
    std::map<G4String,NuclChannel>  fNuclChannelMap;    
    std::map<G4String,ParticleData> fParticleDataMap;

    G4bool   fTargetXXX;                    
    G4double fPbalance[3];
    G4int    fNbGamma[3];


};

#endif

