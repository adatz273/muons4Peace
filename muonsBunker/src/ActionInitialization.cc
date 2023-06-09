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
/// \file ActionInitialization.cc
/// \brief Implementation of the ActionInitialization class

#include "ActionInitialization.hh"
#include "PrimaryGeneratorAction.hh"
#include "PrimaryGeneratorActionGun.hh"
#include "RunAction.hh"
#include "SteppingAction.hh"
#include "FileManager.hh"

namespace muonsBunker
{

  ActionInitialization::ActionInitialization(DetectorConstruction* det)
  : fDetector(det)
  {}

  ActionInitialization::~ActionInitialization()
  {}

  void ActionInitialization::BuildForMaster() const
  {
    RunAction* runAction = new RunAction();
    SetUserAction(runAction);
  }

  void ActionInitialization::Build() const
  {
    FileManager& fileman = FileManager::GetFileManager();
    pt::ptree ptree = fileman.GetPropTree();
    G4int inputMethod = std::stoi(ptree.get<std::string>("input.method"));  // 1 - "Gun", 2 -"EcoMug", 3 -"CRY"

    switch (inputMethod){
      case 1:
        SetUserAction(new PrimaryGeneratorActionGun());
        break;

      case 2:
        SetUserAction(new PrimaryGeneratorAction());
        break;
    
      default:
        std::cout << "---Problem with PrimaryGeneratorAction Choice!---" <<std::endl;
        break;
    }

    RunAction* runAction = new RunAction();
    SetUserAction(runAction);
    SetUserAction(new SteppingAction(fDetector));
  }

}
