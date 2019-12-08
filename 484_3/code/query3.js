//query3
//create a collection "cities" to store every user that lives in every city
//Each document(city) has following schema:
/*
{
  _id: city
  users:[userids]
}
*/

function cities_table(dbname) {
    db = db.getSiblingDB(dbname);
    // TODO: implemente cities collection here
    //Use group to group by city name and push to insert users that have that city
    db.users.aggregate([{
      $group: {
        _id: "$hometown.city",
        users: {$push: {user_id:"$user_id"}}}},
      {$out: "cities"}
    ]);

    // Returns nothing. Instead, it creates a collection inside the datbase.

}
