#include "Run.hh"

#include "DetectorConstruction.hh"
#include "PrimaryGeneratorAction.hh"

#include "G4Material.hh"

namespace cosmicMuonsEcoMug {

Run::Run(DetectorConstruction* det, G4VUserPrimaryGeneratorAction* prim)
:   G4Run(),
    fDetector(det),
    fPrimary(prim) {
    // init data members
    //fNumScatterFin = 0;
}


Run::~Run() {
  // free the memoery that was allocated dynamically (i.e. our histogram object)
  // delete fEdepHistogram;
}

// This method will be called by the master RunManager by passing each theard
// local run (Run) pointers as argument. Therefore, here you define how to
// merge each thread local Run global data structures (all thread local Run-s
// will be merged to the master Run data).

// Called only for the Master's RunAction at the end after all thread local runs
// have been merged.
void  Run::EndOfRunSummary() {
    // Get number of events simulated from the base G4Run object (number of
    // events processed by each worker thread has already been merged at the end
    // of the Merge method when calling the base class Merge method).
    const G4int nEvt = GetNumberOfEvent();
    if (nEvt == 0) return;
    
    //
    //
    // Print out the information
    G4String primInfo = std::to_string( nEvt ) + " E = ";
    //primInfo += fPrimary->GetParticleName() + "\n";

    G4cout<< "\n\n" << G4endl;
    G4cout<< "\n =================================  Run Summary ================================= \n"
       //   << " Collected Hits  : " << fNumScatterFin<< " \n"
          << " ================================================================================ \n"
          << G4endl;
    G4cout<< "\n\n" << G4endl;

    // write the energy deposit histogram to file (ask normalization i.e. 'true')
    //if (fIsActiveEdepHistogram) {
    //  fEdepHistogram->WriteToFile(true);
    //}

}

}