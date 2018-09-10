#ifndef ORIGINCHAIN_H
#include <stdint.h>
typedef struct OriginChainState OriginChainState;

struct OriginChainState {
  struct XYResult* (*newOriginBlock)(struct OriginChainState* self_OriginChainState, struct ByteArray* newHash);
  struct XYResult* (*addSigner)(struct OriginChainState* self_OriginChainState, struct Signer* newSigner);
  struct Signer* currentSigner;
  struct Signer* nextSigner;
  struct ByteArray* latestHash;
  uint32_t index;
};

#define ORIGINCHAIN_H
#endif
