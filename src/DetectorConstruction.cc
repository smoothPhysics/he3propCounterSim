#include "DetectorConstruction.hh"
#include "SensitiveDetector.hh"

#include "G4Material.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4Tubs.hh"
#include "G4VisAttributes.hh"

#include "G4RunManager.hh"

//ToDo: Add e-field to sensitive volume
//Header files for e-field 
/*
#include "G4FieldManager.hh"
#include "G4TransportationManager.hh"
#include "G4EqEMFieldWithSpin.hh"
#include "G4ClassicalRK4.hh"
#include "G4MagIntegratorStepper.hh"
#include "G4ChordFinder.hh"
#include "G4PropagatorInField.hh"
*/

DetectorConstruction::DetectorConstruction():G4VUserDetectorConstruction()
{}


DetectorConstruction::~DetectorConstruction()
{}


G4VPhysicalVolume* DetectorConstruction::Construct()
{
  SetParams();
  DefineMaterials();
  ConstructVolumes();
  return physWorld;
}

/**
* Defining all material used by detector:
* -World Material (Air, 1atm)
* -Wall of ³He-counter (SAE 304 stainless steel)
* -Detector gas (³He, 4atm)
*/
void DetectorConstruction::DefineMaterials()
{

    //World material (Air, 1atm)
    worldMat = nist->FindOrBuildMaterial("G4_AIR");

    //Helium-3 definition from geant4 forum:
    //https://geant4-forum.web.cern.ch/t/helium-3-neutron-capture-cross-section-doesnt-seem-right/2544
    G4double density;
    G4double A; //atomic mass
    G4double Z; //atomic number
    G4int protons=2, neutrons=1;
    G4int nucleons = protons + neutrons;
    A = 3.016*g/mole;
    G4Isotope* he3 = new G4Isotope("He3", protons, nucleons, A);
    G4Element* He3 = new G4Element("Helium3", "He3", 1);
    He3->AddIsotope(he3, 100*perCent);
    G4double pressure = 4*bar;  //Pressure of the detector gas
    G4double temperature = 293*kelvin;
    G4double molar_constant = CLHEP::Avogadro*CLHEP::k_Boltzmann; 
    density = (A*pressure)/(temperature*molar_constant);
    Helium3 = new G4Material("Helium3", density, 1, kStateGas, temperature, pressure);
    Helium3->AddElement(He3, 100*perCent);
    G4cout << "Number of Atoms per Volume:\t" << Helium3->GetTotNbOfAtomsPerVolume() << G4endl;

    //Stainless steel type ANSI 304 definition
    //Construction of all elements used in 304 stainless steel:
    A  =  54.94*g/mole;
    G4Element *elMn = new G4Element("Manganese", "Mn", Z = 25., A);
    
    A = 28.09*g/mole;
    G4Element *elSi = new G4Element("Silicon", "Si", Z = 14., A);

    A = 52.00*g/mole;
    G4Element *elCr = new G4Element("Chromium", "Cr", Z = 24., A);

    A = 58.70*g/mole;
    G4Element *elNi = new G4Element("Nickel", "Ni", Z = 28., A);

    A = 55.85*g/mole;
    G4Element* elFe  = new G4Element("Iron", "Fe", Z = 26., A);

    A = 12.011*g/mole;
    G4Element* elC = new G4Element("Carbon", "C", Z = 6., A);
    
    density = 7.999*g/cm3;
    steel304Mat = new G4Material("Stainless steel 304", density, 6);
    steel304Mat->AddElement(elMn, 0.02);
    steel304Mat->AddElement(elSi, 0.01);
    steel304Mat->AddElement(elCr, 0.19);
    steel304Mat->AddElement(elNi, 0.10);
    steel304Mat->AddElement(elFe, 0.6792);
    steel304Mat->AddElement(elC, 0.0008);

    //Stainless steel definition from geant4
    stainlessSteelMat = nist->FindOrBuildMaterial("G4_STAINLESS-STEEL");

    //Definition of the nickel anode wire:
    density = 8.94*g/cm3;
    //wireMat = nist->FindOrBuildMaterial("G4_Ni");
}

void DetectorConstruction::SetParams() {

  //Create an instance of the nist manager
  nist = G4NistManager::Instance();

  //Side length of the cube sized world
  worldLength = 60*mm;

  //Parameters of the VacuTect He-3 proprotional counter 70063, 100mm, 4bar
  //Datasheet: https://www.vacutec-gmbh.de/fileadmin/VacuTec-Files/produkte/umwelt/Zaehlrohre__GM-__P-__N-_/Neutronen_Zaehlrohre/Application_He-3_Neutron_Detectors.pdf
  tubeLength = 50*mm;
  tubeOuterRadius = (25.4 / 2)*mm;  //tube diameter is 25.4mm
  tubeInnerRadius = ((25.2)/2)*mm;  //thickness of the stainless steel wall is 0.2mm (assumed value)
  wireRadius = 100*um;              //wire which acts as anode
}

/**
* Construction an placement of all used volumes:
* -World Volume (G4Box)
* - ³He counter tube
*/
G4VPhysicalVolume* DetectorConstruction::ConstructVolumes()
{
  G4Box *solidWorld = new G4Box("solidWorld", worldLength*2, worldLength, worldLength*2);
  G4LogicalVolume *logicWorld = new G4LogicalVolume(solidWorld, worldMat, "logicWorld");
  physWorld = new G4PVPlacement(0, G4ThreeVector(0.,0.,0.), logicWorld, "physWorld", 0, false, 0, true);

  //Construction of the He3-Tube --------------------------------------------
  //Construction of the tube volume which contains all components of He-3 neutron detector
  G4Tubs *solidHe3Tube = new G4Tubs("solidHe3Tube", 0, tubeOuterRadius, tubeLength, 0., CLHEP::twopi);
  G4LogicalVolume *logicHe3Tube = new G4LogicalVolume(solidHe3Tube, worldMat, "logicHe3Tube");

  //Construction of the inner gas filled volume
  G4Tubs *solidHe3Vol = new G4Tubs("solidHe3Tube", /*wireRadius*/ 0., tubeInnerRadius, tubeLength, 0., CLHEP::twopi);
  G4LogicalVolume *logicHe3Vol = new G4LogicalVolume(solidHe3Vol, Helium3, "logicHe3Vol");
  new G4PVPlacement(0, G4ThreeVector(0.,0.,0.), logicHe3Vol, "detectorGas", logicHe3Tube , false, 0, true);

  /*
  G4Tubs *solidAnodeVolume = new G4Tubs("solidAnodeVolume", 0, wireRadius, tubeLength, 0., CLHEP::twopi);
  G4LogicalVolume *logicAnodeVol = new G4LogicalVolume(solidAnodeVolume, wireMat, "logicAnodeVol");
  new G4PVPlacement(0, G4ThreeVector(0.,0.,0.), logicAnodeVol, "anode", logicHe3Tube, false, 0, true);
  */

  //Construction of the stainless steel wall of the detector
  G4Tubs *solidWall = new G4Tubs("solidWall", tubeInnerRadius, tubeOuterRadius, tubeLength, 0., CLHEP::twopi);
  G4LogicalVolume *logicWall = new G4LogicalVolume(solidWall, steel304Mat, "logicWall");
  new G4PVPlacement(0, G4ThreeVector(0.,0.,0.), logicWall, "case", logicHe3Tube , false, 0, true);

  //Defining vis attributes of all volumes
  G4VisAttributes * he3Att = new G4VisAttributes(G4Colour(60./256, 125./256, 196./256));  
  he3Att->SetForceWireframe(true);
  he3Att->SetForceSolid(true);
  logicHe3Vol->SetVisAttributes(he3Att); 

  G4VisAttributes * wallAtt = new G4VisAttributes(G4Colour(245./256, 204./256, 203./256));  
  wallAtt->SetForceWireframe(true);
  wallAtt->SetForceSolid(true);
  logicWall->SetVisAttributes(wallAtt); 

  /*
  G4VisAttributes * anodeAtt = new G4VisAttributes(G4Colour(173./256, 220./256, 47./256));  
  anodeAtt->SetForceWireframe(true);
  anodeAtt->SetForceSolid(true);
  logicAnodeVol->SetVisAttributes(anodeAtt); 
  */

  //Assign the logic ³He-gas volume as sensitive detector
  detector = logicHe3Vol;
  
  return new G4PVPlacement(0, G4ThreeVector(0.,0.,tubeLength+10*mm), logicHe3Tube, "He3NeutronDetector", (physWorld->GetLogicalVolume()), false, 0, true);
}

void DetectorConstruction::PrintParameters()
{}

/**
* protected void ConstructSDandField()
* -Create sensitiveDetector
* -ToDo: Construct non uniform electric field 
* helpful examples: examples/extended/field/F02 (/F05)
*/
 
void DetectorConstruction::ConstructSDandField() 
{
    SensitiveDetector *sensitiveDet = new SensitiveDetector("sensitiveDet");
    detector->SetSensitiveDetector(sensitiveDet);

    //ToDo: Add electric field
    
}
