# SPDX-License-Identifier: BSD-3-Clause
# Copyright Contributors to the OpenEXR Project.

import unittest
from imath import V3f

class V3fTestCase(unittest.TestCase):
    def test_baseTypeEpsilon(self):
        result = V3f.baseTypeEpsilon()
        self.assertEqual(result, 1.1920928955078125e-07)

    def test_baseTypeLowest(self):
        result = V3f.baseTypeLowest()
        self.assertEqual(result, -3.4028234663852886e+38)

    def test_baseTypeMax(self):
        result = V3f.baseTypeMax()
        self.assertEqual(result, 3.4028234663852886e+38)

    def test_baseTypeSmallest(self):
        result = V3f.baseTypeSmallest()
        self.assertEqual(result, 1.1754943508222875e-38)

    def test_closestVertex(self):
        vector = V3f(1, 2, 3)
        p1, p2, p3 =  V3f(1, 0, 0), V3f(0, 1, 0), V3f(0, 0, 1)
        result = V3f.closestVertex(vector, p1, p2, p3)
        expected_result = V3f(0, 0, 1)
        self.assertEqual(result, expected_result)

    def test_cross(self):
        vector1 = V3f(1, 2, 3)
        vector2 = V3f(4, 5, 6)
        result = V3f.cross(vector1, vector2)
        expected_result = V3f(-3, 6, -3)
        self.assertEqual(result, expected_result)

    def test_dimensions(self):
        result = V3f.dimensions()
        self.assertEqual(result, 3)

    def test_dot(self):
        vector1 = V3f(1, 2, 3)
        vector2 = V3f(4, 5, 6)
        result = V3f.dot(vector1, vector2)
        expected_result = 32
        self.assertEqual(result, expected_result)

    def test_equalWithAbsError(self):
        vector1 = V3f(1, 2, 3)
        vector2 = V3f(1.01, 2.02, 3.01)
        result = V3f.equalWithAbsError(vector1, vector2, 0.05)
        self.assertTrue(result)

    def test_equalWithRelError(self):
        vector1 = V3f(1, 2, 3)
        vector2 = V3f(1.01, 2.02, 3.01)
        result = V3f.equalWithRelError(vector1, vector2, 0.02)
        self.assertTrue(result)

    def test_length(self):
        vector = V3f(2, 4, 4)
        result = vector.length()
        expected_result = 6
        self.assertAlmostEqual(result, expected_result)

    def test_length2(self):
        vector = V3f(1, 2, 3)
        result = vector.length2()
        expected_result = 14
        self.assertEqual(result, expected_result)

    def test_negate(self):
        vector = V3f(1, 2, 3)
        vector.negate()
        expected_result = V3f(-1, -2, -3)
        self.assertEqual(vector, expected_result)

    def test_normalize(self):
        vector = V3f(1, 2, 3)
        normal_result = vector.normalize()
        expected_result = V3f(0.267261, 0.534522, 0.801783)
        result = V3f.equalWithAbsError(normal_result, expected_result, 0.01)
        self.assertTrue(result)

    def test_normalizeExc(self):
        vector = V3f(1, 2, 3)
        normal_result = vector.normalizeExc()
        expected_result = V3f(0.267261, 0.534522, 0.801783)
        result = V3f.equalWithAbsError(normal_result, expected_result, 0.01)
        self.assertTrue(result)

    def test_normalizeNonNull(self):
        vector = V3f(1, 2, 3)
        normal_result = vector.normalizeNonNull()
        expected_result = V3f(0.267261, 0.534522, 0.801783)
        result = V3f.equalWithAbsError(normal_result, expected_result, 0.01)
        self.assertTrue(result)

    def test_normalized(self):
        vector = V3f(1, 2, 3)
        normal_result = vector.normalized()
        expected_result = V3f(0.267261, 0.534522, 0.801783)
        result = V3f.equalWithAbsError(normal_result, expected_result, 0.01)
        self.assertTrue(result)

    def test_normalizedExc(self):
        vector = V3f(1, 2, 3)
        normal_result = vector.normalizedExc()
        expected_result = V3f(0.267261, 0.534522, 0.801783)
        result = V3f.equalWithAbsError(normal_result, expected_result, 0.01)
        self.assertTrue(result)

    def test_normalizedNonNull(self):
        vector = V3f(1, 2, 3)
        normal_result = vector.normalizedNonNull()
        expected_result = V3f(0.267261, 0.534522, 0.801783)
        result = V3f.equalWithAbsError(normal_result, expected_result, 0.01)
        self.assertTrue(result)

    def test_orthogonal(self):
        vector1 = V3f(2, 2, 0)
        vector2 = V3f(0, 1, 0)
        orthogonal_vec = V3f.orthogonal(vector1, vector2)
        expected_result = V3f(-0.5, 0.5, 0)
        result = V3f.equalWithAbsError(orthogonal_vec, expected_result, 0.01)
        self.assertTrue(result)

    def test_project(self):
        vector = V3f(1, 2, 3)
        direction = V3f(1, 0, 0)
        result = V3f.project(vector, direction)
        expected_result = V3f(1, 0, 0)
        self.assertEqual(result, expected_result)

    def test_reflect(self):
        vector = V3f(4, 5, 6)
        normal = V3f(1, 0, 0)
        result = V3f.reflect(vector, normal)
        expected_result = V3f(4, -5, -6)
        self.assertEqual(result, expected_result)

    def test_xyz(self):
        vector = V3f(1, 2, 3)
        self.assertEqual(vector.x, 1)
        self.assertEqual(vector.y, 2)
        self.assertEqual(vector.z, 3)

if __name__ == '__main__':
    unittest.main()
