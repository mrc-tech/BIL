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







inline std::vector<unsigned char> str2bvec(std::string str)
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



template<typename T>
inline void append_to_vector(std::vector<T> &vec, std::vector<T> v)
{
	// append the vector v to the vector vec
	for(auto i=0; i<v.size(); i++)
		vec.push_back(v[i]);
}



inline std::vector<unsigned char> u32bigEndian2vec(int32_t x)
{
	// convert a 32 bit integer into a byte vector in Big-Endian notation
	std::vector<unsigned char> res;
	
	res.push_back((x & 0xFF000000) >> 24);
	res.push_back((x & 0x00FF0000) >> 16);
	res.push_back((x & 0x0000FF00) >>  8);
	res.push_back((x & 0x000000FF)      );
	
	return res;
}


template<typename T>
inline std::vector<unsigned char> bigEndian2vec(T x)
{
	// convert an integer into a byte vector in Big-Endian notation
	std::vector<unsigned char> res;
	
	if(sizeof(T) == 1){
		// 8 bit input
		res.push_back(x);
	}else
	if(sizeof(T) == 2){
		// 16 bit input
		res.push_back((x & 0xFF00) >> 8);
		res.push_back((x & 0x00FF)     );
	}else
	if(sizeof(T) == 4){
		// 32 bit input
		res.push_back((x & 0xFF000000) >> 24);
		res.push_back((x & 0x00FF0000) >> 16);
		res.push_back((x & 0x0000FF00) >>  8);
		res.push_back((x & 0x000000FF)      );
	}else
	if(sizeof(T) == 8){
		// 64 bit input
		res.push_back((x & 0xFF00000000000000) >> 56);
		res.push_back((x & 0x00FF000000000000) >> 48);
		res.push_back((x & 0x0000FF0000000000) >> 40);
		res.push_back((x & 0x000000FF00000000) >> 32);
		res.push_back((x & 0x00000000FF000000) >> 24);
		res.push_back((x & 0x0000000000FF0000) >> 16);
		res.push_back((x & 0x000000000000FF00) >>  8);
		res.push_back((x & 0x00000000000000FF)      );
	}
	
	
	return res;
}


