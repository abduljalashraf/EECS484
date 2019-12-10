#include "Join.hpp"
#include <functional>



// DON'T NEED TO USE DISK_SIZE_IN_PAGE or DISK_READ() 
// use loadFromDisk in the memory class
// FLUSH OUTPUT BUFFER only when it is full or at the VERY end of the probing phase if it isn't empty


// OH QUESTIONS:

// how to find number of records in left_rel and right_rel?
// What specific mem page to load to? just choose an input buffer page?
// what is the page ID in line 51?


/*
 * TODO: Student implementation
 * Input: Disk, Memory, Disk page ids for left relation, Disk page ids for right relation
 * Output: Vector of Buckets after partition
 */
vector<Bucket> partition(
    Disk* disk, 
    Mem* mem, 
    pair<unsigned int, unsigned int> left_rel, 
    pair<unsigned int, unsigned int> right_rel) {

	
	// ACTUALLY FIND THE LARGER/SMALLER RELATIONS
	pair<unsigned int, unsigned int> smaller_rel = left_rel;
	pair<unsigned int, unsigned int> larger_rel = right_rel;


	// initialize our output vector
	Bucket empty_bucket(disk);
	vector<Bucket> partitions((MEM_SIZE_IN_PAGE - 1), empty_bucket);


	// hash all the tuples of R into buckets
	for (unsigned int i = smaller_rel.first; i < smaller_rel.second; ++i) {
		mem->loadFromDisk(disk, i, SPECIFIC_MEM_PAGE);

		Page* page = mem->mem_page(SPECIFIC_MEM_PAGE);
		unsigned int num_records = page->size();
		for (unsigned int r = 0; r < num_records; ++r) {
			Record record = page->get_record(r); // index of vector<Record> in page.cpp
			unsigned int hash_val = (record.partition_hash()) % (MEM_SIZE_IN_PAGE - 1);

			if (smaller_rel == left_rel) {
				partitions[hash_val].add_left_rel_page(PAGE_ID);
			}
			else {
				partitions[hash_val].add_right_rel_page(PAGE_ID);
			}

		}
	}


	// hash all the tuples of S into buckets
	for (unsigned int i = larger_rel.first; i < larger_rel.second; ++i) {
		mem->loadFromDisk(disk, i, SPECIFIC_MEM_PAGE);
	}

	// write to disk?
    
}


/*
 * TODO: Student implementation
 * Input: Disk, Memory, Vector of Buckets after partition
 * Output: Vector of disk page ids for join result
 */
vector<unsigned int> probe(Disk* disk, Mem* mem, vector<Bucket>& partitions) {

	// need to calculate which relation is smaller?? (compare total sizes, # of records)

	// pull partition Ri and create a hash table of the tuples in Ri

	// check partition Si for matches, hash first though

	// use == to check if any of the relations' keys in the smaller relation match the key of the larger rel
}




// use smaller relation as outer (or left if equal)