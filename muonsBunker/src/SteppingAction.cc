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
/// \file SteppingAction.cc
/// \brief Implementation of the B1::SteppingAction class

#include "SteppingAction.hh"
#include "EventAction.hh"
#include "DetectorConstruction.hh"
#include "G4VSolid.hh"

#include "G4Step.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4LogicalVolume.hh"
#include "G4SystemOfUnits.hh"

namespace cosmicMuonsEcoMug
{

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::SteppingAction(EventAction* eventAction, DetectorConstruction* det)
: fEventAction(eventAction),
  fDetector(det)
{
  fxExt = *(fDetector->GetXExtend());
  fyExt = *(fDetector->GetYExtend());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::~SteppingAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SteppingAction::UserSteppingAction(const G4Step* theStep)
{
  bool includeCheck = true;
 
  if(includeCheck){
    if (theStep->IsFirstStepInVolume()){
      bool killUseless = true;
  //   G4int et = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
      G4ThreeVector muonPos = theStep->GetPreStepPoint()->GetPosition();
      if(muonPos(2) < 0.)
        theStep->GetTrack()->SetTrackStatus(fStopAndKill);
      G4ThreeVector centerDet(0.,0.,0.);
      G4double r_kill = 12.*m;//7.5*m; // half width of bunker, do not check particle closer to det than this distance anymore

      G4double distance = muonPos.diff2(centerDet);

      if(killUseless && (distance > r_kill)){
        const G4String pName = theStep->GetTrack()->GetParticleDefinition()->GetParticleName();
        if ( pName == "mu+" ||  pName == "mu-"){
          //G4int et = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
  
          // check if particle track potentially hits detector area, if not kill it
          G4ThreeVector muonDir = theStep->GetTrack()->GetMomentumDirection();
  
          // calculate point of intersection of trajectory and x-y plane
          G4double temp = -muonPos(2)/muonDir(2);
          G4double  x_hit = muonPos(0) + temp*muonDir(0);
          G4double  y_hit = muonPos(1) + temp*muonDir(1);

  //         G4double xExt = *(fDetector->GetXExtend());
  //         G4double yExt = *(fDetector->GetYExtend());
      
          G4double epsilon = 0.5*m;
  
          if((fxExt + epsilon) - std::abs(x_hit) < 0 || (fyExt + epsilon) - std::abs(y_hit) < 0)
            theStep->GetTrack()->SetTrackStatus(fStopAndKill);
        }
      else
        theStep->GetTrack()->SetTrackStatus(fStopAndKill);
      }
    }
  }
  
//   G4VPhysicalVolume* vol = theStep->GetPreStepPoint()->GetTouchableHandle()->GetVolume();
  
  //std::cout << "Event: " <<et << " volume " << vol->GetName() << std::endl;
  
  //G4int hits = 0;   // false
  // if(...){          // true if second detector is also hit by this particle
  //   hits = 1;
  // }

  G4double counter = 0.;
  // Score particles inside detector: i.e. pre-step point was in target
  if (theStep->GetPreStepPoint()->GetTouchableHandle()->GetVolume()
    != fDetector->GetDetPhysicalVolume() && theStep->GetPostStepPoint()->GetTouchableHandle()->GetVolume()
    == fDetector->GetDetPhysicalVolume()) { 
      
      // Get the G4Track and the ParticleDefinition to see if the particle is muon
      const G4ParticleDefinition* pDef = theStep->GetTrack()->GetParticleDefinition();
        if ( pDef->GetParticleName() == "mu+" ||  pDef->GetParticleName() == "mu-") {
          ++counter;  
          
          // get angles of muons that hit detector
          G4VSolid *detSolid = theStep->GetPostStepPoint()->GetTouchableHandle()->GetSolid();
          //const G4ThreeVector&
          G4ThreeVector normDet = -detSolid->SurfaceNormal(theStep->GetPreStepPoint()->GetPosition());
          G4ThreeVector muonDir = theStep->GetTrack()->GetMomentumDirection();

          //get angle       
          //0! G4double deltaPhi = normDet.azimAngle(muonDir,normDet);
          G4ThreeVector zAxis(0.,0.,1.);
          G4double phi;
          if(zAxis.isParallel(normDet))
            phi = muonDir.getPhi();
          else
            phi = muonDir.azimAngle(zAxis, normDet);
            // azimAnlge(v2, ref) -> projected on plane perp to ref direction
            // no option to only set refDir, not v2, so take zAxis as v2

          // G4double phiDet = normDet.getPhi(); 
          // G4double phiMuon = muonDir.getPhi();
          // G4double deltaPhi2 = phiDet -phiMuon;
          // G4double deltaPhi3 = normDet.azimAngle(muonDir); //correc tonly if detector an z are parallel 
        
          // std::cout << "------------ phi ----------------" << std::endl;
          // std::cout << "Phi = "<< phi << std::endl;
          // std::cout << "DeltaPhi2 = "<< deltaPhi2 << std::endl;
          // std::cout << "DeltaPhi3 = "<< deltaPhi3 << std::endl;

          G4double deltaTheta = normDet.polarAngle(muonDir, normDet);
          // G4double thetaDet = normDet.getTheta();
          // G4double thetaMuon = muonDir.getTheta();
          // G4double deltaTheta2 = thetaDet - thetaMuon;

          // std::cout << "------------ theta ----------------" << std::endl;
          // std::cout << "DeltaTheta1 = "<< deltaTheta << std::endl;
                    
          // G4double angle = normDet.angle(muonDir);

          // std::cout << "------------ angle ----------------" << std::endl;
          // std::cout << "Angle = "<< angle << std::endl;   

          // position of particle entering detector
          
          G4ThreeVector posDet = theStep->GetPostStepPoint()->GetPosition();
          G4double Ekin = theStep->GetPreStepPoint()->GetKineticEnergy();
          // std::cout << "--------------Controll OUTPUT------------"<< std::endl;
          // std::cout << "Detector Position: ("<< posDet[0]/m << "," << posDet[1]/m << "," << posDet[2]/m << ")" << std::endl;
          G4int evt = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
          G4AnalysisManager *man = G4AnalysisManager::Instance();
          man->FillNtupleIColumn(0, evt);
          man->FillNtupleDColumn(1,posDet[0]/m);            
          man->FillNtupleDColumn(2,posDet[1]/m);  
          man->FillNtupleDColumn(3,posDet[2]/m);
          man->FillNtupleDColumn(4,deltaTheta);            
          man->FillNtupleDColumn(5,phi);  
          man->FillNtupleDColumn(6,Ekin/GeV);  
          man->AddNtupleRow(0); 

          // kill particle afterwards, as its of no use anymore
          theStep->GetTrack()->SetTrackStatus(fStopAndKill);
       }
        //fEventAction->AddNumberScatterPerStep( counter );
    }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

 }

}
