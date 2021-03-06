
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
    //For each pair, find if theyre not friends (userA doesnt have userB in friends array and vice versa)
    //add array of users to final array 

    //flat_users contains all user/friend combos without arrays. The lower id is listed first.
    //Check that the min of userA and userB and the max of userA and userB are not corresponding 
    //entries in the flat_users table. This means that they are not friends

    //Find males who have an existing hometown
    var males = db.users.find({gender: "male", hometown: {$length: {$gt: 0}}});
    males.forEach(function(userA){
        //find females who have an existing hometown that is the same as the male, and a YOB within year_diff
        var female = db.users.find({gender: "female", hometown: {$length: {$gt: 0}}, 
        YOB: {$lt: userA.YOB + year_diff, $gt: userA.YOB - year_diff}});
        female.forEach(function(userB){
            //If user A is older than user B
            if(userA.user_id > userB.user_id && userA.hometown.city == userB.hometown.city){
                if(userB.friends){
                    //and user B has a friends array
                    var indA = userB.friends.indexOf(userA.user_id);
                    //if they arent friends
                    if(indA == -1){
                        pairs.push([userA.user_id, userB.user_id]);
                    }
                }
                else{
                    pairs.push([userA.user_id, userB.user_id]);
                }
            }
            else if(userA.user_id < userB.user_id && userA.hometown.city == userB.hometown.city){
                if(userA.friends){
                    var indB = userA.friends.indexOf(userB.user_id);
                    if(indB == -1){
                        pairs.push([userA.user_id, userB.user_id]);
                    }
                }
                else{
                    pairs.push([userA.user_id, userB.user_id]);
                }
            }
        })
    })
    
    // Return an array of arrays.
    return pairs;
}
