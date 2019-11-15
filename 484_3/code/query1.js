// query1 : find users whose hometown citys the specified city. 

function find_user(city, dbname){
    db = db.getSiblingDB(dbname);
    var results = [];
    // TODO: return a Javascript array of user_ids. 
    // db.users.find(...);
    var cur = db.users.find({hometown: {$exists : true}, "hometown.city" : city}, {user_id: 1, _id: 0});
    printjson(results[1]);
    results = cur.toArray();
    // See test.js for a partial correctness check.  
    // The result will be an array of integers. The order does not matter.                                                               
    return results;
}
