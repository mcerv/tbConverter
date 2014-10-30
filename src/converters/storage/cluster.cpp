#include "cluster.h"

#include <cassert>
#include <vector>
#include <iostream>

#include "hit.h"
#include "track.h"
#include "plane.h"

using std::cout;
using std::endl;

namespace Storage {

void Cluster::print()
{
  cout << "\nCluster:\n"
       << "  Address: " << this << "\n"
       << "  Pix: (" << getPixX() << " , " << getPixY() << ")\n"
       << "  Pix err: (" << getPixErrX() << " , " << getPixErrY() << ")\n"
       << "  Pos: (" << getPosX() << " , " << getPosY() << " , " << getPosZ() << ")\n"
       << "  Pos err: (" << getPosErrX() << " , " << getPosErrY() << " , " << getPosErrZ() << ")\n"
       << "  Num hits: " << getNumHits() << "\n"
       << "  Plane: "  << getPlane() << endl;
}

void Cluster::setTrack(Track* track)
{
  assert(!_track && "Cluster: can't use a cluster for more than one track");
  _track = track;
}

Track* Cluster::getTrack() const { return _track; }

void Cluster::addHit(Hit* hit)
{
  hit->setCluster(this);
  _hits.push_back(hit);
  _numHits++;
}

Hit* Cluster::getHit(unsigned int n) const
{
  assert(n < getNumHits() && "Cluster: hit index exceeds vector range");
  return _hits.at(n);
}

Plane* Cluster::getPlane() const { return _plane; }

Cluster::Cluster() :
  _posX(0), _posY(0), _posZ(0), _posErrX(0), _posErrY(0), _posErrZ(0),
  _track(0), _numHits(0), _index(-1), _plane(0)
{ }

}
