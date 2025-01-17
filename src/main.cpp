#include "../src/optimizer/optimizer.h"

using namespace op;
using namespace path;
namespace fs = std::filesystem;


int main(int argc, char *argv[])
{

  string path;
  // debug(argc);
  if (argc == 2){
    path = argv[1];
  }else{
    warn("Wrong number of arguments, expected path to config!");
    return 1;
  }

  executionConfig eConf(path);
  op::Optimizer opti(
		     make_shared<op::InitStrategy>(op::InitStrategy()),
		     make_shared<op::SelectionStrategy>(op::SelectionStrategy()),
		     make_shared<op::DualPointCrossover>(op::DualPointCrossover()),
		     make_shared<op::MutationStrategy>(op::MutationStrategy()),
		     make_shared<op::FitnessStrategy>(op::FitnessStrategy()),
		     eConf);
  if(eConf.scenario == 0)  // elitist selection
    opti.optimizePath(eConf.printInfo);
  else
    opti.optimizePath_Turn_RWS(eConf.printInfo);



  if(eConf.retrain){
    mapgen::emulateCoveredMapSegment(opti.eConf.gmap, eConf.start);
    opti.eConf.maxIterations = eConf.retrain;
    if(eConf.scenario == 0)  // elitist selection
      opti.optimizePath(eConf.printInfo);
    else
      opti.optimizePath_Turn_RWS(eConf.printInfo);

  }


  return 0;
}
