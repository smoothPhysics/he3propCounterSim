#include "PhysicsList.hh"

#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"

#include "G4HadronElasticPhysicsHP.hh"
#include "G4HadronElasticPhysicsXS.hh"

#include "G4HadronPhysicsQGSP_BIC_HP.hh"
#include "G4IonElasticPhysics.hh"
#include "G4IonPhysicsXS.hh"
#include "GammaNuclearPhysicsLEND.hh"
//#include "G4RadioactiveDecayPhysics.hh"
#include "G4EmStandardPhysics.hh"
#include "G4EmExtraPhysics.hh"

// particles
#include "G4BosonConstructor.hh"
#include "G4LeptonConstructor.hh"
#include "G4MesonConstructor.hh"
#include "G4BosonConstructor.hh"
#include "G4BaryonConstructor.hh"
#include "G4IonConstructor.hh"
#include "G4ShortLivedConstructor.hh"

/**
* Constructor of the used modular physic list, 
* which implements all relevant physics used for the neutron detetctor simulation
*/
PhysicsList::PhysicsList()
:G4VModularPhysicsList()
{
  G4int verb = 0;  
  SetVerboseLevel(verb);
  
  //add new units for cross sections
  new G4UnitDefinition( "mm2/g",  "mm2/g", "Surface/Mass", mm2/g);
  new G4UnitDefinition( "um2/mg", "um2/mg","Surface/Mass", um*um/mg);  
  
  //Hadron Elastic scattering
  RegisterPhysics( new G4HadronElasticPhysicsHP(verb));  
  
  //Hadron Inelastic physics
  //  -QGS Quark gluon string model (>~20GeV) 
  //  -BIC Binary Cascade Model (<~10 GeV) 
  //  -HP High Precision Neutron Model (<20MeV, relevant model for this sim) 
  RegisterPhysics( new G4HadronPhysicsQGSP_BIC_HP(verb));
    
  //Ion Elastic scattering
  RegisterPhysics( new G4IonElasticPhysics(verb));
  
  //Ion Inelastic physics
  RegisterPhysics( new G4IonPhysicsXS(verb));

  //EM physics
  RegisterPhysics( new G4EmStandardPhysics(verb));
  RegisterPhysics( new G4EmExtraPhysics(verb));

  //Gamma physics
  RegisterPhysics( new GammaNuclearPhysicsLEND("gamma"));
  
  //Radioactive decay
  //RegisterPhysics(new G4RadioactiveDecayPhysics());
}


PhysicsList::~PhysicsList()
{ }

/**
* virtual void ConstructParticle():
* Construction of all relevant particle types
*/
void PhysicsList::ConstructParticle()
{
  G4BosonConstructor pBosonConstructor;
  pBosonConstructor.ConstructParticle();

  G4LeptonConstructor pLeptonConstructor;
  pLeptonConstructor.ConstructParticle();

  G4MesonConstructor pMesonConstructor;
  pMesonConstructor.ConstructParticle();

  G4BaryonConstructor pBaryonConstructor;
  pBaryonConstructor.ConstructParticle();

  G4IonConstructor pIonConstructor;
  pIonConstructor.ConstructParticle();

  G4ShortLivedConstructor pShortLivedConstructor;
  pShortLivedConstructor.ConstructParticle();

  G4cout << "Particles constructed!" << G4endl;  
}

void PhysicsList::SetCuts()
{
   SetCutValue(0.*mm, "proton");
   SetCutValue(0.*mm, "triton");
}
