#include "MyMesh.h"

namespace MeshLib{
    
/*! Normalize mesh
* \param pMesh the input mesh
*/
void normalize_mesh(CMyMesh * pMesh)
{
	CPoint s(0, 0, 0);
	for (CMyMesh::_MeshVertexIterator viter(pMesh); !viter.end(); ++viter)
	{
		CMyVertex * v = *viter;
		s = s + v->point();
	}
	s = s / pMesh->numVertices();

	for (CMyMesh::_MeshVertexIterator viter(pMesh); !viter.end(); ++viter)
	{
		CMyVertex * v = *viter;
		CPoint p = v->point();
		p = p - s;
		v->point() = p;
	}

	double d = 0;
	for (CMyMesh::_MeshVertexIterator viter(pMesh); !viter.end(); ++viter)
	{
		CMyVertex * v = *viter;
		CPoint p = v->point();
		for (int k = 0; k < 3; k++)
		{
			d = (d > fabs(p[k])) ? d : fabs(p[k]);
		}
	}

	for (CMyMesh::_MeshVertexIterator viter(pMesh); !viter.end(); ++viter)
	{
		CMyVertex * v = *viter;
		CPoint p = v->point();
		p = p / d;
		v->point() = p;
	}
};

/*! Compute the face normal and vertex normal
* \param pMesh the input mesh
*/
void compute_normal(CMyMesh * pMesh)
{
	for (CMyMesh::_MeshVertexIterator viter(pMesh); !viter.end(); ++viter)
	{
		CMyVertex * v = *viter;
		CPoint n(0, 0, 0);
		for (CMyMesh::_VertexFaceIterator vfiter(v); !vfiter.end(); ++vfiter)
		{
			CMyFace * pF = *vfiter;

			CPoint p[3];
			CHalfEdge * he = pF->halfedge();
			for (int k = 0; k < 3; k++)
			{
				p[k] = he->target()->point();
				he = he->he_next();
			}

			CPoint fn = (p[1] - p[0]) ^ (p[2] - p[0]);
			pF->normal() = fn / fn.norm();
			n += fn;
		}

		n = n / n.norm();
		v->normal() = n;
	}
};


}
