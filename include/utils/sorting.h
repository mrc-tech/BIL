#ifndef MRC_SORTING
#define MRC_SORTING

#include <algorithm> // for "sort"
#include <vector>

template<typename A,typename B>
void sort_by_vector(std::vector<A>& score, std::vector<B>& data)
{
	// Sort data and score vectors based on score vector
	std::vector< std::pair<int, unsigned char> > vec; // vector of pairs (sort by first member)
	for(int i=0; i<data.size(); i++) vec.push_back(make_pair(score[i], data[i])); // populate vector of pairs
	std::sort(vec.begin(), vec.end());
	for(int i=0; i<score.size(); i++) score[i] = vec[i].first;
	for(int i=0; i<data.size(); i++)  data[i]  = vec[i].second;
}

#endif
