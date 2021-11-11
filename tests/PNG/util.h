// Reverse bits to change endianness
template <typename T>
T reverse(T n, unsigned bits_num)
{
    T rv = 0;
REVERSE_BITS:
    for (int i = 0; i < bits_num; i++)
    {
#pragma HLS PIPELINE
        rv <<= 1;
        rv |= (n & 0x01);
        n >>= 1;
    }
    return rv;
}
// usare sizeof(T) invece di bits_num????
