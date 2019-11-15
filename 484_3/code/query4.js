
// query 4: find user pairs (A,B) that meet the following constraints:
// i) user A is male and user B is female
// ii) their Year_Of_Birth difference is less than year_diff
// iii) user A and B are not friends
// iv) user A and B are from the same hometown city
// The following is the schema for output pairs:
// [
//      [user_id1, user_id2],
//      [user_id1, user_id3],
//      [user_id4, user_id2],
//      ...
//  ]
// user_id is the field from the users collection. Do not use the _id field in users.
  
function suggest_friends(year_diff, dbname) {
    db = db.getSiblingDB(dbname);
    var pairs = [];
    // TODO: implement suggest friends
    
    //Find all users that are male loop through results.
    //In each loop find all female users who's YOB is within year_diff of the male user and has the same hometown
    //For each pair, find if theyre not friends (the smaller id is in the first position of flat_users and the larger id is in second position)
    //add array of users to final array 

    //indexOf returns -1 if not found
                // if(!db.flat_users.find({"user_id": Math.min(userA.user_id, userB.user_id), "friends": Math.max(userA.user_id, userB.user_id)}).hasNext())

    db.users.find({gender: "male"}).forEach(function(userA){
        db.users.find({gender: "female", "hometown.city": userA.hometown.city, YOB: {$lt: userA.YOB + year_diff, $gt: userA.YOB - year_diff}}).forEach(function(userB){
            if(userA.friends.indexOf(userB.user_id) == -1) 
            pairs.push([userA.user_id, userB.user_id]);
        })
    });

    
    // Return an array of arrays.
    return pairs;
}
