SELECT DISTINCT S1.SID, S1.Name
FROM Students S1, Students S2, Members M1, Members M2
WHERE S1.SID <> S2.SID AND M1.PID = M2.PID 
AND M1.SID = S1.SID AND M2.SID = S2.SID
AND S2.SID IN (SELECT DISTINCT E1.SID
			   FROM Enrollments E1, Enrollments E2, Enrollments E3, 
			   Courses C1, Courses C2, Courses C3
			   WHERE E1.CID = C1.CID AND E2.CID = C2.CID AND E3.CID = C3.CID
			   AND C1.C_Name = 'EECS280'
   			   AND (C2.C_Name = 'EECS484' OR C2.C_Name = 'EECS485')
			   AND (C3.C_Name = 'EECS482' OR C3.C_Name = 'EECS483')
			   AND E1.SID = E2.SID AND E2.SID = E3.SID)
ORDER BY S1.Name DESC;