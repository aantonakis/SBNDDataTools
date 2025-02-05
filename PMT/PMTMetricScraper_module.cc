////////////////////////////////////////////////////////////////////////
// Class:       PMTMetricScraper
// Plugin Type: analyzer (Unknown Unknown)
// File:        PMTMetricScraper_module.cc
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


using namespace sbnd::trigger;

class PMTMetricScraper;


class PMTMetricScraper : public art::EDAnalyzer {
public:
  explicit PMTMetricScraper(fhicl::ParameterSet const& p);
  // The compiler-generated destructor is fine for non-base
  // classes without bare pointers or other resource use.

  // Plugins should not be copied or assigned.
  PMTMetricScraper(PMTMetricScraper const&) = delete;
  PMTMetricScraper(PMTMetricScraper&&) = delete;
  PMTMetricScraper& operator=(PMTMetricScraper const&) = delete;
  PMTMetricScraper& operator=(PMTMetricScraper&&) = delete;

  // Required functions.
  void analyze(art::Event const& e) override;
  void beginJob() override;

private:

  // Declare member data here.

  unsigned int fEventID;
  unsigned int fRun;
  unsigned int fSubRun;

  TNtuple *fPMT_tree;


};


PMTMetricScraper::PMTMetricScraper(fhicl::ParameterSet const& p)
  : EDAnalyzer{p}  // ,
  // More initializers here.
{
  // Call appropriate consumes<>() for any products to be retrieved by this module.
}

void PMTMetricScraper::analyze(art::Event const& e)
{
  // Implementation of required member function here.


  fEventID = e.id().event();
  std::cout << "Processing Event: " << fEventID << std::endl;  

  fRun = e.run();
  fSubRun = e.subRun();

  std::cout << "Processing SubRun " << fSubRun << std::endl;


  // Try to grab the PMT Metric Info for this event
  art::Handle<std::vector<sbnd::trigger::pmtSoftwareTrigger>> pmtMetricHandle;
  std::vector<art::Ptr<sbnd::trigger::pmtSoftwareTrigger>> pmtMetricVec;
  if (e.getByLabel("pmtmetricproducer", pmtMetricHandle))
    art::fill_ptr_vector(pmtMetricVec, pmtMetricHandle);

  // Check if there are any reconstructed particles in this event
  if (pmtMetricVec.empty()) {
   
     std::cout << "empty pmtmetrics in this event :(" << std::endl;
     return;
  }

  for (unsigned int i = 0; i < pmtMetricVec.size(); ++i) {

    //bool foundBeamTrigger; 
    //int nAboveThreshold;
    //int trig_ts;  // relative to beam window start, in ns
    //double promptPE;       // total PE for the 100 ns following trigger time 
    //double prelimPE;       // total PE for the 1 us before trigger time
    //double peakPE;
    //
    art::Ptr<sbnd::trigger::pmtSoftwareTrigger> pmt_trig = pmtMetricVec[i];

    //std::cout << "peak time" << pmt_trig->peaktime << std::endl;
    //double peaktime;
    fPMT_tree->Fill(fRun, 
		    fSubRun, 
		    fEventID, 
		    pmt_trig->peaktime, 
		    pmt_trig->trig_ts, 
		    pmt_trig->peakPE,
		    pmt_trig->promptPE,
		    pmt_trig->prelimPE,
		    pmt_trig->nAboveThreshold,
		    pmt_trig->foundBeamTrigger
		   );

  } // end of loop over pmt metric vector

}

void PMTMetricScraper::beginJob()
{
  art::ServiceHandle<art::TFileService> tfs;

  fPMT_tree = tfs->make<TNtuple>("pmt_metric_tree", "pmt_metric_tree", "run:subrun:evt:t:ts:pe_peak:pe_prompt:pe_prelim:n:beamTrig"); 
    
/*
    // All of the possible metrics
    bool foundBeamTrigger; 
    int nAboveThreshold;
    int trig_ts;  // relative to beam window start, in ns
    double promptPE;       // total PE for the 100 ns following trigger time 
    double prelimPE;       // total PE for the 1 us before trigger time
    double peakPE;
    double peaktime;

*/

}

DEFINE_ART_MODULE(PMTMetricScraper)
