#pragma once

#include "Mesh/Vertex.h"
#include "Mesh/Edge.h"
#include "Mesh/Face.h"
#include "Mesh/HalfEdge.h"
#include "Mesh/BaseMesh.h"

#include "Mesh/boundary.h"
#include "Mesh/iterators.h"
#include "Parser/parser.h"

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
		CMyVertex() :m_curvature(0.) {};
		~CMyVertex() {};

		double & curvature() { return m_curvature; };
	protected:
		double m_curvature;
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
		typedef CBoundary<V, E, F, H>					CBoundary;
		typedef CLoop<V, E, F, H>						CLoop;

		typedef MeshVertexIterator<V, E, F, H>			MeshVertexIterator;
		typedef MeshEdgeIterator<V, E, F, H>			MeshEdgeIterator;
		typedef MeshFaceIterator<V, E, F, H>			MeshFaceIterator;
		typedef MeshHalfEdgeIterator<V, E, F, H>		MeshHalfEdgeIterator;

		typedef VertexVertexIterator<V, E, F, H>		VertexVertexIterator;
		typedef VertexEdgeIterator<V, E, F, H>			VertexEdgeIterator;
		typedef VertexFaceIterator<V, E, F, H>			VertexFaceIterator;
		typedef VertexInHalfedgeIterator<V, E, F, H>	VertexInHalfedgeIterator;
		typedef VertexOutHalfedgeIterator<V, E, F, H>	VertexOutHalfedgeIterator;

		typedef FaceVertexIterator<V, E, F, H>			FaceVertexIterator;
		typedef FaceEdgeIterator<V, E, F, H>			FaceEdgeIterator;
		typedef FaceHalfedgeIterator<V, E, F, H>		FaceHalfedgeIterator;

		void   check_gauss_bonnet_theory();
	protected:
		void   _calc_angles();
		void   _calc_gauss_curvatures();
		double _calc_euler_characteristic();
		double _cosine_law(double a, double b, double c);
	};

	typedef MyMesh<CMyVertex, CMyEdge, CMyFace, CMyHalfEdge> CMyMesh;

	template<typename V, typename E, typename F, typename H>
	void MyMesh<V, E, F, H>::check_gauss_bonnet_theory()
	{
		//1. Calculate the Gauss Curvature for each vertex.
		_calc_gauss_curvatures();
		double sum = 0.;
		for (MeshVertexIterator viter(this); !viter.end(); ++viter)
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
	}

	template<typename V, typename E, typename F, typename H>
	void MyMesh<V, E, F, H>::_calc_gauss_curvatures()
	{

	}

	template<typename V, typename E, typename F, typename H>
	double MyMesh<V, E, F, H>::_calc_euler_characteristic()
	{
		return 1;
	}

	template<typename V, typename E, typename F, typename H>
	inline double MyMesh<V, E, F, H>::_cosine_law(double a, double b, double c)
	{
		return 0;
	}
}
