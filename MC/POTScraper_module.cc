////////////////////////////////////////////////////////////////////////
// Class:       POTScraper
// Plugin Type: analyzer (Unknown Unknown)
// File:        POTScraper_module.cc
//
// Generated at Thu Jan  9 17:02:29 2025 by Alexander Antonakis using cetskelgen
// from cetlib version 3.18.02.
////////////////////////////////////////////////////////////////////////

#include "art/Framework/Core/EDAnalyzer.h"
#include "art/Framework/Core/ModuleMacros.h"
#include "art/Framework/Principal/Event.h"
#include "art/Framework/Principal/Handle.h"
#include "art/Framework/Principal/Run.h"
#include "art/Framework/Principal/SubRun.h"
#include "canvas/Utilities/InputTag.h"
#include "fhiclcpp/ParameterSet.h"
#include "messagefacility/MessageLogger/MessageLogger.h"

#include "art/Framework/Core/EDProducer.h"
#include "art/Framework/Core/ModuleMacros.h"
#include "art/Framework/Principal/Event.h"
#include "art/Framework/Principal/Handle.h"
#include "canvas/Persistency/Common/Ptr.h"
#include "art/Framework/Principal/Run.h"
#include "art/Framework/Principal/SubRun.h"
#include "canvas/Utilities/InputTag.h"
#include "fhiclcpp/ParameterSet.h"
#include "messagefacility/MessageLogger/MessageLogger.h"

#include "larcoreobj/SummaryData/POTSummary.h"

#include "sbndaq-artdaq-core/Overlays/Common/CAENV1730Fragment.hh"
#include "artdaq-core/Data/Fragment.hh"
#include "sbndaq-artdaq-core/Overlays/FragmentType.hh"
#include "artdaq-core/Data/ContainerFragment.hh"
#include "sbnobj/SBND/Timing/DAQTimestamp.hh"

#include "sbndaq-artdaq-core/Overlays/SBND/PTBFragment.hh"
#include "sbndcode/Decoders/PTB/sbndptb.h"

#include "art_root_io/TFileService.h"
#include "TH1D.h"
#include "TNtuple.h"

#include "lardataobj/RawData/OpDetWaveform.h"


// include the object we care about
#include "sbndaq-artdaq-core/Obj/SBND/pmtSoftwareTrigger.hh"


//using namespace sbnd::trigger;

class POTScraper;

/*

// information from the subrun
struct SubRunData_t {
  
  SubRunData_t() { Clear(); }
  void Clear() { pot = -99999.; }
  Double_t pot; //protons on target
  Double_t goodpot; // good pots
  Int_t spills;
  Int_t goodspills;

}; // struct SubRunData_t

*/

class POTScraper : public art::EDAnalyzer {
public:
  explicit POTScraper(fhicl::ParameterSet const& p);
  // The compiler-generated destructor is fine for non-base
  // classes without bare pointers or other resource use.

  // Plugins should not be copied or assigned.
  POTScraper(POTScraper const&) = delete;
  POTScraper(POTScraper&&) = delete;
  POTScraper& operator=(POTScraper const&) = delete;
  POTScraper& operator=(POTScraper&&) = delete;

  // Required functions.
  void analyze(art::Event const& e) override;
  void beginSubRun(const art::SubRun& sr);
  void beginJob() override;

private:

  // Declare member data here.

  //Create a SubRun Tree
  TNtuple *fSubRunTree;

  //SubRunData_t SubRunData;

  unsigned int fEventID;
  unsigned int fRun;
  unsigned int fSubRun;

  //Double_t totPOT = 0.;
  //Double_t totGoodPOT = 0.;
  //Int_t totSpills = 0;
  //Int_t goodSpills = 0;

  
  const std::string fPOTModuleLabel;

};


POTScraper::POTScraper(fhicl::ParameterSet const& p)
  : EDAnalyzer{p},  // ,
  // More initializers here.
  fPOTModuleLabel(p.get<std::string>("POTModuleLabel"))

{
  // Call appropriate consumes<>() for any products to be retrieved by this module.
}


void POTScraper::beginSubRun(const art::SubRun& sr)
{
  std::cout << "In the beginSubRun Function" << std::endl;
  art::Handle< sumdata::POTSummary > potListHandle;
  //sr.getByLabel(fPOTModuleLabel,potListHandle);
  
  if(sr.getByLabel(fPOTModuleLabel,potListHandle)) {
    //SubRunData.pot=potListHandle->totpot;
    //SubRunData.goodpot=potListHandle->totgoodpot;
    //totPOT += potListHandle->totpot;
    //totGoodPOT += potListHandle->totgoodpot;
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << "sr.run() " << sr.run() << " sr.subRun() " << sr.subRun() << " sr.id() " << sr.id() << std::endl;
    std::cout << "POT in SubRun " << potListHandle->totpot << std::endl;
    std::cout << "Good POT in SubRun " << potListHandle->totgoodpot << std::endl;
    std::cout << "Spills in SubRun " << potListHandle->totspills << std::endl;
    std::cout << "Good Spills in SubRun " << potListHandle->goodspills << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
    fSubRunTree->Fill(
                      sr.run(),
                      sr.subRun(),
                      potListHandle->totpot,
                      potListHandle->totgoodpot,
                      potListHandle->totspills,
                      potListHandle->goodspills
                     );

  }
  else {
    std::cout << "Failed to get subrun data!" << std::endl;
    //SubRunData.pot=0.;
  }
} // End of beginSubRun function




void POTScraper::analyze(art::Event const& e)
{
  // Implementation of required member function here.


  fEventID = e.id().event();
  std::cout << "Processing Event: " << fEventID << std::endl;  

  fRun = e.run();
  fSubRun = e.subRun();

  std::cout << "Processing Run " << fRun << std::endl;
  std::cout << "Processing SubRun " << fSubRun << std::endl;


}

void POTScraper::beginJob()
{
  art::ServiceHandle<art::TFileService> tfs;

  fSubRunTree = tfs->make<TNtuple>("subrun_tree", "subrun_tree", "run:subrun:pot:goodpot:spills:goodspills");  

}



DEFINE_ART_MODULE(POTScraper)
