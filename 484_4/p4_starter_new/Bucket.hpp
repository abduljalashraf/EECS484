#ifndef _BUCKET_HPP_
#define _BUCKET_HPP_

#include "Page.hpp"
#include "Disk.hpp"

class Bucket {
    
public:

    Bucket(Disk *disk);
    // add the disk page id of left relation into this bucket/partition
    void add_left_rel_page(unsigned int page_id);
    // add the disk page id of right relation into this bucket/partition
    void add_right_rel_page(unsigned int page_id);
    // return a list of page ids in disk from left relation in this bucket/partition
    vector<unsigned int> get_left_rel();
    // return a list of page ids in disk from right relation in this bucket/partition
    vector<unsigned int> get_right_rel();

    // number of record for left relation in this bucket
    // This is maintained by add_left_rel_page() function
    unsigned int num_left_rel_record = 0;
    // number of record from right relation in this bucket
    // This is maintained by add_right_rel_page() function
    unsigned int num_right_rel_record = 0;

private:
	// list of disk page ids contain the records from left relation in this bucket
    vector<unsigned int> left_rel;
    // list of disk page ids contain the records from right relation in this bucket
    vector<unsigned int> right_rel;
    // Pointer to the disk
    Disk *disk;
};


#endif