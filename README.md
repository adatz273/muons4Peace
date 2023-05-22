# muons

**Compiling the examples**
  - requires Geant4 v11.0.0
  - CRY current version (https://nuclear.llnl.gov/simulation/) for `cosmicMuons` and `cosmicMuonsEcoMug`
  - the EcoMug library (https://github.com/dr4kan/EcoMug) for `cosmicMuonsEcoMug`
  
**Running the examples**
  - set up environment for Geant4 (and CRY) (e.g. see `geant4CRY.sh`)
  - for CRY
    - CRY needs to be updated before run, e.g. with /control/execute cmd.file  
    
**CRY library (Cosmic Ray Shower Library)**
https://nuclear.llnl.gov/simulation/doc_cry_v1.7/cry.pdf
 - Options changeable by user
      - type of particle types that are returned
      - altitude, allowed values = [0 m, 2100 m, 11300 m]
      - latitude, between (-90,90)
      - date (to include effect of solar acitivity)
      - max n of particles in a shower
      - subbox Length, l[m] = [1, 3, 10, 30, 100, 300], in between larger box is used and particles outside neglected
  
  **EcoMug (Efficiant Cosmic Muon Generator for cosmic-ray muon applications)**
  https://www.sciencedirect.com/science/article/abs/pii/S0168900221007178
   - Options changeable by user
      - different generation geometries: flat sky(plane), half sphere, cylinder
      - minimum/maximum momentum
      - minimum/maximum zenith angle $\theta$
      - minimum/maximum azimuth angle $\phi$
      
 **Analysis**
  - analysis with root (https://root.cern/install/), using version 6.26/04
  
  **Boost Library**
  https://www.boost.org/
  - for file handling the property tree of the boost library is utilized
    
  
