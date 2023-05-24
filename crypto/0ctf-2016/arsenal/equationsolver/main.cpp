#include <cstdio>
#include <cstdint>
#include <cassert>
#include <cstring>
#include <ctime>
#include <array>
#include <tuple>
using namespace std;

#include "flatmultimap.h"
#include "aes.h"

#define MAKE_UINT32(a, b, c, d) ((uint32_t)(a) | ((uint32_t)(b)<<8) | ((uint32_t)(c)<<16) | ((uint32_t)(d)<<24))
typedef flatmultimap<uint32_t, tuple<uint8_t, uint8_t, uint8_t>, 97924731 > LUTTable;

// build lookup table to find k,C2,C3 by the first four values of S[SMul1[SMul2[i^k1]^k2]^k3]
void build_lookup_table(LUTTable &atable, const uint8_t* SLevel1, const uint8_t* SLevel2)
{
    for (int k1=0;k1<256;k1++)
    {
        for (int k2=0;k2<256;k2++)
        {
            for (int k3=0;k3<256;k3++)
            {
                const auto a0 = S[SLevel1[SLevel2[0^k1]^k2]^k3];
                const auto a1 = S[SLevel1[SLevel2[1^k1]^k2]^k3];
                const auto a2 = S[SLevel1[SLevel2[2^k1]^k2]^k3];
                const auto a3 = S[SLevel1[SLevel2[3^k1]^k2]^k3];
                const auto a4 = S[SLevel1[SLevel2[4^k1]^k2]^k3];
                const auto a5 = S[SLevel1[SLevel2[5^k1]^k2]^k3];

                const uint32_t key = MAKE_UINT32((a1^a2), (a2^a3), (a3^a4), (a4^a5)); // for arsenal (first element is not given)
                const auto value = make_tuple(k1, k2, k3);

                atable.insert(key, value);
            }
        }
    }
}

int solve3(int argc, const char* argv[])
{
    /*
    try to solve the equation system

    {
    a(i) = S[SMul1[SMul2[i^k]^C2]^C3]
    b(i) = S[SMul1[SMul2[i^k]^C4]^C5]
    c(i) = S[SMul1[SMul2[i^k]^C6]^C7]
    d(i) = S[SMul1[SMul2[i^k]^C8]^C9]

    array0  ={ S[MIX0(a,b,c,d)^k140]^k180 (for i=0 to 255) }
    array1  ={ S[MIX1(a,b,c,d)^k141]^k171 (for i=0 to 255) }
    array2  ={ S[MIX2(a,b,c,d)^k142]^k162 (for i=0 to 255) }
    array3  ={ S[MIX3(a,b,c,d)^k143]^k193 (for i=0 to 255) }
    }
    */

    // parse arguments: "aMul1,aMul2" , "bMul1,bMul2" , "cMul1,cMul2" , "dMul1, dMul2" , array0, array1, array2, array3
    assert(argc == 1 + 8);

    const unsigned char *SLevel1[4], *SLevel2[4];
    for (int i=0;i<4;i++)
    {
        int Mul1 = argv[1+i][0]-'0';
        assert(Mul1 >= 1 && Mul1 <= 3);
        SLevel1[i]=SMuli[Mul1-1];

        int Mul2 = argv[1+i][2]-'0';
        assert(Mul2 >= 1 && Mul2 <= 3);
        SLevel2[i]=SMuli[Mul2-1];
    }

    static unsigned char values[4][256];
    for (int j=0;j<4;j++)
    {
        char *pch = strtok(strdup(argv[5+j]), ", ;");
        int i;
        for (i=0;i<256 && pch!=nullptr;i++)
        {
            values[j][i] = atoi(pch);
            pch = strtok(nullptr, ", ;");
        }
        assert(i==256);
    }

    // generate look up tables
    LUTTable abcdtables[4];
    {
        const clock_t starttime = clock();
        for (int i=0;i<4;i++)
        {
            build_lookup_table(abcdtables[i], SLevel1[i], SLevel2[i]);
        }

        const clock_t endtime = clock();
        printf("lookup tables built in %.3f s\n", (endtime-starttime)/(float)CLOCKS_PER_SEC);
    }

    // brute force
    const int numofelements=5;
    int answercount = 0;

    #pragma omp parallel for num_threads(4)
    for (int k180=0;k180<256;k180++)
    {
        #define EXPERIMENTAL
        #ifdef EXPERIMENTAL
        if (answercount > 0)
        {
            // stop when one answer is found
            continue;
        }
        #endif
        const clock_t starttime = clock();

        uint8_t mix[4][numofelements];
        for (int i=0,j=0;j<numofelements;j++)
        {
            mix[i][j] = inv_S[values[i][j]^k180];
        }

        for (int k171=0;k171<256;k171++)
        {
            for (int i=1,j=0;j<numofelements;j++)
            {
                mix[i][j] = inv_S[values[i][j]^k171];
            }

            for (int k162=0;k162<256;k162++)
            {
                for (int i=2,j=0;j<numofelements;j++)
                {
                    mix[i][j] = inv_S[values[i][j]^k162];
                }

                for (int k193=0;k193<256;k193++)
                {
                    const int OK[4] = {k180, k171, k162, k193};

                    for (int i=3,j=0;j<numofelements;j++)
                    {
                        mix[i][j] = inv_S[values[i][j]^OK[i]];
                    }

                    uint8_t abcd[numofelements-1][4];
                    for (int i=0;i<numofelements-1;i++)
                    {
                        auto ret = fastinvmixcolumns({mix[0][i]^mix[0][i+1], mix[1][i]^mix[1][i+1], mix[2][i]^mix[2][i+1], mix[3][i]^mix[3][i+1]});
                        abcd[i][0] = ret[0]; // a[i]^a[i+1]
                        abcd[i][1] = ret[1]; // b[i]^b[i+1]
                        abcd[i][2] = ret[2]; // c[i]^c[i+1]
                        abcd[i][3] = ret[3]; // d[i]^d[i+1]
                    }

                    const auto akey = MAKE_UINT32(abcd[0][0], abcd[1][0], abcd[2][0], abcd[3][0]);

                    const auto arange = abcdtables[0].equal_range(akey);
                    if (arange.first == arange.second) continue;

                    for (auto ait = arange.first; ait != arange.second; ait++)
                    {
                        const auto avalue = ait.value();
                        const uint8_t ak1 = get<0>(avalue);

                        const auto bkey = MAKE_UINT32(abcd[0][1], abcd[1][1], abcd[2][1], abcd[3][1]);
                        const auto brange = abcdtables[1].equal_range(bkey);

                        for (auto bit = brange.first; bit != brange.second; bit++)
                        {
                            const uint8_t bk1 = get<0>(bit.value());
                            if (ak1 != bk1) continue;

                            const auto ckey = MAKE_UINT32(abcd[0][2], abcd[1][2], abcd[2][2], abcd[3][2]);
                            const auto crange = abcdtables[2].equal_range(ckey);

                            for (auto cit = crange.first; cit != crange.second; cit++)
                            {
                                const uint8_t ck1 = get<0>(bit.value());
                                if (bk1 != ck1) continue;

                                const auto dkey = MAKE_UINT32(abcd[0][3], abcd[1][3], abcd[2][3], abcd[3][3]);
                                const auto drange = abcdtables[3].equal_range(dkey);

                                for (auto dit = drange.first; dit != drange.second; dit++)
                                {
                                    const uint8_t dk1 = get<0>(dit.value());
                                    if (ck1 != dk1) continue;

                                    #pragma omp critical (print)
                                    {
                                        printf("bingo k1=%d ok1=%d ok2=%d ok3=%d ok4=%d\n", ak1, OK[0], OK[1], OK[2], OK[3]);
                                    }

                                    #pragma omp atomic
                                    answercount++;
                                }
                            }
                        }
                    }
                }
            }
        }

        const clock_t endtime = clock();

        #pragma omp critical (print)
        {
            printf("one iteration: %.3f s\n", (endtime-starttime)/(float)CLOCKS_PER_SEC); // 2.5 seconds
        }

    }

    printf("%d answer(s) found\n", answercount);

    return (answercount>0)?0:1;
}

int main(int argc, const char* argv[])
{
    return solve3(argc, argv);
}
