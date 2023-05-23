
// This class is a class derived from G4VUserPrimaryGeneratorAction for 
// constructing the process used to generate incident particles.

#ifndef PrimaryGeneratorAction_h
#define PrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ThreeVector.hh"
#include "G4DataVector.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleGun.hh"
#include "Randomize.hh"
#include "globals.hh"

#include "G4AnalysisManager.hh"
#include "EcoMug.h"

class G4Event;

namespace muonsBunker{
  class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
  {
    public:
      PrimaryGeneratorAction();
      ~PrimaryGeneratorAction();
      
    public:
      void GeneratePrimaries(G4Event* anEvent);
  
    private:
      G4ParticleTable* fparticleTable;
      G4ParticleGun* fparticleGun = nullptr;
      EcoMug* fmuonGen;
      G4ParticleDefinition* fparticleMu = nullptr;
      G4ParticleDefinition* fparticleAntiMu = nullptr;
  };
}
#endif
