## ABOUT

This multithreaded Geant4 program simulates a ³He counter tube for thermal and epithermal neutron detection. The detector simulated is the VacuTec 70063 proportional counter with 4bar pressure.
Datasheet: https://www.vacutec-gmbh.de/fileadmin/VacuTec-Files/produkte/umwelt/Zaehlrohre__GM-__P-__N-_/Neutronen_Zaehlrohre/He-3_NEUTRON_DETECTORS_flyer_710_3.pdf
	
## GEOMETRY DEFINITION

The world is a Box filled with Air at 1 bar pressure.
Inside of the world volume is the ³He counter tube (G4Tubs) which has following components 
asigned to his volume:
	-Stainless steel wall (AES 304 stainless steel)
	-Active detector Gas (³He, 4bar pressure)
	-Currently only as comment: Anode wire in the middle of the tube with diameter 100um

The detector measurements can be changed in the DetectorConstruction.hh header file
 	
## PHYSICS LIST
 
The PhysicsList implements builders for:
	-Hadronic elastic physics
	-Hadronic inelastic physics (QGSP_BIC_HP)
	-Ion elastic physics
	-Ion inelastic
	-EM physics
	-Gamma nuclear physics 
 	 
## THE PRIMARY GENERATOR
 
Primary particle: neutron
Default energy: 25 meV (thermal neutrons)

Each event is 1 neutron with momentum in negative x direction pointing towards the 
³He volume of the counter tube.

Two options for the position of the particle gun are available (both are in the PrimaryGeneratorAction.cc file, one option is always commented):
	-Neutron bombardment over the full length and height of the counter tube
	-Focused neutron beam with round cross sectional area

## ANALYSIS

Goal of each run of n events is to:
	1. Store all reaction points of the n + ³He -> p + ³H process in the counter tube
	2. Calculate the ³He inelastic cross section and compare it with the G4HadronicProcessStore which
	has its cross section data from the ENDF (https://www-nds.iaea.org/exfor/endf.htm) libary. 
	3. Get the total number of the previous mentioned process
	4. Get the number of neutrons passing the counter tube
	5. List all hadronic processes with the counter tube (wall + gas) 
	6. List all produced particles through inelastic processes
	7. Get the energy distribution of the proton and the triton
	8. To see the wall effect: Get the energy deposit of the secondary produces particles for each inelastic event in the counter tube.

1, 4, 7 and 8 are saved to a root file with the default name hits.root. It is possible to change the filename via /analysis/setFileName <fileName> in the geant4 interactive mode.

The cross section information (point 2) is added to the output file crossSection.txt.

The detector efficiency data, list of nuclear reactions, cross section and the list of produced particles gets written to the output file output.txt.
	
# HOW TO START ?

If you dont have geant4 installed: https://geant4.web.cern.ch/download/10.7.4.html

To build the application: 
	1. Create a build directory in the program folder
	2. Go to the build directory and execute: $ cmake ..
	3. After that you can execute:  $ make 
	and it should be the geant4 application with the name neutronDet.
	4. Run the executable NeutronDet

To start the application:
	1. After starting the application it is necessary to initialize the Geant4 kernel with the
	interactive command /run/initialize. 
	2. Optionally: you can change the name of the root output file with the interactive commmand
	/analysis/setFileName <fileName>
	3. Create a run with the interactive command /run/beamOn <number of events>

The above mentioned commands are only necessary if you dont use one of the 3 provided macro files which are explained in the next section.

There are also 3 macro files provided: 
	-vis.mac (to visualize the geometry)
	-crossSectionAnalysis.mac (creates various runs of 10^5 neutrons with different energies. The energy dependent cross section can then be plotted using the crossSection.txt file)
	-hitsAnalysis.mac (creates various runs of 10^5 neutrons with different energies. Before each run it creates a new root file with the following name: "hits_<neutron energy>_<detector pressure>.root")
	All the macro files can be used with the command /control/execute <fileName>.

    
    
