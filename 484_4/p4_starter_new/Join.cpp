#include "Join.hpp"
#include <functional>
#include <iostream>

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
            
            //get left rel first and use the size of that vector to loop through items in each bucket
            vector<unsigned int> left_rel = partitions[i].get_left_rel();
			for (unsigned int j = 0; j < left_rel.size(); j++) {
				unsigned int disk_page = left_rel[j];                                           //Find page on disk where left_rel is
				mem->loadFromDisk(disk, disk_page, (MEM_SIZE_IN_PAGE - 2));
				Page* input_buffer = mem->mem_page((MEM_SIZE_IN_PAGE - 2));                     //load left_rel into input buffer page from memory
				unsigned int num_records = input_buffer->size();
				for (unsigned int r = 0; r < num_records; ++r) {                                //Loop through left_rel records in input buffer
					Record record = input_buffer->get_record(r);                                // index of vector<Record> in page.cpp
					unsigned int hash_val = (record.probe_hash()) % (MEM_SIZE_IN_PAGE - 2);     //re-hash the record value and get a new index

					// loadRecord into memory at the the new hashed value
					(mem->mem_page(hash_val))->loadRecord(record);
                    
				}
			} //left_rel rehashing done
            
			Page* output_buffer = mem->mem_page((MEM_SIZE_IN_PAGE - 1));                        //creates output buffer page that points to last page in memory
            vector<unsigned int> right_rel = partitions[i].get_right_rel();
            for (unsigned int j = 0; j < right_rel.size(); j++) {
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
						if (leftRecord == rightRecord) { 
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
            //if output buffer is not empty flush the results
               if(!(output_buffer->size() > 0)){
                   unsigned int flushed_disk_page = mem->flushToDisk(disk, (MEM_SIZE_IN_PAGE - 1));
                   result.push_back(flushed_disk_page);
               }

		}//if left



		if (outer_rel == "right") {

			// loop through the left_rel items in each bucket
            vector<unsigned int> right_rel = partitions[i].get_right_rel();
			for (unsigned int j = 0; j < right_rel.size(); j++) {
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
			}//right_rel rehashing is done

			Page* output_buffer = mem->mem_page((MEM_SIZE_IN_PAGE - 1));                            //create an output buffer page that points to the last page in
            vector<unsigned int> left_rel = partitions[i].get_left_rel();
			for (unsigned int j = 0; j < left_rel.size(); j++) {
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
						if (rightRecord == leftRecord) {                                            //compare reocrds in matching page to current left record
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
            //if output buffer is not empty flush the results
               if(!(output_buffer->size() > 0)){
                   unsigned int flushed_disk_page = mem->flushToDisk(disk, (MEM_SIZE_IN_PAGE - 1));
                   result.push_back(flushed_disk_page);
               }
		}//end if  
	}
    return result;
}