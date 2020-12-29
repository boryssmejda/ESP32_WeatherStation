CREATE TABLE Sensors (
    sensor_id INT NOT NULL AUTO_INCREMENT,
    sensor_name VARCHAR(45) NOT NULL,

    PRIMARY KEY(sensor_id),
    UNIQUE(sensor_name)
);

CREATE TABLE Boards_locations (
    board_location_id INT NOT NULL AUTO_INCREMENT,
    board_location_name VARCHAR(100) NOT NULL,

    PRIMARY KEY(board_location_id),
    UNIQUE(board_location_name)
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
    reading_time TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
    sensor_id INT NOT NULL,
    measured_quantity_id INT NOT NULL,
    board_location_id INT NOT NULL,

    PRIMARY KEY(ID),

    FOREIGN KEY (measurement_unit_id)
        REFERENCES Measurement_units(measurement_unit_id),

    FOREIGN KEY (sensor_id)
        REFERENCES Sensors(sensor_id),

    FOREIGN KEY (measured_quantity_id)
        REFERENCES Measured_quantities(measured_quantity_id),

    FOREIGN KEY (board_location_id)
        REFERENCES Boards_locations(board_location_id)
);