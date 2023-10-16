# SPDX-License-Identifier: BSD-3-Clause
# Copyright Contributors to the OpenEXR Project.

# Move this back to test_m44.py once this has been evaluated
# for pybind11 AND boost python bindings

import unittest
from imath import V3f, M44f, Eulerf

class M44fTestCase(unittest.TestCase):
    def test_baseTypeEpsilon(self):
        result = M44f.baseTypeEpsilon()
        self.assertEqual(result, 1.1920928955078125e-07)

    def test_baseTypeLowest(self):
        result = M44f.baseTypeLowest()
        self.assertEqual(result, -3.4028234663852886e+38)

    def test_baseTypeMax(self):
        result = M44f.baseTypeMax()
        self.assertEqual(result, 3.4028234663852886e+38)

    def test_baseTypeSmallest(self):
        result = M44f.baseTypeSmallest()
        self.assertEqual(result, 1.1754943508222875e-38)

    def test_determinant(self):
        matrix = M44f(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16)
        result = matrix.determinant()
        expected_result = 0.0
        self.assertEqual(result, expected_result)

    def test_equalWithAbsError(self):
        matrix1 = M44f(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16)
        matrix2 = M44f(1.01, 2.02, 3.01, 4.02, 5.01, 6.02, 7.01, 8.02, 9.01, 10.02, 11.01, 12.02, 13.01, 14.02, 15.01, 16.02)
        result = M44f.equalWithAbsError(matrix1, matrix2, 0.05)
        self.assertTrue(result)

    def test_equalWithRelError(self):
        matrix1 = M44f(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16)
        matrix2 = M44f(1.01, 2.02, 3.01, 4.02, 5.01, 6.02, 7.01, 8.02, 9.01, 10.02, 11.01, 12.02, 13.01, 14.02, 15.01, 16.02)
        result = M44f.equalWithRelError(matrix1, matrix2, 0.02)
        self.assertTrue(result)

    def test_extractAndRemoveScalingAndShear(self):
        """
        check scale, shear and if the they were removed from the matrix
        """
        matrix = M44f()
        scaling, shear = V3f(), V3f()
        matrix.scale(V3f(1, 2, 3))
        M44f.extractAndRemoveScalingAndShear(matrix, scaling, shear)
        expected_scaling = V3f(1, 2, 3)
        self.assertEqual(scaling, expected_scaling)
        matrix = M44f()
        scaling, shear = V3f(), V3f()
        matrix.shear(V3f(1, 2, 3))
        M44f.extractAndRemoveScalingAndShear(matrix, scaling, shear)
        expected_shear = V3f(1, 2, 3)
        self.assertEqual(shear, expected_shear)
        self.assertEqual(matrix, M44f())

    def test_extractEulerXYZ(self):
        """
        TODO: doesn't work
        """
        matrix = M44f().rotate(Eulerf(45, 30, 15))
        # result = M44f.extractEulerXYZ(matrix) # ?
        result = Eulerf()
        M44f.extractEulerXYZ(matrix, result)
        expected_result = Eulerf(45, 30, 15)
        # self.assertEqual(result, expected_result)

    # def test_extractEulerZYX(self):
    #     matrix = M44f().rotate(Eulerf(45, 30, 15))
    #     result = M44f.extractEulerZYX(matrix)
    #     expected_result = Eulerf(15, 30, 45)
    #     self.assertEqual(result, expected_result)

    def test_extractSHRT(self):
        """
        TODO: do 4 tests?
        """
        matrix = M44f().setTranslation(V3f(1, 2, 3)).rotate(Eulerf(45, 30, 15)).setScale(V3f(2, 3, 4)).setShear(V3f(0.1, 0.2, 0.3))
        s, h, r, t = V3f(), V3f(), V3f(), V3f()
        M44f.extractSHRT(matrix, s, h, r, t)
        expected_scale = V3f(2, 3, 4)
        expected_shear = V3f(0.1, 0.2, 0.3)
        expected_rotation = Eulerf(45, 30, 15)
        expected_translation = V3f(1, 2, 3)
        # self.assertEqual(s, expected_scale)
        # self.assertEqual(h, expected_shear)
        # self.assertEqual(r, expected_rotation)
        # self.assertEqual(t, expected_translation)

    def test_extractScaling(self):
        matrix = M44f().setScale(V3f(2, 3, 4))
        result = V3f()
        M44f.extractScaling(matrix, result)
        expected_result = V3f(2, 3, 4)
        self.assertEqual(result, expected_result)

    def test_extractScalingAndShear(self):
        """
        same as test_extractAndRemoveScalingAndShear
        except the matrix doesn't change
        """
        matrix = M44f()
        scaling, shear = V3f(), V3f()
        matrix.scale(V3f(1, 2, 3))
        M44f.extractScalingAndShear(matrix, scaling, shear)
        expected_scaling = V3f(1, 2, 3)
        self.assertEqual(scaling, expected_scaling)
        matrix = M44f()
        scaling, shear = V3f(), V3f()
        matrix.shear(V3f(1, 2, 3))
        M44f.extractScalingAndShear(matrix, scaling, shear)
        expected_shear = V3f(1, 2, 3)
        self.assertEqual(shear, expected_shear)

    # def test_fastMinor(self):
    #     matrix = M44f(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16)
    #     result = M44f.fastMinor(matrix, 1, 2, 3, 1)
    #     expected_result = 0.0
    #     self.assertEqual(result, expected_result)

    # def test_gjInverse(self):
    #     matrix = M44f(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16)
    #     result = M44f.gjInverse(matrix)
    #     expected_result = M44f(-2.0, 1.0, 0.0, 0.0, 1.5, -1.0, 0.5, 0.0, -1.0, 1.0, 0.0, 0.0, 0.5, -0.5, 0.0, 0.0)
    #     self.assertEqual(result, expected_result)

    # def test_gjInvert(self):
    #     matrix = M44f(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16)
    #     result = matrix.gjInvert()
    #     expected_result = M44f(-2.0, 1.0, 0.0, 0.0, 1.5, -1.0, 0.5, 0.0, -1.0, 1.0, 0.0, 0.0, 0.5, -0.5, 0.0, 0.0)
    #     self.assertEqual(result, expected_result)

    # def test_inverse(self):
    #     matrix = M44f(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16)
    #     result = matrix.inverse()
    #     expected_result = M44f(-2.0, 1.0, 0.0, 0.0, 1.5, -1.0, 0.5, 0.0, -1.0, 1.0, 0.0, 0.0, 0.5, -0.5, 0.0, 0.0)
    #     self.assertEqual(result, expected_result)

    # def test_invert(self):
    #     matrix = M44f(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16)
    #     result = matrix.invert()
    #     expected_result = M44f(-2.0, 1.0, 0.0, 0.0, 1.5, -1.0, 0.5, 0.0, -1.0, 1.0, 0.0, 0.0, 0.5, -0.5, 0.0, 0.0)
    #     self.assertEqual(result, expected_result)

    # def test_makeIdentity(self):
    #     result = M44f.makeIdentity()
    #     expected_result = M44f(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1)
    #     self.assertEqual(result, expected_result)

    # def test_minorOf(self):
    #     matrix = M44f(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16)
    #     result = M44f.minorOf(matrix, 1, 2, 3, 1)

    #     expected_result = 0.0
    #     self.assertEqual(result, expected_result)

    def test_multDirMatrix(self):
        direction = V3f(1, 0, 0)
        matrix = M44f().rotate(Eulerf(45, 0, 0))
        result = matrix.multDirMatrix(direction)
        expected_result = V3f(1, 0, 0)
        self.assertEqual(result, expected_result)

    def test_multVecMatrix(self):
        vector = V3f(1, 2, 3)
        matrix = M44f().setScale(V3f(2, 3, 4))
        result = matrix.multVecMatrix(vector)
        expected_result = V3f(2, 6, 12)
        self.assertEqual(result, expected_result)

    def test_negate(self):
        matrix = M44f(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16)
        result = M44f.negate(matrix)
        expected_result = M44f(-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12, -13, -14, -15, -16)
        self.assertEqual(result, expected_result)

    def test_removeScaling(self):
        matrix = M44f().setScale(V3f(2, 3, 4))
        matrix.removeScaling()
        self.assertEqual(matrix, M44f())

    # def test_removeScalingAndShear(self):
    #     matrix = M44f().setScale(V3f(2, 3, 4)).setShear(V3f(0.1, 0.2, 0.3)).setTranslation(V3f(1, 2, 3))
    #     result = M44f.removeScalingAndShear(matrix)

    #     expected_result = M44f().setTranslation(V3f(1, 2, 3))
    #     self.assertEqual(result, expected_result)

    # def test_rotate(self):
    #     matrix = M44f().rotate(Eulerf(45, 0, 0))
    #     vector = V3f(1, 0, 0)
    #     result = matrix.rotate(vector)
    #     expected_result = V3f(1, 0, 0)
    #     self.assertEqual(result, expected_result)

    # def test_rotationMatrix(self):
    #     angles = Eulerf(45, 30, 15)
    #     result = M44f.rotationMatrix(angles)
    #     expected_result = M44f().rotate(angles)
    #     self.assertEqual(result, expected_result)

    # def test_rotationMatrixWithUpDir(self):
    #     angles = Eulerf(45, 30, 15)
    #     up_direction = V3f(0, 1, 0)
    #     result = M44f.rotationMatrixWithUpDir(angles, up_direction)
    #     expected_result = M44f().rotate(angles)
    #     self.assertEqual(result, expected_result)

    def test_sansScaling(self):
        matrix = M44f().setScale(V3f(2, 3, 4)).setTranslation(V3f(1, 2, 3))
        result = M44f.sansScaling(matrix)
        expected_result = M44f().setTranslation(V3f(1, 2, 3))
        self.assertEqual(result, expected_result)

    # def test_sansScalingAndShear(self):
    #     matrix = M44f().setScale(V3f(2, 3, 4)).setShear(V3f(0.1, 0.2, 0.3)).setTranslation(V3f(1, 2, 3))
    #     result = M44f.sansScalingAndShear(matrix)
    #     expected_result = M44f().setTranslation(V3f(1, 2, 3))
    #     self.assertEqual(result, expected_result)

    # def test_scale(self):
    #     matrix = M44f().setScale(V3f(2, 3, 4))
    #     vector = V3f(1, 1, 1)
    #     result = matrix.scale(vector)
    #     expected_result = V3f(2, 3, 4)
    #     self.assertEqual(result, expected_result)

    # def test_setScale(self):
    #     scale_vector = V3f(2, 3, 4)
    #     result = M44f().setScale(scale_vector)
    #     expected_result = M44f(2, 0, 0, 0, 0, 3, 0, 0, 0, 0, 4, 0, 0, 0, 0, 1)
    #     self.assertEqual(result, expected_result)

    # def test_setShear(self):
    #     shear_vector = V3f(1, 2, 3)
    #     result = M44f().setShear(shear_vector)
    #     expected_result = M44f(1, 0, 0, 0, 2, 0, 0, 3, 0, 0, 1, 0, 0, 0, 0, 1)
    #     self.assertEqual(result, expected_result)

    # def test_setTranslation(self):
    #     translation_vector = V3f(1, 2, 3)
    #     result = M44f().setTranslation(translation_vector)
    #     expected_result = M44f(1, 0, 0, 1, 0, 1, 0, 2, 0, 0, 1, 3, 0, 0, 0, 1)
    #     self.assertEqual(result, expected_result)

    def test_setValue(self):
        values = M44f(
            1, 2, 3, 4,
            5, 6, 7, 8,
            9, 10, 11, 12,
            13, 14, 15, 16
        )
        result = M44f()
        result.setValue(values)
        expected_result = M44f(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16)
        self.assertEqual(result, expected_result)

    # def test_shear(self):
    #     shear_vector = V3f(1, 2, 3)
    #     result = M44f()
    #     result.setShear(shear_vector)
    #     expected_result = M44f(1, 0, 0, 0, 2, 0, 0, 3, 0, 0, 1, 0, 0, 0, 0, 1)
    #     self.assertEqual(result, expected_result)

    def test_singularValueDecomposition(self):
        """
        TODO this
        """
        matrix = M44f(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16)
        u, sigma, v_t = M44f.singularValueDecomposition(matrix, True)
        # SVD implementation uses two-sided Jacobi rotations to iteratively diagonalize the matrix
        # M44f, V4f, M44f
        # expected_u = M44f()
        # expected_sigma = M44f()
        # expected_v_t = M44f()
        # self.assertEqual(u, expected_u)
        # self.assertEqual(sigma, expected_sigma)
        # self.assertEqual(v_t, expected_v_t)

    # def test_symmetricEigensolve(self):
    #     matrix = M44f(1, 2, 3, 4, 2, 5, 6, 7, 3, 6, 8, 9, 4, 7, 9, 10)
    #     eigenvectors, eigenvalues = M44f.symmetricEigensolve(matrix)
    #     expected_eigenvectors = [V3f(), V3f(), V3f(), V3f()]
    #     expected_eigenvalues = V3f()
    #     self.assertEqual(eigenvectors, expected_eigenvectors)
    #     self.assertEqual(eigenvalues, expected_eigenvalues)

    def test_translate(self):
        matrix = M44f().setTranslation(V3f(1, 2, 3))
        vector = V3f(2, 3, 4)
        result = matrix.translate(vector)
        expected_result = V3f(3, 5, 7)
        self.assertEqual(result.translation(), expected_result)

    def test_translation(self):
        matrix = M44f().setTranslation(V3f(1, 2, 3))
        result = matrix.translation()
        expected_result = V3f(1, 2, 3)
        self.assertEqual(result, expected_result)

    def test_transpose(self):
        matrix = M44f(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16)
        result = M44f.transpose(matrix)
        expected_result = M44f(1, 5, 9, 13, 2, 6, 10, 14, 3, 7, 11, 15, 4, 8, 12, 16)
        self.assertEqual(result, expected_result)

    def test_transposed(self):
        matrix = M44f(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16)
        result = matrix.transposed()
        expected_result = M44f(1, 5, 9, 13, 2, 6, 10, 14, 3, 7, 11, 15, 4, 8, 12, 16)
        self.assertEqual(result, expected_result)

if __name__ == '__main__':
    unittest.main()
