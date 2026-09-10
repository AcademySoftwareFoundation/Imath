# Copyright Contributors to the MaterialX Project
# SPDX-License-Identifier: Apache-2.0
# copied from: https://github.com/AcademySoftwareFoundation/MaterialX/blob/main/python/mtx_skbuild_plugin.py
# Modifications Copyright (c) Contributors to the OpenEXR Project.

"""
This is a custom scikit-build-core plugin that will
fetch the Imath version from the CMake project.
"""
import os
import tempfile
import subprocess
from pathlib import Path
from typing import FrozenSet, Dict, Optional, Union

from scikit_build_core.file_api.query import stateless_query
from scikit_build_core.file_api.reply import load_reply_dir


def dynamic_metadata(
    fields: FrozenSet[str],
    settings: Optional[Dict[str, object]] = None,
) -> Dict[str, Union[str, Dict[str, Optional[str]]]]:
    print("imath_skbuild_plugin: Computing Imath version from CMake...")

    if fields != "version":
        msg = f"Only the 'version' field is supported: fields={fields}"
        raise ValueError(msg)

    if settings:
        msg = "No inline configuration is supported"
        raise ValueError(msg)

    if "IMATH_RELEASE_CANDIDATE_TAG" in os.environ:

        # e.g. "v3.2.0-rc4"
        #
        # If IMATH_RELEASE_CANDIDATE_TAG is set,
        # the build is for a publish to test.pypi.org. Multiple test
        # publishes may happen in the course of preparing for a
        # release, but published packages require unique
        # names/versions, so use the release candidate tag as the
        # version (minus the leading 'v'),

        rct = os.environ["IMATH_RELEASE_CANDIDATE_TAG"]
        version = rct[1:]

    else:

        # This file lives in <repo>/src/python/
        source_dir = Path(__file__).resolve().parents[2]

        with tempfile.TemporaryDirectory() as tmpdir:
            # We will use CMake's file API to get the version
            # instead of parsing the CMakeLists files.

            # First generate the query folder so that CMake can generate replies.
            reply_dir = stateless_query(Path(tmpdir))

            # Run cmake (configure). CMake will generate a reply automatically.
            # Only the version is needed, so skip the bindings and tests.
            try:
                subprocess.run(
                    [
                        "cmake",
                        "-S",
                        str(source_dir),
                        "-B",
                        tmpdir,
                        "-DPYTHON=OFF",
                        "-DBUILD_TESTING=OFF",
                    ],
                    stdout=subprocess.PIPE,
                    stderr=subprocess.STDOUT,
                    check=True,
                    text=True,
                )
            except subprocess.CalledProcessError as exc:
                print(exc.stdout)
                raise RuntimeError(
                    "Failed to configure project to get the version"
                ) from exc

            # Get the generated replies.
            index = load_reply_dir(reply_dir)

            # Get the version from the CMAKE_PROJECT_VERSION variable.
            entries = [
                entry
                for entry in index.reply.cache_v2.entries
                if entry.name == "CMAKE_PROJECT_VERSION"
            ]

            if not entries:
                raise ValueError("Could not find Imath version from CMake project")

            if len(entries) > 1:
                raise ValueError("More than one entry for CMAKE_PROJECT_VERSION found...")

            version = entries[0].value

    print("imath_skbuild_plugin: Computed version: {0}".format(version))

    return version
