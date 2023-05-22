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
/// \file RunAction.cc
/// \brief Implementation of the B1::RunAction class

#include "RunAction.hh"
#include "PrimaryGeneratorAction.hh"
#include "DetectorConstruction.hh"
#include "FileManager.hh"

#include <algorithm>
#include <string>
#include <ctime>  
#include <chrono>
#include <iostream>

// #include "Run.hh"

#include "G4RunManager.hh"
#include "G4Run.hh"
#include "G4AccumulableManager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"



namespace cosmicMuonsEcoMug
{

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::RunAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::~RunAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::BeginOfRunAction(const G4Run* aRun)
{
  // inform the runManager to save random number seed
 
  G4RunManager::GetRunManager()->SetRandomNumberStore(false);

  // get name of directory to store results
  FileManager& fileman = FileManager::GetFileManager();
  pt::ptree ptree = fileman.GetPropTree();

  std::string name = ptree.get<std::string>("output.foldername");
  std::string path = ptree.get<std::string>("output.pathname");
  std::string pathname = path+name;
  
  
  std::mutex mtx;
  G4bool write = true;
  mtx.lock();
  if(write){
    G4int eventsInRun = aRun->GetNumberOfEventToBeProcessed();
    fileman.AddValuePropTree("input.nparticles", eventsInRun);
    write = false;
  }
  mtx.unlock();

  // reset accumulables to their initial values
  //G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
  //accumulableManager->Reset();
  

  

  // create root File to store results add date stamp to ensure no file is overwritten
  G4AnalysisManager *man = G4AnalysisManager::Instance();
  man->SetNtupleMerging(true);

  auto timeNow = std::chrono::system_clock::now();
  std::time_t now_time = std::chrono::system_clock::to_time_t(timeNow);
  std::string dateStamp = std::ctime(&now_time);
  dateStamp = dateStamp.substr(0,13) + dateStamp.substr(14,2);
  dateStamp.erase(std::remove_if(dateStamp.begin(), dateStamp.end(), isspace), dateStamp.end());
  std::string filename = pathname +"/results" + dateStamp+".root";
  
  man->OpenFile(filename);
  man->CreateNtuple("Hits","hits");
  man->CreateNtupleIColumn("fEvent");
  man->CreateNtupleDColumn("fx");
  man->CreateNtupleDColumn("fy"); 
  man->CreateNtupleDColumn("fz");
  man->CreateNtupleDColumn("theta");
  man->CreateNtupleDColumn("phi"); 
  man->CreateNtupleDColumn("Ekin");   //kinetic energy when entering detector  
  man->FinishNtuple(0);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::EndOfRunAction(const G4Run* run)
{
  G4int nofEvents = run->GetNumberOfEvent();
  if (nofEvents == 0) return;

  // Merge accumulables
  // G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
  // accumulableManager->Merge();

  // Compute dose = total energy deposit in a run and its variance
  //
  //G4double edep  = fEdep.GetValue();
  //G4double edep2 = fEdep2.GetValue();

  //G4double rms = edep2 - edep*edep/nofEvents;
  //if (rms > 0.) rms = std::sqrt(rms); else rms = 0.;

  const DetectorConstruction* detConstruction
   = static_cast<const DetectorConstruction*>
     (G4RunManager::GetRunManager()->GetUserDetectorConstruction());
  //G4double mass = detConstruction->GetScoringVolume()->GetMass();
  //G4double dose = edep/mass;
  //G4double rmsDose = rms/mass;

  // Run conditions
  //  note: There is no primary generator action object for "master"
  //        run manager for multi-threaded mode.
  const PrimaryGeneratorAction* generatorAction
   = static_cast<const PrimaryGeneratorAction*>
     (G4RunManager::GetRunManager()->GetUserPrimaryGeneratorAction());
  G4String runCondition;
  if (generatorAction)
  {
    //const G4ParticleGun* particleGun = generatorAction->GetParticleGun();
    //runCondition += particleGun->GetParticleDefinition()->GetParticleName();
    //runCondition += " of ";
    //G4double particleEnergy = particleGun->GetParticleEnergy();
    //runCondition += G4BestUnit(particleEnergy,"Energy");
  }

     // write and close root file
     G4AnalysisManager *man = G4AnalysisManager::Instance();
     man->Write();
     man->CloseFile();
  if (IsMaster()) {
    G4cout
     << G4endl
     << "--------------------End of Global Run-----------------------";
  }
  else {
    G4cout
     << G4endl
     << "--------------------End of Local Run------------------------";
  }

  G4cout
     << G4endl
     //<< " The run consists of " << nofEvents << " "<< runCondition
     << G4endl
     //<< " Cumulated dose per run, in scoring volume : "
     //<< G4BestUnit(dose,"Dose") << " rms = " << G4BestUnit(rmsDose,"Dose")
     << G4endl
     << "------------------------------------------------------------"
     << G4endl
     << G4endl;
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// void RunAction::AddEdep(G4double edep)
// {
//   fEdep  += edep;
//   fEdep2 += edep*edep;
// }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

}
