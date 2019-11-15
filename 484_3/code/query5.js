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


  // if((user2.friends.indexOf(user1.user_id) != -1) && user2.age > max)
  // max = user2.age;
  // results.user1 


  //Get all friends of user1
  var friends = [];
  var max = 0;
  db.users.aggregate([
    {$unwind: "$friends"},
    {$project: {
      _id: 0,
      user_id: 1,
      friends: 1,
    }},
    {$group: {
      _id: "$user_id",
      friends: {$push: {user_id:"$friends"}}}},
  ]);



  // friends.forEach(function(friend){
  //   if(friend.age > max){
  //     max = friend.age;
  //   }
  // })

  return results
}
