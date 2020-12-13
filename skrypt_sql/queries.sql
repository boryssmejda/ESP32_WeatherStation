SELECT measured_value, reading_time, measured_quantity_id, measurement_unit_id 
FROM Measurements
WHERE measured_quantity_id=(
        SELECT measured_quantity_id FROM Measured_quantities WHERE measured_quantity_name = 'Temperature'
    );

// Get All Temperature measurements with the measurement units
SELECT m.measured_value, m.reading_time, mu.unit_name
FROM Measurements m
LEFT JOIN Measurement_units mu ON mu.measurement_unit_id = m.measurement_unit_id
LEFT JOIN Measured_quantities mq ON mq.measured_quantity_id = m.measured_quantity_id
WHERE mq.measured_quantity_name = 'Temperature'