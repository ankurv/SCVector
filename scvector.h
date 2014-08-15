#include <iostream>
#include <stdint.h>
#include <vector>
#include <cstring>
#include <sstream>
using namespace std;

static const uint32_t MAXVARINTLEN  = 10;
static const uint32_t INIT_SC_SIZE = 512;
static const uint32_t BLOCK_PADDING = 10;
static const uint32_t MAXBLOCKLEN = (10*1024*1024);

/* returns bytes used by the varint value */
inline uint32_t VarIntLenght(uint64_t v)
{
    uint32_t i = 1;
    for(; v ; v >>= 7, i++){}
    return i;
}
/* returns bytes used by the varint value */
inline uint32_t WriteVarInt(unsigned char *p, uint64_t v)
{
    unsigned char *q =  p;
    do { *q++ = static_cast<unsigned char>((v & 0x7f) | 0x80); v >>= 7; }while( v!=0 );
    q[-1] &= 0x7f; 
    return (uint32_t) (q - p);
}

/* returns bytes read to get the varint value*/
inline uint32_t ReadVarInt(const unsigned char * const p, uint64_t &v)
{
    unsigned char b;
    bool done = false;
    uint32_t i, s;
    for (i = 0, s = 0; (!done && (i<MAXVARINTLEN)) ; ++i)
    {
        b = p[i];
        if (b < 0x80) { v |= uint64_t(b) << s; done = true; }
        else { v |= uint64_t(b&0x7f) << s; s += 7; }
    }
    return i;
}

class SCVectorIterator
{
    public:
        SCVectorIterator(const unsigned char* d):_dataptr(d), _skip_length (0), _read_value(0){}
        SCVectorIterator(const SCVectorIterator& iter):_dataptr(iter._dataptr), _skip_length(iter._skip_length), _read_value(iter._read_value){}
        inline bool operator==(const SCVectorIterator& iter) { return _dataptr == iter._dataptr; }
        inline bool operator!=(const SCVectorIterator& iter) { return _dataptr != iter._dataptr; }
        inline void readValue()
        {
            if (*_dataptr == 0) { //position to next block address
                uint64_t next_block_address = 0;
                _skip_length = ReadVarInt(_dataptr+1, next_block_address);
                _dataptr = reinterpret_cast<unsigned char*>(next_block_address);
            }
            uint64_t delta_value = 0;
            _skip_length =  ReadVarInt(_dataptr, delta_value);
            _read_value += delta_value;
        }
        inline uint64_t operator*() //return value pointed by _dataptr
        {
            if (_skip_length) //was just read
                return _read_value;
            readValue();
            //check end of block
            return _read_value;
        }
        inline SCVectorIterator& operator++()
        {
            if (!_skip_length) {
                readValue();
            }
            _dataptr +=_skip_length;
            _skip_length = 0;
            return *this;
        }
    private:
        const unsigned char* _dataptr;
        uint32_t             _skip_length;
        uint64_t             _read_value;

};

class SCVector
{
    public:
        SCVector(uint32_t block_size_init = INIT_SC_SIZE, uint32_t max_block_size = MAXBLOCKLEN)
        {
            _block_size_to_create   = block_size_init;
            _max_block_size         = max_block_size;
            _space_left_in_block    = 0;
            _num_items              = 0;
            _total_memory           = 0;
            _written_value          = 0;
            _curr_data_ptr          = NULL;
        }
        ~SCVector()
        {
            for (size_t i = 0 ;i < _blocks.size() ; i++)
                delete [] _blocks[i];
        }
        const unsigned char* begin() const { return _blocks[0]; }
        const unsigned char* end()   const { return _curr_data_ptr;}
        const unsigned char* push_back(uint64_t v)
        {
            if (_space_left_in_block < MAXVARINTLEN)
                CreateNewBlock();
            uint32_t space_used = WriteVarInt(_curr_data_ptr, v - _written_value);
            _space_left_in_block -= space_used;
            _curr_data_ptr       += space_used;
            _written_value        = v;
            ++_num_items;
            return _curr_data_ptr;
        }

        string print_stats()
        {
            stringstream os;
            os << "Total items stored  = " << _num_items << endl;
            os << "Number of blocks    = " << _blocks.size() << endl;
            os << "Total memory(KB)    = " << ((float)_total_memory)/(1024)<< endl;
            return os.str();
        }
    private:
        uint32_t                _block_size_to_create;
        uint32_t                _max_block_size;
        uint32_t                _space_left_in_block;
        uint32_t                _num_items;
        uint32_t                _total_memory;
        uint64_t                _written_value;
        unsigned char*          _curr_data_ptr;
        vector<unsigned char*>  _blocks;
        void CreateNewBlock()
        {
            uint32_t memory_needed = _block_size_to_create + BLOCK_PADDING;
            unsigned char* new_block = new unsigned char [memory_needed];
            if (_curr_data_ptr)
            {
                *_curr_data_ptr++ = 0; //marker
                WriteVarInt(_curr_data_ptr, (uint64_t)new_block); //put address of the next block at the end
            }
            _curr_data_ptr = new_block;
            _space_left_in_block = _block_size_to_create;
            _blocks.push_back(_curr_data_ptr);
            _total_memory += memory_needed;
            //for next block
            _block_size_to_create = (_block_size_to_create < MAXBLOCKLEN) ? (_block_size_to_create*2) : MAXBLOCKLEN;
        }
};

