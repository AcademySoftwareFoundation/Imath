//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

#ifdef NDEBUG
#    undef NDEBUG
#endif

#include <boost/python.hpp>
#include <boost/python/handle.hpp>
#include <boost/python/object.hpp>
#include <ImathMath.h>
#include <PyImathBox.h>
#include <PyImathColor.h>
#include <PyImathEuler.h>
#include <PyImathFrustum.h>
#include <PyImathLine.h>
#include <PyImathMatrix.h>
#include <PyImathPlane.h>
#include <PyImathQuat.h>
#include <PyImathRandom.h>
#include <PyImathVec.h>
#include <PyImathShear.h>

#include <testStringTable.h>

using namespace boost::python;

#define TEST(x) if (argc < 2 || !strcmp (argv[1], #x)) x();

PyObject *
wrapTester (PyObject * /* self */, PyObject *args)
{
    try {
        
        char *type;
        if (PyArg_ParseTuple (args, "s", &type))
        {
            std::string typeStr (type);

            if (typeStr == "Box2i")
                return PyImath::Box2i::wrap (Imath::Box2i (Imath::V2i (1, 2), 
                                                           Imath::V2i (3, 4)));
            else if (typeStr == "Box2f")
                return PyImath::Box2f::wrap (Imath::Box2f (Imath::V2f (1.1f, 2.2f), 
                                                           Imath::V2f (3.3f, 4.4f)));
            else if (typeStr == "Box2d")
                return PyImath::Box2d::wrap (Imath::Box2d (Imath::V2d (1.1, 2.2), 
                                                           Imath::V2d (3.3, 4.4)));

            if (typeStr == "Box3i")
                return PyImath::Box3i::wrap (Imath::Box3i (Imath::V3i (1, 2, 3), 
                                                           Imath::V3i (4, 5, 6)));
            else if (typeStr == "Box3f")
                return PyImath::Box3f::wrap (Imath::Box3f (Imath::V3f (1.1f, 2.2f, 3.3f), 
                                                           Imath::V3f (4.4f, 5.5f, 6.6f)));
            else if (typeStr == "Box3d")
                return PyImath::Box3d::wrap (Imath::Box3d (Imath::V3d (1.1, 2.2, 3.3), 
                                                           Imath::V3d (4.4, 5.5, 6.6)));

            else if (typeStr == "Color3c")
                return PyImath::Color3c::wrap (Imath::Color3c (1, 2, 3));
            else if (typeStr == "Color3f")
                return PyImath::Color3f::wrap (Imath::Color3f (1.1f, 2.2f, 3.3f));

            else if (typeStr == "Color4c")
                return PyImath::Color4c::wrap (Imath::Color4c (1, 2, 3, 4));
            else if (typeStr == "Color4f")
                return PyImath::Color4f::wrap (Imath::Color4f (1.1f, 2.2f, 3.3f, 4.4f));

            else if (typeStr == "Eulerf")
                return PyImath::Eulerf::wrap (Imath::Eulerf (Imath::V3f (1.1f, 2.2f, 3.3f), 
                                                             Imath::Eulerf::XZY));
            else if (typeStr == "Eulerd")
                return PyImath::Eulerd::wrap (Imath::Eulerd (Imath::V3d (1.1f, 2.2f, 3.3f), 
                                                             Imath::Eulerd::XZY));

            else if (typeStr == "Frustumf")
                return PyImath::Frustumf::wrap (Imath::Frustumf (1.1f, 2.2f, 3.3f,
                                                                 4.4f, 5.5f, 6.6f));
            else if (typeStr == "Frustumd")
                return PyImath::Frustumd::wrap (Imath::Frustumd (1.1, 2.2, 3.3,
                                                                 4.4, 5.5, 6.6));

            else if (typeStr == "Line3f")
                return PyImath::Line3f::wrap (Imath::Line3f (Imath::V3f (1.1f, 2.2f, 3.3f),
                                                             Imath::V3f (4.4f, 5.5f, 6.6f)));
            else if (typeStr == "Line3d")
                return PyImath::Line3d::wrap (Imath::Line3d (Imath::V3d (1.1, 2.2, 3.3),
                                                             Imath::V3d (4.4, 5.5, 6.6)));

            else if (typeStr == "M33f")
                return PyImath::M33f::wrap (Imath::M33f (1.1f, 2.2f, 3.3f,
                                                         4.4f, 5.5f, 6.6f,
                                                         7.7f, 8.8f, 9.9f));
            else if (typeStr == "M33d")
                return PyImath::M33d::wrap (Imath::M33d (1.1, 2.2, 3.3,
                                                         4.4, 5.5, 6.6,
                                                         7.7, 8.8, 9.9));

            else if (typeStr == "M44f")
                return PyImath::M44f::wrap (Imath::M44f (1.1f, 2.2f, 3.3f, 4.4f,
                                                         5.5f, 6.6f, 7.7f, 8.8f, 
                                                         9.9f, 10.10f, 11.11f, 12.12f,
                                                         13.13f, 14.14f, 15.15f, 16.16f));
            else if (typeStr == "M44d")
                return PyImath::M44d::wrap (Imath::M44d (1.1, 2.2, 3.3, 4.4,
                                                         5.5, 6.6, 7.7, 8.8, 
                                                         9.9, 10.10, 11.11, 12.12,
                                                         13.13, 14.14, 15.15, 16.16));

            else if (typeStr == "Plane3f")
                return PyImath::Plane3f::wrap (Imath::Plane3f (Imath::V3f (1.1f, 2.2f, 3.3f), 4.4f));
            else if (typeStr == "Plane3d")
                return PyImath::Plane3d::wrap (Imath::Plane3d (Imath::V3d (1.1, 2.2, 3.3), 4.4));

            else if (typeStr == "Quatf")
                return PyImath::Quatf::wrap (Imath::Quatf (1.1f, 2.2f, 3.3f, 4.4f));
            else if (typeStr == "Quatd")
                return PyImath::Quatd::wrap (Imath::Quatd (1.1, 2.2, 3.3, 4.4));

            else if (typeStr == "Rand32")
                return PyImath::Rand32::wrap (Imath::Rand32 ());
            else if (typeStr == "Rand48")
                return PyImath::Rand48::wrap (Imath::Rand48 ());

            else if (typeStr == "Shear6f")
                return PyImath::Shear6f::wrap (Imath::Shear6f (1.1f, 2.2f, 3.3f,
                                                               4.4f, 5.5f, 6.6f));
            else if (typeStr == "Shear6d")
                return PyImath::Shear6d::wrap (Imath::Shear6d (1.1, 2.2, 3.3,
                                                               4.4, 5.5, 6.6));

            else if (typeStr == "V2i")
                return PyImath::V2i::wrap (Imath::V2i (1, 2));
            else if (typeStr == "V2f")
                return PyImath::V2f::wrap (Imath::V2f (1.1f, 2.2f));
            else if (typeStr == "V2d")
                return PyImath::V2d::wrap (Imath::V2d (1.1, 2.2));

            else if (typeStr == "V3i")
                return PyImath::V3i::wrap (Imath::V3i (1, 2, 3));
            else if (typeStr == "V3f")
                return PyImath::V3f::wrap (Imath::V3f (1.1f, 2.2f, 3.3f));
            else if (typeStr == "V3d")
                return PyImath::V3d::wrap (Imath::V3d (1.1, 2.2, 3.3));
        }
        PyErr_Clear();

        PyErr_SetString(PyExc_TypeError, "wrap testing failed");
    }
    catch (const boost::python::error_already_set& e)	\
    {							\
	return 0;					\
    }							\
    catch (...)						\
    {							\
	boost::python::handle_exception();		\
	return 0;					\
    }

    return 0;
}

PyMethodDef methods[] =
{
    {"wrap", (PyCFunction) wrapTester, METH_VARARGS, ""}, {0, 0}
};

void
initWrapTester ()
{
#if PY_MAJOR_VERSION >= 3
    static struct PyModuleDef moduleDef =
        {
            PyModuleDef_HEAD_INIT,
            "wrap",
            "wrap module",
            -1,
            methods
        };

    PyObject* module   = PyModule_Create(&moduleDef);
    assert (module != 0);
    // I (jyost) don't know if there's a better way to do the following,
    // but this works.  (I did look at the docs and try various things to
    // see if I could find a better way, and this is what I landed on.)
    auto sysModule     = PyImport_AddModule("sys");
    auto sysModuleDict = PyModule_GetDict(sysModule);
    auto modulesDict   = PyDict_GetItemString(sysModuleDict, "modules");
    PyDict_SetItemString(modulesDict, moduleDef.m_name, module);
#else
    PyObject *module = Py_InitModule ("wrap", methods);
    assert (module != 0);
#endif
}


void
testBox2 ()
{
    object mainModule ((handle<> (borrowed (PyImport_AddModule ("__main__")))));
    object mainNamespace = mainModule.attr ("__dict__");

    std::cerr << "Testing Box2*:\n";

    std::string code = 
        "from imath import *\n"
        "b2i = Box2i ((1, 2), (3, 4))\n"
        "b2f = Box2f ((5.5, 6.6), (7.7, 8.8))\n"
        "b2d = Box2d ((9.9, 10.10), (11.11, 12.12))\n"
        "t2i = ((1, 2), (3, 4))\n"
        "t2V2i = (V2i (1, 2), V2i (3, 4))\n"
        "t2iMix = (V2i (1, 2), (3, 4))\n"
        "t2f = ((5.5, 6.6), (7.7, 8.8))\n"
        "t2V2f = (V2f (5.5, 6.6), V2f (7.7, 8.8))\n"
        "t2fMix = (V2f (5.5, 6.6), (7.7, 8.8))\n"
        "t2V2d = (V2d (9.9, 10.10), V2d (11.11, 12.12))\n"
        "t2dMix = (V2d (9.9, 10.10), (11.11, 12.12))\n"
        "i = 1\n"
        "from wrap import *\n"
        "w = wrap ('Box2i')\n"
        "assert w == Box2i ((1, 2), (3, 4))\n"
        "w = wrap ('Box2f')\n"
        "assert w.min().equalWithAbsError ((1.1, 2.2), 1e-7)\n"
        "assert w.max().equalWithAbsError ((3.3, 4.4), 1e-7)\n"
        "w = wrap ('Box2d')\n"
        "assert w.min().equalWithAbsError ((1.1, 2.2), 1e-7)\n"
        "assert w.max().equalWithAbsError ((3.3, 4.4), 1e-7)\n";

    handle<> ignored (PyRun_String (code.c_str(), Py_file_input, 
                                    mainNamespace.ptr(), mainNamespace.ptr()));
    //

    extract <object> eb2i (mainNamespace ["b2i"]);
    assert (eb2i.check());
    
    PyObject *b2iObj = eb2i().ptr();
    assert (b2iObj != 0);

    extract <object> eb2f (mainNamespace ["b2f"]);
    assert (eb2f.check());
    
    PyObject *b2fObj = eb2f().ptr();
    assert (b2fObj != 0);

    extract <object> eb2d (mainNamespace ["b2d"]);
    assert (eb2d.check());
    
    PyObject *b2dObj = eb2d().ptr();
    assert (b2dObj != 0);

    extract <object> et2i (mainNamespace["t2i"]);
    assert (et2i.check());

    PyObject *t2iObj = et2i().ptr();
    assert (t2iObj != 0);

    extract <object> et2V2i (mainNamespace["t2V2i"]);
    assert (et2V2i.check());

    PyObject *t2V2iObj = et2V2i().ptr();
    assert (t2V2iObj != 0);

    extract <object> et2iMix (mainNamespace["t2iMix"]);
    assert (et2iMix.check());

    PyObject *t2iMixObj = et2iMix().ptr();
    assert (t2iMixObj != 0);

    extract <object> et2f (mainNamespace["t2f"]);
    assert (et2f.check());

    PyObject *t2fObj = et2f().ptr();
    assert (t2fObj != 0);

    extract <object> et2V2f (mainNamespace["t2V2f"]);
    assert (et2V2f.check());

    PyObject *t2V2fObj = et2V2f().ptr();
    assert (t2V2fObj != 0);

    extract <object> et2fMix (mainNamespace["t2fMix"]);
    assert (et2fMix.check());

    PyObject *t2fMixObj = et2fMix().ptr();
    assert (t2fMixObj != 0);

    extract <object> et2V2d (mainNamespace["t2V2d"]);
    assert (et2V2d.check());

    PyObject *t2V2dObj = et2V2d().ptr();
    assert (t2V2dObj != 0);

    extract <object> et2dMix (mainNamespace["t2dMix"]);
    assert (et2dMix.check());

    PyObject *t2dMixObj = et2dMix().ptr();
    assert (t2dMixObj != 0);

    extract <object> ei (mainNamespace["i"]);
    assert (ei.check());

    PyObject *iObj = ei().ptr();
    assert (iObj != 0);

    //

    std::cerr << "Testing Box2i:\n";

    Imath::Box2i b2iAsB2i;
    assert (PyImath::Box2i::convert (b2iObj, &b2iAsB2i) == 1);
    assert (b2iAsB2i == Imath::Box2i (Imath::V2i (1, 2), 
                                      Imath::V2i (3, 4)));

    Imath::Box2i b2fAsB2i;
    assert (PyImath::Box2i::convert (b2fObj, &b2fAsB2i) == 1);
    assert (b2fAsB2i == Imath::Box2i (Imath::V2i (5, 6),
                                      Imath::V2i (7, 8)));

    Imath::Box2i b2dAsB2i;
    assert (PyImath::Box2i::convert (b2dObj, &b2dAsB2i) == 1);
    assert (b2dAsB2i == Imath::Box2i (Imath::V2i (9, 10),
                                      Imath::V2i (11, 12)));

    Imath::Box2i t2iAsB2i;
    assert (PyImath::Box2i::convert (t2iObj, &t2iAsB2i) == 1);
    assert (t2iAsB2i == Imath::Box2i (Imath::V2i (1, 2), 
                                      Imath::V2i (3, 4)));

    Imath::Box2i t2V2iAsB2i;
    assert (PyImath::Box2i::convert (t2V2iObj, &t2V2iAsB2i) == 1);
    assert (t2V2iAsB2i == Imath::Box2i (Imath::V2i (1, 2), 
                                        Imath::V2i (3, 4)));

    Imath::Box2i t2iMixAsB2i;
    assert (PyImath::Box2i::convert (t2iMixObj, &t2iMixAsB2i) == 1);
    assert (t2iMixAsB2i == Imath::Box2i (Imath::V2i (1, 2), 
                                         Imath::V2i (3, 4)));

    Imath::Box2i t2fAsB2i;
    assert (PyImath::Box2i::convert (t2fObj, &t2fAsB2i) == 1);
    assert (t2fAsB2i == Imath::Box2i (Imath::V2i (5, 6), 
                                      Imath::V2i (7, 8)));

    Imath::Box2i t2V2fAsB2i;
    assert (PyImath::Box2i::convert (t2V2fObj, &t2V2fAsB2i) == 1);
    assert (t2V2fAsB2i == Imath::Box2i (Imath::V2i (5, 6),
                                        Imath::V2i (7, 8)));

    Imath::Box2i t2fMixAsB2i;
    assert (PyImath::Box2i::convert (t2fMixObj, &t2fMixAsB2i) == 1);
    assert (t2fMixAsB2i == Imath::Box2i (Imath::V2i (5, 6), 
                                         Imath::V2i (7, 8)));

    Imath::Box2i t2V2dAsB2i;
    assert (PyImath::Box2i::convert (t2V2dObj, &t2V2dAsB2i) == 1);
    assert (t2V2dAsB2i == Imath::Box2i (Imath::V2i (9, 10),
                                        Imath::V2i (11, 12)));

    Imath::Box2i t2dMixAsB2i;
    assert (PyImath::Box2i::convert (t2dMixObj, &t2dMixAsB2i) == 1);
    assert (t2dMixAsB2i == Imath::Box2i (Imath::V2i (9, 10),
                                         Imath::V2i (11, 12)));

    Imath::Box2i iAsB2i;
    assert (PyImath::Box2i::convert (iObj, &iAsB2i) == 0);

    std::cerr << "ok\n";

    //

    std::cerr << "Testing Box2f:\n";

    Imath::Box2f b2iAsB2f;
    assert (PyImath::Box2f::convert (b2iObj, &b2iAsB2f) == 1);
    assert (b2iAsB2f.min.equalWithAbsError (Imath::V2f (1.0f, 2.0f), 1e-7));
    assert (b2iAsB2f.max.equalWithAbsError (Imath::V2f (3.0f, 4.0f), 1e-7));

    Imath::Box2f b2fAsB2f;
    assert (PyImath::Box2f::convert (b2fObj, &b2fAsB2f) == 1);
    assert (b2fAsB2f.min.equalWithAbsError (Imath::V2f (5.5f, 6.6f), 1e-7));
    assert (b2fAsB2f.max.equalWithAbsError (Imath::V2f (7.7f, 8.8f), 1e-7));

    Imath::Box2f b2dAsB2f;
    assert (PyImath::Box2f::convert (b2dObj, &b2dAsB2f) == 1);
    assert (b2dAsB2f.min.equalWithAbsError (Imath::V2f (9.9f, 10.10f), 1e-7));
    assert (b2dAsB2f.max.equalWithAbsError (Imath::V2f (11.11f, 12.12f), 1e-7));

    Imath::Box2f t2iAsB2f;
    assert (PyImath::Box2f::convert (t2iObj, &t2iAsB2f) == 1);
    assert (t2iAsB2f.min.equalWithAbsError (Imath::V2f (1.0f, 2.0f), 1e-7)); 
    assert (t2iAsB2f.max.equalWithAbsError (Imath::V2f (3.0f, 4.0f), 1e-7)); 

    Imath::Box2f t2V2iAsB2f;
    assert (PyImath::Box2f::convert (t2V2iObj, &t2V2iAsB2f) == 1);
    assert (t2V2iAsB2f.min.equalWithAbsError (Imath::V2f (1.0f, 2.0f), 1e-7));
    assert (t2V2iAsB2f.max.equalWithAbsError (Imath::V2f (3.0f, 4.0f), 1e-7));

    Imath::Box2f t2iMixAsB2f;
    assert (PyImath::Box2f::convert (t2iMixObj, &t2iMixAsB2f) == 1);
    assert (t2iMixAsB2f.min.equalWithAbsError (Imath::V2f (1.0f, 2.0f), 1e-7));
    assert (t2iMixAsB2f.max.equalWithAbsError (Imath::V2f (3.0, 4.0f), 1e-7));

    Imath::Box2f t2fAsB2f;
    assert (PyImath::Box2f::convert (t2fObj, &t2fAsB2f) == 1);
    assert (t2fAsB2f.min.equalWithAbsError (Imath::V2f (5.5f, 6.6f), 1e-7));
    assert (t2fAsB2f.max.equalWithAbsError (Imath::V2f (7.7f, 8.8f), 1e-7));

    Imath::Box2f t2V2fAsB2f;
    assert (PyImath::Box2f::convert (t2V2fObj, &t2V2fAsB2f) == 1);
    assert (t2V2fAsB2f.min.equalWithAbsError (Imath::V2f (5.5f, 6.6f), 1e-7));
    assert (t2V2fAsB2f.max.equalWithAbsError (Imath::V2f (7.7f, 8.8f), 1e-7));

    Imath::Box2f t2fMixAsB2f;
    assert (PyImath::Box2f::convert (t2fMixObj, &t2fMixAsB2f) == 1);
    assert (t2fMixAsB2f.min.equalWithAbsError (Imath::V2f (5.5f, 6.6f), 1e-7));
    assert (t2fMixAsB2f.max.equalWithAbsError (Imath::V2f (7.7f, 8.8f), 1e-7));

    Imath::Box2f t2V2dAsB2f;
    assert (PyImath::Box2f::convert (t2V2dObj, &t2V2dAsB2f) == 1);
    assert (t2V2dAsB2f.min.equalWithAbsError (Imath::V2f (9.9f, 10.10f), 1e-7));
    assert (t2V2dAsB2f.max.equalWithAbsError (Imath::V2f (11.11, 12.12f), 1e-7));

    Imath::Box2f t2dMixAsB2f;
    assert (PyImath::Box2f::convert (t2dMixObj, &t2dMixAsB2f) == 1);
    assert (t2dMixAsB2f.min.equalWithAbsError (Imath::V2f (9.9f, 10.10f), 1e-7));
    assert (t2dMixAsB2f.max.equalWithAbsError (Imath::V2f (11.11f, 12.12f), 1e-7));

    Imath::Box2f iAsB2f;
    assert (PyImath::Box2f::convert (iObj, &iAsB2f) == 0);

    std::cerr << "ok\n";

    //

    std::cerr << "Testing Box2d:\n";

    Imath::Box2d b2iAsB2d;
    assert (PyImath::Box2d::convert (b2iObj, &b2iAsB2d) == 1);
    assert (b2iAsB2d.min.equalWithAbsError (Imath::V2d (1.0, 2.0), 1e-7));
    assert (b2iAsB2d.max.equalWithAbsError (Imath::V2d (3.0, 4.0), 1e-7));

    Imath::Box2d b2fAsB2d;
    assert (PyImath::Box2d::convert (b2fObj, &b2fAsB2d) == 1);
    assert (b2fAsB2d.min.equalWithAbsError (Imath::V2d (5.5, 6.6), 1e-7));
    assert (b2fAsB2d.max.equalWithAbsError (Imath::V2d (7.7, 8.8), 1e-6));

    Imath::Box2d b2dAsB2d;
    assert (PyImath::Box2d::convert (b2dObj, &b2dAsB2d) == 1);
    assert (b2dAsB2d.min.equalWithAbsError (Imath::V2d (9.9, 10.10), 1e-6));
    assert (b2dAsB2d.max.equalWithAbsError (Imath::V2d (11.11, 12.12), 1e-6));

    Imath::Box2d t2iAsB2d;
    assert (PyImath::Box2d::convert (t2iObj, &t2iAsB2d) == 1);
    assert (t2iAsB2d.min.equalWithAbsError (Imath::V2d (1.0, 2.0), 1e-7)); 
    assert (t2iAsB2d.max.equalWithAbsError (Imath::V2d (3.0, 4.0), 1e-7)); 

    Imath::Box2d t2V2iAsB2d;
    assert (PyImath::Box2d::convert (t2V2iObj, &t2V2iAsB2d) == 1);
    assert (t2V2iAsB2d.min.equalWithAbsError (Imath::V2d (1.0, 2.0), 1e-7));
    assert (t2V2iAsB2d.max.equalWithAbsError (Imath::V2d (3.0, 4.0), 1e-7));

    Imath::Box2d t2iMixAsB2d;
    assert (PyImath::Box2d::convert (t2iMixObj, &t2iMixAsB2d) == 1);
    assert (t2iMixAsB2d.min.equalWithAbsError (Imath::V2d (1.0, 2.0), 1e-7));
    assert (t2iMixAsB2d.max.equalWithAbsError (Imath::V2d (3.0, 4.0), 1e-7));

    Imath::Box2d t2fAsB2d;
    assert (PyImath::Box2d::convert (t2fObj, &t2fAsB2d) == 1);
    assert (t2fAsB2d.min.equalWithAbsError (Imath::V2d (5.5, 6.6), 1e-7));
    assert (t2fAsB2d.max.equalWithAbsError (Imath::V2d (7.7, 8.8), 1e-7));

    Imath::Box2d t2V2fAsB2d;
    assert (PyImath::Box2d::convert (t2V2fObj, &t2V2fAsB2d) == 1);
    assert (t2V2fAsB2d.min.equalWithAbsError (Imath::V2d (5.5, 6.6), 1e-7));
    assert (t2V2fAsB2d.max.equalWithAbsError (Imath::V2d (7.7, 8.8), 1e-6));

    Imath::Box2d t2fMixAsB2d;
    assert (PyImath::Box2d::convert (t2fMixObj, &t2fMixAsB2d) == 1);
    assert (t2fMixAsB2d.min.equalWithAbsError (Imath::V2d (5.5, 6.6), 1e-7));
    assert (t2fMixAsB2d.max.equalWithAbsError (Imath::V2d (7.7, 8.8), 1e-7));

    Imath::Box2d t2V2dAsB2d;
    assert (PyImath::Box2d::convert (t2V2dObj, &t2V2dAsB2d) == 1);
    assert (t2V2dAsB2d.min.equalWithAbsError (Imath::V2d (9.9, 10.10), 1e-7));
    assert (t2V2dAsB2d.max.equalWithAbsError (Imath::V2d (11.11, 12.12), 1e-7));

    Imath::Box2d t2dMixAsB2d;
    assert (PyImath::Box2d::convert (t2dMixObj, &t2dMixAsB2d) == 1);
    assert (t2dMixAsB2d.min.equalWithAbsError (Imath::V2d (9.9, 10.10), 1e-7));
    assert (t2dMixAsB2d.max.equalWithAbsError (Imath::V2d (11.11, 12.12), 1e-7));

    Imath::Box2d iAsB2d;
    assert (PyImath::Box2d::convert (iObj, &iAsB2d) == 0);

    std::cerr << "ok\n";
}


void
testBox3 ()
{
    object mainModule ((handle<> (borrowed (PyImport_AddModule ("__main__")))));
    object mainNamespace = mainModule.attr ("__dict__");

    std::cerr << "Testing Box3*:\n";

    std::string code = 
        "from imath import *\n"
        "b3i = Box3i ((1, 2, 3), (4, 5, 6))\n"
        "b3f = Box3f ((7.7, 8.8, 9.9), (10.10, 11.11, 12.12))\n"
        "b3d = Box3d ((13.13, 14.14, 15.15), (16.16, 17.17, 18.18))\n"
        "t2i = ((1, 2, 3), (4, 5, 6))\n"
        "t2V3i = (V3i (1, 2, 3), V3i (4, 5, 6))\n"
        "t2iMix = (V3i (1, 2, 3), (4, 5, 6))\n"
        "t2f = ((7.7, 8.8, 9.9), (10.10, 11.11, 12.12))\n"
        "t2V3f = (V3f (7.7, 8.8, 9.9), V3f (10.10, 11.11, 12.12))\n"
        "t2fMix = (V3f (7.7, 8.8, 9.9), (10.10, 11.11, 12.12))\n"
        "t2V3d = (V3d (13.13, 14.14, 15.15), V3d (16.16, 17.17, 18.18))\n"
        "t2dMix = (V3d (13.13, 14.14, 15.15), (16.16, 17.17, 18.18))\n"
        "i = 1\n"
        "from wrap import *\n"
        "w = wrap ('Box3i')\n"
        "assert w == Box3i ((1, 2, 3), (4, 5, 6))\n"
        "w = wrap ('Box3f')\n"
        "assert w.min().equalWithAbsError ((1.1, 2.2, 3.3), 1e-7)\n"
        "assert w.max().equalWithAbsError ((4.4, 5.5, 6.6), 1e-7)\n"
        "w = wrap ('Box3d')\n"
        "assert w.min().equalWithAbsError ((1.1, 2.2, 3.3), 1e-7)\n"
        "assert w.max().equalWithAbsError ((4.4, 5.5, 6.6), 1e-7)\n";

    handle<> ignored (PyRun_String (code.c_str(), Py_file_input, 
                                    mainNamespace.ptr(), mainNamespace.ptr()));

    //

    extract <object> eb3i (mainNamespace ["b3i"]);
    assert (eb3i.check());
    
    PyObject *b3iObj = eb3i().ptr();
    assert (b3iObj != 0);

    extract <object> eb3f (mainNamespace ["b3f"]);
    assert (eb3f.check());
    
    PyObject *b3fObj = eb3f().ptr();
    assert (b3fObj != 0);

    extract <object> eb3d (mainNamespace ["b3d"]);
    assert (eb3d.check());
    
    PyObject *b3dObj = eb3d().ptr();
    assert (b3dObj != 0);

    extract <object> et2i (mainNamespace["t2i"]);
    assert (et2i.check());

    PyObject *t2iObj = et2i().ptr();
    assert (t2iObj != 0);

    extract <object> et2V3i (mainNamespace["t2V3i"]);
    assert (et2V3i.check());

    PyObject *t2V3iObj = et2V3i().ptr();
    assert (t2V3iObj != 0);

    extract <object> et2iMix (mainNamespace["t2iMix"]);
    assert (et2iMix.check());

    PyObject *t2iMixObj = et2iMix().ptr();
    assert (t2iMixObj != 0);

    extract <object> et2f (mainNamespace["t2f"]);
    assert (et2f.check());

    PyObject *t2fObj = et2f().ptr();
    assert (t2fObj != 0);

    extract <object> et2V3f (mainNamespace["t2V3f"]);
    assert (et2V3f.check());

    PyObject *t2V3fObj = et2V3f().ptr();
    assert (t2V3fObj != 0);

    extract <object> et2fMix (mainNamespace["t2fMix"]);
    assert (et2fMix.check());

    PyObject *t2fMixObj = et2fMix().ptr();
    assert (t2fMixObj != 0);

    extract <object> et2V3d (mainNamespace["t2V3d"]);
    assert (et2V3d.check());

    PyObject *t2V3dObj = et2V3d().ptr();
    assert (t2V3dObj != 0);

    extract <object> et2dMix (mainNamespace["t2dMix"]);
    assert (et2dMix.check());

    PyObject *t2dMixObj = et2dMix().ptr();
    assert (t2dMixObj != 0);

    extract <object> ei (mainNamespace["i"]);
    assert (ei.check());

    PyObject *iObj = ei().ptr();
    assert (iObj != 0);

    //

    std::cerr << "Testing Box3i:\n";

    Imath::Box3i b3iAsB3i;
    assert (PyImath::Box3i::convert (b3iObj, &b3iAsB3i) == 1);
    assert (b3iAsB3i == Imath::Box3i (Imath::V3i (1, 2, 3), 
                                      Imath::V3i (4, 5, 6)));

    Imath::Box3i b3fAsB3i;
    assert (PyImath::Box3i::convert (b3fObj, &b3fAsB3i) == 1);
    assert (b3fAsB3i == Imath::Box3i (Imath::V3i (7, 8, 9),
                                      Imath::V3i (10, 11, 12)));

    Imath::Box3i b3dAsB3i;
    assert (PyImath::Box3i::convert (b3dObj, &b3dAsB3i) == 1);
    assert (b3dAsB3i == Imath::Box3i (Imath::V3i (13, 14, 15),
                                      Imath::V3i (16, 17, 18)));

    Imath::Box3i t2iAsB3i;
    assert (PyImath::Box3i::convert (t2iObj, &t2iAsB3i) == 1);
    assert (t2iAsB3i == Imath::Box3i (Imath::V3i (1, 2, 3), 
                                      Imath::V3i (4, 5, 6)));

    Imath::Box3i t2V3iAsB3i;
    assert (PyImath::Box3i::convert (t2V3iObj, &t2V3iAsB3i) == 1);
    assert (t2V3iAsB3i == Imath::Box3i (Imath::V3i (1, 2, 3), 
                                        Imath::V3i (4, 5, 6)));

    Imath::Box3i t2iMixAsB3i;
    assert (PyImath::Box3i::convert (t2iMixObj, &t2iMixAsB3i) == 1);
    assert (t2iMixAsB3i == Imath::Box3i (Imath::V3i (1, 2, 3), 
                                         Imath::V3i (4, 5, 6)));

    Imath::Box3i t2fAsB3i;
    assert (PyImath::Box3i::convert (t2fObj, &t2fAsB3i) == 1);
    assert (t2fAsB3i == Imath::Box3i (Imath::V3i (7, 8, 9), 
                                      Imath::V3i (10, 11, 12)));

    Imath::Box3i t2V3fAsB3i;
    assert (PyImath::Box3i::convert (t2V3fObj, &t2V3fAsB3i) == 1);
    assert (t2V3fAsB3i == Imath::Box3i (Imath::V3i (7, 8, 9),
                                        Imath::V3i (10, 11, 12)));

    Imath::Box3i t2fMixAsB3i;
    assert (PyImath::Box3i::convert (t2fMixObj, &t2fMixAsB3i) == 1);
    assert (t2fMixAsB3i == Imath::Box3i (Imath::V3i (7, 8, 9), 
                                         Imath::V3i (10, 11, 12)));

    Imath::Box3i t2V3dAsB3i;
    assert (PyImath::Box3i::convert (t2V3dObj, &t2V3dAsB3i) == 1);
    assert (t2V3dAsB3i == Imath::Box3i (Imath::V3i (13, 14, 15),
                                        Imath::V3i (16, 17, 18)));

    Imath::Box3i t2dMixAsB3i;
    assert (PyImath::Box3i::convert (t2dMixObj, &t2dMixAsB3i) == 1);
    assert (t2dMixAsB3i == Imath::Box3i (Imath::V3i (13, 14, 15),
                                         Imath::V3i (16, 17, 18)));

    Imath::Box3i iAsB3i;
    assert (PyImath::Box3i::convert (iObj, &iAsB3i) == 0);

    std::cerr << "ok\n";

    //

    std::cerr << "Testing Box3f:\n";

    Imath::Box3f b3iAsB3f;
    assert (PyImath::Box3f::convert (b3iObj, &b3iAsB3f) == 1);
    assert (b3iAsB3f.min.equalWithAbsError (Imath::V3f (1.0f, 2.0f, 3.0f), 1e-7));
    assert (b3iAsB3f.max.equalWithAbsError (Imath::V3f (4.0f, 5.0f, 6.0f), 1e-7));

    Imath::Box3f b3fAsB3f;
    assert (PyImath::Box3f::convert (b3fObj, &b3fAsB3f) == 1);
    assert (b3fAsB3f.min.equalWithAbsError (Imath::V3f (7.7f, 8.8f, 9.9f), 1e-7));
    assert (b3fAsB3f.max.equalWithAbsError (Imath::V3f (10.10f, 11.11f, 12.12f), 1e-7));

    Imath::Box3f b3dAsB3f;
    assert (PyImath::Box3f::convert (b3dObj, &b3dAsB3f) == 1);
    assert (b3dAsB3f.min.equalWithAbsError (Imath::V3f (13.13f, 14.14f, 15.15f), 1e-7));
    assert (b3dAsB3f.max.equalWithAbsError (Imath::V3f (16.16f, 17.17f, 18.18f), 1e-7));

    Imath::Box3f t2iAsB3f;
    assert (PyImath::Box3f::convert (t2iObj, &t2iAsB3f) == 1);
    assert (t2iAsB3f.min.equalWithAbsError (Imath::V3f (1.0f, 2.0f, 3.0f), 1e-7)); 
    assert (t2iAsB3f.max.equalWithAbsError (Imath::V3f (4.0f, 5.0f, 6.0f), 1e-7)); 

    Imath::Box3f t2V3iAsB3f;
    assert (PyImath::Box3f::convert (t2V3iObj, &t2V3iAsB3f) == 1);
    assert (t2V3iAsB3f.min.equalWithAbsError (Imath::V3f (1.0f, 2.0f, 3.0f), 1e-7));
    assert (t2V3iAsB3f.max.equalWithAbsError (Imath::V3f (4.0f, 5.0f, 6.0f), 1e-7));

    Imath::Box3f t2iMixAsB3f;
    assert (PyImath::Box3f::convert (t2iMixObj, &t2iMixAsB3f) == 1);
    assert (t2iMixAsB3f.min.equalWithAbsError (Imath::V3f (1.0f, 2.0f, 3.0f), 1e-7));
    assert (t2iMixAsB3f.max.equalWithAbsError (Imath::V3f (4.0f, 5.0f, 6.0f), 1e-7));

    Imath::Box3f t2fAsB3f;
    assert (PyImath::Box3f::convert (t2fObj, &t2fAsB3f) == 1);
    assert (t2fAsB3f.min.equalWithAbsError (Imath::V3f (7.7f, 8.8f, 9.9f), 1e-7));
    assert (t2fAsB3f.max.equalWithAbsError (Imath::V3f (10.10f, 11.11f, 12.12f), 1e-7));

    Imath::Box3f t2V3fAsB3f;
    assert (PyImath::Box3f::convert (t2V3fObj, &t2V3fAsB3f) == 1);
    assert (t2V3fAsB3f.min.equalWithAbsError (Imath::V3f (7.7f, 8.8f, 9.9f), 1e-7));
    assert (t2V3fAsB3f.max.equalWithAbsError (Imath::V3f (10.10f, 11.11f, 12.12f), 1e-7));

    Imath::Box3f t2fMixAsB3f;
    assert (PyImath::Box3f::convert (t2fMixObj, &t2fMixAsB3f) == 1);
    assert (t2fMixAsB3f.min.equalWithAbsError (Imath::V3f (7.7f, 8.8f, 9.9f), 1e-7));
    assert (t2fMixAsB3f.max.equalWithAbsError (Imath::V3f (10.10f, 11.11f, 12.12f), 1e-7));

    Imath::Box3f t2V3dAsB3f;
    assert (PyImath::Box3f::convert (t2V3dObj, &t2V3dAsB3f) == 1);
    assert (t2V3dAsB3f.min.equalWithAbsError (Imath::V3f (13.13f, 14.14f, 15.15f), 1e-7));
    assert (t2V3dAsB3f.max.equalWithAbsError (Imath::V3f (16.16f, 17.17f, 18.18f), 1e-7));

    Imath::Box3f t2dMixAsB3f;
    assert (PyImath::Box3f::convert (t2dMixObj, &t2dMixAsB3f) == 1);
    assert (t2dMixAsB3f.min.equalWithAbsError (Imath::V3f (13.13f, 14.14f, 15.15f), 1e-7));
    assert (t2dMixAsB3f.max.equalWithAbsError (Imath::V3f (16.16f, 17.17f, 18.18f), 1e-7));

    Imath::Box3f iAsB3f;
    assert (PyImath::Box3f::convert (iObj, &iAsB3f) == 0);

    std::cerr << "ok\n";

    //

    std::cerr << "Testing Box3d:\n";

    Imath::Box3d b3iAsB3d;
    assert (PyImath::Box3d::convert (b3iObj, &b3iAsB3d) == 1);
    assert (b3iAsB3d.min.equalWithAbsError (Imath::V3d (1.0, 2.0, 3.0), 1e-7));
    assert (b3iAsB3d.max.equalWithAbsError (Imath::V3d (4.0, 5.0, 6.0), 1e-7));

    Imath::Box3d b3fAsB3d;
    assert (PyImath::Box3d::convert (b3fObj, &b3fAsB3d) == 1);
    assert (b3fAsB3d.min.equalWithAbsError (Imath::V3d (7.7, 8.8, 9.9), 1e-6));
    assert (b3fAsB3d.max.equalWithAbsError (Imath::V3d (10.10, 11.11, 12.12), 1e-6));

    Imath::Box3d b3dAsB3d;
    assert (PyImath::Box3d::convert (b3dObj, &b3dAsB3d) == 1);
    assert (b3dAsB3d.min.equalWithAbsError (Imath::V3d (13.13, 14.14, 15.15), 1e-6));
    assert (b3dAsB3d.max.equalWithAbsError (Imath::V3d (16.16, 17.17, 18.18), 1e-6));

    Imath::Box3d t2iAsB3d;
    assert (PyImath::Box3d::convert (t2iObj, &t2iAsB3d) == 1);
    assert (t2iAsB3d.min.equalWithAbsError (Imath::V3d (1.0, 2.0, 3.0), 1e-7)); 
    assert (t2iAsB3d.max.equalWithAbsError (Imath::V3d (4.0, 5.0, 6.0), 1e-7)); 

    Imath::Box3d t2V3iAsB3d;
    assert (PyImath::Box3d::convert (t2V3iObj, &t2V3iAsB3d) == 1);
    assert (t2V3iAsB3d.min.equalWithAbsError (Imath::V3d (1.0, 2.0, 3.0), 1e-7));
    assert (t2V3iAsB3d.max.equalWithAbsError (Imath::V3d (4.0, 5.0, 6.0), 1e-7));

    Imath::Box3d t2iMixAsB3d;
    assert (PyImath::Box3d::convert (t2iMixObj, &t2iMixAsB3d) == 1);
    assert (t2iMixAsB3d.min.equalWithAbsError (Imath::V3d (1.0, 2.0, 3.0), 1e-7));
    assert (t2iMixAsB3d.max.equalWithAbsError (Imath::V3d (4.0, 5.0, 6.0), 1e-7));

    Imath::Box3d t2fAsB3d;
    assert (PyImath::Box3d::convert (t2fObj, &t2fAsB3d) == 1);
    assert (t2fAsB3d.min.equalWithAbsError (Imath::V3d (7.7, 8.8, 9.9), 1e-7));
    assert (t2fAsB3d.max.equalWithAbsError (Imath::V3d (10.10, 11.11, 12.12), 1e-7));

    Imath::Box3d t2V3fAsB3d;
    assert (PyImath::Box3d::convert (t2V3fObj, &t2V3fAsB3d) == 1);
    assert (t2V3fAsB3d.min.equalWithAbsError (Imath::V3d (7.7, 8.8, 9.9), 1e-6));
    assert (t2V3fAsB3d.max.equalWithAbsError (Imath::V3d (10.10, 11.11, 12.12), 1e-6));

    Imath::Box3d t2fMixAsB3d;
    assert (PyImath::Box3d::convert (t2fMixObj, &t2fMixAsB3d) == 1);
    assert (t2fMixAsB3d.min.equalWithAbsError (Imath::V3d (7.7, 8.8, 9.9), 1e-6));
    assert (t2fMixAsB3d.max.equalWithAbsError (Imath::V3d (10.10, 11.11, 12.12), 1e-6));

    Imath::Box3d t2V3dAsB3d;
    assert (PyImath::Box3d::convert (t2V3dObj, &t2V3dAsB3d) == 1);
    assert (t2V3dAsB3d.min.equalWithAbsError (Imath::V3d (13.13, 14.14, 15.15), 1e-7));
    assert (t2V3dAsB3d.max.equalWithAbsError (Imath::V3d (16.16, 17.17, 18.18), 1e-7));

    Imath::Box3d t2dMixAsB3d;
    assert (PyImath::Box3d::convert (t2dMixObj, &t2dMixAsB3d) == 1);
    assert (t2dMixAsB3d.min.equalWithAbsError (Imath::V3d (13.13, 14.14, 15.15), 1e-7));
    assert (t2dMixAsB3d.max.equalWithAbsError (Imath::V3d (16.16, 17.17, 18.18), 1e-7));

    Imath::Box3d iAsB3d;
    assert (PyImath::Box3d::convert (iObj, &iAsB3d) == 0);

    std::cerr << "ok\n";
}


void
testC3 ()
{
    object mainModule ((handle<> (borrowed (PyImport_AddModule ("__main__")))));
    object mainNamespace = mainModule.attr ("__dict__");

    std::cerr << "Testing C3*:\n";

    std::string code = 
        "from imath import *\n"
        "c3c = Color3c (1, 2, 3)\n"
        "c3f = Color3f (1.1, 2.2, 3.3)\n"
        "t3c = (4, 5, 6)\n"
        "t3f = (4.4, 5.5, 6.6)\n"
        "v3i = V3i (7, 8, 9)\n"
        "v3f = V3f (10.10, 11.11, 12.12)\n"
        "v3d = V3f (13.13, 14.14, 15.15)\n"
        "l3c = [16, 17, 18]\n"
        "l3f = [19.19, 20.20, 21.21]\n"
        "i = 1\n"
        "from wrap import *\n"
        "w = wrap ('Color3c')\n"
        "assert w == Color3c (1, 2, 3)\n"
        "w = wrap ('Color3f')\n"
        "assert equal (w[0], 1.1, 1e-6)\n"
        "assert equal (w[1], 2.2, 1e-6)\n"
        "assert equal (w[2], 3.3, 1e-6)\n";

    handle<> ignored (PyRun_String (code.c_str(), Py_file_input, 
                                    mainNamespace.ptr(), mainNamespace.ptr()));

    //

    extract <object> ec3c (mainNamespace ["c3c"]);
    assert (ec3c.check());

    PyObject *c3cObj = ec3c().ptr();
    assert (c3cObj != 0);

    extract <object> ec3f (mainNamespace ["c3f"]);
    assert (ec3f.check());

    PyObject *c3fObj = ec3f().ptr();
    assert (c3fObj != 0);

    extract <object> et3c (mainNamespace ["t3c"]);
    assert (et3c.check());

    PyObject *t3cObj = et3c().ptr();
    assert (t3cObj != 0);

    extract <object> et3f (mainNamespace ["t3f"]);
    assert (et3f.check());

    PyObject *t3fObj = et3f().ptr();
    assert (t3fObj != 0);

    extract <object> ev3i (mainNamespace ["v3i"]);
    assert (ev3i.check());

    PyObject *v3iObj = ev3i().ptr();
    assert (v3iObj != 0);

    extract <object> ev3f (mainNamespace ["v3f"]);
    assert (ev3f.check());

    PyObject *v3fObj = ev3f().ptr();
    assert (v3fObj != 0);

    extract <object> ev3d (mainNamespace ["v3d"]);
    assert (ev3d.check());

    PyObject *v3dObj = ev3d().ptr();
    assert (v3dObj != 0);

    extract <object> el3c (mainNamespace ["l3c"]);
    assert (el3c.check());

    PyObject *l3cObj = el3c().ptr();
    assert (l3cObj != 0);

    extract <object> el3f (mainNamespace ["l3f"]);
    assert (el3f.check());

    PyObject *l3fObj = el3f().ptr();
    assert (l3fObj != 0);

    extract <object> ei (mainNamespace["i"]);
    assert (ei.check());

    PyObject *iObj = ei().ptr();
    assert (iObj != 0);

    //

    std::cerr << "Testing C3c:\n";

    Imath::C3c c3cAsC3c;
    assert (PyImath::C3c::convert (c3cObj, &c3cAsC3c) == 1);
    assert (c3cAsC3c == Imath::C3c (1, 2, 3));

    Imath::C3c c3fAsC3c;
    assert (PyImath::C3c::convert (c3fObj, &c3fAsC3c) == 1);
    assert (c3fAsC3c == Imath::C3c (1, 2, 3));

    Imath::C3c t3cAsC3c;
    assert (PyImath::C3c::convert (t3cObj, &t3cAsC3c) == 1);
    assert (t3cAsC3c == Imath::C3c (4, 5, 6));

    Imath::C3c t3fAsC3c;
    assert (PyImath::C3c::convert (t3fObj, &t3fAsC3c) == 1);
    assert (t3fAsC3c == Imath::C3c (4, 5, 6));

    Imath::C3c v3iAsC3c;
    assert (PyImath::C3c::convert (v3iObj, &v3iAsC3c) == 1);
    assert (v3iAsC3c == Imath::C3c (7, 8, 9));

    Imath::C3c v3fAsC3c;
    assert (PyImath::C3c::convert (v3fObj, &v3fAsC3c) == 1);
    assert (v3fAsC3c == Imath::C3c (10, 11, 12));

    Imath::C3c v3dAsC3c;
    assert (PyImath::C3c::convert (v3dObj, &v3dAsC3c) == 1);
    assert (v3dAsC3c == Imath::C3c (13, 14, 15));

    Imath::C3c l3cAsC3c;
    assert (PyImath::C3c::convert (l3cObj, &l3cAsC3c) == 1);
    assert (l3cAsC3c == Imath::C3c (16, 17, 18));

    Imath::C3c l3fAsC3c;
    assert (PyImath::C3c::convert (l3fObj, &l3fAsC3c) == 1);
    assert (l3fAsC3c == Imath::C3c (19, 20, 21));

    Imath::C3c iAsC3c;
    assert (PyImath::C3c::convert (iObj, &iAsC3c) == 0);

    std::cerr << "ok\n";

    //

    std::cerr << "Testing C3f:\n";

    Imath::C3f c3cAsC3f;
    assert (PyImath::C3f::convert (c3cObj, &c3cAsC3f) == 1);
    assert (c3cAsC3f.equalWithAbsError (Imath::C3f (1.0f, 2.0f, 3.0f), 1e-7));

    Imath::C3f c3fAsC3f;
    assert (PyImath::C3f::convert (c3fObj, &c3fAsC3f) == 1);
    assert (c3fAsC3f.equalWithAbsError (Imath::C3f (1.1f, 2.2f, 3.3f), 1e-7));

    Imath::C3f t3cAsC3f;
    assert (PyImath::C3f::convert (t3cObj, &t3cAsC3f) == 1);
    assert (t3cAsC3f.equalWithAbsError (Imath::C3f (4.0f, 5.0f, 6.0f), 1e-7));

    Imath::C3f t3fAsC3f;
    assert (PyImath::C3f::convert (t3fObj, &t3fAsC3f) == 1);
    assert (t3fAsC3f.equalWithAbsError (Imath::C3f (4.4f, 5.5f, 6.6f), 1e-7));

    Imath::C3f v3iAsC3f;
    assert (PyImath::C3f::convert (v3iObj, &v3iAsC3f) == 1);
    assert (v3iAsC3f.equalWithAbsError (Imath::C3f (7.0f, 8.0f, 9.0f), 1e-7));

    Imath::C3f v3fAsC3f;
    assert (PyImath::C3f::convert (v3fObj, &v3fAsC3f) == 1);
    assert (v3fAsC3f.equalWithAbsError (Imath::C3f (10.10f, 11.11f, 12.12f), 1e-7));

    Imath::C3f v3dAsC3f;
    assert (PyImath::C3f::convert (v3dObj, &v3dAsC3f) == 1);
    assert (v3dAsC3f.equalWithAbsError (Imath::C3f (13.13f, 14.14f, 15.15f), 1e-7));

    Imath::C3f l3cAsC3f;
    assert (PyImath::C3f::convert (l3cObj, &l3cAsC3f) == 1);
    assert (l3cAsC3f == Imath::C3f (16.0, 17.0, 18.0));

    Imath::C3f l3fAsC3f;
    assert (PyImath::C3f::convert (l3fObj, &l3fAsC3f) == 1);
    assert (l3fAsC3f == Imath::C3f (19.19, 20.20, 21.21));

    Imath::C3f iAsC3f;
    assert (PyImath::C3f::convert (iObj, &iAsC3f) == 0);

    std::cerr << "ok\n";
}


void
testC4 ()
{
    object mainModule ((handle<> (borrowed (PyImport_AddModule ("__main__")))));
    object mainNamespace = mainModule.attr ("__dict__");

    std::cerr << "Testing C4*:\n";

    std::string code = 
        "from imath import *\n"
        "c4c = Color4c (1, 2, 3, 4)\n"
        "c4f = Color4f (1.1, 2.2, 3.3, 4.4)\n"
        "t4c = (5, 6, 7, 8)\n"
        "t4f = (5.5, 6.6, 7.7, 8.8)\n"
        "l4c = [9, 10, 11, 12]\n"
        "l4f = [13.13, 14.14, 15.15, 16.16]\n"
        "i = 1\n"
        "from wrap import *\n"
        "w = wrap ('Color4c')\n"
        "assert w == Color4c (1, 2, 3, 4)\n"
        "w = wrap ('Color4f')\n"
        "assert equal (w[0], 1.1, 1e-7)\n"
        "assert equal (w[1], 2.2, 1e-7)\n"
        "assert equal (w[2], 3.3, 1e-7)\n"
        "assert equal (w[3], 4.4, 1e-7)\n";

    handle<> ignored (PyRun_String (code.c_str(), Py_file_input, 
                                    mainNamespace.ptr(), mainNamespace.ptr()));

    //

    extract <object> ec4c (mainNamespace ["c4c"]);
    assert (ec4c.check());

    PyObject *c4cObj = ec4c().ptr();
    assert (c4cObj != 0);

    extract <object> ec4f (mainNamespace ["c4f"]);
    assert (ec4f.check());

    PyObject *c4fObj = ec4f().ptr();
    assert (c4fObj != 0);

    extract <object> et4c (mainNamespace ["t4c"]);
    assert (et4c.check());

    PyObject *t4cObj = et4c().ptr();
    assert (t4cObj != 0);

    extract <object> et4f (mainNamespace ["t4f"]);
    assert (et4f.check());

    PyObject *t4fObj = et4f().ptr();
    assert (t4fObj != 0);

    extract <object> el4c (mainNamespace ["l4c"]);
    assert (el4c.check());

    PyObject *l4cObj = el4c().ptr();
    assert (l4cObj != 0);

    extract <object> el4f (mainNamespace ["l4f"]);
    assert (el4f.check());

    PyObject *l4fObj = el4f().ptr();
    assert (l4fObj != 0);

    extract <object> ei (mainNamespace["i"]);
    assert (ei.check());

    PyObject *iObj = ei().ptr();
    assert (iObj != 0);

    //

    std::cerr << "Testing C4c:\n";

    Imath::C4c c4cAsC4c;
    assert (PyImath::C4c::convert (c4cObj, &c4cAsC4c) == 1);
    assert (c4cAsC4c == Imath::C4c (1, 2, 3, 4));

    Imath::C4c c4fAsC4c;
    assert (PyImath::C4c::convert (c4fObj, &c4fAsC4c) == 1);
    assert (c4fAsC4c == Imath::C4c (1, 2, 3, 4));

    Imath::C4c t4cAsC4c;
    assert (PyImath::C4c::convert (t4cObj, &t4cAsC4c) == 1);
    assert (t4cAsC4c == Imath::C4c (5, 6, 7, 8));

    Imath::C4c t4fAsC4c;
    assert (PyImath::C4c::convert (t4fObj, &t4fAsC4c) == 1);
    assert (t4fAsC4c == Imath::C4c (5, 6, 7, 8));

    Imath::C4c l4cAsC4c;
    assert (PyImath::C4c::convert (l4cObj, &l4cAsC4c) == 1);
    assert (l4cAsC4c == Imath::C4c (9, 10, 11, 12));

    Imath::C4c l4fAsC4c;
    assert (PyImath::C4c::convert (l4fObj, &l4fAsC4c) == 1);
    assert (l4fAsC4c == Imath::C4c (13, 14, 15, 16));

    Imath::C4c iAsC4c;
    assert (PyImath::C4c::convert (iObj, &iAsC4c) == 0);

    std::cerr << "ok\n";

    //

    std::cerr << "Testing C4f:\n";

    Imath::C4f c4cAsC4f;
    assert (PyImath::C4f::convert (c4cObj, &c4cAsC4f) == 1);
    assert (Imath::equalWithAbsError (c4cAsC4f[0], 1.0f, 1e-7f));
    assert (Imath::equalWithAbsError (c4cAsC4f[1], 2.0f, 1e-7f));
    assert (Imath::equalWithAbsError (c4cAsC4f[2], 3.0f, 1e-7f));
    assert (Imath::equalWithAbsError (c4cAsC4f[3], 4.0f, 1e-7f));

    Imath::C4f c4fAsC4f;
    assert (PyImath::C4f::convert (c4fObj, &c4fAsC4f) == 1);
    assert (Imath::equalWithAbsError (c4fAsC4f[0], 1.1f, 1e-7f));
    assert (Imath::equalWithAbsError (c4fAsC4f[1], 2.2f, 1e-7f));
    assert (Imath::equalWithAbsError (c4fAsC4f[2], 3.3f, 1e-7f));
    assert (Imath::equalWithAbsError (c4fAsC4f[3], 4.4f, 1e-7f));

    Imath::C4f t4cAsC4f;
    assert (PyImath::C4f::convert (t4cObj, &t4cAsC4f) == 1);
    assert (Imath::equalWithAbsError (t4cAsC4f[0], 5.0f, 1e-7f));
    assert (Imath::equalWithAbsError (t4cAsC4f[1], 6.0f, 1e-7f));
    assert (Imath::equalWithAbsError (t4cAsC4f[2], 7.0f, 1e-7f));
    assert (Imath::equalWithAbsError (t4cAsC4f[3], 8.0f, 1e-7f));

    Imath::C4f t4fAsC4f;
    assert (PyImath::C4f::convert (t4fObj, &t4fAsC4f) == 1);
    assert (Imath::equalWithAbsError (t4fAsC4f[0], 5.5f, 1e-7f));
    assert (Imath::equalWithAbsError (t4fAsC4f[1], 6.6f, 1e-7f));
    assert (Imath::equalWithAbsError (t4fAsC4f[2], 7.7f, 1e-7f));
    assert (Imath::equalWithAbsError (t4fAsC4f[3], 8.8f, 1e-7f));

    Imath::C4f l4cAsC4f;
    assert (PyImath::C4f::convert (l4cObj, &l4cAsC4f) == 1);
    assert (Imath::equalWithAbsError (l4cAsC4f[0], 9.0f, 1e-7f));
    assert (Imath::equalWithAbsError (l4cAsC4f[1], 10.0f, 1e-7f));
    assert (Imath::equalWithAbsError (l4cAsC4f[2], 11.0f, 1e-7f));
    assert (Imath::equalWithAbsError (l4cAsC4f[3], 12.0f, 1e-7f));

    Imath::C4f l4fAsC4f;
    assert (PyImath::C4f::convert (l4fObj, &l4fAsC4f) == 1);
    assert (Imath::equalWithAbsError (l4fAsC4f[0], 13.13f, 1e-7f));
    assert (Imath::equalWithAbsError (l4fAsC4f[1], 14.14f, 1e-7f));
    assert (Imath::equalWithAbsError (l4fAsC4f[2], 15.15f, 1e-7f));
    assert (Imath::equalWithAbsError (l4fAsC4f[3], 16.16f, 1e-7f));

    Imath::C4f iAsC4f;
    assert (PyImath::C4f::convert (iObj, &iAsC4f) == 0);

    std::cerr << "ok\n";
}


void
testEuler ()
{
    object mainModule ((handle<> (borrowed (PyImport_AddModule ("__main__")))));
    object mainNamespace = mainModule.attr ("__dict__");

    std::cerr << "Testing Euler*:\n";

    std::string code = 
        "from imath import *\n"
        "ef = Eulerf (V3f(1.1, 2.2, 3.3), EULER_XZY)\n"
        "ed = Eulerd (V3d(4.4, 5.5, 6.6), EULER_ZYX)\n"
        "i = 1\n"
        "from wrap import *\n"
        "w = wrap ('Eulerf')\n"
        "assert equal (w.x, 1.1, 1e-6)\n"
        "assert equal (w.y, 2.2, 1e-6)\n"
        "assert equal (w.z, 3.3, 1e-6)\n"
        "assert w.order() == EULER_XZY\n"
        "w = wrap ('Eulerd')\n"
        "assert equal (w.x, 1.1, 1e-6)\n"
        "assert equal (w.y, 2.2, 1e-6)\n"
        "assert equal (w.z, 3.3, 1e-6)\n"
        "assert w.order() == EULER_XZY\n";

    handle<> ignored (PyRun_String (code.c_str(), Py_file_input, 
                                    mainNamespace.ptr(), mainNamespace.ptr()));

    //

    extract <object> eef (mainNamespace["ef"]);
    assert (eef.check());

    PyObject *efObj = eef().ptr();
    assert (efObj != 0);

    extract <object> eed (mainNamespace["ed"]);
    assert (eed.check());

    PyObject *edObj = eed().ptr();
    assert (edObj != 0);

    extract <object> ei (mainNamespace["i"]);
    assert (ei.check());

    PyObject *iObj = ei().ptr();
    assert (iObj != 0);

    //
    
    std::cerr << "Testing Eulerf:\n";

    Imath::Eulerf efAsEf;
    assert (PyImath::Eulerf::convert (efObj, &efAsEf) == 1);
    Imath::Eulerf eff (Imath::V3f (1.1f, 2.2f, 3.3f), Imath::Eulerf::XZY);
    assert (efAsEf.order() == eff.order());
    assert (efAsEf.toMatrix44().equalWithAbsError (eff.toMatrix44(), 1e-7));

    Imath::Eulerf edAsEf;
    assert (PyImath::Eulerf::convert (edObj, &edAsEf) == 1);
    Imath::Eulerf edf (Imath::V3f (4.4f, 5.5f, 6.6f), Imath::Eulerf::ZYX);
    assert (edAsEf.order() == edf.order());
    assert (edAsEf.toMatrix44().equalWithAbsError (edf.toMatrix44(), 1e-6));

    Imath::Eulerf iAsEf;
    assert (PyImath::Eulerf::convert (iObj, &iAsEf) == 0);

    std::cerr << "ok\n";

    //

    std::cerr << "Testing Eulerd:\n";

    Imath::Eulerd efAsEd;
    assert (PyImath::Eulerd::convert (efObj, &efAsEd) == 1);
    Imath::Eulerd efd (Imath::V3d (1.1, 2.2, 3.3), Imath::Eulerd::XZY);
    assert (efAsEd.order() == efd.order());
    assert (efAsEd.toMatrix44().equalWithAbsError (efd.toMatrix44(), 1e-7));

    Imath::Eulerd edAsEd;
    assert (PyImath::Eulerd::convert (edObj, &edAsEd) == 1);
    Imath::Eulerd edd (Imath::V3d (4.4, 5.5, 6.6), Imath::Eulerd::ZYX);
    assert (edAsEd.order() == edd.order());
    assert (edAsEd.toMatrix44().equalWithAbsError (edd.toMatrix44(), 1e-6));

    Imath::Eulerd iAsEd;
    assert (PyImath::Eulerd::convert (iObj, &iAsEd) == 0);

    std::cerr << "ok\n";
}


void
testFrustum ()
{
    object mainModule ((handle<> (borrowed (PyImport_AddModule ("__main__")))));
    object mainNamespace = mainModule.attr ("__dict__");

    std::cerr << "Testing Frustum*:\n";

    std::string code = 
        "from imath import *\n"
        "ff = Frustumf (-1.0, -100.0, -2.0, 2.0, 3.0, -3.0, 0)\n"
        "fd = Frustumf (-2.0, -50.0, -3.0, 3.0, 2.0, -2.0, 1)\n"
        "i = 1\n"
        "from wrap import *\n"
        "w = wrap ('Frustumf')\n"
        "assert equal (w.nearPlane(), 1.1, 1e-7)\n"
        "assert equal (w.farPlane(), 2.2, 1e-7)\n"
        "assert equal (w.left(), 3.3, 1e-7)\n"
        "assert equal (w.right(), 4.4, 1e-7)\n"
        "assert equal (w.top(), 5.5, 1e-7)\n"
        "assert equal (w.bottom(), 6.6, 1e-7)\n"
        "assert w.orthographic() == 0\n"
        "w = wrap ('Frustumd')\n"
        "assert equal (w.nearPlane(), 1.1, 1e-6)\n"
        "assert equal (w.farPlane(), 2.2, 1e-7)\n"
        "assert equal (w.left(), 3.3, 1e-7)\n"
        "assert equal (w.right(), 4.4, 1e-7)\n"
        "assert equal (w.top(), 5.5, 1e-7)\n"
        "assert equal (w.bottom(), 6.6, 1e-7)\n"
        "assert w.orthographic() == 0\n";

    handle<> ignored (PyRun_String (code.c_str(), Py_file_input, 
                                    mainNamespace.ptr(), mainNamespace.ptr()));

    //

    extract <object> eff (mainNamespace["ff"]);
    assert (eff.check());

    PyObject *ffObj = eff().ptr();
    assert (ffObj != 0);

    extract <object> efd (mainNamespace["fd"]);
    assert (efd.check());

    PyObject *fdObj = efd().ptr();
    assert (fdObj != 0);

    extract <object> ei (mainNamespace["i"]);
    assert (ei.check());

    PyObject *iObj = ei().ptr();
    assert (iObj != 0);

    //
    
    std::cerr << "Testing Frustumf:\n";

    Imath::Frustumf ffAsFf;
    assert (PyImath::Frustumf::convert (ffObj, &ffAsFf) == 1);
    assert (Imath::equalWithAbsError (ffAsFf.nearPlane(), -1.0f, 1e-7f));
    assert (Imath::equalWithAbsError (ffAsFf.farPlane(), -100.0f, 1e-7f));
    assert (Imath::equalWithAbsError (ffAsFf.left(), -2.0f, 1e-7f));
    assert (Imath::equalWithAbsError (ffAsFf.right(), 2.0f, 1e-7f));
    assert (Imath::equalWithAbsError (ffAsFf.top(), 3.0f, 1e-7f));
    assert (Imath::equalWithAbsError (ffAsFf.bottom(), -3.0f, 1e-7f));
    assert (ffAsFf.orthographic() == false);

    Imath::Frustumf fdAsFf;
    assert (PyImath::Frustumf::convert (fdObj, &fdAsFf) == 1);
    assert (Imath::equalWithAbsError (fdAsFf.nearPlane(), -2.0f, 1e-7f));
    assert (Imath::equalWithAbsError (fdAsFf.farPlane(), -50.0f, 1e-7f));
    assert (Imath::equalWithAbsError (fdAsFf.left(), -3.0f, 1e-7f));
    assert (Imath::equalWithAbsError (fdAsFf.right(), 3.0f, 1e-7f));
    assert (Imath::equalWithAbsError (fdAsFf.top(), 2.0f, 1e-7f));
    assert (Imath::equalWithAbsError (fdAsFf.bottom(), -2.0f, 1e-7f));
    assert (fdAsFf.orthographic() == true);

    Imath::Frustumf iAsFf;
    assert (PyImath::Frustumf::convert (iObj, &iAsFf) == 0);

    std::cerr << "ok\n";

    //

    std::cerr << "Testing Frustumd:\n";

    Imath::Frustumd ffAsFd;
    assert (PyImath::Frustumd::convert (ffObj, &ffAsFd) == 1);
    assert (Imath::equalWithAbsError (ffAsFd.nearPlane(), -1.0, 1e-7));
    assert (Imath::equalWithAbsError (ffAsFd.farPlane(), -100.0, 1e-7));
    assert (Imath::equalWithAbsError (ffAsFd.left(), -2.0, 1e-7));
    assert (Imath::equalWithAbsError (ffAsFd.right(), 2.0, 1e-7));
    assert (Imath::equalWithAbsError (ffAsFd.top(), 3.0, 1e-7));
    assert (Imath::equalWithAbsError (ffAsFd.bottom(), -3.0, 1e-7));
    assert (ffAsFd.orthographic() == false);

    Imath::Frustumd fdAsFd;
    assert (PyImath::Frustumd::convert (fdObj, &fdAsFd) == 1);
    assert (Imath::equalWithAbsError (fdAsFd.nearPlane(), -2.0, 1e-7));
    assert (Imath::equalWithAbsError (fdAsFd.farPlane(), -50.0, 1e-7));
    assert (Imath::equalWithAbsError (fdAsFd.left(), -3.0, 1e-7));
    assert (Imath::equalWithAbsError (fdAsFd.right(), 3.0, 1e-7));
    assert (Imath::equalWithAbsError (fdAsFd.top(), 2.0, 1e-7));
    assert (Imath::equalWithAbsError (fdAsFd.bottom(), -2.0, 1e-7));
    assert (fdAsFd.orthographic() == true);

    Imath::Frustumf iAsFd;
    assert (PyImath::Frustumf::convert (iObj, &iAsFd) == 0);

    std::cerr << "ok\n";
}


void
testLine ()
{
    object mainModule ((handle<> (borrowed (PyImport_AddModule ("__main__")))));
    object mainNamespace = mainModule.attr ("__dict__");

    std::cerr << "Testing Line*:\n";

    std::string code = 
        "from imath import *\n"
        "l3f = Line3f (V3f (1.1, 2.2, 3.3), V3f (4.4, 5.5, 6.6))\n"
        "l3d = Line3d (V3d (7.7, 8.8, 9.9), V3d (10.10, 11.11, 12.12))\n"
        "i = 1\n"
        "from wrap import *\n"
        "w = wrap ('Line3f')\n"
        "p1 = V3f (1.1, 2.2, 3.3)\n"
        "p2 = V3f (4.4, 5.5, 6.6)\n"
        "d = (p2 - p1).normalize()\n"
        "assert w.pos().equalWithAbsError (p1, 1e-6)\n"
        "assert w.dir().equalWithAbsError (d, 1e-6)\n"
        "w = wrap ('Line3d')\n"
        "p1 = V3d (1.1, 2.2, 3.3)\n"
        "p2 = V3d (4.4, 5.5, 6.6)\n"
        "d = (p2 - p1).normalize()\n"
        "assert w.pos().equalWithAbsError (p1, 1e-6)\n"
        "assert w.dir().equalWithAbsError (d, 1e-6)\n";

    handle<> ignored (PyRun_String (code.c_str(), Py_file_input, 
                                    mainNamespace.ptr(), mainNamespace.ptr()));

    //

    extract <object> el3f (mainNamespace["l3f"]);
    assert (el3f.check());

    PyObject *l3fObj = el3f().ptr();
    assert (l3fObj != 0);

    extract <object> el3d (mainNamespace["l3d"]);
    assert (el3d.check());

    PyObject *l3dObj = el3d().ptr();
    assert (l3dObj != 0);

    extract <object> ei (mainNamespace["i"]);
    assert (ei.check());

    PyObject *iObj = ei().ptr();
    assert (iObj != 0);

    //
    
    std::cerr << "Testing Line3f:\n";

    Imath::Line3f l3fAsL3f;
    assert (PyImath::Line3f::convert (l3fObj, &l3fAsL3f) == 1);
    Imath::V3f l3fP1f (1.1f, 2.2f, 3.3f);
    Imath::V3f l3fP2f (4.4f, 5.5f, 6.6f);
    Imath::V3f l3fPosf = l3fP1f;
    Imath::V3f l3fDirf = (l3fP2f - l3fP1f).normalize();
    assert (l3fAsL3f.pos.equalWithAbsError (l3fPosf, 1e-7));
    assert (l3fAsL3f.dir.equalWithAbsError (l3fDirf, 1e-7));

    Imath::Line3f l3dAsL3f;
    assert (PyImath::Line3f::convert (l3dObj, &l3dAsL3f) == 1);
    Imath::V3f l3dP1f (7.7f, 8.8f, 9.9f);
    Imath::V3f l3dP2f (10.10f, 11.11f, 12.12f);
    Imath::V3f l3dPosf = l3dP1f;
    Imath::V3f l3dDirf = (l3dP2f - l3dP1f).normalize();
    assert (l3dAsL3f.pos.equalWithAbsError (l3dPosf, 1e-7));
    assert (l3dAsL3f.dir.equalWithAbsError (l3dDirf, 1e-6));

    Imath::Line3f iAsL3f;
    assert (PyImath::Line3f::convert (iObj, &iAsL3f) == 0);

    std::cerr << "ok\n";

    //

    std::cerr << "Testing Line3d:\n";

    Imath::Line3d l3fAsL3d;
    assert (PyImath::Line3d::convert (l3fObj, &l3fAsL3d) == 1);
    Imath::V3d l3fP1d (1.1, 2.2, 3.3);
    Imath::V3d l3fP2d (4.4, 5.5, 6.6);
    Imath::V3d l3fPosd = l3fP1d;
    Imath::V3d l3fDird = (l3fP2d - l3fP1d).normalize();
    assert (l3fAsL3d.pos.equalWithAbsError (l3fPosd, 1e-7));
    assert (l3fAsL3d.dir.equalWithAbsError (l3fDird, 1e-7));

    Imath::Line3d l3dAsL3d;
    assert (PyImath::Line3d::convert (l3dObj, &l3dAsL3d) == 1);
    Imath::V3d l3dP1d (7.7, 8.8, 9.9);
    Imath::V3d l3dP2d (10.10, 11.11, 12.12);
    Imath::V3d l3dPosd = l3dP1d;
    Imath::V3d l3dDird = (l3dP2d - l3dP1d).normalize();
    assert (l3dAsL3d.pos.equalWithAbsError (l3dPosd, 1e-7));
    assert (l3dAsL3d.dir.equalWithAbsError (l3dDird, 1e-7));

    Imath::Line3d iAsL3d;
    assert (PyImath::Line3d::convert (iObj, &iAsL3d) == 0);

    std::cerr << "ok\n";
}


void
testM33 ()
{
    object mainModule ((handle<> (borrowed (PyImport_AddModule ("__main__")))));
    object mainNamespace = mainModule.attr ("__dict__");

    std::cerr << "Testing M33*:\n";

    std::string code = 
        "from imath import *\n"
        "m33f = M33f ((1.1, 2.2, 3.3),\n"
        "             (4.4, 5.5, 6.6),\n"
        "             (7.7, 8.8, 9.9))\n"
        "m33d = M33d ((10.10, 11.11, 12.12),\n"
        "             (13.13, 14.14, 15.15),\n"
        "             (16.16, 17.17, 18.18))\n"
        "i = 1\n"
        "from wrap import *\n"
        "w = wrap ('M33f')\n"
        "assert w.equalWithAbsError (m33f, 1e-6)\n"
        "w = wrap ('M33d')\n"
        "m33d2 = M33d (m33f)\n"
        "assert w.equalWithAbsError (m33d2, 1e-6)\n";

    handle<> ignored (PyRun_String (code.c_str(), Py_file_input, 
                                    mainNamespace.ptr(), mainNamespace.ptr()));

    //

    extract <object> em33f (mainNamespace["m33f"]);
    assert (em33f.check());

    PyObject *m33fObj = em33f().ptr();
    assert (m33fObj != 0);

    extract <object> em33d (mainNamespace["m33d"]);
    assert (em33d.check());

    PyObject *m33dObj = em33d().ptr();
    assert (m33dObj != 0);

    extract <object> ei (mainNamespace["i"]);
    assert (ei.check());

    PyObject *iObj = ei().ptr();
    assert (iObj != 0);

    //
    
    std::cerr << "Testing M33f:\n";

    Imath::M33f m33fAsM33f;
    assert (PyImath::M33f::convert (m33fObj, &m33fAsM33f) == 1);
    Imath::M33f m33ff (1.1f, 2.2f, 3.3f, 
                       4.4f, 5.5f, 6.6f, 
                       7.7f, 8.8f, 9.9f);
    assert (m33fAsM33f.equalWithAbsError (m33ff, 1e-7));

    Imath::M33f m33dAsM33f;
    assert (PyImath::M33f::convert (m33dObj, &m33dAsM33f) == 1);
    Imath::M33f m33df (10.10f, 11.11f, 12.12f, 
                       13.13f, 14.14f, 15.15f, 
                       16.16f, 17.17f, 18.18f);
    assert (m33dAsM33f.equalWithAbsError (m33df, 1e-7));

    Imath::M33f iAsM33f;
    assert (PyImath::M33f::convert (iObj, &iAsM33f) == 0);

    std::cerr << "ok\n";

    //

    std::cerr << "Testing M33d:\n";

    Imath::M33d m33fAsM33d;
    assert (PyImath::M33d::convert (m33fObj, &m33fAsM33d) == 1);
    Imath::M33d m33fd (1.1, 2.2, 3.3, 
                       4.4, 5.5, 6.6, 
                       7.7, 8.8, 9.9);
    assert (m33fAsM33d.equalWithAbsError (m33fd, 1e-6));

    Imath::M33d m33dAsM33d;
    assert (PyImath::M33d::convert (m33dObj, &m33dAsM33d) == 1);
    Imath::M33d m33dd (10.10, 11.11, 12.12, 
                       13.13, 14.14, 15.15, 
                       16.16, 17.17, 18.18);
    assert (m33dAsM33d.equalWithAbsError (m33dd, 1e-7));

    Imath::M33d iAsM33d;
    assert (PyImath::M33d::convert (iObj, &iAsM33d) == 0);

    std::cerr << "ok\n";
}


void
testM44 ()
{
    object mainModule ((handle<> (borrowed (PyImport_AddModule ("__main__")))));
    object mainNamespace = mainModule.attr ("__dict__");

    std::cerr << "Testing M44*:\n";

    std::string code = 
        "from imath import *\n"
        "m44f = M44f ((1.1, 2.2, 3.3, 4.4),\n"
        "             (5.5, 6.6, 7.7, 8.8),\n"
        "             (9.9, 10.10, 11.11, 12.12),\n"
        "             (13.13, 14.14, 15.15, 16.16))\n"
        "m44d = M44d ((17.17, 18.18, 19.19, 20.20),\n"
        "             (21.21, 22.22, 23.23, 24.24),\n"
        "             (25.25, 26.26, 27.27, 28.28),\n"
        "             (29.29, 30.30, 31.31, 32.32))\n"
        "i = 1\n"
        "from wrap import *\n"
        "w = wrap ('M44f')\n"
        "assert w.equalWithAbsError (m44f, 1e-6)\n"
        "w = wrap ('M44d')\n"
        "m44d2 = M44d (m44f)\n"
        "assert w.equalWithAbsError (m44d2, 1e-6)\n";

    handle<> ignored (PyRun_String (code.c_str(), Py_file_input, 
                                    mainNamespace.ptr(), mainNamespace.ptr()));

    //

    extract <object> em44f (mainNamespace["m44f"]);
    assert (em44f.check());

    PyObject *m44fObj = em44f().ptr();
    assert (m44fObj != 0);

    extract <object> em44d (mainNamespace["m44d"]);
    assert (em44d.check());

    PyObject *m44dObj = em44d().ptr();
    assert (m44dObj != 0);

    extract <object> ei (mainNamespace["i"]);
    assert (ei.check());

    PyObject *iObj = ei().ptr();
    assert (iObj != 0);

    //
    
    std::cerr << "Testing M44f:\n";

    Imath::M44f m44fAsM44f;
    assert (PyImath::M44f::convert (m44fObj, &m44fAsM44f) == 1);
    Imath::M44f m44ff (1.1f, 2.2f, 3.3f, 4.4f,
                       5.5f, 6.6f, 7.7f, 8.8f,
                       9.9f, 10.10f, 11.11f, 12.12f,
                       13.13f, 14.14f, 15.15f, 16.16f);
    assert (m44fAsM44f.equalWithAbsError (m44ff, 1e-7f));

    Imath::M44f m44dAsM44f;
    assert (PyImath::M44f::convert (m44dObj, &m44dAsM44f) == 1);
    Imath::M44f m44df (17.17f, 18.18f, 19.19f, 20.20f,
                       21.21f, 22.22f, 23.23f, 24.24f,
                       25.25f, 26.26f, 27.27f, 28.28f,
                       29.29f, 30.30f, 31.31f, 32.32f);
    assert (m44dAsM44f.equalWithAbsError (m44df, 1e-7f));

    Imath::M44f iAsM44f;
    assert (PyImath::M44f::convert (iObj, &iAsM44f) == 0);

    std::cerr << "ok\n";

    //

    std::cerr << "Testing M44d:\n";

    Imath::M44d m44fAsM44d;
    assert (PyImath::M44d::convert (m44fObj, &m44fAsM44d) == 1);
    Imath::M44d m44fd (1.1, 2.2, 3.3, 4.4,
                       5.5, 6.6, 7.7, 8.8,
                       9.9, 10.10, 11.11, 12.12,
                       13.13, 14.14, 15.15, 16.16);
    assert (m44fAsM44d.equalWithAbsError (m44fd, 1e-6));

    Imath::M44d m44dAsM44d;
    assert (PyImath::M44d::convert (m44dObj, &m44dAsM44d) == 1);
    Imath::M44d m44dd (17.17, 18.18, 19.19, 20.20,
                       21.21, 22.22, 23.23, 24.24,
                       25.25, 26.26, 27.27, 28.28,
                       29.29, 30.30, 31.31, 32.32);
    assert (m44dAsM44d.equalWithAbsError (m44dd, 1e-7));

    Imath::M44d iAsM44d;
    assert (PyImath::M44d::convert (iObj, &iAsM44d) == 0);

    std::cerr << "ok\n";
}


void
testPlane ()
{
    object mainModule ((handle<> (borrowed (PyImport_AddModule ("__main__")))));
    object mainNamespace = mainModule.attr ("__dict__");

    std::cerr << "Testing Plane*:\n";

    std::string code = 
        "from imath import *\n"
        "p3f = Plane3f (V3f (1.1, 2.2, 3.3), 4.4)\n"
        "p3d = Plane3d (V3d (5.5, 6.6, 7.7), 8.8)\n"
        "i = 1\n"
        "from wrap import *\n"
        "w = wrap ('Plane3f')\n"
        "n = V3f (1.1, 2.2, 3.3).normalize()\n"
        "assert w.normal().equalWithAbsError (n, 1e-7)\n"
        "assert equal (w.distance(), 4.4, 1e-7)\n"
        "w = wrap ('Plane3d')\n"
        "n = V3d (1.1, 2.2, 3.3).normalize()\n"
        "assert w.normal().equalWithAbsError (n, 1e-7)\n"
        "assert equal (w.distance(), 4.4, 1e-7)\n";

    handle<> ignored (PyRun_String (code.c_str(), Py_file_input, 
                                    mainNamespace.ptr(), mainNamespace.ptr()));

    //

    extract <object> ep3f (mainNamespace["p3f"]);
    assert (ep3f.check());

    PyObject *p3fObj = ep3f().ptr();
    assert (p3fObj != 0);

    extract <object> ep3d (mainNamespace["p3d"]);
    assert (ep3d.check());

    PyObject *p3dObj = ep3d().ptr();
    assert (p3dObj != 0);

    extract <object> ei (mainNamespace["i"]);
    assert (ei.check());

    PyObject *iObj = ei().ptr();
    assert (iObj != 0);

    //
    
    std::cerr << "Testing Plane3f:\n";

    Imath::Plane3f p3fAsP3f;
    assert (PyImath::Plane3f::convert (p3fObj, &p3fAsP3f) == 1);
    Imath::V3f p3fnf (1.1f, 2.2f, 3.3f);
    p3fnf.normalize();
    assert (p3fAsP3f.normal.equalWithAbsError (p3fnf, 1e-6f));
    assert (Imath::equalWithAbsError (p3fAsP3f.distance, 4.4f, 1e-7f));

    Imath::Plane3f p3dAsP3f;
    assert (PyImath::Plane3f::convert (p3dObj, &p3dAsP3f) == 1);
    Imath::V3f p3dnf (5.5f, 6.6f, 7.7f);
    p3dnf.normalize();
    assert (p3dAsP3f.normal.equalWithAbsError (p3dnf, 1e-7f));
    assert (Imath::equalWithAbsError (p3dAsP3f.distance, 8.8f, 1e-7f));

    Imath::Plane3f iAsP3f;
    assert (PyImath::Plane3f::convert (iObj, &iAsP3f) == 0);

    std::cerr << "ok\n";

    //

    std::cerr << "Testing Plane3d:\n";

    Imath::Plane3d p3fAsP3d;
    assert (PyImath::Plane3d::convert (p3fObj, &p3fAsP3d) == 1);
    Imath::V3d p3fnd (1.1, 2.2, 3.3);
    p3fnd.normalize();
    assert (p3fAsP3d.normal.equalWithAbsError (p3fnd, 1e-7));
    assert (Imath::equalWithAbsError (p3fAsP3d.distance, 4.4, 1e-7));

    Imath::Plane3d p3dAsP3d;
    assert (PyImath::Plane3d::convert (p3dObj, &p3dAsP3d) == 1);
    Imath::V3d p3dnd (5.5, 6.6, 7.7);
    p3dnd.normalize();
    assert (p3dAsP3d.normal.equalWithAbsError (p3dnd, 1e-7));
    assert (Imath::equalWithAbsError (p3dAsP3d.distance, 8.8, 1e-7));

    Imath::Plane3d iAsP3d;
    assert (PyImath::Plane3d::convert (iObj, &iAsP3d) == 0);

    std::cerr << "ok\n";
}


void
testQuat ()
{
    object mainModule ((handle<> (borrowed (PyImport_AddModule ("__main__")))));
    object mainNamespace = mainModule.attr ("__dict__");

    std::cerr << "Testing Quat*:\n";

    std::string code = 
        "from imath import *\n"
        "qf = Quatf (1.1, 2.2, 3.3, 4.4)\n"
        "qd = Quatd (5.5, V3d (6.6, 7.7, 8.8))\n"
        "t = (9.9, 10.10, 11.11, 12.12)\n"
        "i = 1\n"
        "from wrap import *\n"
        "w = wrap ('Quatf')\n"
        "assert equal (w.r(), 1.1, 1e-7)\n"
        "assert w.v().equalWithAbsError (V3f (2.2, 3.3, 4.4), 1e-7)\n"
        "w = wrap ('Quatd')\n"
        "assert equal (w.r(), 1.1, 1e-7)\n"
        "assert w.v().equalWithAbsError (V3d (2.2, 3.3, 4.4), 1e-7)\n";

    handle<> ignored (PyRun_String (code.c_str(), Py_file_input, 
                                    mainNamespace.ptr(), mainNamespace.ptr()));

    //

    extract <object> eqf (mainNamespace["qf"]);
    assert (eqf.check());

    PyObject *qfObj = eqf().ptr();
    assert (qfObj != 0);

    extract <object> eqd (mainNamespace["qd"]);
    assert (eqd.check());

    PyObject *qdObj = eqd().ptr();
    assert (qdObj != 0);

    extract <object> et (mainNamespace["t"]);
    assert (et.check());

    PyObject *tObj = et().ptr();
    assert (tObj != 0);

    extract <object> ei (mainNamespace["i"]);
    assert (ei.check());

    PyObject *iObj = ei().ptr();
    assert (iObj != 0);

    //
    
    std::cerr << "Testing Quatf:\n";

    Imath::Quatf qfAsQf;
    assert (PyImath::Quatf::convert (qfObj, &qfAsQf) == 1);
    assert (Imath::equalWithAbsError (qfAsQf.r, 1.1f, 1e-7f));
    assert (qfAsQf.v.equalWithAbsError (Imath::V3f (2.2f, 3.3f, 4.4f), 1e-7f));

    Imath::Quatf qdAsQf;
    assert (PyImath::Quatf::convert (qdObj, &qdAsQf) == 1);
    assert (Imath::equalWithAbsError (qdAsQf.r, 5.5f, 1e-7f));
    assert (qdAsQf.v.equalWithAbsError (Imath::V3f (6.6f, 7.7f, 8.8f), 1e-7f));

    Imath::Quatf tAsQf;
    assert (PyImath::Quatf::convert (tObj, &tAsQf) == 1);
    assert (Imath::equalWithAbsError (tAsQf.r, 9.9f, 1e-7f));
    assert (tAsQf.v.equalWithAbsError (Imath::V3f (10.10f, 11.11f, 12.12f), 1e-7f));

    Imath::Quatf iAsQf;
    assert (PyImath::Quatf::convert (iObj, &iAsQf) == 0);

    std::cerr << "ok\n";

    //
    
    std::cerr << "Testing Quatd:\n";

    Imath::Quatd qfAsQd;
    assert (PyImath::Quatd::convert (qfObj, &qfAsQd) == 1);
    assert (Imath::equalWithAbsError (qfAsQd.r, 1.1, 1e-7));
    assert (qfAsQd.v.equalWithAbsError (Imath::V3d (2.2, 3.3, 4.4), 1e-7));

    Imath::Quatd qdAsQd;
    assert (PyImath::Quatd::convert (qdObj, &qdAsQd) == 1);
    assert (Imath::equalWithAbsError (qdAsQd.r, 5.5, 1e-7));
    assert (qdAsQd.v.equalWithAbsError (Imath::V3d (6.6, 7.7, 8.8), 1e-7));

    Imath::Quatd tAsQd;
    assert (PyImath::Quatd::convert (tObj, &tAsQd) == 1);
    assert (Imath::equalWithAbsError (tAsQd.r, 9.9, 1e-7));
    assert (tAsQd.v.equalWithAbsError (Imath::V3d (10.10, 11.11, 12.12), 1e-7));

    Imath::Quatd iAsQd;
    assert (PyImath::Quatd::convert (iObj, &iAsQd) == 0);

    std::cerr << "ok\n";
}


void
testRandom ()
{
    object mainModule ((handle<> (borrowed (PyImport_AddModule ("__main__")))));
    object mainNamespace = mainModule.attr ("__dict__");

    std::cerr << "Testing Random*:\n";

    std::string code = 
        "from imath import *\n"
        "from wrap import *\n"
        "w = wrap ('Rand32')\n"
        "r1 = w.nextf()\n"
        "r2 = w.nextf()\n"
        "assert r1 != r2\n"
        "w = wrap ('Rand48')\n"
        "r1 = w.nextf()\n"
        "r2 = w.nextf()\n"
        "assert r1 != r2\n";

    handle<> ignored (PyRun_String (code.c_str(), Py_file_input, 
                                    mainNamespace.ptr(), mainNamespace.ptr()));

    //
    
    std::cerr << "Testing Rand32:\n";

    std::cerr << "ok\n";

    //
    
    std::cerr << "Testing Rand48:\n";

    std::cerr << "ok\n";
}


void
testShear ()
{
    object mainModule ((handle<> (borrowed (PyImport_AddModule ("__main__")))));
    object mainNamespace = mainModule.attr ("__dict__");

    std::cerr << "Testing Shear6*:\n";

    std::string code = 
        "from imath import *\n"
        "s6f = Shear6f (1.1, 2.2, 3.3, 4.4, 5.5, 6.6)\n"
        "s6d = Shear6d (7.7, 8.8, 9.9, 10.10, 11.11, 12.12)\n"
        "t3 = (13.13, 14.14, 15.15)\n"
        "t6 = (16.16, 17.17, 18.18, 19.19, 20.20, 21.21)\n"
        "v3i = V3i (22, 23, 24)\n"
        "v3f = V3f (25.25, 26.26, 27.27)\n"
        "v3d = V3d (28.28, 29.29, 30.30)\n"
        "i = 1\n"
        "from wrap import *\n"
        "w = wrap ('Shear6f')\n"
        "assert w.equalWithAbsError (s6f, 1e-6)\n"
        "w = wrap ('Shear6d')\n"
        "s6d2 = Shear6d (s6f)\n"
        "assert w.equalWithAbsError (s6d2, 1e-6)\n";

    handle<> ignored (PyRun_String (code.c_str(), Py_file_input, 
                                    mainNamespace.ptr(), mainNamespace.ptr()));

    //

    extract <object> es6f (mainNamespace["s6f"]);
    assert (es6f.check());
    PyObject *s6fObj = es6f().ptr();
    assert (s6fObj != 0);

    extract <object> es6d (mainNamespace["s6d"]);
    assert (es6d.check());

    PyObject *s6dObj = es6d().ptr();
    assert (s6dObj != 0);

    extract <object> et3 (mainNamespace["t3"]);
    assert (et3.check());

    PyObject *t3Obj = et3().ptr();
    assert (t3Obj != 0);

    extract <object> et6 (mainNamespace["t6"]);
    assert (et6.check());

    PyObject *t6Obj = et6().ptr();
    assert (t6Obj != 0);

    extract <object> ev3i (mainNamespace ["v3i"]);
    assert (ev3i.check());

    PyObject *v3iObj = ev3i().ptr();
    assert (v3iObj != 0);

    extract <object> ev3f (mainNamespace ["v3f"]);
    assert (ev3f.check());
    
    PyObject *v3fObj = ev3f().ptr();
    assert (v3fObj != 0);

    extract <object> ev3d (mainNamespace ["v3d"]);
    assert (ev3d.check());
    
    PyObject *v3dObj = ev3d().ptr();
    assert (v3dObj != 0);

    extract <object> ei (mainNamespace["i"]);
    assert (ei.check());

    PyObject *iObj = ei().ptr();
    assert (iObj != 0);

    //
    
    std::cerr << "Testing Shear6f:\n";

    Imath::Shear6f s6fAsS6f;
    assert (PyImath::Shear6f::convert (s6fObj, &s6fAsS6f) == 1);
    Imath::Shear6f s6ff (1.1f, 2.2f, 3.3f, 4.4f, 5.5f, 6.6f);
    assert (s6fAsS6f.equalWithAbsError (s6ff, 1e-7f));

    Imath::Shear6f s6dAsS6f;
    assert (PyImath::Shear6f::convert (s6dObj, &s6dAsS6f) == 1);
    Imath::Shear6f s6df (7.7f, 8.8f, 9.9f, 10.10f, 11.11f, 12.12f);
    assert (s6dAsS6f.equalWithAbsError (s6df, 1e-7f));

    Imath::Shear6f t3AsS6f;
    assert (PyImath::Shear6f::convert (t3Obj, &t3AsS6f) == 1);
    Imath::Shear6f t3f (13.13f, 14.14f, 15.15f, 0, 0, 0);
    assert (t3AsS6f.equalWithAbsError (t3f, 1e-7f));

    Imath::Shear6f t6AsS6f;
    assert (PyImath::Shear6f::convert (t6Obj, &t6AsS6f) == 1);
    Imath::Shear6f t6f (16.16f, 17.17f, 18.18f, 19.19f, 20.20f, 21.21f);
    assert (t6AsS6f.equalWithAbsError (t6f, 1e-7f));

    Imath::Shear6f v3iAsS6f;
    assert (PyImath::Shear6f::convert (v3iObj, &v3iAsS6f) == 1);
    Imath::Shear6f v3if (22.0f, 23.0f, 24.0f, 0, 0, 0);
    assert (v3iAsS6f.equalWithAbsError (v3if, 1e-7f));

    Imath::Shear6f v3fAsS6f;
    assert (PyImath::Shear6f::convert (v3fObj, &v3fAsS6f) == 1);
    Imath::Shear6f v3ff (25.25f, 26.26f, 27.27f, 0, 0, 0);
    assert (v3fAsS6f.equalWithAbsError (v3ff, 1e-7f));

    Imath::Shear6f v3dAsS6f;
    assert (PyImath::Shear6f::convert (v3dObj, &v3dAsS6f) == 1);
    Imath::Shear6f v3df (28.28f, 29.29f, 30.30f, 0, 0, 0);
    assert (v3dAsS6f.equalWithAbsError (v3df, 1e-7f));

    Imath::Shear6f iAsS6f;
    assert (PyImath::Shear6f::convert (iObj, &iAsS6f) == 0);

    std::cerr << "ok\n";

    //

    std::cerr << "Testing Shear6d:\n";

    Imath::Shear6d s6fAsS6d;
    assert (PyImath::Shear6d::convert (s6fObj, &s6fAsS6d) == 1);
    Imath::Shear6d s6fd (1.1, 2.2, 3.3, 4.4, 5.5, 6.6);
    assert (s6fAsS6d.equalWithAbsError (s6fd, 1e-7));

    Imath::Shear6d s6dAsS6d;
    assert (PyImath::Shear6d::convert (s6dObj, &s6dAsS6d) == 1);
    Imath::Shear6d s6dd (7.7, 8.8, 9.9, 10.10, 11.11, 12.12);
    assert (s6dAsS6d.equalWithAbsError (s6dd, 1e-7));

    Imath::Shear6d t3AsS6d;
    assert (PyImath::Shear6d::convert (t3Obj, &t3AsS6d) == 1);
    Imath::Shear6d t3d (13.13, 14.14, 15.15, 0, 0, 0);
    assert (t3AsS6d.equalWithAbsError (t3d, 1e-7));

    Imath::Shear6d t6AsS6d;
    assert (PyImath::Shear6d::convert (t6Obj, &t6AsS6d) == 1);
    Imath::Shear6d t6d (16.16, 17.17, 18.18, 19.19, 20.20, 21.21);
    assert (t6AsS6d.equalWithAbsError (t6d, 1e-7));

    Imath::Shear6d v3iAsS6d;
    assert (PyImath::Shear6d::convert (v3iObj, &v3iAsS6d) == 1);
    Imath::Shear6d v3id (22.0, 23.0, 24.0, 0, 0, 0);
    assert (v3iAsS6d.equalWithAbsError (v3id, 1e-7));

    Imath::Shear6d v3fAsS6d;
    assert (PyImath::Shear6d::convert (v3fObj, &v3fAsS6d) == 1);
    Imath::Shear6d v3fd (25.25, 26.26, 27.27, 0, 0, 0);
    assert (v3fAsS6d.equalWithAbsError (v3fd, 1e-6));

    Imath::Shear6d v3dAsS6d;
    assert (PyImath::Shear6d::convert (v3dObj, &v3dAsS6d) == 1);
    Imath::Shear6d v3dd (28.28, 29.29, 30.30, 0, 0, 0);
    assert (v3dAsS6d.equalWithAbsError (v3dd, 1e-7));

    Imath::Shear6d iAsS6d;
    assert (PyImath::Shear6d::convert (iObj, &iAsS6d) == 0);

    std::cerr << "ok\n";
}


void
testV2 ()
{
    object mainModule ((handle<> (borrowed (PyImport_AddModule ("__main__")))));
    object mainNamespace = mainModule.attr ("__dict__");

    std::cerr << "Testing V2*:\n";

    std::string code = 
        "from imath import *\n"
        "v2i = V2i (1, 2)\n"
        "v2f = V2f (3.3, 4.4)\n"
        "v2d = V2d (5.5, 6.6)\n"
        "t = (7, 8)\n"
        "i = 1\n"
        "from wrap import *\n"
        "w = wrap ('V2i')\n"
        "assert w == V2i (1, 2)\n"
        "w = wrap ('V2f')\n"
        "assert w.equalWithAbsError (V2f (1.1, 2.2), 1e-7)\n"
        "w = wrap ('V2d')\n"
        "assert w.equalWithAbsError (V2d (1.1, 2.2), 1e-7)\n";

    handle<> ignored (PyRun_String (code.c_str(), Py_file_input, 
                                    mainNamespace.ptr(), mainNamespace.ptr()));

    //

    extract <object> ev2i (mainNamespace ["v2i"]);
    assert (ev2i.check());
    
    PyObject *v2iObj = ev2i().ptr();
    assert (v2iObj != 0);

    extract <object> ev2f (mainNamespace ["v2f"]);
    assert (ev2f.check());
    
    PyObject *v2fObj = ev2f().ptr();
    assert (v2fObj != 0);

    extract <object> ev2d (mainNamespace ["v2d"]);
    assert (ev2d.check());
    
    PyObject *v2dObj = ev2d().ptr();
    assert (v2dObj != 0);

    extract <object> et (mainNamespace["t"]);
    assert (et.check());

    PyObject *tObj = et().ptr();
    assert (tObj != 0);

    extract <object> ei (mainNamespace["i"]);
    assert (ei.check());

    PyObject *iObj = ei().ptr();
    assert (iObj != 0);

    //

    std::cerr << "Testing V2i:\n";

    Imath::V2i v2iAsV2i;
    assert (PyImath::V2i::convert (v2iObj, &v2iAsV2i) == 1);
    assert (v2iAsV2i == Imath::V2i (1, 2));

    Imath::V2i v2fAsV2i;
    assert (PyImath::V2i::convert (v2fObj, &v2fAsV2i) == 1);
    assert (v2fAsV2i == Imath::V2i (3, 4));

    Imath::V2i v2dAsV2i;
    assert (PyImath::V2i::convert (v2dObj, &v2dAsV2i) == 1);
    assert (v2dAsV2i == Imath::V2i (5, 6));

    Imath::V2i tAsV2i;
    assert (PyImath::V2i::convert (tObj, &tAsV2i) == 1);
    assert (tAsV2i == Imath::V2i (7, 8));

    Imath::V2i iAsV2i;
    assert (PyImath::V2i::convert (iObj, &iAsV2i) == 0);

    std::cerr << "ok\n";

    //

    std::cerr << "Testing V2f:\n";

    Imath::V2f v2iAsV2f;
    assert (PyImath::V2f::convert (v2iObj, &v2iAsV2f) == 1);
    assert (v2iAsV2f.equalWithAbsError (Imath::V2f (1.0f, 2.0f), 1e-7));

    Imath::V2f v2fAsV2f;
    assert (PyImath::V2f::convert (v2fObj, &v2fAsV2f) == 1);
    assert (v2fAsV2f.equalWithAbsError (Imath::V2f (3.3f, 4.4f), 1e-7));

    Imath::V2f v2dAsV2f;
    assert (PyImath::V2f::convert (v2dObj, &v2dAsV2f) == 1);
    assert (v2dAsV2f.equalWithAbsError (Imath::V2f (5.5f, 6.6f), 1e-7));

    Imath::V2f tAsV2f;
    assert (PyImath::V2f::convert (tObj, &tAsV2f) == 1);
    assert (tAsV2f.equalWithAbsError (Imath::V2f (7.0f, 8.0f), 1e-7));

    Imath::V2f iAsV2f;
    assert (PyImath::V2f::convert (iObj, &iAsV2f) == 0);

    std::cerr << "ok\n";

    //

    std::cerr << "Testing V2d:\n";

    Imath::V2d v2iAsV2d;
    assert (PyImath::V2d::convert (v2iObj, &v2iAsV2d) == 1);
    assert (v2iAsV2d.equalWithAbsError (Imath::V2d (1.0, 2.0), 1e-7));

    Imath::V2d v2fAsV2d;
    assert (PyImath::V2d::convert (v2fObj, &v2fAsV2d) == 1);
    assert (v2fAsV2d.equalWithAbsError (Imath::V2f (3.3, 4.4), 1e-7));

    Imath::V2d v2dAsV2d;
    assert (PyImath::V2d::convert (v2dObj, &v2dAsV2d) == 1);
    assert (v2dAsV2d.equalWithAbsError (Imath::V2f (5.5, 6.6), 1e-7));

    Imath::V2d tAsV2d;
    assert (PyImath::V2d::convert (tObj, &tAsV2d) == 1);
    assert (tAsV2d.equalWithAbsError (Imath::V2d (7.0, 8.0), 1e-7));

    Imath::V2d iAsV2d;
    assert (PyImath::V2d::convert (iObj, &iAsV2d) == 0);

    std::cerr << "ok\n";
}


void
testV3 ()
{
    object mainModule ((handle<> (borrowed (PyImport_AddModule ("__main__")))));
    object mainNamespace = mainModule.attr ("__dict__");

    std::cerr << "Testing V3*:\n";

    std::string code = 
        "from imath import *\n"
        "v3i = V3i (1, 2, 3)\n"
        "v3f = V3f (4.4, 5.5, 6.6)\n"
        "v3d = V3d (7.7, 8.8, 9.9)\n"
        "t = (10, 11, 12)\n"
        "i = 1\n"
        "from wrap import *\n"
        "w = wrap ('V3i')\n"
        "assert w == V3i (1, 2, 3)\n"
        "w = wrap ('V3f')\n"
        "assert w.equalWithAbsError (V3f (1.1, 2.2, 3.3), 1e-7)\n"
        "w = wrap ('V3d')\n"
        "assert w.equalWithAbsError (V3d (1.1, 2.2, 3.3), 1e-7)\n";

    handle<> ignored (PyRun_String (code.c_str(), Py_file_input, 
                                    mainNamespace.ptr(), mainNamespace.ptr()));

    //

    extract <object> ev3i (mainNamespace ["v3i"]);
    assert (ev3i.check());

    PyObject *v3iObj = ev3i().ptr();
    assert (v3iObj != 0);

    extract <object> ev3f (mainNamespace ["v3f"]);
    assert (ev3f.check());
    
    PyObject *v3fObj = ev3f().ptr();
    assert (v3fObj != 0);

    extract <object> ev3d (mainNamespace ["v3d"]);
    assert (ev3d.check());
    
    PyObject *v3dObj = ev3d().ptr();
    assert (v3dObj != 0);

    extract <object> et (mainNamespace["t"]);
    assert (et.check());

    PyObject *tObj = et().ptr();
    assert (tObj != 0);

    extract <object> ei (mainNamespace["i"]);
    assert (ei.check());

    PyObject *iObj = ei().ptr();
    assert (iObj != 0);

    //
    
    std::cerr << "Testing V3i:\n";

    Imath::V3i v3iAsV3i;
    assert (PyImath::V3i::convert (v3iObj, &v3iAsV3i) == 1);
    assert (v3iAsV3i == Imath::V3i (1, 2, 3));

    Imath::V3i v3fAsV3i;
    assert (PyImath::V3i::convert (v3fObj, &v3fAsV3i) == 1);
    assert (v3fAsV3i == Imath::V3i (4, 5, 6));

    Imath::V3d v3iAsV3d;
    assert (PyImath::V3d::convert (v3iObj, &v3iAsV3d) == 1);
    assert (v3iAsV3d.equalWithAbsError (Imath::V3d (1.0, 2.0, 3.0), 1e-7));

    Imath::V3i tAsV3i;
    assert (PyImath::V3i::convert (tObj, &tAsV3i) == 1);
    assert (tAsV3i == Imath::V3i (10, 11, 12));

    Imath::V3i iAsV3i;
    assert (PyImath::V3i::convert (iObj, &iAsV3i) == 0);

    std::cerr << "ok\n";

    //

    std::cerr << "Testing V3f:\n";

    Imath::V3f v3iAsV3f;
    assert (PyImath::V3f::convert (v3iObj, &v3iAsV3f) == 1);
    assert (v3iAsV3f.equalWithAbsError (Imath::V3f (1.0f, 2.0f, 3.0f), 1e-7));

    Imath::V3f v3fAsV3f;
    assert (PyImath::V3f::convert (v3fObj, &v3fAsV3f) == 1);
    assert (v3fAsV3f.equalWithAbsError (Imath::V3f (4.4f, 5.5f, 6.6f), 1e-7));

    Imath::V3f v3dAsV3f;
    assert (PyImath::V3f::convert (v3dObj, &v3dAsV3f) == 1);
    assert (v3dAsV3f.equalWithAbsError (Imath::V3f (7.7f, 8.8f, 9.9f), 1e-7));

    Imath::V3f tAsV3f;
    assert (PyImath::V3f::convert (tObj, &tAsV3f) == 1);
    assert (tAsV3f.equalWithAbsError (Imath::V3f (10.0f, 11.0f, 12.0f), 1e-7));

    Imath::V3f iAsV3f;
    assert (PyImath::V3f::convert (iObj, &iAsV3f) == 0);

    std::cerr << "ok\n";

    //

    std::cerr << "Testing V3d:\n";

    Imath::V3i v3dAsV3i;
    assert (PyImath::V3i::convert (v3dObj, &v3dAsV3i) == 1);
    assert (v3dAsV3i == Imath::V3i (7, 8, 9));

    Imath::V3d v3fAsV3d;
    assert (PyImath::V3d::convert (v3fObj, &v3fAsV3d) == 1);
    assert (v3fAsV3d.equalWithAbsError (Imath::V3f (4.4, 5.5, 6.6), 1e-7));

    Imath::V3d v3dAsV3d;
    assert (PyImath::V3d::convert (v3dObj, &v3dAsV3d) == 1);
    assert (v3dAsV3d.equalWithAbsError (Imath::V3f (7.7, 8.8, 9.9), 1e-6));

    Imath::V3d tAsV3d;
    assert (PyImath::V3d::convert (tObj, &tAsV3d) == 1);
    assert (tAsV3d.equalWithAbsError (Imath::V3d (10.0, 11.0, 12.0), 1e-7));

    Imath::V3d iAsV3d;
    assert (PyImath::V3d::convert (iObj, &iAsV3d) == 0);

    std::cerr << "ok\n";
}

int
main (int argc, char *argv[])
{
    Py_Initialize();
    if (Py_IsInitialized())
    {
        initWrapTester();

        TEST (testBox2);
        TEST (testBox3);
        TEST (testC3);
        TEST (testC4);
        TEST (testEuler);
        TEST (testFrustum);
        TEST (testLine);
        TEST (testM33);
        TEST (testM44);
        TEST (testPlane);
        TEST (testQuat);
        TEST (testRandom);
        TEST (testShear);
        TEST (testV2);
        TEST (testV3);
    
        Py_Finalize();
    }

    TEST (testStringTable);

    return 0;
}
