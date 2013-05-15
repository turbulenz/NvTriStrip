#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>

#include "NvTriStrip.h"

using namespace std;

const static int versionMajor = 1;
const static int versionMinor = 0;


static void PrintUsage()
{
    cout << "Version: " << versionMajor << "." << versionMinor << endl;
    cout << "Usage: NvTriStripper [options] <input >output" << endl;
    cout << "Options:" << endl;
    cout << "  -cs, --cachesize SIZE  The cache size to optimize for (default 16)" << endl;
    cout << "  -s, --stitchstrips     Stitch strips together (default does not stich)" << endl;
    cout << "  -h, --help             Print this message" << endl;
    cout << "  -v, --version          Print the programs's version" << endl;
    cout << endl;
    cout << "Input would be read from stdin, one index per line." << endl;
    cout << "The maximum index value supported is 65535." << endl;
    cout << "An index value of -1 will signify the end of the input stream." << endl;
    cout << endl;
    cout << "Ouput would be written to stdin." << endl;
    cout << "The first line would be the number of primitive groups." << endl;
    cout << "The first line for each group would be the primitive type:" << endl;
    cout << "  0  for triangle lists" << endl;
    cout << "  1  for triangle strips" << endl;
    cout << "The second line for each group would be the number of indices." << endl;
    cout << "The third line for each group would be the indices separated by spaces." << endl;
}


int main (int argc, char * const argv[])
{
    bool stitchstrips = false;
    int argidx = 1;

    while (argidx < argc)
    {
        //cerr << "argument: " << argidx << " = " << argv[argidx] << endl;
        if (strcmp(argv[argidx], "-cs") == 0 ||
            strcmp(argv[argidx], "--cachesize") == 0)
        {
            argidx++;

            int cachesize = 16;
            int num = sscanf(argv[argidx], "%d", &cachesize);
            if (num != 1)
            {
                cerr << "Error reading cache size for -cs argument" << endl;
                exit(-1);
            }

            //cerr << "Cache Size " << cachesize << endl;
            SetCacheSize(cachesize);
            argidx++;
        }
        else if (strcmp(argv[argidx], "-s") == 0 ||
                 strcmp(argv[argidx], "--stitchstrips") == 0)
        {
            //cerr << "Stitching on" << endl;
            argidx++;
            stitchstrips = true;
        }
        else if (strcmp(argv[argidx], "-v") == 0 ||
                 strcmp(argv[argidx], "--version") == 0)
        {
            cout << versionMajor << "." << versionMinor << endl;
            exit(0);
        }
        else if (strcmp(argv[argidx], "-h") == 0 ||
                 strcmp(argv[argidx], "--help") == 0)
        {
            PrintUsage();
            exit(0);
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

    //Call NvTriStrip to generate the strips
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
