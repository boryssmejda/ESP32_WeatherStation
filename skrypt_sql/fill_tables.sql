# ======================= SENSORS_LOCATIONS =========================================

INSERT INTO Sensors_locations(sensor_location_name)
    VALUES ("Garden");

INSERT INTO Sensors_locations(sensor_location_name)
    VALUES ("Office");


# ============================ SENSORS =================================================

INSERT INTO Sensors(sensor_name)
    VALUES ("BME280");

INSERT INTO Sensors(sensor_name)
    VALUES ("BH1750");

INSERT INTO Sensors(sensor_name)
    VALUES ("PMSA003");

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
