#ifndef SEALIB_RANKSELECT_H
#define SEALIB_RANKSELECT_H

#include <sealib/rankstructure.h>
namespace Sealib {
/**
* Space efficient RankSelect implementation.
* Extends the RankStructure with the select function.
* @author Johannes Meintrup
*/
class RankSelect : public RankStructure {
public:
 virtual unsigned long select(unsigned long k) const = 0;
};
}
#endif //SEALIB_RANKSELECT_H
