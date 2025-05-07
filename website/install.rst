..
  SPDX-License-Identifier: BSD-3-Clause
  Copyright Contributors to the OpenEXR Project.

.. _Install:

Install
=======

.. toctree::
   :caption: Install

Linux
-----

The Imath library is available for download and installation in
binary form via package managers on many Linux distributions.

Refer to the current version of Imath on various major Linux distros at
`repology.org <https://repology.org/project/imath/versions>`_:

.. image:: https://repology.org/badge/vertical-allrepos/imath.svg?exclude_unsupported=1&columns=4&header=Imath%20Packaging%20Status&minversion=3.0
   :target: https://repology.org/project/imath/versions

Older versions of Imath were distributed as a component of OpenEXR
called ``ilmbase``. We do not recommend using these outdated
versions.

To install via ``yum`` on RHEL/CentOS:

.. code-block::

    % sudo yum makecache
    % sudo yum install imath

To install via ``apt-get`` on Ubuntu:

.. code-block::

    % sudo apt-get update
    % sudo apt-get install imath

macOS
-----

On macOS, install via `Homebrew <https://formulae.brew.sh/formula/imath>`_:

.. code-block::

   % brew install imath

Alternatively, you can install on macOS via `MacPorts
<https://ports.macports.org/port/imath/>`_:

.. code-block::

   % port install imath

Windows
-------

Install via `vcpkg <https://vcpkg.io/en/packages>`_:

.. code-block::

   % .\vcpkg install imath

Python
------

Please note that ``pip install imath`` installs the `imath
<https://pypi.org/project/imath/>`_ module, which is not
affiliated with the OpenEXR project or the ASWF. Please direct
questions there.

Build from Source
-----------------

Imath builds on Linux, macOS, Microsoft Windows via CMake, and is
cross-compilable on other systems.

Download the source from the `GitHub releases page
<https://github.com/AcademySoftwareFoundation/Imath/releases>`_
page, or clone the `repo <https://github.com/AcademySoftwareFoundation/Imath>`_.

The ``release`` branch of the repo always points to the most advanced
release.


Prerequisites
~~~~~~~~~~~~~

Make sure these are installed on your system before building Imath:

* Imath requires CMake version 3.14 or newer
* C++ compiler that supports C++14

The instructions that follow describe building Imath with CMake.

Linux/macOS
~~~~~~~~~~~

To build via CMake, you need to first identify three directories:

1. The source directory, i.e. the top-level directory of the
   downloaded source archive or cloned repo, referred to below as ``$srcdir``
2. A temporary directory to hold the build artifacts, referred to below as
   ``$builddir``
3. A destination directory into which to install the
   libraries and headers, referred to below as ``$installdir``.

To build:

.. code-block::

    % cd $builddir
    % cmake $srcdir --install-prefix $installdir
    % cmake --build $builddir --target install --config Release

Note that the CMake configuration prefers to apply an out-of-tree
build process, since there may be multiple build configurations
(i.e. debug and release), one per folder, all pointing at once source
tree, hence the ``$builddir`` noted above, referred to in CMake
parlance as the *build directory*. You can place this directory
wherever you like.

See the CMake Configuration Options section below for the most common
configuration options especially the install directory. Note that with
no arguments, as above, ``make install`` installs the header files in
``/usr/local/include``, the object libraries in ``/usr/local/lib``, and the
executable programs in ``/usr/local/bin``.

Windows
~~~~~~~

Under Windows, if you are using a command line-based setup, such as
cygwin, you can of course follow the above. For Visual Studio, cmake
generators are "multiple configuration", so you don't even have to set
the build type, although you will most likely need to specify the
install location.  Install Directory By default, ``make install``
installs the headers, libraries, and programs into ``/usr/local``, but you
can specify a local install directory to cmake via the
``CMAKE_INSTALL_PREFIX`` variable:

.. code-block::

    % cmake .. -DCMAKE_INSTALL_PREFIX=$Imath_install_directory

Example Application
-------------------

A basic program that uses the Imath library:

.. literalinclude:: examples/intro.cpp

And a ``CMakeLists.txt`` configuration file:

.. literalinclude:: examples/quickstart/CMakeLists.txt

Python Bindings
~~~~~~~~~~~~~~~

To build a C++ program that uses the the ``PyImath`` python bindings,
add ``PyImath`` as a ``COMPONENT`` to ``find_package(Imath)``:

.. literalinclude:: examples/quickstart.pyimath/CMakeLists.txt


Library Names and Namespaces
----------------------------

When building shared a library, the library is identified with a
series of symbolic links with embedded release numbers in the
filenames:

.. code-block::

    libImath.so -> libImath-3_2.so.31
    libImath-3_2.so.31 -> libImath-3_2.so.31.3.2.0
    libImath-3_2.so.31.3.2.0 (the actual shared object file)

The ``31`` identifies the soversion of the ABI, and the ``3.2.0``
identifies the release major, minor, and patch numbers. Furthermore,
by default, the library name is suffixed with the major and minor
release numbers, which serves as an aid in building an application
that requires simultaneous support for multiple versions of the
library, i.e linking against both release v3.2 and v3.1 in teh same
application, in the rare event this is ever necessary. Note that this
feature is accompanied by the C++ namespace, which by default embeds
all symbols in a namespace that encodes the major and minor release,
i.e. ``Imath_3_2``, ``Imath_3_1``, etc.

This namespacing and library suffixing can be controlled throught the
build-time cmake option ``IMATH_NAMESPACE`` and ``IMATH_LIB_SUFFIX``,
respectively. If ``IMATH_LIB_SUFFIX`` is provided when configuring
cmake, that string will appear after ``libImath`` in the library
filename, in place of the default ``-3_2``.  Note that if
``IMATH_LIB_SUFFIX=""``, the suffix will be empty.

In addition to the versioned and suffixed shared object names, the
build also installs a symbolic link with the unversioned and
unsuffixed name ``libImath.so``. This can be disabled with the
``IMATH_INSTALL_SYM_LINK`` cmake option.

CMake Build-time Configuration Options
--------------------------------------

* ``BUILD_SHARED_LIBS``

  This is the primary control whether to build static libraries or
  shared libraries / dlls (side note: technically a convention, hence
  not an official ``CMAKE_`` variable, it is defined within cmake and
  used everywhere to control this static / shared behavior)

* ``BUILD_TESTING``

  Build the test suite. Default is ``ON``.  To run the test suite, use
  ``make test`` or use ``ctest``.

* ``IMATH_INSTALL_PKG_CONFIG``

  If true, install the ``Imath.pc`` package config file. On by default.

* ``IMATH_INSTALL_SYM_LINK``

  If true, create a symbolic link to the shared object with a name
  with no suffix, i.e. ``libImath.so``. On by default.

* ``IMATH_BUILD_APPLE_FRAMEWORKS``

  If true, the build configures the library into an Apple framework,
  i.e. ``Imath.framework``, rather than the traditional ``lib`` and
  ``includes`` subdirectories.  Use in conjunction with
  ``CMAKE_SYSTEM_NAME=iOS``.

Building the Python Bindings
----------------------------

The bindings as historically maintained and distributed are
implemented via Boost.Python. To build the bindings, set the cmake
configuration ``PYTHON`` to ``ON``.

The bindings consist of two parts: the ``imath`` module, and the
``libPyImath.so`` shared object, which provides a C++ API for the
wrappings for use in other C++ applications.

The ``PYTHON_INSTALL_DIR`` cmake variable designates the installation
directory for the ``imath`` module.

The ``IMATH_TEST_PYTHON`` cmake variable controls whether the python
test suite will be invoked during test execution. It is sometimes
convenient to be able to build the bindings but exclude them from the
tests.

Similar to ``IMATH_LIB_SUFFIX``, the ``PYIMATH_LIB_SUFFIX`` provides a
suffix for the ``libPyImath.so`` shared library. By default, the
suffix includes the imath release, similar to ``libImath.so`` python
version as well as the python version, e.g. ``libPyImath_Python3_9-3_2.so``.

The ``IMATH_INSTALL_PKG_CONFIG`` variable also controls whether to
install a ``PyImath.pc`` package config file.

Building the PyBind11 Python Bindings
-------------------------------------

A new and not-yet-complete implementation of the python bindings
exists alongside the Boost.Python bindings that have been historically
distributed. The intention is that these will soon entirely replace
the Boost.Python implementation, but they are not yet complete. This
is provided for development purposes only.

To build the pybind11 bindings, set the cmake configuration
``PYBIND11`` to ``ON``.

The bindings consist of two parts: the ``pybindimath`` module, and the
``libPyBindImath.so`` shared object, which provides a C++ API for the
wrappings for use in other C++ applications.

The ``PYTHON_INSTALL_DIR`` cmake variable designates the installation
directory for the ``pybindimath`` module.

The ``IMATH_TEST_PYBIND11`` cmake variable controls whether the python
test suite will be invoked during test execution. It is sometimes
convenient to be able to build the bindings but exclude them from the
tests.

Similar to ``IMATH_LIB_SUFFIX``, the ``PYBINDIMATH_LIB_SUFFIX``
provides a suffix for the ``libPyBindImath.so`` shared library. By
default, the suffix includes the imath release, similar to
``libImath.so`` python version as well as the python version,
e.g. ``libPyBindImath_Python3_9-3_2.so``.

The ``IMATH_INSTALL_PKG_CONFIG`` variable also controls whether to
install a ``PyBindImath.pc`` package config file.

Porting Applications from OpenEXR v2 to v3
------------------------------------------

See the :doc:`PortingGuide` for details about differences from previous
releases and how to address them. Also refer to the porting guide for
details about changes to Imath.

Building the Website
--------------------

The Imath technical documentation at `https://imath.readthedocs.io
<https://imath.readthedocs.io>`_ is generated via `Sphinx
<https://www.sphinx-doc.org>`_ with the `Breathe
<https://breathe.readthedocs.io>`_ extension using information
extracted from header comments by `Doxygen <https://www.doxygen.nl>`_,
using the `sphinx-press-theme
<https://pypi.org/project/sphinx-press-theme>`_, and is hosted by
`Read the Docs <https://readthedocs.org/projects/imath/>`_.
The website source is in `restructured text
<https://www.sphinx-doc.org/en/master/usage/restructuredtext/basics.html>`_
in the ``website`` directory.

To build the website locally from the source headers and
``.rst`` files, set the CMake option ``BUILD_WEBSITE=ON``. This adds
``website`` CMake target. Generation is off by default.

Building the website requires that ``sphinx``, ``breathe``, and
``doxygen`` are installed. It further requires the `sphinx-press-theme
<https://pypi.org/project/sphinx-press-theme>`_. Complete dependencies are
described in the `requirements.txt
<https://github.com/AcademySoftwareFoundation/imath/blob/main/website/requirements.txt>`_
file.

On Debian/Ubuntu Linux:

.. code-block::

    % apt-get install doxygen python3-sphinx
    % pip3 install breathe
    % pip3 install sphinx_press_theme

    % mkdir _build
    % cd _build
    % cmake .. -DBUILD_WEBSITE=ON
    % cmake --build . --target website

