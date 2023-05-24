#ifndef FLATMULTIMAP_H_INCLUDED
#define FLATMULTIMAP_H_INCLUDED

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

#endif // FLATMULTIMAP_H_INCLUDED
