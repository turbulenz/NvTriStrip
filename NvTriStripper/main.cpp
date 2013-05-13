#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>

#include "NvTriStrip.h"

using namespace std;

int main (int argc, char * const argv[])
{
    bool stitchstrips = false;
    int argidx = 1;

    while (argidx < argc)
    {
        //cerr << "argument: " << argidx << " = " << argv[argidx] << endl;
        if (strcmp(argv[argidx], "-cs") == 0)
        {
            argidx++;

            int cachesize = 16;
            int num = sscanf(argv[argidx], "%d", &cachesize);
            if (num != 1)
            {
                cerr << "Error reading cache size for -cs argument" << endl;
                exit(-1);
            }

            SetCacheSize(cachesize);
            argidx++;
        }
        else if (strcmp(argv[argidx], "-s") == 0)
        {
            //cerr << "Stitching on" << endl;
            argidx++;
            stitchstrips = true;
        }
    }

    SetStitchStrips(stitchstrips);

    if (!stitchstrips)
    {
        SetListsOnly(true);
    }

    int idx = 0;
    std::vector<unsigned short> indices;
    cin.exceptions(ifstream::eofbit | ifstream::failbit | ifstream::badbit);

    //read vertex indicies from stdin until we
    //get a -1 to signify the end of the data stream from stdin
    while (true)
    {
        try
        {
            cin >> skipws >> idx;
        }
        catch (ifstream::failure e)
        {
            cout << "Exception reading file: ";
            exit(-1);
        }

        //cerr << "read: " << idx << endl;

        if (idx  == -1)
            break;

        indices.push_back(idx);
    }

    //Get the NvTriStrip code to do it's evil work
    PrimitiveGroup *prims;
    unsigned short numprims;
    bool done = GenerateStrips(&indices[0], indices.size(), &prims, &numprims);

    //first line is the number of primitive groups.
    cout << numprims << " " << endl;
    for (unsigned int primidx = 0; primidx < numprims; primidx++)
    {
        PrimitiveGroup &pg = prims[primidx];

        cout << pg.type << endl;
        cout << pg.numIndices << endl;;

        for (unsigned int i = 0; i < pg.numIndices; i++)
        {
            cout << pg.indices[i] << " ";
        }

        cout << endl;
    }

    //cleanup
    delete[] prims;

    return 0;
}
