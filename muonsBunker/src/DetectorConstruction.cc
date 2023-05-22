//******************************************************************************
// DetectorConstruction.cc
//
// 1.00 JMV, LLNL, MAR-2002:  First version.
//******************************************************************************
//
#include "DetectorConstruction.hh"
#include "FileManager.hh"

#include "G4Element.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4MaterialTable.hh"
#include "G4Box.hh"
#include "G4Sphere.hh"
#include "G4Tubs.hh"
#include "G4Cons.hh"
#include "G4LogicalVolume.hh"
#include "G4RotationMatrix.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4UnionSolid.hh"
#include "G4IntersectionSolid.hh"
#include "G4SystemOfUnits.hh"

#include "globals.hh"
//----------------
#include "G4GenericTrap.hh"
#include "G4UnionSolid.hh"
//#include "TrackerHit.hh"
//#include "TrackerSD.hh"
#include "G4VisAttributes.hh"
#include "G4SDManager.hh"
#include "G4EllipticalTube.hh"

#include <boost/property_tree/ptree.hpp>
#include <memory>
#include <string>
#include <cmath>
//--------------

namespace cosmicMuonsEcoMug {
DetectorConstruction::DetectorConstruction()
{;}

DetectorConstruction::~DetectorConstruction()
{;}


G4VPhysicalVolume* DetectorConstruction::Construct()
{
  //------------------------------------------------------ materials

  // Air, from PhysicalConstants.h
  //
  G4NistManager* man = G4NistManager::Instance();
  G4Material* matAir = man->FindOrBuildMaterial("G4_AIR");

  // Polyethylene
  //
  //G4Material* matPE  = man->FindOrBuildMaterial("G4_POLYETHYLENE");

  // He-3 detector materials
  //
  //G4Material* matHe3  = new G4Material("He3",  2., 3.*g/mole, 0.00049*g/cm3, kStateGas);

  // Uranium ball material
  //
  //G4Element* elHEU = new G4Element("Highly-enriched Uranium", "HEU", 2);
  //elHEU->AddIsotope(new G4Isotope("U-235", 92, 235, 235.*g/mole), .93);
  //elHEU->AddIsotope(new G4Isotope("U-238", 92, 238, 238.*g/mole), .07);
  //G4Material* matHEU = new G4Material("HEU", 19.1*g/cm3, 1, kStateSolid);
  //matHEU->AddElement(elHEU, 1.00);

  // regular concrete for bunker (from input-buechel-storage.py)
  G4Material* matH  = man->FindOrBuildMaterial("G4_H");
  G4Material* matO  = man->FindOrBuildMaterial("G4_O");
  G4Material* matNA = man->FindOrBuildMaterial("G4_Na");
  G4Material* matAl = man->FindOrBuildMaterial("G4_Al");
  G4Material* matSi = man->FindOrBuildMaterial("G4_Si");
  G4Material* matCa = man->FindOrBuildMaterial("G4_Ca");
  G4Material* matFe = man->FindOrBuildMaterial("G4_Fe"); 
    
  G4double densityConcrete = 2.3*g/cm3;
  G4int ncompsConcrete = 7;
  G4Material* mat_concrete = new G4Material("concrete", densityConcrete, ncompsConcrete);
  mat_concrete->AddMaterial(matH, 0.168038);
  mat_concrete->AddMaterial(matO, 0.563183);
  mat_concrete->AddMaterial(matNA, 0.021365);
  mat_concrete->AddMaterial(matAl, 0.021343);
  mat_concrete->AddMaterial(matSi, 0.203231);
  mat_concrete->AddMaterial(matCa, 0.018595);
  mat_concrete->AddMaterial(matFe, 0.004246);
  
  // steel for frontdoor
  G4Material* matC = man->FindOrBuildMaterial("G4_C"); 

  G4double densitySteel = 7.82*g/cm3;
  G4int ncompsSteel = 2;
  G4Material* mat_steel = new G4Material("steel", densitySteel, ncompsSteel);
  mat_steel->AddMaterial(matC, 0.022831);
  mat_steel->AddMaterial(matFe, 0.977169);

  // steel concrete for walls 
  G4double frac_steel = 300. /(densitySteel/(g/cm3)*1e3);
  G4double frac_concrete = 1. - frac_steel;
  G4double density_wall = frac_concrete*densityConcrete + frac_steel*densitySteel;
  G4int ncompsWall = 2;
  G4Material* matWall = new G4Material("steelconcrete",density_wall, ncompsWall);
  matWall->AddMaterial(mat_concrete, frac_concrete);
  matWall->AddMaterial(mat_steel, frac_steel);

  //------------------------------------- volumes----------------------------------//
  G4bool checkOverlaps = true;
  G4ThreeVector placement(0., 0., 0.);

  G4ThreeVector axisX(1.0,0,0);
  G4ThreeVector axisY(0,1.0,0);
  G4ThreeVector axisZ(0,0,1.0);

  //----------------- World volume, filled with air---------------------//
  // 
  G4Sphere* solidWorld = new G4Sphere("World", 0.0*cm, 22.0*m,
                                               0.0*deg, 360.0*deg,
                                               0.0*deg, 180.0*deg);
  G4LogicalVolume* logicWorld
    = new G4LogicalVolume(solidWorld, matAir, "World");
  G4VPhysicalVolume* physWorld 
    = new G4PVPlacement(0, 
                        G4ThreeVector(0.0, 0.0, 0.0),
                        logicWorld,
			                  "World ",
                         0,
                         false,
                         0);


  //--------------------bunker measures-------------------------------//
  G4double length = 30.78 *m;  
  G4double width  = 15.0  *m;

  G4double pash1 = 1.102 *m;
  G4double pash4 = 1.615 *m;

  G4double pRMin = 9.71 *m;
  G4double pRMax = 10.81*m; 
  
  G4double sidewidth  = (12.68/2.)*m;
  G4double sideheight = 5.072*m;
  

  
  //------------------simple concrete bunker------------------------//
  //-----roof
  G4double heightRoof = (pash1/2.);  //constructors take half
  G4double widthRoof = (width/2.);
  G4double lengthRoof = (length/2.);
  G4double zPosRoof = 3.97*m + heightRoof;

  G4Box* concSolid = new G4Box("concSolid",widthRoof,lengthRoof,heightRoof);
  G4LogicalVolume* concLog = new G4LogicalVolume(concSolid, matWall, "roofLog");
  G4ThreeVector placementconc(0., 0., zPosRoof);

  G4VPhysicalVolume* concPhysical = new G4PVPlacement(0,                       //no rotation
                                                     placementconc,            //at
                                                     concLog,                  //its logical volume
                                                     "Conc",                   //its name
                                                     logicWorld,               //its mother  volume
                                                     false,                    //no boolean operation
                                                     0,                        //copy number
                                                     checkOverlaps);

  // -----sides
  G4TwoVector v0(0*m, 0*m);
  G4TwoVector v1(v0[0], v0[1]+ sideheight);
  G4TwoVector v2(sidewidth+v0[0], v0[1]);
  G4TwoVector v2_2(v0[0]-sidewidth, v0[1]);
  const std::vector<G4TwoVector> vertices =  {v0,v1,v2,v2,v0,v1,v2,v2};
  const std::vector<G4TwoVector> vertices_2 =  {v0,v1,v2_2,v2_2,v0,v1,v2_2,v2_2};

  G4GenericTrap* sideSolid = new G4GenericTrap("bunker side", length/2., vertices);
  G4GenericTrap* sideSolid_2 = new G4GenericTrap("bunker side", length/2., vertices_2);
  
  G4LogicalVolume* sideLog = new G4LogicalVolume(sideSolid, matWall, "wallLog"); 
  G4LogicalVolume* sideLog_2 = new G4LogicalVolume(sideSolid_2, matWall, "wallLog"); 
  
  G4RotationMatrix* rotA = new G4RotationMatrix(axisX, -90*deg);

  G4VPhysicalVolume* sidePhysical1 = new G4PVPlacement(rotA,                           //no rotation
                                                     G4ThreeVector(widthRoof,0.,0.),   //at
                                                     sideLog,                          //its logical volume
                                                     "side1",                          //its name
                                                     logicWorld,                       //its mother  volume
                                                     false,                            //no boolean operation
                                                     0,                                //copy number
                                                     checkOverlaps);

   G4VPhysicalVolume* sidePhysical2 = new G4PVPlacement(rotA,                          //no rotation
                                                     G4ThreeVector(-widthRoof,0.,0.),  //at
                                                     sideLog_2,                        //its logical volume
                                                     "side2",                          //its name
                                                     logicWorld,                       //its mother  volume
                                                     false,                            //no boolean operation
                                                     0,                                //copy number
                                                     checkOverlaps);

  //-----backwall
  G4double backWallthickness = (pash1/2.);
  G4double backWallheight = (3.97/2.)*m;
  G4Box* backSolid = new G4Box("concSolid",widthRoof, backWallthickness,backWallheight);                                                   
  G4LogicalVolume* backLog = new G4LogicalVolume(backSolid, matWall, "backLog");
  G4ThreeVector placementBackWall(0,-lengthRoof+backWallthickness,backWallheight);

  G4VPhysicalVolume* backPhysical = new G4PVPlacement(0,                       //no rotation
                                                     placementBackWall,        //at
                                                     backLog,                  //its logical volume
                                                     "back",                   //its name
                                                     logicWorld,               //its mother  volume
                                                     false,                    //no boolean operation
                                                     0,                        //copy number
                                                     checkOverlaps);

  //-----front door
  G4LogicalVolume* frontLog = new G4LogicalVolume(backSolid, mat_steel, "frontLog");   
  G4double dooroffset = 3.*m;
  G4ThreeVector placementFrontDoor(0,+lengthRoof-dooroffset-backWallthickness,backWallheight);

  G4VPhysicalVolume* frontPhysical = new G4PVPlacement(0,                       //no rotation
                                                     placementFrontDoor,        //at
                                                     frontLog,                  //its logical volume
                                                     "front",                   //its name
                                                     logicWorld,                //its mother  volume
                                                     false,                     //no boolean operation
                                                     0,                         //copy number
                                                     checkOverlaps);


  //----------------------------- counting volume-------------------------------//
  G4double detSide   = 0.5*m;            // length of detection area
  G4double detHeight = 0.1*m;            // actually does not matter 
  G4Box* detSolid = new G4Box("detSolid",detSide, detSide,detHeight);
  G4LogicalVolume* detLog = new G4LogicalVolume(detSolid, mat_concrete, "detLog");
  G4ThreeVector placementDet(0., 0., 0.);
    
  G4VPhysicalVolume* detPhysical = new G4PVPlacement(0,                       //no rotation
                                                     placementDet,            //at 
                                                     detLog,                  //its logical volume
                                                     "Detector",              //its name
                                                     logicWorld,              //its mother  volume
                                                     false,                   //no boolean operation
                                                     0,                       //copy number
                                                     checkOverlaps);          //overlaps checking
 xExt = std::make_shared<G4double>(detSide);
 yExt = std::make_shared<G4double>(detSide);

  // ------------------------------ Pu cube ------------------------------------//
  G4String materialName = "G4_Pu";
  G4Material* matPu = man->FindOrBuildMaterial(materialName);
  G4double cubeX = 0.025*m;
  G4double cubeY = cubeX;
  G4double cubeZ = cubeX;

  G4Box* cubeSolid = new G4Box("cubeSolid",cubeX, cubeY,cubeZ);
  G4LogicalVolume* cubeLog = new G4LogicalVolume(cubeSolid, matPu, "cubeLog");

  G4VPhysicalVolume* cubePhysical = nullptr;
  FileManager& fileman = FileManager::GetFileManager();
  pt::ptree ptree = fileman.GetPropTree();
  G4int use_fM = std::stoi(ptree.get<std::string>("input.fissileMat"));

  if(use_fM){
     cubePhysical = new G4PVPlacement(0,                       //no rotation
                                      G4ThreeVector(0., 0., 0.),  //at (for world: G4ThreeVector(0., 0., 4.52*m))
                                      cubeLog,                 //its logical volume
                                      "cube",                  //its name
                                      concLog,                 //its mother  volume
                                      false,                   //no boolean operation
                                      0,                       //copy number
                                      checkOverlaps);          //overlaps checking
  }
  

  // visualize Eco Mug half sphere of muon generation (only for visualization, do not use during actual simulations)
//   G4double r_outer = 14*m;
//   G4double r_inner = r_outer - 0.05*m;
//   G4double start_phi = 0*deg;
//   G4double delta_phi = 180*deg;
//   G4double start_theta = 0*deg;
//   G4double delta_theta = 180*deg;
//
//   G4RotationMatrix* rotX = new G4RotationMatrix(axisX, -90*deg);
//   G4Sphere* skyHS = new G4Sphere("solidskyHS", r_inner, r_outer, start_phi, delta_phi,start_theta, delta_theta);
//   G4LogicalVolume* skyHSLog = new G4LogicalVolume(skyHS, matO, "skyLog");
//
//    new G4PVPlacement(rotX,                       //no rotation
//                      G4ThreeVector(0., 0., 0.),  //at
//                      skyHSLog,                   //its logical volume
//                      "skyHS",                    //its name
//                      logicWorld,                 //its mother  volume
//                      false,                      //no boolean operation
//                      0,                          //copy number
//                      checkOverlaps);             //overlaps checking

  
  fTargetPhysVol = cubePhysical;
  fDetPhysVol = detPhysical;
  fWorldPhysVol = physWorld;

  // write detector details to prop tree
  fileman.AddValuePropTree("detector.sizeX", detSide/m);
  fileman.AddValuePropTree("detector.sizeY", detSide/m);
  fileman.AddValuePropTree("detector.sizeZ", detHeight/m);
  
  // write fissile mat details to prop tree
  if(use_fM){
    fileman.AddValuePropTree("fissileMat.material", materialName);
    fileman.AddValuePropTree("fissileMat.sizeX", cubeX/m);
    fileman.AddValuePropTree("fissileMat.sizeY", cubeY/m);
    fileman.AddValuePropTree("fissileMat.sizeZ", cubeZ/m);
  }
  else
    fileman.AddValuePropTree("fissileMat.used", "no");

  // Must return pointer to the master physical volume
  return physWorld;
}

const G4VPhysicalVolume* DetectorConstruction::GetDetPhysicalVolume() const{
  return fDetPhysVol;
}

const G4VPhysicalVolume* DetectorConstruction::GetTargetPhysicalVolume() const{
  return fTargetPhysVol;
}

const G4VPhysicalVolume* DetectorConstruction::GetWorldPhysicalVolume() const{
  return fWorldPhysVol;
}

const std::shared_ptr<G4double> DetectorConstruction::GetXExtend(){
  return xExt;
}


const std::shared_ptr<G4double> DetectorConstruction::GetYExtend(){
  return yExt;
}

}
