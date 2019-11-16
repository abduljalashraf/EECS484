var l1 = load('query1.js')
var l2 = load('query2.js')
var l3 = load('query3.js')
var l4 = load('query4.js')
var l5 = load('query5.js')
var l6 = load('query6.js')
var l7 = load('query7.js')
var l8 = load('query8.js')

// fill in your database name
// Your dbname is your uniqname
var dbname = 'justanj';

// test query1
var test1 = find_user('Bucklebury',dbname);
var ans1 = test1.length;
if(ans1 === 56){
  print("test1 correct!")
} else {
	print("test1 incorrect!")
	print("result1: ", ans1)
}


// test query2
unwind_friends(dbname)
var ans2 = db.flat_users.find().count();
if(ans2 === 21378){
  print("test2 correct!")
} else {
	print("test2 incorrect!")
	print("result2: ", ans2)
}


// test query3
cities_table(dbname)
var ans3 = db.cities.find({"_id" : "Bucklebury"}).next().users.length;
if(ans3 === 56){
  print("test3 correct!")
} else {
	print("test3 incorrect!")
	print("result3: ", ans3)
}



// test query4
var test4 = suggest_friends(5,dbname);
var ans4 = test4.length;
if (ans4 === 90){
	print("test4 correct!")
} else {
	print("test4 incorrect!")
	print("result4: ", ans4)
}


// test query5
var test5 = oldest_friend(dbname);
if(Object.keys(test5).length === 799){
 if(test5.hasOwnProperty(517)){
	var ans5 = test5[517];
    if(test5[517] == 426){
      print("test5 correct!")
    } else {
			print("test5 incorrect!")
			print("result5: ", ans5)
		}
 	} else {
		print("test5 incorrect!")
	}
} else {
	print("test5 incorrect!")
	print("key length: ", Object.keys(test5).length)
}

// test query6
var test6 = find_average_friendcount(dbname);
if (test6 > 26 & test6 < 27) {
  print("test6 correct!")
} else {
	print("test6 incorrect!")
	print("result6: ", test6)
}

// Test query 7 with map reduce
var result7 = db.users.mapReduce(
  num_month_mapper,
  num_month_reducer,
  {
    out: "born_each_month",
		finalize: num_month_finalizer
  }
);
var ans7 = db.born_each_month.count();
if (ans7 == 12) {
  print("test7 correct!")
} else {
	print("test7 incorrect!")
	print("result7: ", ans7)
}

// test query 8
// run the mapreduce function with function objects
var result8 = db.users.mapReduce(
  city_average_friendcount_mapper,
  city_average_friendcount_reducer,
  {
    out: "friend_city_population",
		finalize: city_average_friendcount_finalizer
  }
);
var ans8 = db.friend_city_population.count();
if (ans8 == 16) {
  print("test8 correct!")
} else {
	print("test8 incorrect!")
	print("result8: ", ans8)
}

