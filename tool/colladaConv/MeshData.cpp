#include "Exception.h"
#include "MeshData.h"

CMeshData::CMeshData():
m_pMeshArray()
, m_MeshCount(0)
{
}

CMeshData::~CMeshData()
{
   Invalidate();
}

void CMeshData::Invalidate()
{
   m_MeshCount = 0;
   m_pMeshArray.clear();
}
