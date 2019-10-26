SELECT DISTINCT A.Author_ID, A.First_Name, A.Last_Name
FROM Authors A
WHERE NOT EXISTS (
	SELECT DISTINCT B1.Subject_ID
	FROM Authors Y, Books B1
	WHERE Y.First_Name = "J. K." AND Y.Last_Name = "Rowling" AND B1.Author_ID = Y.Author_ID
	EXCEPT
	SELECT DISTINCT B2.Subject_ID
	FROM Authors X, Books B2
	WHERE X.Author_ID = A.Author_ID AND B2.Author_ID = A.Author_ID
);



