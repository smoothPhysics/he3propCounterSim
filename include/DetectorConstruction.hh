#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

#include "G4NistManager.hh"

class G4LogicalVolume;
class G4Material;

class DetectorConstruction : public G4VUserDetectorConstruction
{
  public:
  
    DetectorConstruction();
   ~DetectorConstruction();
  
    virtual G4VPhysicalVolume* Construct();

    void SetParams(); 
    void PrintParameters();

    const
     G4VPhysicalVolume* GetWorld()      {return physWorld;};               
     G4double           GetLength()     {return worldLength;};  
     G4double           GetRadius()     { return tubeInnerRadius;};    
     G4Material*        GetMaterial()   {return Helium3;};
     
  private:
    G4NistManager *nist;

    //Materials used
    G4Material *worldMat;
    G4Material *Helium3;
    G4Material *steel304Mat;
    G4Material *stainlessSteelMat;
    //G4Material *wireMat;

    G4VPhysicalVolume *physDetector;
    G4VPhysicalVolume *physWorld;

    G4LogicalVolume *detector; //sensitiveDetector

    //Size of the world (G4Box)
    G4double worldLength;
    G4double tubeLength, tubeInnerRadius, tubeOuterRadius, wireRadius;
    //G4double endcapLength, endcapInnerRadius, endcapOuterRadius;  

    void DefineMaterials();
    G4VPhysicalVolume* ConstructVolumes(); 
    virtual void ConstructSDandField(); 
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

