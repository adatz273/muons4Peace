//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
/// \file PrimaryGeneratorActionGun.cc
/// \brief Implementation of the B1::PrimaryGeneratorActionGun class

#include "PrimaryGeneratorActionGun.hh"

#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4RunManager.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"
#include "FileManager.hh"

namespace cosmicMuonsEcoMug
{

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorActionGun::PrimaryGeneratorActionGun()
{
  G4int n_particle = 1;
  fParticleGun  = new G4ParticleGun(n_particle);

  FileManager& fileman = FileManager::GetFileManager();
  pt::ptree ptree = fileman.GetPropTree();
  //--------EcoMug--------------- 
  //fmuonGen->SetSeed(1234); // for reproduceability
  //read the following from file to enable flexibility;

  G4double px = std::stod(ptree.get<std::string>("gun.px"));
  G4double py = std::stod(ptree.get<std::string>("gun.py"));
  G4double pz = std::stod(ptree.get<std::string>("gun.pz"));


  // default particle kinematic
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4String particleName;
  G4ParticleDefinition* particle
    = particleTable->FindParticle(particleName="mu-");
  fParticleGun->SetParticleDefinition(particle);
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(px,py,pz));
  fParticleGun->SetParticleEnergy(4.*GeV);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorActionGun::~PrimaryGeneratorActionGun()
{
  delete fParticleGun;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PrimaryGeneratorActionGun::GeneratePrimaries(G4Event* anEvent)
{
  FileManager& fileman = FileManager::GetFileManager();
  pt::ptree ptree = fileman.GetPropTree();

  G4double x0 = std::stod(ptree.get<std::string>("gun.x"))*m;
  G4double y0 = std::stod(ptree.get<std::string>("gun.y"))*m;
  G4double z0 = std::stod(ptree.get<std::string>("gun.z"))*m;

  fParticleGun->SetParticlePosition(G4ThreeVector(x0,y0,z0));
  fParticleGun->GeneratePrimaryVertex(anEvent);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

}


