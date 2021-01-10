#include "mapGen.h"

// using namespace std;
// using namespace grid_map;
using namespace mapgen;

shared_ptr<GridMap> mapgen::generateMapType(int width, int height, float res, int type, Position& start) {
  GridMap map;
  map.setGeometry(Length(height, width), res);
  bool startSet = false;
  switch(type){
  case 0:{ // empty
    map.add("obstacle", 0);
    assertm(map.getPosition(Index(0,0), start), "Cannot get position by index while map generation type 0");
    break;
  }
  case 1:{ // bounds
    map.add("obstacle", 1);
    int h_offset = static_cast<int>(map.getSize()(0)*0.1);
    int w_offset = static_cast<int>(map.getSize()(1)*0.1);


    Index submapStartIndex(h_offset, w_offset);
    Index submapBufferSize(
			   static_cast<int>(map.getSize()(0) -2*h_offset),
			   static_cast<int>(map.getSize()(1) - 2*w_offset)
			   );


    for (grid_map::SubmapIterator iterator(map,submapStartIndex, submapBufferSize);
      !iterator.isPastEnd(); ++iterator) {
      if(!startSet){
	startSet = true;
	assertm(map.getPosition(*iterator, start), "Cannot get position by index while map generation type 1");
      }
      map.at("obstacle", *iterator) = 0;

    }
    break;
  }
  // case 2:{ // Center square
  //   break;
  // }
  default:{
    warn("Unkown map type -- use empty map");
    map.add("obstacle", 0);
  }
  }

  return make_shared<GridMap>(map);
}

shared_ptr<GridMap> mapgen::changeMapRes(shared_ptr<GridMap> gmap, float res) {
  GridMap nmap;
  float oldRes = gmap->getResolution();
  debug("Old res: ", oldRes);
  GridMapCvProcessing::changeResolution(*gmap, nmap, res);
  debug("New Res: ", nmap.getResolution());
  return make_shared<GridMap>(nmap);
}

void mapgen::drawPathOnMap(shared_ptr<GridMap> gmap, vector<Position>& path, bool inc) {

  // Two stages:
  // 1. Draw coverage pixel line, use the inc parameter for that -> inc == true
  // 2. Draw black line -> inc == false

  for (auto nWP = next(path.begin(),1); nWP != path.end(); ++nWP) {
    auto pWP = prev(nWP, 1);
    for (LineIterator it(*gmap, *pWP, *nWP);  !it.isPastEnd(); ++it) {
      gmap->at("map", *it) = inc ? gmap->at("map", *it) + 1 : 0;
      debug("Draw: ", *it);
    }
  }
}
