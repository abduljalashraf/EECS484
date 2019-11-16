import java.io.FileWriter;
import java.io.IOException;
import java.sql.Connection;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.TreeSet;
import java.util.Vector;



//json.simple 1.1
// import org.json.simple.JSONObject;
// import org.json.simple.JSONArray;

// Alternate implementation of JSON modules.
import org.json.JSONObject;
import org.json.JSONArray;

public class GetData{
	
    static String prefix = "jiaqni.";
	
    // You must use the following variable as the JDBC connection
    Connection oracleConnection = null;
	
    // You must refer to the following variables for the corresponding 
    // tables in your database

    String cityTableName = null;
    String userTableName = null;
    String friendsTableName = null;
    String currentCityTableName = null;
    String hometownCityTableName = null;
    String programTableName = null;
    String educationTableName = null;
    String eventTableName = null;
    String participantTableName = null;
    String albumTableName = null;
    String photoTableName = null;
    String coverPhotoTableName = null;
    String tagTableName = null;

    // This is the data structure to store all users' information
    // DO NOT change the name
    JSONArray users_info = new JSONArray();		// declare a new JSONArray

	
    // DO NOT modify this constructor
    public GetData(String u, Connection c) {
	super();
	String dataType = u;
	oracleConnection = c;
	// You will use the following tables in your Java code
	cityTableName = prefix+dataType+"_CITIES";
	userTableName = prefix+dataType+"_USERS";
	friendsTableName = prefix+dataType+"_FRIENDS";
	currentCityTableName = prefix+dataType+"_USER_CURRENT_CITY";
	hometownCityTableName = prefix+dataType+"_USER_HOMETOWN_CITY";
	programTableName = prefix+dataType+"_PROGRAMS";
	educationTableName = prefix+dataType+"_EDUCATION";
	eventTableName = prefix+dataType+"_USER_EVENTS";
	albumTableName = prefix+dataType+"_ALBUMS";
	photoTableName = prefix+dataType+"_PHOTOS";
	tagTableName = prefix+dataType+"_TAGS";
    }
	
	
	
	
    //implement this function

    @SuppressWarnings("unchecked")
    public JSONArray toJSON() throws SQLException{ 

    	JSONArray users_info = new JSONArray();
		
		// Your implementation goes here....
    	

    	try (Statement stmt = oracleConnection.createStatement(ResultSet.TYPE_SCROLL_INSENSITIVE, ResultSet.CONCUR_READ_ONLY)) {
    		Statement stmt2 = oracleConnection.createStatement(ResultSet.TYPE_SCROLL_INSENSITIVE, ResultSet.CONCUR_READ_ONLY);
			Statement stmt3 = oracleConnection.createStatement(ResultSet.TYPE_SCROLL_INSENSITIVE, ResultSet.CONCUR_READ_ONLY);

			ResultSet rst = stmt.executeQuery(
    			"SELECT U.User_ID, U.First_Name, U.Last_Name, U.Gender, U.Year_of_Birth, U.Month_of_Birth, U.Day_of_Birth " +
    			"FROM " + userTableName + " U "
    		);

    		while (rst.next()) {

    			JSONObject temp_user = new JSONObject();

                temp_user.put("user_id", rst.getInt(1));
                temp_user.put("first_name", rst.getString(2));
                temp_user.put("last_name", rst.getString(3));
                temp_user.put("gender", rst.getString(4));
                temp_user.put("YEAR_OF_BIRTH", rst.getInt(5));
                temp_user.put("MONTH_OF_BIRTH", rst.getInt(6));
                temp_user.put("DAY_OF_BIRTH", rst.getInt(7));

    			// separate query for hometown/current:

    			ResultSet rst2 = stmt2.executeQuery(
    				"SELECT C.City_Name, C.State_Name, C.Country_Name " +
    				"FROM " + hometownCityTableName + " H, " + cityTableName + " C " +
    				"WHERE C.City_ID = H.Hometown_City_ID AND H.User_ID = " + rst.getInt(1)
                );
                
				JSONObject temp_obj = new JSONObject();
				if(!rst2.next()){
					temp_user.put("hometown", temp_obj);
				}
				else{
					rst2.next();
					temp_obj.put("city", rst2.getString(1));
					temp_obj.put("state", rst2.getString(2));
					temp_obj.put("Country", rst2.getString(3));
					temp_user.put("hometown", temp_obj);
				}
				
    			// add to temp object
    			// with result set, temp_user.put("user_id", "1")
    			// if doesn't exist, set as empty JSON object

    			ResultSet rst3 = stmt3.executeQuery(
    				"SELECT CT.City_Name, CT.State_Name, CT.Country_Name " +
    				"FROM " + currentCityTableName + " C, " + cityTableName + " CT " +
    				"WHERE CT.City_ID = C.Current_City_ID AND C.User_ID = " + rst.getInt(1)
    			);
    			// add to temp object
				// if doesn't exist, set as empty JSON object
				if(!rst3.next()){
					temp_user.put("current", temp_obj);
				}
				else{
					rst3.next();
					temp_obj.put("city", rst3.getString(1));
					temp_obj.put("state", rst3.getString(2));
					temp_obj.put("Country", rst3.getString(3));

					temp_user.put("current", temp_obj);
				}

    			// create a JSONarray for friends (only w/ greater IDs) and add that
    			rst2 = stmt2.executeQuery(
    				"SELECT F.USER2_ID " +
    				"FROM " + friendsTableName + " F " +
    				"WHERE F.USER1_ID = " + rst.getInt(1)
                );
                JSONArray friends = new JSONArray();
                while(rst2.next()){
                    friends.put(rst2.getInt(1));
                }

                temp_user.put("friends", friends);

    			// add temp_user to users_info and repeat
				users_info.put(temp_user);
				rst2.close();
				stmt2.close();
    		}

    		rst.close();
			stmt.close();
			
    	}
    	catch (SQLException e) {
    		System.err.println(e.getMessage());
    	}

		return users_info;
    }

    // This outputs to a file "output.json"
    public void writeJSON(JSONArray users_info) {
	// DO NOT MODIFY this function
	try {
	    FileWriter file = new FileWriter(System.getProperty("user.dir")+"/output.json");
	    file.write(users_info.toString());
	    file.flush();
	    file.close();

	} catch (IOException e) {
	    e.printStackTrace();
	}
		
    }
}
