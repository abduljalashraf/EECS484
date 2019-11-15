// query6 : Find the Average friend count per user for users
//
// Return a decimal variable as the average user friend count of all users
// in the users document.

function find_average_friendcount(dbname){
  db = db.getSiblingDB(dbname)
  // TODO: return a decimal number of average friend count
  print(db.flat_user.find().count());
  var total = 1.0 * db.flat_user.find().count() / db.users.find().count();
  
  return total;  

}
