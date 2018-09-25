#include <iostream>
#include <fstream>
#include "sdsl/int_vector.hpp"
#include "sdsl/util.hpp"
#include "sdsl/int_vector.hpp"	// for the bit_vector class
#include <vector>		// std::vector
#include <algorithm>
#include "bloomfilter.h"

//using namespace std;
//using namespace sdsl;
const size_t sizebloom = 100;
BF bloom(sizebloom);

typedef bit_vector::size_type size_type;

template < typename T1, typename T2 > struct less_second {
	typedef pair < T1, T2 > type;
	bool operator () (type const &a, type const &b)const {
		return a.second < b.second;
	}
};

//function search, returns indexes in a vector
vector < int >search(const string & kmer, BF & bloomfilter)
{
	std::vector < int >null;
	if (bloomfilter.test_kmer(kmer)) {
		return bloomfilter.get_index(kmer);
	} else
		return null;
}

/*****************************************
 * Main
 *****************************************/

int main(int argc, char *argv[])
{
	BF bloom(sizebloom);
	vector < int >output;
	vector < string > kmer_vector;
	ifstream kmer_file("input/kmer.txt");
	std::string line;

	//add k-mer to bloom filter
	while (std::getline(kmer_file, line)) {
		bloom.add_kmer(line);
		kmer_vector.push_back(line);
	}
	kmer_file.close();

	//switch mode: we can add the indexes for each k-mer
	bloom.switch_mode(1);
	//add k-mer indexes to data structure
	for (int j = 0; j < kmer_vector.size(); j++) {
		string kmer_ref = kmer_vector[j];
		bloom.add_to_kmer(kmer_ref, 2);
	}

	//testing with some k-mers
	//single add
	bloom.add_to_kmer("ATC", 1984);
	bloom.add_to_kmer("CGG", 55);
	bloom.add_to_kmer("CTT", 14);
	bloom.add_to_kmer("TGG", 4);

	//multiple add  
	vector < int >ATC_idx = { 213, 434, 535, 65 };
	bloom.multiple_add_to_kmer("ATC", ATC_idx);

	//switch mode: now we can get the indexes
	bloom.switch_mode(2);
	cout << "searching... " << endl;
	output = search("CTT", bloom);

	//print the indexes vector
	if (!output.empty()) {
		for (int j = 0; j < output.size(); j++)
			cout << output[j] << " ";

	} else
		cout << "il kmer non è presente nel bloom filter" << endl;

}
