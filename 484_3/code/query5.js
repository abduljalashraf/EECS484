// find the oldest friend for each user who has a friend. 
// For simplicity, use only year of birth to determine age, if there is a tie, use the one with smallest user_id
// return a javascript object : key is the user_id and the value is the oldest_friend id
// You may find query 2 and query 3 helpful. You can create selections if you want. Do not modify users collection.
//
//You should return something like this:(order does not matter)
//{user1:userx1, user2:userx2, user3:userx3,...}

function oldest_friend(dbname){
  db = db.getSiblingDB(dbname);
  var results = {};
  // TODO: implement oldest friends
  // return an javascript object described above
  db.friends.drop();
  //Make table of all friends by flipping the flat_users table and combining it with the original flat_users table
  // db.createCollection("friends", {capped: true, size: 2 * db.flat_users.find().count()});
	db.flat_users.find().forEach(function(d) {
    db.friends.insert([{"user1":d.user_id, "user2":d.friends}, {"user1":d.friends, "user2":d.user_id}]);
  });
  
  //put birth years into an array using user_id as the index
  var yob = [];
	db.users.find().forEach(function(d) {yob[d.user_id] = d.YOB;});

  
  db.friends.aggregate([{$group: {_id: "$user1", friend: {$push: "$user2"}}}]).forEach(function(user){
    //init values for _id, year of birth of the first friend, and the first friend
    var uid = user._id;
    var maxYear = yob[user.friend[0]];
    var oldestFriend = user.friend[0];
    //loop through friends and check if friend[i]'s age is larger than the current friend
    //if it is larger, update the max friend. Older friend will have a smaller year
    for(i = 0; i < user.friend.length; i++){
      //if next friend is older than current friend, update the year and which friend is the oldest
      if(maxYear < yob[user.friend[i]]){
        maxYear = yob[user.friend[i]];
        oldestFriend = user.friend[i];
      }
      else if(maxYear == yob[user.friend[i]]){
        //if next friend is the same age, take lowest user_id
        oldestFriend = Math.min(oldestFriend, user.friend[i]);
      }
    }

    results[uid] = oldestFriend;
  })
  return results
}
