#include "xyobject.h"
#ifndef REPOSITORY_H
typedef struct RepositoryProvider RepositoryProvider;
typedef struct OriginChainRepository OriginChainRepository;

struct RepositoryProvider{
  uint repository;
  uint logicalEnd;
  XYResult* (*write)(ByteArray* value, uint offset, uint timeout);
  XYResult* (*read)(uint offset, uint timeout);
  XYResult* (*readRows)(uint beginning, uint end, uint timeout);
  XYResult* (*delete)(uint offset, uint timeout);
  XYResult* (*deleteRows)(uint beginning, uint end, uint timeout);
};

struct OriginChainRepository {
  XYResult* (*append)(ByteArray* value, uint timeout);
  RepositoryProvider* repo;
};

#define REPOSITORY_H
#endif
