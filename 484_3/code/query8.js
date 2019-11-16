// query 8: Find the city average friend count per user using MapReduce
// Using the same terminology in query6, we are asking you to write the mapper,
// reducer and finalizer to find the average friend count for each city.


var city_average_friendcount_mapper = function() {
  // implement the Map function of average friend count
  emit(this.hometown_city, {user_count: 1, friends: this.friends.length});
};

var city_average_friendcount_reducer = function(key, values) {
  // implement the reduce function of average friend count
  //Find the total number of users and friends in each city
  var val = {user_count: 0, friend_count: 0};
  for(i = 0; i < values.length; i++){
    val.user_count += values[i].user_count;
    val.friend_count += values[i].friends;
  }

  return val;
};

var city_average_friendcount_finalizer = function(key, reduceVal) {
  // We've implemented a simple forwarding finalize function. This implementation 
  // is naive: it just forwards the reduceVal to the output collection.
  // Feel free to change it if needed.
  return 1.0 * reduceVal.friend_count / reduceVal.user_count;
}
