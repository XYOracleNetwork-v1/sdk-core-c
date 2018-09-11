#ifndef ORIGINCHAIN_H
#include <stdint.h>
#include "crypto.h"
#include "xyobject.h"
typedef struct OriginChainState OriginChainState;

struct OriginChainState {
  struct XYResult* (*newOriginBlock)(OriginChainState* self_OriginChainState, ByteArray* newHash);
  struct XYResult* (*addSigner)(OriginChainState* self_OriginChainState, Signer* newSigner);
  struct Signer* currentSigner;
  struct Signer* nextSigner;
  struct ByteArray* latestHash;
  uint32_t index;
};

#define ORIGINCHAIN_H
#endif
