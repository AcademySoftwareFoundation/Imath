//
// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the OpenEXR Project.
//

#include <iomanip>
#include <iostream>

using namespace std;

//-----------------------------------------------------
// Compute a lookup table for float-to-half conversion.
//
// When indexed with the combined sign and exponent of
// a float, the table either returns the combined sign
// and exponent of the corresponding half, or zero if
// the corresponding half may not be normalized (zero,
// denormalized, overflow).
//-----------------------------------------------------

void
initELut (unsigned short eLut[])
{
    for (int i = 0; i < 0x100; i++)
    {
        int e = (i & 0x0ff) - (127 - 15);

        if (e <= 0 || e >= 30)
        {
            //
            // Special case
            //

            eLut[i]         = 0;
            eLut[i | 0x100] = 0;
        }
        else
        {
            //
            // Common case - normalized half, no exponent overflow possible
            //

            eLut[i]         = (e << 10);
            eLut[i | 0x100] = ((e << 10) | 0x8000);
        }
    }
}

//------------------------------------------------------------
// Main - prints the sign-and-exponent conversion lookup table
//------------------------------------------------------------

int
main()
{
    const int tableSize = 1 << 9;
    unsigned short eLut[tableSize];
    initELut (eLut);

    cout << "//\n"
            "// This is an automatically generated file.\n"
            "// Do not edit.\n"
            "//\n\n";

    cout << "{\n    ";

    for (int i = 0; i < tableSize; i++)
    {
        cout << setw (5) << eLut[i] << ", ";

        if (i % 8 == 7)
        {
            cout << "\n";

            if (i < tableSize - 1)
                cout << "    ";
        }
    }

    cout << "};\n";
    return 0;
}
