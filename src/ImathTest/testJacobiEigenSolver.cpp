//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

#ifdef NDEBUG
#    undef NDEBUG
#endif

#include <ImathMatrix.h>
#include <ImathMatrixAlgo.h>
#include <algorithm>
#include <cassert>
#include <cmath>
#include <ctime>
#include <iostream>
#include <limits>
#include <math.h>

using namespace std;
using namespace IMATH_INTERNAL_NAMESPACE;

// clang-format off

const Matrix33<double> A33_1 ( 1, 0, 0, 0, 1, 0, 0, 0, 1 );
const Matrix33<double> A33_2 ( 1, 0, 0, 0,-1, 0, 0, 0, 1 );
const Matrix33<double> A33_3 ( 1, 0, 0, 0, 1, 0, 0, 0, 0 );
const Matrix33<double> A33_4 ( 1, 0, 0, 0, 0, 0, 0, 0, 0 );
const Matrix33<double> A33_5 ( 0, 0, 0, 0, 0, 0, 0, 0, 0 );
const Matrix33<double> A33_6 ( 1, 0, 0, 0, 1e-10, 0, 0, 0, 0 );
const Matrix33<double> A33_7 ( 1, 0, 0, 0, 1e-10, 0, 0, 0, 1e+10 );
const Matrix33<double> A33_8 (
     0.25058694044821,  0.49427229444416,  0.81415724537748,
     0.49427229444416,  0.80192384710853, -0.61674948224910,
     0.81415724537748, -0.61674948224910, -1.28486154645285);
const Matrix33<double> A33_9 (
     4,  -30,    60,
   -30,  300,  -675,
    60, -675,  1620);

const Matrix44<double> A44_1 ( 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 );
const Matrix44<double> A44_2 ( 1, 0, 0, 0, 0,-1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 );
const Matrix44<double> A44_3 ( 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 );
const Matrix44<double> A44_4 ( 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 );
const Matrix44<double> A44_5 ( 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 );
const Matrix44<double> A44_6 ( 1, 0, 0, 0, 0, 1e-20, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
const Matrix44<double> A44_7 ( 1, 0, 0, 0, 0, 1e-20, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1e+20 );
const Matrix44<double> A44_8 (
     4.05747631538951,  0.16358123075600,  0.11541756047409, -1.65369223465270,
     0.16358123075600,  0.57629829390780,  3.88542912704029,  0.92016316185369,
     0.11541756047409,  3.88542912704029,  0.65367032943707, -0.21971103270410,
    -1.65369223465270,  0.92016316185369, -0.21971103270410, -0.28108876552761);
const Matrix44<double> A44_9 (
     4,  -30,    60,   -35,
   -30,  300,  -675,   420,
    60, -675,  1620, -1050,
   -35,  420, -1050,   700);

// clang-format on

template <typename TM>
void
verifyOrthonormal (const TM& A, const typename TM::BaseType threshold)
{
    const TM prod = A * A.transposed();
    for (size_t i = 0; i < TM::dimensions(); ++i)
        for (size_t j = 0; j < TM::dimensions(); ++j)
            if (i == j) 
                assert (std::abs (prod[i][j] - 1) < threshold);
            else
                assert (std::abs (prod[i][j]) < threshold);
}

template <typename TM>
typename TM::BaseType
computeThreshold(const TM& A)
{
   typedef typename TM::BaseType T;
   T maxAbsEntry(0);

   for (size_t i = 0; i < TM::dimensions(); ++i)
       for (size_t j = 0; j < TM::dimensions(); ++j)
           maxAbsEntry = std::max (maxAbsEntry, std::abs(A[i][j]));

   const T eps = std::numeric_limits<T>::epsilon();
   maxAbsEntry = std::max(maxAbsEntry, eps);

   return maxAbsEntry * T(100) * eps;
}

template<class TM>
void
testJacobiEigenSolver(const TM& A)
{
    using std::abs;

    typedef typename TM::BaseType T;
    typedef typename TM::BaseVecType TV;

    const T threshold = computeThreshold(A);

    TM AA(A);
    TV S;
    TM V;

    jacobiEigenSolver(AA, S, V);

    // Orthogonality of V
    verifyOrthonormal(V, threshold);
 
    // Determinant of V
    assert(abs(V.determinant()) - 1 < threshold);

    // Determinant of A and S
    TM MS;
    for (size_t i = 0; i < TM::dimensions(); ++i)
        for (size_t j = 0; j < TM::dimensions(); ++j)
            if(i == j)
                MS[i][j] = S[i];
            else
                MS[i][j] = 0;

    assert (abs (A.determinant()) - abs (MS.determinant()) < threshold);

    // A = V * S * V^T
    TM MA = V * MS * V.transposed();

    for (size_t i = 0; i < TM::dimensions(); ++i) 
        for (size_t j =0; j < TM::dimensions(); ++j) 
            assert(abs(A[i][j]-MA[i][j]) < threshold);
}

template<class TM>
void
testMinMaxEigenValue(const TM& A)
{
  typedef typename TM::BaseVecType TV;
  typedef typename TM::BaseType T;

  TV minV, maxV, S;
  TM U, V;
  
  const T threshold = computeThreshold(A);

  {
      TM A1(A);
      minEigenVector(A1, minV);
      TM A2(A);
      maxEigenVector(A2, maxV);
  }
  {
      TM A3(A);
      jacobiSVD(A3, U, S, V);
  }

  const int dim = TM::dimensions();

    for (int i = 0; i < dim; ++i)
    {
      assert(abs(minV[i]-V[i][dim - 1]) < threshold);
      assert(abs(maxV[i]-V[i][0]) < threshold);
  }
}

template <class T>
void
testJacobiTiming()
{

    int rounds(100000);
    clock_t tJacobi,tSVD, t;

    {
        Matrix33<T> A,V,U;
        Vec3<T> S;

        t = clock();
        for (int i = 0; i < rounds; ++i)
        {
            A = Matrix33<T>(A33_7);
            jacobiEigenSolver(A, S, V);
            A = Matrix33<T>(A33_8);
            jacobiEigenSolver(A, S, V);
        }
        tJacobi = clock() - t;
        cout << "Jacobi EigenSolver of 3x3 matrices took " << tJacobi << " clocks." << endl;

        t = clock();
        for (int i = 0; i < rounds; ++i)
        {
            A = Matrix33<T>(A33_7);
            jacobiSVD(A, U, S, V);
            A = Matrix33<T>(A33_8);
            jacobiSVD(A, U, S, V);
        }
        tSVD = clock() - t;
        cout << "TinySVD            of 3x3 matrices took " << tSVD << " clocks." << endl;
        cout << (float)(tSVD-tJacobi)*100.0f/(float)(tSVD) << "% speed up." << endl;
    }

    {
        Matrix44<T> A,V,U;
        Vec4<T> S;

        t = clock();
        for (int i = 0; i < rounds; ++i)
        {
            A = Matrix44<T>(A44_7);
            jacobiEigenSolver(A, S, V);
            A = Matrix44<T>(A44_8);
            jacobiEigenSolver(A, S, V);
        }
        tJacobi = clock() - t;
        cout << "Jacobi EigenSolver of 4x4 matrices took " << tJacobi << " clocks" << endl;

        t = clock();
        for (int i = 0; i < rounds; ++i)
        {
            A = Matrix44<T>(A44_7);
            jacobiSVD(A, U, S, V);
            A = Matrix44<T>(A44_8);
            jacobiSVD(A, U, S, V);
        }
        tSVD = clock() - t;
        cout << "TinySVD            of 4x4 matrices took " << tSVD << " clocks" << endl;
        cout << (float)(tSVD-tJacobi)*100.0f/(float)(tSVD) << "% speed up." << endl;
    }
}

template <class T>
void
testJacobiEigenSolverImp()
{
    testJacobiEigenSolver(Matrix33<T>(A33_1));
    testJacobiEigenSolver(Matrix33<T>(A33_2));
    testJacobiEigenSolver(Matrix33<T>(A33_3));
    testJacobiEigenSolver(Matrix33<T>(A33_4));
    testJacobiEigenSolver(Matrix33<T>(A33_5));
    testJacobiEigenSolver(Matrix33<T>(A33_6));
    testJacobiEigenSolver(Matrix33<T>(A33_7));
    testJacobiEigenSolver(Matrix33<T>(A33_8));

    testJacobiEigenSolver(Matrix44<T>(A44_1));
    testJacobiEigenSolver(Matrix44<T>(A44_2));
    testJacobiEigenSolver(Matrix44<T>(A44_3));
    testJacobiEigenSolver(Matrix44<T>(A44_4));
    testJacobiEigenSolver(Matrix44<T>(A44_5));
    testJacobiEigenSolver(Matrix44<T>(A44_6));
    testJacobiEigenSolver(Matrix44<T>(A44_7));
    testJacobiEigenSolver(Matrix44<T>(A44_8));
}

template <class T>
void
testMinMaxEigenValueImp()
{
    testMinMaxEigenValue(Matrix33<T>(A33_7));
    testMinMaxEigenValue(Matrix33<T>(A33_8));

    testMinMaxEigenValue(Matrix44<T>(A44_7));
    testMinMaxEigenValue(Matrix44<T>(A44_8));
}

void
testJacobiEigenSolver()
{
    cout << endl;
    cout << "************ Testing IMATH_INTERNAL_NAMESPACE::ImathJacobiEigenSolver ************"
         << endl;
    
    cout << "Jacobi EigenSolver in single precision...";
    testJacobiEigenSolverImp<float>();
    cout << "PASS" << endl;

    cout << "Jacobi EigenSolver in double precision...";
    testJacobiEigenSolverImp<double>();
    cout << "PASS" << endl;

    cout << "Min/Max EigenValue in single precision...";
    testMinMaxEigenValueImp<float>();
    cout << "PASS" << endl;

    cout << "Min/Max EigenValue in double precision...";
    testMinMaxEigenValueImp<double>();
    cout << "PASS" << endl;

    cout << "Timing Jacobi EigenSolver in single precision...\n";
    testJacobiTiming<float>();

    cout << "Timing Jacobi EigenSolver in double precision...\n";
    testJacobiTiming<double>();
    
    cout << "************      ALL PASS          ************" << endl;
}
