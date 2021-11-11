#include <vector>
#include <fstream>




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







std::vector<unsigned char> str2bvec(std::string str)
{
	// converte una stringa in un vettore di bytes
	std::vector<unsigned char> res;
	for(int i=0; i<str.length(); i++) res.push_back(str[i]);
	return res;
}





template<typename T>
inline void write_to_stream(std::ofstream& stream, const std::vector<T>& t)
{
	for(auto i=0; i<t.size(); i++)
		stream.write(reinterpret_cast<const char*>(&t[i]), sizeof(T));
}




void append_to_vector(std::vector<unsigned char> &vec, std::vector<unsigned char> v)
{
	// append the vector v to the vector vec
	for(auto i=0; i<v.size(); i++)
		vec.push_back(v[i]);
}


