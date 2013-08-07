/* ============================================================================
 * Copyright (c) 2010, Michael A. Jackson (BlueQuartz Software)
 * Copyright (c) 2010, Dr. Michael A. Groeber (US Air Force Research Laboratories
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice, this
 * list of conditions and the following disclaimer in the documentation and/or
 * other materials provided with the distribution.
 *
 * Neither the name of Michael A. Groeber, Michael A. Jackson, the US Air Force,
 * BlueQuartz Software nor the names of its contributors may be used to endorse
 * or promote products derived from this software without specific prior written
 * permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
 * USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *  This code was written under United States Air Force Contract number
 *                           FA8650-07-D-5800
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#include "TriclinicOps.h"
// Include this FIRST because there is a needed define for some compiles
// to expose some of the constants needed below
#include "DREAM3DLib/Common/DREAM3DMath.h"
#include "DREAM3DLib/Math/OrientationMath.h"

namespace Detail
{
  static const float TriclinicDim1InitValue = powf((0.75f*((DREAM3D::Constants::k_Pi)-sinf((DREAM3D::Constants::k_Pi)))),(1.0f/3.0f));
  static const float TriclinicDim2InitValue = powf((0.75f*((DREAM3D::Constants::k_Pi)-sinf((DREAM3D::Constants::k_Pi)))),(1.0f/3.0f));
  static const float TriclinicDim3InitValue = powf((0.75f*((DREAM3D::Constants::k_Pi)-sinf((DREAM3D::Constants::k_Pi)))),(1.0f/3.0f));
  static const float TriclinicDim1StepValue = TriclinicDim1InitValue/36.0f;
  static const float TriclinicDim2StepValue = TriclinicDim2InitValue/36.0f;
  static const float TriclinicDim3StepValue = TriclinicDim3InitValue/36.0f;
}

static const QuatF TriclinicQuatSym[1] = {
  QuaternionMathF::New(0.000000000f, 0.000000000f, 0.000000000f, 1.000000000f)};


static const float TriclinicRodSym[1][3] = {0.0f, 0.0f,0.0f};


static const float TriclinicMatSym[1][3][3] =
{{{1.0, 0.0, 0.0},
  {0.0, 1.0, 0.0},
  {0.0, 0.0, 1.0}}};



// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
TriclinicOps::TriclinicOps()
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
TriclinicOps::~TriclinicOps()
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
float TriclinicOps::getMisoQuat(QuatF &q1, QuatF &q2, float &n1, float &n2, float &n3)
{

  int numsym = 1;

  return _calcMisoQuat(TriclinicQuatSym, numsym, q1, q2, n1, n2, n3);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
float TriclinicOps::_calcMisoQuat(const QuatF quatsym[24], int numsym,
QuatF &q1, QuatF &q2,
float &n1, float &n2, float &n3)
{
  float wmin = 9999999.0f; //,na,nb,nc;
  float w = 0;
  float n1min = 0.0f;
  float n2min = 0.0f;
  float n3min = 0.0f;
  QuatF qr;
  QuatF qc;
  QuatF q2inv;
  QuaternionMathF::Copy(q2, q2inv);
  QuaternionMathF::Conjugate(q2inv);

  QuaternionMathF::Multiply(q2inv, q1, qr);
  for (int i = 0; i < numsym; i++)
  {
     QuaternionMathF::Multiply(qr, quatsym[i], qc);
        if (qc.w < -1) {
      qc.w = -1;
    }
    else if (qc.w > 1) {
      qc.w = 1;
    }

    OrientationMath::QuattoAxisAngle(qc, w, n1, n2, n3);

    if (w > DREAM3D::Constants::k_Pi) {
      w = DREAM3D::Constants::k_2Pi - w;
    }
    if (w < wmin)
    {
      wmin = w;
      n1min = n1;
      n2min = n2;
      n3min = n3;
    }
  }
  float denom = sqrt((n1min*n1min+n2min*n2min+n3min*n3min));
  n1 = n1min/denom;
  n2 = n2min/denom;
  n3 = n3min/denom;
  if(denom == 0) n1 = 0.0, n2 = 0.0, n3 = 1.0;
  if(wmin == 0) n1 = 0.0, n2 = 0.0, n3 = 1.0;
  return wmin;

}

void TriclinicOps::getODFFZRod(float &r1,float &r2, float &r3)
{
  int numsym = 1;

  _calcRodNearestOrigin(TriclinicRodSym, numsym, r1, r2, r3);
}

void TriclinicOps::getQuatSymOp(int i, QuatF &q)
{
  QuaternionMathF::Copy(TriclinicQuatSym[i], q);
}

void TriclinicOps::getRodSymOp(int i,float *r)
{
  r[0] = TriclinicRodSym[i][0];
  r[1] = TriclinicRodSym[i][1];
  r[2] = TriclinicRodSym[i][2];
}

void TriclinicOps::getMatSymOp(int i,float g[3][3])
{
  g[0][0] = TriclinicMatSym[i][0][0];
  g[0][1] = TriclinicMatSym[i][0][1];
  g[0][2] = TriclinicMatSym[i][0][2];
  g[1][0] = TriclinicMatSym[i][1][0];
  g[1][1] = TriclinicMatSym[i][1][1];
  g[1][2] = TriclinicMatSym[i][1][2];
  g[2][0] = TriclinicMatSym[i][2][0];
  g[2][1] = TriclinicMatSym[i][2][1];
  g[2][2] = TriclinicMatSym[i][2][2];
}

void TriclinicOps::getMDFFZRod(float &r1,float &r2, float &r3)
{
  float w, n1, n2, n3;
  float FZw, FZn1, FZn2, FZn3;

  OrientationOps::_calcRodNearestOrigin(TriclinicRodSym, 1, r1, r2, r3);
  OrientationMath::RodtoAxisAngle(r1, r2, r3, w, n1, n2, n3);

  OrientationMath::AxisAngletoRod(FZw, FZn1, FZn2, FZn3, r1, r2, r3);
}

void TriclinicOps::getNearestQuat(QuatF &q1, QuatF &q2)
{
  int numsym = 1;

  _calcNearestQuat(TriclinicQuatSym, numsym, q1, q2);
}

void TriclinicOps::getFZQuat(QuatF &qr)
{
  int numsym = 1;

  _calcQuatNearestOrigin(TriclinicQuatSym, numsym, qr);
}

int TriclinicOps::getMisoBin(float r1, float r2, float r3)
{
  float dim[3];
  float bins[3];
  float step[3];

  OrientationMath::RodtoHomochoric(r1, r2, r3);

  dim[0] = Detail::TriclinicDim1InitValue;
  dim[1] = Detail::TriclinicDim2InitValue;
  dim[2] = Detail::TriclinicDim3InitValue;
  step[0] = Detail::TriclinicDim1StepValue;
  step[1] = Detail::TriclinicDim2StepValue;
  step[2] = Detail::TriclinicDim3StepValue;
  bins[0] = 72.0f;
  bins[1] = 72.0f;
  bins[2] = 72.0f;

  return _calcMisoBin(dim, bins, step, r1, r2, r3);
}

void TriclinicOps::determineEulerAngles(int choose, float &synea1, float &synea2, float &synea3)
{
  float init[3];
  float step[3];
  float phi[3];
  float r1, r2, r3;

  init[0] = Detail::TriclinicDim1InitValue;
  init[1] = Detail::TriclinicDim2InitValue;
  init[2] = Detail::TriclinicDim3InitValue;
  step[0] = Detail::TriclinicDim1StepValue;
  step[1] = Detail::TriclinicDim2StepValue;
  step[2] = Detail::TriclinicDim3StepValue;
  phi[0] = static_cast<float>(choose % 72);
  phi[1] = static_cast<float>((choose / 72) % 72);
  phi[2] = static_cast<float>(choose / (72 * 72));

  _calcDetermineHomochoricValues(init, step, phi, choose, r1, r2, r3);
  OrientationMath::HomochorictoRod(r1, r2, r3);
  getODFFZRod(r1, r2, r3);
  OrientationMath::RodtoEuler(r1, r2, r3, synea1, synea2, synea3);
}

void TriclinicOps::determineRodriguesVector(int choose, float &r1, float &r2, float &r3)
{
  float init[3];
  float step[3];
  float phi[3];

  init[0] = Detail::TriclinicDim1InitValue;
  init[1] = Detail::TriclinicDim2InitValue;
  init[2] = Detail::TriclinicDim3InitValue;
  step[0] = Detail::TriclinicDim1StepValue;
  step[1] = Detail::TriclinicDim2StepValue;
  step[2] = Detail::TriclinicDim3StepValue;
  phi[0] = static_cast<float>(choose % 72);
  phi[1] = static_cast<float>((choose / 72) % 72);
  phi[2] = static_cast<float>(choose / (72 * 72));

  _calcDetermineHomochoricValues(init, step, phi, choose, r1, r2, r3);
  OrientationMath::HomochorictoRod(r1, r2, r3);
  getMDFFZRod(r1, r2, r3);
}
int TriclinicOps::getOdfBin(float r1, float r2, float r3)
{
  float dim[3];
  float bins[3];
  float step[3];

  OrientationMath::RodtoHomochoric(r1, r2, r3);

  dim[0] = Detail::TriclinicDim1InitValue;
  dim[1] = Detail::TriclinicDim2InitValue;
  dim[2] = Detail::TriclinicDim3InitValue;
  step[0] = Detail::TriclinicDim1StepValue;
  step[1] = Detail::TriclinicDim2StepValue;
  step[2] = Detail::TriclinicDim3StepValue;
  bins[0] = 72.0f;
  bins[1] = 72.0f;
  bins[2] = 72.0f;

  return _calcODFBin(dim, bins, step, r1, r2, r3);
}

void TriclinicOps::getSchmidFactorAndSS(float loadx, float loady, float loadz, float &schmidfactor, int &slipsys)
{
  schmidfactor = 0;
  slipsys = 0;
}

void TriclinicOps::getmPrime(QuatF &q1, QuatF &q2, float LD[3], float &mPrime)
{
  mPrime = 0;
}

void TriclinicOps::getF1(QuatF &q1, QuatF &q2, float LD[3], bool maxSF, float &F1)
{
  F1 = 0;
}

void TriclinicOps::getF1spt(QuatF &q1, QuatF &q2, float LD[3], bool maxSF, float &F1spt)
{
  F1spt = 0;
}

void TriclinicOps::getF7(QuatF &q1, QuatF &q2, float LD[3], bool maxSF, float &F7)
{
  F7 = 0;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void TriclinicOps::generateSphereCoordsFromEulers(FloatArrayType *eulers, FloatArrayType *xyz001, FloatArrayType *xyz011, FloatArrayType *xyz111)
{
  size_t nOrientations = eulers->GetNumberOfTuples();
  QuaternionMath<float>::Quaternion q1;
  TriclinicOps ops;
  float g[3][3];
  float gTranpose[3][3];
  float* currentEuler = NULL;
  float direction[3] = {0.0, 0.0, 0.0};

  // Sanity Check the size of the arrays
  if (xyz001->GetNumberOfTuples() < nOrientations * 6)
  {
    xyz001->Resize(nOrientations * 6 * 3);
  }
  if (xyz011->GetNumberOfTuples() < nOrientations * 12)
  {
    xyz011->Resize(nOrientations * 12 * 3);
  }
  if (xyz111->GetNumberOfTuples() < nOrientations * 8)
  {
    xyz111->Resize(nOrientations * 8 * 3);
  }

  // Geneate all the Coordinates
  for(size_t i = 0; i < nOrientations; ++i)
  {

    currentEuler = eulers->GetPointer(i * 3);

    OrientationMath::EulertoMat(currentEuler[0], currentEuler[1], currentEuler[2], g);
    MatrixMath::Transpose3x3(g, gTranpose);

    // -----------------------------------------------------------------------------
    // 001 Family
    direction[0] = 1.0; direction[1] = 0.0; direction[2] = 0.0;
    MatrixMath::Multiply3x3with3x1(gTranpose, direction, xyz001->GetPointer(i*18));
    MatrixMath::Copy3x1(xyz001->GetPointer(i*18),xyz001->GetPointer(i*18 + 3));
    MatrixMath::Multiply3x1withConstant(xyz001->GetPointer(i*18 + 3),-1);
    direction[0] = 0.0; direction[1] = 1.0; direction[2] = 0.0;
    MatrixMath::Multiply3x3with3x1(gTranpose, direction, xyz001->GetPointer(i*18 + 6));
    MatrixMath::Copy3x1(xyz001->GetPointer(i*18 + 6),xyz001->GetPointer(i*18 + 9));
    MatrixMath::Multiply3x1withConstant(xyz001->GetPointer(i*18 + 9),-1);
    direction[0] = 0.0; direction[1] = 0.0; direction[2] = 1.0;
    MatrixMath::Multiply3x3with3x1(gTranpose, direction, xyz001->GetPointer(i*18 + 12));
    MatrixMath::Copy3x1(xyz001->GetPointer(i*18 + 12),xyz001->GetPointer(i*18 + 15));
    MatrixMath::Multiply3x1withConstant(xyz001->GetPointer(i*18 + 15),-1);

    // -----------------------------------------------------------------------------
    // 011 Family
    direction[0] = DREAM3D::Constants::k_1OverRoot2; direction[1] = DREAM3D::Constants::k_1OverRoot2; direction[2] = 0.0;
    MatrixMath::Multiply3x3with3x1(gTranpose, direction, xyz011->GetPointer(i*36));
    MatrixMath::Copy3x1(xyz011->GetPointer(i*36),xyz011->GetPointer(i*36 + 3));
    MatrixMath::Multiply3x1withConstant(xyz011->GetPointer(i*36 + 3),-1);
    direction[0] = DREAM3D::Constants::k_1OverRoot2; direction[1] = 0.0; direction[2] = DREAM3D::Constants::k_1OverRoot2;
    MatrixMath::Multiply3x3with3x1(gTranpose, direction, xyz011->GetPointer(i*36 + 6));
    MatrixMath::Copy3x1(xyz011->GetPointer(i*36+6),xyz011->GetPointer(i*36 + 9));
    MatrixMath::Multiply3x1withConstant(xyz011->GetPointer(i*36 + 9),-1);
    direction[0] = 0.0; direction[1] = DREAM3D::Constants::k_1OverRoot2; direction[2] = DREAM3D::Constants::k_1OverRoot2;
    MatrixMath::Multiply3x3with3x1(gTranpose, direction, xyz011->GetPointer(i*36 + 12));
    MatrixMath::Copy3x1(xyz011->GetPointer(i*36+12),xyz011->GetPointer(i*36 + 15));
    MatrixMath::Multiply3x1withConstant(xyz011->GetPointer(i*36 + 15),-1);
    direction[0] = -DREAM3D::Constants::k_1OverRoot2; direction[1] = -DREAM3D::Constants::k_1OverRoot2; direction[2] = 0.0;
    MatrixMath::Multiply3x3with3x1(gTranpose, direction, xyz011->GetPointer(i*36 + 18));
    MatrixMath::Copy3x1(xyz011->GetPointer(i*36+18),xyz011->GetPointer(i*36 + 21));
    MatrixMath::Multiply3x1withConstant(xyz011->GetPointer(i*36 + 21),-1);
    direction[0] = -DREAM3D::Constants::k_1OverRoot2; direction[1] = 0.0; direction[2] = DREAM3D::Constants::k_1OverRoot2;
    MatrixMath::Multiply3x3with3x1(gTranpose, direction, xyz011->GetPointer(i*36 + 24));
    MatrixMath::Copy3x1(xyz011->GetPointer(i*36+24),xyz011->GetPointer(i*36 + 27));
    MatrixMath::Multiply3x1withConstant(xyz011->GetPointer(i*36 + 27),-1);
    direction[0] = 0.0; direction[1] = -DREAM3D::Constants::k_1OverRoot2; direction[2] = DREAM3D::Constants::k_1OverRoot2;
    MatrixMath::Multiply3x3with3x1(gTranpose, direction, xyz011->GetPointer(i*36 + 30));
    MatrixMath::Copy3x1(xyz011->GetPointer(i*36+30),xyz011->GetPointer(i*36 + 33));
    MatrixMath::Multiply3x1withConstant(xyz011->GetPointer(i*36 + 33),-1);

    // -----------------------------------------------------------------------------
    // 111 Family
    direction[0] = DREAM3D::Constants::k_1OverRoot3; direction[1] = DREAM3D::Constants::k_1OverRoot3; direction[2] = DREAM3D::Constants::k_1OverRoot3;
    MatrixMath::Multiply3x3with3x1(gTranpose, direction, xyz111->GetPointer(i*24));
    MatrixMath::Copy3x1(xyz111->GetPointer(i*24),xyz111->GetPointer(i*24 + 3));
    MatrixMath::Multiply3x1withConstant(xyz111->GetPointer(i*24 + 3),-1);
    direction[0] = -DREAM3D::Constants::k_1OverRoot3; direction[1] = DREAM3D::Constants::k_1OverRoot3; direction[2] = DREAM3D::Constants::k_1OverRoot3;
    MatrixMath::Multiply3x3with3x1(gTranpose, direction, xyz111->GetPointer(i*24 + 6));
    MatrixMath::Copy3x1(xyz111->GetPointer(i*24+6),xyz111->GetPointer(i*24 + 9));
    MatrixMath::Multiply3x1withConstant(xyz111->GetPointer(i*24 + 9),-1);
    direction[0] = DREAM3D::Constants::k_1OverRoot3; direction[1] = -DREAM3D::Constants::k_1OverRoot3; direction[2] = DREAM3D::Constants::k_1OverRoot3;
    MatrixMath::Multiply3x3with3x1(gTranpose, direction, xyz111->GetPointer(i*24 + 12));
    MatrixMath::Copy3x1(xyz111->GetPointer(i*24+12),xyz111->GetPointer(i*24 + 15));
    MatrixMath::Multiply3x1withConstant(xyz111->GetPointer(i*24 + 15),-1);
    direction[0] = DREAM3D::Constants::k_1OverRoot3; direction[1] = DREAM3D::Constants::k_1OverRoot3; direction[2] = -DREAM3D::Constants::k_1OverRoot3;
    MatrixMath::Multiply3x3with3x1(gTranpose, direction, xyz111->GetPointer(i*24 + 18));
    MatrixMath::Copy3x1(xyz111->GetPointer(i*24+18),xyz111->GetPointer(i*24 + 21));
    MatrixMath::Multiply3x1withConstant(xyz111->GetPointer(i*24 + 21),-1);
  }

}
