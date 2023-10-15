/**
* Run class from geant4-10.7.4/examples/extended/hadronic/Hadr03 adapted for
* ³He neutron detector simulation.
*   -Added calculation and adaption regarding the neutron detector efficiency
*   -Adapted output
*   -Removed non relevant information
*   -Added output file for console prints and for ³He cross section information
*/

#include "Run.hh"
#include "DetectorConstruction.hh"
#include "PrimaryGeneratorAction.hh"

#include "G4ProcessTable.hh"
#include "G4HadronicProcessStore.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"


Run::Run(DetectorConstruction* det)
: G4Run(),
  fDetector(det), fParticle(0), fEkin(0.),
  fTotalCount(0), fGammaCount(0), totalTransportationDetectorCount(0),
  fSumTrack(0.), fSumTrack2(0.),
  sumDetectorTrack(0.), sumDetectorTrack2(0.),
  fTargetXXX(false)
{
  for (G4int i=0; i<3; i++) { fPbalance[i] = 0. ; }
  for (G4int i=0; i<3; i++) { fNbGamma[i] = 0 ; }
  fPbalance[1] = DBL_MAX;
  fNbGamma[1]  = 10000;
}

Run::~Run()
{ }


void Run::SetPrimary(G4ParticleDefinition* particle, G4double energy)
{ 
  fParticle = particle;
  fEkin = energy;
} 


void Run::SetTargetXXX(G4bool flag)
{ 
  fTargetXXX = flag;
}
 

void Run::CountProcesses(G4VProcess* process) 
{
  G4String procName = process->GetProcessName();
  std::map<G4String,G4int>::iterator it = fProcCounter.find(procName);
  if ( it == fProcCounter.end()) {
    fProcCounter[procName] = 1;
  }
  else {
    fProcCounter[procName]++; 
  }
}                 


void Run::SumTrack(G4double trackl)
{
  fSumTrack += trackl; fSumTrack2 += trackl*trackl;  
}

void Run::SumDetectorTrack(G4double trackl) {
  sumDetectorTrack += trackl;
  sumDetectorTrack2 += trackl*trackl;
}

void Run::CountDetectorHit() {
  fTotalCount++;
}

void Run::CountTransportationDetector() {
  totalTransportationDetectorCount++;
}


void Run::CountNuclearChannel(G4String name, G4double Q)
{
  std::map<G4String, NuclChannel>::iterator it = fNuclChannelMap.find(name);
  if ( it == fNuclChannelMap.end()) {
    fNuclChannelMap[name] = NuclChannel(1, Q);
  }
  else {
    NuclChannel& data = it->second;
    data.fCount++;
    data.fQ += Q;
  }       
}

void Run::ParticleCount(G4String name, G4double Ekin)
{
  std::map<G4String, ParticleData>::iterator it = fParticleDataMap.find(name);
  if (it == fParticleDataMap.end()) {
    fParticleDataMap[name] = ParticleData(1, Ekin, Ekin, Ekin);
  }
  else {
    ParticleData& data = it->second;
    data.fCount++;
    data.fEmean += Ekin;
    
    //Update min & max kinetic energy
    G4double emin = data.fEmin;
    if (Ekin < emin) data.fEmin = Ekin;
    G4double emax = data.fEmax;
    if (Ekin > emax) data.fEmax = Ekin; 
  }   
}

void Run::Balance(G4double Pbal)
{ 
  fPbalance[0] += Pbal;
  //update min max   
  if (fTotalCount == 1) fPbalance[1] = fPbalance[2] = Pbal;  
  if (Pbal < fPbalance[1]) fPbalance[1] = Pbal;
  if (Pbal > fPbalance[2]) fPbalance[2] = Pbal;    
}


void Run::CountGamma(G4int nGamma)
{ 
  fGammaCount++;
  fNbGamma[0] += nGamma;
  //update min max   
  if (fGammaCount == 1) fNbGamma[1] = fNbGamma[2] = nGamma;  
  if (nGamma < fNbGamma[1]) fNbGamma[1] = nGamma;
  if (nGamma > fNbGamma[2]) fNbGamma[2] = nGamma;    
}


void Run::Merge(const G4Run* run)
{
  const Run* localRun = static_cast<const Run*>(run);
  
  //primary particle info
  //
  fParticle = localRun->fParticle;
  fEkin     = localRun->fEkin;
  
  // accumulate sums
  //
  fTotalCount += localRun->fTotalCount;
  totalTransportationDetectorCount += localRun->totalTransportationDetectorCount;
  fGammaCount += localRun->fGammaCount;
  fSumTrack += localRun->fSumTrack;
  fSumTrack2 += localRun->fSumTrack2;
  sumDetectorTrack += localRun->sumDetectorTrack;
  sumDetectorTrack2 += localRun->sumDetectorTrack2;


  fPbalance[0] += localRun->fPbalance[0];
  G4double min,max;
  min = localRun->fPbalance[1]; max = localRun->fPbalance[2];
  if (fPbalance[1] > min) fPbalance[1] = min;
  if (fPbalance[2] < max) fPbalance[2] = max;

  fNbGamma[0] += localRun->fNbGamma[0];
  G4int nbmin, nbmax; 
  nbmin = localRun->fNbGamma[1]; nbmax = localRun->fNbGamma[2];
  if (fNbGamma[1] > nbmin) fNbGamma[1] = nbmin;
  if (fNbGamma[2] < nbmax) fNbGamma[2] = nbmax;
  
  //map: processes count
  std::map<G4String,G4int>::const_iterator itp;
  for ( itp = localRun->fProcCounter.begin();
        itp != localRun->fProcCounter.end(); ++itp ) {

    G4String procName = itp->first;
    G4int localCount = itp->second;
    if ( fProcCounter.find(procName) == fProcCounter.end()) {
      fProcCounter[procName] = localCount;
    }
    else {
      fProcCounter[procName] += localCount;
    }  
  }
      
  //map: nuclear channels
  std::map<G4String,NuclChannel>::const_iterator itc;
  for (itc = localRun->fNuclChannelMap.begin(); 
       itc != localRun->fNuclChannelMap.end(); ++itc) {
    
    G4String name = itc->first;
    const NuclChannel& localData = itc->second;   
    if ( fNuclChannelMap.find(name) == fNuclChannelMap.end()) {
      fNuclChannelMap[name]
       = NuclChannel(localData.fCount, localData.fQ);
    }
    else {
      NuclChannel& data = fNuclChannelMap[name];   
      data.fCount += localData.fCount;
      data.fQ     += localData.fQ;
    }   
  } 
        
  //map: particles count
  std::map<G4String,ParticleData>::const_iterator itn;
  for (itn = localRun->fParticleDataMap.begin(); 
       itn != localRun->fParticleDataMap.end(); ++itn) {
    
    G4String name = itn->first;
    const ParticleData& localData = itn->second;   
    if ( fParticleDataMap.find(name) == fParticleDataMap.end()) {
      fParticleDataMap[name]
       = ParticleData(localData.fCount, 
                      localData.fEmean, 
                      localData.fEmin, 
                      localData.fEmax);
    }
    else {
      ParticleData& data = fParticleDataMap[name];   
      data.fCount += localData.fCount;
      data.fEmean += localData.fEmean;
      G4double emin = localData.fEmin;
      if (emin < data.fEmin) data.fEmin = emin;
      G4double emax = localData.fEmax;
      if (emax > data.fEmax) data.fEmax = emax; 
    }   
  }
  
  G4Run::Merge(run); 
} 


void Run::EndOfRun(G4bool print) 
{
  G4int prec = 5, wid = prec + 2;  
  G4int dfprec = G4cout.precision(prec);
  
  std::string  filename = "hits.txt";
  std::ofstream dataFile;
  bool fileOpen = false;
  dataFile.open(filename, std::ios::app);
  if (dataFile.is_open()) {
    fileOpen = true;
  }

  //run condition
  G4Material* material = fDetector->GetMaterial();
  G4double density = material->GetDensity();
  
  G4String Particle = fParticle->GetParticleName();    
  G4cout << "\nThe run is " << numberOfEvent << " neutrons" << " of "
         << G4BestUnit(fEkin,"Energy") << " through " 
         << "a VacuTect ³He proprotional counter tube (70063, 4bar)" 
         << G4endl;
  if(fileOpen) {
    dataFile << "\nThe run is " << numberOfEvent << " neutrons" << " of "
         << G4BestUnit(fEkin,"Energy") << " through " 
         << "a VacuTect ³He proprotional counter tube (70063, 4bar)"
         << G4endl;
  }

  if (numberOfEvent == 0) { G4cout.precision(dfprec); return;}
             
  //frequency of processes
  G4cout << "\nProcesses (without transportation):" << G4endl;
  if(fileOpen)
    dataFile << "\nProcesses (without transportation):" << G4endl;
  std::map<G4String,G4int>::iterator it;    
  for (it = fProcCounter.begin(); it != fProcCounter.end(); it++) {
    G4String procName = it->first;
    G4int    count    = it->second;
    if(procName != "Transportation") {
    G4cout << procName << "= " << count << "\t";
    if(fileOpen)
      dataFile << procName << "= " << count << "\t";
    }
  }
      
  
  G4int hits = fTotalCount;
  G4int transportations = totalTransportationDetectorCount;
  G4int entries = (hits + transportations); 
  G4cout << "\n\nEntries, hits and transportations in ³He volume:" << G4endl; 
  G4cout << "Entries:\t\t" << entries
         << " (" << (((G4double)entries / (G4double)numberOfEvent)*100.0) << "% of incident neutrons)"
         << G4endl;
  G4cout << "Hits:\t\t\t" << hits
         << " (" << (((G4double)hits / (G4double)entries)*100.0) << "% of entries)"
         << G4endl;
  G4cout << "Transportations:\t" << transportations
         << " (" << (((G4double)transportations / (G4double)entries)*100.0) << "% of entries)"
         << G4endl;

  G4cout << "\nRelative effiency (hits to entered neutrons):\t"
         << (((G4double)hits / (G4double)entries)*100.0) << "%" 
         << "\nTotal effiency (hits to incident neutrons):\t"
         << (((G4double)hits / (G4double)numberOfEvent)*100.0) << "%" << G4endl;

  if(fileOpen) {
    dataFile << "\n\nEntries, hits and transportations in ³He volume:" << G4endl; 
    dataFile << "Entries:\t\t" << (hits + transportations)
          << " (" << (((G4double)(hits + transportations) / (G4double)numberOfEvent)*100.0) << "% of incident neutrons)"
          << G4endl;
    dataFile << "Hits:\t\t" << hits
          << " (" << (((G4double)hits / (G4double)entries)*100.0) << "% of entries)"
          << G4endl;
    dataFile << "Transportations:\t" << transportations
          << " (" << (((G4double)transportations / (G4double)entries)*100.0) << "% of entries)"
          << G4endl;

    dataFile << "\nRelative effiency (hits to entered neutrons):\t"
          << (((G4double)hits / (G4double)entries)*100.0) << "%" 
          << "\nTotal effiency (hits to incident neutrons):\t"
          << (((G4double)hits / (G4double)numberOfEvent)*100.0) << "%" << G4endl;
  }

  
  if (fTotalCount == 0) fTotalCount = 1;   //force printing anyway
  
  //compute mean free path and related quantities
  G4double MeanFreePath = fSumTrack / fTotalCount;   
  G4double MeanTrack2   = fSumTrack2/fTotalCount;    
  G4double rms = std::sqrt(std::fabs(MeanTrack2 - MeanFreePath*MeanFreePath));
  G4double CrossSection = 0.0;
  if(MeanFreePath > 0.0) { CrossSection = 1./MeanFreePath; }
  G4double massicMFP = MeanFreePath*density;
  G4double massicCS  = 0.0;
  if(massicMFP > 0.0) { massicCS = 1./massicMFP; }
   
  G4cout << "\n\nMeanFreePath:\t"   << G4BestUnit(MeanFreePath,"Length")
         << " +- "                   << G4BestUnit(rms,"Length")
         << "\tmassic: "             << G4BestUnit(massicMFP, "Mass/Surface")
         << G4endl;
  if(fileOpen) {
    dataFile  << "\n\nMeanFreePath:\t"   << G4BestUnit(MeanFreePath,"Length")
              << " +- "                   << G4BestUnit(rms,"Length")
              << "\tmassic: "             << G4BestUnit(massicMFP, "Mass/Surface")
              << G4endl;
  }
         
  //inelastic cross section per atom
  G4double crossSection = 0;
  if (material->GetNumberOfElements() == 1) {
    G4double nbAtoms = material->GetTotNbOfAtomsPerVolume();
    crossSection = CrossSection/nbAtoms;
    G4cout << "Number of Atoms per Volume:\t" << nbAtoms 
           << "\nInelastic cross section per atom:\t" << G4BestUnit(crossSection,"Surface") 
           << G4endl;
    if(fileOpen) {
      dataFile  << "Number of Atoms per Volume:\t" << nbAtoms 
                << "\nInelastic cross section per atom:\t" << G4BestUnit(crossSection,"Surface") 
                << G4endl;
    }
  }  
   
  //check cross section from G4HadronicProcessStore
  G4cout << "\nVerification: " << "crossSections from G4HadronicProcessStore:";
  if(fileOpen) dataFile << "\nVerification: " << "crossSections from G4HadronicProcessStore:";
  
  G4ProcessTable* processTable  = G4ProcessTable::GetProcessTable();
  G4HadronicProcessStore* store = G4HadronicProcessStore::Instance();
  G4double sumc1 = 0.0, sumc2 = 0.0; 
  const G4Element* element = material->GetElement(0);
  for (it = fProcCounter.begin(); it != fProcCounter.end(); it++) {
    G4String procName = it->first;
    if(procName == "neutronInelastic") {
      G4VProcess* process = processTable->FindProcess(procName, fParticle);
      G4double xs1 =
      store->GetCrossSectionPerVolume(fParticle,fEkin,process,material);
      G4double massSigma = xs1/density;
      sumc1 += massSigma;      
      G4double xs2 = store->GetCrossSectionPerAtom(fParticle,fEkin,process,element,material);
      sumc2 += xs2;
      G4cout << "\n" << std::setw(20) << procName << "= "
              << G4BestUnit(massSigma, "Surface/Mass") << "\t"
              << G4BestUnit(xs2, "Surface");
      if(fileOpen) {
        dataFile  << "\n" << std::setw(20) << procName << "= "
                << G4BestUnit(massSigma, "Surface/Mass") << "\t"
                << G4BestUnit(xs2, "Surface");
      }
    }
  }             
              
 //nuclear channel count
 G4cout << "\n\n List of nuclear reactions: \n" << G4endl; 
if(fileOpen) dataFile << "\n\n List of nuclear reactions: \n" << G4endl; 
 std::map<G4String,NuclChannel>::iterator ic;               
 for (ic = fNuclChannelMap.begin(); ic != fNuclChannelMap.end(); ic++) { 
    G4String name    = ic->first;
    NuclChannel data = ic->second;
    G4int count = data.fCount;
    G4double Q  = data.fQ/count; 
    if (print && !name.contains("XXX")) {        
      G4cout << std::setw(60) << name << ": " << std::setw(7) << count
             << "   Q = " << std::setw(wid) << G4BestUnit(Q, "Energy")
             << G4endl;    
      if(fileOpen) 
        dataFile  << std::setw(60) << name << ": " << std::setw(7) << count
                  << "   Q = " << std::setw(wid) << G4BestUnit(Q, "Energy")
                  << G4endl; 
    }        
 } 
            
 //particles count
 G4cout << "\n List of generated particles:" << G4endl;
 if(fileOpen) dataFile << "\n List of generated particles:" << G4endl;
     
 std::map<G4String,ParticleData>::iterator itn;               
 for (itn = fParticleDataMap.begin(); itn != fParticleDataMap.end(); itn++) { 
    G4String name = itn->first;
    ParticleData data = itn->second;
    G4int count = data.fCount;
    G4double eMean = data.fEmean/count;
    G4double eMin = data.fEmin;
    G4double eMax = data.fEmax;          
    G4cout << "  " << std::setw(13) << name << ": " << std::setw(7) << count
           << "  Emean = " << std::setw(wid) << G4BestUnit(eMean, "Energy")
           << "\t( "  << G4BestUnit(eMin, "Energy")
           << " --> " << G4BestUnit(eMax, "Energy") 
           << ")" << G4endl;
    if(fileOpen) {
      dataFile  << "  " << std::setw(13) << name << ": " << std::setw(7) << count
                << "  Emean = " << std::setw(wid) << G4BestUnit(eMean, "Energy")
                << "\t( "  << G4BestUnit(eMin, "Energy")
                << " --> " << G4BestUnit(eMax, "Energy") 
                << ")" << G4endl;
    }            
  }

  if(fileOpen) {
    dataFile << "============================================================================\n" << G4endl;
    dataFile.close();
  }
    

  //Store crossSection Information in txt file
  filename = "crossSectionHe3.txt";
  std::ofstream xsFile;
  xsFile.open(filename, std::ios::app);
  if (!xsFile.is_open()) {
      std::cerr << "Failed to open the file." << std::endl;
  } else {
    xsFile << G4BestUnit(fEkin, "Energy") << ", " << G4BestUnit(crossSection,"Surface") << std::endl;
    xsFile.close();
  }

  //remove all contents in fProcCounter, fCount 
  fProcCounter.clear();
  fNuclChannelMap.clear();      
  fParticleDataMap.clear();
                          
  //restore default format         
  G4cout.precision(dfprec);   
}
