#include "Join.hpp"
#include <functional>



// DON'T NEED TO USE DISK_SIZE_IN_PAGE or DISK_READ() 
// use loadFromDisk in the memory class
// FLUSH OUTPUT BUFFER only when it is full or at the VERY end of the probing phase if it isn't empty


// OH QUESTIONS:

// how to find number of records in left_rel and right_rel? need to calculate which rel is smaller WHEN?
// What specific mem page to load to? just choose an input buffer page?
// what is the page ID in line 51?
// flush output buffers to disk after hashing R? when flushing to disk, why do you not have to know where to put it? 
// check understanding of partition and probe
// final result of probe is what??


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


	// initialize our output vector
	Bucket empty_bucket(disk);
	vector<Bucket> partitions((MEM_SIZE_IN_PAGE - 1), empty_bucket);


	// hash all the tuples of left_rel into buckets
	for (unsigned int i = left_rel.first; i < left_rel.second; ++i) {

		mem->loadFromDisk(disk, i, (MEM_SIZE_IN_PAGE - 1)); // input buffer page is the last one in memory
		Page* input_buffer = mem->mem_page((MEM_SIZE_IN_PAGE - 1));
		unsigned int num_records = input_buffer->size();

		for (unsigned int r = 0; r < num_records; ++r) {

			Record record = input_buffer->get_record(r); // index of vector<Record> in page.cpp
			unsigned int hash_val = (record.partition_hash()) % (MEM_SIZE_IN_PAGE - 1);

			// check if this memory page is full
			if ((mem->mem_page(hash_val))->full()) {
				unsigned int flushed_disk_page = mem->flushToDisk(disk, hash_val);
				partitions[hash_val].add_left_rel_page(flushed_disk_page);
			}

			// loadRecord
			(mem->mem_page(hash_val))->loadRecord(record);

		}

	}
	// flush if anything left in B-1 buckets in memory pages (loop through and check size)
	for (unsigned int i = 0; i < (MEM_SIZE_IN_PAGE - 1); ++i) {
		if (((mem->mem_page(i))->size()) > 0) {
			unsigned int flushed_disk_page = mem->flushToDisk(disk, i+1);
			partitions[i+1].add_left_rel_page(flushed_disk_page);
		}
	}


	// hash all the tuples of right_rel into buckets
	for (unsigned int i = right_rel.first; i < right_rel.second; ++i) {

		mem->loadFromDisk(disk, i, (MEM_SIZE_IN_PAGE - 1)); // input buffer page is the last one in memory
		Page* input_buffer = mem->mem_page((MEM_SIZE_IN_PAGE - 1));
		unsigned int num_records = input_buffer->size();

		for (unsigned int r = 0; r < num_records; ++r) {

			Record record = input_buffer->get_record(r); // index of vector<Record> in page.cpp
			unsigned int hash_val = (record.partition_hash()) % (MEM_SIZE_IN_PAGE - 1);

			// check if this memory page is full
			if ((mem->mem_page(hash_val))->full()) {
				unsigned int flushed_disk_page = mem->flushToDisk(disk, hash_val);
				partitions[hash_val].add_right_rel_page(flushed_disk_page);
			}

			// loadRecord
			(mem->mem_page(hash_val))->loadRecord(record);

		}
	}
	// flush if anything left in B-1 buckets in memory pages (loop through and check size)
	for (unsigned int i = 0; i < (MEM_SIZE_IN_PAGE - 1); ++i) {
		if (((mem->mem_page(i))->size()) > 0) {
			unsigned int flushed_disk_page = mem->flushToDisk(disk, i+1);
			partitions[i+1].add_right_rel_page(flushed_disk_page);
		}
	}

	return partitions;
    
}


/*
 * TODO: Student implementation
 * Input: Disk, Memory, Vector of Buckets after partition
 * Output: Vector of disk page ids for join result
 */
vector<unsigned int> probe(Disk* disk, Mem* mem, vector<Bucket>& partitions) {

	vector<unsigned int> result;
	
	unsigned int num_right_rel = 0;
	unsigned int num_left_rel = 0;
	for (unsigned int i = 0; i < partitions.size(); ++i) {
		num_right_rel += partitions[i].num_right_rel_record;
		num_left_rel += partitions[i].num_left_rel_record;
	}

	string outer_rel;
	if (num_right_rel >= num_left_rel) {
		outer_rel = "left";
	}
	else {
		outer_rel = "right";
	}

	// loop through each bucket one by one
	for (unsigned int i = 0; i < partitions.size(); ++i) {

		if (outer_rel == "left") {

			// loop through the left_rel items in each bucket
			for (unsigned int j = 0; j < partitions[i].num_left_rel_record; j++) {
				vector<unsigned int> left_rel = partitions[i].get_left_rel();
				unsigned int disk_page = left_rel[j];
				mem->loadFromDisk(disk, disk_page, (MEM_SIZE_IN_PAGE - 2));
				Page* input_buffer = mem->mem_page((MEM_SIZE_IN_PAGE - 2));
				unsigned int num_records = input_buffer->size();
				for (unsigned int r = 0; r < num_records; ++r) {
					Record record = input_buffer->get_record(r); // index of vector<Record> in page.cpp
					unsigned int hash_val = (record.probe_hash()) % (MEM_SIZE_IN_PAGE - 2);

					// loadRecord
					(mem->mem_page(hash_val))->loadRecord(record);
				}
			}

			Page* output_buffer = mem->mem_page((MEM_SIZE_IN_PAGE - 1));
			for (unsigned int j = 0; j < partitions[i].num_right_rel_record; j++) {
				vector<unsigned int> right_rel = partitions[i].get_right_rel();
				unsigned int disk_page = right_rel[j];
				mem->loadFromDisk(disk, disk_page, (MEM_SIZE_IN_PAGE - 2));
				Page* input_buffer = mem->mem_page((MEM_SIZE_IN_PAGE - 2));
				unsigned int num_records = input_buffer->size();
				for (unsigned int r = 0; r < num_records; ++r) {
					Record record = input_buffer->get_record(r); // index of vector<Record> in page.cpp
					unsigned int hash_val = (record.probe_hash()) % (MEM_SIZE_IN_PAGE - 2);

					Page* matching_page = mem->mem_page(hash_val);
					for (unsigned int s = 0; s < matching_page->size(); s++) {
						if (s == r) {
							// WE HAVE A MATCH
							// check if output buffer is full
							if (output_buffer->full()) {
								unsigned int flushed_disk_page = mem->flushToDisk(disk, (MEM_SIZE_IN_PAGE - 1));
								result.push_back(flushed_disk_page);
							}

							// load to output buffer
							(mem->mem_page((MEM_SIZE_IN_PAGE - 1)))->loadPair(record, matching_page->get_record(s));

						}
					}
				}
			}

		}



		if (outer_rel == "right") {

			// loop through the left_rel items in each bucket
			for (unsigned int j = 0; j < partitions[i].num_right_rel_record; j++) {
				vector<unsigned int> right_rel = partitions[i].get_right_rel();
				unsigned int disk_page = right_rel[j];
				mem->loadFromDisk(disk, disk_page, (MEM_SIZE_IN_PAGE - 2));
				Page* input_buffer = mem->mem_page((MEM_SIZE_IN_PAGE - 2));
				unsigned int num_records = input_buffer->size();
				for (unsigned int r = 0; r < num_records; ++r) {
					Record record = input_buffer->get_record(r); // index of vector<Record> in page.cpp
					unsigned int hash_val = (record.probe_hash()) % (MEM_SIZE_IN_PAGE - 2);

					// loadRecord
					(mem->mem_page(hash_val))->loadRecord(record);
				}
			}

			Page* output_buffer = mem->mem_page((MEM_SIZE_IN_PAGE - 1));
			for (unsigned int j = 0; j < partitions[i].num_left_rel_record; j++) {
				vector<unsigned int> left_rel = partitions[i].get_left_rel();
				unsigned int disk_page = left_rel[j];
				mem->loadFromDisk(disk, disk_page, (MEM_SIZE_IN_PAGE - 2));
				Page* input_buffer = mem->mem_page((MEM_SIZE_IN_PAGE - 2));
				unsigned int num_records = input_buffer->size();
				for (unsigned int r = 0; r < num_records; ++r) {
					Record record = input_buffer->get_record(r); // index of vector<Record> in page.cpp
					unsigned int hash_val = (record.probe_hash()) % (MEM_SIZE_IN_PAGE - 2);

					Page* matching_page = mem->mem_page(hash_val);
					for (unsigned int s = 0; s < matching_page->size(); s++) {
						if (s == r) {
							// WE HAVE A MATCH
							// check if output buffer is full
							if (output_buffer->full()) {
								unsigned int flushed_disk_page = mem->flushToDisk(disk, (MEM_SIZE_IN_PAGE - 1));
								result.push_back(flushed_disk_page);
							}

							// load to output buffer
							(mem->mem_page((MEM_SIZE_IN_PAGE - 1)))->loadPair(record, matching_page->get_record(s));

						}
					}
				}
			}

		}

	}



	// my understanding of probe:
	// vector of buckets, partitions. Go through this one at a time and look at a whole bucket of Ri. Re-hash these things with new hashing function
	// then take the matching bucket Si and hash each of these things with the new hashing function. only things that match to the same bucket could match
	// this is when you use == to compare and look for matches/joins


	// in the probing phase, one buffer input page, one output buffer page, B - 2 left, use last two as input/output

	// pull partition Ri and create a hash table of the tuples in Ri

	// check partition Si for matches, hash first though

	// use == to check if any of the relations' keys in the smaller relation match the key of the larger rel
	return result;
}




// use smaller relation as outer (or left if equal)