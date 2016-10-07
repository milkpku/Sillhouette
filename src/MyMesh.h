#ifndef _MYMESH_H_
#define _MYMESH_H_

#include "Mesh/Vertex.h"
#include "Mesh/Edge.h"
#include "Mesh/Face.h"
#include "Mesh/HalfEdge.h"
#include "Mesh/BaseMesh.h"

#include "Mesh/boundary.h"
#include "Mesh/iterators.h"
#include "Parser/parser.h"

#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846264338327950288419716939937510
#endif

namespace MeshLib
{
	class CMyVertex;
	class CMyEdge;
	class CMyFace;
	class CMyHalfEdge;

	class CMyVertex : public CVertex
	{
	public:
		CMyVertex() :m_curvature(0.){};
		~CMyVertex() {};

		double & curvature() { return m_curvature; };
        unsigned int & draw_id() {return m_draw_id; };
	protected:
		double m_curvature; 
        unsigned int m_draw_id;
	};

	class CMyEdge : public CEdge
	{
	public:
		CMyEdge() {};
		~CMyEdge() {};

		inline double length() 
		{
			CPoint & source = this->halfedge(0)->source()->point();
			CPoint & target = this->halfedge(0)->target()->point();
			return (target - source).norm();
		};
	};

	class CMyFace : public CFace
	{
	public:
		CMyFace() : m_normal(0, 0, 0) {};
		~CMyFace() {};

		CPoint & normal() { return m_normal; };
	protected:
		CPoint m_normal;
	};

	class CMyHalfEdge : public CHalfEdge
	{
	public:
		CMyHalfEdge() :m_angle(.0) {};
		~CMyHalfEdge() {};

		double & angle() { return m_angle; };
	protected:
		double m_angle;
	};

	template<typename V, typename E, typename F, typename H>
	class MyMesh : public CBaseMesh<V, E, F, H>
	{
	public:
        typedef CBoundary<V, E, F, H>					_CBoundary;
		typedef CLoop<V, E, F, H>						_CLoop;

		typedef MeshVertexIterator<V, E, F, H>			_MeshVertexIterator;
		typedef MeshEdgeIterator<V, E, F, H>			_MeshEdgeIterator;
		typedef MeshFaceIterator<V, E, F, H>			_MeshFaceIterator;
		typedef MeshHalfEdgeIterator<V, E, F, H>		_MeshHalfEdgeIterator;

		typedef VertexVertexIterator<V, E, F, H>		_VertexVertexIterator;
		typedef VertexEdgeIterator<V, E, F, H>			_VertexEdgeIterator;
		typedef VertexFaceIterator<V, E, F, H>			_VertexFaceIterator;
		typedef VertexInHalfedgeIterator<V, E, F, H>	_VertexInHalfedgeIterator;
		typedef VertexOutHalfedgeIterator<V, E, F, H>	_VertexOutHalfedgeIterator;

		typedef FaceVertexIterator<V, E, F, H>			_FaceVertexIterator;
		typedef FaceEdgeIterator<V, E, F, H>			_FaceEdgeIterator;
		typedef FaceHalfedgeIterator<V, E, F, H>		_FaceHalfedgeIterator;

		void   check_gauss_bonnet_theory();
	protected:
        // calculate angles respond for each halgedge
		void   _calc_angles();
        // calculate gauss curvature for each vertex
		void   _calc_gauss_curvatures();
        // sum gauss curvature and calc euler number
		double _calc_euler_characteristic();
		double _cosine_law(CPoint a, CPoint b, CPoint c);
	};

	typedef MyMesh<CMyVertex, CMyEdge, CMyFace, CMyHalfEdge> CMyMesh;

	template<typename V, typename E, typename F, typename H>
	void MyMesh<V, E, F, H>::check_gauss_bonnet_theory()
	{
		//1. Calculate the Gauss Curvature for each vertex.
		_calc_gauss_curvatures();
		double sum = 0.;
		for (_MeshVertexIterator viter(this); !viter.end(); ++viter)
		{
			V * pV = *viter;
			sum += pV->curvature();
		}
		std::cout << "1). The sum of Gauss curvature:" << sum << std::endl;

		//2. Calculate the Euler Characteristic.
		double euler = _calc_euler_characteristic();
		std::cout << "2). The Euler Characteristic:" << euler << std::endl;

		double g = (2.0 - euler) / 2.0;
		std::cout << " 2.1). The genus equals:" << g << std::endl;

		//3. Check the Gauss-Bonnet Theory.
		double error = fabs(sum - 2.0 * M_PI * euler);
		std::cout << "3). Check whether sum(K_i) == 2 * PI * Euler Char.\tError:" << error << std::endl;
	}

	template<typename V, typename E, typename F, typename H>
	void MyMesh<V, E, F, H>::_calc_angles()
	{
        for (_MeshHalfEdgeIterator hiter(this); !hiter.end(); ++hiter)
        {
            H * pH = *hiter;
            double &ang = pH->angle();
            double _cos = this->_cosine_law(pH->he_next()->target()->point(), pH->source()->point(), pH->target()->point());
            ang = acos(_cos);
        }
	}

	template<typename V, typename E, typename F, typename H>
	void MyMesh<V, E, F, H>::_calc_gauss_curvatures()
	{
        this->_calc_angles();
        for (_MeshHalfEdgeIterator hiter(this); !hiter.end(); ++hiter)
        {
            H * pH = *hiter;
            V * pV = (V*) pH->he_next()->target();
            pV->curvature() += pH->angle();
        }

        for (_MeshVertexIterator viter(this); !viter.end(); ++viter)
        {
            V * pV = *viter;
            double &_cur = pV->curvature();
            _cur = 2*M_PI - _cur;
            if (pV->boundary())
            {
                _cur -= M_PI;
            }
        }
	}

	template<typename V, typename E, typename F, typename H>
	double MyMesh<V, E, F, H>::_calc_euler_characteristic()
	{
        double euler = 0;
        for (_MeshVertexIterator viter(this); !viter.end(); ++viter)
            euler += 1;
        for (_MeshEdgeIterator eiter(this); !eiter.end(); ++eiter)
            euler -= 1;
        for (_MeshFaceIterator fiter(this); !fiter.end(); ++fiter)
            euler += 1;
		return euler ;
	}

	template<typename V, typename E, typename F, typename H>
	inline double MyMesh<V, E, F, H>::_cosine_law(CPoint a, CPoint b, CPoint c)
	{
        CPoint r1 = b - a;
        CPoint r2 = c - a;
        return r1*r2 / (r1.norm() * r2.norm());
	}

/*! Normalize mesh
* \param pMesh the input mesh
*/
void normalize_mesh(CMyMesh * pMesh);

/*! Compute the face normal and vertex normal
* \param pMesh the input mesh
*/
void compute_normal(CMyMesh * pMesh);

}

#endif
