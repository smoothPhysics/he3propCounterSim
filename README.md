## ABOUT

This multithreaded Geant4 program simulates a ³He counter tube for thermal and epithermal neutron detection. The detector simulated is the VacuTec 70063 proportional counter with 4bar pressure.
Datasheet: https://www.vacutec-gmbh.de/fileadmin/VacuTec-Files/produkte/umwelt/Zaehlrohre__GM-__P-__N-_/Neutronen_Zaehlrohre/He-3_NEUTRON_DETECTORS_flyer_710_3.pdf
	
## GEOMETRY DEFINITION

The world is a Box filled with Air at 1 bar pressure.
Inside of the world volume is the ³He counter tube (G4Tubs) which has following components asigned to his volume:<br />
- Stainless steel wall (AES 304 stainless steel)<br />
- Active detector Gas (³He, 4bar pressure)<br />
- Currently only as comment: Anode wire in the middle of the tube with diameter 100um<br /><br />

The detector measurements can be changed in the DetectorConstruction.hh header file
 	
## PHYSICS LIST
 
The PhysicsList implements builders for:<br />
- Hadronic elastic physics<br />
- Hadronic inelastic physics (QGSP_BIC_HP)<br />
- Ion elastic physics<br />
- Ion inelastic<br />
- EM physics<br />
- Gamma nuclear physics <br />
 	 
## THE PRIMARY GENERATOR
 
Primary particle: neutron<br />
Default energy: 25 meV (thermal neutrons)<br /><br />

Each event is 1 neutron with momentum in negative x direction pointing towards the 
³He volume of the counter tube.<br /><br />

Two options for the position of the particle gun are available (both are in the PrimaryGeneratorAction.cc file, one option is always commented):<br />
- Neutron bombardment over the full length and height of the counter tube<br />
- Focused neutron beam with round cross sectional area<br />

## ANALYSIS

Goal of each run of n events is to:<br />
1. Store all reaction points of the n + ³He -> p + ³H process in the counter tube<br />
2. Calculate the ³He inelastic cross section and compare it with the G4HadronicProcessStore which has its cross section data from the ENDF (https://www-nds.iaea.org/exfor/endf.htm) libary.<br />
3. Get the total number of the previous mentioned process<br />
4. Get the number of neutrons passing the counter tube<br />
5. List all hadronic processes with the counter tube (wall + gas) <br />
6. List all produced particles through inelastic processes<br />
7. Get the energy distribution of the proton and the triton<br />
8. To see the wall effect: Get the energy deposit of the secondary produces particles for each inelastic event in the counter tube.<br /><br />

1, 4, 7 and 8 are saved to a root file with the default name hits.root. It is possible to change the filename via /analysis/setFileName <fileName> in the geant4 interactive mode.<br /><br />

The cross section information (point 2) is added to the output file crossSection.txt.<br /><br />

The detector efficiency data, list of nuclear reactions, cross section and the list of produced particles gets written to the output file output.txt.<br />
	
# HOW TO START ?

If you dont have geant4 installed: https://geant4.web.cern.ch/download/10.7.4.html<br /><br />

To build the application: <br />
1. Create a build directory in the program folder<br />
2. Go to the build directory and execute: 
```console
cmake ..
```
3. Build the program:
```console
make
```
4. Run the executable NeutronDet<br /><br />

To start the application:<br />
1. After starting the application it is necessary to initialize the Geant4 kernel with the interactive command /run/initialize.<br />
2. Optionally: you can change the name of the root output file with the interactive commmand /analysis/setFileName <fileName><br />
3. Create a run with the interactive command /run/beamOn <number of events><br /><br />

The above mentioned commands are only necessary if you dont use one of the 3 provided macro files which are explained in the next section.<br /><br />

There are also 3 macro files provided:<br /> 
- vis.mac (to visualize the geometry)<br />
- crossSectionAnalysis.mac (creates various runs of 10^5 neutrons with different energies. The energy dependent cross section can then be plotted using the crossSection.txt file)<br />
- hitsAnalysis.mac (creates various runs of 10^5 neutrons with different energies. Before each run it creates a new root file with the following name: "hits_<neutron energy>_<detector pressure>.root")<br /><br />

All the macro files can be used with the command /control/execute <fileName>

    
    
