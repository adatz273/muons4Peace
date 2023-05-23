//******************************************************************************
// DetectorConstruction.hh
//
// 1.00 RMK, LANL, MAR-2002:  First version.
//******************************************************************************
//
#ifndef DetectorConstruction_H
#define DetectorConstruction_H 1

class G4VPhysicalVolume;

#include "G4VUserDetectorConstruction.hh"
#include <memory>

namespace muonsBunker {
  class DetectorConstruction : public G4VUserDetectorConstruction
  {
    public:
      DetectorConstruction();
      ~DetectorConstruction();
      const G4VPhysicalVolume* GetDetPhysicalVolume() const;
      const G4VPhysicalVolume* GetTargetPhysicalVolume() const;
      const G4VPhysicalVolume* GetWorldPhysicalVolume() const;
      const std::shared_ptr<G4double> GetXExtend();
      const std::shared_ptr<G4double> GetYExtend();
            
       G4VPhysicalVolume* Construct();

    private:
      G4VPhysicalVolume* fDetPhysVol = nullptr;
      G4VPhysicalVolume* fTargetPhysVol = nullptr;
      G4VPhysicalVolume* fWorldPhysVol = nullptr;
    
      std::shared_ptr<G4double> xExt = nullptr;
      std::shared_ptr<G4double> yExt = nullptr;
  };
}

#endif
