#ifndef WRAP_NTROADBANKLOOKUP_HPP
#define WRAP_NTROADBANKLOOKUP_HPP

#include <boost/unordered_map.hpp>
#include <boost/shared_ptr.hpp>
#include "TrigFTKAna/AnaRoad.hpp"
#include "TrigFTKAna/AnaSuperstrip.hpp"

typedef boost::unordered_map<AnaRoad::RoadID, boost::shared_ptr<const AnaRoad> > NtRoadLookup;
typedef boost::unordered_map<AnaRoad::BankID,NtRoadLookup> NtRoadBankLookup;
typedef boost::unordered_map<AnaSuperstrip::SuperstripID, boost::shared_ptr<const AnaSuperstrip> > NtRoadSuperstripLookup;
typedef boost::unordered_map<unsigned int,NtRoadSuperstripLookup> NtRoadLayerLookup;

#endif // WRAP_NTROADBANKLOOKUP_HPP
