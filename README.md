# Muons for Peace

This code is based on example B1 from Geant4. It examines how atmospheric muons can be used as an imaging tool (muography) for a bunker. In particular, it investigates the feasibility of this technique to find small quantitites of fissile material within the bunker. For the muon generation `EcoMug` is used. The code can be executed multithreaded, if Geant4 is compiled such that it supports multithreading.
The current setup allows to change certain parameters without recompiling the code, this can be done in `inputMuons.ini` and `simpleRun.mac`. The data is stored as `.root` files, additonally the parameters as set in `inputMuons.ini` will be stored for each run in `parameters.ini`.


## Compiling and running the code
- in addition to the Geant4 prerequesits the boost library is required, which can be installed as  `$ sudo apt install libboost-all-dev`.

### Geant4
  - requires Geant4 v11.0.0 (or similar new version)
  - follow install instructions on https://geant4-userdoc.web.cern.ch/UsersGuides/InstallationGuide/html/installguide.html#buildandinstall and post-install setup on https://geant4-userdoc.web.cern.ch/UsersGuides/InstallationGuide/html/postinstall.html

  **Summary of install instructions** <br>
   (ubuntu)
   1. download source code from https://geant4.web.cern.ch/download/11.1.1.html
   2. unpack into directroy of choice e.g. `/home/Geant4/` &#8594; `/home/Geant4/geant4-v11.1.1`
   3. create seperate build directory e.g. `geant4-v11.1.1-build` &#8594; `/home/Geant4/geant4-v11.1.1-build`
   4. create cmake files inside build dir <br>`/home/Geant4/geant4-v11.1.1-build $ cmake -DCMAKE_INSTALL_PREFIX=/home/Geant4/geant4-v11.1.1-install -DGEANT4_INSTALL_DATA=ON -DGEANT4_USE_QT=ON ../geant4-v11.1.1` (using QT for visualization)
   5. run make `$ make -jN` (N number of cores (plus some))
   6. run make install `$ make install`
   7. configure environment for Geant4 `$ source ~/Geant4/geant4-v11.1.1-install/bin/geant4.sh` <br>
      note: this needs to be done for every new shell

  **Test installation** <br>
  Test if Geant4 installation was successfull by executing example B1
  1. copy source code for example `B1` from `geant4-v11.1.1/examples/basic/` to e.g. `/home/Geant4/examples/` &#8594; `/home/Geant4/examples/B1`
  2. create build dir `/home/Geant4/examples/B1-build`
  3. source Geant4 script (if not already in bashrc)
  	- alternative: source custom script (`geant4init.sh` with replacing the pathnames), if G4DIR variable is exported, the cmake option `-DGEANT4_DIR` is not needed anymore
  5. go to build dir and create cmake files `$ cmake -DGEANT4_DIR=/home/Geant4/geant4-v11.1.1-install/lib/Geant4-11.1.1 ../B1` <br>
	- `DGEANT4_DIR=` location of Geant4 headers/libs
	- second argument is source file
  5. use `make -jN` and `make install`
  6. execute example by `./exampleB1` &#8594; this should also start visualization <br>
     or `./exampleB1 run1.mac` &#8594; this will use parameters specified in run1.mac, no visualization

### EcoMug
  - already included in this repo (but github repo at https://github.com/dr4kan/EcoMug), see below for details

### Muons Bunker
Basically the same steps as for example B1, only boost library is needed additionally.
  1. install boost library (e.g. `$ sudo apt install libboost-all-dev`)
  2. use source code from this repository
  3. source Geant4 script
  4. create build directory & inside compile code by `/muonsBunker-build $ cmake -DGEANT4_DIR=/home/Geant4/geant4-v11.1.1-install/lib/Geant4-11.1.1 ../muonsBunker`, then `make -jN`
  5. create `results` folder in the same directory, e.g. `/muons4Peace/muonsBunker`,`/muons4Peace/muonsBunker-build`, `/muons4Peace/results`
  6. choose parameters for run in `inputMuons.ini`, specify amount of events in `simpleRun.mac`
  7. execute example by `./muonsMain` &#8594; this should also start visualization <br>
     or `./muonsMain simpleRun.mac` &#8594; this will use parameters specified in simpleRun.mac, no visualization


## EcoMug (Efficiant Cosmic Muon Generator for cosmic-ray muon applications)
  - https://www.sciencedirect.com/science/article/abs/pii/S0168900221007178
  - header only library, easy to use cosmic muon generator, developed specifically for muography applications
  - some of the options changeable by user
    - different generation geometries: flat sky(plane), half sphere, cylinder
    - minimum/maximum momentum
    - minimum/maximum zenith angle $\theta$
    - minimum/maximum azimuth angle $\phi$
     
## Muons Bunker Code
### Geometry 
(defined in `DetectorConstruction.cc`)<br>
<br>
  **Bunker**
  - steel concrete bunker, with length = 30.78 m, width = 15. m and height = 5.072 m

  **Fissile Material**
  - fissile material, currently Plutonium cube, placed in the center of the roof
  - 5 cm x 5 cm cube

  **Detector**
  - placed in the center of the bunker
  - 1 m x 1 m surface
  - currently simple plane that can store information on muon position, energy, azimuth and zenith angle

### `inputMuons.ini`
 Allows redefintion of certain parameters without recompiling <br>
 - **Section [input]**
   - the muon generating method can be choosen, either a Geant4 Particle Gun or EcoMug
   - if the simulation should run with or without the fissile material cube <br>
 - **Section [gun]**
   - if the input method is a particle gun, the position and momentum can be choosen <br>
 - **Section [ecomug]**
   - allows to choose between the different EcoMug geometries, a flat plane, a half sphere and a zylinder (note that zylinder was not used and is thus not further specified in the following sections)
   - and to set the center of the geometry
 - **Section [sky]**
   - x and y dimension of the sky can be set 
 - **Section [halfSphere]**
   - set radius of half sphere
  - **Section [results]**
    - specify pathname to folder where folders with results are stored (needs to exist before simulation run)
    - choose foldername (which will be created at pathname) which contains results (and `parameters.ini` with the parameters specified in `inputMuons.ini`)
     - if it does not exists its created, otherwise new results are stored in same folder (but measurements will not be overwritten due to time stamps that are added to each of the filenames)

### Class `FileManager`
 - additional class to handle `inputMuons.ini` threadsafe (uses `boost` library for parsing file)

## Analysis
  - analysis with root (https://root.cern/install/), using version 6.26/04
  - can be installed via `$sudo snap install root-framework`
