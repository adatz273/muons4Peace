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
/// \brief Implementation of the RunAction class
#include "G4RunManager.hh"
#include "G4AccumulableManager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4Run.hh"

#include "RunAction.hh"
#include "PrimaryGeneratorAction.hh"
#include "DetectorConstruction.hh"
#include "FileManager.hh"

#include <algorithm>
#include <string>
#include <ctime>  
#include <chrono>
#include <iostream>


namespace muonsBunker
{

  RunAction::RunAction()
  {}
  
  RunAction::~RunAction()
  {}

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
    
    // have one thread write the number of events in the final properties file
    std::mutex mtx;
    G4bool write = true;
    mtx.lock();
    if(write){
      G4int eventsInRun = aRun->GetNumberOfEventToBeProcessed();
      fileman.AddValuePropTree("input.nparticles", eventsInRun);
      write = false;
    }
    mtx.unlock();
  
    // create root File to store results 
    G4AnalysisManager *man = G4AnalysisManager::Instance();
    man->SetNtupleMerging(true);
  
    // store data with a time stamp to ensure they are not overwritten by accident
    auto timeNow = std::chrono::system_clock::now();
    std::time_t now_time = std::chrono::system_clock::to_time_t(timeNow);
    std::string dateStamp = std::ctime(&now_time);
    dateStamp = dateStamp.substr(0,13) + dateStamp.substr(14,2);
    dateStamp.erase(std::remove_if(dateStamp.begin(), dateStamp.end(), isspace), dateStamp.end());
    std::string filename = pathname +"/results" + dateStamp+".root";
    
    man->OpenFile(filename);
    man->CreateNtuple("Hits","hits");
    man->CreateNtupleIColumn("fEvent");
    man->CreateNtupleDColumn("fx");      // x position of hit on detector
    man->CreateNtupleDColumn("fy");      // y position
    man->CreateNtupleDColumn("fz");      // z position 
    man->CreateNtupleDColumn("theta");   // zenith angle 
    man->CreateNtupleDColumn("phi");     // azimuth angle
    man->CreateNtupleDColumn("Ekin");    // kinetic energy 
    man->FinishNtuple(0);

  }
  
  void RunAction::EndOfRunAction(const G4Run* run)
  {
    G4int nofEvents = run->GetNumberOfEvent();
    if (nofEvents == 0) return;

    const DetectorConstruction* detConstruction
     = static_cast<const DetectorConstruction*>
       (G4RunManager::GetRunManager()->GetUserDetectorConstruction());
     
    // Run conditions
    //  note: There is no primary generator action object for "master"
    //        run manager for multi-threaded mode.
    const PrimaryGeneratorAction* generatorAction
     = static_cast<const PrimaryGeneratorAction*>
       (G4RunManager::GetRunManager()->GetUserPrimaryGeneratorAction());
    
       // write and close root file
       G4AnalysisManager *man = G4AnalysisManager::Instance();
       man->Write();
       man->CloseFile();
    if (IsMaster()) {
      G4cout
       << G4endl
       << "--------------------End of Global Run-----------------------"
       << G4endl;
    }
    else {
      G4cout
       << G4endl
       << "--------------------End of Local Run------------------------"
       << G4endl;
    }
  }
}
  