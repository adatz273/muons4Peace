//******************************************************************************
// PrimaryGeneratorAction.cc
//
// 1.00 JMV, LLNL, Jan-2007:  First version.
//******************************************************************************
//

#include <iomanip>
#include "PrimaryGeneratorAction.hh"
#include "G4SystemOfUnits.hh"
#include "G4RunManager.hh"

#include "FileManager.hh"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

namespace pt = boost::property_tree;
using namespace std;

#include "G4Event.hh"

//----------------------------------------------------------------------------//
//
PrimaryGeneratorAction::PrimaryGeneratorAction()
{
  // define a particle gun
  fparticleGun = new G4ParticleGun();
  fmuonGen = new EcoMug();
  
  FileManager& fileman = FileManager::GetFileManager();
  pt::ptree ptree = fileman.GetPropTree();
  //--------EcoMug--------------- 
  //fmuonGen->SetSeed(1234); // for reproduceability
  //read the following from file to enable flexibility;

  G4int geometry = std::stod(ptree.get<std::string>("ecomug.geometry"));
  G4double centerX = std::stod(ptree.get<std::string>("ecomug.centerX"));
  G4double centerY = std::stod(ptree.get<std::string>("ecomug.centerY"));
  G4double centerZ = std::stod(ptree.get<std::string>("ecomug.centerZ"));
  
  G4double skySizeX, skySizeY, radius;
  switch (geometry){ 
  case 1:
    fmuonGen->SetUseSky(); // plane surface generation
    skySizeX = std::stod(ptree.get<std::string>("sky.sizeX"));
    skySizeY = std::stod(ptree.get<std::string>("sky.sizeY"));

    fmuonGen->SetSkySize({{skySizeX, skySizeY}}); // x and y size of the plane
    fmuonGen->SetSkyCenterPosition({{centerX, centerY, centerZ}}); // (x,y,z) position of the center of the plane
    fmuonGen->SetMaximumTheta(0.12); //~7 degree
    break;

  case 2:
    fmuonGen->SetUseHSphere();
    radius = std::stod(ptree.get<std::string>("halfSphere.radius"));
    fmuonGen->SetHSphereRadius(radius);
    fmuonGen->SetHSphereCenterPosition({{centerX, centerY, centerZ}});
//    fmuonGen->SetMaximumTheta(M_PI/18.);
    break;

  default:
    std::cout << "---Problem with EcoMugGeometry!---" <<std::endl;
    break;
  }

  // MT
  G4String particleName;
  fparticleAntiMu = fparticleTable->FindParticle(particleName="mu-");
  fparticleMu = fparticleTable->FindParticle(particleName="mu+");
}

//----------------------------------------------------------------------------//
PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete fparticleGun;
  delete fmuonGen;
}


//----------------------------------------------------------------------------//
void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{ 
  // G4String particleName;
  G4ParticleDefinition* particle;
  fmuonGen->Generate();
  
  // Generate Muon Properties
  std::array<double, 3> muon_pos = fmuonGen->GetGenerationPosition();
  G4double muon_ptot = fmuonGen->GetGenerationMomentum();
  G4double muon_theta = fmuonGen->GetGenerationTheta();
  G4double muon_phi = fmuonGen->GetGenerationPhi();
  G4double muon_charge = fmuonGen->GetCharge();

  if(muon_charge > 0)
    particle = fparticleMu;
  else
    particle = fparticleAntiMu;

  // pass information to particle gun  
  fparticleGun->SetParticleDefinition(particle);
  fparticleGun->SetParticlePosition(G4ThreeVector(muon_pos[0]*m,muon_pos[1]*m, muon_pos[2]*m));
  fparticleGun->SetParticleMomentum(G4ParticleMomentum(
    muon_ptot*sin(muon_theta)*cos(muon_phi)*GeV,
    muon_ptot*sin(muon_theta)*sin(muon_phi)*GeV,
    muon_ptot*cos(muon_theta)*GeV
  )); //from spherical to cartesian coodinates


  fparticleGun->GeneratePrimaryVertex(anEvent);
}
