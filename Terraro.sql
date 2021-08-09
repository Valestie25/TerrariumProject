CREATE DATABASE IF NOT EXISTS terrarium;

CREATE TABLE Terrarium (
	Id_Terrarium INT NOT NULL,   
    Terrarium_Num INT NOT NULL, 
    Num_Iguanas INT NOT NULL,
    PRIMARY KEY (Id_Terrarium)
    );

INSERT INTO Terrarium(Id_Terrarium, Terrarium_num, Num_Iguanas)
values (1, 1, 5);


SELECT * FROM Terrarium;
DROP TABLE Terrarium;

INSERT INTO Terrarium(Id_Terrarium, Num_Iguanas)
VALUES(2, 5);

CREATE TABLE Values_R(
	Id INT NOT NULL AUTO_INCREMENT, 
    Temperature DOUBLE,
    Humidity INT,
    Air_Quality INT,
    Last_Update TIMESTAMP NOT NULL DEFAULT now(),
    PRIMARY KEY(Id)
    );
    
    INSERT INTO Values_R(Id, Num_Terrarium, Temperature, Humidity, Air_Quality)
VALUES(1, 1, 53,33,429); 

INSERT INTO Values_R(Num_Terrarium, Temperature, Humidity, Air_Quality)
VALUES(1,52,30,400); 

INSERT INTO Values_R(Num_Terrarium, Temperature, Humidity, Air_Quality)
VALUES(2,52,30,400); 
    
DROP TABLE Values_R;
SELECT * FROM Values_R;