CREATE TABLE Sensors (
    sensor_id INT NOT NULL AUTO_INCREMENT,
    sensor_name VARCHAR(45) NOT NULL,

    PRIMARY KEY(sensor_id),
    UNIQUE(sensor_name)
);

CREATE TABLE Sensors_locations (
    sensor_location_id INT NOT NULL AUTO_INCREMENT,
    sensor_location_name VARCHAR(100) NOT NULL,

    PRIMARY KEY(sensor_location_id),
    UNIQUE(sensor_location_name)
);

CREATE TABLE Measured_quantities (
    measured_quantity_id INT NOT NULL AUTO_INCREMENT,
    measured_quantity_name VARCHAR(45),

    PRIMARY KEY(measured_quantity_id),
    UNIQUE(measured_quantity_name)
);

CREATE TABLE Measurement_units (
    measurement_unit_id INT NOT NULL AUTO_INCREMENT,
    unit_name VARCHAR(45),

    PRIMARY KEY(measurement_unit_id),
    UNIQUE(unit_name)
);

CREATE TABLE Measurements (
    ID INT NOT NULL AUTO_INCREMENT,
    measured_value DOUBLE NOT NULL,
    measurement_unit_id INT NOT NULL,
    reading_time TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
    sensor_id INT NOT NULL,
    measured_quantity_id INT NOT NULL,
    sensor_location_id INT NOT NULL,

    PRIMARY KEY(ID),
    FOREIGN KEY (measurement_unit_id)   REFERENCES Measurement_units(measurement_unit_id),
    FOREIGN KEY (sensor_id)             REFERENCES Sensors(sensor_id),
    FOREIGN KEY (measured_quantity_id)  REFERENCES Measured_quantities(measured_quantity_id),
    FOREIGN KEY (sensor_location_id)    REFERENCES Sensors_locations(sensor_location_id)
);