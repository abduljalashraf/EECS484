#include "Join.hpp"
#include <functional>
#include <iostream>




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
    mem->reset();

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
            Record record = (mem->mem_page(i))->get_record(0);
            unsigned int hash_val = (record.partition_hash()) % (MEM_SIZE_IN_PAGE - 1);
            
			unsigned int flushed_disk_page = mem->flushToDisk(disk, i);
            partitions[hash_val].add_left_rel_page(flushed_disk_page);
//			partitions[i+1].add_left_rel_page(flushed_disk_page);
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
            //THIS IS A TEST
            Record record = (mem->mem_page(i))->get_record(0);
            unsigned int hash_val = (record.partition_hash()) % (MEM_SIZE_IN_PAGE - 1);
			unsigned int flushed_disk_page = mem->flushToDisk(disk, i);
            partitions[hash_val].add_right_rel_page(flushed_disk_page);
//			partitions[i+1].add_right_rel_page(flushed_disk_page);

		}
	}

	return partitions;
    
}



//Where are we accessing disk wrong? is it from the value in partition


/*
 * TODO: Student implementation
 * Input: Disk, Memory, Vector of Buckets after partition
 * Output: Vector of disk page ids for join result
 */
vector<unsigned int> probe(Disk* disk, Mem* mem, vector<Bucket>& partitions) {
    mem->reset();
	vector<unsigned int> result;

	unsigned int num_right_rel = 0;
	unsigned int num_left_rel = 0;

	for (unsigned int i = 0; i < partitions.size(); ++i) {
		num_right_rel += partitions[i].num_right_rel_record;
		num_left_rel += partitions[i].num_left_rel_record;
	}
//    std::cout << "numRightRel = " << num_right_rel << std::endl;
//    std::cout << "numLeftRel = " << num_left_rel << std::endl;


	string outer_rel;
	if (num_right_rel >= num_left_rel) {
		outer_rel = "left";
	}
	else {
		outer_rel = "right";
	}

	// loop through each bucket one by one in the partitions vector
	for (unsigned int i = 0; i < partitions.size(); ++i) {

		if (outer_rel == "left") {

			// loop through the left_rel items in each bucket
			for (unsigned int j = 0; j < partitions[i].num_left_rel_record; j++) {
				vector<unsigned int> left_rel = partitions[i].get_left_rel();
				unsigned int disk_page = left_rel[j];                                           //Find page on disk where left_rel is
				mem->loadFromDisk(disk, disk_page, (MEM_SIZE_IN_PAGE - 2));
				Page* input_buffer = mem->mem_page((MEM_SIZE_IN_PAGE - 2));                     //load left_rel into input buffer page from memory
				unsigned int num_records = input_buffer->size();
				for (unsigned int r = 0; r < num_records; ++r) {                                //Loop through left_rel records in input buffer
					Record record = input_buffer->get_record(r);                                // index of vector<Record> in page.cpp
					unsigned int hash_val = (record.probe_hash()) % (MEM_SIZE_IN_PAGE - 2);     //re-hash the record value and get a new index

					// loadRecord into memory at the the new hashed value
					(mem->mem_page(hash_val))->loadRecord(record);
                    
                    //Do we have to check for memory overflow and flush here? ^^^
				}
			} //left_rel rehashing is done now
            
            //Start right_rel rehashing
			Page* output_buffer = mem->mem_page((MEM_SIZE_IN_PAGE - 1));                        //creates output buffer page that points to last page in memory
            for (unsigned int j = 0; j < partitions[i].num_right_rel_record; j++) {
                std::cout << "num_right_record = " << j << std::endl;

				vector<unsigned int> right_rel = partitions[i].get_right_rel();                 //get right_rel from ith bucket in partition
				unsigned int disk_page = right_rel[j];                                          //get page on disk where right_rel is
				mem->loadFromDisk(disk, disk_page, (MEM_SIZE_IN_PAGE - 2));
				Page* input_buffer = mem->mem_page((MEM_SIZE_IN_PAGE - 2));                     //create input buffer page that points to second to last page
				unsigned int num_records = input_buffer->size();
                //loop through right_rel records in input buffer
				for (unsigned int r = 0; r < num_records; ++r) {
					Record rightRecord = input_buffer->get_record(r);                           // index of vector<Record> in page.cpp
					unsigned int hash_val = (rightRecord.probe_hash()) % (MEM_SIZE_IN_PAGE - 2);//re-hash right_rel record and get new index value
                    
                    //get matching page to current re-hashed page from memory
					Page* matching_page = mem->mem_page(hash_val);
					for (unsigned int s = 0; s < matching_page->size(); s++) {
                        Record leftRecord = matching_page->get_record(s);
						if (leftRecord == rightRecord) {                                        //we are comparing records here, is that right?
							// WE HAVE A MATCH
							// check if output buffer is full

							if (output_buffer->full()) {
								unsigned int flushed_disk_page = mem->flushToDisk(disk, (MEM_SIZE_IN_PAGE - 1));
								result.push_back(flushed_disk_page);

							}

							// load to output buffer
                            //record = right_rel, matching_page->get_record(s) = left_rel
							(mem->mem_page((MEM_SIZE_IN_PAGE - 1)))->loadPair(leftRecord, rightRecord);
                            std::cout << "CHECKFORERROR" << std::endl;


						}
					}
				}
			}

		}



		if (outer_rel == "right") {

			// loop through the left_rel items in each bucket
			for (unsigned int j = 0; j < partitions[i].num_right_rel_record; j++) {
				vector<unsigned int> right_rel = partitions[i].get_right_rel();
				unsigned int disk_page = right_rel[j];                                              //find page on disk where right_rel is
				mem->loadFromDisk(disk, disk_page, (MEM_SIZE_IN_PAGE - 2));
				Page* input_buffer = mem->mem_page((MEM_SIZE_IN_PAGE - 2));                         //load right_rel into input buffer
				unsigned int num_records = input_buffer->size();
				for (unsigned int r = 0; r < num_records; ++r) {
					Record record = input_buffer->get_record(r);                                    // index of vector<Record> in page.cpp
					unsigned int hash_val = (record.probe_hash()) % (MEM_SIZE_IN_PAGE - 2);         //hash right_rel record

					// loadRecord into memory at hashed index from above
					(mem->mem_page(hash_val))->loadRecord(record);
				}
			}//right_rel rehashing is now complete

			Page* output_buffer = mem->mem_page((MEM_SIZE_IN_PAGE - 1));                            //create an output buffer page that points to the last page in                                                                                            memory
			for (unsigned int j = 0; j < partitions[i].num_left_rel_record; j++) {
				vector<unsigned int> left_rel = partitions[i].get_left_rel();
				unsigned int disk_page = left_rel[j];
				mem->loadFromDisk(disk, disk_page, (MEM_SIZE_IN_PAGE - 2));
				Page* input_buffer = mem->mem_page((MEM_SIZE_IN_PAGE - 2));                         //create input buffer page that points to second to last  mem page
				unsigned int num_records = input_buffer->size();
                //loop through left_rel records
				for (unsigned int r = 0; r < num_records; ++r) {
					Record leftRecord = input_buffer->get_record(r);                                // index of vector<Record> in page.cpp
					unsigned int hash_val = (leftRecord.probe_hash()) % (MEM_SIZE_IN_PAGE - 2);
                    
                    //Create a page that points to the matching rehashed left_rel page in memory
					Page* matching_page = mem->mem_page(hash_val);
					for (unsigned int s = 0; s < matching_page->size(); s++) {
                        Record rightRecord = matching_page->get_record(s);
						if (rightRecord == leftRecord) {                                            //compare recrds in matching page to current left record
							// WE HAVE A MATCH
							// check if output buffer is full
							if (output_buffer->full()) {
								unsigned int flushed_disk_page = mem->flushToDisk(disk, (MEM_SIZE_IN_PAGE - 1));
								result.push_back(flushed_disk_page);
							}

							// load to output buffer
							(mem->mem_page((MEM_SIZE_IN_PAGE - 1)))->loadPair(leftRecord, rightRecord);

						}
					}
				}
			}

		}

	}

    return result;

	// my understanding of probe:
	// vector of buckets, partitions. Go through this one at a time and look at a whole bucket of Ri. Re-hash these things with new hashing function
	// then take the matching bucket Si and hash each of these things with the new hashing function. only things that match to the same bucket could match
	// this is when you use == to compare and look for matches/joins


	// in the probing phase, one buffer input page, one output buffer page, B - 2 left, use last two as input/output

	// pull partition Ri and create a hash table of the tuples in Ri

	// check partition Si for matches, hash first though

	// use == to check if any of the relations' keys in the smaller relation match the key of the larger rel
}




// use smaller relation as outer (or left if equal)
