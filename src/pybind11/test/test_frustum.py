import sys
import os
import pytest

import pybindimath

@pytest.mark.parametrize("Frustum", [pybindimath.Frustumf, pybindimath.Frustumd])
def test_frustum(Frustum):

    # The following tests do not check for the correctness of the arguments,
    # but if bindings work correctly

    f = Frustum()
    assert Frustum(f) is not None
    f = Frustum(1, 100, 0, 1, 2, 3.0, False)
    assert f.near() == pytest.approx(1.0)
    assert f.far() == pytest.approx(100.0)
    assert f.left() == pytest.approx(0.0)
    assert f.right() == pytest.approx(1.0)
    assert f.top() == pytest.approx(2.0)
    assert f.bottom() == pytest.approx(3.0)
    assert f.orthographic() is False

    f = Frustum(10, 1000, 0.5, 0.5, 1.0)
    assert f.near() == pytest.approx(10.0)
    assert f.far() == pytest.approx(1000.0)
    assert f.fovx() == pytest.approx(0.5)
    assert f.fovy() == pytest.approx(0.5)
    assert f.aspect() == pytest.approx(1.0)

    f.modifyNearAndFar(20, 2000.0)
    assert f.near() == pytest.approx(20.0)
    assert f.far() == pytest.approx(2000.0)

    m = f.projectionMatrix()
    assert m is not None
    assert m[0][0] is not None

    assert f.projectPointToScreen((1, 2, 3)) is not None
    with pytest.raises(ValueError) as e:
        f.projectPointToScreen((1, 2))

    assert str(e.value) == "projectPointToScreen expects a sequence of length 3"
