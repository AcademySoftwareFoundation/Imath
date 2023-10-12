# SPDX-License-Identifier: BSD-3-Clause
# Copyright Contributors to the OpenEXR Project.

import unittest
import imath

class Vec3Test(unittest.TestCase):
    def testV3f(self):
        v = imath.V3f(0.5, -0.25, 1.0)

        self.assertEqual(v.x, 0.5)
        self.assertEqual(v.y, -0.25)
        self.assertEqual(v.z, 1.0)

        v.negate()
        self.assertEqual(v.x, -0.5)
        self.assertEqual(v.y, 0.25)
        self.assertEqual(v.z, -1.0)
