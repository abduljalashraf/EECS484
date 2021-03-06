// query1 : find users whose hometown citys the specified city. 

function find_user(city, dbname){
    db = db.getSiblingDB(dbname);
    var results = [];
    // TODO: return a Javascript array of user_ids. 
    // db.users.find(...);
    //Select distinct user id's where the user has a hometown and the hometown == city
    var cur = db.users.distinct("user_id", {hometown: {$exists : true}, "hometown.city" : city});
    cur.forEach(function(item){
        results.push(item)
    });
    // See test.js for a partial correctness check.  
    // The result will be an array of integers. The order does not matter.                                                               
    return results;
}
