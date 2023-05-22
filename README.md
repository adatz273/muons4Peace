# Muons for Peace

This code is based on Geant4 example B1. It examines how atmospheric muons can be used as an imaging tool (muography) for a bunker. In particular, it investigates, the feasibility of this technique to find small quantitites of fissile material within the bunker. For the muon generation `EcoMug` is used. The code can be executed multithreaded, if Geant4 is compiled to support multithreading.
The current setup allows to change certain parameters without recompiling the code, this can be done in `cosmicMuons.ini` and `simpleRun.mac`. The data is stored as `.root` files, additonally the parameters as set in `cosmicMuons.ini` are stored for each run.


## Compiling and running the examples
- in addition to the Geant4 prerequesits the boost library is required, which can be installed as  `$ sudo apt install libboost-all-dev`.

**Geant4**
  - requires Geant4 v11.0.0 (or similar new version)
  - follow install instructions on https://geant4-userdoc.web.cern.ch/UsersGuides/InstallationGuide/html/installguide.html#buildandinstall
  - summary of install and run process with useful configuration for this repo can be found at `installG4`

**EcoMug**
  - aleady included in this repo (but github repo at https://github.com/dr4kan/EcoMug)

## EcoMug`(Efficiant Cosmic Muon Generator for cosmic-ray muon applications)**
- https://www.sciencedirect.com/science/article/abs/pii/S0168900221007178
- header only lib, easy to use cosmic muon generator, developed sepcifically for muography applications
- Options changeable by user
  - different generation geometries: flat sky(plane), half sphere, cylinder
  - minimum/maximum momentum
  - minimum/maximum zenith angle $\theta$
  - minimum/maximum azimuth angle $\phi$
      
## Analysis
  - analysis with root (https://root.cern/install/), using version 6.26/04
  - can be installed via `$sudo snap install root-framework`


    
  
