#ifndef MUONCORRECTION_H
#define MUONCORRECTION_H

#include "TLorentzVector.h"

class MuonCorrection {
public:
  virtual ~MuonCorrection() = default;

  virtual TLorentzVector Apply(const TLorentzVector& fMu) = 0;
};

#endif
