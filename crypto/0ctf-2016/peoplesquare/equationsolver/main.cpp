#include <cstdio>
#include <cstdint>
#include <cassert>
#include <cstring>
#include <ctime>
#include <array>
#include <tuple>
//#include <map>
#include <set>

using namespace std;

#define MAKE_UINT32(a, b, c, d) ((uint32_t)(a) | ((uint32_t)(b)<<8) | ((uint32_t)(c)<<16) | ((uint32_t)(d)<<24))

const unsigned char S[256] = {
        0x63, 0x7C, 0x77, 0x7B, 0xF2, 0x6B, 0x6F, 0xC5, 0x30, 0x01, 0x67, 0x2B, 0xFE, 0xD7, 0xAB, 0x76,
        0xCA, 0x82, 0xC9, 0x7D, 0xFA, 0x59, 0x47, 0xF0, 0xAD, 0xD4, 0xA2, 0xAF, 0x9C, 0xA4, 0x72, 0xC0,
        0xB7, 0xFD, 0x93, 0x26, 0x36, 0x3F, 0xF7, 0xCC, 0x34, 0xA5, 0xE5, 0xF1, 0x71, 0xD8, 0x31, 0x15,
        0x04, 0xC7, 0x23, 0xC3, 0x18, 0x96, 0x05, 0x9A, 0x07, 0x12, 0x80, 0xE2, 0xEB, 0x27, 0xB2, 0x75,
        0x09, 0x83, 0x2C, 0x1A, 0x1B, 0x6E, 0x5A, 0xA0, 0x52, 0x3B, 0xD6, 0xB3, 0x29, 0xE3, 0x2F, 0x84,
        0x53, 0xD1, 0x00, 0xED, 0x20, 0xFC, 0xB1, 0x5B, 0x6A, 0xCB, 0xBE, 0x39, 0x4A, 0x4C, 0x58, 0xCF,
        0xD0, 0xEF, 0xAA, 0xFB, 0x43, 0x4D, 0x33, 0x85, 0x45, 0xF9, 0x02, 0x7F, 0x50, 0x3C, 0x9F, 0xA8,
        0x51, 0xA3, 0x40, 0x8F, 0x92, 0x9D, 0x38, 0xF5, 0xBC, 0xB6, 0xDA, 0x21, 0x10, 0xFF, 0xF3, 0xD2,
        0xCD, 0x0C, 0x13, 0xEC, 0x5F, 0x97, 0x44, 0x17, 0xC4, 0xA7, 0x7E, 0x3D, 0x64, 0x5D, 0x19, 0x73,
        0x60, 0x81, 0x4F, 0xDC, 0x22, 0x2A, 0x90, 0x88, 0x46, 0xEE, 0xB8, 0x14, 0xDE, 0x5E, 0x0B, 0xDB,
        0xE0, 0x32, 0x3A, 0x0A, 0x49, 0x06, 0x24, 0x5C, 0xC2, 0xD3, 0xAC, 0x62, 0x91, 0x95, 0xE4, 0x79,
        0xE7, 0xC8, 0x37, 0x6D, 0x8D, 0xD5, 0x4E, 0xA9, 0x6C, 0x56, 0xF4, 0xEA, 0x65, 0x7A, 0xAE, 0x08,
        0xBA, 0x78, 0x25, 0x2E, 0x1C, 0xA6, 0xB4, 0xC6, 0xE8, 0xDD, 0x74, 0x1F, 0x4B, 0xBD, 0x8B, 0x8A,
        0x70, 0x3E, 0xB5, 0x66, 0x48, 0x03, 0xF6, 0x0E, 0x61, 0x35, 0x57, 0xB9, 0x86, 0xC1, 0x1D, 0x9E,
        0xE1, 0xF8, 0x98, 0x11, 0x69, 0xD9, 0x8E, 0x94, 0x9B, 0x1E, 0x87, 0xE9, 0xCE, 0x55, 0x28, 0xDF,
        0x8C, 0xA1, 0x89, 0x0D, 0xBF, 0xE6, 0x42, 0x68, 0x41, 0x99, 0x2D, 0x0F, 0xB0, 0x54, 0xBB, 0x16
};

const unsigned char inv_S[256] = {
        0x52, 0x09, 0x6A, 0xD5, 0x30, 0x36, 0xA5, 0x38, 0xBF, 0x40, 0xA3, 0x9E, 0x81, 0xF3, 0xD7, 0xFB,
        0x7C, 0xE3, 0x39, 0x82, 0x9B, 0x2F, 0xFF, 0x87, 0x34, 0x8E, 0x43, 0x44, 0xC4, 0xDE, 0xE9, 0xCB,
        0x54, 0x7B, 0x94, 0x32, 0xA6, 0xC2, 0x23, 0x3D, 0xEE, 0x4C, 0x95, 0x0B, 0x42, 0xFA, 0xC3, 0x4E,
        0x08, 0x2E, 0xA1, 0x66, 0x28, 0xD9, 0x24, 0xB2, 0x76, 0x5B, 0xA2, 0x49, 0x6D, 0x8B, 0xD1, 0x25,
        0x72, 0xF8, 0xF6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xD4, 0xA4, 0x5C, 0xCC, 0x5D, 0x65, 0xB6, 0x92,
        0x6C, 0x70, 0x48, 0x50, 0xFD, 0xED, 0xB9, 0xDA, 0x5E, 0x15, 0x46, 0x57, 0xA7, 0x8D, 0x9D, 0x84,
        0x90, 0xD8, 0xAB, 0x00, 0x8C, 0xBC, 0xD3, 0x0A, 0xF7, 0xE4, 0x58, 0x05, 0xB8, 0xB3, 0x45, 0x06,
        0xD0, 0x2C, 0x1E, 0x8F, 0xCA, 0x3F, 0x0F, 0x02, 0xC1, 0xAF, 0xBD, 0x03, 0x01, 0x13, 0x8A, 0x6B,
        0x3A, 0x91, 0x11, 0x41, 0x4F, 0x67, 0xDC, 0xEA, 0x97, 0xF2, 0xCF, 0xCE, 0xF0, 0xB4, 0xE6, 0x73,
        0x96, 0xAC, 0x74, 0x22, 0xE7, 0xAD, 0x35, 0x85, 0xE2, 0xF9, 0x37, 0xE8, 0x1C, 0x75, 0xDF, 0x6E,
        0x47, 0xF1, 0x1A, 0x71, 0x1D, 0x29, 0xC5, 0x89, 0x6F, 0xB7, 0x62, 0x0E, 0xAA, 0x18, 0xBE, 0x1B,
        0xFC, 0x56, 0x3E, 0x4B, 0xC6, 0xD2, 0x79, 0x20, 0x9A, 0xDB, 0xC0, 0xFE, 0x78, 0xCD, 0x5A, 0xF4,
        0x1F, 0xDD, 0xA8, 0x33, 0x88, 0x07, 0xC7, 0x31, 0xB1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xEC, 0x5F,
        0x60, 0x51, 0x7F, 0xA9, 0x19, 0xB5, 0x4A, 0x0D, 0x2D, 0xE5, 0x7A, 0x9F, 0x93, 0xC9, 0x9C, 0xEF,
        0xA0, 0xE0, 0x3B, 0x4D, 0xAE, 0x2A, 0xF5, 0xB0, 0xC8, 0xEB, 0xBB, 0x3C, 0x83, 0x53, 0x99, 0x61,
        0x17, 0x2B, 0x04, 0x7E, 0xBA, 0x77, 0xD6, 0x26, 0xE1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0C, 0x7D
};

const unsigned char SMuli[3][256] = {
    {// S*1
        0x63, 0x7C, 0x77, 0x7B, 0xF2, 0x6B, 0x6F, 0xC5, 0x30, 0x01, 0x67, 0x2B, 0xFE, 0xD7, 0xAB, 0x76,
        0xCA, 0x82, 0xC9, 0x7D, 0xFA, 0x59, 0x47, 0xF0, 0xAD, 0xD4, 0xA2, 0xAF, 0x9C, 0xA4, 0x72, 0xC0,
        0xB7, 0xFD, 0x93, 0x26, 0x36, 0x3F, 0xF7, 0xCC, 0x34, 0xA5, 0xE5, 0xF1, 0x71, 0xD8, 0x31, 0x15,
        0x04, 0xC7, 0x23, 0xC3, 0x18, 0x96, 0x05, 0x9A, 0x07, 0x12, 0x80, 0xE2, 0xEB, 0x27, 0xB2, 0x75,
        0x09, 0x83, 0x2C, 0x1A, 0x1B, 0x6E, 0x5A, 0xA0, 0x52, 0x3B, 0xD6, 0xB3, 0x29, 0xE3, 0x2F, 0x84,
        0x53, 0xD1, 0x00, 0xED, 0x20, 0xFC, 0xB1, 0x5B, 0x6A, 0xCB, 0xBE, 0x39, 0x4A, 0x4C, 0x58, 0xCF,
        0xD0, 0xEF, 0xAA, 0xFB, 0x43, 0x4D, 0x33, 0x85, 0x45, 0xF9, 0x02, 0x7F, 0x50, 0x3C, 0x9F, 0xA8,
        0x51, 0xA3, 0x40, 0x8F, 0x92, 0x9D, 0x38, 0xF5, 0xBC, 0xB6, 0xDA, 0x21, 0x10, 0xFF, 0xF3, 0xD2,
        0xCD, 0x0C, 0x13, 0xEC, 0x5F, 0x97, 0x44, 0x17, 0xC4, 0xA7, 0x7E, 0x3D, 0x64, 0x5D, 0x19, 0x73,
        0x60, 0x81, 0x4F, 0xDC, 0x22, 0x2A, 0x90, 0x88, 0x46, 0xEE, 0xB8, 0x14, 0xDE, 0x5E, 0x0B, 0xDB,
        0xE0, 0x32, 0x3A, 0x0A, 0x49, 0x06, 0x24, 0x5C, 0xC2, 0xD3, 0xAC, 0x62, 0x91, 0x95, 0xE4, 0x79,
        0xE7, 0xC8, 0x37, 0x6D, 0x8D, 0xD5, 0x4E, 0xA9, 0x6C, 0x56, 0xF4, 0xEA, 0x65, 0x7A, 0xAE, 0x08,
        0xBA, 0x78, 0x25, 0x2E, 0x1C, 0xA6, 0xB4, 0xC6, 0xE8, 0xDD, 0x74, 0x1F, 0x4B, 0xBD, 0x8B, 0x8A,
        0x70, 0x3E, 0xB5, 0x66, 0x48, 0x03, 0xF6, 0x0E, 0x61, 0x35, 0x57, 0xB9, 0x86, 0xC1, 0x1D, 0x9E,
        0xE1, 0xF8, 0x98, 0x11, 0x69, 0xD9, 0x8E, 0x94, 0x9B, 0x1E, 0x87, 0xE9, 0xCE, 0x55, 0x28, 0xDF,
        0x8C, 0xA1, 0x89, 0x0D, 0xBF, 0xE6, 0x42, 0x68, 0x41, 0x99, 0x2D, 0x0F, 0xB0, 0x54, 0xBB, 0x16
    },
    {// S*2
        198,248,238,246,255,214,222,145,96,2,206,86,231,181,77,236,143,31,137,250,239,178,142,251,65,179,95,69,35,83,228,155,117,225,61,76,108,126,245,131,104,81,209,249,226,171,98,42,8,149,70,157,48,55,10,47,14,36,27,223,205,78,127,234,18,29,88,52,54,220,180,91,164,118,183,125,82,221,94,19,166,185,0,193,64,227,121,182,212,141,103,114,148,152,176,133,187,197,79,237,134,154,102,17,138,233,4,254,160,120,37,75,162,93,128,5,63,33,112,241,99,119,175,66,32,229,253,191,129,24,38,195,190,53,136,46,147,85,252,122,200,186,50,230,192,25,158,163,68,84,59,11,140,199,107,40,167,188,22,173,219,100,116,20,146,12,72,184,159,189,67,196,57,49,211,242,213,139,110,218,1,177,156,73,216,172,243,207,202,244,71,16,111,240,74,92,56,87,115,151,203,161,232,62,150,97,13,15,224,124,113,204,144,6,247,28,194,106,174,105,23,153,58,39,217,235,43,34,210,169,7,51,45,60,21,201,135,170,80,165,3,89,9,26,101,215,132,208,130,41,90,30,123,168,109,44
    },
    {// S*3
        165,132,153,141,13,189,177,84,80,3,169,125,25,98,230,154,69,157,64,135,21,235,201,11,236,103,253,234,191,247,150,91,194,28,174,106,90,65,2,79,92,244,52,8,147,115,83,63,12,82,101,94,40,161,15,181,9,54,155,61,38,105,205,159,27,158,116,46,45,178,238,251,246,77,97,206,123,62,113,151,245,104,0,44,96,31,200,237,190,70,217,75,222,212,232,74,107,42,229,22,197,215,85,148,207,16,6,129,240,68,186,227,243,254,192,138,173,188,72,4,223,193,117,99,48,26,14,109,76,20,53,47,225,162,204,57,87,242,130,71,172,231,43,149,160,152,209,127,102,126,171,131,202,41,211,60,121,226,29,118,59,86,78,30,219,10,108,228,93,110,239,166,168,164,55,139,50,67,89,183,140,100,210,224,180,250,7,37,175,142,233,24,213,136,111,114,36,241,199,81,35,124,156,33,221,220,134,133,144,66,196,170,216,5,1,18,163,95,249,208,145,88,39,185,56,19,179,51,187,112,137,167,182,34,146,32,73,255,120,122,143,248,128,23,218,49,198,184,195,176,119,17,203,252,214,58
    }
};

int solve2(int argc, const char* argv[])
{
    // try to solve S[SMul1[SMul2[i^k1]^k2]^k3]^k4=array

    assert(argc == 1 + 3);
    // argv = Mul1, Mul2, array of 256 values for each i

    // parse arguments
    int Mul1 = atoi(argv[1]);
    assert(Mul1 >= 1 && Mul1 <= 3);
    const unsigned char *SLevel1=SMuli[Mul1-1];

    int Mul2 = atoi(argv[2]);
    assert(Mul2 >= 1 && Mul2 <= 3);
    const unsigned char *SLevel2=SMuli[Mul2-1];

    unsigned char arr[256];
    char *pch = strtok(strdup(argv[3]), ", ;");
    int i;
    for (i=0;i<256 && pch!=nullptr;i++)
    {
        arr[i] = atoi(pch);
        pch = strtok(nullptr, ", ;");
    }
    assert(i==256);
    //for (i=0;i<256;i++)
    //    printf("%d,", arr[i]);

    long long answercnt=0;

    // try solve it
    for (int k1=0;k1<256;k1++)
    {
        for (int k2=0;k2<256;k2++)
        {
            for (int k3=0;k3<256;k3++)
            {
                bool wrong=false;
                unsigned char last=S[SLevel1[SLevel2[0^k1]^k2]^k3];
                unsigned char k4=arr[0]^last;
                for (int i=1;i<256;i++)
                {
                    unsigned char cur=S[SLevel1[SLevel2[(i)^k1]^k2]^k3];
                    if ((arr[i]^arr[i-1])!=(cur^last))
                    {
                        wrong=true;
                        break;
                    }
                    last=cur;
                }
                if (wrong) continue;

                // found correct answer
                printf("lucky k1=%d k2=%d k3=%d k4=%d\n", k1, k2, k3, k4);
                ++answercnt;
            }
        }
    }
    return answercnt==1?0:1;
}

template <typename KeyType, typename ValueType, int EntriesCount>
class flatmultimap
{
protected:
    struct Entry
    {
        KeyType key;
        ValueType value;
    };

    Entry* entries;

    mutable long long accinsertoffset=0;
    mutable long long totinserts=0;
    mutable long long accsearchoffset=0;
    mutable long long totsearches=0;
    mutable long long tothits=0;

public:
    static size_t nextpos(size_t pos)
    {
        if (__builtin_expect(((pos+1)==EntriesCount), 0))
            return 0;
        else
            return pos+1;
    }

    flatmultimap()
    {
        entries = new Entry[EntriesCount];
        memset(entries, 0, sizeof(Entry) * EntriesCount);
    }

    ~flatmultimap()
    {
        delete []entries;
        entries = nullptr;
    }

    flatmultimap(const flatmultimap&) = delete;

    size_t insert(const KeyType &key, const ValueType &value)
    {
        assert(key!=0);

        size_t pos=key%EntriesCount;
        size_t offset;
        for (offset=0;/*entries[pos].key!=key &&*/ entries[pos].key!=0;offset++,pos=nextpos(pos));
        assert(offset<=200);

        accinsertoffset += offset;

        offset = 0;
        entries[pos].key = key;
        entries[pos].value = value;

        ++totinserts;

        return pos;
    }

    class iterator
    {
    protected:
        const flatmultimap<KeyType, ValueType, EntriesCount>& m_map;
        const KeyType& m_key;
        size_t m_pos;

    public:
        iterator() = delete;
        iterator(const flatmultimap& map, const KeyType& key, const size_t startpos):m_map(map),m_key(key),m_pos(startpos) {}

        bool operator != (const iterator& another) const
        {
            return m_key != another.m_key || m_pos != another.m_pos;
        }

        bool operator == (const iterator& another) const
        {
            return m_key == another.m_key && m_pos == another.m_pos;
        }

        iterator operator ++(int)
        {
            for (m_pos=nextpos(m_pos);
                m_map.entries[m_pos].key!=0 && m_map.entries[m_pos].key!=m_key;
                m_pos=nextpos(m_pos))
                {

                }
            return *this;
        }

        const ValueType& value() const
        {
            return m_map.entries[m_pos].value;
        }
    };

    bool contains(const KeyType &key) const
    {
        ++totsearches;

        size_t pos=key%EntriesCount;
        for (;entries[pos].key!=0 && entries[pos].key!=key;pos=nextpos(pos))
            ++accsearchoffset;

        if (entries[pos].key==0)
            return false;

        ++tothits;
        return true;
    }

    pair<iterator, iterator> equal_range(const KeyType &key) const
    {
        ++totsearches;

        size_t pos=key%EntriesCount;
        for (;entries[pos].key!=0 && entries[pos].key!=key;pos=nextpos(pos))
            ++accsearchoffset;

        if (entries[pos].key==0)
        {
            // not found
            auto dummyit = iterator(*this, key, pos);
            return make_pair(dummyit, dummyit);
        }

        // found first match
        ++tothits;
        auto begin = iterator(*this, key, pos);

        for (pos=nextpos(pos);entries[pos].key!=0;pos=nextpos(pos))
        {
            ++accsearchoffset;
        }

        auto end = iterator(*this, key, pos);
        return make_pair(begin, end);
    }

    void printstats() const
    {
        puts("FlatMultiMap stats:");
        printf("- %I64u insertions, %I64u offsets\n", totinserts, accinsertoffset);
        printf("- Insert performance: %.4lf / insertion\n", 1.0f+(double)accinsertoffset/totinserts);
        printf("- %I64u searches, %I64u hits, %I64u offsets\n", totsearches, tothits, accsearchoffset);
        printf("- Search performance: %.4lf / search\n", 1.0f+(double)accsearchoffset/totsearches);
        printf("- Search hit rate: %.4lf / search\n", (double)tothits/totsearches);
    }
};

// build lookup table to find k,C2,C3 by the first four values of S[SMul1[SMul2[i^k1]^k2]^k3]
typedef flatmultimap<uint32_t, tuple<uint8_t, uint8_t, uint8_t>, 97924731 > TableType;

void buildtable(TableType &atable, const uint8_t* SLevel1, const uint8_t* SLevel2, bool check=false)
{
    size_t maxdup = 0;

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

                //const auto key = make_tuple(a0^a1, a1^a2, a2^a3, a3^a4);
                const uint32_t key = MAKE_UINT32((a0^a1), (a1^a2), (a2^a3), (a3^a4));
                const auto value = make_tuple(k1, k2, k3);

                atable.insert(key, value);
                /*
                if (atable.count(key) > maxdup)
                {
                    maxdup = atable.count(key);
                }
                */
            }
        }
    }

    printf("maxdup in lookup table: %u\n", maxdup); // 3

    if (check)
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

                    //const auto key = make_tuple(a0^a1, a1^a2, a2^a3, a3^a4);
                    const uint32_t key = MAKE_UINT32((a0^a1), (a1^a2), (a2^a3), (a3^a4));
                    const auto value = make_tuple(k1, k2, k3);

                    auto range = atable.equal_range(key);
                    bool found = false;

                    for (auto it=range.first;it!=range.second;it++)
                    {
                        if (it.value() == value)
                        {
                            found=true;
                        }
                    }

                    assert(found);

                }
            }
        }

        puts("check passed");
    }
}

void testmymap()
{
    {
        flatmultimap<int, int, 16> a;
        for (int i=1;i<16;i++)
        {
            a.insert(i,i);
        }
        for (int i=1;i<16;i++)
        {
            auto range = a.equal_range(i);
            int foundcount=0;
            for (auto it=range.first;it!=range.second;it++)
            {
                assert(it.value()==i);
                if (it.value()==i)
                    ++foundcount;
            }
            assert(foundcount==1);
        }
    }
    {
        flatmultimap<int, int, 16> a;
        for (int i=1;i<8;i++)
        {
            a.insert(i,i);
            a.insert(i,i+1);
        }
        for (int i=1;i<8;i++)
        {
            auto range = a.equal_range(i);
            int foundcount=0;
            for (auto it=range.first;it!=range.second;it++)
            {
                assert(it.value()==i || it.value()==i+1);
                if (it.value()==i ||it.value()==i+1)
                    ++foundcount;
            }
            assert(foundcount==2);
        }
        for (int i=9;i<100;i++)
        {
            auto range = a.equal_range(i);
            assert(range.first == range.second);
        }
    }
    {
        TableType table;
        buildtable(table, SMuli[1], SMuli[1], true);
        table.printstats();
    }


    puts("tests done");
}

inline uint8_t GMul(uint8_t u, uint8_t v)
{
    uint8_t p = 0;

    for (int i = 0; i < 8; ++i)
    {
        if (u & 0x01)      //
        {
            p ^= v;
        }

        int flag = (v & 0x80);
        v <<= 1;
        if (flag)
        {
            v ^= 0x1B; /* x^8 + x^4 + x^3 + x + 1 */
        }

        u >>= 1;
    }

    return p;
}

const uint8_t invM[4][4] = {{0x0E, 0x0B, 0x0D, 0x09},
    {0x09, 0x0E, 0x0B, 0x0D},
    {0x0D, 0x09, 0x0E, 0x0B},
    {0x0B, 0x0D, 0x09, 0x0E}
};


inline uint8_t invmix(int i, const array<uint8_t, 4> &column)
{
    return GMul(invM[i][0], column[0]) ^ GMul(invM[i][1], column[1]) ^ GMul(invM[i][2], column[2]) ^ GMul(invM[i][3], column[3]);
}

inline uint8_t GMulE(uint8_t x)
{
    static const uint8_t tableE[256] = {0,14,28,18,56,54,36,42,112,126,108,98,72,70,84,90,224,238,252,242,216,214,196,202,144,158,140,130,168,166,180,186,219,213,199,201,227,237,255,241,171,165,183,185,147,157,143,129,59,53,39,41,3,13,31,17,75,69,87,89,115,125,111,97,173,163,177,191,149,155,137,135,221,211,193,207,229,235,249,247,77,67,81,95,117,123,105,103,61,51,33,47,5,11,25,23,118,120,106,100,78,64,82,92,6,8,26,20,62,48,34,44,150,152,138,132,174,160,178,188,230,232,250,244,222,208,194,204,65,79,93,83,121,119,101,107,49,63,45,35,9,7,21,27,161,175,189,179,153,151,133,139,209,223,205,195,233,231,245,251,154,148,134,136,162,172,190,176,234,228,246,248,210,220,206,192,122,116,102,104,66,76,94,80,10,4,22,24,50,60,46,32,236,226,240,254,212,218,200,198,156,146,128,142,164,170,184,182,12,2,16,30,52,58,40,38,124,114,96,110,68,74,88,86,55,57,43,37,15,1,19,29,71,73,91,85,127,113,99,109,215,217,203,197,239,225,243,253,167,169,187,181,159,145,131,141};
    return tableE[x];
}

inline uint8_t GMulB(uint8_t x)
{
    static const uint8_t tableB[256] = {0,11,22,29,44,39,58,49,88,83,78,69,116,127,98,105,176,187,166,173,156,151,138,129,232,227,254,245,196,207,210,217,123,112,109,102,87,92,65,74,35,40,53,62,15,4,25,18,203,192,221,214,231,236,241,250,147,152,133,142,191,180,169,162,246,253,224,235,218,209,204,199,174,165,184,179,130,137,148,159,70,77,80,91,106,97,124,119,30,21,8,3,50,57,36,47,141,134,155,144,161,170,183,188,213,222,195,200,249,242,239,228,61,54,43,32,17,26,7,12,101,110,115,120,73,66,95,84,247,252,225,234,219,208,205,198,175,164,185,178,131,136,149,158,71,76,81,90,107,96,125,118,31,20,9,2,51,56,37,46,140,135,154,145,160,171,182,189,212,223,194,201,248,243,238,229,60,55,42,33,16,27,6,13,100,111,114,121,72,67,94,85,1,10,23,28,45,38,59,48,89,82,79,68,117,126,99,104,177,186,167,172,157,150,139,128,233,226,255,244,197,206,211,216,122,113,108,103,86,93,64,75,34,41,52,63,14,5,24,19,202,193,220,215,230,237,240,251,146,153,132,143,190,181,168,163
};
    return tableB[x];
}

inline uint8_t GMulD(uint8_t x)
{
    static const uint8_t tableD[256] = {0,13,26,23,52,57,46,35,104,101,114,127,92,81,70,75,208,221,202,199,228,233,254,243,184,181,162,175,140,129,150,155,187,182,161,172,143,130,149,152,211,222,201,196,231,234,253,240,107,102,113,124,95,82,69,72,3,14,25,20,55,58,45,32,109,96,119,122,89,84,67,78,5,8,31,18,49,60,43,38,189,176,167,170,137,132,147,158,213,216,207,194,225,236,251,246,214,219,204,193,226,239,248,245,190,179,164,169,138,135,144,157,6,11,28,17,50,63,40,37,110,99,116,121,90,87,64,77,218,215,192,205,238,227,244,249,178,191,168,165,134,139,156,145,10,7,16,29,62,51,36,41,98,111,120,117,86,91,76,65,97,108,123,118,85,88,79,66,9,4,19,30,61,48,39,42,177,188,171,166,133,136,159,146,217,212,195,206,237,224,247,250,183,186,173,160,131,142,153,148,223,210,197,200,235,230,241,252,103,106,125,112,83,94,73,68,15,2,21,24,59,54,33,44,12,1,22,27,56,53,34,47,100,105,126,115,80,93,74,71,220,209,198,203,232,229,242,255,180,185,174,163,128,141,154,151
};
    return tableD[x];
}

inline uint8_t GMul9(uint8_t x)
{
    static const uint8_t table9[256] = {0,9,18,27,36,45,54,63,72,65,90,83,108,101,126,119,144,153,130,139,180,189,166,175,216,209,202,195,252,245,238,231,59,50,41,32,31,22,13,4,115,122,97,104,87,94,69,76,171,162,185,176,143,134,157,148,227,234,241,248,199,206,213,220,118,127,100,109,82,91,64,73,62,55,44,37,26,19,8,1,230,239,244,253,194,203,208,217,174,167,188,181,138,131,152,145,77,68,95,86,105,96,123,114,5,12,23,30,33,40,51,58,221,212,207,198,249,240,235,226,149,156,135,142,177,184,163,170,236,229,254,247,200,193,218,211,164,173,182,191,128,137,146,155,124,117,110,103,88,81,74,67,52,61,38,47,16,25,2,11,215,222,197,204,243,250,225,232,159,150,141,132,187,178,169,160,71,78,85,92,99,106,113,120,15,6,29,20,43,34,57,48,154,147,136,129,190,183,172,165,210,219,192,201,246,255,228,237,10,3,24,17,46,39,60,53,66,75,80,89,102,111,116,125,161,168,179,186,133,140,151,158,233,224,251,242,205,196,223,214,49,56,35,42,21,28,7,14,121,112,107,98,93,84,79,70};
    return table9[x];
}

inline uint8_t fastinvmix0(const array<uint8_t, 4> &column)
{
    return GMulE(column[0]) ^ GMulB(column[1]) ^ GMulD(column[2]) ^ GMul9(column[3]);
}

inline uint8_t fastinvmix1(const array<uint8_t, 4> &column)
{
    return GMul9(column[0]) ^ GMulE(column[1]) ^ GMulB(column[2]) ^ GMulD(column[3]);
}

inline uint8_t fastinvmix2(const array<uint8_t, 4> &column)
{
    return GMulD(column[0]) ^ GMul9(column[1]) ^ GMulE(column[2]) ^ GMulB(column[3]);
}

inline uint8_t fastinvmix3(const array<uint8_t, 4> &column)
{
    return GMulB(column[0]) ^ GMulD(column[1]) ^ GMul9(column[2]) ^ GMulE(column[3]);
}

inline array<uint8_t, 4> fastinvmixcolumns(const array<uint8_t, 4> &column)
{
    return {fastinvmix0(column), fastinvmix1(column), fastinvmix2(column), fastinvmix3(column)};
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

    array0 = { S[MIX0(a,b,c,d)^k140]^k180 (for i=0 to 255) }
    array1 = { S[MIX1(a,b,c,d)^k141]^k171 (for i=0 to 255) }
    array2 = { S[MIX2(a,b,c,d)^k142]^k162 (for i=0 to 255) }
    array3 = { S[MIX3(a,b,c,d)^k143]^k193 (for i=0 to 255) }
    }
    */

    // parse arguments
    //argv = aMul, bMul, cMul, dMul, array0, array1, array2, array3
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

    // test
    auto tmp = fastinvmixcolumns({81,157,145,125});
    printf("%d %d %d %d\n", tmp[0], tmp[1], tmp[2], tmp[3]);
    assert(tmp[0]==196 && tmp[1]==19 && tmp[2]==68 && tmp[3]==179);

    // generate look up tables
    TableType abcdtables[4];
    {
        const clock_t starttime = clock();
        for (int i=0;i<4;i++)
        {
            buildtable(abcdtables[i], SLevel1[i], SLevel2[i]);
        }

        const clock_t endtime = clock();
        printf("lookup tables built in %.3f s\n", (endtime-starttime)/(float)CLOCKS_PER_SEC);
    }

    // brute force
    const int numofelements=4;
    int answercount = 0;

    #pragma omp parallel for num_threads(4)
    for (int k180=0;k180<256;k180++)
    {
        #define EXPERIMENTAL
        #ifdef EXPERIMENTAL
        if (answercount > 0)
        {
            continue;
        }
        #endif
        const clock_t starttime = clock();

        uint8_t mix[4][numofelements+1];
        for (int i=0,j=0;j<=numofelements;j++)
        {
            mix[i][j] = inv_S[values[i][j]^k180];
        }

        for (int k171=0;k171<256;k171++)
        {
            for (int i=1,j=0;j<=numofelements;j++)
            {
                mix[i][j] = inv_S[values[i][j]^k171];
            }

            for (int k162=0;k162<256;k162++)
            {
                for (int i=2,j=0;j<=numofelements;j++)
                {
                    mix[i][j] = inv_S[values[i][j]^k162];
                }

                for (int k193=0;k193<256;k193++)
                {
                    const int OK[4] = {k180, k171, k162, k193};

                    for (int i=3;i<4;i++)
                    {
                        for (int j=0;j<=numofelements;j++)
                        {
                            mix[i][j] = inv_S[values[i][j]^OK[i]];
                        }
                    }


                    uint8_t abcd[numofelements][4];
                    for (int i=0;i<numofelements;i++)
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

                                    #pragma omp critical (critical)
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

        #pragma omp critical (critical)
        {
            printf("one: %.3f s\n", (endtime-starttime)/(float)CLOCKS_PER_SEC); // 2.5 seconds

            //#define DEBUG

            #ifdef DEBUG
            for (int i=0;i<4;i++)
            {
                printf("%c table performace: \n", i+'A');
                abcdtables[i].printstats();
            }
            #endif

        }



    }

    printf("%d answer(s) found\n", answercount);

    return (answercount>0)?0:1;
}

int main(int argc, const char* argv[])
{
    //testmymap();
    return solve3(argc, argv);
}
