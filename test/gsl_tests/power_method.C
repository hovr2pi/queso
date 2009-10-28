/*--------------------------------------------------------------------------
 *--------------------------------------------------------------------------
 *
 * Copyright (C) 2008 The PECOS Development Team
 *
 * Please see http://pecos.ices.utexas.edu for more information.
 *
 * This file is part of the QUESO Library (Quantification of Uncertainty
 * for Estimation, Simulation and Optimization).
 *
 * QUESO is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * QUESO is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with QUESO. If not, see <http://www.gnu.org/licenses/>.
 *
 *--------------------------------------------------------------------------
 *
 * $Id$
 *
 * Brief description of this file: 
 * 
 *--------------------------------------------------------------------------
 *-------------------------------------------------------------------------- */

#include <cmath>

#include <uqEnvironment.h>
#include <uqGslMatrix.h>
#include <uqVectorRV.h>

int actualChecking(const uqFullEnvironmentClass* env);

using std::fabs;

int main(int argc, char* argv[]) 
{
  int return_flag = 0;

   // Initialize environment
  MPI_Init(&argc,&argv);
  uqFullEnvironmentClass* env =
    new uqFullEnvironmentClass(MPI_COMM_WORLD,"input","");

  return_flag = actualChecking(env);

  delete env;
  MPI_Finalize();

  // Fin.
  return return_flag;
}

/* Separated this out into a function because we want
   the destructor for paramSpace to be called before
   we delete env in main. */
int actualChecking(const uqFullEnvironmentClass* env)
{
  int return_flag = 0;

  // This is the tolerance used in the Power method algorithm.
  const double fp_tol = 1.0e-13;

    // Instantiate the parameter space
  uqVectorSpaceClass<uqGslVectorClass,uqGslMatrixClass>
    paramSpace( (*env), "param_", 2, NULL);

  // Populate Matrix.
  uqGslMatrixClass* Matrix = paramSpace.newMatrix();
  (*Matrix)(0,0) = 4.; (*Matrix)(0,1) = 3.;
  (*Matrix)(1,0) = 5.; (*Matrix)(1,1) = 7.;

  // Conduct test.
  double eValue = 0.0;
  uqGslVectorClass eVector( (*paramSpace.newVector()) );

  Matrix->largestEigen( eValue, eVector );

  // MATLAB reports the following for the eigenvalues and eigenvectors
  // of the matrix:
  // A = [ 4, 3; 5, 7];
  // [V,D] = eig(A)
  // V =
  //
  //  -0.749062754969087  -0.468750367387953
  //   0.662499048390352  -0.883330681610041
  //
  // D =
  //
  //   1.346688068540963                   0
  //                   0   9.653311931459037

  double eValueExact = 9.653311931459037;

  // Note the minus sign doesn't matter (as long as the sign change is
  // consistent within the vector). We just need to make sure
  // that we get the right values in the vector.
  uqGslVectorClass eVectorExact( (*paramSpace.newVector() ) );
  eVectorExact[0] = -0.468750367387953;
  eVectorExact[1] = -0.883330681610041;

  // MATLAB returns normalized vectors while the Power method
  // function does not, so we must normalize.
  double norm = eVector.norm2();
  eVector /= norm;

  if( fabs(eValueExact - eValue) > fp_tol )
    {
      return_flag = 1;
    }

  if( fabs( eVector[0] - eVectorExact[0] ) > fp_tol ||
      fabs( eVector[1] - eVectorExact[1] ) > fp_tol   )
    {
      if( fabs( -eVector[0] - eVectorExact[0] ) > fp_tol ||
	  fabs( -eVector[1] - eVectorExact[1] ) > fp_tol   )
	{
	  return_flag = 1;
	}
    }

  // Deallocate pointers we created.
  delete Matrix;

  return return_flag;
}