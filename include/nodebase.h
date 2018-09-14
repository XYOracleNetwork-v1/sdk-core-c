#ifndef NODEBASE_H
#include <stdint.h>
typedef struct BaseNode BaseNode;

struct BaseNode {
  struct StorageProvider* StorageProvider;
  struct HashProvider* HashProvider;
  struct XyoZigZagBoundWitnessSession* currentBoundWitnessSession;
  struct OriginChainNavigator* originBlocks;
  struct OriginChainState* originState;
  void (*addHeuristic)(int index, struct XYObject* (*f)());
  void (*removeHeuristic)(int index);
  void (*addListener)(int index, struct XYObject* (*f)());
  void (*removeListener)(int index);
  struct XYResult* (*doBoundWitness)(struct ByteArray* startingData, struct NetworkPipe*);
  struct XYResult* (*selfSignOriginChain)(int flag);
  int (*getChoice)(int catalog);
  int supportedFlags;

};

#define NODEBASE_H
#endif
