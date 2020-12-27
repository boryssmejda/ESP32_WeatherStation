# ======================= SENSORS_LOCATIONS =========================================

INSERT INTO Sensors_locations(sensor_location_name)
    VALUES ("Tuszyn");

INSERT INTO Sensors_locations(sensor_location_name)
    VALUES ("Andrespol");

INSERT INTO Sensors_locations(sensor_location_name)
    VALUES ("Bukowiec");

# ============================ SENSORS =================================================

INSERT INTO Sensors(sensor_name)
    VALUES ("BME280");

INSERT INTO Sensors(sensor_name)
    VALUES ("BH1750");

INSERT INTO Sensors(sensor_name)
    VALUES ("PMSA003");

INSERT INTO Sensors(sensor_name)
    VALUES("Li-ion 18650");

# ================================ MEASURED_QUANTITIES ====================================

INSERT INTO Measured_quantities(measured_quantity_name)
    VALUES ("Temperature");

INSERT INTO Measured_quantities(measured_quantity_name)
    VALUES ("Pressure");

INSERT INTO Measured_quantities(measured_quantity_name)
    VALUES ("Humidity");

INSERT INTO Measured_quantities(measured_quantity_name)
    VALUES ("Luminosity");

INSERT INTO Measured_quantities(measured_quantity_name)
    VALUES ("PM1.0");

INSERT INTO Measured_quantities(measured_quantity_name)
    VALUES ("PM2.5");

INSERT INTO Measured_quantities(measured_quantity_name)
    VALUES ("PM10");

INSERT INTO Measured_quantities(measured_quantity_name)
    VALUES ("Battery Voltage");


# ========================== MEASUREMENT_UNITS ===========================

INSERT INTO Measurement_units(unit_name)
    VALUES ("*C");

INSERT INTO Measurement_units(unit_name)
    VALUES ("hPa");

INSERT INTO Measurement_units(unit_name)
    VALUES ("%");

INSERT INTO Measurement_units(unit_name)
    VALUES ("lx");

INSERT INTO Measurement_units(unit_name)
    VALUES ("ug/m3");

INSERT INTO Measurement_units(unit_name)
    VALUES ("V");