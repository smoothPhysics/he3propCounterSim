#ifndef SENSITIVEDETECTOR_HH
#define SENSITIVEDETECTOR_HH

#include "G4VSensitiveDetector.hh"
#include "G4String.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"
#include "G4RunManager.hh"

class SensitiveDetector : public G4VSensitiveDetector {
    public:
        SensitiveDetector(G4String);
        ~SensitiveDetector();
        G4double *SumTrack;
        G4double *ParticleCount;
        G4double *MeanFreePath;
        G4double *CrossSection;

    private:
        G4double totalEDepositPerEvent;
        std::map<G4int,G4double> fEnergyDepositMap;
        
        virtual bool ProcessHits(G4Step *, G4TouchableHistory *);
};

#endif